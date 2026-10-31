// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/ran/positioning/common.h"
#include "ocudu/ran/positioning/trp_information_exchange.h"

namespace ocudu::ocucp {

/// \brief Convert the NG RAN access point position from ASN.1 to common type.
inline ng_ran_access_point_position_t
asn1_to_access_point_position(const asn1::f1ap::access_point_position_s& asn1_position)
{
  ng_ran_access_point_position_t position;

  if (asn1_position.latitude_sign == asn1::f1ap::access_point_position_s::latitude_sign_opts::options::north) {
    position.latitude_sign = latitude_sign_t::north;
  } else {
    position.latitude_sign = latitude_sign_t::south;
  }
  position.latitude  = asn1_position.latitude;
  position.longitude = asn1_position.longitude;
  if (asn1_position.direction_of_altitude ==
      asn1::f1ap::access_point_position_s::direction_of_altitude_opts::options::height) {
    position.direction_of_altitude = direction_of_altitude_t::height;
  } else {
    position.direction_of_altitude = direction_of_altitude_t::depth;
  }
  position.altitude                  = asn1_position.altitude;
  position.uncertainty_semi_major    = asn1_position.uncertainty_semi_major;
  position.uncertainty_semi_minor    = asn1_position.uncertainty_semi_minor;
  position.orientation_of_major_axis = asn1_position.orientation_of_major_axis;
  position.uncertainty_altitude      = asn1_position.uncertainty_altitude;
  position.confidence                = asn1_position.confidence;

  return position;
}

/// \brief Convert the NG RAN high accuracy access point position from ASN.1 to common type.
inline ng_ran_high_accuracy_access_point_position_t
asn1_to_ha_access_point_position(const asn1::f1ap::ngran_high_accuracy_access_point_position_s& asn1_position)
{
  ng_ran_high_accuracy_access_point_position_t position;

  position.latitude                  = asn1_position.latitude;
  position.longitude                 = asn1_position.longitude;
  position.altitude                  = asn1_position.altitude;
  position.uncertainty_semi_major    = asn1_position.uncertainty_semi_major;
  position.uncertainty_semi_minor    = asn1_position.uncertainty_semi_minor;
  position.orientation_of_major_axis = asn1_position.orientation_of_major_axis;
  position.horizontal_confidence     = asn1_position.horizontal_confidence;
  position.uncertainty_altitude      = asn1_position.uncertainty_altitude;
  position.vertical_confidence       = asn1_position.vertical_confidence;

  return position;
}

/// \brief Convert the relative geodetic location from ASN.1 to common type.
inline relative_geodetic_location_t
asn1_to_relative_geodetic_location(const asn1::f1ap::relative_geodetic_location_s& asn_location)
{
  relative_geodetic_location_t location;

  // Fill milli arc second units.
  if (asn_location.milli_arc_second_units ==
      asn1::f1ap::relative_geodetic_location_s::milli_arc_second_units_opts::options::zerodot03) {
    location.milli_arc_second_units = milli_arc_second_units_t::zerodot03;
  } else if (asn_location.milli_arc_second_units ==
             asn1::f1ap::relative_geodetic_location_s::milli_arc_second_units_opts::options::zerodot3) {
    location.milli_arc_second_units = milli_arc_second_units_t::zerodot3;
  } else {
    location.milli_arc_second_units = milli_arc_second_units_t::three;
  }

  // Fill height units.
  if (asn_location.height_units == asn1::f1ap::relative_geodetic_location_s::height_units_opts::options::mm) {
    location.height_units = height_units_t::mm;
  } else if (asn_location.height_units == asn1::f1ap::relative_geodetic_location_s::height_units_opts::options::cm) {
    location.height_units = height_units_t::cm;
  } else {
    location.height_units = height_units_t::m;
  }

  location.delta_latitude  = asn_location.delta_latitude;
  location.delta_longitude = asn_location.delta_longitude;
  location.delta_height    = asn_location.delta_height;

  // Fill location uncertainty.
  location.location_uncertainty.horizontal_uncertainty = asn_location.location_uncertainty.horizontal_uncertainty;
  location.location_uncertainty.horizontal_confidence  = asn_location.location_uncertainty.horizontal_confidence;
  location.location_uncertainty.vertical_uncertainty   = asn_location.location_uncertainty.vertical_uncertainty;
  location.location_uncertainty.vertical_confidence    = asn_location.location_uncertainty.vertical_confidence;

  return location;
}

/// \brief Convert the relative cartesian location from ASN.1 to common type.
inline relative_cartesian_location_t
asn1_to_relative_cartesian_location(const asn1::f1ap::relative_cartesian_location_s& asn1_location)
{
  relative_cartesian_location_t location;

  // Fill xyz unit.
  if (asn1_location.xy_zunit == asn1::f1ap::relative_cartesian_location_s::xy_zunit_opts::options::mm) {
    location.xyz_unit = xyz_unit_t::mm;
  } else if (asn1_location.xy_zunit == asn1::f1ap::relative_cartesian_location_s::xy_zunit_opts::cm) {
    location.xyz_unit = xyz_unit_t::cm;
  } else {
    location.xyz_unit = xyz_unit_t::dm;
  }

  location.xvalue = asn1_location.xvalue;
  location.yvalue = asn1_location.yvalue;
  location.zvalue = asn1_location.zvalue;

  // Fill location uncertainty.
  location.location_uncertainty.horizontal_uncertainty = asn1_location.location_uncertainty.horizontal_uncertainty;
  location.location_uncertainty.horizontal_confidence  = asn1_location.location_uncertainty.horizontal_confidence;
  location.location_uncertainty.vertical_uncertainty   = asn1_location.location_uncertainty.vertical_uncertainty;
  location.location_uncertainty.vertical_confidence    = asn1_location.location_uncertainty.vertical_confidence;

  return location;
}

/// \brief Convert the geographical coordinates from ASN.1 to common type.
inline geographical_coordinates_t
asn1_to_geographical_coordinates(const asn1::f1ap::geographical_coordinates_s& asn1_geo_coords)
{
  geographical_coordinates_t geo_coords;

  // Fill TRP position definition type.
  if (asn1_geo_coords.trp_position_definition_type.type() ==
      asn1::f1ap::trp_position_definition_type_c::types_opts::options::direct) {
    trp_position_direct_t direct;
    if (asn1_geo_coords.trp_position_definition_type.direct().accuracy.type() ==
        asn1::f1ap::trp_position_direct_accuracy_c::types_opts::options::trp_position) {
      direct.accuracy =
          asn1_to_access_point_position(asn1_geo_coords.trp_position_definition_type.direct().accuracy.trp_position());
    } else {
      direct.accuracy = asn1_to_ha_access_point_position(
          asn1_geo_coords.trp_position_definition_type.direct().accuracy.trph_aposition());
    }
    geo_coords.trp_position_definition_type = direct;
  } else {
    trp_position_refd_t refd;

    // Fill ref point.
    if (asn1_geo_coords.trp_position_definition_type.refd().ref_point.type() ==
        asn1::f1ap::ref_point_c::types_opts::options::coordinate_id) {
      refd.ref_point = asn1_geo_coords.trp_position_definition_type.refd().ref_point.coordinate_id();
    } else if (asn1_geo_coords.trp_position_definition_type.refd().ref_point.type() ==
               asn1::f1ap::ref_point_c::types_opts::options::ref_point_coordinate) {
      refd.ref_point = asn1_to_access_point_position(
          asn1_geo_coords.trp_position_definition_type.refd().ref_point.ref_point_coordinate());
    } else {
      refd.ref_point = asn1_to_ha_access_point_position(
          asn1_geo_coords.trp_position_definition_type.refd().ref_point.ref_point_coordinate_ha());
    }

    // Fill ref point type.
    if (asn1_geo_coords.trp_position_definition_type.refd().ref_point_type.type() ==
        asn1::f1ap::trp_ref_point_type_c::types_opts::options::trp_position_relative_geodetic) {
      refd.ref_point_type = asn1_to_relative_geodetic_location(
          asn1_geo_coords.trp_position_definition_type.refd().ref_point_type.trp_position_relative_geodetic());
    } else {
      refd.ref_point_type = asn1_to_relative_cartesian_location(
          asn1_geo_coords.trp_position_definition_type.refd().ref_point_type.trp_position_relative_cartesian());
    }

    geo_coords.trp_position_definition_type = refd;
  }

  // Fill DL PRS res coordinates.
  if (asn1_geo_coords.dl_prs_res_coordinates_present) {
    dl_prs_res_coordinates_t dl_prs_res_coords;

    for (const auto& asn1_dl_prs_res_set_arp : asn1_geo_coords.dl_prs_res_coordinates.listof_dl_prs_res_set_arp) {
      dl_prs_res_set_arp_t dl_prs_res_set_arp;

      // Fill DL PRS res set ID.
      dl_prs_res_set_arp.dl_prs_res_set_id = asn1_dl_prs_res_set_arp.dl_prs_res_set_id;

      // Fill DL PRS res set ARP location.
      if (asn1_dl_prs_res_set_arp.dl_prs_res_set_arp_location.type() ==
          asn1::f1ap::dl_prs_res_set_arp_location_c::types_opts::options::relative_geodetic_location) {
        dl_prs_res_set_arp.dl_prs_res_set_arp_location = asn1_to_relative_geodetic_location(
            asn1_dl_prs_res_set_arp.dl_prs_res_set_arp_location.relative_geodetic_location());
      } else {
        dl_prs_res_set_arp.dl_prs_res_set_arp_location = asn1_to_relative_cartesian_location(
            asn1_dl_prs_res_set_arp.dl_prs_res_set_arp_location.relative_cartesian_location());
      }

      // Fill list of DL PRS res set ARP.
      for (const auto& asn1_dl_prs_res_arp : asn1_dl_prs_res_set_arp.listof_dl_prs_res_arp) {
        dl_prs_res_arp_t dl_prs_res_arp;

        dl_prs_res_arp.dl_prs_res_id = asn1_dl_prs_res_arp.dl_prs_res_id;

        if (asn1_dl_prs_res_arp.dl_prs_res_arp_location.type() ==
            asn1::f1ap::dl_prs_res_arp_location_c::types_opts::options::relative_geodetic_location) {
          dl_prs_res_arp.dl_prs_res_arp_location = asn1_to_relative_geodetic_location(
              asn1_dl_prs_res_arp.dl_prs_res_arp_location.relative_geodetic_location());
        } else {
          dl_prs_res_arp.dl_prs_res_arp_location = asn1_to_relative_cartesian_location(
              asn1_dl_prs_res_arp.dl_prs_res_arp_location.relative_cartesian_location());
        }

        dl_prs_res_set_arp.listof_dl_prs_res_arp.push_back(dl_prs_res_arp);
      }

      dl_prs_res_coords.listof_dl_prs_res_set_arp.push_back(dl_prs_res_set_arp);
    }

    geo_coords.dl_prs_res_coordinates = dl_prs_res_coords;
  }

  // Fill IE exts.
  if (asn1_geo_coords.ie_exts_present and asn1_geo_coords.ie_exts.arp_location_info_present) {
    for (const auto& asn1_arp_location_info_item : asn1_geo_coords.ie_exts.arp_location_info) {
      arp_location_info_item_t arp_location_info_item;
      arp_location_info_item.arp_id = asn1_arp_location_info_item.arp_id;

      if (asn1_arp_location_info_item.arp_location_type.type() ==
          asn1::f1ap::arp_location_type_c::types_opts::options::arp_position_relative_geodetic) {
        arp_location_info_item.arp_location_type = asn1_to_relative_geodetic_location(
            asn1_arp_location_info_item.arp_location_type.arp_position_relative_geodetic());
      } else {
        arp_location_info_item.arp_location_type = asn1_to_relative_cartesian_location(
            asn1_arp_location_info_item.arp_location_type.arp_position_relative_cartesian());
      }

      geo_coords.arp_location_info.push_back(arp_location_info_item);
    }
  }

  return geo_coords;
}

} // namespace ocudu::ocucp
