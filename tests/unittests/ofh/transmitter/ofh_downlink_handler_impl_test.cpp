// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../phy/support/resource_grid_test_doubles.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ofh_data_flow_cplane_scheduling_commands_test_doubles.h"
#include "tests/ocudu_test_requirements.h"
#include "transmitter/helpers.h"
#include "transmitter/ofh_data_flow_uplane_data.h"
#include "transmitter/ofh_downlink_handler_impl.h"
#include "ocudu/adt/format.h"
#include "ocudu/ofh/ofh_error_notifier.h"
#include "ocudu/phy/support/resource_grid_context.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include "ocudu/ran/antenna_topology.h"
#include <gtest/gtest.h>
#include <numeric>

using namespace ocudu;
using namespace ofh;
using namespace ocudu::ofh::testing;
using namespace std::chrono_literals;

namespace {

/// Spy User-Plane downlink data data flow.
class data_flow_uplane_data_spy : public data_flow_uplane_data, public operation_controller
{
public:
  struct spy_info {
    unsigned eaxc = -1;
    unsigned port = -1;
  };

  // See interface for documentation.
  void start() override {}

  // See interface for documentation.
  void stop() override {}

  // See interface for documentation.
  operation_controller& get_operation_controller() override { return *this; }

  // See interface for documentation.
  void enqueue_section_type_1_message(const data_flow_uplane_resource_grid_context& context,
                                      const shared_resource_grid&                   grid) override
  {
    has_enqueue_section_type_1_message_method_been_called = true;
    eaxc                                                  = context.eaxc;
    section_type_1_calls.push_back({context.eaxc, context.port});
  }

  // See interface for documentation.
  data_flow_message_encoding_metrics_collector* get_metrics_collector() override { return nullptr; }

  /// Returns true if the method enqueue section type 1 message has been called, otherwise false.
  bool has_enqueue_section_type_1_method_been_called() const
  {
    return has_enqueue_section_type_1_message_method_been_called;
  }

  /// Returns the configured eAxC.
  unsigned get_eaxc() const { return eaxc; }

  /// Returns the information of every enqueued section type 1 message, in enqueueing order.
  span<const spy_info> get_section_type_1_calls() const { return section_type_1_calls; }

private:
  bool                  has_enqueue_section_type_1_message_method_been_called = false;
  unsigned              eaxc                                                  = -1;
  std::vector<spy_info> section_type_1_calls;
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

} // namespace

static constexpr units::bytes mtu_size{9000};

static downlink_handler_impl_config generate_default_config()
{
  downlink_handler_impl_config config;
  config.dl_eaxc                       = {24};
  config.sector                        = 0;
  config.cp                            = cyclic_prefix::NORMAL;
  config.scs                           = subcarrier_spacing::kHz30;
  config.enable_log_warnings_for_lates = true;
  config.is_cat_b_enabled              = false;
  // Transmission timing parameters corresponding to:
  // T1a_max_cp_dl=500us, T1a_min_cp_dl=200us,
  // T1a_max_cp_ul=300us, T1a_min_cp_ul=150us,
  // T1a_max_up=250us, T1a_min_up=100us.
  config.tx_timing_params = {13, 6, 8, 5, 6, 3};
  return config;
}

static downlink_handler_impl_dependencies
generate_dependencies(error_notifier&                                           notifier,
                      std::unique_ptr<data_flow_cplane_scheduling_commands_spy> cplane,
                      std::unique_ptr<data_flow_uplane_data_spy>                uplane)
{
  return {ocudulog::fetch_basic_logger("TEST"),
          notifier,
          std::move(cplane),
          std::move(uplane),
          std::make_shared<ether::eth_frame_pool>(ocudulog::fetch_basic_logger("TEST"),
                                                  mtu_size,
                                                  2,
                                                  ofh::message_type::control_plane,
                                                  ofh::data_direction::downlink),
          std::make_shared<ether::eth_frame_pool>(ocudulog::fetch_basic_logger("TEST"),
                                                  mtu_size,
                                                  2,
                                                  ofh::message_type::user_plane,
                                                  ofh::data_direction::downlink)};
}

/// Returns the number of symbols that a resource grid must precede its slot by to be transmitted in time.
static unsigned get_nof_symbols_before_ota(const downlink_handler_impl_config& config)
{
  return get_biggest_min_tx_parameter(config.tx_timing_params);
}

/// Notifies the given handler an OTA time that precedes the given slot by the given number of symbols.
static void notify_ota_time(downlink_handler_impl&              handler,
                            const downlink_handler_impl_config& config,
                            slot_point                          slot,
                            unsigned                            nof_symbols_before_slot)
{
  slot_symbol_point ota_time(slot, 0, get_nsymb_per_slot(config.cp));
  ota_time -= nof_symbols_before_slot;
  handler.get_ota_symbol_boundary_notifier().on_new_symbol({ota_time, {}});
}

TEST(ofh_downlink_handler_impl, handling_downlink_data_use_control_and_user_plane)
{
  downlink_handler_impl_config config = generate_default_config();

  error_notifier_spy notifier_spy;
  auto               cplane     = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
  const auto&        cplane_spy = *cplane;
  auto               uplane     = std::make_unique<data_flow_uplane_data_spy>();
  const auto&        uplane_spy = *uplane;

  downlink_handler_impl handler(config, generate_dependencies(notifier_spy, std::move(cplane), std::move(uplane)));
  handler.start();

  resource_grid_reader_spy rg_reader_spy(1, 1, 1);
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{});
  resource_grid_writer_spy rg_writer_spy(1, 1, 1);
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy rg(rg_spy);

  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 1);
  rg_context.sector = 1;

  // Set the OTA well before the grid slot, so that the grid is not late.
  notify_ota_time(handler, config, rg_context.slot, 3 * get_nof_symbols_before_ota(config));

  handler.handle_dl_data(rg_context, rg.get_grid());

  ASSERT_FALSE(notifier_spy.is_downlink_late());
  ASSERT_FALSE(notifier_spy.is_uplink_late());

  // Assert Control-Plane.
  ASSERT_TRUE(cplane_spy.has_enqueue_section_type_1_method_been_called());
  const data_flow_cplane_scheduling_commands_spy::spy_info& info = cplane_spy.get_spy_info();
  ASSERT_EQ(rg_context.slot, info.slot);
  ASSERT_EQ(config.dl_eaxc[0], info.eaxc);
  ASSERT_EQ(data_direction::downlink, info.direction);
  ASSERT_EQ(filter_index_type::standard_channel_filter, info.filter_type);

  // Assert User-Plane.
  ASSERT_TRUE(uplane_spy.has_enqueue_section_type_1_method_been_called());
  ASSERT_EQ(config.dl_eaxc[0], uplane_spy.get_eaxc());
}

