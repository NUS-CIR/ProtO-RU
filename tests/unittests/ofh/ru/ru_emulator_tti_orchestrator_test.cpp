// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_tti_orchestrator.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/prach/prach_format_type.h"
#include "ocudu/ran/slot_point_extended.h"
#include <gtest/gtest.h>

using namespace ocudu;

namespace {

class uplink_plane_handler_spy : public ru_uplink_plane_handler
{
public:
  unsigned             nof_uplink_requests = 0;
  unsigned             nof_prach_requests  = 0;
  slot_point           last_slot;
  prach_buffer_context last_prach_context = {};

  void handle_prach_occasion(const prach_buffer_context& context, shared_prach_buffer) override
  {
    ++nof_prach_requests;
    last_prach_context = context;
  }
  void handle_new_uplink_slot(const resource_grid_context& context, const shared_resource_grid&) override
  {
    ++nof_uplink_requests;
    last_slot = context.slot;
  }
};

/// Minimal O-RU sector double: only \ref get_recorded_prach_occasion is exercised by the orchestrator.
class ru_sector_spy : public ofh::ru_sector
{
public:
  std::optional<ofh::ru_prach_occasion> occasion;

  void                                           on_new_frame(span<const uint8_t>) override {}
  span<ofh::ota_symbol_boundary_notifier* const> get_ota_symbol_boundary_notifiers() override { return {}; }
  uint64_t                                       get_nof_rx_on_time_messages() const override { return 0; }
  uint64_t                                       get_nof_rx_early_messages() const override { return 0; }
  uint64_t                                       get_nof_rx_late_messages() const override { return 0; }
  ofh::ru_uplink_iq_sink*                        get_uplink_iq_sink() override { return nullptr; }

  std::optional<ofh::ru_prach_occasion> get_recorded_prach_occasion(slot_point slot) const override
  {
    if (occasion && occasion->slot == slot) {
      return occasion;
    }
    return std::nullopt;
  }
};

/// PRACH occasions are peeked this many slots in the past (the production value is max_proc_delay).
constexpr unsigned PRACH_LOOKBACK = 2;

ru_emulator_tti_orchestrator_config make_config()
{
  ru_emulator_tti_orchestrator_config config;
  config.sector                    = 0;
  config.nof_prb                   = 51;
  config.nof_ul_ports              = 1;
  config.scs                       = subcarrier_spacing::kHz30;
  config.prach_peek_lookback_slots = PRACH_LOOKBACK;
  return config;
}

tti_boundary_context make_tti(uint32_t slot_count)
{
  tti_boundary_context ctx;
  ctx.slot = slot_point_extended(subcarrier_spacing::kHz30, slot_count);
  return ctx;
}

} // namespace

TEST(ru_emulator_tti_orchestrator_test, tti_boundary_requests_uplink_capture)
{
  auto          sdr_upper_phy = create_ru_emulator_sdr_upper_phy({/*sector=*/0, /*nof_prb=*/51, /*nof_dl_ports=*/1});
  ru_sector_spy sector;
  auto          orchestrator =
      create_ru_emulator_tti_orchestrator(make_config(), ocudulog::fetch_basic_logger("TEST"), *sdr_upper_phy, sector);

  uplink_plane_handler_spy ul;
  orchestrator->connect(ul);

  orchestrator->on_tti_boundary(make_tti(5));

  // The orchestrator requested an uplink capture for the slot; no PRACH was scheduled.
  ASSERT_EQ(ul.nof_uplink_requests, 1);
  ASSERT_EQ(ul.last_slot, slot_point(to_numerology_value(subcarrier_spacing::kHz30), 5));
  ASSERT_EQ(ul.nof_prach_requests, 0);
}

TEST(ru_emulator_tti_orchestrator_test, recorded_prach_occasion_requests_capture_with_lookback)
{
  auto       sdr_upper_phy = create_ru_emulator_sdr_upper_phy({/*sector=*/0, /*nof_prb=*/51, /*nof_dl_ports=*/1});
  slot_point prach_slot(to_numerology_value(subcarrier_spacing::kHz30), 7);

  // The O-RU sector recorded a PRACH Control-Plane occasion for slot 7, starting at symbol 2 on eAxC 4.
  ru_sector_spy sector;
  sector.occasion = ofh::ru_prach_occasion{
      prach_slot, /*eaxc=*/4, /*start_symbol=*/2, /*rb_offset=*/6, prach_subcarrier_spacing::kHz30};

  auto orchestrator =
      create_ru_emulator_tti_orchestrator(make_config(), ocudulog::fetch_basic_logger("TEST"), *sdr_upper_phy, sector);
  uplink_plane_handler_spy ul;
  orchestrator->connect(ul);

  // The occasion's own TTI fires too early for its Control-Plane to have been recorded in deployment; it is peeked
  // with the lookback instead, so the occasion's own TTI requests nothing.
  orchestrator->on_tti_boundary(make_tti(7));
  ASSERT_EQ(ul.nof_prach_requests, 0);

  // The TTI 'lookback' slots after the occasion peeks it and requests a capture with the hardcoded short-format
  // context, filled with the occasion's slot and start symbol.
  orchestrator->on_tti_boundary(make_tti(7 + PRACH_LOOKBACK));
  ASSERT_EQ(ul.nof_prach_requests, 1);
  ASSERT_EQ(ul.last_prach_context.slot, prach_slot);
  ASSERT_EQ(ul.last_prach_context.start_symbol, 2);
  ASSERT_EQ(ul.last_prach_context.rb_offset, 6);
  ASSERT_EQ(ul.last_prach_context.format, prach_format_type::B4);
  ASSERT_EQ(ul.last_prach_context.nof_prb_ul_grid, 51);
  ASSERT_EQ(ul.last_prach_context.nof_td_occasions, 1);
  ASSERT_EQ(ul.last_prach_context.nof_fd_occasions, 1);
  ASSERT_EQ(ul.last_prach_context.pusch_scs, subcarrier_spacing::kHz30);
  ASSERT_EQ(ul.last_prach_context.ports.size(), 1);
}

