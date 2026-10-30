// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/du/du_high/du_manager/converters/asn1_ntn_config_helpers.h"
#include "lib/du/du_high/du_manager/converters/asn1_sys_info_packer.h"
#include "tests/ocudu_test_requirements.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/bcch_dl_sch_msg.h"
#include "ocudu/asn1/rrc_nr/sys_info.h"
#include "ocudu/pcap/mac_pcap.h"
#include "ocudu/ran/sib/system_info_config.h"
#include "ocudu/support/executors/task_worker.h"
#include <chrono>
#include <cmath>
#include <gtest/gtest.h>
#include <string>
#include <utility>

using namespace ocudu;
using namespace odu;

// Global variables for PCAP support
bool             g_enable_pcap = false;
ocudu::mac_pcap* g_pcap        = nullptr;

static std::pair<double, double> decode_ellipsoid_point_to_lat_lon_deg(const asn1::dyn_octstring& encoded)
{
  uint64_t bits = 0;
  for (uint8_t octet : encoded) {
    bits = (bits << 8u) | octet;
  }

  bool     south   = ((bits >> 47u) & 0x1u) != 0;
  uint32_t lat_enc = static_cast<uint32_t>((bits >> 24u) & 0x7fffffu);
  uint32_t lon_enc = static_cast<uint32_t>(bits & 0xffffffu);
  int32_t  lon_raw = static_cast<int32_t>(lon_enc) - 8388608;

  // Inverse of the TS 23.032 sec. 6.1 encoding: latitude scale 2^23/90, longitude scale 2^24/360.
  double lat_deg = static_cast<double>(lat_enc) * 90.0 / 8388608.0;
  if (south) {
    lat_deg = -lat_deg;
  }
  double lon_deg = static_cast<double>(lon_raw) * 360.0 / 16777216.0;

  return {lat_deg, lon_deg};
}

// Helper function to create a fully populated ntn_config with customizable parameters
ntn_config make_test_ntn_config(unsigned koffset_ms       = 260,
                                unsigned k_mac            = 512,
                                double   pos_x            = 1300.0,
                                double   pos_y            = 2600.0,
                                double   pos_z            = 3900.0,
                                unsigned sync_validity    = 60,
                                bool     use_rhcp         = true,
                                bool     enable_ta_report = true)
{
  ntn_config cfg;

  // Basic timing parameters
  cfg.cell_specific_koffset.emplace(std::chrono::milliseconds(koffset_ms));
  cfg.k_mac                    = std::chrono::milliseconds{k_mac};
  cfg.ntn_ul_sync_validity_dur = sync_validity;

  // ECEF ephemeris
  cfg.ephemeris_info.emplace();
  auto& ecef       = std::get<ecef_coordinates_t>(cfg.ephemeris_info.value());
  ecef.position_x  = pos_x;
  ecef.position_y  = pos_y;
  ecef.position_z  = pos_z;
  ecef.velocity_vx = pos_x / 5000.0; // Derived from position for variety
  ecef.velocity_vy = pos_y / 5000.0;
  ecef.velocity_vz = pos_z / 5000.0;

  // Epoch time
  cfg.epoch_time.emplace();
  cfg.epoch_time->sfn             = static_cast<unsigned>(koffset_ms * 2);
  cfg.epoch_time->subframe_number = koffset_ms % 10;

  // Timing advance
  cfg.ta_info.emplace();
  cfg.ta_info->ta_common               = 1000.0 - koffset_ms;
  cfg.ta_info->ta_common_drift         = 0.5;
  cfg.ta_info->ta_common_drift_variant = 0.01;
  cfg.ta_info->ta_common_offset        = 50.0;

  // Polarization
  cfg.polarization.emplace();
  auto pol_type = use_rhcp ? ntn_polarization_t::polarization_type::rhcp : ntn_polarization_t::polarization_type::lhcp;
  cfg.polarization->dl = pol_type;
  cfg.polarization->ul = pol_type;

  cfg.ta_report = enable_ta_report;

  return cfg;
}