TEST(ofh_downlink_handler_impl, late_rg_is_not_handled)
{
  downlink_handler_impl_config config = generate_default_config();

  error_notifier_spy notifier_spy;
  auto               cplane     = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
  const auto&        cplane_spy = *cplane;
  auto               uplane     = std::make_unique<data_flow_uplane_data_spy>();
  const auto&        uplane_spy = *uplane;

  downlink_handler_impl handler(config, generate_dependencies(notifier_spy, std::move(cplane), std::move(uplane)));
  handler.start();

  resource_grid_reader_spy rg_reader_spy(1, 1, 1);
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{});
  resource_grid_writer_spy rg_writer_spy(1, 1, 1);
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy rg(rg_spy);

  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 1);
  rg_context.sector = 1;

  // Set the OTA exactly at the transmission window boundary, which makes the grid late.
  notify_ota_time(handler, config, rg_context.slot, get_nof_symbols_before_ota(config));

  handler.handle_dl_data(rg_context, rg.get_grid());

  // Assert Control-Plane.
  ASSERT_FALSE(cplane_spy.has_enqueue_section_type_1_method_been_called());
  ASSERT_FALSE(uplane_spy.has_enqueue_section_type_1_method_been_called());
  ASSERT_TRUE(notifier_spy.is_downlink_late());
}

TEST(ofh_downlink_handler_impl, same_slot_fails)
{
  downlink_handler_impl_config config = generate_default_config();

  error_notifier_spy notifier_spy;
  auto               cplane     = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
  const auto&        cplane_spy = *cplane;
  auto               uplane     = std::make_unique<data_flow_uplane_data_spy>();
  const auto&        uplane_spy = *uplane;

  downlink_handler_impl handler(config, generate_dependencies(notifier_spy, std::move(cplane), std::move(uplane)));
  handler.start();

  resource_grid_reader_spy rg_reader_spy(1, 1, 1);
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{});
  resource_grid_writer_spy rg_writer_spy(1, 1, 1);
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy rg(rg_spy);

  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 1);
  rg_context.sector = 1;

  // Set the OTA to the same slot and symbol as the grid, which makes the grid late.
  notify_ota_time(handler, config, rg_context.slot, 0);

  handler.handle_dl_data(rg_context, rg.get_grid());

  // Assert Control-Plane.
  ASSERT_FALSE(cplane_spy.has_enqueue_section_type_1_method_been_called());
  ASSERT_FALSE(uplane_spy.has_enqueue_section_type_1_method_been_called());
  ASSERT_TRUE(notifier_spy.is_downlink_late());
}