TEST(ru_emulator_tti_orchestrator_test, fifteen_khz_scs_propagates_to_prach_context)
{
  auto       sdr_upper_phy = create_ru_emulator_sdr_upper_phy({/*sector=*/0, /*nof_prb=*/52, /*nof_dl_ports=*/1});
  slot_point prach_slot(to_numerology_value(subcarrier_spacing::kHz15), 7);

  ru_sector_spy sector;
  sector.occasion = ofh::ru_prach_occasion{
      prach_slot, /*eaxc=*/4, /*start_symbol=*/2, /*rb_offset=*/4, prach_subcarrier_spacing::kHz15};

  ru_emulator_tti_orchestrator_config config;
  config.sector                    = 0;
  config.nof_prb                   = 52;
  config.nof_ul_ports              = 1;
  config.scs                       = subcarrier_spacing::kHz15;
  config.prach_peek_lookback_slots = PRACH_LOOKBACK;

  auto orchestrator =
      create_ru_emulator_tti_orchestrator(config, ocudulog::fetch_basic_logger("TEST"), *sdr_upper_phy, sector);
  uplink_plane_handler_spy ul;
  orchestrator->connect(ul);

  tti_boundary_context ctx;
  ctx.slot = slot_point_extended(subcarrier_spacing::kHz15, 7 + PRACH_LOOKBACK);
  orchestrator->on_tti_boundary(ctx);

  ASSERT_EQ(ul.nof_uplink_requests, 1);
  ASSERT_EQ(ul.last_slot, slot_point(to_numerology_value(subcarrier_spacing::kHz15), 7 + PRACH_LOOKBACK));
  ASSERT_EQ(ul.nof_prach_requests, 1);
  ASSERT_EQ(ul.last_prach_context.slot, prach_slot);
  ASSERT_EQ(ul.last_prach_context.pusch_scs, subcarrier_spacing::kHz15);
  ASSERT_EQ(ul.last_prach_context.nof_prb_ul_grid, 52);
  ASSERT_EQ(ul.last_prach_context.rb_offset, 4);
}

TEST(ru_emulator_tti_orchestrator_test, gps_slot_offset_translates_ofh_side_lookups)
{
  auto sdr_upper_phy = create_ru_emulator_sdr_upper_phy({/*sector=*/0, /*nof_prb=*/51, /*nof_dl_ports=*/1});

  // The radio ticks slot 9; with a GPS slot offset of 3 the OFH side keys this TTI as wire slot 12, and the PRACH
  // lookback peeks wire slot 10 (radio slot 7).
  const unsigned   gps_slot_offset = 3;
  const slot_point radio_tti_slot(to_numerology_value(subcarrier_spacing::kHz30), 9);
  const slot_point prach_radio_slot(to_numerology_value(subcarrier_spacing::kHz30), 7);
  const slot_point prach_ofh_slot(to_numerology_value(subcarrier_spacing::kHz30), 10);

  // A PRACH occasion was recorded from the O-DU's Control-Plane for wire slot 10.
  ru_sector_spy sector;
  sector.occasion = ofh::ru_prach_occasion{
      prach_ofh_slot, /*eaxc=*/4, /*start_symbol=*/2, /*rb_offset=*/6, prach_subcarrier_spacing::kHz30};

  auto orchestrator =
      create_ru_emulator_tti_orchestrator(make_config(), ocudulog::fetch_basic_logger("TEST"), *sdr_upper_phy, sector);
  orchestrator->set_gps_slot_offset(gps_slot_offset);

  uplink_plane_handler_spy ul;
  orchestrator->connect(ul);

  orchestrator->on_tti_boundary(make_tti(9));

  // The recorded PRACH occasion is matched through the wire numbering; the radio-facing requests carry the radio slot
  // numbering.
  ASSERT_EQ(ul.nof_prach_requests, 1);
  ASSERT_EQ(ul.last_prach_context.slot, prach_radio_slot);
  ASSERT_EQ(ul.last_prach_context.start_symbol, 2);
  ASSERT_EQ(ul.nof_uplink_requests, 1);
  ASSERT_EQ(ul.last_slot, radio_tti_slot);
}

TEST(ru_emulator_tti_orchestrator_test, boundary_before_connect_is_ignored)
{
  auto          sdr_upper_phy = create_ru_emulator_sdr_upper_phy({/*sector=*/0, /*nof_prb=*/51, /*nof_dl_ports=*/1});
  ru_sector_spy sector;
  auto          orchestrator =
      create_ru_emulator_tti_orchestrator(make_config(), ocudulog::fetch_basic_logger("TEST"), *sdr_upper_phy, sector);

  // No handler connected yet: the boundary is a no-op (does not crash).
  orchestrator->on_tti_boundary(make_tti(5));
}
