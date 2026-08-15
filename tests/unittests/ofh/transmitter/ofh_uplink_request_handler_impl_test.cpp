// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/transmitter/helpers.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "../../../../lib/ofh/transmitter/ofh_uplink_request_handler_impl.h"
#include "../../phy/support/resource_grid_test_doubles.h"
#include "../receiver/helpers.h"
#include "ofh_data_flow_cplane_scheduling_commands_test_doubles.h"
#include "ocudu/adt/format.h"
#include "ocudu/ofh/ofh_error_notifier.h"
#include "ocudu/ofh/ofh_uplane_rx_symbol_notifier.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_timing_parameters.h"
#include "ocudu/phy/support/resource_grid.h"
#include "ocudu/phy/support/resource_grid_mapper.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include "ocudu/phy/support/resource_grid_writer.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;
using namespace ocudu::ofh::testing;

static const static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> eaxc            = {2};
static const static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc      = {4};
static constexpr unsigned                                    REPOSITORY_SIZE = 20U;
static constexpr units::bytes                                mtu_size{9000};

namespace {

/// Spy User-Plane received symbol notifier
class uplane_rx_symbol_notifier_spy : public uplane_rx_symbol_notifier
{
  const resource_grid_reader* rg_reader = nullptr;

public:
  void on_new_uplink_symbol(const uplane_rx_symbol_context& context, shared_resource_grid grid, bool is_valid) override
  {
    rg_reader = &(grid.get_reader());
  }

  void on_new_prach_window_data(const prach_buffer_context& context, shared_prach_buffer buffer) override {}

  const resource_grid_reader* get_reasource_grid_reader() const { return rg_reader; }
};

/// Error notifier spy implementation.
class error_notifier_spy : public error_notifier
{
  bool dl_late    = false;
  bool ul_late    = false;
  bool prach_late = false;

public:
  // See interface for documentation.
  void on_late_downlink_message(const error_context& context) override { dl_late = true; }

  // See interface for documentation.
  void on_late_uplink_message(const error_context& context) override { ul_late = true; }

  // See interface for documentation.
  void on_late_prach_message(const error_context& context) override { prach_late = true; }

  bool is_downlink_late() const { return dl_late; }
  bool is_uplink_late() const { return ul_late; }
  bool is_prach_late() const { return prach_late; }
};

class ofh_uplink_request_handler_impl_fixture : public ::testing::Test
{
protected:
  const cyclic_prefix                         cp               = {cyclic_prefix::NORMAL};
  subcarrier_spacing                          scs              = subcarrier_spacing::kHz30;
  const tdd_ul_dl_config_common               ttd_pattern      = {subcarrier_spacing::kHz30, {10, 6, 6, 3, 3}, {}};
  tx_window_timing_parameters                 tx_timing_params = {0, 0, 8, 5, 0, 0};
  unsigned                                    nof_symbols      = get_nsymb_per_slot(cp);
  std::unique_ptr<prach_buffer_pool>          prach_pool;
  error_notifier_spy                          notifier_spy;
  uplink_request_handler_impl_config          cfg;
  resource_grid_reader_spy                    reader_spy;
  resource_grid_writer_spy                    writer_spy;
  resource_grid_spy                           grid;
  shared_resource_grid_spy                    shared_grid;
  std::shared_ptr<rx_grid_context_repository> ul_slot_repo;
  std::shared_ptr<prach_context_repository>   ul_prach_repo;
  std::shared_ptr<uplink_notified_grid_symbol_repository> notified_symbol_repo;
  data_flow_cplane_scheduling_commands_spy*               data_flow;
  data_flow_cplane_scheduling_commands_spy*               data_flow_prach;
  uplink_request_handler_impl                             handler;
  uplink_request_handler_impl                             handler_prach_cp_en;

  explicit ofh_uplink_request_handler_impl_fixture() :
    prach_pool(create_prach_buffer_pool(1)),
    reader_spy(1, 14, 1),
    writer_spy(1, 14, 1),
    grid(reader_spy, writer_spy),
    shared_grid(grid),
    ul_slot_repo(std::make_shared<rx_grid_context_repository>(REPOSITORY_SIZE)),
    ul_prach_repo(std::make_shared<prach_context_repository>(REPOSITORY_SIZE)),
    notified_symbol_repo(std::make_unique<uplink_notified_grid_symbol_repository>(REPOSITORY_SIZE)),
    handler(get_config_prach_cp_disabled(), get_dependencies_prach_cp_disabled()),
    handler_prach_cp_en(get_config_prach_cp_enabled(), get_dependencies_prach_cp_enabled())
  {
  }

