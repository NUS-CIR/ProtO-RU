// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "asn1_ntn_config_helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/lpp/reference_location.h"
#include "ocudu/support/error_handling.h"
#include <cmath>

using namespace ocudu;
using namespace asn1::rrc_nr;

/// Offset between 1900-01-01 (3GPP ASN.1 epoch) and 1970-01-01 (Unix epoch), in milliseconds.
static constexpr int64_t MS_BETWEEN_1900_AND_1970 = 2208988800LL * 1000LL;

ntn_cfg_r17_s ocudu::odu::make_asn1_rrc_cell_ntn_cfg(const ntn_config& ntn_cfg)
{
  ntn_cfg_r17_s out_ntn_cfg;

  // Epoch Time. Exempt from SI change notification or valueTag modification in SIB1.
  if (ntn_cfg.epoch_time.has_value()) {
    out_ntn_cfg.epoch_time_r17_present          = true;
    out_ntn_cfg.epoch_time_r17.sfn_r17          = ntn_cfg.epoch_time->sfn;
    out_ntn_cfg.epoch_time_r17.sub_frame_nr_r17 = ntn_cfg.epoch_time->subframe_number;
  }

  // Validity duration for UL sync assistance info in seconds.
  // Exempt from SI change notification or valueTag modification in SIB1.
  if (ntn_cfg.ntn_ul_sync_validity_dur.has_value()) {
    out_ntn_cfg.ntn_ul_sync_validity_dur_r17_present = true;
    switch (*ntn_cfg.ntn_ul_sync_validity_dur) {
      case 5:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s5;
        break;
      case 10:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s10;
        break;
      case 15:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s15;
        break;
      case 20:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s20;
        break;
      case 25:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s25;
        break;
      case 30:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s30;
        break;
      case 35:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s35;
        break;
      case 40:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s40;
        break;
      case 45:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s45;
        break;
      case 50:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s50;
        break;
      case 55:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s55;
        break;
      case 60:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s60;
        break;
      case 120:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s120;
        break;
      case 180:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s180;
        break;
      case 240:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s240;
        break;
      case 900:
        out_ntn_cfg.ntn_ul_sync_validity_dur_r17.value = ntn_cfg_r17_s::ntn_ul_sync_validity_dur_r17_opts::s900;
        break;
      default:
        report_fatal_error("Invalid ntn_ul_sync_validity_dur {}.", *ntn_cfg.ntn_ul_sync_validity_dur);
    }
  }

  // Cell-specific-k-offset.
  if (ntn_cfg.cell_specific_koffset.has_value()) {
    out_ntn_cfg.cell_specific_koffset_r17_present = true;
    out_ntn_cfg.cell_specific_koffset_r17         = ntn_cfg.cell_specific_koffset->count();
  }

  // K-mac.
  if (ntn_cfg.k_mac.has_value()) {
    out_ntn_cfg.kmac_r17_present = true;
    out_ntn_cfg.kmac_r17         = ntn_cfg.k_mac->count();
  }

  // TA-Info. Exempt from SI change notification or valueTag modification in SIB1.
  if (ntn_cfg.ta_info.has_value()) {
    out_ntn_cfg.ta_info_r17_present             = true;
    ta_info_r17_s& ta_info                      = out_ntn_cfg.ta_info_r17;
    ta_info.ta_common_drift_r17_present         = true;
    ta_info.ta_common_drift_variant_r17_present = true;
    ta_info.ta_common_r17                       = static_cast<uint32_t>(ntn_cfg.ta_info->ta_common / 0.004072);
    ta_info.ta_common_drift_r17                 = static_cast<int32_t>(ntn_cfg.ta_info->ta_common_drift / 0.0002);
    ta_info.ta_common_drift_variant_r17 = static_cast<uint16_t>(ntn_cfg.ta_info->ta_common_drift_variant / 0.00002);
    if (ntn_cfg.ta_info->ta_common_offset) {
      ta_info.ta_common_r17 += static_cast<int32_t>(*ntn_cfg.ta_info->ta_common_offset / 0.004072);
    }
  }

  if (ntn_cfg.polarization.has_value()) {
    const auto& polarization = *ntn_cfg.polarization;
    if (polarization.dl.has_value()) {
      out_ntn_cfg.ntn_polarization_dl_r17_present = true;
      if (*polarization.dl == ntn_polarization_t::polarization_type::lhcp) {
        out_ntn_cfg.ntn_polarization_dl_r17 = ntn_cfg_r17_s::ntn_polarization_dl_r17_opts::lhcp;
      } else if (*polarization.dl == ntn_polarization_t::polarization_type::rhcp) {
        out_ntn_cfg.ntn_polarization_dl_r17 = ntn_cfg_r17_s::ntn_polarization_dl_r17_opts::rhcp;
      } else {
        out_ntn_cfg.ntn_polarization_dl_r17 = ntn_cfg_r17_s::ntn_polarization_dl_r17_opts::linear;
      }
    }
    if (ntn_cfg.polarization->ul.has_value()) {
      out_ntn_cfg.ntn_polarization_ul_r17_present = true;
      if (*polarization.ul == ntn_polarization_t::polarization_type::lhcp) {
        out_ntn_cfg.ntn_polarization_ul_r17 = ntn_cfg_r17_s::ntn_polarization_ul_r17_opts::lhcp;
      } else if (*polarization.ul == ntn_polarization_t::polarization_type::rhcp) {
        out_ntn_cfg.ntn_polarization_ul_r17 = ntn_cfg_r17_s::ntn_polarization_ul_r17_opts::rhcp;
      } else {
        out_ntn_cfg.ntn_polarization_ul_r17 = ntn_cfg_r17_s::ntn_polarization_ul_r17_opts::linear;
      }
    }
  }

  // Ephemeris info. Exempt from SI change notification or valueTag modification in SIB1.
  if (ntn_cfg.ephemeris_info.has_value()) {
    out_ntn_cfg.ephemeris_info_r17_present = true;
    if (const auto* pos_vel = std::get_if<ecef_coordinates_t>(&*ntn_cfg.ephemeris_info)) {
      out_ntn_cfg.ephemeris_info_r17.set_position_velocity_r17();
      position_velocity_r17_s& rv = out_ntn_cfg.ephemeris_info_r17.position_velocity_r17();
      rv.position_x_r17           = static_cast<int32_t>(pos_vel->position_x / 1.3);
      rv.position_y_r17           = static_cast<int32_t>(pos_vel->position_y / 1.3);
      rv.position_z_r17           = static_cast<int32_t>(pos_vel->position_z / 1.3);
      rv.velocity_vx_r17          = static_cast<int32_t>(pos_vel->velocity_vx / 0.06);
      rv.velocity_vy_r17          = static_cast<int32_t>(pos_vel->velocity_vy / 0.06);
      rv.velocity_vz_r17          = static_cast<int32_t>(pos_vel->velocity_vz / 0.06);
    } else {
      const auto& orbital_elem = std::get<orbital_coordinates_t>(*ntn_cfg.ephemeris_info);
      out_ntn_cfg.ephemeris_info_r17.set_orbital_r17();
      orbital_r17_s& orbit      = out_ntn_cfg.ephemeris_info_r17.orbital_r17();
      orbit.semi_major_axis_r17 = static_cast<uint64_t>((orbital_elem.semi_major_axis - 6500000) / 0.004249);
      orbit.eccentricity_r17    = static_cast<uint32_t>(orbital_elem.eccentricity / 0.00000001431);
      // Per TS 38.331, inclination-r17 is signed: positive field maps to [0, pi/2], negative field maps to [pi/2, pi]
      // via actual value = field value * step + pi.
      const double incl =
          (orbital_elem.inclination <= M_PI / 2) ? orbital_elem.inclination : orbital_elem.inclination - M_PI;
      orbit.inclination_r17  = static_cast<int32_t>(incl / 0.00000002341);
      orbit.longitude_r17    = static_cast<uint32_t>(orbital_elem.longitude / 0.00000002341);
      orbit.periapsis_r17    = static_cast<uint32_t>(orbital_elem.periapsis / 0.00000002341);
      orbit.mean_anomaly_r17 = static_cast<uint32_t>(orbital_elem.mean_anomaly / 0.00000002341);
    }
  }

  // TA-Report.
  if (ntn_cfg.ta_report.has_value()) {
    out_ntn_cfg.ta_report_r17_present = *ntn_cfg.ta_report;
  }

  return out_ntn_cfg;
}

