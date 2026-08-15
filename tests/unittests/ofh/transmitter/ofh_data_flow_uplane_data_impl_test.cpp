// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/transmitter/ofh_data_flow_uplane_data_impl.h"
#include "../../phy/support/resource_grid_test_doubles.h"
#include "../compression/ofh_iq_compressor_test_doubles.h"
#include "../ecpri/ecpri_packet_builder_test_doubles.h"
#include "../ethernet/vlan_ethernet_frame_builder_test_doubles.h"
#include "ocudu/adt/interval.h"
#include "ocudu/ofh/ethernet/ethernet_frame_pool.h"
#include "ocudu/phy/support/prach_buffer.h"
#include "ocudu/phy/support/resource_grid_context.h"
#include "ocudu/ran/resource_block.h"
#include <gtest/gtest.h>
#include <vector>

using namespace ocudu;
using namespace ofh;

namespace {

/// Spy OFH User-Plane packet builder.
class ofh_uplane_packet_builder_spy : public uplane_message_builder
{
  static_vector<uplane_message_params, MAX_NSYMB_PER_SLOT> uplane_msg_params;
  std::vector<std::vector<cbf16_t>>                        iq_data;

public:
  ofh_uplane_packet_builder_spy()
  {
    iq_data.resize(MAX_NSYMB_PER_SLOT);
    for (auto& iq_symbol : iq_data) {
      iq_symbol.resize(MAX_NOF_SUBCARRIERS);
    }
  }

  units::bytes get_header_size(const ru_compression_params& params) const override { return units::bytes(8); }

  unsigned build_message(span<uint8_t> buffer, span<const cbf16_t> grid, const uplane_message_params& params) override
  {
    std::copy(grid.begin(), grid.end(), iq_data[params.symbol_id].begin() + params.start_prb * NOF_SUBCARRIERS_PER_RB);
    uplane_msg_params.push_back(params);

    return 0;
  }

  /// Returns the number of built packets.
  unsigned nof_built_packets() const { return uplane_msg_params.size(); }

  /// Retuns a span of the User-Plane message parameters processed by this builder.
  span<const uplane_message_params> get_uplane_params() const { return uplane_msg_params; }

  /// Returns a pointer to the resource grid reader processed by this builder.
  span<const cbf16_t> get_iq_data(unsigned symbol) const { return iq_data[symbol]; }
};

/// Minimal PRACH buffer double returning a fixed, distinguishable preamble sequence for every symbol.
class prach_buffer_double : public prach_buffer
{
  std::vector<cbf16_t> data;

public:
  explicit prach_buffer_double(unsigned sequence_length) : data(sequence_length)
  {
    // A non-zero ramp so the test can tell the preamble samples apart from the frequency-domain guard.
    for (unsigned i = 0; i != sequence_length; ++i) {
      data[i] = to_cbf16(cf_t(static_cast<float>(i + 1), 0.0F));
    }
  }

  unsigned            get_max_nof_ports() const override { return 1; }
  unsigned            get_max_nof_td_occasions() const override { return 1; }
  unsigned            get_max_nof_fd_occasions() const override { return 1; }
  unsigned            get_max_nof_symbols() const override { return MAX_NSYMB_PER_SLOT; }
  unsigned            get_sequence_length() const override { return data.size(); }
  span<cbf16_t>       get_symbol(unsigned, unsigned, unsigned, unsigned) override { return data; }
  span<const cbf16_t> get_symbol(unsigned, unsigned, unsigned, unsigned) const override { return data; }
};

} // namespace

class ofh_data_flow_uplane_data_impl_fixture : public ::testing::TestWithParam<ru_compression_params>
{
protected:
  const unsigned                          nof_symbols;
  const unsigned                          ru_nof_prbs;
  const unsigned                          du_nof_prbs;
  const ether::vlan_frame_params          vlan_params  = {{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11},
                                                          {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x22},
                                                          ether::vlan_parameters{.tci_vid = 1, .tci_pcp = 7},
                                                          0xaabb};
  const ru_compression_params             compr_params = GetParam();
  data_flow_uplane_data_impl              data_flow;
  ether::testing::vlan_frame_builder_spy* vlan_builder;
  ecpri::testing::packet_builder_spy*     ecpri_builder;
  ofh_uplane_packet_builder_spy*          uplane_builder;
  resource_grid_reader_spy                rg_reader_spy;
  resource_grid_writer_spy                rg_writer_spy;
  resource_grid_spy                       rg_spy;
  shared_resource_grid_spy                shared_rg_spy;