  uplink_request_handler_impl_dependencies get_dependencies_prach_cp_disabled()
  {
    auto temp = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
    data_flow = temp.get();

    return {ocudulog::fetch_basic_logger("TEST"),
            notifier_spy,
            ul_slot_repo,
            ul_prach_repo,
            notified_symbol_repo,
            std::move(temp),
            std::make_shared<ether::eth_frame_pool>(ocudulog::fetch_basic_logger("TEST"),
                                                    mtu_size,
                                                    2,
                                                    ofh::message_type::control_plane,
                                                    ofh::data_direction::uplink)};
  }

  uplink_request_handler_impl_dependencies get_dependencies_prach_cp_enabled()
  {
    auto temp       = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
    data_flow_prach = temp.get();

    return {ocudulog::fetch_basic_logger("TEST"),
            notifier_spy,
            ul_slot_repo,
            ul_prach_repo,
            notified_symbol_repo,
            std::move(temp),
            std::make_shared<ether::eth_frame_pool>(ocudulog::fetch_basic_logger("TEST"),
                                                    mtu_size,
                                                    2,
                                                    ofh::message_type::control_plane,
                                                    ofh::data_direction::uplink)};
  }

  uplink_request_handler_impl_config get_config_prach_cp_disabled()
  {
    uplink_request_handler_impl_config config;
    config.sector              = 0;
    config.prach_eaxc          = {};
    config.ul_data_eaxc        = eaxc;
    config.is_prach_cp_enabled = false;
    config.cp                  = cyclic_prefix::NORMAL;
    config.scs                 = subcarrier_spacing::kHz30;
    config.tdd_config.emplace(ttd_pattern);
    config.tx_timing_params              = tx_timing_params;
    config.enable_log_warnings_for_lates = true;

    return config;
  }

  uplink_request_handler_impl_config get_config_prach_cp_enabled()
  {
    uplink_request_handler_impl_config config;
    config.sector                        = 0;
    config.prach_eaxc                    = prach_eaxc;
    config.ul_data_eaxc                  = {};
    config.is_prach_cp_enabled           = true;
    config.cp                            = cyclic_prefix::NORMAL;
    config.scs                           = subcarrier_spacing::kHz30;
    config.tx_timing_params              = tx_timing_params;
    config.enable_log_warnings_for_lates = true;

    return config;
  }
};

struct prach_time_offset_test_params {
  prach_format_type  format;
  subcarrier_spacing scs;
  unsigned           slot_index;
  uint8_t            start_symbol;
  phy_time_unit      expected_time_offset;
};

static const prach_time_offset_test_params prach_time_offset_test_cases[] = {
    // B4 at 30 kHz: symbol 2 starts at 2208 Ts and the unextended PRACH CP is 468 Ts.
    {prach_format_type::B4, subcarrier_spacing::kHz30, 0, 2, phy_time_unit::from_units_of_kappa(2676)},
    {prach_format_type::B4, subcarrier_spacing::kHz30, 1, 2, phy_time_unit::from_units_of_kappa(2676)},
    // At symbol zero, the PRACH CP overlaps a half-millisecond boundary.
    {prach_format_type::B4, subcarrier_spacing::kHz30, 0, 0, phy_time_unit::from_units_of_kappa(484)},
    {prach_format_type::B4, subcarrier_spacing::kHz30, 1, 0, phy_time_unit::from_units_of_kappa(484)},
    // At 15 kHz, symbol 7 starts at 0.5 ms. Cover occasions before, ending at, across and after it.
    {prach_format_type::A1, subcarrier_spacing::kHz15, 0, 2, phy_time_unit::from_units_of_kappa(4688)},
    {prach_format_type::A1, subcarrier_spacing::kHz15, 0, 5, phy_time_unit::from_units_of_kappa(11264)},
    {prach_format_type::A1, subcarrier_spacing::kHz15, 0, 6, phy_time_unit::from_units_of_kappa(13472)},
    {prach_format_type::A1, subcarrier_spacing::kHz15, 0, 7, phy_time_unit::from_units_of_kappa(15664)},
    {prach_format_type::A1, subcarrier_spacing::kHz15, 0, 8, phy_time_unit::from_units_of_kappa(17856)},
    // A B4 occasion at 15 kHz starting at zero overlaps both extension boundaries.
    {prach_format_type::B4, subcarrier_spacing::kHz15, 0, 0, phy_time_unit::from_units_of_kappa(968)},
    // At higher numerologies, only some slots contain a longer OFDM CP or a PRACH CP extension.
    {prach_format_type::A1, subcarrier_spacing::kHz60, 0, 0, phy_time_unit::from_units_of_kappa(88)},
    {prach_format_type::A1, subcarrier_spacing::kHz60, 1, 0, phy_time_unit::from_units_of_kappa(72)},
    {prach_format_type::A1, subcarrier_spacing::kHz60, 2, 0, phy_time_unit::from_units_of_kappa(88)},
    {prach_format_type::A1, subcarrier_spacing::kHz60, 0, 2, phy_time_unit::from_units_of_kappa(1184)},
    {prach_format_type::A1, subcarrier_spacing::kHz60, 1, 2, phy_time_unit::from_units_of_kappa(1168)},
    {prach_format_type::A1, subcarrier_spacing::kHz60, 2, 2, phy_time_unit::from_units_of_kappa(1184)},
    // An occasion ending exactly at the 0.5 ms slot boundary is not extended.
    {prach_format_type::A1, subcarrier_spacing::kHz60, 1, 12, phy_time_unit::from_units_of_kappa(6648)},
    {prach_format_type::B4, subcarrier_spacing::kHz120, 0, 2, phy_time_unit::from_units_of_kappa(681)},
    {prach_format_type::B4, subcarrier_spacing::kHz120, 1, 2, phy_time_unit::from_units_of_kappa(665)},
    {prach_format_type::B4, subcarrier_spacing::kHz120, 4, 2, phy_time_unit::from_units_of_kappa(681)},
    // Long preambles use 15 kHz reference symbol timing and never extend the PRACH CP.
    {prach_format_type::zero, subcarrier_spacing::kHz15, 0, 0, phy_time_unit::from_units_of_kappa(3168)},
    {prach_format_type::zero, subcarrier_spacing::kHz15, 0, 2, phy_time_unit::from_units_of_kappa(7568)},
    {prach_format_type::one, subcarrier_spacing::kHz15, 0, 0, phy_time_unit::from_units_of_kappa(21024)},
    {prach_format_type::three, subcarrier_spacing::kHz30, 0, 2, phy_time_unit::from_units_of_kappa(7568)}};

class ofh_prach_time_offset_test : public ofh_uplink_request_handler_impl_fixture,
                                   public ::testing::WithParamInterface<prach_time_offset_test_params>
{};

} // namespace

