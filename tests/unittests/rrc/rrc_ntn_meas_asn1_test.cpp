// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/rrc/ue/rrc_measurement_types_asn1_converters.h"
#include "tests/ocudu_test_requirements.h"
#include <gtest/gtest.h>
#include <variant>

using namespace ocudu;
using namespace ocucp;

// ============================================================================
// Direct ASN1 encoding tests for ntn-NeighbourCellInfo-r18 in MeasObjectNR
// (cells_to_add_mod_list_ext_v1800). These tests call meas_obj_nr_to_rrc_asn1()
// directly, without the full RRC UE machinery.
// ============================================================================

static rrc_cells_to_add_mod make_cell(pci_t pci)
{
  rrc_cells_to_add_mod cell;
  cell.pci = pci;
  return cell;
}

static rrc_ntn_neighbour_cell_info make_ntn_info(bool with_ref_location = true)
{
  rrc_ntn_neighbour_cell_info info;
  info.epoch_time.sfn             = 512;
  info.epoch_time.subframe_number = 3;

  ecef_coordinates_t ecef;
  ecef.position_x  = -3621225.25;
  ecef.position_y  = -5839350.24;
  ecef.position_z  = 101120.52;
  ecef.velocity_vx = 3498.87;
  ecef.velocity_vy = -2055.89;
  ecef.velocity_vz = 6104.62;
  info.ephemeris   = ecef;

  if (with_ref_location) {
    info.ref_location = reference_location{45.0, 90.0};
  }
  return info;
}

/// Without NTN info the v1800 extension list must be absent.
TEST(ntn_meas_asn1, without_ntn_info_ext_list_is_absent)
{
  OCUDU_TEST_REQUIREMENTS("CU-NTN-MOB-1");

  rrc_meas_obj_nr meas_obj;
  meas_obj.cells_to_add_mod_list.push_back(make_cell(1));
  meas_obj.cells_to_add_mod_list.push_back(make_cell(2));

  auto asn1_obj = meas_obj_nr_to_rrc_asn1(meas_obj);

  EXPECT_FALSE(asn1_obj.cells_to_add_mod_list_ext_v1800.is_present());
  ASSERT_EQ(asn1_obj.cells_to_add_mod_list.size(), 2);
}

/// referenceLocation-r18 is OPTIONAL (Need R, TS 38.331): NTN info without it must still emit the IE, with the
/// reference location left absent (empty octet string).
TEST(ntn_meas_asn1, ntn_info_without_ref_location_still_emits_ie)
{
  OCUDU_TEST_REQUIREMENTS("CU-NTN-MOB-1");

  rrc_meas_obj_nr meas_obj;
  meas_obj.cells_to_add_mod_list.push_back(make_cell(1));
  meas_obj.cells_to_add_mod_list.back().ntn_neighbour_info = make_ntn_info(/*with_ref_location=*/false);

  auto asn1_obj = meas_obj_nr_to_rrc_asn1(meas_obj);

  ASSERT_TRUE(asn1_obj.cells_to_add_mod_list_ext_v1800.is_present());
  const auto& ext_list = *asn1_obj.cells_to_add_mod_list_ext_v1800;
  ASSERT_EQ(ext_list.size(), 1);
  ASSERT_TRUE(ext_list[0].ntn_neighbour_cell_info_r18_present);
  EXPECT_EQ(ext_list[0].ntn_neighbour_cell_info_r18.ref_location_r18.size(), 0);
}

/// The extension list must be parallel to cells_to_add_mod_list: one entry per cell, with the IE present only for
/// cells that carry NTN info.
TEST(ntn_meas_asn1, ext_list_is_parallel_to_cells_to_add_mod_list)
{
  OCUDU_TEST_REQUIREMENTS("CU-NTN-MOB-1");

  rrc_meas_obj_nr meas_obj;
  meas_obj.cells_to_add_mod_list.push_back(make_cell(1));
  meas_obj.cells_to_add_mod_list.push_back(make_cell(2));
  meas_obj.cells_to_add_mod_list.push_back(make_cell(3));
  meas_obj.cells_to_add_mod_list[1].ntn_neighbour_info = make_ntn_info();

  auto asn1_obj = meas_obj_nr_to_rrc_asn1(meas_obj);
  ASSERT_TRUE(asn1_obj.cells_to_add_mod_list_ext_v1800.is_present());
  const auto& ext_list = *asn1_obj.cells_to_add_mod_list_ext_v1800;
  ASSERT_EQ(ext_list.size(), meas_obj.cells_to_add_mod_list.size());
  EXPECT_FALSE(ext_list[0].ntn_neighbour_cell_info_r18_present);
  EXPECT_TRUE(ext_list[1].ntn_neighbour_cell_info_r18_present);
  EXPECT_FALSE(ext_list[2].ntn_neighbour_cell_info_r18_present);
}