  ofh_data_flow_uplane_data_impl_fixture() :
    nof_symbols(3),
    ru_nof_prbs(273),
    du_nof_prbs(273),
    data_flow(get_config(), generate_data_flow_dependencies()),
    rg_reader_spy(1, nof_symbols, du_nof_prbs),
    rg_spy(rg_reader_spy, rg_writer_spy),
    shared_rg_spy(rg_spy)
  {
    initialize_grid_reader();
  }

  data_flow_uplane_data_impl_config get_config()
  {
    data_flow_uplane_data_impl_config config;
    config.ru_nof_prbs  = ru_nof_prbs;
    config.compr_params = compr_params;

    return config;
  }

  data_flow_uplane_data_impl_dependencies generate_data_flow_dependencies()
  {
    data_flow_uplane_data_impl_dependencies dependencies;
    dependencies.logger         = &ocudulog::fetch_basic_logger("TEST");
    dependencies.compressor_sel = std::make_unique<ofh::testing::iq_compressor_dummy>();
    dependencies.frame_pool     = std::make_shared<ether::eth_frame_pool>(
        *dependencies.logger, units::bytes(9000), 2, ofh::message_type::user_plane, ofh::data_direction::downlink);

    {
      auto temp               = std::make_unique<ofh_uplane_packet_builder_spy>();
      uplane_builder          = temp.get();
      dependencies.up_builder = std::move(temp);
    }
    {
      auto temp                = std::make_unique<ether::testing::vlan_frame_builder_spy>(vlan_params);
      vlan_builder             = temp.get();
      dependencies.eth_builder = std::move(temp);
    }
    {
      auto temp                  = std::make_unique<ecpri::testing::packet_builder_spy>();
      ecpri_builder              = temp.get();
      dependencies.ecpri_builder = std::move(temp);
    }

    return dependencies;
  }

  void initialize_grid_reader()
  {
    for (uint8_t symbol = 0; symbol != nof_symbols; ++symbol) {
      for (uint16_t k = 0, e = du_nof_prbs * NOF_SUBCARRIERS_PER_RB; k != e; ++k) {
        rg_reader_spy.write(
            resource_grid_reader_spy::expected_entry_t{0, symbol, k, (k > 200) ? cf_t{1, 1} : cf_t{1, 0}});
      }
    }
  }
};

static const std::array<ru_compression_params, 2> compr_params = {
    {{compression_type::none, 16}, {compression_type::BFP, 9}}};
static const std::array<std::vector<interval<unsigned>>, 2> segmented_prbs = {{{{0, 186}, {186, 273}}, {{0, 273}}}};

INSTANTIATE_TEST_SUITE_P(compression_params, ofh_data_flow_uplane_data_impl_fixture, ::testing::ValuesIn(compr_params));