TEST_P(ofh_prach_time_offset_test, time_offset_is_slot_relative_and_includes_prach_cp)
{
  const auto& params = GetParam();

  prach_buffer_context context{};
  context.nof_fd_occasions = 1;
  context.nof_td_occasions = 1;
  context.format           = params.format;
  context.slot             = slot_point(to_numerology_value(params.scs), 20, params.slot_index);
  context.pusch_scs        = params.scs;
  context.start_symbol     = params.start_symbol;

  auto config = get_config_prach_cp_enabled();
  config.scs  = params.scs;
  uplink_request_handler_impl prach_handler(config, get_dependencies_prach_cp_enabled());

  slot_symbol_point ota_time(context.slot, 0, nof_symbols);
  ota_time -= get_biggest_min_tx_parameter(tx_timing_params) + 1;
  prach_handler.get_ota_symbol_boundary_notifier().on_new_symbol({ota_time, {}});

  prach_handler.handle_prach_occasion(context, prach_pool->get());
  ul_prach_repo->process_pending_contexts();

  ASSERT_TRUE(data_flow_prach->has_enqueue_section_type_3_method_been_called());
  const auto info = data_flow_prach->get_spy_info();
  ASSERT_EQ(info.time_offset, params.expected_time_offset.to_samples(30.72e6));
  if (is_short_preamble(context.format)) {
    ASSERT_EQ(info.start_symbol, context.start_symbol);
  }
}

INSTANTIATE_TEST_SUITE_P(prach_timing,
                         ofh_prach_time_offset_test,
                         ::testing::ValuesIn(prach_time_offset_test_cases),
                         [](const ::testing::TestParamInfo<prach_time_offset_test_params>& param_info) {
                           return fmt::format("format{}_{}kHz_slot{}_symbol{}",
                                              to_string(param_info.param.format),
                                              scs_to_khz(param_info.param.scs),
                                              param_info.param.slot_index,
                                              param_info.param.start_symbol);
                         });