TEST(srs_sib19_ntn_test, orbital_ephemeris_retrograde_inclination_packs)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-SI-1");

  const double sun_sync_inclination = 1.7104;

  sib19_info sib19;
  sib19.ntn_cfg.emplace();
  auto& orbit         = sib19.ntn_cfg->ephemeris_info.emplace(orbital_coordinates_t{});
  auto& orb           = std::get<orbital_coordinates_t>(orbit);
  orb.semi_major_axis = 6900000.0;
  orb.eccentricity    = 0.001;
  orb.periapsis       = 1.0;
  orb.longitude       = 2.0;
  orb.mean_anomaly    = 3.0;
  orb.inclination     = sun_sync_inclination;

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  ASSERT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  ASSERT_TRUE(sib19_decoded.ntn_cfg_r17_present);
  ASSERT_EQ(sib19_decoded.ntn_cfg_r17.ephemeris_info_r17.type(),
            asn1::rrc_nr::ephemeris_info_r17_c::types::orbital_r17);
  const auto& decoded_orbit = sib19_decoded.ntn_cfg_r17.ephemeris_info_r17.orbital_r17();

  // Negative branch is used and decodes back to the original inclination via field * step + pi.
  EXPECT_LT(decoded_orbit.inclination_r17, 0);
  const double decoded_inclination = decoded_orbit.inclination_r17 * 0.00000002341 + M_PI;
  EXPECT_NEAR(decoded_inclination, sun_sync_inclination, 1e-6);
}

TEST(srs_sib19_ntn_test, distance_thresh_encoding)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-SI-1");

  sib19_info sib19;
  sib19.distance_thres = 5000; // 5000 meters / 50 = 100

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  EXPECT_TRUE(sib19_decoded.distance_thresh_r17_present);
  EXPECT_EQ(sib19_decoded.distance_thresh_r17, 100);
}

TEST(srs_sib19_ntn_test, make_asn1_rrc_cell_sib19_moving_ref_location)
{
  sib19_info sib19;
  // Set moving reference location with geodetic coordinates
  sib19.moving_ref_location.emplace();
  sib19.moving_ref_location->latitude  = 45.5;   // degrees
  sib19.moving_ref_location->longitude = -122.3; // degrees

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  // Verify moving_ref_location_r18 is present and contains data
  EXPECT_FALSE(sib19_decoded.moving_ref_location_r18.empty());
  // The octstring should contain LPP ellipsoid point encoding (at least a few bytes)
  EXPECT_GT(sib19_decoded.moving_ref_location_r18.size(), 0);

  auto [decoded_lat, decoded_lon] = decode_ellipsoid_point_to_lat_lon_deg(sib19_decoded.moving_ref_location_r18);
  EXPECT_NEAR(decoded_lat, 45.5, 3e-5);
  EXPECT_NEAR(decoded_lon, -122.3, 3e-5);
}

TEST(srs_sib19_ntn_test, make_asn1_rrc_cell_sib19_ref_location_positive_longitude)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-SI-1");

  sib19_info sib19;
  sib19.ref_location.emplace();
  sib19.ref_location->latitude  = 48.135;
  sib19.ref_location->longitude = 11.582;

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  EXPECT_FALSE(sib19_decoded.ref_location_r17.empty());
  EXPECT_EQ(sib19_decoded.ref_location_r17.size(), 6u);

  auto [decoded_lat, decoded_lon] = decode_ellipsoid_point_to_lat_lon_deg(sib19_decoded.ref_location_r17);
  EXPECT_NEAR(decoded_lat, 48.135, 3e-5);
  EXPECT_NEAR(decoded_lon, 11.582, 3e-5);
}

TEST(srs_sib19_ntn_test, make_asn1_rrc_cell_sib19_coverage_enhancements)
{
  sib19_info sib19;
  // Set coverage enhancements
  sib19.coverage_enhancements.emplace();
  sib19.coverage_enhancements->nof_msg4_harq_ack_rep    = 4;  // 0-15 range
  sib19.coverage_enhancements->rsrp_thres_msg4_harq_ack = 60; // 0-127 range

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  // Verify coverage enhancements fields
  EXPECT_TRUE(sib19_decoded.ntn_cov_enh_r18.is_present());
  EXPECT_EQ(sib19_decoded.ntn_cov_enh_r18->nof_msg4_harq_ack_repeats_r18.to_number(), 4);
  EXPECT_TRUE(sib19_decoded.ntn_cov_enh_r18->rsrp_thres_msg4_harq_ack_r18_present);
  EXPECT_EQ(sib19_decoded.ntn_cov_enh_r18->rsrp_thres_msg4_harq_ack_r18, 60);
}