TEST_P(ofh_data_flow_uplane_data_impl_fixture, calling_enqueue_section_type_1_message_success)
{
  data_flow_uplane_resource_grid_context context;
  context.port         = 0;
  context.sector       = 0;
  context.slot         = slot_point(0, 0, 0);
  context.eaxc         = 2;
  context.symbol_range = {0, 3};

  data_flow.enqueue_section_type_1_message(context, shared_rg_spy.get_grid());

  // Assert VLAN parameters.
  const ether::vlan_frame_params& vlan = vlan_builder->get_vlan_frame_params();
  ASSERT_TRUE(vlan_builder->has_build_vlan_frame_method_been_called());
  ASSERT_EQ(vlan_params.eth_type, vlan.eth_type);
  ASSERT_EQ(vlan_params.mac_dst_address, vlan.mac_dst_address);
  ASSERT_EQ(vlan_params.mac_src_address, vlan.mac_src_address);
  ASSERT_EQ(vlan_params.vlan_config->tci_vid, vlan.vlan_config->tci_vid);
  ASSERT_EQ(vlan_params.vlan_config->tci_pcp, vlan.vlan_config->tci_pcp);

  // Assert eCPRI parameters.
  ASSERT_TRUE(ecpri_builder->has_build_data_packet_method_been_called());
  ASSERT_FALSE(ecpri_builder->has_build_control_packet_method_been_called());
  // Assert there is only one packet per symbol.
  span<const ecpri::iq_data_parameters> data_params = ecpri_builder->get_data_parameters();
  ASSERT_EQ(data_params.size(), nof_symbols * segmented_prbs[static_cast<unsigned>(compr_params.type)].size());
  sequence_identifier_generator generator;
  for (const auto& param : data_params) {
    ASSERT_EQ(param.seq_id >> 8U, generator.generate(context.eaxc));
    ASSERT_EQ(param.pc_id, context.eaxc);
  }

  // Assert Open Fronthaul parameters.
  span<const uplane_message_params>      uplane_params = uplane_builder->get_uplane_params();
  const std::vector<interval<unsigned>>& seg_prbs      = segmented_prbs[static_cast<unsigned>(compr_params.type)];
  ASSERT_EQ(uplane_builder->nof_built_packets(), nof_symbols * seg_prbs.size());

  unsigned symbol_id = 0;
  unsigned prb_index = 0;
  for (const auto& param : uplane_params) {
    ASSERT_EQ(param.direction, data_direction::downlink);
    ASSERT_EQ(param.slot, context.slot);
    ASSERT_EQ(param.filter_index, filter_index_type::standard_channel_filter);
    ASSERT_EQ(param.start_prb, seg_prbs[prb_index].start());
    ASSERT_EQ(param.nof_prb, seg_prbs[prb_index].length());
    ASSERT_EQ(param.symbol_id, symbol_id);
    ASSERT_EQ(param.sect_type, section_type::type_1);
    ASSERT_EQ(param.compression_params.data_width, compr_params.data_width);
    ASSERT_EQ(param.compression_params.type, compr_params.type);

    // Check the symbols.
    std::vector<cbf16_t> iq_symbols(param.nof_prb * NOF_SUBCARRIERS_PER_RB, 0);
    rg_reader_spy.get(iq_symbols, 0, symbol_id, param.start_prb * NOF_SUBCARRIERS_PER_RB);

    ASSERT_EQ(span<const cbf16_t>(iq_symbols),
              uplane_builder->get_iq_data(symbol_id).subspan(param.start_prb * NOF_SUBCARRIERS_PER_RB,
                                                             param.nof_prb * NOF_SUBCARRIERS_PER_RB));

    // Increase the segmented PRB index.
    prb_index = (prb_index + 1) % seg_prbs.size();
    // Increase symbol index when all the PRBs for a symbol have been checked.
    if (prb_index == 0) {
      ++symbol_id;
    }
  }
}

TEST(ofh_data_flow_uplane_data_impl, frame_buffer_size_of_nof_prbs_plus_headers_size_generates_one_packet_per_symbol)
{
  data_flow_uplane_resource_grid_context context;
  context.port         = 0;
  context.sector       = 0;
  context.slot         = slot_point(0, 0, 0);
  context.eaxc         = 2;
  context.symbol_range = {0, 3};

  data_flow_uplane_data_impl_config config;

  config.ru_nof_prbs  = 273;
  config.compr_params = {compression_type::BFP, 9};

  ether::vlan_frame_params vlan_params = {{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11},
                                          {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x22},
                                          ether::vlan_parameters{.tci_vid = 1},
                                          0xaabb};

  data_flow_uplane_data_impl_dependencies dependencies;
  dependencies.logger         = &ocudulog::fetch_basic_logger("TEST");
  dependencies.compressor_sel = std::make_unique<ofh::testing::iq_compressor_dummy>();

  unsigned prb_size   = 28;
  unsigned frame_size = prb_size * config.ru_nof_prbs;

  ofh_uplane_packet_builder_spy* uplane_builder;

  {
    auto temp = std::make_unique<ofh_uplane_packet_builder_spy>();
    frame_size += temp->get_header_size(config.compr_params).value();
    uplane_builder          = temp.get();
    dependencies.up_builder = std::move(temp);
  }
  {
    auto temp = std::make_unique<ether::testing::vlan_frame_builder_spy>(vlan_params);
    frame_size += temp->get_header_size().value();
    dependencies.eth_builder = std::move(temp);
  }
  {
    auto temp = std::make_unique<ecpri::testing::packet_builder_spy>();
    frame_size += temp->get_header_size(ecpri::message_type::iq_data).value();
    dependencies.ecpri_builder = std::move(temp);
  }

  dependencies.frame_pool = std::make_shared<ether::eth_frame_pool>(
      *dependencies.logger, units::bytes(frame_size), 2, ofh::message_type::user_plane, ofh::data_direction::downlink);

  resource_grid_reader_spy rg_reader_spy(1, context.symbol_range.length(), config.ru_nof_prbs);
  for (uint8_t symbol = 0; symbol != context.symbol_range.length(); ++symbol) {
    for (uint16_t k = 0, e = config.ru_nof_prbs * NOF_SUBCARRIERS_PER_RB; k != e; ++k) {
      rg_reader_spy.write(
          resource_grid_reader_spy::expected_entry_t{0, symbol, k, (k > 200) ? cf_t{1, 1} : cf_t{1, 0}});
    }
  }
  resource_grid_writer_spy rg_writer_spy;
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy shared_rg_spy(rg_spy);

  data_flow_uplane_data_impl data_flow(config, std::move(dependencies));
  data_flow.enqueue_section_type_1_message(context, shared_rg_spy.get_grid());

  // Assert number of packets.
  ASSERT_EQ(uplane_builder->nof_built_packets(), context.symbol_range.length());
}

