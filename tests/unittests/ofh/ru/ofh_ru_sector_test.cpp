// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/ru_sector.h"
#include "ocudu/phy/support/prach_buffer.h"
#include "ocudu/phy/support/support_factories.h"
#include "ocudu/ran/resource_block.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

namespace {

class eth_transmitter_spy : public ether::transmitter
{
public:
  void                                  send(span<span<const uint8_t>>) override {}
  ether::transmitter_metrics_collector* get_metrics_collector() override { return nullptr; }
};

class prach_buffer_double : public prach_buffer
{
  std::vector<cbf16_t> data{1};

public:
  unsigned            get_max_nof_ports() const override { return 1; }
  unsigned            get_max_nof_td_occasions() const override { return 1; }
  unsigned            get_max_nof_fd_occasions() const override { return 1; }
  unsigned            get_max_nof_symbols() const override { return MAX_NSYMB_PER_SLOT; }
  unsigned            get_sequence_length() const override { return data.size(); }
  span<cbf16_t>       get_symbol(unsigned, unsigned, unsigned, unsigned) override { return data; }
  span<const cbf16_t> get_symbol(unsigned, unsigned, unsigned, unsigned) const override { return data; }
};

class upper_phy_dummy : public ru_upper_phy
{
  prach_buffer_double prach;

public:
  shared_resource_grid get_downlink_rx_grid(slot_point) override { return {}; }
  void                 on_downlink_rx_grid_completed(slot_point, const shared_resource_grid&) override {}
  shared_resource_grid get_uplink_tx_grid(slot_point) override { return {}; }
  const prach_buffer&  get_prach_tx_buffer(slot_point) override { return prach; }
};

ru_sector_config make_config()
{
  ru_sector_config config;
  config.sector                  = 0;
  config.scs                     = subcarrier_spacing::kHz30;
  config.cp                      = cyclic_prefix::NORMAL;
  config.ru_nof_prbs             = 51;
  config.ul_compr_params         = {compression_type::BFP, 9};
  config.dl_compr_params         = {compression_type::BFP, 9};
  config.prach_compr_params      = {compression_type::BFP, 9};
  config.vlan_params             = {{0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x11},
                                    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x22},
                                    ether::vlan_parameters{.tci_vid = 1},
                                    0xaabb};
  config.dl_eaxc                 = {0};
  config.ul_eaxc                 = {0};
  config.prach_eaxc              = {4};
  config.mtu_size                = units::bytes(9000);
  config.nof_frames_per_symbol   = 2;
  config.tx_window_start_symbols = 2;
  config.tx_window_end_symbols   = 0;
  config.rx_window               = {0, 10};

  return config;
}

} // namespace

TEST(ru_sector_test, sector_is_constructed_and_processes_a_frame)
{
  upper_phy_dummy upper_phy;

  ru_sector_dependencies dependencies;
  dependencies.logger          = &ocudulog::fetch_basic_logger("TEST");
  dependencies.upper_phy       = &upper_phy;
  dependencies.eth_transmitter = std::make_unique<eth_transmitter_spy>();

  std::unique_ptr<ru_sector> sector = create_ru_sector(make_config(), std::move(dependencies));
  ASSERT_NE(sector, nullptr);

  // The sector exposes its OTA notifiers for the application to subscribe to its timing source.
  ASSERT_EQ(sector->get_ota_symbol_boundary_notifiers().size(), 3);

  // A short/garbage frame is decoded and dropped without crashing.
  const uint8_t frame[] = {0x00, 0x01, 0x02, 0x03};
  sector->on_new_frame(frame);

  // In immediate mode (the default) there is no uplink IQ sink to drive.
  ASSERT_EQ(sector->get_uplink_iq_sink(), nullptr);
}

TEST(ru_sector_test, sector_without_vlan_configuration_is_constructed)
{
  upper_phy_dummy upper_phy;

  ru_sector_dependencies dependencies;
  dependencies.logger          = &ocudulog::fetch_basic_logger("TEST");
  dependencies.upper_phy       = &upper_phy;
  dependencies.eth_transmitter = std::make_unique<eth_transmitter_spy>();

  // Interfaces that insert the VLAN tag themselves (an SR-IOV VF port VLAN, for instance) are configured without one,
  // so the sector must build untagged frames instead of asking for a VLAN frame builder.
  ru_sector_config config = make_config();
  config.vlan_params.vlan_config.reset();

  std::unique_ptr<ru_sector> sector = create_ru_sector(config, std::move(dependencies));
  ASSERT_NE(sector, nullptr);

  const uint8_t frame[] = {0x00, 0x01, 0x02, 0x03};
  sector->on_new_frame(frame);
}

TEST(ru_sector_test, store_and_respond_sector_exposes_uplink_iq_sink)
{
  upper_phy_dummy upper_phy;

  ru_sector_dependencies dependencies;
  dependencies.logger          = &ocudulog::fetch_basic_logger("TEST");
  dependencies.upper_phy       = &upper_phy;
  dependencies.eth_transmitter = std::make_unique<eth_transmitter_spy>();

  ru_sector_config config     = make_config();
  config.uplink_response_mode = ru_uplink_response_mode::store_and_respond;

  std::unique_ptr<ru_sector> sector = create_ru_sector(config, std::move(dependencies));
  ASSERT_NE(sector, nullptr);

  // In store-and-respond mode the sector exposes the uplink IQ sink the radio drives.
  ru_uplink_iq_sink* sink = sector->get_uplink_iq_sink();
  ASSERT_NE(sink, nullptr);

  // Pushing a captured uplink symbol with no recorded Control-Plane request is a no-op (does not crash).
  auto                                        rg_factory = create_resource_grid_factory();
  std::vector<std::unique_ptr<resource_grid>> grids;
  grids.push_back(rg_factory->create(1, MAX_NSYMB_PER_SLOT, 51 * NOF_SUBCARRIERS_PER_RB));
  auto                 pool = create_generic_resource_grid_pool(std::move(grids));
  slot_point           slot(to_numerology_value(subcarrier_spacing::kHz30), 0);
  shared_resource_grid grid = pool->allocate_resource_grid(slot);
  sink->handle_uplink_symbol(slot, 0, grid);
}