TEST(srs_sib19_ntn_test, make_asn1_rrc_cell_sib19_sat_switch)
{
  sib19_info sib19;
  // Set satellite switch with resync
  sib19.sat_switch_with_resync.emplace();
  sib19.sat_switch_with_resync->ntn_cfg.cell_specific_koffset.emplace(std::chrono::milliseconds(100));
  sib19.sat_switch_with_resync->t_service_start =
      std::chrono::system_clock::time_point(std::chrono::milliseconds(123456789));
  sib19.sat_switch_with_resync->ssb_time_offset_sf.emplace(50); // 0-159 range

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  // Verify satellite switch fields
  EXPECT_TRUE(sib19_decoded.sat_switch_with_re_sync_r18.is_present());
  EXPECT_EQ(sib19_decoded.sat_switch_with_re_sync_r18->ntn_cfg_r18.cell_specific_koffset_r17, 100);
  EXPECT_TRUE(sib19_decoded.sat_switch_with_re_sync_r18->t_service_start_r18_present);
  // Verify conversion: Unix time (ms) -> ASN.1 format (multiples of 10ms since 1900)
  constexpr int64_t expected_asn1_value = (123456789 + 2208988800LL * 1000LL) / 10;
  EXPECT_EQ(sib19_decoded.sat_switch_with_re_sync_r18->t_service_start_r18, expected_asn1_value);
  EXPECT_TRUE(sib19_decoded.sat_switch_with_re_sync_r18->ssb_time_offset_r18_present);
  EXPECT_EQ(sib19_decoded.sat_switch_with_re_sync_r18->ssb_time_offset_r18, 50);
}

TEST(srs_sib19_ntn_test, make_asn1_rrc_cell_sib19_neighbor_cells)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-SI-3");

  sib19_info sib19;
  // Add 2 neighbor cells with mixed optional fields
  neighbor_ntn_cell neighbor1;
  neighbor1.phys_cell_id.emplace(100);
  neighbor1.carrier_freq.emplace(arfcn_t{650000});
  sib19.ncells.push_back(neighbor1);

  neighbor_ntn_cell neighbor2;
  neighbor2.phys_cell_id.emplace(200);
  neighbor2.ntn_cfg.emplace();
  neighbor2.ntn_cfg->cell_specific_koffset.emplace(std::chrono::milliseconds(150));
  sib19.ncells.push_back(neighbor2);

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  // Verify neighbor cells
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17.size(), 2);
  EXPECT_TRUE(sib19_decoded.ntn_neigh_cell_cfg_list_r17[0].pci_r17_present);
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17[0].pci_r17, 100);
  EXPECT_TRUE(sib19_decoded.ntn_neigh_cell_cfg_list_r17[0].carrier_freq_r17_present);
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17[0].carrier_freq_r17, 650000);

  EXPECT_TRUE(sib19_decoded.ntn_neigh_cell_cfg_list_r17[1].pci_r17_present);
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17[1].pci_r17, 200);
  EXPECT_TRUE(sib19_decoded.ntn_neigh_cell_cfg_list_r17[1].ntn_cfg_r17_present);
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17[1].ntn_cfg_r17.cell_specific_koffset_r17, 150);
}

TEST(srs_sib19_ntn_test, make_asn1_rrc_cell_sib19_neighbor_cells_extended)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-SI-3");

  sib19_info sib19;
  // Add 6 neighbors (first 4 in base list, remaining 2 in extension list)
  for (int i = 0; i < 6; i++) {
    neighbor_ntn_cell neighbor;
    neighbor.phys_cell_id.emplace(100 + i);
    sib19.ncells.push_back(neighbor);
  }

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  // Verify first 4 neighbors in base list
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17.size(), 4);
  for (int i = 0; i < 4; i++) {
    EXPECT_TRUE(sib19_decoded.ntn_neigh_cell_cfg_list_r17[i].pci_r17_present);
    EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17[i].pci_r17, 100 + i);
  }

  // Verify remaining 2 neighbors in extension list
  EXPECT_TRUE(sib19_decoded.ntn_neigh_cell_cfg_list_ext_v1720.is_present());
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_ext_v1720->size(), 2);
  for (int i = 0; i < 2; i++) {
    EXPECT_TRUE((*sib19_decoded.ntn_neigh_cell_cfg_list_ext_v1720)[i].pci_r17_present);
    EXPECT_EQ((*sib19_decoded.ntn_neigh_cell_cfg_list_ext_v1720)[i].pci_r17, 104 + i);
  }
}