TEST(ofh_downlink_handler_impl, rg_in_the_frontier_is_handled)
{
  downlink_handler_impl_config config = generate_default_config();

  error_notifier_spy notifier_spy;
  auto               cplane     = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
  const auto&        cplane_spy = *cplane;
  auto               uplane     = std::make_unique<data_flow_uplane_data_spy>();
  const auto&        uplane_spy = *uplane;

  downlink_handler_impl handler(config, generate_dependencies(notifier_spy, std::move(cplane), std::move(uplane)));
  handler.start();

  resource_grid_reader_spy rg_reader_spy(1, 1, 1);
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{});
  resource_grid_writer_spy rg_writer_spy(1, 1, 1);
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy rg(rg_spy);

  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 1);
  rg_context.sector = 1;

  // Set the OTA one symbol before the transmission window boundary, the earliest OTA that is not late.
  notify_ota_time(handler, config, rg_context.slot, get_nof_symbols_before_ota(config) + 1);

  handler.handle_dl_data(rg_context, rg.get_grid());

  // Assert Control-Plane.
  ASSERT_TRUE(cplane_spy.has_enqueue_section_type_1_method_been_called());
  ASSERT_TRUE(uplane_spy.has_enqueue_section_type_1_method_been_called());
  ASSERT_FALSE(notifier_spy.is_downlink_late());
}

TEST(ofh_downlink_handler_impl, category_a_transmits_one_beam_port_per_eaxc)
{
  downlink_handler_impl_config config = generate_default_config();
  config.dl_eaxc                      = {24, 25};

  error_notifier_spy notifier_spy;
  auto               cplane     = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
  const auto&        cplane_spy = *cplane;
  auto               uplane     = std::make_unique<data_flow_uplane_data_spy>();
  const auto&        uplane_spy = *uplane;

  downlink_handler_impl handler(config, generate_dependencies(notifier_spy, std::move(cplane), std::move(uplane)));
  handler.start();

  // Write the second antenna port only, the first one is left empty.
  resource_grid_reader_spy rg_reader_spy(2, 1, 1);
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{1, 0, 0, {1.0F, 0.0F}});
  resource_grid_writer_spy rg_writer_spy(2, 1, 1);
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy rg(rg_spy);

  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 1);
  rg_context.sector = 1;
  notify_ota_time(handler, config, rg_context.slot, 3 * get_nof_symbols_before_ota(config));

  handler.handle_dl_data(rg_context, rg.get_grid());

  ASSERT_FALSE(notifier_spy.is_downlink_late());

  // All beam-ports (including empty ones) corresponding to the configured eAxCs are transmitted.
  span<const data_flow_cplane_scheduling_commands_spy::spy_info> cplane_calls = cplane_spy.get_section_type_1_calls();
  ASSERT_EQ(2, cplane_calls.size());
  ASSERT_EQ(config.dl_eaxc[0], cplane_calls[0].eaxc);
  ASSERT_EQ(config.dl_eaxc[1], cplane_calls[1].eaxc);
  ASSERT_EQ(to_beam_id(0), cplane_calls[0].beam_id);
  ASSERT_EQ(to_beam_id(1), cplane_calls[1].beam_id);

  span<const data_flow_uplane_data_spy::spy_info> uplane_calls = uplane_spy.get_section_type_1_calls();
  ASSERT_EQ(2, uplane_calls.size());
  ASSERT_EQ(0, uplane_calls[0].port);
  ASSERT_EQ(config.dl_eaxc[0], uplane_calls[0].eaxc);
  ASSERT_EQ(1, uplane_calls[1].port);
  ASSERT_EQ(config.dl_eaxc[1], uplane_calls[1].eaxc);
}