TEST_F(ofh_uplink_request_handler_impl_fixture,
       handle_prach_request_when_cplane_message_is_disable_for_prach_does_not_generate_cplane_message)
{
  prach_buffer_context context;
  context.nof_fd_occasions = 1;
  context.nof_td_occasions = 1;
  context.format           = prach_format_type::B4;
  context.slot             = slot_point(1, 20, 1);
  context.pusch_scs        = subcarrier_spacing::kHz30;

  // Set the OTA to the same slot as the buffer context.
  slot_symbol_point ota_time(context.slot, 0, nof_symbols);

  // Delay the OTA 3 times the minimum delay requirements.
  ota_time -= (3 * get_biggest_min_tx_parameter(tx_timing_params));
  handler.get_ota_symbol_boundary_notifier().on_new_symbol({ota_time, {}});

  handler.handle_prach_occasion(context, prach_pool->get());
  ul_prach_repo->process_pending_contexts();

  // Assert data flow.
  ASSERT_FALSE(data_flow->has_enqueue_section_type_1_method_been_called());
  ASSERT_FALSE(data_flow->has_enqueue_section_type_3_method_been_called());
  ASSERT_FALSE(notifier_spy.is_downlink_late());
  ASSERT_FALSE(notifier_spy.is_uplink_late());
  ASSERT_FALSE(notifier_spy.is_prach_late());
}

TEST_F(ofh_uplink_request_handler_impl_fixture, handle_prach_request_generates_cplane_message)
{
  prach_buffer_context context;
  context.nof_fd_occasions = 1;
  context.nof_td_occasions = 1;
  context.format           = prach_format_type::B4;
  context.slot             = slot_point(1, 20, 1);
  context.pusch_scs        = subcarrier_spacing::kHz30;
  context.start_symbol     = 0;

  // Set the OTA to the same slot as the buffer context.
  slot_symbol_point ota_time(context.slot, 0, nof_symbols);

  // Delay the OTA 1 slot + the minimum delay requirements.
  ota_time -= (get_biggest_min_tx_parameter(tx_timing_params) + 1);
  handler_prach_cp_en.get_ota_symbol_boundary_notifier().on_new_symbol({ota_time, {}});

  handler_prach_cp_en.handle_prach_occasion(context, prach_pool->get());
  ul_prach_repo->process_pending_contexts();

  // Assert data flow.
  ASSERT_FALSE(data_flow_prach->has_enqueue_section_type_1_method_been_called());
  ASSERT_TRUE(data_flow_prach->has_enqueue_section_type_3_method_been_called());

  const data_flow_cplane_scheduling_commands_spy::spy_info& info = data_flow_prach->get_spy_info();
  ASSERT_EQ(context.slot, info.slot);
  ASSERT_EQ(prach_eaxc[0], info.eaxc);
  ASSERT_EQ(data_direction::uplink, info.direction);
  ASSERT_EQ(filter_index_type::ul_prach_preamble_short, info.filter_type);
  ASSERT_FALSE(notifier_spy.is_downlink_late());
  ASSERT_FALSE(notifier_spy.is_uplink_late());
  ASSERT_FALSE(notifier_spy.is_prach_late());
}

TEST_F(ofh_uplink_request_handler_impl_fixture, handle_late_prach_request_does_not_generate_cplane_message)
{
  prach_buffer_context context;
  context.nof_fd_occasions = 1;
  context.nof_td_occasions = 1;
  context.format           = prach_format_type::B4;
  context.slot             = slot_point(1, 20, 1);
  context.pusch_scs        = subcarrier_spacing::kHz30;
  context.start_symbol     = 0;

  // Set the OTA to the same slot as the buffer context.
  slot_symbol_point ota_time(context.slot, 0, nof_symbols);

  // Delay the OTA exactly by the minimum delay requirements.
  ota_time -= get_biggest_min_tx_parameter(tx_timing_params);
  handler_prach_cp_en.get_ota_symbol_boundary_notifier().on_new_symbol({ota_time, {}});

  handler_prach_cp_en.handle_prach_occasion(context, prach_pool->get());
  ul_prach_repo->process_pending_contexts();

  // Assert data flow.
  ASSERT_FALSE(data_flow_prach->has_enqueue_section_type_1_method_been_called());
  ASSERT_FALSE(data_flow_prach->has_enqueue_section_type_3_method_been_called());
  ASSERT_TRUE(ul_prach_repo->get(context.slot).empty());
  ASSERT_FALSE(notifier_spy.is_downlink_late());
  ASSERT_FALSE(notifier_spy.is_downlink_late());
  ASSERT_FALSE(notifier_spy.is_uplink_late());
  ASSERT_TRUE(notifier_spy.is_prach_late());
}