/// Epoch time, ECEF ephemeris quantization, and reference location encoding.
TEST(ntn_meas_asn1, epoch_and_ecef_ephemeris_encode_correctly)
{
  OCUDU_TEST_REQUIREMENTS("CU-NTN-MOB-1");

  rrc_meas_obj_nr meas_obj;
  meas_obj.cells_to_add_mod_list.push_back(make_cell(1));
  meas_obj.cells_to_add_mod_list.back().ntn_neighbour_info = make_ntn_info();

  auto asn1_obj = meas_obj_nr_to_rrc_asn1(meas_obj);

  ASSERT_TRUE(asn1_obj.cells_to_add_mod_list_ext_v1800.is_present());
  const auto& ntn_asn1 = (*asn1_obj.cells_to_add_mod_list_ext_v1800)[0].ntn_neighbour_cell_info_r18;

  EXPECT_EQ(ntn_asn1.epoch_time_r18.sfn_r17, 512U);
  EXPECT_EQ(ntn_asn1.epoch_time_r18.sub_frame_nr_r17, 3U);

  // ECEF state vector: position quantized in 1.3 m steps, velocity in 0.06 m/s steps.
  ASSERT_EQ(ntn_asn1.ephemeris_info_r18.type(), asn1::rrc_nr::ephemeris_info_r17_c::types::position_velocity_r17);
  const auto& rv = ntn_asn1.ephemeris_info_r18.position_velocity_r17();
  EXPECT_EQ(rv.position_x_r17, static_cast<int32_t>(-3621225.25 / 1.3));
  EXPECT_EQ(rv.position_y_r17, static_cast<int32_t>(-5839350.24 / 1.3));
  EXPECT_EQ(rv.position_z_r17, static_cast<int32_t>(101120.52 / 1.3));
  EXPECT_EQ(rv.velocity_vx_r17, static_cast<int32_t>(3498.87 / 0.06));
  EXPECT_EQ(rv.velocity_vy_r17, static_cast<int32_t>(-2055.89 / 0.06));
  EXPECT_EQ(rv.velocity_vz_r17, static_cast<int32_t>(6104.62 / 0.06));

  // Reference location: 6-byte packed EllipsoidPoint, lat=45 deg north, lon=90 deg east.
  ASSERT_EQ(ntn_asn1.ref_location_r18.size(), 6U);
  // latitudeSign=north(0), degreesLatitude=floor(45 * 2^23 / 90)=4194304 -> top 24 bits = 0x400000.
  EXPECT_EQ(ntn_asn1.ref_location_r18[0], 0x40U);
  EXPECT_EQ(ntn_asn1.ref_location_r18[1], 0x00U);
  EXPECT_EQ(ntn_asn1.ref_location_r18[2], 0x00U);
  // degreesLongitude=floor(90 * 2^24 / 360)=4194304, offset-encoded: +8388608 = 0xc00000.
  EXPECT_EQ(ntn_asn1.ref_location_r18[3], 0xc0U);
  EXPECT_EQ(ntn_asn1.ref_location_r18[4], 0x00U);
  EXPECT_EQ(ntn_asn1.ref_location_r18[5], 0x00U);
}

/// Orbital ephemeris quantization per TS 38.331.
TEST(ntn_meas_asn1, orbital_ephemeris_encodes_correctly)
{
  OCUDU_TEST_REQUIREMENTS("CU-NTN-MOB-1");

  orbital_coordinates_t orb;
  orb.semi_major_axis = 6900000.0;
  orb.eccentricity    = 0.001;
  orb.periapsis       = 1.0;
  orb.longitude       = 2.0;
  orb.inclination     = 0.5;
  orb.mean_anomaly    = 3.0;

  rrc_meas_obj_nr meas_obj;
  meas_obj.cells_to_add_mod_list.push_back(make_cell(1));
  rrc_ntn_neighbour_cell_info info                         = make_ntn_info();
  info.ephemeris                                           = orb;
  meas_obj.cells_to_add_mod_list.back().ntn_neighbour_info = info;

  auto asn1_obj = meas_obj_nr_to_rrc_asn1(meas_obj);

  ASSERT_TRUE(asn1_obj.cells_to_add_mod_list_ext_v1800.is_present());
  const auto& ntn_asn1 = (*asn1_obj.cells_to_add_mod_list_ext_v1800)[0].ntn_neighbour_cell_info_r18;

  ASSERT_EQ(ntn_asn1.ephemeris_info_r18.type(), asn1::rrc_nr::ephemeris_info_r17_c::types::orbital_r17);
  const auto& orbit = ntn_asn1.ephemeris_info_r18.orbital_r17();
  EXPECT_EQ(orbit.semi_major_axis_r17, static_cast<uint64_t>((6900000.0 - 6500000) / 0.004249));
  EXPECT_EQ(orbit.eccentricity_r17, static_cast<uint32_t>(0.001 / 0.00000001431));
  EXPECT_EQ(orbit.inclination_r17, static_cast<int32_t>(0.5 / 0.00000002341));
  EXPECT_EQ(orbit.longitude_r17, static_cast<uint32_t>(2.0 / 0.00000002341));
  EXPECT_EQ(orbit.periapsis_r17, static_cast<uint32_t>(1.0 / 0.00000002341));
  EXPECT_EQ(orbit.mean_anomaly_r17, static_cast<uint32_t>(3.0 / 0.00000002341));
}