TEST(ofh_data_flow_uplane_data_impl, frame_buffer_size_of_nof_prbs_generates_two_packets_per_symbol)
{
  data_flow_uplane_resource_grid_context context;
  context.port         = 0;
  context.sector       = 0;
  context.slot         = slot_point(0, 0, 0);
  context.eaxc         = 2;
  context.symbol_range = {0, 3};

  data_flow_uplane_data_impl_config config;

  config.ru_nof_prbs  = 273;
  config.compr_params = {compression_type::BFP, 9};

  ether::vlan_frame_params vlan_params = {{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11},
                                          {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x22},
                                          ether::vlan_parameters{.tci_vid = 1},
                                          0xaabb};

  data_flow_uplane_data_impl_dependencies dependencies;
  dependencies.logger         = &ocudulog::fetch_basic_logger("TEST");
  dependencies.compressor_sel = std::make_unique<ofh::testing::iq_compressor_dummy>();

  unsigned prb_size  = 28;
  unsigned prbs_size = prb_size * config.ru_nof_prbs;

  dependencies.frame_pool = std::make_shared<ether::eth_frame_pool>(
      *dependencies.logger, units::bytes(prbs_size), 2, message_type::user_plane, data_direction::downlink);
  ofh_uplane_packet_builder_spy* uplane_builder;

  {
    auto temp               = std::make_unique<ofh_uplane_packet_builder_spy>();
    uplane_builder          = temp.get();
    dependencies.up_builder = std::move(temp);
  }
  {
    auto temp                = std::make_unique<ether::testing::vlan_frame_builder_spy>(vlan_params);
    dependencies.eth_builder = std::move(temp);
  }
  {
    auto temp                  = std::make_unique<ecpri::testing::packet_builder_spy>();
    dependencies.ecpri_builder = std::move(temp);
  }
  resource_grid_reader_spy rg_reader_spy(1, context.symbol_range.length(), config.ru_nof_prbs);
  for (uint8_t symbol = 0; symbol != context.symbol_range.length(); ++symbol) {
    for (uint16_t k = 0, e = config.ru_nof_prbs * NOF_SUBCARRIERS_PER_RB; k != e; ++k) {
      rg_reader_spy.write(
          resource_grid_reader_spy::expected_entry_t{0, symbol, k, (k > 200) ? cf_t{1, 1} : cf_t{1, 0}});
    }
  }
  resource_grid_writer_spy rg_writer_spy;
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy shared_rg_spy(rg_spy);

  data_flow_uplane_data_impl data_flow(config, std::move(dependencies));
  data_flow.enqueue_section_type_1_message(context, shared_rg_spy.get_grid());

  // Assert number of packets. As the packet should not fit in the frame, check that it generated 2 packets per symbol.
  ASSERT_EQ(uplane_builder->nof_built_packets(), context.symbol_range.length() * 2);
}