TEST(ofh_downlink_handler_impl, category_b_maps_non_empty_beam_ports_onto_eaxcs)
{
  OCUDU_TEST_REQUIREMENTS("RU-OFH-CATB-WDBF");

  downlink_handler_impl_config config = generate_default_config();
  config.dl_eaxc                      = {24, 25};
  config.is_cat_b_enabled             = true;

  error_notifier_spy notifier_spy;
  auto               cplane     = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
  const auto&        cplane_spy = *cplane;
  auto               uplane     = std::make_unique<data_flow_uplane_data_spy>();
  const auto&        uplane_spy = *uplane;

  downlink_handler_impl handler(config, generate_dependencies(notifier_spy, std::move(cplane), std::move(uplane)));
  handler.start();

  // Sparse resource grid: only the beam-ports 1 and 3 carry a transmission.
  resource_grid_reader_spy rg_reader_spy(4, 1, 1);
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{1, 0, 0, {1.0F, 0.0F}});
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{3, 0, 0, {1.0F, 0.0F}});
  resource_grid_writer_spy rg_writer_spy(4, 1, 1);
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy rg(rg_spy);

  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 1);
  rg_context.sector = 1;
  notify_ota_time(handler, config, rg_context.slot, 3 * get_nof_symbols_before_ota(config));

  handler.handle_dl_data(rg_context, rg.get_grid());

  ASSERT_FALSE(notifier_spy.is_downlink_late());

  // The non-empty beam-ports are compacted onto the eAxC pool, whilst the beam identifier keeps the beam-port index.
  span<const data_flow_cplane_scheduling_commands_spy::spy_info> cplane_calls = cplane_spy.get_section_type_1_calls();
  ASSERT_EQ(2, cplane_calls.size());
  ASSERT_EQ(config.dl_eaxc[0], cplane_calls[0].eaxc);
  ASSERT_EQ(to_beam_id(1), cplane_calls[0].beam_id);
  ASSERT_EQ(config.dl_eaxc[1], cplane_calls[1].eaxc);
  ASSERT_EQ(to_beam_id(3), cplane_calls[1].beam_id);

  span<const data_flow_uplane_data_spy::spy_info> uplane_calls = uplane_spy.get_section_type_1_calls();
  ASSERT_EQ(2, uplane_calls.size());
  ASSERT_EQ(1, uplane_calls[0].port);
  ASSERT_EQ(config.dl_eaxc[0], uplane_calls[0].eaxc);
  ASSERT_EQ(3, uplane_calls[1].port);
  ASSERT_EQ(config.dl_eaxc[1], uplane_calls[1].eaxc);
}

TEST(ofh_downlink_handler_impl, category_b_rejects_more_active_beam_ports_than_configured_eaxcs)
{
  OCUDU_TEST_REQUIREMENTS("RU-OFH-CATB-WDBF");

  downlink_handler_impl_config config = generate_default_config();
  config.dl_eaxc                      = {24, 25};
  config.is_cat_b_enabled             = true;

  error_notifier_spy notifier_spy;
  auto               cplane = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
  auto               uplane = std::make_unique<data_flow_uplane_data_spy>();

  downlink_handler_impl handler(config, generate_dependencies(notifier_spy, std::move(cplane), std::move(uplane)));
  handler.start();

  // Three active beam-ports, but only two eAxCs are configured.
  resource_grid_reader_spy rg_reader_spy(4, 1, 1);
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{0, 0, 0, {1.0F, 0.0F}});
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{1, 0, 0, {1.0F, 0.0F}});
  rg_reader_spy.write(resource_grid_reader_spy::expected_entry_t{2, 0, 0, {1.0F, 0.0F}});
  resource_grid_writer_spy rg_writer_spy(4, 1, 1);
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy rg(rg_spy);

  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 1);
  rg_context.sector = 1;
  notify_ota_time(handler, config, rg_context.slot, 3 * get_nof_symbols_before_ota(config));

  ASSERT_DEATH(
      handler.handle_dl_data(rg_context, rg.get_grid()),
      fmt::format("Resource grid needs '{}' downlink eAxCs and only '{}' are configured", 3, config.dl_eaxc.size()));
}

TEST(ofh_downlink_handler_impl, category_a_rejects_more_beam_ports_than_eaxcs)
{
  // Use a topology that defines DFT beams on top of the port-selection ones.
  static constexpr antenna_topology topology = antenna_topology::single_panel_two_one;
  static_assert(get_total_nof_beams(topology) > get_total_nof_ports(topology),
                "The topology must define beams beyond the antenna ports");

  downlink_handler_impl_config config = generate_default_config();
  config.dl_eaxc.resize(get_total_nof_ports(topology));
  std::iota(config.dl_eaxc.begin(), config.dl_eaxc.end(), 24);

  error_notifier_spy notifier_spy;
  auto               cplane = std::make_unique<data_flow_cplane_scheduling_commands_spy>();
  auto               uplane = std::make_unique<data_flow_uplane_data_spy>();

  downlink_handler_impl handler(config, generate_dependencies(notifier_spy, std::move(cplane), std::move(uplane)));
  handler.start();

  // Category A cannot transmit a grid sized to the beam grid, even when the DFT beam-ports are empty.
  resource_grid_reader_spy rg_reader_spy(get_total_nof_beams(topology), 1, 1);
  resource_grid_writer_spy rg_writer_spy(get_total_nof_beams(topology), 1, 1);
  resource_grid_spy        rg_spy(rg_reader_spy, rg_writer_spy);
  shared_resource_grid_spy rg(rg_spy);

  resource_grid_context rg_context;
  rg_context.slot   = slot_point(1, 1, 1);
  rg_context.sector = 1;
  notify_ota_time(handler, config, rg_context.slot, 3 * get_nof_symbols_before_ota(config));

  ASSERT_DEATH(handler.handle_dl_data(rg_context, rg.get_grid()),
               fmt::format("Resource grid needs '{}' downlink eAxCs and only '{}' are configured",
                           get_total_nof_beams(topology),
                           config.dl_eaxc.size()));
}