TEST(srs_sib19_ntn_test, make_asn1_rrc_cell_sib19_all_r18_fields)
{
  sib19_info sib19;

  // Set all R18 fields simultaneously
  sib19.moving_ref_location.emplace();
  sib19.moving_ref_location->latitude  = 37.7749;
  sib19.moving_ref_location->longitude = -122.4194;

  sib19.coverage_enhancements.emplace();
  sib19.coverage_enhancements->nof_msg4_harq_ack_rep    = 8;
  sib19.coverage_enhancements->rsrp_thres_msg4_harq_ack = 75;

  sib19.sat_switch_with_resync.emplace();
  sib19.sat_switch_with_resync->ntn_cfg.cell_specific_koffset.emplace(std::chrono::milliseconds(200));
  sib19.sat_switch_with_resync->ssb_time_offset_sf.emplace(100);

  neighbor_ntn_cell neighbor;
  neighbor.phys_cell_id.emplace(300);
  sib19.ncells.push_back(neighbor);

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  // Verify all R18 fields are present
  EXPECT_FALSE(sib19_decoded.moving_ref_location_r18.empty());
  EXPECT_TRUE(sib19_decoded.ntn_cov_enh_r18.is_present());
  EXPECT_TRUE(sib19_decoded.sat_switch_with_re_sync_r18.is_present());
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17.size(), 1);

  // Verify extension group 1 is enabled
}

TEST(srs_sib19_ntn_test, max_ncells_supported)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-SI-3");

  sib19_info sib19;
  // Add maximum 8 neighbors (4 base + 4 extension)
  for (int i = 0; i < 8; i++) {
    neighbor_ntn_cell neighbor;
    neighbor.phys_cell_id.emplace(100 + i);
    sib19.ncells.push_back(neighbor);
  }

  std::string js_str;
  auto        buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(buf.empty());

  asn1::cbit_ref            bref(buf);
  asn1::rrc_nr::sib19_r17_s sib19_decoded;
  EXPECT_EQ(sib19_decoded.unpack(bref), asn1::OCUDUASN_SUCCESS);

  // Verify all 8 neighbors are encoded (4 in base, 4 in extension)
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_r17.size(), 4);
  EXPECT_TRUE(sib19_decoded.ntn_neigh_cell_cfg_list_ext_v1720.is_present());
  EXPECT_EQ(sib19_decoded.ntn_neigh_cell_cfg_list_ext_v1720->size(), 4);

  // NOTE: Adding more than 8 neighbors will trigger an assertion in debug builds
  // due to ASN.1 constraint: base list (1..4) + extension list (1..4) = max 8
}