TEST(ofh_data_flow_uplane_data_impl, enqueue_prach_message_builds_uplink_prach_uplane_messages)
{
  data_flow_uplane_data_impl_config config;
  config.ru_nof_prbs  = 273;
  config.compr_params = {compression_type::BFP, 9};
  // An RU transmitter builds uplink messages.
  config.direction = data_direction::uplink;

  ether::vlan_frame_params vlan_params = {{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11},
                                          {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x22},
                                          ether::vlan_parameters{.tci_vid = 1},
                                          0xaabb};

  data_flow_uplane_data_impl_dependencies dependencies;
  dependencies.logger         = &ocudulog::fetch_basic_logger("TEST");
  dependencies.compressor_sel = std::make_unique<ofh::testing::iq_compressor_dummy>();
  // PRACH frames go in a frame-pool space of their own (uplane_prach).
  dependencies.frame_pool = std::make_shared<ether::eth_frame_pool>(
      *dependencies.logger, units::bytes(9000), 2, message_type::uplane_prach, data_direction::uplink);

  ofh_uplane_packet_builder_spy* uplane_builder;
  {
    auto temp               = std::make_unique<ofh_uplane_packet_builder_spy>();
    uplane_builder          = temp.get();
    dependencies.up_builder = std::move(temp);
  }
  dependencies.eth_builder   = std::make_unique<ether::testing::vlan_frame_builder_spy>(vlan_params);
  dependencies.ecpri_builder = std::make_unique<ecpri::testing::packet_builder_spy>();

  data_flow_uplane_data_impl data_flow(config, std::move(dependencies));

  // 12-PRB PRACH section, short preamble of 139 samples spanning 2 symbols.
  prach_buffer_double prach(139);

  data_flow_uplane_prach_context context;
  context.slot         = slot_point(0, 0, 0);
  context.sector       = 0;
  context.port         = 0;
  context.eaxc         = 4;
  context.prb_start    = 0;
  context.nof_prb      = 12;
  context.start_symbol = 2;
  context.nof_symbols  = 2;
  context.filter_index = filter_index_type::ul_prach_preamble_short;
  context.prach_scs    = prach_subcarrier_spacing::kHz15;

  data_flow.enqueue_prach_message(context, prach);

  // One PRACH User-Plane message per PRACH symbol.
  ASSERT_EQ(uplane_builder->nof_built_packets(), context.nof_symbols);

  span<const uplane_message_params> uplane_params = uplane_builder->get_uplane_params();
  unsigned                          symbol_id     = context.start_symbol;
  for (const auto& param : uplane_params) {
    ASSERT_EQ(param.direction, data_direction::uplink);
    ASSERT_EQ(param.filter_index, filter_index_type::ul_prach_preamble_short);
    ASSERT_EQ(param.slot, context.slot);
    ASSERT_EQ(param.start_prb, context.prb_start);
    ASSERT_EQ(param.nof_prb, context.nof_prb);
    ASSERT_EQ(param.symbol_id, symbol_id);
    ASSERT_EQ(param.sect_type, section_type::type_1);
    ++symbol_id;
  }

  // The short PRACH preamble is placed within the PRB section with a 2-RE frequency-domain guard at the bottom: REs 0
  // and 1 are zero, the 139-sample preamble occupies REs [2, 141), and the rest of the 12-PRB section is zero.
  constexpr unsigned prach_re_offset = 2;
  for (unsigned symbol = context.start_symbol, end = context.start_symbol + context.nof_symbols; symbol != end;
       ++symbol) {
    span<const cbf16_t> iq = uplane_builder->get_iq_data(symbol);
    ASSERT_EQ(to_cf(iq[0]), cf_t(0.0F, 0.0F));
    ASSERT_EQ(to_cf(iq[1]), cf_t(0.0F, 0.0F));
    // First and last preamble samples land at REs 2 and 2 + 138.
    ASSERT_EQ(to_cf(iq[prach_re_offset]), cf_t(1.0F, 0.0F));
    ASSERT_EQ(to_cf(iq[prach_re_offset + 138]), cf_t(139.0F, 0.0F));
    // The remainder of the 12-PRB (144-RE) section is guard.
    ASSERT_EQ(to_cf(iq[prach_re_offset + 139]), cf_t(0.0F, 0.0F));
  }
}