TEST_F(ofh_uplink_request_handler_impl_fixture, handle_uplink_slot_generates_cplane_message)
{
  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 7);
  rg_context.sector = 1;

  // Set the OTA to the same slot as the resource grid context.
  slot_symbol_point ota_time(rg_context.slot, 0, nof_symbols);

  // Delay the OTA three times the minimum delay requirements.
  ota_time -= (3 * get_biggest_min_tx_parameter(tx_timing_params));
  handler.get_ota_symbol_boundary_notifier().on_new_symbol({ota_time, {}});

  handler.handle_new_uplink_slot(rg_context, shared_grid.get_grid());
  ul_slot_repo->process_pending_contexts();

  // Assert data flow.
  ASSERT_TRUE(data_flow->has_enqueue_section_type_1_method_been_called());
  const data_flow_cplane_scheduling_commands_spy::spy_info& info = data_flow->get_spy_info();
  ASSERT_EQ(rg_context.slot, info.slot);
  ASSERT_EQ(eaxc[0], info.eaxc);
  ASSERT_EQ(data_direction::uplink, info.direction);

  const ofdm_symbol_range symbol_range = get_active_tdd_ul_symbols(ttd_pattern, rg_context.slot.slot_index(), cp);
  for (unsigned i = 0, e = writer_spy.get_nof_symbols(); i != e; ++i) {
    ASSERT_FALSE(ul_slot_repo->get(rg_context.slot, i).empty());
  }

  // Assert that the symbol range equals the number of symbols of the grid.
  ASSERT_EQ(0, symbol_range.start());
  ASSERT_EQ(writer_spy.get_nof_symbols(), symbol_range.stop());
  ASSERT_FALSE(notifier_spy.is_downlink_late());
  ASSERT_FALSE(notifier_spy.is_uplink_late());
  ASSERT_FALSE(notifier_spy.is_prach_late());
}

TEST_F(ofh_uplink_request_handler_impl_fixture, handle_late_uplink_request_does_not_generates_cplane_message)
{
  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 7);
  rg_context.sector = 1;

  // Set the OTA to the same slot as the resource grid context.
  slot_symbol_point ota_time(rg_context.slot, 0, nof_symbols);
  handler.get_ota_symbol_boundary_notifier().on_new_symbol({ota_time, {}});

  handler.handle_new_uplink_slot(rg_context, shared_grid.get_grid());
  ul_slot_repo->process_pending_contexts();

  // Assert data flow.
  ASSERT_FALSE(data_flow->has_enqueue_section_type_1_method_been_called());
  ASSERT_FALSE(notifier_spy.is_downlink_late());
  ASSERT_TRUE(notifier_spy.is_uplink_late());
  ASSERT_FALSE(notifier_spy.is_prach_late());
}

TEST_F(ofh_uplink_request_handler_impl_fixture,
       handle_uplink_in_special_slot_generates_cplane_message_with_valid_symbols)
{
  resource_grid_context rg_context;
  // Use special slot.
  rg_context.slot   = slot_point(1, 1, 6);
  rg_context.sector = 1;

  // Set the OTA to the same slot as the resource grid context.
  slot_symbol_point ota_time(rg_context.slot, 0, nof_symbols);

  // Delay the OTA 3 slots.
  ota_time -= (3 * get_biggest_min_tx_parameter(tx_timing_params));
  handler.get_ota_symbol_boundary_notifier().on_new_symbol({ota_time, {}});

  handler.handle_new_uplink_slot(rg_context, shared_grid.get_grid());
  ul_slot_repo->process_pending_contexts();

  // Assert data flow.
  ASSERT_TRUE(data_flow->has_enqueue_section_type_1_method_been_called());
  const data_flow_cplane_scheduling_commands_spy::spy_info& info = data_flow->get_spy_info();
  ASSERT_EQ(rg_context.slot, info.slot);
  ASSERT_EQ(eaxc[0], info.eaxc);
  ASSERT_EQ(data_direction::uplink, info.direction);

  const ofdm_symbol_range symbol_range = get_active_tdd_ul_symbols(ttd_pattern, rg_context.slot.slot_index(), cp);
  for (unsigned i = 0, e = writer_spy.get_nof_symbols(); i != e; ++i) {
    if (i >= symbol_range.start() && i < symbol_range.stop()) {
      ASSERT_FALSE(ul_slot_repo->get(rg_context.slot, i).empty());
    } else {
      ASSERT_TRUE(ul_slot_repo->get(rg_context.slot, i).empty());
    }
  }
}