static byte_buffer pack_asn1_ellipsoid_point(const geodetic_coordinates_t& location)
{
  return lpp::pack_reference_location(reference_location{location.latitude, location.longitude});
}

/// \brief Encodes a neighbor NTN cell configuration to ASN.1 format.
/// \param[in] neighbor The neighbor cell parameters including optional NTN config.
/// \return The filled ASN.1 NTN-NeighCellConfig-r17 structure.
/// \remark Per TS 38.331, maximum 4 neighbor cells supported in SIB19.
static asn1::rrc_nr::ntn_neigh_cell_cfg_r17_s make_asn1_ntn_neighbor_cell(const neighbor_ntn_cell& neighbor)
{
  asn1::rrc_nr::ntn_neigh_cell_cfg_r17_s neigh_asn1;

  if (neighbor.ntn_cfg.has_value()) {
    neigh_asn1.ntn_cfg_r17_present = true;
    neigh_asn1.ntn_cfg_r17         = odu::make_asn1_rrc_cell_ntn_cfg(*neighbor.ntn_cfg);
  }

  if (neighbor.carrier_freq.has_value()) {
    neigh_asn1.carrier_freq_r17_present = true;
    neigh_asn1.carrier_freq_r17         = neighbor.carrier_freq->value();
  }

  if (neighbor.phys_cell_id.has_value()) {
    neigh_asn1.pci_r17_present = true;
    neigh_asn1.pci_r17         = *neighbor.phys_cell_id;
  }

  return neigh_asn1;
}