TEST(ofh_data_flow_uplane_data_impl, long_prach_uses_frequency_mapping_offset)
{
  struct test_case {
    subcarrier_spacing pusch_scs;
    unsigned           expected_offset;
  };
  const std::array<test_case, 2> cases = {{{subcarrier_spacing::kHz15, 7}, {subcarrier_spacing::kHz30, 1}}};

  for (const test_case& test : cases) {
    data_flow_uplane_data_impl_config config = {};
    config.sector                            = 0;
    config.cp                                = cyclic_prefix::NORMAL;
    config.ru_nof_prbs                       = 273;
    config.compr_params                      = {compression_type::BFP, 9};
    config.direction                         = data_direction::uplink;

    ether::vlan_frame_params vlan_params = {{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11},
                                            {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x22},
                                            ether::vlan_parameters{.tci_vid = 1},
                                            0xaabb};

    data_flow_uplane_data_impl_dependencies dependencies;
    dependencies.logger         = &ocudulog::fetch_basic_logger("TEST");
    dependencies.compressor_sel = std::make_unique<ofh::testing::iq_compressor_dummy>();
    dependencies.frame_pool     = std::make_shared<ether::eth_frame_pool>(
        *dependencies.logger, units::bytes(9000), 2, message_type::uplane_prach, data_direction::uplink);

    ofh_uplane_packet_builder_spy* uplane_builder;
    {
      auto temp               = std::make_unique<ofh_uplane_packet_builder_spy>();
      uplane_builder          = temp.get();
      dependencies.up_builder = std::move(temp);
    }
    dependencies.eth_builder   = std::make_unique<ether::testing::vlan_frame_builder_spy>(vlan_params);
    dependencies.ecpri_builder = std::make_unique<ecpri::testing::packet_builder_spy>();

    data_flow_uplane_data_impl data_flow(config, std::move(dependencies));
    prach_buffer_double        prach(/*long preamble sequence length=*/839);

    data_flow_uplane_prach_context context;
    context.slot         = slot_point(to_numerology_value(test.pusch_scs), 0, 0);
    context.sector       = 0;
    context.port         = 0;
    context.eaxc         = 4;
    context.prb_start    = 0;
    context.nof_prb      = 72;
    context.start_symbol = 2;
    context.nof_symbols  = 1;
    context.filter_index = filter_index_type::ul_prach_preamble_1p25khz;
    context.prach_scs    = prach_subcarrier_spacing::kHz1_25;

    data_flow.enqueue_prach_message(context, prach);

    ASSERT_EQ(uplane_builder->nof_built_packets(), 1);
    span<const cbf16_t> iq = uplane_builder->get_iq_data(context.start_symbol);
    for (unsigned re = 0; re != test.expected_offset; ++re) {
      ASSERT_EQ(iq[re], cbf16_t{});
    }
    ASSERT_EQ(iq[test.expected_offset], to_cbf16(cf_t(1.0F, 0.0F)));
    ASSERT_EQ(iq[test.expected_offset + 838], to_cbf16(cf_t(839.0F, 0.0F)));
    ASSERT_EQ(iq[test.expected_offset + 839], cbf16_t{});
  }
}

TEST(ofh_data_flow_uplane_data_impl, prach_section_too_small_does_not_build_message)
{
  data_flow_uplane_data_impl_config config = {};
  config.sector                            = 0;
  config.cp                                = cyclic_prefix::NORMAL;
  config.ru_nof_prbs                       = 273;
  config.compr_params                      = {compression_type::BFP, 9};
  config.direction                         = data_direction::uplink;

  ether::vlan_frame_params vlan_params = {{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11},
                                          {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x22},
                                          ether::vlan_parameters{.tci_vid = 1},
                                          0xaabb};

  data_flow_uplane_data_impl_dependencies dependencies;
  dependencies.logger         = &ocudulog::fetch_basic_logger("TEST");
  dependencies.compressor_sel = std::make_unique<ofh::testing::iq_compressor_dummy>();
  dependencies.frame_pool     = std::make_shared<ether::eth_frame_pool>(
      *dependencies.logger, units::bytes(9000), 2, message_type::uplane_prach, data_direction::uplink);

  ofh_uplane_packet_builder_spy* uplane_builder;
  {
    auto temp               = std::make_unique<ofh_uplane_packet_builder_spy>();
    uplane_builder          = temp.get();
    dependencies.up_builder = std::move(temp);
  }
  dependencies.eth_builder   = std::make_unique<ether::testing::vlan_frame_builder_spy>(vlan_params);
  dependencies.ecpri_builder = std::make_unique<ecpri::testing::packet_builder_spy>();

  data_flow_uplane_data_impl data_flow(config, std::move(dependencies));
  prach_buffer_double        prach(/*long preamble sequence length=*/839);

  data_flow_uplane_prach_context context;
  context.slot         = slot_point(to_numerology_value(subcarrier_spacing::kHz30), 0, 0);
  context.sector       = 0;
  context.port         = 0;
  context.eaxc         = 4;
  context.prb_start    = 0;
  context.nof_prb      = 1;
  context.start_symbol = 2;
  context.nof_symbols  = 1;
  context.filter_index = filter_index_type::ul_prach_preamble_1p25khz;
  context.prach_scs    = prach_subcarrier_spacing::kHz1_25;

  data_flow.enqueue_prach_message(context, prach);

  ASSERT_EQ(uplane_builder->nof_built_packets(), 0);
}