TEST(srs_sib19_ntn_test, complete_sib19_all_fields_with_pcap)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-SI-1");

  sib19_info sib19;

  // ===== Base R17 Fields =====
  // NTN Config (serving satellite)
  sib19.ntn_cfg = make_test_ntn_config();

  // Distance Threshold
  sib19.distance_thres = 10000; // 10 km

  // Reference Location
  sib19.ref_location.emplace();
  sib19.ref_location->latitude  = 37.7749;
  sib19.ref_location->longitude = -122.4194;

  // T-Service (UTC timepoint)
  sib19.t_service = std::chrono::system_clock::time_point(std::chrono::milliseconds(1000000000));

  // ===== R18 Extension Fields =====
  // Moving Reference Location
  sib19.moving_ref_location.emplace();
  sib19.moving_ref_location->latitude  = -45.5;
  sib19.moving_ref_location->longitude = 12.3;

  // Coverage Enhancements
  sib19.coverage_enhancements.emplace();
  sib19.coverage_enhancements->nof_msg4_harq_ack_rep    = 4;
  sib19.coverage_enhancements->rsrp_thres_msg4_harq_ack = 60;

  // Satellite Switch with Resynchronization (target satellite with different params)
  sib19.sat_switch_with_resync.emplace();
  sib19.sat_switch_with_resync->ntn_cfg = make_test_ntn_config(100, 256, 2000.0, 3000.0, 4000.0, 30, false, false);
  sib19.sat_switch_with_resync->t_service_start =
      std::chrono::system_clock::time_point(std::chrono::milliseconds(123456789));
  sib19.sat_switch_with_resync->ssb_time_offset_sf.emplace(50);

  // Neighbor Cells (8 neighbors: 4 base + 4 extension)
  const std::array<unsigned, 8> sync_durs = {30, 35, 40, 45, 50, 55, 60, 120}; // Valid per TS 38.331
  for (int i = 0; i < 8; i++) {
    neighbor_ntn_cell neighbor;
    neighbor.phys_cell_id = 100 + i;
    neighbor.carrier_freq = arfcn_t{static_cast<unsigned>(650000 + i * 1000)};
    neighbor.ntn_cfg      = make_test_ntn_config(100 + i * 10,
                                            256 + i * 32,
                                            1000.0 + i * 100,
                                            2000.0 + i * 100,
                                            3000.0 + i * 100,
                                            sync_durs[i],
                                            true,
                                            i % 2 == 0);
    sib19.ncells.push_back(neighbor);
  }

  // Create ASN.1 SIB19 structure
  asn1::rrc_nr::sib19_r17_s sib19_asn1 = odu::make_asn1_rrc_cell_sib19(sib19);

  // Pack as SystemInformation BCCH-DL-SCH message (as done in real network)
  asn1::rrc_nr::bcch_dl_sch_msg_s si_msg;
  si_msg.msg.set_c1();
  si_msg.msg.c1().set_sys_info();
  auto& sys_info = si_msg.msg.c1().sys_info();
  sys_info.crit_exts.set_sys_info();
  auto& sys_info_r15 = sys_info.crit_exts.sys_info();
  sys_info_r15.sib_type_and_info.resize(1);
  sys_info_r15.sib_type_and_info[0].set_sib19_v1700();
  sys_info_r15.sib_type_and_info[0].sib19_v1700() = sib19_asn1;

  // Pack to byte buffer
  byte_buffer         bcch_buf;
  asn1::bit_ref       bref(bcch_buf);
  asn1::OCUDUASN_CODE ret = si_msg.pack(bref);
  EXPECT_EQ(ret, asn1::OCUDUASN_SUCCESS);

  // Generate JSON representation for debugging
  std::string js_str;
  auto        sib19_test_buf = asn1_packer::pack_sib19(sib19, &js_str);
  EXPECT_FALSE(sib19_test_buf.empty());

  // Verify decoding
  asn1::cbit_ref                  bref_decode(bcch_buf);
  asn1::rrc_nr::bcch_dl_sch_msg_s si_msg_decoded;
  EXPECT_EQ(si_msg_decoded.unpack(bref_decode), asn1::OCUDUASN_SUCCESS);
  EXPECT_TRUE(si_msg_decoded.msg.c1().type() == asn1::rrc_nr::bcch_dl_sch_msg_type_c::c1_c_::types::sys_info);

  // Write to PCAP if enabled
  if (g_enable_pcap && g_pcap) {
    // Create a copy for PCAP since we need bcch_buf for verification
    byte_buffer pcap_buf = bcch_buf.deep_copy().value();

    ocudu::mac_nr_context_info context = {};
    context.radioType                  = ocudu::PCAP_FDD_RADIO;
    context.direction                  = ocudu::PCAP_DIRECTION_DOWNLINK;
    context.rntiType                   = ocudu::PCAP_SI_RNTI;
    context.rnti                       = 0xffff; // SI-RNTI
    context.ueid                       = 0;
    context.harqid                     = 0;
    context.system_frame_number        = 0;
    context.sub_frame_number           = 5;

    g_pcap->push_pdu(context, std::move(pcap_buf));
    printf("\n=== SIB19 with all fields written to PCAP ===\n");
    printf("PCAP file: /tmp/sib19_ntn_complete.pcap\n");
    printf("Use Wireshark (ver. 4.6.3 or higher) to analyze: wireshark /tmp/sib19_ntn_complete.pcap\n");
    printf("Filter: mac-nr.rnti == 0xffff\n");
    printf("\nJSON representation:\n%s\n", js_str.c_str());
  }

  // Verify the packed message
  EXPECT_FALSE(bcch_buf.empty());
  EXPECT_GT(bcch_buf.length(), 100); // Should be substantial size with all fields
}

int main(int argc, char** argv)
{
  // Check for '--enable_pcap' cmd line argument
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--enable_pcap") {
      g_enable_pcap = true;
    }
  }

  ocudulog::init();

  std::unique_ptr<task_worker_executor> pcap_exec;
  std::unique_ptr<task_worker>          pcap_worker;
  std::unique_ptr<ocudu::mac_pcap>      pcap_writer;

  if (g_enable_pcap) {
    pcap_worker = std::make_unique<task_worker>("pcap_worker", 128);
    pcap_exec   = std::make_unique<task_worker_executor>(*pcap_worker);
    pcap_writer = create_mac_pcap("/tmp/sib19_ntn_complete.pcap", ocudu::mac_pcap_type::udp, *pcap_exec);
    g_pcap      = pcap_writer.get();
    printf("\n=== PCAP enabled: /tmp/sib19_ntn_complete.pcap ===\n\n");
  }

  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();

  if (pcap_writer) {
    pcap_writer->close();
    pcap_writer.reset();
  }
  if (pcap_worker) {
    pcap_worker->wait_pending_tasks();
    pcap_worker->stop();
  }

  return result;
}