// ============================================================================
// ntn-PolarizationDL/UL-r17 in cells_to_add_mod_list_ext_v1710.
// ============================================================================

using ntn_pol = ntn_polarization_t;

/// The v1710 extension list must be parallel to cells_to_add_mod_list, with polarization filled only for cells that
/// carry it.
TEST(ntn_meas_asn1, polarization_ext_v1710_is_parallel_to_cells_to_add_mod_list)
{
  OCUDU_TEST_REQUIREMENTS("CU-NTN-MOB-1");

  rrc_meas_obj_nr meas_obj;
  meas_obj.cells_to_add_mod_list.push_back(make_cell(1));
  meas_obj.cells_to_add_mod_list.push_back(make_cell(2));
  meas_obj.cells_to_add_mod_list.push_back(make_cell(3));
  meas_obj.cells_to_add_mod_list[1].ntn_polarization =
      ntn_pol{ntn_pol::polarization_type::rhcp, ntn_pol::polarization_type::lhcp};

  auto asn1_obj = meas_obj_nr_to_rrc_asn1(meas_obj);
  ASSERT_TRUE(asn1_obj.cells_to_add_mod_list_ext_v1710.is_present());
  const auto& ext_list = *asn1_obj.cells_to_add_mod_list_ext_v1710;
  ASSERT_EQ(ext_list.size(), meas_obj.cells_to_add_mod_list.size());
  EXPECT_FALSE(ext_list[0].ntn_polarization_dl_r17_present);
  EXPECT_FALSE(ext_list[0].ntn_polarization_ul_r17_present);
  EXPECT_TRUE(ext_list[1].ntn_polarization_dl_r17_present);
  EXPECT_TRUE(ext_list[1].ntn_polarization_ul_r17_present);
  EXPECT_FALSE(ext_list[2].ntn_polarization_dl_r17_present);
  EXPECT_FALSE(ext_list[2].ntn_polarization_ul_r17_present);
}

/// DL and UL polarization enums map correctly.
TEST(ntn_meas_asn1, polarization_dl_ul_enums_encode_correctly)
{
  OCUDU_TEST_REQUIREMENTS("CU-NTN-MOB-1");

  using ext_v1710 = asn1::rrc_nr::cells_to_add_mod_ext_v1710_s;

  rrc_meas_obj_nr meas_obj;
  meas_obj.cells_to_add_mod_list.push_back(make_cell(1));
  meas_obj.cells_to_add_mod_list.back().ntn_polarization =
      ntn_pol{ntn_pol::polarization_type::lhcp, ntn_pol::polarization_type::linear};

  auto asn1_obj = meas_obj_nr_to_rrc_asn1(meas_obj);

  ASSERT_TRUE(asn1_obj.cells_to_add_mod_list_ext_v1710.is_present());
  const auto& ext_cell = (*asn1_obj.cells_to_add_mod_list_ext_v1710)[0];
  ASSERT_TRUE(ext_cell.ntn_polarization_dl_r17_present);
  ASSERT_TRUE(ext_cell.ntn_polarization_ul_r17_present);
  EXPECT_EQ(ext_cell.ntn_polarization_dl_r17.value, ext_v1710::ntn_polarization_dl_r17_opts::lhcp);
  EXPECT_EQ(ext_cell.ntn_polarization_ul_r17.value, ext_v1710::ntn_polarization_ul_r17_opts::linear);
}

/// A single configured direction (DL only) marks only that direction present.
TEST(ntn_meas_asn1, polarization_dl_only_leaves_ul_absent)
{
  OCUDU_TEST_REQUIREMENTS("CU-NTN-MOB-1");

  using ext_v1710 = asn1::rrc_nr::cells_to_add_mod_ext_v1710_s;

  rrc_meas_obj_nr meas_obj;
  meas_obj.cells_to_add_mod_list.push_back(make_cell(1));
  meas_obj.cells_to_add_mod_list.back().ntn_polarization = ntn_pol{ntn_pol::polarization_type::rhcp, std::nullopt};

  auto asn1_obj = meas_obj_nr_to_rrc_asn1(meas_obj);

  ASSERT_TRUE(asn1_obj.cells_to_add_mod_list_ext_v1710.is_present());
  const auto& ext_cell = (*asn1_obj.cells_to_add_mod_list_ext_v1710)[0];
  ASSERT_TRUE(ext_cell.ntn_polarization_dl_r17_present);
  EXPECT_FALSE(ext_cell.ntn_polarization_ul_r17_present);
  EXPECT_EQ(ext_cell.ntn_polarization_dl_r17.value, ext_v1710::ntn_polarization_dl_r17_opts::rhcp);
}