sib19_r17_s ocudu::odu::make_asn1_rrc_cell_sib19(const sib19_info& sib19_params)
{
  sib19_r17_s sib19;

  // Distance Threshold.
  sib19.distance_thresh_r17_present = false;

  // T-Service, currently not supported.
  sib19.t_service_r17_present = false;

  // NTN-Config
  if (sib19_params.ntn_cfg.has_value()) {
    sib19.ntn_cfg_r17_present = true;
    sib19.ntn_cfg_r17         = make_asn1_rrc_cell_ntn_cfg(*sib19_params.ntn_cfg);
  }

  // T-Service.
  if (sib19_params.t_service.has_value()) {
    sib19.t_service_r17_present = true;
    // The field indicates a time in multiples of 10 ms after 00:00:00 on Gregorian calendar date 1 January, 1900
    // (midnight between Sunday, December 31, 1899 and Monday, January 1, 1900).
    // Convert from system_clock::time_point (Unix epoch 1970-01-01) to ASN.1 format (1900-01-01).
    // Get milliseconds since Unix epoch.
    auto ms_since_epoch =
        std::chrono::duration_cast<std::chrono::milliseconds>(sib19_params.t_service->time_since_epoch()).count();

    // Convert to multiples of 10ms since 1900-01-01.
    sib19.t_service_r17 = (ms_since_epoch + MS_BETWEEN_1900_AND_1970) / 10;
  }

  // Reference Location.
  if (sib19_params.ref_location.has_value()) {
    auto ref_point_buff = pack_asn1_ellipsoid_point(*sib19_params.ref_location);
    if (!sib19.ref_location_r17.resize(ref_point_buff.length())) {
      ocudu_assertion_failure("Failed to pack reference location in SIB19.");
    }
    std::copy(ref_point_buff.begin(), ref_point_buff.end(), sib19.ref_location_r17.begin());
  }

  // Distance Threshold.
  if (sib19_params.distance_thres.has_value()) {
    sib19.distance_thresh_r17_present = true;
    sib19.distance_thresh_r17         = static_cast<uint16_t>(*sib19_params.distance_thres / 50);
  }

  // NTN Neighbor Cell Configuration List.
  if (!sib19_params.ncells.empty()) {
    // Validate total size constraint: base list (1..4) + extension list (1..4) = max 8.
    ocudu_assert(sib19_params.ncells.size() <= MAX_NOF_NTN_NEIGHBORS,
                 "NTN neighbor cell list size {} exceeds maximum {}",
                 sib19_params.ncells.size(),
                 MAX_NOF_NTN_NEIGHBORS);

    // Pack first MAX_NOF_NTN_NEIGHBORS_BASE_LIST neighbors into base list
    size_t base_count = std::min(sib19_params.ncells.size(), MAX_NOF_NTN_NEIGHBORS_BASE_LIST);
    for (size_t i = 0; i != base_count; ++i) {
      sib19.ntn_neigh_cell_cfg_list_r17.push_back(make_asn1_ntn_neighbor_cell(sib19_params.ncells[i]));
    }

    // Pack remaining neighbors into extension list (R17 extension).
    if (sib19_params.ncells.size() > MAX_NOF_NTN_NEIGHBORS_BASE_LIST) {
      sib19.ntn_neigh_cell_cfg_list_ext_v1720.set_present();
      for (size_t i = MAX_NOF_NTN_NEIGHBORS_BASE_LIST, e = sib19_params.ncells.size(); i != e; ++i) {
        sib19.ntn_neigh_cell_cfg_list_ext_v1720->push_back(make_asn1_ntn_neighbor_cell(sib19_params.ncells[i]));
      }
    }
  }

  // Moving Reference Location (R18 extension).
  if (sib19_params.moving_ref_location.has_value()) {
    auto moving_ref_buff = pack_asn1_ellipsoid_point(*sib19_params.moving_ref_location);
    if (!sib19.moving_ref_location_r18.resize(moving_ref_buff.length())) {
      ocudu_assertion_failure("Failed to pack moving reference location in SIB19.");
    }
    std::copy(moving_ref_buff.begin(), moving_ref_buff.end(), sib19.moving_ref_location_r18.begin());
  }

  // Coverage Enhancements (R18 extension).
  if (sib19_params.coverage_enhancements.has_value()) {
    sib19.ntn_cov_enh_r18.set_present();
    auto&       cov_enh        = *sib19.ntn_cov_enh_r18;
    const auto& cov_enh_params = *sib19_params.coverage_enhancements;

    unsigned nof_rep = cov_enh_params.nof_msg4_harq_ack_rep;
    ocudu_assert(nof_rep < 16, "Invalid nof_msg4_harq_ack_repeats value {}, must be 0-15", nof_rep);
    cov_enh.nof_msg4_harq_ack_repeats_r18.from_number(nof_rep);

    // Optional field: RSRP threshold (0-127).
    if (cov_enh_params.rsrp_thres_msg4_harq_ack.has_value()) {
      cov_enh.rsrp_thres_msg4_harq_ack_r18_present = true;
      unsigned rsrp_val                            = *cov_enh_params.rsrp_thres_msg4_harq_ack;
      ocudu_assert(rsrp_val <= 127, "Invalid rsrp_thres_msg4_harq_ack value {}, must be 0-127", rsrp_val);
      cov_enh.rsrp_thres_msg4_harq_ack_r18 = static_cast<uint8_t>(rsrp_val);
    }
  }

  // Satellite Switch with Resynchronization (R18 extension).
  if (sib19_params.sat_switch_with_resync.has_value()) {
    const auto& sat_switch_with_resync = *sib19_params.sat_switch_with_resync;
    sib19.sat_switch_with_re_sync_r18.set_present();
    auto& sat_switch = *sib19.sat_switch_with_re_sync_r18;

    // Mandatory nested NTN config.
    sat_switch.ntn_cfg_r18 = make_asn1_rrc_cell_ntn_cfg(sat_switch_with_resync.ntn_cfg);

    // Optional t_service_start (convert time_point to multiples of 10ms since 1900-01-01).
    if (sat_switch_with_resync.t_service_start.has_value()) {
      sat_switch.t_service_start_r18_present = true;
      auto ms_since_epoch                    = std::chrono::duration_cast<std::chrono::milliseconds>(
                                sat_switch_with_resync.t_service_start->time_since_epoch())
                                .count();
      sat_switch.t_service_start_r18 = (ms_since_epoch + MS_BETWEEN_1900_AND_1970) / 10;
    }

    // Optional ssb_time_offset (0-159 subframes).
    if (sat_switch_with_resync.ssb_time_offset_sf.has_value()) {
      sat_switch.ssb_time_offset_r18_present = true;
      uint8_t offset                         = sat_switch_with_resync.ssb_time_offset_sf->value();
      ocudu_assert(offset <= 159, "Invalid ssb_time_offset value {}, must be 0-159", offset);
      sat_switch.ssb_time_offset_r18 = offset;
    }
  }

  return sib19;
}
