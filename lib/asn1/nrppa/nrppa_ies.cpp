// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/nrppa/nrppa_ies.h"

using namespace asn1;
using namespace asn1::nrppa;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// TriggeringMessage ::= ENUMERATED
const char* trigger_msg_opts::to_string() const
{
  static const char* names[] = {"initiating-message", "successful-outcome", "unsuccessful-outcome"};
  return convert_enum_idx(names, 3, value, "trigger_msg_e");
}

// LocationUncertainty ::= SEQUENCE
OCUDUASN_CODE location_uncertainty_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, horizontal_uncertainty, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_integer(bref, horizontal_confidence, (uint8_t)0u, (uint8_t)100u, false, true));
  HANDLE_CODE(pack_integer(bref, vertical_uncertainty, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_integer(bref, vertical_confidence, (uint8_t)0u, (uint8_t)100u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_uncertainty_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(horizontal_uncertainty, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_integer(horizontal_confidence, bref, (uint8_t)0u, (uint8_t)100u, false, true));
  HANDLE_CODE(unpack_integer(vertical_uncertainty, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_integer(vertical_confidence, bref, (uint8_t)0u, (uint8_t)100u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void location_uncertainty_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("horizontalUncertainty", horizontal_uncertainty);
  j.write_int("horizontalConfidence", horizontal_confidence);
  j.write_int("verticalUncertainty", vertical_uncertainty);
  j.write_int("verticalConfidence", vertical_confidence);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RelativeGeodeticLocation ::= SEQUENCE
OCUDUASN_CODE relative_geodetic_location_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(milli_arc_second_units.pack(bref));
  HANDLE_CODE(height_units.pack(bref));
  HANDLE_CODE(pack_integer(bref, delta_latitude, (int16_t)-1024, (int16_t)1023, false, true));
  HANDLE_CODE(pack_integer(bref, delta_longitude, (int16_t)-1024, (int16_t)1023, false, true));
  HANDLE_CODE(pack_integer(bref, delta_height, (int16_t)-1024, (int16_t)1023, false, true));
  HANDLE_CODE(location_uncertainty.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE relative_geodetic_location_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(milli_arc_second_units.unpack(bref));
  HANDLE_CODE(height_units.unpack(bref));
  HANDLE_CODE(unpack_integer(delta_latitude, bref, (int16_t)-1024, (int16_t)1023, false, true));
  HANDLE_CODE(unpack_integer(delta_longitude, bref, (int16_t)-1024, (int16_t)1023, false, true));
  HANDLE_CODE(unpack_integer(delta_height, bref, (int16_t)-1024, (int16_t)1023, false, true));
  HANDLE_CODE(location_uncertainty.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void relative_geodetic_location_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("milli-Arc-SecondUnits", milli_arc_second_units.to_string());
  j.write_str("heightUnits", height_units.to_string());
  j.write_int("deltaLatitude", delta_latitude);
  j.write_int("deltaLongitude", delta_longitude);
  j.write_int("deltaHeight", delta_height);
  j.write_fieldname("locationUncertainty");
  location_uncertainty.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* relative_geodetic_location_s::milli_arc_second_units_opts::to_string() const
{
  static const char* names[] = {"zerodot03", "zerodot3", "three"};
  return convert_enum_idx(names, 3, value, "relative_geodetic_location_s::milli_arc_second_units_e_");
}

const char* relative_geodetic_location_s::height_units_opts::to_string() const
{
  static const char* names[] = {"mm", "cm", "m"};
  return convert_enum_idx(names, 3, value, "relative_geodetic_location_s::height_units_e_");
}

// RelativeCartesianLocation ::= SEQUENCE
OCUDUASN_CODE relative_cartesian_location_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(xy_zunit.pack(bref));
  HANDLE_CODE(pack_integer(bref, xvalue, (int32_t)-65536, (int32_t)65535, false, true));
  HANDLE_CODE(pack_integer(bref, yvalue, (int32_t)-65536, (int32_t)65535, false, true));
  HANDLE_CODE(pack_integer(bref, zvalue, (int32_t)-32768, (int32_t)32767, false, true));
  HANDLE_CODE(location_uncertainty.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE relative_cartesian_location_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(xy_zunit.unpack(bref));
  HANDLE_CODE(unpack_integer(xvalue, bref, (int32_t)-65536, (int32_t)65535, false, true));
  HANDLE_CODE(unpack_integer(yvalue, bref, (int32_t)-65536, (int32_t)65535, false, true));
  HANDLE_CODE(unpack_integer(zvalue, bref, (int32_t)-32768, (int32_t)32767, false, true));
  HANDLE_CODE(location_uncertainty.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void relative_cartesian_location_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("xYZunit", xy_zunit.to_string());
  j.write_int("xvalue", xvalue);
  j.write_int("yvalue", yvalue);
  j.write_int("zvalue", zvalue);
  j.write_fieldname("locationUncertainty");
  location_uncertainty.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* relative_cartesian_location_s::xy_zunit_opts::to_string() const
{
  static const char* names[] = {"mm", "cm", "dm"};
  return convert_enum_idx(names, 3, value, "relative_cartesian_location_s::xy_zunit_e_");
}

// ARPLocationType ::= CHOICE
void arp_location_type_c::destroy_()
{
  switch (type_) {
    case types::arp_position_relative_geodetic:
      c.destroy<relative_geodetic_location_s>();
      break;
    case types::arp_position_relative_cartesian:
      c.destroy<relative_cartesian_location_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void arp_location_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::arp_position_relative_geodetic:
      c.init<relative_geodetic_location_s>();
      break;
    case types::arp_position_relative_cartesian:
      c.init<relative_cartesian_location_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "arp_location_type_c");
  }
}
arp_location_type_c::arp_location_type_c(const arp_location_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::arp_position_relative_geodetic:
      c.init(other.c.get<relative_geodetic_location_s>());
      break;
    case types::arp_position_relative_cartesian:
      c.init(other.c.get<relative_cartesian_location_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "arp_location_type_c");
  }
}
arp_location_type_c& arp_location_type_c::operator=(const arp_location_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::arp_position_relative_geodetic:
      c.set(other.c.get<relative_geodetic_location_s>());
      break;
    case types::arp_position_relative_cartesian:
      c.set(other.c.get<relative_cartesian_location_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "arp_location_type_c");
  }

  return *this;
}
relative_geodetic_location_s& arp_location_type_c::set_arp_position_relative_geodetic()
{
  set(types::arp_position_relative_geodetic);
  return c.get<relative_geodetic_location_s>();
}
relative_cartesian_location_s& arp_location_type_c::set_arp_position_relative_cartesian()
{
  set(types::arp_position_relative_cartesian);
  return c.get<relative_cartesian_location_s>();
}
protocol_ie_single_container_s<arp_location_type_ext_ies_o>& arp_location_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>();
}
void arp_location_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::arp_position_relative_geodetic:
      j.write_fieldname("aRPPositionRelativeGeodetic");
      c.get<relative_geodetic_location_s>().to_json(j);
      break;
    case types::arp_position_relative_cartesian:
      j.write_fieldname("aRPPositionRelativeCartesian");
      c.get<relative_cartesian_location_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "arp_location_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE arp_location_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::arp_position_relative_geodetic:
      HANDLE_CODE(c.get<relative_geodetic_location_s>().pack(bref));
      break;
    case types::arp_position_relative_cartesian:
      HANDLE_CODE(c.get<relative_cartesian_location_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "arp_location_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE arp_location_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::arp_position_relative_geodetic:
      HANDLE_CODE(c.get<relative_geodetic_location_s>().unpack(bref));
      break;
    case types::arp_position_relative_cartesian:
      HANDLE_CODE(c.get<relative_cartesian_location_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "arp_location_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* arp_location_type_c::types_opts::to_string() const
{
  static const char* names[] = {"aRPPositionRelativeGeodetic", "aRPPositionRelativeCartesian", "choice-extension"};
  return convert_enum_idx(names, 3, value, "arp_location_type_c::types");
}

// ARPLocationInformation-Item ::= SEQUENCE
OCUDUASN_CODE arp_location_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, arp_id, (uint8_t)1u, (uint8_t)16u, true, true));
  HANDLE_CODE(arp_location_type.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE arp_location_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(arp_id, bref, (uint8_t)1u, (uint8_t)16u, true, true));
  HANDLE_CODE(arp_location_type.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void arp_location_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("aRP-ID", arp_id);
  j.write_fieldname("aRPLocationType");
  arp_location_type.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SCS-SpecificCarrier ::= SEQUENCE
OCUDUASN_CODE scs_specific_carrier_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, offset_to_carrier, (uint16_t)0u, (uint16_t)2199u, true, true));
  HANDLE_CODE(subcarrier_spacing.pack(bref));
  HANDLE_CODE(pack_integer(bref, carrier_bw, (uint16_t)1u, (uint16_t)275u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE scs_specific_carrier_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(offset_to_carrier, bref, (uint16_t)0u, (uint16_t)2199u, true, true));
  HANDLE_CODE(subcarrier_spacing.unpack(bref));
  HANDLE_CODE(unpack_integer(carrier_bw, bref, (uint16_t)1u, (uint16_t)275u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void scs_specific_carrier_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("offsetToCarrier", offset_to_carrier);
  j.write_str("subcarrierSpacing", subcarrier_spacing.to_string());
  j.write_int("carrierBandwidth", carrier_bw);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* scs_specific_carrier_s::subcarrier_spacing_opts::to_string() const
{
  static const char* names[] = {"kHz15", "kHz30", "kHz60", "kHz120", "kHz480", "kHz960"};
  return convert_enum_idx(names, 6, value, "scs_specific_carrier_s::subcarrier_spacing_e_");
}
uint16_t scs_specific_carrier_s::subcarrier_spacing_opts::to_number() const
{
  static const uint16_t numbers[] = {15, 30, 60, 120, 480, 960};
  return map_enum_number(numbers, 6, value, "scs_specific_carrier_s::subcarrier_spacing_e_");
}

// Combined-PosSRSResourceSet-Item ::= SEQUENCE
OCUDUASN_CODE combined_pos_srs_res_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pci_nr_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, point_a, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (pci_nr_present) {
    HANDLE_CODE(pack_integer(bref, pci_nr, (uint16_t)0u, (uint16_t)1007u, false, true));
  }
  HANDLE_CODE(pack_integer(bref, possrs_res_set_id, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(scs_specific_carrier.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE combined_pos_srs_res_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(pci_nr_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(point_a, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (pci_nr_present) {
    HANDLE_CODE(unpack_integer(pci_nr, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  }
  HANDLE_CODE(unpack_integer(possrs_res_set_id, bref, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(scs_specific_carrier.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void combined_pos_srs_res_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pointA", point_a);
  if (pci_nr_present) {
    j.write_int("pCI-NR", pci_nr);
  }
  j.write_int("possrsResourceSetID", possrs_res_set_id);
  j.write_fieldname("scs-specificCarrier");
  scs_specific_carrier.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PosSRSResourceSet-Aggregation-Item ::= SEQUENCE
OCUDUASN_CODE pos_srs_res_set_aggregation_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, combined_pos_srs_res_set_list, 2, 3, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_srs_res_set_aggregation_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(combined_pos_srs_res_set_list, bref, 2, 3, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_srs_res_set_aggregation_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("combined-posSRSResourceSet-List");
  for (const auto& e1 : combined_pos_srs_res_set_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// AbortTransmission-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t abort_tx_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {122};
  return map_enum_number(names, 1, idx, "id");
}
bool abort_tx_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 122 == id;
}
crit_e abort_tx_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 122) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
abort_tx_ext_ies_o::value_c abort_tx_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 122) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e abort_tx_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 122) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void abort_tx_ext_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_array("");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
}
OCUDUASN_CODE abort_tx_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 32, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE abort_tx_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 32, true));
  return OCUDUASN_SUCCESS;
}

const char* abort_tx_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"PosSRSResourceSet-Aggregation-List"};
  return convert_enum_idx(names, 1, value, "abort_tx_ext_ies_o::value_c::types");
}

// AbortTransmission ::= CHOICE
void abort_tx_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<abort_tx_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void abort_tx_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::deactiv_srs_res_set_id:
      break;
    case types::release_all:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<abort_tx_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "abort_tx_c");
  }
}
abort_tx_c::abort_tx_c(const abort_tx_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::deactiv_srs_res_set_id:
      c.init(other.c.get<uint8_t>());
      break;
    case types::release_all:
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "abort_tx_c");
  }
}
abort_tx_c& abort_tx_c::operator=(const abort_tx_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::deactiv_srs_res_set_id:
      c.set(other.c.get<uint8_t>());
      break;
    case types::release_all:
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "abort_tx_c");
  }

  return *this;
}
uint8_t& abort_tx_c::set_deactiv_srs_res_set_id()
{
  set(types::deactiv_srs_res_set_id);
  return c.get<uint8_t>();
}
void abort_tx_c::set_release_all()
{
  set(types::release_all);
}
protocol_ie_single_container_s<abort_tx_ext_ies_o>& abort_tx_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>();
}
void abort_tx_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::deactiv_srs_res_set_id:
      j.write_int("deactivateSRSResourceSetID", c.get<uint8_t>());
      break;
    case types::release_all:
      j.write_null("releaseALL");
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "abort_tx_c");
  }
  j.end_obj();
}
OCUDUASN_CODE abort_tx_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::deactiv_srs_res_set_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)15u, true, true));
      break;
    case types::release_all:
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "abort_tx_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE abort_tx_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::deactiv_srs_res_set_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)15u, true, true));
      break;
    case types::release_all:
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "abort_tx_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* abort_tx_c::types_opts::to_string() const
{
  static const char* names[] = {"deactivateSRSResourceSetID", "releaseALL", "choice-extension"};
  return convert_enum_idx(names, 3, value, "abort_tx_c::types");
}

// TransmissionCombn8 ::= SEQUENCE
OCUDUASN_CODE tx_combn8_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, comb_offset_n8, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(pack_integer(bref, cyclic_shift_n8, (uint8_t)0u, (uint8_t)5u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tx_combn8_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(comb_offset_n8, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(unpack_integer(cyclic_shift_n8, bref, (uint8_t)0u, (uint8_t)5u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void tx_combn8_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("combOffset-n8", comb_offset_n8);
  j.write_int("cyclicShift-n8", cyclic_shift_n8);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TransmissionComb-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t tx_comb_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {111};
  return map_enum_number(names, 1, idx, "id");
}
bool tx_comb_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 111 == id;
}
crit_e tx_comb_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 111) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
tx_comb_ext_ies_o::value_c tx_comb_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 111) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e tx_comb_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 111) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void tx_comb_ext_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE tx_comb_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tx_comb_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* tx_comb_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TransmissionCombn8"};
  return convert_enum_idx(names, 1, value, "tx_comb_ext_ies_o::value_c::types");
}
uint8_t tx_comb_ext_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {8};
  return map_enum_number(numbers, 1, value, "tx_comb_ext_ies_o::value_c::types");
}

// TransmissionComb ::= CHOICE
void tx_comb_c::destroy_()
{
  switch (type_) {
    case types::n2:
      c.destroy<n2_s_>();
      break;
    case types::n4:
      c.destroy<n4_s_>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<tx_comb_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void tx_comb_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::n2:
      c.init<n2_s_>();
      break;
    case types::n4:
      c.init<n4_s_>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<tx_comb_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_c");
  }
}
tx_comb_c::tx_comb_c(const tx_comb_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::n2:
      c.init(other.c.get<n2_s_>());
      break;
    case types::n4:
      c.init(other.c.get<n4_s_>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<tx_comb_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_c");
  }
}
tx_comb_c& tx_comb_c::operator=(const tx_comb_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::n2:
      c.set(other.c.get<n2_s_>());
      break;
    case types::n4:
      c.set(other.c.get<n4_s_>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<tx_comb_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_c");
  }

  return *this;
}
tx_comb_c::n2_s_& tx_comb_c::set_n2()
{
  set(types::n2);
  return c.get<n2_s_>();
}
tx_comb_c::n4_s_& tx_comb_c::set_n4()
{
  set(types::n4);
  return c.get<n4_s_>();
}
protocol_ie_single_container_s<tx_comb_ext_ies_o>& tx_comb_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<tx_comb_ext_ies_o>>();
}
void tx_comb_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::n2:
      j.write_fieldname("n2");
      j.start_obj();
      j.write_int("combOffset-n2", c.get<n2_s_>().comb_offset_n2);
      j.write_int("cyclicShift-n2", c.get<n2_s_>().cyclic_shift_n2);
      j.end_obj();
      break;
    case types::n4:
      j.write_fieldname("n4");
      j.start_obj();
      j.write_int("combOffset-n4", c.get<n4_s_>().comb_offset_n4);
      j.write_int("cyclicShift-n4", c.get<n4_s_>().cyclic_shift_n4);
      j.end_obj();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<tx_comb_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_c");
  }
  j.end_obj();
}
OCUDUASN_CODE tx_comb_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::n2:
      HANDLE_CODE(pack_integer(bref, c.get<n2_s_>().comb_offset_n2, (uint8_t)0u, (uint8_t)1u, false, true));
      HANDLE_CODE(pack_integer(bref, c.get<n2_s_>().cyclic_shift_n2, (uint8_t)0u, (uint8_t)7u, false, true));
      break;
    case types::n4:
      HANDLE_CODE(pack_integer(bref, c.get<n4_s_>().comb_offset_n4, (uint8_t)0u, (uint8_t)3u, false, true));
      HANDLE_CODE(pack_integer(bref, c.get<n4_s_>().cyclic_shift_n4, (uint8_t)0u, (uint8_t)11u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<tx_comb_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tx_comb_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::n2:
      HANDLE_CODE(unpack_integer(c.get<n2_s_>().comb_offset_n2, bref, (uint8_t)0u, (uint8_t)1u, false, true));
      HANDLE_CODE(unpack_integer(c.get<n2_s_>().cyclic_shift_n2, bref, (uint8_t)0u, (uint8_t)7u, false, true));
      break;
    case types::n4:
      HANDLE_CODE(unpack_integer(c.get<n4_s_>().comb_offset_n4, bref, (uint8_t)0u, (uint8_t)3u, false, true));
      HANDLE_CODE(unpack_integer(c.get<n4_s_>().cyclic_shift_n4, bref, (uint8_t)0u, (uint8_t)11u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<tx_comb_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* tx_comb_c::types_opts::to_string() const
{
  static const char* names[] = {"n2", "n4", "choice-extension"};
  return convert_enum_idx(names, 3, value, "tx_comb_c::types");
}
uint8_t tx_comb_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4};
  return map_enum_number(numbers, 2, value, "tx_comb_c::types");
}

// ResourceTypePeriodic ::= SEQUENCE
OCUDUASN_CODE res_type_periodic_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, offset, (uint16_t)0u, (uint16_t)2559u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_periodic_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(offset, bref, (uint16_t)0u, (uint16_t)2559u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_type_periodic_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("periodicity", periodicity.to_string());
  j.write_int("offset", offset);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* res_type_periodic_s::periodicity_opts::to_string() const
{
  static const char* names[] = {"slot1",
                                "slot2",
                                "slot4",
                                "slot5",
                                "slot8",
                                "slot10",
                                "slot16",
                                "slot20",
                                "slot32",
                                "slot40",
                                "slot64",
                                "slot80",
                                "slot160",
                                "slot320",
                                "slot640",
                                "slot1280",
                                "slot2560"};
  return convert_enum_idx(names, 17, value, "res_type_periodic_s::periodicity_e_");
}
uint16_t res_type_periodic_s::periodicity_opts::to_number() const
{
  static const uint16_t numbers[] = {1, 2, 4, 5, 8, 10, 16, 20, 32, 40, 64, 80, 160, 320, 640, 1280, 2560};
  return map_enum_number(numbers, 17, value, "res_type_periodic_s::periodicity_e_");
}

// ResourceTypeSemi-persistent ::= SEQUENCE
OCUDUASN_CODE res_type_semi_persistent_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, offset, (uint16_t)0u, (uint16_t)2559u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_semi_persistent_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(offset, bref, (uint16_t)0u, (uint16_t)2559u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_type_semi_persistent_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("periodicity", periodicity.to_string());
  j.write_int("offset", offset);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* res_type_semi_persistent_s::periodicity_opts::to_string() const
{
  static const char* names[] = {"slot1",
                                "slot2",
                                "slot4",
                                "slot5",
                                "slot8",
                                "slot10",
                                "slot16",
                                "slot20",
                                "slot32",
                                "slot40",
                                "slot64",
                                "slot80",
                                "slot160",
                                "slot320",
                                "slot640",
                                "slot1280",
                                "slot2560"};
  return convert_enum_idx(names, 17, value, "res_type_semi_persistent_s::periodicity_e_");
}
uint16_t res_type_semi_persistent_s::periodicity_opts::to_number() const
{
  static const uint16_t numbers[] = {1, 2, 4, 5, 8, 10, 16, 20, 32, 40, 64, 80, 160, 320, 640, 1280, 2560};
  return map_enum_number(numbers, 17, value, "res_type_semi_persistent_s::periodicity_e_");
}

// ResourceTypeAperiodic ::= SEQUENCE
OCUDUASN_CODE res_type_aperiodic_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(aperiodic_res_type.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_aperiodic_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(aperiodic_res_type.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_type_aperiodic_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("aperiodicResourceType", "true");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* res_type_aperiodic_s::aperiodic_res_type_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "res_type_aperiodic_s::aperiodic_res_type_e_");
}

// ResourceType ::= CHOICE
void res_type_c::destroy_()
{
  switch (type_) {
    case types::periodic:
      c.destroy<res_type_periodic_s>();
      break;
    case types::semi_persistent:
      c.destroy<res_type_semi_persistent_s>();
      break;
    case types::aperiodic:
      c.destroy<res_type_aperiodic_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<res_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void res_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::periodic:
      c.init<res_type_periodic_s>();
      break;
    case types::semi_persistent:
      c.init<res_type_semi_persistent_s>();
      break;
    case types::aperiodic:
      c.init<res_type_aperiodic_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<res_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_type_c");
  }
}
res_type_c::res_type_c(const res_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::periodic:
      c.init(other.c.get<res_type_periodic_s>());
      break;
    case types::semi_persistent:
      c.init(other.c.get<res_type_semi_persistent_s>());
      break;
    case types::aperiodic:
      c.init(other.c.get<res_type_aperiodic_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<res_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_type_c");
  }
}
res_type_c& res_type_c::operator=(const res_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::periodic:
      c.set(other.c.get<res_type_periodic_s>());
      break;
    case types::semi_persistent:
      c.set(other.c.get<res_type_semi_persistent_s>());
      break;
    case types::aperiodic:
      c.set(other.c.get<res_type_aperiodic_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<res_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_type_c");
  }

  return *this;
}
res_type_periodic_s& res_type_c::set_periodic()
{
  set(types::periodic);
  return c.get<res_type_periodic_s>();
}
res_type_semi_persistent_s& res_type_c::set_semi_persistent()
{
  set(types::semi_persistent);
  return c.get<res_type_semi_persistent_s>();
}
res_type_aperiodic_s& res_type_c::set_aperiodic()
{
  set(types::aperiodic);
  return c.get<res_type_aperiodic_s>();
}
protocol_ie_single_container_s<res_type_ext_ies_o>& res_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<res_type_ext_ies_o>>();
}
void res_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::periodic:
      j.write_fieldname("periodic");
      c.get<res_type_periodic_s>().to_json(j);
      break;
    case types::semi_persistent:
      j.write_fieldname("semi-persistent");
      c.get<res_type_semi_persistent_s>().to_json(j);
      break;
    case types::aperiodic:
      j.write_fieldname("aperiodic");
      c.get<res_type_aperiodic_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<res_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "res_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE res_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::periodic:
      HANDLE_CODE(c.get<res_type_periodic_s>().pack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<res_type_semi_persistent_s>().pack(bref));
      break;
    case types::aperiodic:
      HANDLE_CODE(c.get<res_type_aperiodic_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<res_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "res_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::periodic:
      HANDLE_CODE(c.get<res_type_periodic_s>().unpack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<res_type_semi_persistent_s>().unpack(bref));
      break;
    case types::aperiodic:
      HANDLE_CODE(c.get<res_type_aperiodic_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<res_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "res_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* res_type_c::types_opts::to_string() const
{
  static const char* names[] = {"periodic", "semi-persistent", "aperiodic", "choice-extension"};
  return convert_enum_idx(names, 4, value, "res_type_c::types");
}

// NrofSymbolsExtended ::= ENUMERATED
const char* nrof_symbols_extended_opts::to_string() const
{
  static const char* names[] = {"n8", "n10", "n12", "n14"};
  return convert_enum_idx(names, 4, value, "nrof_symbols_extended_e");
}
uint8_t nrof_symbols_extended_opts::to_number() const
{
  static const uint8_t numbers[] = {8, 10, 12, 14};
  return map_enum_number(numbers, 4, value, "nrof_symbols_extended_e");
}

// RepetitionFactorExtended ::= ENUMERATED
const char* repeat_factor_extended_opts::to_string() const
{
  static const char* names[] = {"n3", "n5", "n6", "n7", "n8", "n10", "n12", "n14"};
  return convert_enum_idx(names, 8, value, "repeat_factor_extended_e");
}
uint8_t repeat_factor_extended_opts::to_number() const
{
  static const uint8_t numbers[] = {3, 5, 6, 7, 8, 10, 12, 14};
  return map_enum_number(numbers, 8, value, "repeat_factor_extended_e");
}

// StartRBHopping ::= ENUMERATED
const char* start_rb_hop_opts::to_string() const
{
  static const char* names[] = {"enable"};
  return convert_enum_idx(names, 1, value, "start_rb_hop_e");
}

// StartRBIndex ::= CHOICE
void start_rb_idx_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void start_rb_idx_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::freq_scaling_factor2:
      break;
    case types::freq_scaling_factor4:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "start_rb_idx_c");
  }
}
start_rb_idx_c::start_rb_idx_c(const start_rb_idx_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::freq_scaling_factor2:
      c.init(other.c.get<uint8_t>());
      break;
    case types::freq_scaling_factor4:
      c.init(other.c.get<uint8_t>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "start_rb_idx_c");
  }
}
start_rb_idx_c& start_rb_idx_c::operator=(const start_rb_idx_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::freq_scaling_factor2:
      c.set(other.c.get<uint8_t>());
      break;
    case types::freq_scaling_factor4:
      c.set(other.c.get<uint8_t>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "start_rb_idx_c");
  }

  return *this;
}
uint8_t& start_rb_idx_c::set_freq_scaling_factor2()
{
  set(types::freq_scaling_factor2);
  return c.get<uint8_t>();
}
uint8_t& start_rb_idx_c::set_freq_scaling_factor4()
{
  set(types::freq_scaling_factor4);
  return c.get<uint8_t>();
}
protocol_ie_single_container_s<start_rb_idx_ext_ies_o>& start_rb_idx_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>();
}
void start_rb_idx_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::freq_scaling_factor2:
      j.write_int("freqScalingFactor2", c.get<uint8_t>());
      break;
    case types::freq_scaling_factor4:
      j.write_int("freqScalingFactor4", c.get<uint8_t>());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "start_rb_idx_c");
  }
  j.end_obj();
}
OCUDUASN_CODE start_rb_idx_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::freq_scaling_factor2:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)1u, false, true));
      break;
    case types::freq_scaling_factor4:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)3u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "start_rb_idx_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE start_rb_idx_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::freq_scaling_factor2:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)1u, false, true));
      break;
    case types::freq_scaling_factor4:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)3u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "start_rb_idx_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* start_rb_idx_c::types_opts::to_string() const
{
  static const char* names[] = {"freqScalingFactor2", "freqScalingFactor4", "choice-extension"};
  return convert_enum_idx(names, 3, value, "start_rb_idx_c::types");
}
uint8_t start_rb_idx_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4};
  return map_enum_number(numbers, 2, value, "start_rb_idx_c::types");
}

// SRSResource-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t srs_res_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {107, 108, 109, 110};
  return map_enum_number(names, 4, idx, "id");
}
bool srs_res_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {107, 108, 109, 110};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e srs_res_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 107:
      return crit_e::ignore;
    case 108:
      return crit_e::ignore;
    case 109:
      return crit_e::ignore;
    case 110:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
srs_res_ext_ies_o::ext_c srs_res_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 107:
      ret.set(ext_c::types::nrof_symbols_extended);
      break;
    case 108:
      ret.set(ext_c::types::repeat_factor_extended);
      break;
    case 109:
      ret.set(ext_c::types::start_rb_hop);
      break;
    case 110:
      ret.set(ext_c::types::start_rb_idx);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e srs_res_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 107:
      return presence_e::optional;
    case 108:
      return presence_e::optional;
    case 109:
      return presence_e::optional;
    case 110:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void srs_res_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::nrof_symbols_extended:
      c = nrof_symbols_extended_e{};
      break;
    case types::repeat_factor_extended:
      c = repeat_factor_extended_e{};
      break;
    case types::start_rb_hop:
      c = start_rb_hop_e{};
      break;
    case types::start_rb_idx:
      c = start_rb_idx_c{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_ext_ies_o::ext_c");
  }
}
nrof_symbols_extended_e& srs_res_ext_ies_o::ext_c::nrof_symbols_extended()
{
  assert_choice_type(types::nrof_symbols_extended, type_, "Extension");
  return c.get<nrof_symbols_extended_e>();
}
repeat_factor_extended_e& srs_res_ext_ies_o::ext_c::repeat_factor_extended()
{
  assert_choice_type(types::repeat_factor_extended, type_, "Extension");
  return c.get<repeat_factor_extended_e>();
}
start_rb_hop_e& srs_res_ext_ies_o::ext_c::start_rb_hop()
{
  assert_choice_type(types::start_rb_hop, type_, "Extension");
  return c.get<start_rb_hop_e>();
}
start_rb_idx_c& srs_res_ext_ies_o::ext_c::start_rb_idx()
{
  assert_choice_type(types::start_rb_idx, type_, "Extension");
  return c.get<start_rb_idx_c>();
}
const nrof_symbols_extended_e& srs_res_ext_ies_o::ext_c::nrof_symbols_extended() const
{
  assert_choice_type(types::nrof_symbols_extended, type_, "Extension");
  return c.get<nrof_symbols_extended_e>();
}
const repeat_factor_extended_e& srs_res_ext_ies_o::ext_c::repeat_factor_extended() const
{
  assert_choice_type(types::repeat_factor_extended, type_, "Extension");
  return c.get<repeat_factor_extended_e>();
}
const start_rb_hop_e& srs_res_ext_ies_o::ext_c::start_rb_hop() const
{
  assert_choice_type(types::start_rb_hop, type_, "Extension");
  return c.get<start_rb_hop_e>();
}
const start_rb_idx_c& srs_res_ext_ies_o::ext_c::start_rb_idx() const
{
  assert_choice_type(types::start_rb_idx, type_, "Extension");
  return c.get<start_rb_idx_c>();
}
void srs_res_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::nrof_symbols_extended:
      j.write_str(c.get<nrof_symbols_extended_e>().to_string());
      break;
    case types::repeat_factor_extended:
      j.write_str(c.get<repeat_factor_extended_e>().to_string());
      break;
    case types::start_rb_hop:
      j.write_str("enable");
      break;
    case types::start_rb_idx:
      c.get<start_rb_idx_c>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE srs_res_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::nrof_symbols_extended:
      HANDLE_CODE(c.get<nrof_symbols_extended_e>().pack(bref));
      break;
    case types::repeat_factor_extended:
      HANDLE_CODE(c.get<repeat_factor_extended_e>().pack(bref));
      break;
    case types::start_rb_hop:
      HANDLE_CODE(c.get<start_rb_hop_e>().pack(bref));
      break;
    case types::start_rb_idx:
      HANDLE_CODE(c.get<start_rb_idx_c>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::nrof_symbols_extended:
      HANDLE_CODE(c.get<nrof_symbols_extended_e>().unpack(bref));
      break;
    case types::repeat_factor_extended:
      HANDLE_CODE(c.get<repeat_factor_extended_e>().unpack(bref));
      break;
    case types::start_rb_hop:
      HANDLE_CODE(c.get<start_rb_hop_e>().unpack(bref));
      break;
    case types::start_rb_idx:
      HANDLE_CODE(c.get<start_rb_idx_c>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* srs_res_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"NrofSymbolsExtended", "RepetitionFactorExtended", "StartRBHopping", "StartRBIndex"};
  return convert_enum_idx(names, 4, value, "srs_res_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<srs_res_ext_ies_o>;

OCUDUASN_CODE srs_res_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += nrof_symbols_extended_present ? 1 : 0;
  nof_ies += repeat_factor_extended_present ? 1 : 0;
  nof_ies += start_rb_hop_present ? 1 : 0;
  nof_ies += start_rb_idx_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (nrof_symbols_extended_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)107, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nrof_symbols_extended.pack(bref));
  }
  if (repeat_factor_extended_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)108, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(repeat_factor_extended.pack(bref));
  }
  if (start_rb_hop_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)109, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(start_rb_hop.pack(bref));
  }
  if (start_rb_idx_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)110, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(start_rb_idx.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != srs_res_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 107: {
        nrof_symbols_extended_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nrof_symbols_extended.unpack(bref));
        break;
      }
      case 108: {
        repeat_factor_extended_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(repeat_factor_extended.unpack(bref));
        break;
      }
      case 109: {
        start_rb_hop_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(start_rb_hop.unpack(bref));
        break;
      }
      case 110: {
        start_rb_idx_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(start_rb_idx.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void srs_res_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (nrof_symbols_extended_present) {
    j.start_obj();
    j.write_int("id", 107);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", nrof_symbols_extended.to_string());
    j.end_obj();
  }
  if (repeat_factor_extended_present) {
    j.start_obj();
    j.write_int("id", 108);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", repeat_factor_extended.to_string());
    j.end_obj();
  }
  if (start_rb_hop_present) {
    j.start_obj();
    j.write_int("id", 109);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "enable");
    j.end_obj();
  }
  if (start_rb_idx_present) {
    j.start_obj();
    j.write_int("id", 110);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    start_rb_idx.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SRSResource ::= SEQUENCE
OCUDUASN_CODE srs_res_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, srs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(nrof_srs_ports.pack(bref));
  HANDLE_CODE(tx_comb.pack(bref));
  HANDLE_CODE(pack_integer(bref, start_position, (uint8_t)0u, (uint8_t)13u, false, true));
  HANDLE_CODE(nrof_symbols.pack(bref));
  HANDLE_CODE(repeat_factor.pack(bref));
  HANDLE_CODE(pack_integer(bref, freq_domain_position, (uint8_t)0u, (uint8_t)67u, false, true));
  HANDLE_CODE(pack_integer(bref, freq_domain_shift, (uint16_t)0u, (uint16_t)268u, false, true));
  HANDLE_CODE(pack_integer(bref, c_srs, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(pack_integer(bref, b_srs, (uint8_t)0u, (uint8_t)3u, false, true));
  HANDLE_CODE(pack_integer(bref, b_hop, (uint8_t)0u, (uint8_t)3u, false, true));
  HANDLE_CODE(group_or_seq_hop.pack(bref));
  HANDLE_CODE(res_type.pack(bref));
  HANDLE_CODE(pack_integer(bref, seq_id, (uint16_t)0u, (uint16_t)1023u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(nrof_srs_ports.unpack(bref));
  HANDLE_CODE(tx_comb.unpack(bref));
  HANDLE_CODE(unpack_integer(start_position, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  HANDLE_CODE(nrof_symbols.unpack(bref));
  HANDLE_CODE(repeat_factor.unpack(bref));
  HANDLE_CODE(unpack_integer(freq_domain_position, bref, (uint8_t)0u, (uint8_t)67u, false, true));
  HANDLE_CODE(unpack_integer(freq_domain_shift, bref, (uint16_t)0u, (uint16_t)268u, false, true));
  HANDLE_CODE(unpack_integer(c_srs, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(unpack_integer(b_srs, bref, (uint8_t)0u, (uint8_t)3u, false, true));
  HANDLE_CODE(unpack_integer(b_hop, bref, (uint8_t)0u, (uint8_t)3u, false, true));
  HANDLE_CODE(group_or_seq_hop.unpack(bref));
  HANDLE_CODE(res_type.unpack(bref));
  HANDLE_CODE(unpack_integer(seq_id, bref, (uint16_t)0u, (uint16_t)1023u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_res_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSResourceID", srs_res_id);
  j.write_str("nrofSRS-Ports", nrof_srs_ports.to_string());
  j.write_fieldname("transmissionComb");
  tx_comb.to_json(j);
  j.write_int("startPosition", start_position);
  j.write_str("nrofSymbols", nrof_symbols.to_string());
  j.write_str("repetitionFactor", repeat_factor.to_string());
  j.write_int("freqDomainPosition", freq_domain_position);
  j.write_int("freqDomainShift", freq_domain_shift);
  j.write_int("c-SRS", c_srs);
  j.write_int("b-SRS", b_srs);
  j.write_int("b-hop", b_hop);
  j.write_str("groupOrSequenceHopping", group_or_seq_hop.to_string());
  j.write_fieldname("resourceType");
  res_type.to_json(j);
  j.write_int("sequenceId", seq_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* srs_res_s::nrof_srs_ports_opts::to_string() const
{
  static const char* names[] = {"port1", "ports2", "ports4"};
  return convert_enum_idx(names, 3, value, "srs_res_s::nrof_srs_ports_e_");
}
uint8_t srs_res_s::nrof_srs_ports_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4};
  return map_enum_number(numbers, 3, value, "srs_res_s::nrof_srs_ports_e_");
}

const char* srs_res_s::nrof_symbols_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4"};
  return convert_enum_idx(names, 3, value, "srs_res_s::nrof_symbols_e_");
}
uint8_t srs_res_s::nrof_symbols_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4};
  return map_enum_number(numbers, 3, value, "srs_res_s::nrof_symbols_e_");
}

const char* srs_res_s::repeat_factor_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4"};
  return convert_enum_idx(names, 3, value, "srs_res_s::repeat_factor_e_");
}
uint8_t srs_res_s::repeat_factor_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4};
  return map_enum_number(numbers, 3, value, "srs_res_s::repeat_factor_e_");
}

const char* srs_res_s::group_or_seq_hop_opts::to_string() const
{
  static const char* names[] = {"neither", "groupHopping", "sequenceHopping"};
  return convert_enum_idx(names, 3, value, "srs_res_s::group_or_seq_hop_e_");
}

// TransmissionCombPos ::= CHOICE
void tx_comb_pos_c::destroy_()
{
  switch (type_) {
    case types::n2:
      c.destroy<n2_s_>();
      break;
    case types::n4:
      c.destroy<n4_s_>();
      break;
    case types::n8:
      c.destroy<n8_s_>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void tx_comb_pos_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::n2:
      c.init<n2_s_>();
      break;
    case types::n4:
      c.init<n4_s_>();
      break;
    case types::n8:
      c.init<n8_s_>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_pos_c");
  }
}
tx_comb_pos_c::tx_comb_pos_c(const tx_comb_pos_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::n2:
      c.init(other.c.get<n2_s_>());
      break;
    case types::n4:
      c.init(other.c.get<n4_s_>());
      break;
    case types::n8:
      c.init(other.c.get<n8_s_>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_pos_c");
  }
}
tx_comb_pos_c& tx_comb_pos_c::operator=(const tx_comb_pos_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::n2:
      c.set(other.c.get<n2_s_>());
      break;
    case types::n4:
      c.set(other.c.get<n4_s_>());
      break;
    case types::n8:
      c.set(other.c.get<n8_s_>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_pos_c");
  }

  return *this;
}
tx_comb_pos_c::n2_s_& tx_comb_pos_c::set_n2()
{
  set(types::n2);
  return c.get<n2_s_>();
}
tx_comb_pos_c::n4_s_& tx_comb_pos_c::set_n4()
{
  set(types::n4);
  return c.get<n4_s_>();
}
tx_comb_pos_c::n8_s_& tx_comb_pos_c::set_n8()
{
  set(types::n8);
  return c.get<n8_s_>();
}
protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>& tx_comb_pos_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>();
}
void tx_comb_pos_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::n2:
      j.write_fieldname("n2");
      j.start_obj();
      j.write_int("combOffset-n2", c.get<n2_s_>().comb_offset_n2);
      j.write_int("cyclicShift-n2", c.get<n2_s_>().cyclic_shift_n2);
      j.end_obj();
      break;
    case types::n4:
      j.write_fieldname("n4");
      j.start_obj();
      j.write_int("combOffset-n4", c.get<n4_s_>().comb_offset_n4);
      j.write_int("cyclicShift-n4", c.get<n4_s_>().cyclic_shift_n4);
      j.end_obj();
      break;
    case types::n8:
      j.write_fieldname("n8");
      j.start_obj();
      j.write_int("combOffset-n8", c.get<n8_s_>().comb_offset_n8);
      j.write_int("cyclicShift-n8", c.get<n8_s_>().cyclic_shift_n8);
      j.end_obj();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_pos_c");
  }
  j.end_obj();
}
OCUDUASN_CODE tx_comb_pos_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::n2:
      HANDLE_CODE(pack_integer(bref, c.get<n2_s_>().comb_offset_n2, (uint8_t)0u, (uint8_t)1u, false, true));
      HANDLE_CODE(pack_integer(bref, c.get<n2_s_>().cyclic_shift_n2, (uint8_t)0u, (uint8_t)7u, false, true));
      break;
    case types::n4:
      HANDLE_CODE(pack_integer(bref, c.get<n4_s_>().comb_offset_n4, (uint8_t)0u, (uint8_t)3u, false, true));
      HANDLE_CODE(pack_integer(bref, c.get<n4_s_>().cyclic_shift_n4, (uint8_t)0u, (uint8_t)11u, false, true));
      break;
    case types::n8:
      HANDLE_CODE(pack_integer(bref, c.get<n8_s_>().comb_offset_n8, (uint8_t)0u, (uint8_t)7u, false, true));
      HANDLE_CODE(pack_integer(bref, c.get<n8_s_>().cyclic_shift_n8, (uint8_t)0u, (uint8_t)5u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_pos_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tx_comb_pos_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::n2:
      HANDLE_CODE(unpack_integer(c.get<n2_s_>().comb_offset_n2, bref, (uint8_t)0u, (uint8_t)1u, false, true));
      HANDLE_CODE(unpack_integer(c.get<n2_s_>().cyclic_shift_n2, bref, (uint8_t)0u, (uint8_t)7u, false, true));
      break;
    case types::n4:
      HANDLE_CODE(unpack_integer(c.get<n4_s_>().comb_offset_n4, bref, (uint8_t)0u, (uint8_t)3u, false, true));
      HANDLE_CODE(unpack_integer(c.get<n4_s_>().cyclic_shift_n4, bref, (uint8_t)0u, (uint8_t)11u, false, true));
      break;
    case types::n8:
      HANDLE_CODE(unpack_integer(c.get<n8_s_>().comb_offset_n8, bref, (uint8_t)0u, (uint8_t)7u, false, true));
      HANDLE_CODE(unpack_integer(c.get<n8_s_>().cyclic_shift_n8, bref, (uint8_t)0u, (uint8_t)5u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "tx_comb_pos_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* tx_comb_pos_c::types_opts::to_string() const
{
  static const char* names[] = {"n2", "n4", "n8", "choice-extension"};
  return convert_enum_idx(names, 4, value, "tx_comb_pos_c::types");
}
uint8_t tx_comb_pos_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 8};
  return map_enum_number(numbers, 3, value, "tx_comb_pos_c::types");
}

// SRSPeriodicity ::= ENUMERATED
const char* srs_periodicity_opts::to_string() const
{
  static const char* names[] = {"slot1",   "slot2",    "slot4",    "slot5",    "slot8",     "slot10",    "slot16",
                                "slot20",  "slot32",   "slot40",   "slot64",   "slot80",    "slot160",   "slot320",
                                "slot640", "slot1280", "slot2560", "slot5120", "slot10240", "slot40960", "slot81920",
                                "slot128", "slot256",  "slot512",  "slot20480"};
  return convert_enum_idx(names, 25, value, "srs_periodicity_e");
}
uint32_t srs_periodicity_opts::to_number() const
{
  static const uint32_t numbers[] = {1,   2,   4,    5,    8,    10,    16,    20,    32,  40,  64,  80,   160,
                                     320, 640, 1280, 2560, 5120, 10240, 40960, 81920, 128, 256, 512, 20480};
  return map_enum_number(numbers, 25, value, "srs_periodicity_e");
}

// SRSPosPeriodicConfigHyperSFNIndex ::= ENUMERATED
const char* srs_pos_periodic_cfg_hyper_sfn_idx_opts::to_string() const
{
  static const char* names[] = {"even0", "odd1"};
  return convert_enum_idx(names, 2, value, "srs_pos_periodic_cfg_hyper_sfn_idx_e");
}
uint8_t srs_pos_periodic_cfg_hyper_sfn_idx_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1};
  return map_enum_number(numbers, 2, value, "srs_pos_periodic_cfg_hyper_sfn_idx_e");
}

// ResourceTypePeriodicPos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t res_type_periodic_pos_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {165};
  return map_enum_number(names, 1, idx, "id");
}
bool res_type_periodic_pos_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 165 == id;
}
crit_e res_type_periodic_pos_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 165) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
res_type_periodic_pos_ext_ies_o::ext_c res_type_periodic_pos_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 165) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e res_type_periodic_pos_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 165) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void res_type_periodic_pos_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_str(c.to_string());
}
OCUDUASN_CODE res_type_periodic_pos_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_periodic_pos_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* res_type_periodic_pos_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"SRSPosPeriodicConfigHyperSFNIndex"};
  return convert_enum_idx(names, 1, value, "res_type_periodic_pos_ext_ies_o::ext_c::types");
}

// ResourceTypePeriodicPos ::= SEQUENCE
OCUDUASN_CODE res_type_periodic_pos_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(srs_periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, offset, (uint32_t)0u, (uint32_t)81919u, true, true));
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_periodic_pos_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(srs_periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(offset, bref, (uint32_t)0u, (uint32_t)81919u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_type_periodic_pos_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("sRSPeriodicity", srs_periodicity.to_string());
  j.write_int("offset", offset);
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResourceTypeSemi-persistentPos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t res_type_semi_persistent_pos_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {165};
  return map_enum_number(names, 1, idx, "id");
}
bool res_type_semi_persistent_pos_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 165 == id;
}
crit_e res_type_semi_persistent_pos_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 165) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
res_type_semi_persistent_pos_ext_ies_o::ext_c res_type_semi_persistent_pos_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 165) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e res_type_semi_persistent_pos_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 165) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void res_type_semi_persistent_pos_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_str(c.to_string());
}
OCUDUASN_CODE res_type_semi_persistent_pos_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_semi_persistent_pos_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* res_type_semi_persistent_pos_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"SRSPosPeriodicConfigHyperSFNIndex"};
  return convert_enum_idx(names, 1, value, "res_type_semi_persistent_pos_ext_ies_o::ext_c::types");
}

// ResourceTypeSemi-persistentPos ::= SEQUENCE
OCUDUASN_CODE res_type_semi_persistent_pos_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(srs_periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, offset, (uint32_t)0u, (uint32_t)81919u, true, true));
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_semi_persistent_pos_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(srs_periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(offset, bref, (uint32_t)0u, (uint32_t)81919u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_type_semi_persistent_pos_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("sRSPeriodicity", srs_periodicity.to_string());
  j.write_int("offset", offset);
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResourceTypeAperiodicPos ::= SEQUENCE
OCUDUASN_CODE res_type_aperiodic_pos_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, slot_offset, (uint8_t)0u, (uint8_t)32u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_aperiodic_pos_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(slot_offset, bref, (uint8_t)0u, (uint8_t)32u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_type_aperiodic_pos_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("slotOffset", slot_offset);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResourceTypePos ::= CHOICE
void res_type_pos_c::destroy_()
{
  switch (type_) {
    case types::periodic:
      c.destroy<res_type_periodic_pos_s>();
      break;
    case types::semi_persistent:
      c.destroy<res_type_semi_persistent_pos_s>();
      break;
    case types::aperiodic:
      c.destroy<res_type_aperiodic_pos_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void res_type_pos_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::periodic:
      c.init<res_type_periodic_pos_s>();
      break;
    case types::semi_persistent:
      c.init<res_type_semi_persistent_pos_s>();
      break;
    case types::aperiodic:
      c.init<res_type_aperiodic_pos_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_type_pos_c");
  }
}
res_type_pos_c::res_type_pos_c(const res_type_pos_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::periodic:
      c.init(other.c.get<res_type_periodic_pos_s>());
      break;
    case types::semi_persistent:
      c.init(other.c.get<res_type_semi_persistent_pos_s>());
      break;
    case types::aperiodic:
      c.init(other.c.get<res_type_aperiodic_pos_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_type_pos_c");
  }
}
res_type_pos_c& res_type_pos_c::operator=(const res_type_pos_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::periodic:
      c.set(other.c.get<res_type_periodic_pos_s>());
      break;
    case types::semi_persistent:
      c.set(other.c.get<res_type_semi_persistent_pos_s>());
      break;
    case types::aperiodic:
      c.set(other.c.get<res_type_aperiodic_pos_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_type_pos_c");
  }

  return *this;
}
res_type_periodic_pos_s& res_type_pos_c::set_periodic()
{
  set(types::periodic);
  return c.get<res_type_periodic_pos_s>();
}
res_type_semi_persistent_pos_s& res_type_pos_c::set_semi_persistent()
{
  set(types::semi_persistent);
  return c.get<res_type_semi_persistent_pos_s>();
}
res_type_aperiodic_pos_s& res_type_pos_c::set_aperiodic()
{
  set(types::aperiodic);
  return c.get<res_type_aperiodic_pos_s>();
}
protocol_ie_single_container_s<res_type_pos_ext_ies_o>& res_type_pos_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>();
}
void res_type_pos_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::periodic:
      j.write_fieldname("periodic");
      c.get<res_type_periodic_pos_s>().to_json(j);
      break;
    case types::semi_persistent:
      j.write_fieldname("semi-persistent");
      c.get<res_type_semi_persistent_pos_s>().to_json(j);
      break;
    case types::aperiodic:
      j.write_fieldname("aperiodic");
      c.get<res_type_aperiodic_pos_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "res_type_pos_c");
  }
  j.end_obj();
}
OCUDUASN_CODE res_type_pos_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::periodic:
      HANDLE_CODE(c.get<res_type_periodic_pos_s>().pack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<res_type_semi_persistent_pos_s>().pack(bref));
      break;
    case types::aperiodic:
      HANDLE_CODE(c.get<res_type_aperiodic_pos_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "res_type_pos_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_type_pos_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::periodic:
      HANDLE_CODE(c.get<res_type_periodic_pos_s>().unpack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<res_type_semi_persistent_pos_s>().unpack(bref));
      break;
    case types::aperiodic:
      HANDLE_CODE(c.get<res_type_aperiodic_pos_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "res_type_pos_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* res_type_pos_c::types_opts::to_string() const
{
  static const char* names[] = {"periodic", "semi-persistent", "aperiodic", "choice-extension"};
  return convert_enum_idx(names, 4, value, "res_type_pos_c::types");
}

// SSB ::= SEQUENCE
OCUDUASN_CODE ssb_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ssb_idx_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pci_nr, (uint16_t)0u, (uint16_t)1007u, false, true));
  if (ssb_idx_present) {
    HANDLE_CODE(pack_integer(bref, ssb_idx, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ssb_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ssb_idx_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pci_nr, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  if (ssb_idx_present) {
    HANDLE_CODE(unpack_integer(ssb_idx, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ssb_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pCI-NR", pci_nr);
  if (ssb_idx_present) {
    j.write_int("ssb-index", ssb_idx);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSInformationPos ::= SEQUENCE
OCUDUASN_CODE pr_si_nformation_pos_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(prs_res_id_pos_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, prs_id_pos, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_integer(bref, prs_res_set_id_pos, (uint8_t)0u, (uint8_t)7u, false, true));
  if (prs_res_id_pos_present) {
    HANDLE_CODE(pack_integer(bref, prs_res_id_pos, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pr_si_nformation_pos_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(prs_res_id_pos_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prs_id_pos, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_integer(prs_res_set_id_pos, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  if (prs_res_id_pos_present) {
    HANDLE_CODE(unpack_integer(prs_res_id_pos, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pr_si_nformation_pos_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pRS-IDPos", prs_id_pos);
  j.write_int("pRS-Resource-Set-IDPos", prs_res_set_id_pos);
  if (prs_res_id_pos_present) {
    j.write_int("pRS-Resource-IDPos", prs_res_id_pos);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SpatialRelationPos ::= CHOICE
void spatial_relation_pos_c::destroy_()
{
  switch (type_) {
    case types::ssb_pos:
      c.destroy<ssb_s>();
      break;
    case types::pr_si_nformation_pos:
      c.destroy<pr_si_nformation_pos_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void spatial_relation_pos_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ssb_pos:
      c.init<ssb_s>();
      break;
    case types::pr_si_nformation_pos:
      c.init<pr_si_nformation_pos_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "spatial_relation_pos_c");
  }
}
spatial_relation_pos_c::spatial_relation_pos_c(const spatial_relation_pos_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ssb_pos:
      c.init(other.c.get<ssb_s>());
      break;
    case types::pr_si_nformation_pos:
      c.init(other.c.get<pr_si_nformation_pos_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "spatial_relation_pos_c");
  }
}
spatial_relation_pos_c& spatial_relation_pos_c::operator=(const spatial_relation_pos_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ssb_pos:
      c.set(other.c.get<ssb_s>());
      break;
    case types::pr_si_nformation_pos:
      c.set(other.c.get<pr_si_nformation_pos_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "spatial_relation_pos_c");
  }

  return *this;
}
ssb_s& spatial_relation_pos_c::set_ssb_pos()
{
  set(types::ssb_pos);
  return c.get<ssb_s>();
}
pr_si_nformation_pos_s& spatial_relation_pos_c::set_pr_si_nformation_pos()
{
  set(types::pr_si_nformation_pos);
  return c.get<pr_si_nformation_pos_s>();
}
protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>& spatial_relation_pos_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>();
}
void spatial_relation_pos_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ssb_pos:
      j.write_fieldname("sSBPos");
      c.get<ssb_s>().to_json(j);
      break;
    case types::pr_si_nformation_pos:
      j.write_fieldname("pRSInformationPos");
      c.get<pr_si_nformation_pos_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "spatial_relation_pos_c");
  }
  j.end_obj();
}
OCUDUASN_CODE spatial_relation_pos_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ssb_pos:
      HANDLE_CODE(c.get<ssb_s>().pack(bref));
      break;
    case types::pr_si_nformation_pos:
      HANDLE_CODE(c.get<pr_si_nformation_pos_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "spatial_relation_pos_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE spatial_relation_pos_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ssb_pos:
      HANDLE_CODE(c.get<ssb_s>().unpack(bref));
      break;
    case types::pr_si_nformation_pos:
      HANDLE_CODE(c.get<pr_si_nformation_pos_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "spatial_relation_pos_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* spatial_relation_pos_c::types_opts::to_string() const
{
  static const char* names[] = {"sSBPos", "pRSInformationPos", "choice-extension"};
  return convert_enum_idx(names, 3, value, "spatial_relation_pos_c::types");
}

// SlotOffsetRemainingHopsAperiodic ::= SEQUENCE
OCUDUASN_CODE slot_offset_remaining_hops_aperiodic_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(slot_offset_present, 1));
  HANDLE_CODE(bref.pack(start_position_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (slot_offset_present) {
    HANDLE_CODE(pack_integer(bref, slot_offset, (uint8_t)1u, (uint8_t)32u, false, true));
  }
  if (start_position_present) {
    HANDLE_CODE(pack_integer(bref, start_position, (uint8_t)0u, (uint8_t)13u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE slot_offset_remaining_hops_aperiodic_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(slot_offset_present, 1));
  HANDLE_CODE(bref.unpack(start_position_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (slot_offset_present) {
    HANDLE_CODE(unpack_integer(slot_offset, bref, (uint8_t)1u, (uint8_t)32u, false, true));
  }
  if (start_position_present) {
    HANDLE_CODE(unpack_integer(start_position, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void slot_offset_remaining_hops_aperiodic_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (slot_offset_present) {
    j.write_int("slotOffset", slot_offset);
  }
  if (start_position_present) {
    j.write_int("startPosition", start_position);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SlotOffsetRemainingHopsSemiPersistent ::= SEQUENCE
OCUDUASN_CODE slot_offset_remaining_hops_semi_persistent_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(start_position_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(srs_periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, offset, (uint32_t)0u, (uint32_t)81919u, true, true));
  if (start_position_present) {
    HANDLE_CODE(pack_integer(bref, start_position, (uint8_t)0u, (uint8_t)13u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE slot_offset_remaining_hops_semi_persistent_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(start_position_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(srs_periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(offset, bref, (uint32_t)0u, (uint32_t)81919u, true, true));
  if (start_position_present) {
    HANDLE_CODE(unpack_integer(start_position, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void slot_offset_remaining_hops_semi_persistent_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("sRSperiodicity", srs_periodicity.to_string());
  j.write_int("offset", offset);
  if (start_position_present) {
    j.write_int("startPosition", start_position);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SlotOffsetRemainingHopsPeriodic ::= SEQUENCE
OCUDUASN_CODE slot_offset_remaining_hops_periodic_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(start_position_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(srs_periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, offset, (uint32_t)0u, (uint32_t)81919u, true, true));
  if (start_position_present) {
    HANDLE_CODE(pack_integer(bref, start_position, (uint8_t)0u, (uint8_t)13u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE slot_offset_remaining_hops_periodic_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(start_position_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(srs_periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(offset, bref, (uint32_t)0u, (uint32_t)81919u, true, true));
  if (start_position_present) {
    HANDLE_CODE(unpack_integer(start_position, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void slot_offset_remaining_hops_periodic_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("sRSperiodicity", srs_periodicity.to_string());
  j.write_int("offset", offset);
  if (start_position_present) {
    j.write_int("startPosition", start_position);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SlotOffsetRemainingHops ::= CHOICE
void slot_offset_remaining_hops_c::destroy_()
{
  switch (type_) {
    case types::aperiodic:
      c.destroy<slot_offset_remaining_hops_aperiodic_s>();
      break;
    case types::semi_persistent:
      c.destroy<slot_offset_remaining_hops_semi_persistent_s>();
      break;
    case types::periodic:
      c.destroy<slot_offset_remaining_hops_periodic_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void slot_offset_remaining_hops_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::aperiodic:
      c.init<slot_offset_remaining_hops_aperiodic_s>();
      break;
    case types::semi_persistent:
      c.init<slot_offset_remaining_hops_semi_persistent_s>();
      break;
    case types::periodic:
      c.init<slot_offset_remaining_hops_periodic_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "slot_offset_remaining_hops_c");
  }
}
slot_offset_remaining_hops_c::slot_offset_remaining_hops_c(const slot_offset_remaining_hops_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::aperiodic:
      c.init(other.c.get<slot_offset_remaining_hops_aperiodic_s>());
      break;
    case types::semi_persistent:
      c.init(other.c.get<slot_offset_remaining_hops_semi_persistent_s>());
      break;
    case types::periodic:
      c.init(other.c.get<slot_offset_remaining_hops_periodic_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "slot_offset_remaining_hops_c");
  }
}
slot_offset_remaining_hops_c& slot_offset_remaining_hops_c::operator=(const slot_offset_remaining_hops_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::aperiodic:
      c.set(other.c.get<slot_offset_remaining_hops_aperiodic_s>());
      break;
    case types::semi_persistent:
      c.set(other.c.get<slot_offset_remaining_hops_semi_persistent_s>());
      break;
    case types::periodic:
      c.set(other.c.get<slot_offset_remaining_hops_periodic_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "slot_offset_remaining_hops_c");
  }

  return *this;
}
slot_offset_remaining_hops_aperiodic_s& slot_offset_remaining_hops_c::set_aperiodic()
{
  set(types::aperiodic);
  return c.get<slot_offset_remaining_hops_aperiodic_s>();
}
slot_offset_remaining_hops_semi_persistent_s& slot_offset_remaining_hops_c::set_semi_persistent()
{
  set(types::semi_persistent);
  return c.get<slot_offset_remaining_hops_semi_persistent_s>();
}
slot_offset_remaining_hops_periodic_s& slot_offset_remaining_hops_c::set_periodic()
{
  set(types::periodic);
  return c.get<slot_offset_remaining_hops_periodic_s>();
}
protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>& slot_offset_remaining_hops_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>();
}
void slot_offset_remaining_hops_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::aperiodic:
      j.write_fieldname("aperiodic");
      c.get<slot_offset_remaining_hops_aperiodic_s>().to_json(j);
      break;
    case types::semi_persistent:
      j.write_fieldname("semi-persistent");
      c.get<slot_offset_remaining_hops_semi_persistent_s>().to_json(j);
      break;
    case types::periodic:
      j.write_fieldname("periodic");
      c.get<slot_offset_remaining_hops_periodic_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "slot_offset_remaining_hops_c");
  }
  j.end_obj();
}
OCUDUASN_CODE slot_offset_remaining_hops_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::aperiodic:
      HANDLE_CODE(c.get<slot_offset_remaining_hops_aperiodic_s>().pack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<slot_offset_remaining_hops_semi_persistent_s>().pack(bref));
      break;
    case types::periodic:
      HANDLE_CODE(c.get<slot_offset_remaining_hops_periodic_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "slot_offset_remaining_hops_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE slot_offset_remaining_hops_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::aperiodic:
      HANDLE_CODE(c.get<slot_offset_remaining_hops_aperiodic_s>().unpack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<slot_offset_remaining_hops_semi_persistent_s>().unpack(bref));
      break;
    case types::periodic:
      HANDLE_CODE(c.get<slot_offset_remaining_hops_periodic_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "slot_offset_remaining_hops_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* slot_offset_remaining_hops_c::types_opts::to_string() const
{
  static const char* names[] = {"aperiodic", "semi-persistent", "periodic", "choice-extension"};
  return convert_enum_idx(names, 4, value, "slot_offset_remaining_hops_c::types");
}

// SlotOffsetForRemainingHopsItem ::= SEQUENCE
OCUDUASN_CODE slot_offset_for_remaining_hops_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(slot_offset_remaining_hops.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE slot_offset_for_remaining_hops_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(slot_offset_remaining_hops.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void slot_offset_for_remaining_hops_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("slotOffsetRemainingHops");
  slot_offset_remaining_hops.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TxHoppingConfiguration ::= SEQUENCE
OCUDUASN_CODE tx_hop_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(overlap_value.pack(bref));
  HANDLE_CODE(pack_integer(bref, nof_hops, (uint8_t)2u, (uint8_t)6u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, slot_offset_for_remaining_hops_list, 1, 5, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tx_hop_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(overlap_value.unpack(bref));
  HANDLE_CODE(unpack_integer(nof_hops, bref, (uint8_t)2u, (uint8_t)6u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(slot_offset_for_remaining_hops_list, bref, 1, 5, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void tx_hop_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("overlapValue", overlap_value.to_string());
  j.write_int("numberOfHops", nof_hops);
  j.start_array("slotOffsetForRemainingHopsList");
  for (const auto& e1 : slot_offset_for_remaining_hops_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* tx_hop_cfg_s::overlap_value_opts::to_string() const
{
  static const char* names[] = {"rb0", "rb1", "rb2", "rb4"};
  return convert_enum_idx(names, 4, value, "tx_hop_cfg_s::overlap_value_e_");
}
uint8_t tx_hop_cfg_s::overlap_value_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1, 2, 4};
  return map_enum_number(numbers, 4, value, "tx_hop_cfg_s::overlap_value_e_");
}

// PosSRSResource-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t pos_srs_res_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {139};
  return map_enum_number(names, 1, idx, "id");
}
bool pos_srs_res_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 139 == id;
}
crit_e pos_srs_res_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 139) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
pos_srs_res_item_ext_ies_o::ext_c pos_srs_res_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 139) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e pos_srs_res_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 139) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void pos_srs_res_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE pos_srs_res_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_srs_res_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* pos_srs_res_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"TxHoppingConfiguration"};
  return convert_enum_idx(names, 1, value, "pos_srs_res_item_ext_ies_o::ext_c::types");
}

// PosSRSResource-Item ::= SEQUENCE
OCUDUASN_CODE pos_srs_res_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(spatial_relation_pos_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, srs_pos_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(tx_comb_pos.pack(bref));
  HANDLE_CODE(pack_integer(bref, start_position, (uint8_t)0u, (uint8_t)13u, false, true));
  HANDLE_CODE(nrof_symbols.pack(bref));
  HANDLE_CODE(pack_integer(bref, freq_domain_shift, (uint16_t)0u, (uint16_t)268u, false, true));
  HANDLE_CODE(pack_integer(bref, c_srs, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(group_or_seq_hop.pack(bref));
  HANDLE_CODE(res_type_pos.pack(bref));
  HANDLE_CODE(pack_integer(bref, seq_id, (uint32_t)0u, (uint32_t)65535u, false, true));
  if (spatial_relation_pos_present) {
    HANDLE_CODE(spatial_relation_pos.pack(bref));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_srs_res_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(spatial_relation_pos_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_pos_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(tx_comb_pos.unpack(bref));
  HANDLE_CODE(unpack_integer(start_position, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  HANDLE_CODE(nrof_symbols.unpack(bref));
  HANDLE_CODE(unpack_integer(freq_domain_shift, bref, (uint16_t)0u, (uint16_t)268u, false, true));
  HANDLE_CODE(unpack_integer(c_srs, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(group_or_seq_hop.unpack(bref));
  HANDLE_CODE(res_type_pos.unpack(bref));
  HANDLE_CODE(unpack_integer(seq_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
  if (spatial_relation_pos_present) {
    HANDLE_CODE(spatial_relation_pos.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_srs_res_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("srs-PosResourceId", srs_pos_res_id);
  j.write_fieldname("transmissionCombPos");
  tx_comb_pos.to_json(j);
  j.write_int("startPosition", start_position);
  j.write_str("nrofSymbols", nrof_symbols.to_string());
  j.write_int("freqDomainShift", freq_domain_shift);
  j.write_int("c-SRS", c_srs);
  j.write_str("groupOrSequenceHopping", group_or_seq_hop.to_string());
  j.write_fieldname("resourceTypePos");
  res_type_pos.to_json(j);
  j.write_int("sequenceId", seq_id);
  if (spatial_relation_pos_present) {
    j.write_fieldname("spatialRelationPos");
    spatial_relation_pos.to_json(j);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* pos_srs_res_item_s::nrof_symbols_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4", "n8", "n12"};
  return convert_enum_idx(names, 5, value, "pos_srs_res_item_s::nrof_symbols_e_");
}
uint8_t pos_srs_res_item_s::nrof_symbols_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 8, 12};
  return map_enum_number(numbers, 5, value, "pos_srs_res_item_s::nrof_symbols_e_");
}

const char* pos_srs_res_item_s::group_or_seq_hop_opts::to_string() const
{
  static const char* names[] = {"neither", "groupHopping", "sequenceHopping"};
  return convert_enum_idx(names, 3, value, "pos_srs_res_item_s::group_or_seq_hop_e_");
}

// ResourceSetTypePeriodic ::= SEQUENCE
OCUDUASN_CODE res_set_type_periodic_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(periodic_set.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_set_type_periodic_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(periodic_set.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_set_type_periodic_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("periodicSet", "true");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* res_set_type_periodic_s::periodic_set_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "res_set_type_periodic_s::periodic_set_e_");
}

// ResourceSetTypeSemi-persistent ::= SEQUENCE
OCUDUASN_CODE res_set_type_semi_persistent_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(semi_persistent_set.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_set_type_semi_persistent_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(semi_persistent_set.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_set_type_semi_persistent_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("semi-persistentSet", "true");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* res_set_type_semi_persistent_s::semi_persistent_set_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "res_set_type_semi_persistent_s::semi_persistent_set_e_");
}

// ResourceSetTypeAperiodic ::= SEQUENCE
OCUDUASN_CODE res_set_type_aperiodic_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, srs_res_trigger, (uint8_t)1u, (uint8_t)3u, false, true));
  HANDLE_CODE(pack_integer(bref, slotoffset, (uint8_t)0u, (uint8_t)32u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_set_type_aperiodic_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_res_trigger, bref, (uint8_t)1u, (uint8_t)3u, false, true));
  HANDLE_CODE(unpack_integer(slotoffset, bref, (uint8_t)0u, (uint8_t)32u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_set_type_aperiodic_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSResourceTrigger", srs_res_trigger);
  j.write_int("slotoffset", slotoffset);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResourceSetType ::= CHOICE
void res_set_type_c::destroy_()
{
  switch (type_) {
    case types::periodic:
      c.destroy<res_set_type_periodic_s>();
      break;
    case types::semi_persistent:
      c.destroy<res_set_type_semi_persistent_s>();
      break;
    case types::aperiodic:
      c.destroy<res_set_type_aperiodic_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<res_set_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void res_set_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::periodic:
      c.init<res_set_type_periodic_s>();
      break;
    case types::semi_persistent:
      c.init<res_set_type_semi_persistent_s>();
      break;
    case types::aperiodic:
      c.init<res_set_type_aperiodic_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<res_set_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_set_type_c");
  }
}
res_set_type_c::res_set_type_c(const res_set_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::periodic:
      c.init(other.c.get<res_set_type_periodic_s>());
      break;
    case types::semi_persistent:
      c.init(other.c.get<res_set_type_semi_persistent_s>());
      break;
    case types::aperiodic:
      c.init(other.c.get<res_set_type_aperiodic_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<res_set_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_set_type_c");
  }
}
res_set_type_c& res_set_type_c::operator=(const res_set_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::periodic:
      c.set(other.c.get<res_set_type_periodic_s>());
      break;
    case types::semi_persistent:
      c.set(other.c.get<res_set_type_semi_persistent_s>());
      break;
    case types::aperiodic:
      c.set(other.c.get<res_set_type_aperiodic_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<res_set_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "res_set_type_c");
  }

  return *this;
}
res_set_type_periodic_s& res_set_type_c::set_periodic()
{
  set(types::periodic);
  return c.get<res_set_type_periodic_s>();
}
res_set_type_semi_persistent_s& res_set_type_c::set_semi_persistent()
{
  set(types::semi_persistent);
  return c.get<res_set_type_semi_persistent_s>();
}
res_set_type_aperiodic_s& res_set_type_c::set_aperiodic()
{
  set(types::aperiodic);
  return c.get<res_set_type_aperiodic_s>();
}
protocol_ie_single_container_s<res_set_type_ext_ies_o>& res_set_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<res_set_type_ext_ies_o>>();
}
void res_set_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::periodic:
      j.write_fieldname("periodic");
      c.get<res_set_type_periodic_s>().to_json(j);
      break;
    case types::semi_persistent:
      j.write_fieldname("semi-persistent");
      c.get<res_set_type_semi_persistent_s>().to_json(j);
      break;
    case types::aperiodic:
      j.write_fieldname("aperiodic");
      c.get<res_set_type_aperiodic_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<res_set_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "res_set_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE res_set_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::periodic:
      HANDLE_CODE(c.get<res_set_type_periodic_s>().pack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<res_set_type_semi_persistent_s>().pack(bref));
      break;
    case types::aperiodic:
      HANDLE_CODE(c.get<res_set_type_aperiodic_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<res_set_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "res_set_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_set_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::periodic:
      HANDLE_CODE(c.get<res_set_type_periodic_s>().unpack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<res_set_type_semi_persistent_s>().unpack(bref));
      break;
    case types::aperiodic:
      HANDLE_CODE(c.get<res_set_type_aperiodic_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<res_set_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "res_set_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* res_set_type_c::types_opts::to_string() const
{
  static const char* names[] = {"periodic", "semi-persistent", "aperiodic", "choice-extension"};
  return convert_enum_idx(names, 4, value, "res_set_type_c::types");
}

// SRSResourceSet ::= SEQUENCE
OCUDUASN_CODE srs_res_set_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, srs_res_set_id1, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, srs_res_id_list, 1, 16, integer_packer<uint8_t>(0, 63, false, true), true));
  HANDLE_CODE(res_set_type.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_set_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_res_set_id1, bref, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(srs_res_id_list, bref, 1, 16, integer_packer<uint8_t>(0, 63, false, true), true));
  HANDLE_CODE(res_set_type.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_res_set_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSResourceSetID1", srs_res_set_id1);
  j.start_array("sRSResourceID-List");
  for (const auto& e1 : srs_res_id_list) {
    j.write_int(e1);
  }
  j.end_array();
  j.write_fieldname("resourceSetType");
  res_set_type.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PosResourceSetTypePeriodic ::= SEQUENCE
OCUDUASN_CODE pos_res_set_type_periodic_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(posperiodic_set.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_res_set_type_periodic_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(posperiodic_set.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_res_set_type_periodic_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("posperiodicSet", "true");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* pos_res_set_type_periodic_s::posperiodic_set_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "pos_res_set_type_periodic_s::posperiodic_set_e_");
}

// PosResourceSetTypeSemi-persistent ::= SEQUENCE
OCUDUASN_CODE pos_res_set_type_semi_persistent_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(possemi_persistent_set.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_res_set_type_semi_persistent_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(possemi_persistent_set.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_res_set_type_semi_persistent_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("possemi-persistentSet", "true");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* pos_res_set_type_semi_persistent_s::possemi_persistent_set_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "pos_res_set_type_semi_persistent_s::possemi_persistent_set_e_");
}

// PosResourceSetTypeAperiodic ::= SEQUENCE
OCUDUASN_CODE pos_res_set_type_aperiodic_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, srs_res_trigger, (uint8_t)1u, (uint8_t)3u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_res_set_type_aperiodic_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_res_trigger, bref, (uint8_t)1u, (uint8_t)3u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_res_set_type_aperiodic_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSResourceTrigger", srs_res_trigger);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PosResourceSetType ::= CHOICE
void pos_res_set_type_c::destroy_()
{
  switch (type_) {
    case types::periodic:
      c.destroy<pos_res_set_type_periodic_s>();
      break;
    case types::semi_persistent:
      c.destroy<pos_res_set_type_semi_persistent_s>();
      break;
    case types::aperiodic:
      c.destroy<pos_res_set_type_aperiodic_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void pos_res_set_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::periodic:
      c.init<pos_res_set_type_periodic_s>();
      break;
    case types::semi_persistent:
      c.init<pos_res_set_type_semi_persistent_s>();
      break;
    case types::aperiodic:
      c.init<pos_res_set_type_aperiodic_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pos_res_set_type_c");
  }
}
pos_res_set_type_c::pos_res_set_type_c(const pos_res_set_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::periodic:
      c.init(other.c.get<pos_res_set_type_periodic_s>());
      break;
    case types::semi_persistent:
      c.init(other.c.get<pos_res_set_type_semi_persistent_s>());
      break;
    case types::aperiodic:
      c.init(other.c.get<pos_res_set_type_aperiodic_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pos_res_set_type_c");
  }
}
pos_res_set_type_c& pos_res_set_type_c::operator=(const pos_res_set_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::periodic:
      c.set(other.c.get<pos_res_set_type_periodic_s>());
      break;
    case types::semi_persistent:
      c.set(other.c.get<pos_res_set_type_semi_persistent_s>());
      break;
    case types::aperiodic:
      c.set(other.c.get<pos_res_set_type_aperiodic_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pos_res_set_type_c");
  }

  return *this;
}
pos_res_set_type_periodic_s& pos_res_set_type_c::set_periodic()
{
  set(types::periodic);
  return c.get<pos_res_set_type_periodic_s>();
}
pos_res_set_type_semi_persistent_s& pos_res_set_type_c::set_semi_persistent()
{
  set(types::semi_persistent);
  return c.get<pos_res_set_type_semi_persistent_s>();
}
pos_res_set_type_aperiodic_s& pos_res_set_type_c::set_aperiodic()
{
  set(types::aperiodic);
  return c.get<pos_res_set_type_aperiodic_s>();
}
protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>& pos_res_set_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>();
}
void pos_res_set_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::periodic:
      j.write_fieldname("periodic");
      c.get<pos_res_set_type_periodic_s>().to_json(j);
      break;
    case types::semi_persistent:
      j.write_fieldname("semi-persistent");
      c.get<pos_res_set_type_semi_persistent_s>().to_json(j);
      break;
    case types::aperiodic:
      j.write_fieldname("aperiodic");
      c.get<pos_res_set_type_aperiodic_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "pos_res_set_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE pos_res_set_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::periodic:
      HANDLE_CODE(c.get<pos_res_set_type_periodic_s>().pack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<pos_res_set_type_semi_persistent_s>().pack(bref));
      break;
    case types::aperiodic:
      HANDLE_CODE(c.get<pos_res_set_type_aperiodic_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "pos_res_set_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_res_set_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::periodic:
      HANDLE_CODE(c.get<pos_res_set_type_periodic_s>().unpack(bref));
      break;
    case types::semi_persistent:
      HANDLE_CODE(c.get<pos_res_set_type_semi_persistent_s>().unpack(bref));
      break;
    case types::aperiodic:
      HANDLE_CODE(c.get<pos_res_set_type_aperiodic_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "pos_res_set_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* pos_res_set_type_c::types_opts::to_string() const
{
  static const char* names[] = {"periodic", "semi-persistent", "aperiodic", "choice-extension"};
  return convert_enum_idx(names, 4, value, "pos_res_set_type_c::types");
}

// PosSRSResourceSet-Item ::= SEQUENCE
OCUDUASN_CODE pos_srs_res_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, possrs_res_set_id, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(
      pack_dyn_seq_of(bref, poss_rs_res_id_per_set_list, 1, 16, integer_packer<uint8_t>(0, 63, false, true), true));
  HANDLE_CODE(posres_set_type.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_srs_res_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(possrs_res_set_id, bref, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(
      unpack_dyn_seq_of(poss_rs_res_id_per_set_list, bref, 1, 16, integer_packer<uint8_t>(0, 63, false, true), true));
  HANDLE_CODE(posres_set_type.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_srs_res_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("possrsResourceSetID", possrs_res_set_id);
  j.start_array("possRSResourceIDPerSet-List");
  for (const auto& e1 : poss_rs_res_id_per_set_list) {
    j.write_int(e1);
  }
  j.end_array();
  j.write_fieldname("posresourceSetType");
  posres_set_type.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSConfig ::= SEQUENCE
OCUDUASN_CODE srs_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(srs_res_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(pos_srs_res_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(srs_res_set_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(pos_srs_res_set_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (srs_res_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, srs_res_list, 1, 64, true));
  }
  if (pos_srs_res_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_srs_res_list, 1, 64, true));
  }
  if (srs_res_set_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, srs_res_set_list, 1, 16, true));
  }
  if (pos_srs_res_set_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_srs_res_set_list, 1, 16, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool srs_res_list_present;
  HANDLE_CODE(bref.unpack(srs_res_list_present, 1));
  bool pos_srs_res_list_present;
  HANDLE_CODE(bref.unpack(pos_srs_res_list_present, 1));
  bool srs_res_set_list_present;
  HANDLE_CODE(bref.unpack(srs_res_set_list_present, 1));
  bool pos_srs_res_set_list_present;
  HANDLE_CODE(bref.unpack(pos_srs_res_set_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (srs_res_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(srs_res_list, bref, 1, 64, true));
  }
  if (pos_srs_res_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pos_srs_res_list, bref, 1, 64, true));
  }
  if (srs_res_set_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(srs_res_set_list, bref, 1, 16, true));
  }
  if (pos_srs_res_set_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pos_srs_res_set_list, bref, 1, 16, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_res_list.size() > 0) {
    j.start_array("sRSResource-List");
    for (const auto& e1 : srs_res_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pos_srs_res_list.size() > 0) {
    j.start_array("posSRSResource-List");
    for (const auto& e1 : pos_srs_res_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (srs_res_set_list.size() > 0) {
    j.start_array("sRSResourceSet-List");
    for (const auto& e1 : srs_res_set_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pos_srs_res_set_list.size() > 0) {
    j.start_array("posSRSResourceSet-List");
    for (const auto& e1 : pos_srs_res_set_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ActiveULBWP ::= SEQUENCE
OCUDUASN_CODE active_ul_bwp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(shift7dot5k_hz_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, location_and_bw, (uint32_t)0u, (uint32_t)37949u, true, true));
  HANDLE_CODE(subcarrier_spacing.pack(bref));
  HANDLE_CODE(cp.pack(bref));
  HANDLE_CODE(pack_integer(bref, tx_direct_current_location, (uint16_t)0u, (uint16_t)3301u, true, true));
  if (shift7dot5k_hz_present) {
    HANDLE_CODE(shift7dot5k_hz.pack(bref));
  }
  HANDLE_CODE(srs_cfg.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE active_ul_bwp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(shift7dot5k_hz_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(location_and_bw, bref, (uint32_t)0u, (uint32_t)37949u, true, true));
  HANDLE_CODE(subcarrier_spacing.unpack(bref));
  HANDLE_CODE(cp.unpack(bref));
  HANDLE_CODE(unpack_integer(tx_direct_current_location, bref, (uint16_t)0u, (uint16_t)3301u, true, true));
  if (shift7dot5k_hz_present) {
    HANDLE_CODE(shift7dot5k_hz.unpack(bref));
  }
  HANDLE_CODE(srs_cfg.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void active_ul_bwp_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("locationAndBandwidth", location_and_bw);
  j.write_str("subcarrierSpacing", subcarrier_spacing.to_string());
  j.write_str("cyclicPrefix", cp.to_string());
  j.write_int("txDirectCurrentLocation", tx_direct_current_location);
  if (shift7dot5k_hz_present) {
    j.write_str("shift7dot5kHz", "true");
  }
  j.write_fieldname("sRSConfig");
  srs_cfg.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* active_ul_bwp_s::subcarrier_spacing_opts::to_string() const
{
  static const char* names[] = {"kHz15", "kHz30", "kHz60", "kHz120", "kHz480", "kHz960"};
  return convert_enum_idx(names, 6, value, "active_ul_bwp_s::subcarrier_spacing_e_");
}
uint16_t active_ul_bwp_s::subcarrier_spacing_opts::to_number() const
{
  static const uint16_t numbers[] = {15, 30, 60, 120, 480, 960};
  return map_enum_number(numbers, 6, value, "active_ul_bwp_s::subcarrier_spacing_e_");
}

const char* active_ul_bwp_s::cp_opts::to_string() const
{
  static const char* names[] = {"normal", "extended"};
  return convert_enum_idx(names, 2, value, "active_ul_bwp_s::cp_e_");
}

const char* active_ul_bwp_s::shift7dot5k_hz_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "active_ul_bwp_s::shift7dot5k_hz_e_");
}

// RelativePathDelay-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t relative_path_delay_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {147, 148, 149, 150, 151, 152};
  return map_enum_number(names, 6, idx, "id");
}
bool relative_path_delay_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {147, 148, 149, 150, 151, 152};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e relative_path_delay_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 147:
      return crit_e::ignore;
    case 148:
      return crit_e::ignore;
    case 149:
      return crit_e::ignore;
    case 150:
      return crit_e::ignore;
    case 151:
      return crit_e::ignore;
    case 152:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
relative_path_delay_ext_ies_o::value_c relative_path_delay_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 147:
      ret.set(value_c::types::report_granularitykminus1_add_path);
      break;
    case 148:
      ret.set(value_c::types::report_granularitykminus2_add_path);
      break;
    case 149:
      ret.set(value_c::types::report_granularitykminus3_add_path);
      break;
    case 150:
      ret.set(value_c::types::report_granularitykminus4_add_path);
      break;
    case 151:
      ret.set(value_c::types::report_granularitykminus5_add_path);
      break;
    case 152:
      ret.set(value_c::types::report_granularitykminus6_add_path);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e relative_path_delay_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 147:
      return presence_e::mandatory;
    case 148:
      return presence_e::mandatory;
    case 149:
      return presence_e::mandatory;
    case 150:
      return presence_e::mandatory;
    case 151:
      return presence_e::mandatory;
    case 152:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void relative_path_delay_ext_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::report_granularitykminus1_add_path:
      c = uint16_t{};
      break;
    case types::report_granularitykminus2_add_path:
      c = uint16_t{};
      break;
    case types::report_granularitykminus3_add_path:
      c = uint32_t{};
      break;
    case types::report_granularitykminus4_add_path:
      c = uint32_t{};
      break;
    case types::report_granularitykminus5_add_path:
      c = uint32_t{};
      break;
    case types::report_granularitykminus6_add_path:
      c = uint32_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_ext_ies_o::value_c");
  }
}
uint16_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus1_add_path()
{
  assert_choice_type(types::report_granularitykminus1_add_path, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus2_add_path()
{
  assert_choice_type(types::report_granularitykminus2_add_path, type_, "Value");
  return c.get<uint16_t>();
}
uint32_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus3_add_path()
{
  assert_choice_type(types::report_granularitykminus3_add_path, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus4_add_path()
{
  assert_choice_type(types::report_granularitykminus4_add_path, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus5_add_path()
{
  assert_choice_type(types::report_granularitykminus5_add_path, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus6_add_path()
{
  assert_choice_type(types::report_granularitykminus6_add_path, type_, "Value");
  return c.get<uint32_t>();
}
const uint16_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus1_add_path() const
{
  assert_choice_type(types::report_granularitykminus1_add_path, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus2_add_path() const
{
  assert_choice_type(types::report_granularitykminus2_add_path, type_, "Value");
  return c.get<uint16_t>();
}
const uint32_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus3_add_path() const
{
  assert_choice_type(types::report_granularitykminus3_add_path, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus4_add_path() const
{
  assert_choice_type(types::report_granularitykminus4_add_path, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus5_add_path() const
{
  assert_choice_type(types::report_granularitykminus5_add_path, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& relative_path_delay_ext_ies_o::value_c::report_granularitykminus6_add_path() const
{
  assert_choice_type(types::report_granularitykminus6_add_path, type_, "Value");
  return c.get<uint32_t>();
}
void relative_path_delay_ext_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::report_granularitykminus1_add_path:
      j.write_int(c.get<uint16_t>());
      break;
    case types::report_granularitykminus2_add_path:
      j.write_int(c.get<uint16_t>());
      break;
    case types::report_granularitykminus3_add_path:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus4_add_path:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus5_add_path:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus6_add_path:
      j.write_int(c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_ext_ies_o::value_c");
  }
}
OCUDUASN_CODE relative_path_delay_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::report_granularitykminus1_add_path:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)32701u, false, true));
      break;
    case types::report_granularitykminus2_add_path:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)65401u, false, true));
      break;
    case types::report_granularitykminus3_add_path:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)130801u, false, true));
      break;
    case types::report_granularitykminus4_add_path:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)261601u, false, true));
      break;
    case types::report_granularitykminus5_add_path:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)523201u, false, true));
      break;
    case types::report_granularitykminus6_add_path:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)1046401u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_ext_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE relative_path_delay_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::report_granularitykminus1_add_path:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)32701u, false, true));
      break;
    case types::report_granularitykminus2_add_path:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)65401u, false, true));
      break;
    case types::report_granularitykminus3_add_path:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)130801u, false, true));
      break;
    case types::report_granularitykminus4_add_path:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)261601u, false, true));
      break;
    case types::report_granularitykminus5_add_path:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)523201u, false, true));
      break;
    case types::report_granularitykminus6_add_path:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)1046401u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_ext_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* relative_path_delay_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..32701)",
                                "INTEGER (0..65401)",
                                "INTEGER (0..130801)",
                                "INTEGER (0..261601)",
                                "INTEGER (0..523201)",
                                "INTEGER (0..1046401)"};
  return convert_enum_idx(names, 6, value, "relative_path_delay_ext_ies_o::value_c::types");
}

// RelativePathDelay ::= CHOICE
void relative_path_delay_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void relative_path_delay_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::k0:
      break;
    case types::k1:
      break;
    case types::k2:
      break;
    case types::k3:
      break;
    case types::k4:
      break;
    case types::k5:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_c");
  }
}
relative_path_delay_c::relative_path_delay_c(const relative_path_delay_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::k0:
      c.init(other.c.get<uint16_t>());
      break;
    case types::k1:
      c.init(other.c.get<uint16_t>());
      break;
    case types::k2:
      c.init(other.c.get<uint16_t>());
      break;
    case types::k3:
      c.init(other.c.get<uint16_t>());
      break;
    case types::k4:
      c.init(other.c.get<uint16_t>());
      break;
    case types::k5:
      c.init(other.c.get<uint16_t>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_c");
  }
}
relative_path_delay_c& relative_path_delay_c::operator=(const relative_path_delay_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::k0:
      c.set(other.c.get<uint16_t>());
      break;
    case types::k1:
      c.set(other.c.get<uint16_t>());
      break;
    case types::k2:
      c.set(other.c.get<uint16_t>());
      break;
    case types::k3:
      c.set(other.c.get<uint16_t>());
      break;
    case types::k4:
      c.set(other.c.get<uint16_t>());
      break;
    case types::k5:
      c.set(other.c.get<uint16_t>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_c");
  }

  return *this;
}
uint16_t& relative_path_delay_c::set_k0()
{
  set(types::k0);
  return c.get<uint16_t>();
}
uint16_t& relative_path_delay_c::set_k1()
{
  set(types::k1);
  return c.get<uint16_t>();
}
uint16_t& relative_path_delay_c::set_k2()
{
  set(types::k2);
  return c.get<uint16_t>();
}
uint16_t& relative_path_delay_c::set_k3()
{
  set(types::k3);
  return c.get<uint16_t>();
}
uint16_t& relative_path_delay_c::set_k4()
{
  set(types::k4);
  return c.get<uint16_t>();
}
uint16_t& relative_path_delay_c::set_k5()
{
  set(types::k5);
  return c.get<uint16_t>();
}
protocol_ie_single_container_s<relative_path_delay_ext_ies_o>& relative_path_delay_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>();
}
void relative_path_delay_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::k0:
      j.write_int("k0", c.get<uint16_t>());
      break;
    case types::k1:
      j.write_int("k1", c.get<uint16_t>());
      break;
    case types::k2:
      j.write_int("k2", c.get<uint16_t>());
      break;
    case types::k3:
      j.write_int("k3", c.get<uint16_t>());
      break;
    case types::k4:
      j.write_int("k4", c.get<uint16_t>());
      break;
    case types::k5:
      j.write_int("k5", c.get<uint16_t>());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_c");
  }
  j.end_obj();
}
OCUDUASN_CODE relative_path_delay_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::k0:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)16351u, false, true));
      break;
    case types::k1:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)8176u, false, true));
      break;
    case types::k2:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4088u, false, true));
      break;
    case types::k3:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)2044u, false, true));
      break;
    case types::k4:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1022u, false, true));
      break;
    case types::k5:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)511u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE relative_path_delay_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::k0:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)16351u, false, true));
      break;
    case types::k1:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)8176u, false, true));
      break;
    case types::k2:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4088u, false, true));
      break;
    case types::k3:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)2044u, false, true));
      break;
    case types::k4:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1022u, false, true));
      break;
    case types::k5:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)511u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "relative_path_delay_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* relative_path_delay_c::types_opts::to_string() const
{
  static const char* names[] = {"k0", "k1", "k2", "k3", "k4", "k5", "choice-Extension"};
  return convert_enum_idx(names, 7, value, "relative_path_delay_c::types");
}
uint8_t relative_path_delay_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1, 2, 3, 4, 5};
  return map_enum_number(numbers, 6, value, "relative_path_delay_c::types");
}

// TrpMeasurementTimingQuality ::= SEQUENCE
OCUDUASN_CODE trp_meas_timing_quality_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, meas_quality, (uint8_t)0u, (uint8_t)31u, false, true));
  HANDLE_CODE(resolution.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_timing_quality_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(meas_quality, bref, (uint8_t)0u, (uint8_t)31u, false, true));
  HANDLE_CODE(resolution.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_meas_timing_quality_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("measurementQuality", meas_quality);
  j.write_str("resolution", resolution.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* trp_meas_timing_quality_s::resolution_opts::to_string() const
{
  static const char* names[] = {"m0dot1", "m1", "m10", "m30"};
  return convert_enum_idx(names, 4, value, "trp_meas_timing_quality_s::resolution_e_");
}
float trp_meas_timing_quality_s::resolution_opts::to_number() const
{
  static const float numbers[] = {0.1, 1.0, 10.0, 30.0};
  return map_enum_number(numbers, 4, value, "trp_meas_timing_quality_s::resolution_e_");
}
const char* trp_meas_timing_quality_s::resolution_opts::to_number_string() const
{
  static const char* number_strs[] = {"0.1", "1", "10", "30"};
  return convert_enum_idx(number_strs, 4, value, "trp_meas_timing_quality_s::resolution_e_");
}

// TrpMeasurementAngleQuality ::= SEQUENCE
OCUDUASN_CODE trp_meas_angle_quality_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(zenith_quality_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, azimuth_quality, (uint16_t)0u, (uint16_t)255u, false, true));
  if (zenith_quality_present) {
    HANDLE_CODE(pack_integer(bref, zenith_quality, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  HANDLE_CODE(resolution.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_angle_quality_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(zenith_quality_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(azimuth_quality, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (zenith_quality_present) {
    HANDLE_CODE(unpack_integer(zenith_quality, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  HANDLE_CODE(resolution.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_meas_angle_quality_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("azimuthQuality", azimuth_quality);
  if (zenith_quality_present) {
    j.write_int("zenithQuality", zenith_quality);
  }
  j.write_str("resolution", "deg0dot1");
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* trp_meas_angle_quality_s::resolution_opts::to_string() const
{
  static const char* names[] = {"deg0dot1"};
  return convert_enum_idx(names, 1, value, "trp_meas_angle_quality_s::resolution_e_");
}
float trp_meas_angle_quality_s::resolution_opts::to_number() const
{
  static const float numbers[] = {0.1};
  return map_enum_number(numbers, 1, value, "trp_meas_angle_quality_s::resolution_e_");
}
const char* trp_meas_angle_quality_s::resolution_opts::to_number_string() const
{
  static const char* number_strs[] = {"0.1"};
  return convert_enum_idx(number_strs, 1, value, "trp_meas_angle_quality_s::resolution_e_");
}

// TRPPhaseQuality ::= SEQUENCE
OCUDUASN_CODE trp_phase_quality_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, phase_quality_idx, (uint8_t)0u, (uint8_t)179u, false, true));
  HANDLE_CODE(resolution.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_phase_quality_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(phase_quality_idx, bref, (uint8_t)0u, (uint8_t)179u, false, true));
  HANDLE_CODE(resolution.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_phase_quality_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("phaseQualityIndex", phase_quality_idx);
  j.write_str("resolution", resolution.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* trp_phase_quality_s::resolution_opts::to_string() const
{
  static const char* names[] = {"deg0dot1", "deg1"};
  return convert_enum_idx(names, 2, value, "trp_phase_quality_s::resolution_e_");
}
float trp_phase_quality_s::resolution_opts::to_number() const
{
  static const float numbers[] = {0.1, 1.0};
  return map_enum_number(numbers, 2, value, "trp_phase_quality_s::resolution_e_");
}
const char* trp_phase_quality_s::resolution_opts::to_number_string() const
{
  static const char* number_strs[] = {"0.1", "1"};
  return convert_enum_idx(number_strs, 2, value, "trp_phase_quality_s::resolution_e_");
}

// TrpMeasurementQuality-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t trp_meas_quality_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {133};
  return map_enum_number(names, 1, idx, "id");
}
bool trp_meas_quality_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 133 == id;
}
crit_e trp_meas_quality_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 133) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
trp_meas_quality_ext_ies_o::value_c trp_meas_quality_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 133) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_meas_quality_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 133) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void trp_meas_quality_ext_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE trp_meas_quality_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_quality_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* trp_meas_quality_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TRPPhaseQuality"};
  return convert_enum_idx(names, 1, value, "trp_meas_quality_ext_ies_o::value_c::types");
}

// TrpMeasurementQuality ::= CHOICE
void trp_meas_quality_c::destroy_()
{
  switch (type_) {
    case types::timing_meas_quality:
      c.destroy<trp_meas_timing_quality_s>();
      break;
    case types::angle_meas_quality:
      c.destroy<trp_meas_angle_quality_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void trp_meas_quality_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::timing_meas_quality:
      c.init<trp_meas_timing_quality_s>();
      break;
    case types::angle_meas_quality:
      c.init<trp_meas_angle_quality_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_quality_c");
  }
}
trp_meas_quality_c::trp_meas_quality_c(const trp_meas_quality_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::timing_meas_quality:
      c.init(other.c.get<trp_meas_timing_quality_s>());
      break;
    case types::angle_meas_quality:
      c.init(other.c.get<trp_meas_angle_quality_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_quality_c");
  }
}
trp_meas_quality_c& trp_meas_quality_c::operator=(const trp_meas_quality_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::timing_meas_quality:
      c.set(other.c.get<trp_meas_timing_quality_s>());
      break;
    case types::angle_meas_quality:
      c.set(other.c.get<trp_meas_angle_quality_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_quality_c");
  }

  return *this;
}
trp_meas_timing_quality_s& trp_meas_quality_c::set_timing_meas_quality()
{
  set(types::timing_meas_quality);
  return c.get<trp_meas_timing_quality_s>();
}
trp_meas_angle_quality_s& trp_meas_quality_c::set_angle_meas_quality()
{
  set(types::angle_meas_quality);
  return c.get<trp_meas_angle_quality_s>();
}
protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>& trp_meas_quality_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>();
}
void trp_meas_quality_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::timing_meas_quality:
      j.write_fieldname("timingMeasQuality");
      c.get<trp_meas_timing_quality_s>().to_json(j);
      break;
    case types::angle_meas_quality:
      j.write_fieldname("angleMeasQuality");
      c.get<trp_meas_angle_quality_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_quality_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_meas_quality_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::timing_meas_quality:
      HANDLE_CODE(c.get<trp_meas_timing_quality_s>().pack(bref));
      break;
    case types::angle_meas_quality:
      HANDLE_CODE(c.get<trp_meas_angle_quality_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_quality_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_quality_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::timing_meas_quality:
      HANDLE_CODE(c.get<trp_meas_timing_quality_s>().unpack(bref));
      break;
    case types::angle_meas_quality:
      HANDLE_CODE(c.get<trp_meas_angle_quality_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_quality_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_meas_quality_c::types_opts::to_string() const
{
  static const char* names[] = {"timingMeasQuality", "angleMeasQuality", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "trp_meas_quality_c::types");
}

// LCS-to-GCS-Translation ::= SEQUENCE
OCUDUASN_CODE lcs_to_gcs_translation_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, alpha, (uint16_t)0u, (uint16_t)3599u, false, true));
  HANDLE_CODE(pack_integer(bref, beta, (uint16_t)0u, (uint16_t)3599u, false, true));
  HANDLE_CODE(pack_integer(bref, gamma, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE lcs_to_gcs_translation_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(alpha, bref, (uint16_t)0u, (uint16_t)3599u, false, true));
  HANDLE_CODE(unpack_integer(beta, bref, (uint16_t)0u, (uint16_t)3599u, false, true));
  HANDLE_CODE(unpack_integer(gamma, bref, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void lcs_to_gcs_translation_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("alpha", alpha);
  j.write_int("beta", beta);
  j.write_int("gamma", gamma);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// UL-AoA ::= SEQUENCE
OCUDUASN_CODE ul_ao_a_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(zenith_ao_a_present, 1));
  HANDLE_CODE(bref.pack(lcs_to_gcs_translation_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, azimuth_ao_a, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (zenith_ao_a_present) {
    HANDLE_CODE(pack_integer(bref, zenith_ao_a, (uint16_t)0u, (uint16_t)1799u, false, true));
  }
  if (lcs_to_gcs_translation_present) {
    HANDLE_CODE(lcs_to_gcs_translation.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ao_a_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(zenith_ao_a_present, 1));
  HANDLE_CODE(bref.unpack(lcs_to_gcs_translation_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(azimuth_ao_a, bref, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (zenith_ao_a_present) {
    HANDLE_CODE(unpack_integer(zenith_ao_a, bref, (uint16_t)0u, (uint16_t)1799u, false, true));
  }
  if (lcs_to_gcs_translation_present) {
    HANDLE_CODE(lcs_to_gcs_translation.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ul_ao_a_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("azimuthAoA", azimuth_ao_a);
  if (zenith_ao_a_present) {
    j.write_int("zenithAoA", zenith_ao_a);
  }
  if (lcs_to_gcs_translation_present) {
    j.write_fieldname("lCS-to-GCS-Translation");
    lcs_to_gcs_translation.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ZoA ::= SEQUENCE
OCUDUASN_CODE zo_a_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(lcs_to_gcs_translation_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, zenith_ao_a, (uint16_t)0u, (uint16_t)1799u, false, true));
  if (lcs_to_gcs_translation_present) {
    HANDLE_CODE(lcs_to_gcs_translation.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE zo_a_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(lcs_to_gcs_translation_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(zenith_ao_a, bref, (uint16_t)0u, (uint16_t)1799u, false, true));
  if (lcs_to_gcs_translation_present) {
    HANDLE_CODE(lcs_to_gcs_translation.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void zo_a_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("zenithAoA", zenith_ao_a);
  if (lcs_to_gcs_translation_present) {
    j.write_fieldname("lCS-to-GCS-Translation");
    lcs_to_gcs_translation.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MultipleULAoA-Item ::= CHOICE
void multiple_ul_ao_a_item_c::destroy_()
{
  switch (type_) {
    case types::ul_ao_a:
      c.destroy<ul_ao_a_s>();
      break;
    case types::ul_zo_a:
      c.destroy<zo_a_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void multiple_ul_ao_a_item_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ul_ao_a:
      c.init<ul_ao_a_s>();
      break;
    case types::ul_zo_a:
      c.init<zo_a_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "multiple_ul_ao_a_item_c");
  }
}
multiple_ul_ao_a_item_c::multiple_ul_ao_a_item_c(const multiple_ul_ao_a_item_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ul_ao_a:
      c.init(other.c.get<ul_ao_a_s>());
      break;
    case types::ul_zo_a:
      c.init(other.c.get<zo_a_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "multiple_ul_ao_a_item_c");
  }
}
multiple_ul_ao_a_item_c& multiple_ul_ao_a_item_c::operator=(const multiple_ul_ao_a_item_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ul_ao_a:
      c.set(other.c.get<ul_ao_a_s>());
      break;
    case types::ul_zo_a:
      c.set(other.c.get<zo_a_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "multiple_ul_ao_a_item_c");
  }

  return *this;
}
ul_ao_a_s& multiple_ul_ao_a_item_c::set_ul_ao_a()
{
  set(types::ul_ao_a);
  return c.get<ul_ao_a_s>();
}
zo_a_s& multiple_ul_ao_a_item_c::set_ul_zo_a()
{
  set(types::ul_zo_a);
  return c.get<zo_a_s>();
}
protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>& multiple_ul_ao_a_item_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>();
}
void multiple_ul_ao_a_item_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ul_ao_a:
      j.write_fieldname("uL-AoA");
      c.get<ul_ao_a_s>().to_json(j);
      break;
    case types::ul_zo_a:
      j.write_fieldname("ul-ZoA");
      c.get<zo_a_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "multiple_ul_ao_a_item_c");
  }
  j.end_obj();
}
OCUDUASN_CODE multiple_ul_ao_a_item_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ul_ao_a:
      HANDLE_CODE(c.get<ul_ao_a_s>().pack(bref));
      break;
    case types::ul_zo_a:
      HANDLE_CODE(c.get<zo_a_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "multiple_ul_ao_a_item_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multiple_ul_ao_a_item_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ul_ao_a:
      HANDLE_CODE(c.get<ul_ao_a_s>().unpack(bref));
      break;
    case types::ul_zo_a:
      HANDLE_CODE(c.get<zo_a_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "multiple_ul_ao_a_item_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* multiple_ul_ao_a_item_c::types_opts::to_string() const
{
  static const char* names[] = {"uL-AoA", "ul-ZoA", "choice-extension"};
  return convert_enum_idx(names, 3, value, "multiple_ul_ao_a_item_c::types");
}

// MultipleULAoA ::= SEQUENCE
OCUDUASN_CODE multiple_ul_ao_a_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, multiple_ul_ao_a, 1, 8, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multiple_ul_ao_a_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(multiple_ul_ao_a, bref, 1, 8, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void multiple_ul_ao_a_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("multipleULAoA");
  for (const auto& e1 : multiple_ul_ao_a) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// UL-SRS-RSRPP ::= SEQUENCE
OCUDUASN_CODE ul_srs_rsrp_p_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, first_path_rsrp_p, (uint8_t)0u, (uint8_t)126u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_srs_rsrp_p_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(first_path_rsrp_p, bref, (uint8_t)0u, (uint8_t)126u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ul_srs_rsrp_p_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("firstPathRSRPP", first_path_rsrp_p);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// AdditionalPathListItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t add_path_list_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {74, 96};
  return map_enum_number(names, 2, idx, "id");
}
bool add_path_list_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {74, 96};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e add_path_list_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 74:
      return crit_e::ignore;
    case 96:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
add_path_list_item_ext_ies_o::ext_c add_path_list_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 74:
      ret.set(ext_c::types::multiple_ul_ao_a);
      break;
    case 96:
      ret.set(ext_c::types::path_pwr);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e add_path_list_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 74:
      return presence_e::optional;
    case 96:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void add_path_list_item_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::multiple_ul_ao_a:
      c = multiple_ul_ao_a_s{};
      break;
    case types::path_pwr:
      c = ul_srs_rsrp_p_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "add_path_list_item_ext_ies_o::ext_c");
  }
}
multiple_ul_ao_a_s& add_path_list_item_ext_ies_o::ext_c::multiple_ul_ao_a()
{
  assert_choice_type(types::multiple_ul_ao_a, type_, "Extension");
  return c.get<multiple_ul_ao_a_s>();
}
ul_srs_rsrp_p_s& add_path_list_item_ext_ies_o::ext_c::path_pwr()
{
  assert_choice_type(types::path_pwr, type_, "Extension");
  return c.get<ul_srs_rsrp_p_s>();
}
const multiple_ul_ao_a_s& add_path_list_item_ext_ies_o::ext_c::multiple_ul_ao_a() const
{
  assert_choice_type(types::multiple_ul_ao_a, type_, "Extension");
  return c.get<multiple_ul_ao_a_s>();
}
const ul_srs_rsrp_p_s& add_path_list_item_ext_ies_o::ext_c::path_pwr() const
{
  assert_choice_type(types::path_pwr, type_, "Extension");
  return c.get<ul_srs_rsrp_p_s>();
}
void add_path_list_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::multiple_ul_ao_a:
      c.get<multiple_ul_ao_a_s>().to_json(j);
      break;
    case types::path_pwr:
      c.get<ul_srs_rsrp_p_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "add_path_list_item_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE add_path_list_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::multiple_ul_ao_a:
      HANDLE_CODE(c.get<multiple_ul_ao_a_s>().pack(bref));
      break;
    case types::path_pwr:
      HANDLE_CODE(c.get<ul_srs_rsrp_p_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "add_path_list_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE add_path_list_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::multiple_ul_ao_a:
      HANDLE_CODE(c.get<multiple_ul_ao_a_s>().unpack(bref));
      break;
    case types::path_pwr:
      HANDLE_CODE(c.get<ul_srs_rsrp_p_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "add_path_list_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* add_path_list_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"MultipleULAoA", "UL-SRS-RSRPP"};
  return convert_enum_idx(names, 2, value, "add_path_list_item_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<add_path_list_item_ext_ies_o>;

OCUDUASN_CODE add_path_list_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += multiple_ul_ao_a_present ? 1 : 0;
  nof_ies += path_pwr_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (multiple_ul_ao_a_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)74, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(multiple_ul_ao_a.pack(bref));
  }
  if (path_pwr_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)96, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(path_pwr.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE add_path_list_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != add_path_list_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 74: {
        multiple_ul_ao_a_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(multiple_ul_ao_a.unpack(bref));
        break;
      }
      case 96: {
        path_pwr_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(path_pwr.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void add_path_list_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (multiple_ul_ao_a_present) {
    j.start_obj();
    j.write_int("id", 74);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    multiple_ul_ao_a.to_json(j);
    j.end_obj();
  }
  if (path_pwr_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    path_pwr.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// AdditionalPathListItem ::= SEQUENCE
OCUDUASN_CODE add_path_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(path_quality_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(relative_time_of_path.pack(bref));
  if (path_quality_present) {
    HANDLE_CODE(path_quality.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE add_path_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(path_quality_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(relative_time_of_path.unpack(bref));
  if (path_quality_present) {
    HANDLE_CODE(path_quality.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void add_path_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("relativeTimeOfPath");
  relative_time_of_path.to_json(j);
  if (path_quality_present) {
    j.write_fieldname("pathQuality");
    path_quality.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DL-PRS-ResourceSet-Item ::= SEQUENCE
OCUDUASN_CODE dl_prs_res_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, dl_prs_res_set_idx, (uint8_t)1u, (uint8_t)8u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_res_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(dl_prs_res_set_idx, bref, (uint8_t)1u, (uint8_t)8u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_prs_res_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dl-prs-ResourceSetIndex", dl_prs_res_set_idx);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// AggregatedPRSResourceSet-Item ::= SEQUENCE
OCUDUASN_CODE aggr_prs_res_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, dl_prs_res_set_list, 1, 3, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aggr_prs_res_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(dl_prs_res_set_list, bref, 1, 3, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void aggr_prs_res_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("dl-PRS-ResourceSet-List");
  for (const auto& e1 : dl_prs_res_set_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// AggregatedPosSRSResourceID-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t aggr_pos_srs_res_id_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {154, 156, 155};
  return map_enum_number(names, 3, idx, "id");
}
bool aggr_pos_srs_res_id_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {154, 156, 155};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e aggr_pos_srs_res_id_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 154:
      return crit_e::ignore;
    case 156:
      return crit_e::ignore;
    case 155:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
aggr_pos_srs_res_id_item_ext_ies_o::ext_c aggr_pos_srs_res_id_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 154:
      ret.set(ext_c::types::point_a);
      break;
    case 156:
      ret.set(ext_c::types::scs_specific_carrier);
      break;
    case 155:
      ret.set(ext_c::types::nr_pci);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e aggr_pos_srs_res_id_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 154:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    case 155:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void aggr_pos_srs_res_id_item_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::point_a:
      c = uint32_t{};
      break;
    case types::scs_specific_carrier:
      c = scs_specific_carrier_s{};
      break;
    case types::nr_pci:
      c = uint16_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "aggr_pos_srs_res_id_item_ext_ies_o::ext_c");
  }
}
uint32_t& aggr_pos_srs_res_id_item_ext_ies_o::ext_c::point_a()
{
  assert_choice_type(types::point_a, type_, "Extension");
  return c.get<uint32_t>();
}
scs_specific_carrier_s& aggr_pos_srs_res_id_item_ext_ies_o::ext_c::scs_specific_carrier()
{
  assert_choice_type(types::scs_specific_carrier, type_, "Extension");
  return c.get<scs_specific_carrier_s>();
}
uint16_t& aggr_pos_srs_res_id_item_ext_ies_o::ext_c::nr_pci()
{
  assert_choice_type(types::nr_pci, type_, "Extension");
  return c.get<uint16_t>();
}
const uint32_t& aggr_pos_srs_res_id_item_ext_ies_o::ext_c::point_a() const
{
  assert_choice_type(types::point_a, type_, "Extension");
  return c.get<uint32_t>();
}
const scs_specific_carrier_s& aggr_pos_srs_res_id_item_ext_ies_o::ext_c::scs_specific_carrier() const
{
  assert_choice_type(types::scs_specific_carrier, type_, "Extension");
  return c.get<scs_specific_carrier_s>();
}
const uint16_t& aggr_pos_srs_res_id_item_ext_ies_o::ext_c::nr_pci() const
{
  assert_choice_type(types::nr_pci, type_, "Extension");
  return c.get<uint16_t>();
}
void aggr_pos_srs_res_id_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::point_a:
      j.write_int(c.get<uint32_t>());
      break;
    case types::scs_specific_carrier:
      c.get<scs_specific_carrier_s>().to_json(j);
      break;
    case types::nr_pci:
      j.write_int(c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "aggr_pos_srs_res_id_item_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE aggr_pos_srs_res_id_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::point_a:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)3279165u, false, true));
      break;
    case types::scs_specific_carrier:
      HANDLE_CODE(c.get<scs_specific_carrier_s>().pack(bref));
      break;
    case types::nr_pci:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1007u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "aggr_pos_srs_res_id_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aggr_pos_srs_res_id_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::point_a:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
      break;
    case types::scs_specific_carrier:
      HANDLE_CODE(c.get<scs_specific_carrier_s>().unpack(bref));
      break;
    case types::nr_pci:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1007u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "aggr_pos_srs_res_id_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* aggr_pos_srs_res_id_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..3279165)", "SCS-SpecificCarrier", "INTEGER (0..1007)"};
  return convert_enum_idx(names, 3, value, "aggr_pos_srs_res_id_item_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<aggr_pos_srs_res_id_item_ext_ies_o>;

OCUDUASN_CODE aggr_pos_srs_res_id_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += nr_pci_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)154, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, point_a, (uint32_t)0u, (uint32_t)3279165u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)156, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(scs_specific_carrier.pack(bref));
  }
  if (nr_pci_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)155, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, nr_pci, (uint16_t)0u, (uint16_t)1007u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aggr_pos_srs_res_id_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != aggr_pos_srs_res_id_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 154: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(point_a, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(scs_specific_carrier.unpack(bref));
        break;
      }
      case 155: {
        nr_pci_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(nr_pci, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void aggr_pos_srs_res_id_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 154);
  j.write_str("criticality", "ignore");
  j.write_int("extensionValue", point_a);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "ignore");
  j.write_fieldname("extensionValue");
  scs_specific_carrier.to_json(j);
  j.end_obj();
  if (nr_pci_present) {
    j.start_obj();
    j.write_int("id", 155);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", nr_pci);
    j.end_obj();
  }
  j.end_array();
}

// AggregatedPosSRSResourceID-Item ::= SEQUENCE
OCUDUASN_CODE aggr_pos_srs_res_id_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, srs_pos_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aggr_pos_srs_res_id_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_pos_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void aggr_pos_srs_res_id_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSPosResource-ID", srs_pos_res_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Expected-Azimuth-AoA ::= SEQUENCE
OCUDUASN_CODE expected_azimuth_ao_a_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, expected_azimuth_ao_a_value, (uint16_t)0u, (uint16_t)3599u, false, true));
  HANDLE_CODE(pack_integer(bref, expected_azimuth_ao_a_uncertainty, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE expected_azimuth_ao_a_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(expected_azimuth_ao_a_value, bref, (uint16_t)0u, (uint16_t)3599u, false, true));
  HANDLE_CODE(unpack_integer(expected_azimuth_ao_a_uncertainty, bref, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void expected_azimuth_ao_a_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("expected-Azimuth-AoA-value", expected_azimuth_ao_a_value);
  j.write_int("expected-Azimuth-AoA-uncertainty", expected_azimuth_ao_a_uncertainty);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Expected-Zenith-AoA ::= SEQUENCE
OCUDUASN_CODE expected_zenith_ao_a_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, expected_zenith_ao_a_value, (uint16_t)0u, (uint16_t)1799u, false, true));
  HANDLE_CODE(pack_integer(bref, expected_zenith_ao_a_uncertainty, (uint16_t)0u, (uint16_t)1799u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE expected_zenith_ao_a_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(expected_zenith_ao_a_value, bref, (uint16_t)0u, (uint16_t)1799u, false, true));
  HANDLE_CODE(unpack_integer(expected_zenith_ao_a_uncertainty, bref, (uint16_t)0u, (uint16_t)1799u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void expected_zenith_ao_a_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("expected-Zenith-AoA-value", expected_zenith_ao_a_value);
  j.write_int("expected-Zenith-AoA-uncertainty", expected_zenith_ao_a_uncertainty);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Expected-UL-AoA ::= SEQUENCE
OCUDUASN_CODE expected_ul_ao_a_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(expected_zenith_ao_a_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(expected_azimuth_ao_a.pack(bref));
  if (expected_zenith_ao_a_present) {
    HANDLE_CODE(expected_zenith_ao_a.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE expected_ul_ao_a_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(expected_zenith_ao_a_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(expected_azimuth_ao_a.unpack(bref));
  if (expected_zenith_ao_a_present) {
    HANDLE_CODE(expected_zenith_ao_a.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void expected_ul_ao_a_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("expected-Azimuth-AoA");
  expected_azimuth_ao_a.to_json(j);
  if (expected_zenith_ao_a_present) {
    j.write_fieldname("expected-Zenith-AoA");
    expected_zenith_ao_a.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Expected-ZoA-only ::= SEQUENCE
OCUDUASN_CODE expected_zo_a_only_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(expected_zo_a_only.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE expected_zo_a_only_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(expected_zo_a_only.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void expected_zo_a_only_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("expected-ZoA-only");
  expected_zo_a_only.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// AngleMeasurementType ::= CHOICE
void angle_meas_type_c::destroy_()
{
  switch (type_) {
    case types::expected_ul_ao_a:
      c.destroy<expected_ul_ao_a_s>();
      break;
    case types::expected_zo_a:
      c.destroy<expected_zo_a_only_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void angle_meas_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::expected_ul_ao_a:
      c.init<expected_ul_ao_a_s>();
      break;
    case types::expected_zo_a:
      c.init<expected_zo_a_only_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "angle_meas_type_c");
  }
}
angle_meas_type_c::angle_meas_type_c(const angle_meas_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::expected_ul_ao_a:
      c.init(other.c.get<expected_ul_ao_a_s>());
      break;
    case types::expected_zo_a:
      c.init(other.c.get<expected_zo_a_only_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "angle_meas_type_c");
  }
}
angle_meas_type_c& angle_meas_type_c::operator=(const angle_meas_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::expected_ul_ao_a:
      c.set(other.c.get<expected_ul_ao_a_s>());
      break;
    case types::expected_zo_a:
      c.set(other.c.get<expected_zo_a_only_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "angle_meas_type_c");
  }

  return *this;
}
expected_ul_ao_a_s& angle_meas_type_c::set_expected_ul_ao_a()
{
  set(types::expected_ul_ao_a);
  return c.get<expected_ul_ao_a_s>();
}
expected_zo_a_only_s& angle_meas_type_c::set_expected_zo_a()
{
  set(types::expected_zo_a);
  return c.get<expected_zo_a_only_s>();
}
protocol_ie_single_container_s<angle_meas_type_ext_ies_o>& angle_meas_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>();
}
void angle_meas_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::expected_ul_ao_a:
      j.write_fieldname("expected-ULAoA");
      c.get<expected_ul_ao_a_s>().to_json(j);
      break;
    case types::expected_zo_a:
      j.write_fieldname("expected-ZoA");
      c.get<expected_zo_a_only_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "angle_meas_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE angle_meas_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::expected_ul_ao_a:
      HANDLE_CODE(c.get<expected_ul_ao_a_s>().pack(bref));
      break;
    case types::expected_zo_a:
      HANDLE_CODE(c.get<expected_zo_a_only_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "angle_meas_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE angle_meas_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::expected_ul_ao_a:
      HANDLE_CODE(c.get<expected_ul_ao_a_s>().unpack(bref));
      break;
    case types::expected_zo_a:
      HANDLE_CODE(c.get<expected_zo_a_only_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "angle_meas_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* angle_meas_type_c::types_opts::to_string() const
{
  static const char* names[] = {"expected-ULAoA", "expected-ZoA", "choice-extension"};
  return convert_enum_idx(names, 3, value, "angle_meas_type_c::types");
}

// AoA-AssistanceInfo ::= SEQUENCE
OCUDUASN_CODE ao_a_assist_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(lcs_to_gcs_translation_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(angle_meas.pack(bref));
  if (lcs_to_gcs_translation_present) {
    HANDLE_CODE(lcs_to_gcs_translation.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ao_a_assist_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(lcs_to_gcs_translation_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(angle_meas.unpack(bref));
  if (lcs_to_gcs_translation_present) {
    HANDLE_CODE(lcs_to_gcs_translation.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ao_a_assist_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("angleMeasurement");
  angle_meas.to_json(j);
  if (lcs_to_gcs_translation_present) {
    j.write_fieldname("lCS-to-GCS-Translation");
    lcs_to_gcs_translation.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BroadcastPeriodicity ::= ENUMERATED
const char* broadcast_periodicity_opts::to_string() const
{
  static const char* names[] = {"ms80", "ms160", "ms320", "ms640", "ms1280", "ms2560", "ms5120"};
  return convert_enum_idx(names, 7, value, "broadcast_periodicity_e");
}
uint16_t broadcast_periodicity_opts::to_number() const
{
  static const uint16_t numbers[] = {80, 160, 320, 640, 1280, 2560, 5120};
  return map_enum_number(numbers, 7, value, "broadcast_periodicity_e");
}

// PosSIB-Type ::= ENUMERATED
const char* pos_sib_type_opts::to_string() const
{
  static const char* names[] = {
      "posSibType1-1",   "posSibType1-2",  "posSibType1-3",  "posSibType1-4",  "posSibType1-5",   "posSibType1-6",
      "posSibType1-7",   "posSibType1-8",  "posSibType2-1",  "posSibType2-2",  "posSibType2-3",   "posSibType2-4",
      "posSibType2-5",   "posSibType2-6",  "posSibType2-7",  "posSibType2-8",  "posSibType2-9",   "posSibType2-10",
      "posSibType2-11",  "posSibType2-12", "posSibType2-13", "posSibType2-14", "posSibType2-15",  "posSibType2-16",
      "posSibType2-17",  "posSibType2-18", "posSibType2-19", "posSibType2-20", "posSibType2-21",  "posSibType2-22",
      "posSibType2-23",  "posSibType2-24", "posSibType2-25", "posSibType3-1",  "posSibType4-1",   "posSibType5-1",
      "posSibType6-1",   "posSibType6-2",  "posSibType6-3",  "posSibType1-9",  "posSibType1-10",  "posSibType6-4",
      "posSibType6-5",   "posSibType6-6",  "posSibType1-11", "posSibType1-12", "posSibType2-17a", "posSibType2-18a",
      "posSibType2-20a", "posSibType2-26", "posSibType2-27", "posSibType6-7",  "posSibType7-1",   "posSibType7-2",
      "posSibType7-3",   "posSibType7-4"};
  return convert_enum_idx(names, 56, value, "pos_sib_type_e");
}

OCUDUASN_CODE pos_sib_segments_item_s_::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(assist_data_sib_elem.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_sib_segments_item_s_::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(assist_data_sib_elem.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_sib_segments_item_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("assistanceDataSIBelement", assist_data_sib_elem.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// AssistanceInformationMetaData ::= SEQUENCE
OCUDUASN_CODE assist_info_meta_data_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(encrypted_present, 1));
  HANDLE_CODE(bref.pack(gns_si_d_present, 1));
  HANDLE_CODE(bref.pack(sba_si_d_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (encrypted_present) {
    HANDLE_CODE(encrypted.pack(bref));
  }
  if (gns_si_d_present) {
    HANDLE_CODE(gns_si_d.pack(bref));
  }
  if (sba_si_d_present) {
    HANDLE_CODE(sba_si_d.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_meta_data_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(encrypted_present, 1));
  HANDLE_CODE(bref.unpack(gns_si_d_present, 1));
  HANDLE_CODE(bref.unpack(sba_si_d_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (encrypted_present) {
    HANDLE_CODE(encrypted.unpack(bref));
  }
  if (gns_si_d_present) {
    HANDLE_CODE(gns_si_d.unpack(bref));
  }
  if (sba_si_d_present) {
    HANDLE_CODE(sba_si_d.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void assist_info_meta_data_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (encrypted_present) {
    j.write_str("encrypted", "true");
  }
  if (gns_si_d_present) {
    j.write_str("gNSSID", gns_si_d.to_string());
  }
  if (sba_si_d_present) {
    j.write_str("sBASID", sba_si_d.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* assist_info_meta_data_s::encrypted_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "assist_info_meta_data_s::encrypted_e_");
}

const char* assist_info_meta_data_s::gns_si_d_opts::to_string() const
{
  static const char* names[] = {"gps", "sbas", "qzss", "galileo", "glonass", "bds", "navic"};
  return convert_enum_idx(names, 7, value, "assist_info_meta_data_s::gns_si_d_e_");
}

const char* assist_info_meta_data_s::sba_si_d_opts::to_string() const
{
  static const char* names[] = {"waas", "egnos", "msas", "gagan"};
  return convert_enum_idx(names, 4, value, "assist_info_meta_data_s::sba_si_d_e_");
}

OCUDUASN_CODE pos_sibs_item_s_::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(assist_info_meta_data_present, 1));
  HANDLE_CODE(bref.pack(broadcast_prio_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pos_sib_type.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, pos_sib_segments, 1, 64, true));
  if (assist_info_meta_data_present) {
    HANDLE_CODE(assist_info_meta_data.pack(bref));
  }
  if (broadcast_prio_present) {
    HANDLE_CODE(pack_integer(bref, broadcast_prio, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_sibs_item_s_::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(assist_info_meta_data_present, 1));
  HANDLE_CODE(bref.unpack(broadcast_prio_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(pos_sib_type.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(pos_sib_segments, bref, 1, 64, true));
  if (assist_info_meta_data_present) {
    HANDLE_CODE(assist_info_meta_data.unpack(bref));
  }
  if (broadcast_prio_present) {
    HANDLE_CODE(unpack_integer(broadcast_prio, bref, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_sibs_item_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("posSIB-Type", pos_sib_type.to_string());
  j.start_array("posSIB-Segments");
  for (const auto& e1 : pos_sib_segments) {
    e1.to_json(j);
  }
  j.end_array();
  if (assist_info_meta_data_present) {
    j.write_fieldname("assistanceInformationMetaData");
    assist_info_meta_data.to_json(j);
  }
  if (broadcast_prio_present) {
    j.write_int("broadcastPriority", broadcast_prio);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

OCUDUASN_CODE sys_info_item_s_::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_periodicity.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, pos_sibs, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_info_item_s_::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_periodicity.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(pos_sibs, bref, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sys_info_item_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("broadcastPeriodicity", broadcast_periodicity.to_string());
  j.start_array("posSIBs");
  for (const auto& e1 : pos_sibs) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Assistance-Information ::= SEQUENCE
OCUDUASN_CODE assist_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, sys_info, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(sys_info, bref, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void assist_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("systemInformation");
  for (const auto& e1 : sys_info) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Outcome ::= ENUMERATED
const char* outcome_opts::to_string() const
{
  static const char* names[] = {"failed"};
  return convert_enum_idx(names, 1, value, "outcome_e");
}

OCUDUASN_CODE assist_info_fail_list_item_s_::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pos_sib_type.pack(bref));
  HANDLE_CODE(outcome.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_fail_list_item_s_::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(pos_sib_type.unpack(bref));
  HANDLE_CODE(outcome.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void assist_info_fail_list_item_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("posSIB-Type", pos_sib_type.to_string());
  j.write_str("outcome", "failed");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Bandwidth-Aggregation-Request-Indication ::= ENUMERATED
const char* bw_aggregation_request_ind_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "bw_aggregation_request_ind_e");
}

// BandwidthSRS ::= CHOICE
void bw_srs_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<bw_srs_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void bw_srs_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::fr1:
      break;
    case types::fr2:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<bw_srs_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bw_srs_c");
  }
}
bw_srs_c::bw_srs_c(const bw_srs_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::fr1:
      c.init(other.c.get<fr1_e_>());
      break;
    case types::fr2:
      c.init(other.c.get<fr2_e_>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bw_srs_c");
  }
}
bw_srs_c& bw_srs_c::operator=(const bw_srs_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::fr1:
      c.set(other.c.get<fr1_e_>());
      break;
    case types::fr2:
      c.set(other.c.get<fr2_e_>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bw_srs_c");
  }

  return *this;
}
bw_srs_c::fr1_e_& bw_srs_c::set_fr1()
{
  set(types::fr1);
  return c.get<fr1_e_>();
}
bw_srs_c::fr2_e_& bw_srs_c::set_fr2()
{
  set(types::fr2);
  return c.get<fr2_e_>();
}
protocol_ie_single_container_s<bw_srs_ext_ies_o>& bw_srs_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>();
}
void bw_srs_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::fr1:
      j.write_str("fR1", c.get<fr1_e_>().to_string());
      break;
    case types::fr2:
      j.write_str("fR2", c.get<fr2_e_>().to_string());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "bw_srs_c");
  }
  j.end_obj();
}
OCUDUASN_CODE bw_srs_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::fr1:
      HANDLE_CODE(c.get<fr1_e_>().pack(bref));
      break;
    case types::fr2:
      HANDLE_CODE(c.get<fr2_e_>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bw_srs_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bw_srs_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::fr1:
      HANDLE_CODE(c.get<fr1_e_>().unpack(bref));
      break;
    case types::fr2:
      HANDLE_CODE(c.get<fr2_e_>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bw_srs_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* bw_srs_c::fr1_opts::to_string() const
{
  static const char* names[] = {"mHz5",
                                "mHz10",
                                "mHz20",
                                "mHz40",
                                "mHz50",
                                "mHz80",
                                "mHz100",
                                "mHz160",
                                "mHz200",
                                "mHz15",
                                "mHz25",
                                "mHz30",
                                "mHz60",
                                "mHz35",
                                "mHz45",
                                "mHz70",
                                "mHz90"};
  return convert_enum_idx(names, 17, value, "bw_srs_c::fr1_e_");
}
uint8_t bw_srs_c::fr1_opts::to_number() const
{
  static const uint8_t numbers[] = {5, 10, 20, 40, 50, 80, 100, 160, 200, 15, 25, 30, 60, 35, 45, 70, 90};
  return map_enum_number(numbers, 17, value, "bw_srs_c::fr1_e_");
}

const char* bw_srs_c::fr2_opts::to_string() const
{
  static const char* names[] = {"mHz50", "mHz100", "mHz200", "mHz400", "mHz600", "mhz800", "mHz1600", "mHz2000"};
  return convert_enum_idx(names, 8, value, "bw_srs_c::fr2_e_");
}
uint16_t bw_srs_c::fr2_opts::to_number() const
{
  static const uint16_t numbers[] = {50, 100, 200, 400, 600, 800, 1600, 2000};
  return map_enum_number(numbers, 8, value, "bw_srs_c::fr2_e_");
}

const char* bw_srs_c::types_opts::to_string() const
{
  static const char* names[] = {"fR1", "fR2", "choice-extension"};
  return convert_enum_idx(names, 3, value, "bw_srs_c::types");
}
uint8_t bw_srs_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "bw_srs_c::types");
}

// Broadcast ::= ENUMERATED
const char* broadcast_opts::to_string() const
{
  static const char* names[] = {"start", "stop"};
  return convert_enum_idx(names, 2, value, "broadcast_e");
}

// CGI-EUTRA ::= SEQUENCE
OCUDUASN_CODE cgi_eutra_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(eutr_acell_id.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cgi_eutra_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(eutr_acell_id.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cgi_eutra_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  j.write_fieldname("eUTRAcellIdentifier");
  eutr_acell_id.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CGI-NR ::= SEQUENCE
OCUDUASN_CODE cgi_nr_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(nr_cell_id.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cgi_nr_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(nr_cell_id.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cgi_nr_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  j.write_fieldname("nRcellIdentifier");
  nr_cell_id.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CPLength-EUTRA ::= ENUMERATED
const char* cp_len_eutra_opts::to_string() const
{
  static const char* names[] = {"normal", "extended"};
  return convert_enum_idx(names, 2, value, "cp_len_eutra_e");
}

// CarrierFreq ::= SEQUENCE
OCUDUASN_CODE carrier_freq_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, point_a, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(pack_integer(bref, offset_to_carrier, (uint16_t)0u, (uint16_t)2199u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE carrier_freq_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(point_a, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(unpack_integer(offset_to_carrier, bref, (uint16_t)0u, (uint16_t)2199u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void carrier_freq_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pointA", point_a);
  j.write_int("offsetToCarrier", offset_to_carrier);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CauseRadioNetwork ::= ENUMERATED
const char* cause_radio_network_opts::to_string() const
{
  static const char* names[] = {"unspecified",
                                "requested-item-not-supported",
                                "requested-item-temporarily-not-available",
                                "serving-NG-RAN-node-changed",
                                "requested-item-not-supported-on-time"};
  return convert_enum_idx(names, 5, value, "cause_radio_network_e");
}

// CauseProtocol ::= ENUMERATED
const char* cause_protocol_opts::to_string() const
{
  static const char* names[] = {"transfer-syntax-error",
                                "abstract-syntax-error-reject",
                                "abstract-syntax-error-ignore-and-notify",
                                "message-not-compatible-with-receiver-state",
                                "semantic-error",
                                "unspecified",
                                "abstract-syntax-error-falsely-constructed-message"};
  return convert_enum_idx(names, 7, value, "cause_protocol_e");
}

// CauseMisc ::= ENUMERATED
const char* cause_misc_opts::to_string() const
{
  static const char* names[] = {"unspecified"};
  return convert_enum_idx(names, 1, value, "cause_misc_e");
}

// Cause ::= CHOICE
void cause_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<cause_ext_ie_o>>();
      break;
    default:
      break;
  }
}
void cause_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::radio_network:
      break;
    case types::protocol:
      break;
    case types::misc:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<cause_ext_ie_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }
}
cause_c::cause_c(const cause_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::radio_network:
      c.init(other.c.get<cause_radio_network_e>());
      break;
    case types::protocol:
      c.init(other.c.get<cause_protocol_e>());
      break;
    case types::misc:
      c.init(other.c.get<cause_misc_e>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<cause_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }
}
cause_c& cause_c::operator=(const cause_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::radio_network:
      c.set(other.c.get<cause_radio_network_e>());
      break;
    case types::protocol:
      c.set(other.c.get<cause_protocol_e>());
      break;
    case types::misc:
      c.set(other.c.get<cause_misc_e>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<cause_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }

  return *this;
}
cause_radio_network_e& cause_c::set_radio_network()
{
  set(types::radio_network);
  return c.get<cause_radio_network_e>();
}
cause_protocol_e& cause_c::set_protocol()
{
  set(types::protocol);
  return c.get<cause_protocol_e>();
}
cause_misc_e& cause_c::set_misc()
{
  set(types::misc);
  return c.get<cause_misc_e>();
}
protocol_ie_single_container_s<cause_ext_ie_o>& cause_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<cause_ext_ie_o>>();
}
void cause_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::radio_network:
      j.write_str("radioNetwork", c.get<cause_radio_network_e>().to_string());
      break;
    case types::protocol:
      j.write_str("protocol", c.get<cause_protocol_e>().to_string());
      break;
    case types::misc:
      j.write_str("misc", "unspecified");
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<cause_ext_ie_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }
  j.end_obj();
}
OCUDUASN_CODE cause_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::radio_network:
      HANDLE_CODE(c.get<cause_radio_network_e>().pack(bref));
      break;
    case types::protocol:
      HANDLE_CODE(c.get<cause_protocol_e>().pack(bref));
      break;
    case types::misc:
      HANDLE_CODE(c.get<cause_misc_e>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<cause_ext_ie_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cause_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::radio_network:
      HANDLE_CODE(c.get<cause_radio_network_e>().unpack(bref));
      break;
    case types::protocol:
      HANDLE_CODE(c.get<cause_protocol_e>().unpack(bref));
      break;
    case types::misc:
      HANDLE_CODE(c.get<cause_misc_e>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<cause_ext_ie_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cause_c::types_opts::to_string() const
{
  static const char* names[] = {"radioNetwork", "protocol", "misc", "choice-Extension"};
  return convert_enum_idx(names, 4, value, "cause_c::types");
}
uint8_t cause_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "cause_c::types");
}

// TRP-Beam-Power-Item ::= SEQUENCE
OCUDUASN_CODE trp_beam_pwr_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(prs_res_set_id_present, 1));
  HANDLE_CODE(bref.pack(relative_pwr_fine_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (prs_res_set_id_present) {
    HANDLE_CODE(pack_integer(bref, prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  }
  HANDLE_CODE(pack_integer(bref, prs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(pack_integer(bref, relative_pwr, (uint8_t)0u, (uint8_t)30u, false, true));
  if (relative_pwr_fine_present) {
    HANDLE_CODE(pack_integer(bref, relative_pwr_fine, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_beam_pwr_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(prs_res_set_id_present, 1));
  HANDLE_CODE(bref.unpack(relative_pwr_fine_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (prs_res_set_id_present) {
    HANDLE_CODE(unpack_integer(prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  }
  HANDLE_CODE(unpack_integer(prs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(unpack_integer(relative_pwr, bref, (uint8_t)0u, (uint8_t)30u, false, true));
  if (relative_pwr_fine_present) {
    HANDLE_CODE(unpack_integer(relative_pwr_fine, bref, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_beam_pwr_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (prs_res_set_id_present) {
    j.write_int("pRSResourceSetID", prs_res_set_id);
  }
  j.write_int("pRSResourceID", prs_res_id);
  j.write_int("relativePower", relative_pwr);
  if (relative_pwr_fine_present) {
    j.write_int("relativePowerFine", relative_pwr_fine);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRP-ElevationAngleList-Item ::= SEQUENCE
OCUDUASN_CODE trp_elevation_angle_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(trp_elevation_angle_fine_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_elevation_angle, (uint8_t)0u, (uint8_t)180u, false, true));
  if (trp_elevation_angle_fine_present) {
    HANDLE_CODE(pack_integer(bref, trp_elevation_angle_fine, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, trp_beam_pwr_list, 2, 24, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_elevation_angle_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(trp_elevation_angle_fine_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_elevation_angle, bref, (uint8_t)0u, (uint8_t)180u, false, true));
  if (trp_elevation_angle_fine_present) {
    HANDLE_CODE(unpack_integer(trp_elevation_angle_fine, bref, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  HANDLE_CODE(unpack_dyn_seq_of(trp_beam_pwr_list, bref, 2, 24, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_elevation_angle_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trp-elevation-angle", trp_elevation_angle);
  if (trp_elevation_angle_fine_present) {
    j.write_int("trp-elevation-angle-fine", trp_elevation_angle_fine);
  }
  j.start_array("trp-beam-power-list");
  for (const auto& e1 : trp_beam_pwr_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRP-BeamAntennaAnglesList-Item ::= SEQUENCE
OCUDUASN_CODE trp_beam_ant_angles_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(trp_azimuth_angle_fine_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_azimuth_angle, (uint16_t)0u, (uint16_t)359u, false, true));
  if (trp_azimuth_angle_fine_present) {
    HANDLE_CODE(pack_integer(bref, trp_azimuth_angle_fine, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, trp_elevation_angle_list, 1, 1801, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_beam_ant_angles_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(trp_azimuth_angle_fine_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_azimuth_angle, bref, (uint16_t)0u, (uint16_t)359u, false, true));
  if (trp_azimuth_angle_fine_present) {
    HANDLE_CODE(unpack_integer(trp_azimuth_angle_fine, bref, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  HANDLE_CODE(unpack_dyn_seq_of(trp_elevation_angle_list, bref, 1, 1801, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_beam_ant_angles_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trp-azimuth-angle", trp_azimuth_angle);
  if (trp_azimuth_angle_fine_present) {
    j.write_int("trp-azimuth-angle-fine", trp_azimuth_angle_fine);
  }
  j.start_array("trp-elevation-angle-list");
  for (const auto& e1 : trp_elevation_angle_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRP-BeamAntennaExplicitInformation ::= SEQUENCE
OCUDUASN_CODE trp_beam_ant_explicit_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(lcs_to_gcs_translation_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, trp_beam_ant_angles, 1, 3600, true));
  if (lcs_to_gcs_translation_present) {
    HANDLE_CODE(lcs_to_gcs_translation.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_beam_ant_explicit_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(lcs_to_gcs_translation_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(trp_beam_ant_angles, bref, 1, 3600, true));
  if (lcs_to_gcs_translation_present) {
    HANDLE_CODE(lcs_to_gcs_translation.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_beam_ant_explicit_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("trp-BeamAntennaAngles");
  for (const auto& e1 : trp_beam_ant_angles) {
    e1.to_json(j);
  }
  j.end_array();
  if (lcs_to_gcs_translation_present) {
    j.write_fieldname("lcs-to-gcs-translation");
    lcs_to_gcs_translation.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Choice-TRP-Beam-Antenna-Info-Item ::= CHOICE
void choice_trp_beam_ant_info_item_c::destroy_()
{
  switch (type_) {
    case types::explicit_value:
      c.destroy<trp_beam_ant_explicit_info_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void choice_trp_beam_ant_info_item_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ref:
      break;
    case types::explicit_value:
      c.init<trp_beam_ant_explicit_info_s>();
      break;
    case types::no_change:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "choice_trp_beam_ant_info_item_c");
  }
}
choice_trp_beam_ant_info_item_c::choice_trp_beam_ant_info_item_c(const choice_trp_beam_ant_info_item_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ref:
      c.init(other.c.get<uint32_t>());
      break;
    case types::explicit_value:
      c.init(other.c.get<trp_beam_ant_explicit_info_s>());
      break;
    case types::no_change:
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "choice_trp_beam_ant_info_item_c");
  }
}
choice_trp_beam_ant_info_item_c&
choice_trp_beam_ant_info_item_c::operator=(const choice_trp_beam_ant_info_item_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ref:
      c.set(other.c.get<uint32_t>());
      break;
    case types::explicit_value:
      c.set(other.c.get<trp_beam_ant_explicit_info_s>());
      break;
    case types::no_change:
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "choice_trp_beam_ant_info_item_c");
  }

  return *this;
}
uint32_t& choice_trp_beam_ant_info_item_c::set_ref()
{
  set(types::ref);
  return c.get<uint32_t>();
}
trp_beam_ant_explicit_info_s& choice_trp_beam_ant_info_item_c::set_explicit_value()
{
  set(types::explicit_value);
  return c.get<trp_beam_ant_explicit_info_s>();
}
void choice_trp_beam_ant_info_item_c::set_no_change()
{
  set(types::no_change);
}
protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>& choice_trp_beam_ant_info_item_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>();
}
void choice_trp_beam_ant_info_item_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ref:
      j.write_int("reference", c.get<uint32_t>());
      break;
    case types::explicit_value:
      j.write_fieldname("explicit");
      c.get<trp_beam_ant_explicit_info_s>().to_json(j);
      break;
    case types::no_change:
      j.write_null("noChange");
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "choice_trp_beam_ant_info_item_c");
  }
  j.end_obj();
}
OCUDUASN_CODE choice_trp_beam_ant_info_item_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ref:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65535u, true, true));
      break;
    case types::explicit_value:
      HANDLE_CODE(c.get<trp_beam_ant_explicit_info_s>().pack(bref));
      break;
    case types::no_change:
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "choice_trp_beam_ant_info_item_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE choice_trp_beam_ant_info_item_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ref:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65535u, true, true));
      break;
    case types::explicit_value:
      HANDLE_CODE(c.get<trp_beam_ant_explicit_info_s>().unpack(bref));
      break;
    case types::no_change:
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "choice_trp_beam_ant_info_item_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* choice_trp_beam_ant_info_item_c::types_opts::to_string() const
{
  static const char* names[] = {"reference", "explicit", "noChange", "choice-extension"};
  return convert_enum_idx(names, 4, value, "choice_trp_beam_ant_info_item_c::types");
}

// CommonTAParameters ::= SEQUENCE
OCUDUASN_CODE common_ta_params_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(epoch_time.pack(bref));
  HANDLE_CODE(ta_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE common_ta_params_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(epoch_time.unpack(bref));
  HANDLE_CODE(ta_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void common_ta_params_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("epochTime", epoch_time.to_string());
  j.write_str("taInfo", ta_info.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TypeOfError ::= ENUMERATED
const char* type_of_error_opts::to_string() const
{
  static const char* names[] = {"not-understood", "missing"};
  return convert_enum_idx(names, 2, value, "type_of_error_e");
}

OCUDUASN_CODE crit_diagnostics_ie_list_item_s_::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ie_crit.pack(bref));
  HANDLE_CODE(pack_integer(bref, ie_id, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(type_of_error.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE crit_diagnostics_ie_list_item_s_::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ie_crit.unpack(bref));
  HANDLE_CODE(unpack_integer(ie_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(type_of_error.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void crit_diagnostics_ie_list_item_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("iECriticality", ie_crit.to_string());
  j.write_int("iE-ID", ie_id);
  j.write_str("typeOfError", type_of_error.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CriticalityDiagnostics ::= SEQUENCE
OCUDUASN_CODE crit_diagnostics_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(proc_code_present, 1));
  HANDLE_CODE(bref.pack(trigger_msg_present, 1));
  HANDLE_CODE(bref.pack(proc_crit_present, 1));
  HANDLE_CODE(bref.pack(nrppatransaction_id_present, 1));
  HANDLE_CODE(bref.pack(ies_crit_diagnostics.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (proc_code_present) {
    HANDLE_CODE(pack_integer(bref, proc_code, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (trigger_msg_present) {
    HANDLE_CODE(trigger_msg.pack(bref));
  }
  if (proc_crit_present) {
    HANDLE_CODE(proc_crit.pack(bref));
  }
  if (nrppatransaction_id_present) {
    HANDLE_CODE(pack_integer(bref, nrppatransaction_id, (uint16_t)0u, (uint16_t)32767u, false, true));
  }
  if (ies_crit_diagnostics.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ies_crit_diagnostics, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE crit_diagnostics_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(proc_code_present, 1));
  HANDLE_CODE(bref.unpack(trigger_msg_present, 1));
  HANDLE_CODE(bref.unpack(proc_crit_present, 1));
  HANDLE_CODE(bref.unpack(nrppatransaction_id_present, 1));
  bool ies_crit_diagnostics_present;
  HANDLE_CODE(bref.unpack(ies_crit_diagnostics_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (proc_code_present) {
    HANDLE_CODE(unpack_integer(proc_code, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (trigger_msg_present) {
    HANDLE_CODE(trigger_msg.unpack(bref));
  }
  if (proc_crit_present) {
    HANDLE_CODE(proc_crit.unpack(bref));
  }
  if (nrppatransaction_id_present) {
    HANDLE_CODE(unpack_integer(nrppatransaction_id, bref, (uint16_t)0u, (uint16_t)32767u, false, true));
  }
  if (ies_crit_diagnostics_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ies_crit_diagnostics, bref, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void crit_diagnostics_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (proc_code_present) {
    j.write_int("procedureCode", proc_code);
  }
  if (trigger_msg_present) {
    j.write_str("triggeringMessage", trigger_msg.to_string());
  }
  if (proc_crit_present) {
    j.write_str("procedureCriticality", proc_crit.to_string());
  }
  if (nrppatransaction_id_present) {
    j.write_int("nrppatransactionID", nrppatransaction_id);
  }
  if (ies_crit_diagnostics.size() > 0) {
    j.start_array("iEsCriticalityDiagnostics");
    for (const auto& e1 : ies_crit_diagnostics) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DL-Bandwidth-EUTRA ::= ENUMERATED
const char* dl_bw_eutra_opts::to_string() const
{
  static const char* names[] = {"bw6", "bw15", "bw25", "bw50", "bw75", "bw100"};
  return convert_enum_idx(names, 6, value, "dl_bw_eutra_e");
}
uint8_t dl_bw_eutra_opts::to_number() const
{
  static const uint8_t numbers[] = {6, 15, 25, 50, 75, 100};
  return map_enum_number(numbers, 6, value, "dl_bw_eutra_e");
}

// DL-PRS ::= SEQUENCE
OCUDUASN_CODE dl_prs_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(dl_prs_res_id_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, prsid, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_integer(bref, dl_prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  if (dl_prs_res_id_present) {
    HANDLE_CODE(pack_integer(bref, dl_prs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(dl_prs_res_id_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prsid, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_integer(dl_prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  if (dl_prs_res_id_present) {
    HANDLE_CODE(unpack_integer(dl_prs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_prs_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("prsid", prsid);
  j.write_int("dl-PRSResourceSetID", dl_prs_res_set_id);
  if (dl_prs_res_id_present) {
    j.write_int("dl-PRSResourceID", dl_prs_res_id);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DL-PRSBWAggregationRequestInfo-Item ::= SEQUENCE
OCUDUASN_CODE dl_prsbw_aggregation_request_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, dl_prs_res_set_idx, (uint8_t)1u, (uint8_t)8u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prsbw_aggregation_request_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(dl_prs_res_set_idx, bref, (uint8_t)1u, (uint8_t)8u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_prsbw_aggregation_request_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dl-prs-ResourceSetIndex", dl_prs_res_set_idx);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DL-PRSMutingPattern ::= CHOICE
void dl_prs_muting_pattern_c::destroy_()
{
  switch (type_) {
    case types::two:
      c.destroy<fixed_bitstring<2, false, true>>();
      break;
    case types::four:
      c.destroy<fixed_bitstring<4, false, true>>();
      break;
    case types::six:
      c.destroy<fixed_bitstring<6, false, true>>();
      break;
    case types::eight:
      c.destroy<fixed_bitstring<8, false, true>>();
      break;
    case types::sixteen:
      c.destroy<fixed_bitstring<16, false, true>>();
      break;
    case types::thirty_two:
      c.destroy<fixed_bitstring<32, false, true>>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void dl_prs_muting_pattern_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::two:
      c.init<fixed_bitstring<2, false, true>>();
      break;
    case types::four:
      c.init<fixed_bitstring<4, false, true>>();
      break;
    case types::six:
      c.init<fixed_bitstring<6, false, true>>();
      break;
    case types::eight:
      c.init<fixed_bitstring<8, false, true>>();
      break;
    case types::sixteen:
      c.init<fixed_bitstring<16, false, true>>();
      break;
    case types::thirty_two:
      c.init<fixed_bitstring<32, false, true>>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_muting_pattern_c");
  }
}
dl_prs_muting_pattern_c::dl_prs_muting_pattern_c(const dl_prs_muting_pattern_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::two:
      c.init(other.c.get<fixed_bitstring<2, false, true>>());
      break;
    case types::four:
      c.init(other.c.get<fixed_bitstring<4, false, true>>());
      break;
    case types::six:
      c.init(other.c.get<fixed_bitstring<6, false, true>>());
      break;
    case types::eight:
      c.init(other.c.get<fixed_bitstring<8, false, true>>());
      break;
    case types::sixteen:
      c.init(other.c.get<fixed_bitstring<16, false, true>>());
      break;
    case types::thirty_two:
      c.init(other.c.get<fixed_bitstring<32, false, true>>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_muting_pattern_c");
  }
}
dl_prs_muting_pattern_c& dl_prs_muting_pattern_c::operator=(const dl_prs_muting_pattern_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::two:
      c.set(other.c.get<fixed_bitstring<2, false, true>>());
      break;
    case types::four:
      c.set(other.c.get<fixed_bitstring<4, false, true>>());
      break;
    case types::six:
      c.set(other.c.get<fixed_bitstring<6, false, true>>());
      break;
    case types::eight:
      c.set(other.c.get<fixed_bitstring<8, false, true>>());
      break;
    case types::sixteen:
      c.set(other.c.get<fixed_bitstring<16, false, true>>());
      break;
    case types::thirty_two:
      c.set(other.c.get<fixed_bitstring<32, false, true>>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_muting_pattern_c");
  }

  return *this;
}
fixed_bitstring<2, false, true>& dl_prs_muting_pattern_c::set_two()
{
  set(types::two);
  return c.get<fixed_bitstring<2, false, true>>();
}
fixed_bitstring<4, false, true>& dl_prs_muting_pattern_c::set_four()
{
  set(types::four);
  return c.get<fixed_bitstring<4, false, true>>();
}
fixed_bitstring<6, false, true>& dl_prs_muting_pattern_c::set_six()
{
  set(types::six);
  return c.get<fixed_bitstring<6, false, true>>();
}
fixed_bitstring<8, false, true>& dl_prs_muting_pattern_c::set_eight()
{
  set(types::eight);
  return c.get<fixed_bitstring<8, false, true>>();
}
fixed_bitstring<16, false, true>& dl_prs_muting_pattern_c::set_sixteen()
{
  set(types::sixteen);
  return c.get<fixed_bitstring<16, false, true>>();
}
fixed_bitstring<32, false, true>& dl_prs_muting_pattern_c::set_thirty_two()
{
  set(types::thirty_two);
  return c.get<fixed_bitstring<32, false, true>>();
}
protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>& dl_prs_muting_pattern_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>();
}
void dl_prs_muting_pattern_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::two:
      j.write_fieldname("two");
      c.get<fixed_bitstring<2, false, true>>().to_json(j);
      break;
    case types::four:
      j.write_fieldname("four");
      c.get<fixed_bitstring<4, false, true>>().to_json(j);
      break;
    case types::six:
      j.write_fieldname("six");
      c.get<fixed_bitstring<6, false, true>>().to_json(j);
      break;
    case types::eight:
      j.write_fieldname("eight");
      c.get<fixed_bitstring<8, false, true>>().to_json(j);
      break;
    case types::sixteen:
      j.write_fieldname("sixteen");
      c.get<fixed_bitstring<16, false, true>>().to_json(j);
      break;
    case types::thirty_two:
      j.write_fieldname("thirty-two");
      c.get<fixed_bitstring<32, false, true>>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_muting_pattern_c");
  }
  j.end_obj();
}
OCUDUASN_CODE dl_prs_muting_pattern_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::two:
      HANDLE_CODE((c.get<fixed_bitstring<2, false, true>>().pack(bref)));
      break;
    case types::four:
      HANDLE_CODE((c.get<fixed_bitstring<4, false, true>>().pack(bref)));
      break;
    case types::six:
      HANDLE_CODE((c.get<fixed_bitstring<6, false, true>>().pack(bref)));
      break;
    case types::eight:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().pack(bref)));
      break;
    case types::sixteen:
      HANDLE_CODE((c.get<fixed_bitstring<16, false, true>>().pack(bref)));
      break;
    case types::thirty_two:
      HANDLE_CODE((c.get<fixed_bitstring<32, false, true>>().pack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_muting_pattern_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_muting_pattern_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::two:
      HANDLE_CODE((c.get<fixed_bitstring<2, false, true>>().unpack(bref)));
      break;
    case types::four:
      HANDLE_CODE((c.get<fixed_bitstring<4, false, true>>().unpack(bref)));
      break;
    case types::six:
      HANDLE_CODE((c.get<fixed_bitstring<6, false, true>>().unpack(bref)));
      break;
    case types::eight:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().unpack(bref)));
      break;
    case types::sixteen:
      HANDLE_CODE((c.get<fixed_bitstring<16, false, true>>().unpack(bref)));
      break;
    case types::thirty_two:
      HANDLE_CODE((c.get<fixed_bitstring<32, false, true>>().unpack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_muting_pattern_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* dl_prs_muting_pattern_c::types_opts::to_string() const
{
  static const char* names[] = {"two", "four", "six", "eight", "sixteen", "thirty-two", "choice-extension"};
  return convert_enum_idx(names, 7, value, "dl_prs_muting_pattern_c::types");
}

// DL-PRSResourceARPLocation ::= CHOICE
void dl_prs_res_arp_location_c::destroy_()
{
  switch (type_) {
    case types::relative_geodetic_location:
      c.destroy<relative_geodetic_location_s>();
      break;
    case types::relative_cartesian_location:
      c.destroy<relative_cartesian_location_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void dl_prs_res_arp_location_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::relative_geodetic_location:
      c.init<relative_geodetic_location_s>();
      break;
    case types::relative_cartesian_location:
      c.init<relative_cartesian_location_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_arp_location_c");
  }
}
dl_prs_res_arp_location_c::dl_prs_res_arp_location_c(const dl_prs_res_arp_location_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::relative_geodetic_location:
      c.init(other.c.get<relative_geodetic_location_s>());
      break;
    case types::relative_cartesian_location:
      c.init(other.c.get<relative_cartesian_location_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_arp_location_c");
  }
}
dl_prs_res_arp_location_c& dl_prs_res_arp_location_c::operator=(const dl_prs_res_arp_location_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::relative_geodetic_location:
      c.set(other.c.get<relative_geodetic_location_s>());
      break;
    case types::relative_cartesian_location:
      c.set(other.c.get<relative_cartesian_location_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_arp_location_c");
  }

  return *this;
}
relative_geodetic_location_s& dl_prs_res_arp_location_c::set_relative_geodetic_location()
{
  set(types::relative_geodetic_location);
  return c.get<relative_geodetic_location_s>();
}
relative_cartesian_location_s& dl_prs_res_arp_location_c::set_relative_cartesian_location()
{
  set(types::relative_cartesian_location);
  return c.get<relative_cartesian_location_s>();
}
protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>& dl_prs_res_arp_location_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>();
}
void dl_prs_res_arp_location_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::relative_geodetic_location:
      j.write_fieldname("relativeGeodeticLocation");
      c.get<relative_geodetic_location_s>().to_json(j);
      break;
    case types::relative_cartesian_location:
      j.write_fieldname("relativeCartesianLocation");
      c.get<relative_cartesian_location_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_arp_location_c");
  }
  j.end_obj();
}
OCUDUASN_CODE dl_prs_res_arp_location_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::relative_geodetic_location:
      HANDLE_CODE(c.get<relative_geodetic_location_s>().pack(bref));
      break;
    case types::relative_cartesian_location:
      HANDLE_CODE(c.get<relative_cartesian_location_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_arp_location_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_res_arp_location_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::relative_geodetic_location:
      HANDLE_CODE(c.get<relative_geodetic_location_s>().unpack(bref));
      break;
    case types::relative_cartesian_location:
      HANDLE_CODE(c.get<relative_cartesian_location_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_arp_location_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* dl_prs_res_arp_location_c::types_opts::to_string() const
{
  static const char* names[] = {"relativeGeodeticLocation", "relativeCartesianLocation", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "dl_prs_res_arp_location_c::types");
}

// DL-PRSResourceSetARPLocation ::= CHOICE
void dl_prs_res_set_arp_location_c::destroy_()
{
  switch (type_) {
    case types::relative_geodetic_location:
      c.destroy<relative_geodetic_location_s>();
      break;
    case types::relative_cartesian_location:
      c.destroy<relative_cartesian_location_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void dl_prs_res_set_arp_location_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::relative_geodetic_location:
      c.init<relative_geodetic_location_s>();
      break;
    case types::relative_cartesian_location:
      c.init<relative_cartesian_location_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_set_arp_location_c");
  }
}
dl_prs_res_set_arp_location_c::dl_prs_res_set_arp_location_c(const dl_prs_res_set_arp_location_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::relative_geodetic_location:
      c.init(other.c.get<relative_geodetic_location_s>());
      break;
    case types::relative_cartesian_location:
      c.init(other.c.get<relative_cartesian_location_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_set_arp_location_c");
  }
}
dl_prs_res_set_arp_location_c& dl_prs_res_set_arp_location_c::operator=(const dl_prs_res_set_arp_location_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::relative_geodetic_location:
      c.set(other.c.get<relative_geodetic_location_s>());
      break;
    case types::relative_cartesian_location:
      c.set(other.c.get<relative_cartesian_location_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_set_arp_location_c");
  }

  return *this;
}
relative_geodetic_location_s& dl_prs_res_set_arp_location_c::set_relative_geodetic_location()
{
  set(types::relative_geodetic_location);
  return c.get<relative_geodetic_location_s>();
}
relative_cartesian_location_s& dl_prs_res_set_arp_location_c::set_relative_cartesian_location()
{
  set(types::relative_cartesian_location);
  return c.get<relative_cartesian_location_s>();
}
protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>& dl_prs_res_set_arp_location_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>();
}
void dl_prs_res_set_arp_location_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::relative_geodetic_location:
      j.write_fieldname("relativeGeodeticLocation");
      c.get<relative_geodetic_location_s>().to_json(j);
      break;
    case types::relative_cartesian_location:
      j.write_fieldname("relativeCartesianLocation");
      c.get<relative_cartesian_location_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_set_arp_location_c");
  }
  j.end_obj();
}
OCUDUASN_CODE dl_prs_res_set_arp_location_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::relative_geodetic_location:
      HANDLE_CODE(c.get<relative_geodetic_location_s>().pack(bref));
      break;
    case types::relative_cartesian_location:
      HANDLE_CODE(c.get<relative_cartesian_location_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_set_arp_location_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_res_set_arp_location_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::relative_geodetic_location:
      HANDLE_CODE(c.get<relative_geodetic_location_s>().unpack(bref));
      break;
    case types::relative_cartesian_location:
      HANDLE_CODE(c.get<relative_cartesian_location_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "dl_prs_res_set_arp_location_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* dl_prs_res_set_arp_location_c::types_opts::to_string() const
{
  static const char* names[] = {"relativeGeodeticLocation", "relativeCartesianLocation", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "dl_prs_res_set_arp_location_c::types");
}

// DL-reference-signal-UERxTx-TD ::= ENUMERATED
const char* dl_ref_sig_ue_rx_tx_td_opts::to_string() const
{
  static const char* names[] = {"csirs", "prs"};
  return convert_enum_idx(names, 2, value, "dl_ref_sig_ue_rx_tx_td_e");
}

// DLPRSResourceARP ::= SEQUENCE
OCUDUASN_CODE dl_prs_res_arp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, dl_prs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(dl_prs_res_arp_location.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_res_arp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(dl_prs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(dl_prs_res_arp_location.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_prs_res_arp_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dl-PRSResourceID", dl_prs_res_id);
  j.write_fieldname("dL-PRSResourceARPLocation");
  dl_prs_res_arp_location.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DLPRSResourceSetARP ::= SEQUENCE
OCUDUASN_CODE dl_prs_res_set_arp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, dl_prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(dl_prs_res_set_arp_location.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, listof_dl_prs_res_arp, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_res_set_arp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(dl_prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(dl_prs_res_set_arp_location.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(listof_dl_prs_res_arp, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_prs_res_set_arp_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dl-PRSResourceSetID", dl_prs_res_set_id);
  j.write_fieldname("dL-PRSResourceSetARPLocation");
  dl_prs_res_set_arp_location.to_json(j);
  j.start_array("listofDL-PRSResourceARP");
  for (const auto& e1 : listof_dl_prs_res_arp) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DLPRSResourceCoordinates ::= SEQUENCE
OCUDUASN_CODE dl_prs_res_coordinates_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, listof_dl_prs_res_set_arp, 1, 2, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_res_coordinates_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(listof_dl_prs_res_set_arp, bref, 1, 2, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_prs_res_coordinates_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("listofDL-PRSResourceSetARP");
  for (const auto& e1 : listof_dl_prs_res_set_arp) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DLPRSResourceID-Item ::= SEQUENCE
OCUDUASN_CODE dl_prs_res_id_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, dl_prs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_prs_res_id_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(dl_prs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_prs_res_id_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dl-PRSResourceID", dl_prs_res_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NG-RANCell ::= CHOICE
void ng_ran_cell_c::destroy_()
{
  switch (type_) {
    case types::eutra_cell_id:
      c.destroy<fixed_bitstring<28, false, true>>();
      break;
    case types::nr_cell_id:
      c.destroy<fixed_bitstring<36, false, true>>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>();
      break;
    default:
      break;
  }
}
void ng_ran_cell_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::eutra_cell_id:
      c.init<fixed_bitstring<28, false, true>>();
      break;
    case types::nr_cell_id:
      c.init<fixed_bitstring<36, false, true>>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ng_ran_cell_c");
  }
}
ng_ran_cell_c::ng_ran_cell_c(const ng_ran_cell_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::eutra_cell_id:
      c.init(other.c.get<fixed_bitstring<28, false, true>>());
      break;
    case types::nr_cell_id:
      c.init(other.c.get<fixed_bitstring<36, false, true>>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ng_ran_cell_c");
  }
}
ng_ran_cell_c& ng_ran_cell_c::operator=(const ng_ran_cell_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::eutra_cell_id:
      c.set(other.c.get<fixed_bitstring<28, false, true>>());
      break;
    case types::nr_cell_id:
      c.set(other.c.get<fixed_bitstring<36, false, true>>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ng_ran_cell_c");
  }

  return *this;
}
fixed_bitstring<28, false, true>& ng_ran_cell_c::set_eutra_cell_id()
{
  set(types::eutra_cell_id);
  return c.get<fixed_bitstring<28, false, true>>();
}
fixed_bitstring<36, false, true>& ng_ran_cell_c::set_nr_cell_id()
{
  set(types::nr_cell_id);
  return c.get<fixed_bitstring<36, false, true>>();
}
protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>& ng_ran_cell_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>();
}
void ng_ran_cell_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::eutra_cell_id:
      j.write_fieldname("eUTRA-CellID");
      c.get<fixed_bitstring<28, false, true>>().to_json(j);
      break;
    case types::nr_cell_id:
      j.write_fieldname("nR-CellID");
      c.get<fixed_bitstring<36, false, true>>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ng_ran_cell_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ng_ran_cell_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::eutra_cell_id:
      HANDLE_CODE((c.get<fixed_bitstring<28, false, true>>().pack(bref)));
      break;
    case types::nr_cell_id:
      HANDLE_CODE((c.get<fixed_bitstring<36, false, true>>().pack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ng_ran_cell_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_cell_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::eutra_cell_id:
      HANDLE_CODE((c.get<fixed_bitstring<28, false, true>>().unpack(bref)));
      break;
    case types::nr_cell_id:
      HANDLE_CODE((c.get<fixed_bitstring<36, false, true>>().unpack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ng_ran_cell_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ng_ran_cell_c::types_opts::to_string() const
{
  static const char* names[] = {"eUTRA-CellID", "nR-CellID", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "ng_ran_cell_c::types");
}

// NG-RAN-CGI ::= SEQUENCE
OCUDUASN_CODE ng_ran_cgi_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(ng_ran_cell.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_cgi_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(ng_ran_cell.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ng_ran_cgi_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  j.write_fieldname("nG-RANcell");
  ng_ran_cell.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NG-RANAccessPointPosition ::= SEQUENCE
OCUDUASN_CODE ng_ran_access_point_position_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(latitude_sign.pack(bref));
  HANDLE_CODE(pack_integer(bref, latitude, (uint32_t)0u, (uint32_t)8388607u, false, true));
  HANDLE_CODE(pack_integer(bref, longitude, (int32_t)-8388608, (int32_t)8388607, false, true));
  HANDLE_CODE(direction_of_altitude.pack(bref));
  HANDLE_CODE(pack_integer(bref, altitude, (uint16_t)0u, (uint16_t)32767u, false, true));
  HANDLE_CODE(pack_integer(bref, uncertainty_semi_major, (uint8_t)0u, (uint8_t)127u, false, true));
  HANDLE_CODE(pack_integer(bref, uncertainty_semi_minor, (uint8_t)0u, (uint8_t)127u, false, true));
  HANDLE_CODE(pack_integer(bref, orientation_of_major_axis, (uint8_t)0u, (uint8_t)179u, false, true));
  HANDLE_CODE(pack_integer(bref, uncertainty_altitude, (uint8_t)0u, (uint8_t)127u, false, true));
  HANDLE_CODE(pack_integer(bref, confidence, (uint8_t)0u, (uint8_t)100u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_access_point_position_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(latitude_sign.unpack(bref));
  HANDLE_CODE(unpack_integer(latitude, bref, (uint32_t)0u, (uint32_t)8388607u, false, true));
  HANDLE_CODE(unpack_integer(longitude, bref, (int32_t)-8388608, (int32_t)8388607, false, true));
  HANDLE_CODE(direction_of_altitude.unpack(bref));
  HANDLE_CODE(unpack_integer(altitude, bref, (uint16_t)0u, (uint16_t)32767u, false, true));
  HANDLE_CODE(unpack_integer(uncertainty_semi_major, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  HANDLE_CODE(unpack_integer(uncertainty_semi_minor, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  HANDLE_CODE(unpack_integer(orientation_of_major_axis, bref, (uint8_t)0u, (uint8_t)179u, false, true));
  HANDLE_CODE(unpack_integer(uncertainty_altitude, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  HANDLE_CODE(unpack_integer(confidence, bref, (uint8_t)0u, (uint8_t)100u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ng_ran_access_point_position_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("latitudeSign", latitude_sign.to_string());
  j.write_int("latitude", latitude);
  j.write_int("longitude", longitude);
  j.write_str("directionOfAltitude", direction_of_altitude.to_string());
  j.write_int("altitude", altitude);
  j.write_int("uncertaintySemi-major", uncertainty_semi_major);
  j.write_int("uncertaintySemi-minor", uncertainty_semi_minor);
  j.write_int("orientationOfMajorAxis", orientation_of_major_axis);
  j.write_int("uncertaintyAltitude", uncertainty_altitude);
  j.write_int("confidence", confidence);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* ng_ran_access_point_position_s::latitude_sign_opts::to_string() const
{
  static const char* names[] = {"north", "south"};
  return convert_enum_idx(names, 2, value, "ng_ran_access_point_position_s::latitude_sign_e_");
}

const char* ng_ran_access_point_position_s::direction_of_altitude_opts::to_string() const
{
  static const char* names[] = {"height", "depth"};
  return convert_enum_idx(names, 2, value, "ng_ran_access_point_position_s::direction_of_altitude_e_");
}
uint8_t ng_ran_access_point_position_s::direction_of_altitude_opts::to_number() const
{
  static const uint8_t numbers[] = {8};
  return map_enum_number(numbers, 1, value, "ng_ran_access_point_position_s::direction_of_altitude_e_");
}

// ResultRSRP-EUTRA-Item ::= SEQUENCE
OCUDUASN_CODE result_rsrp_eutra_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cgi_eutra_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pci_eutra, (uint16_t)0u, (uint16_t)503u, true, true));
  HANDLE_CODE(pack_integer(bref, earfcn, (uint32_t)0u, (uint32_t)262143u, true, true));
  if (cgi_eutra_present) {
    HANDLE_CODE(cgi_eutra.pack(bref));
  }
  HANDLE_CODE(pack_integer(bref, value_rsrp_eutra, (uint16_t)0u, (uint16_t)97u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_rsrp_eutra_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(cgi_eutra_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pci_eutra, bref, (uint16_t)0u, (uint16_t)503u, true, true));
  HANDLE_CODE(unpack_integer(earfcn, bref, (uint32_t)0u, (uint32_t)262143u, true, true));
  if (cgi_eutra_present) {
    HANDLE_CODE(cgi_eutra.unpack(bref));
  }
  HANDLE_CODE(unpack_integer(value_rsrp_eutra, bref, (uint16_t)0u, (uint16_t)97u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_rsrp_eutra_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pCI-EUTRA", pci_eutra);
  j.write_int("eARFCN", earfcn);
  if (cgi_eutra_present) {
    j.write_fieldname("cGI-EUTRA");
    cgi_eutra.to_json(j);
  }
  j.write_int("valueRSRP-EUTRA", value_rsrp_eutra);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultRSRQ-EUTRA-Item ::= SEQUENCE
OCUDUASN_CODE result_rsrq_eutra_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cgi_utra_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pci_eutra, (uint16_t)0u, (uint16_t)503u, true, true));
  HANDLE_CODE(pack_integer(bref, earfcn, (uint32_t)0u, (uint32_t)262143u, true, true));
  if (cgi_utra_present) {
    HANDLE_CODE(cgi_utra.pack(bref));
  }
  HANDLE_CODE(pack_integer(bref, value_rsrq_eutra, (uint8_t)0u, (uint8_t)34u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_rsrq_eutra_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(cgi_utra_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pci_eutra, bref, (uint16_t)0u, (uint16_t)503u, true, true));
  HANDLE_CODE(unpack_integer(earfcn, bref, (uint32_t)0u, (uint32_t)262143u, true, true));
  if (cgi_utra_present) {
    HANDLE_CODE(cgi_utra.unpack(bref));
  }
  HANDLE_CODE(unpack_integer(value_rsrq_eutra, bref, (uint8_t)0u, (uint8_t)34u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_rsrq_eutra_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pCI-EUTRA", pci_eutra);
  j.write_int("eARFCN", earfcn);
  if (cgi_utra_present) {
    j.write_fieldname("cGI-UTRA");
    cgi_utra.to_json(j);
  }
  j.write_int("valueRSRQ-EUTRA", value_rsrq_eutra);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultSS-RSRP-PerSSB-Item ::= SEQUENCE
OCUDUASN_CODE result_ss_rsrp_per_ssb_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, ssb_idx, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(pack_integer(bref, value_ss_rsrp, (uint8_t)0u, (uint8_t)127u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_ss_rsrp_per_ssb_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ssb_idx, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(unpack_integer(value_ss_rsrp, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_ss_rsrp_per_ssb_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sSB-Index", ssb_idx);
  j.write_int("valueSS-RSRP", value_ss_rsrp);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultSS-RSRP-Item ::= SEQUENCE
OCUDUASN_CODE result_ss_rsrp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cgi_nr_present, 1));
  HANDLE_CODE(bref.pack(value_ss_rsrp_cell_present, 1));
  HANDLE_CODE(bref.pack(ss_rsrp_per_ssb.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, nr_pci, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(pack_integer(bref, nr_arfcn, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.pack(bref));
  }
  if (value_ss_rsrp_cell_present) {
    HANDLE_CODE(pack_integer(bref, value_ss_rsrp_cell, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (ss_rsrp_per_ssb.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ss_rsrp_per_ssb, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_ss_rsrp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(cgi_nr_present, 1));
  HANDLE_CODE(bref.unpack(value_ss_rsrp_cell_present, 1));
  bool ss_rsrp_per_ssb_present;
  HANDLE_CODE(bref.unpack(ss_rsrp_per_ssb_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(nr_pci, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(unpack_integer(nr_arfcn, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.unpack(bref));
  }
  if (value_ss_rsrp_cell_present) {
    HANDLE_CODE(unpack_integer(value_ss_rsrp_cell, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (ss_rsrp_per_ssb_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ss_rsrp_per_ssb, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_ss_rsrp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("nR-PCI", nr_pci);
  j.write_int("nR-ARFCN", nr_arfcn);
  if (cgi_nr_present) {
    j.write_fieldname("cGI-NR");
    cgi_nr.to_json(j);
  }
  if (value_ss_rsrp_cell_present) {
    j.write_int("valueSS-RSRP-Cell", value_ss_rsrp_cell);
  }
  if (ss_rsrp_per_ssb.size() > 0) {
    j.start_array("sS-RSRP-PerSSB");
    for (const auto& e1 : ss_rsrp_per_ssb) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultSS-RSRQ-PerSSB-Item ::= SEQUENCE
OCUDUASN_CODE result_ss_rsrq_per_ssb_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, ssb_idx, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(pack_integer(bref, value_ss_rsrq, (uint8_t)0u, (uint8_t)127u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_ss_rsrq_per_ssb_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ssb_idx, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(unpack_integer(value_ss_rsrq, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_ss_rsrq_per_ssb_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sSB-Index", ssb_idx);
  j.write_int("valueSS-RSRQ", value_ss_rsrq);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultSS-RSRQ-Item ::= SEQUENCE
OCUDUASN_CODE result_ss_rsrq_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cgi_nr_present, 1));
  HANDLE_CODE(bref.pack(value_ss_rsrq_cell_present, 1));
  HANDLE_CODE(bref.pack(ss_rsrq_per_ssb.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, nr_pci, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(pack_integer(bref, nr_arfcn, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.pack(bref));
  }
  if (value_ss_rsrq_cell_present) {
    HANDLE_CODE(pack_integer(bref, value_ss_rsrq_cell, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (ss_rsrq_per_ssb.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ss_rsrq_per_ssb, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_ss_rsrq_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(cgi_nr_present, 1));
  HANDLE_CODE(bref.unpack(value_ss_rsrq_cell_present, 1));
  bool ss_rsrq_per_ssb_present;
  HANDLE_CODE(bref.unpack(ss_rsrq_per_ssb_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(nr_pci, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(unpack_integer(nr_arfcn, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.unpack(bref));
  }
  if (value_ss_rsrq_cell_present) {
    HANDLE_CODE(unpack_integer(value_ss_rsrq_cell, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (ss_rsrq_per_ssb_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ss_rsrq_per_ssb, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_ss_rsrq_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("nR-PCI", nr_pci);
  j.write_int("nR-ARFCN", nr_arfcn);
  if (cgi_nr_present) {
    j.write_fieldname("cGI-NR");
    cgi_nr.to_json(j);
  }
  if (value_ss_rsrq_cell_present) {
    j.write_int("valueSS-RSRQ-Cell", value_ss_rsrq_cell);
  }
  if (ss_rsrq_per_ssb.size() > 0) {
    j.start_array("sS-RSRQ-PerSSB");
    for (const auto& e1 : ss_rsrq_per_ssb) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultCSI-RSRP-PerCSI-RS-Item ::= SEQUENCE
OCUDUASN_CODE result_csi_rsrp_per_csi_rs_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, csi_rs_idx, (uint8_t)0u, (uint8_t)95u, false, true));
  HANDLE_CODE(pack_integer(bref, value_csi_rsrp, (uint8_t)0u, (uint8_t)127u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_csi_rsrp_per_csi_rs_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(csi_rs_idx, bref, (uint8_t)0u, (uint8_t)95u, false, true));
  HANDLE_CODE(unpack_integer(value_csi_rsrp, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_csi_rsrp_per_csi_rs_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("cSI-RS-Index", csi_rs_idx);
  j.write_int("valueCSI-RSRP", value_csi_rsrp);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultCSI-RSRP-Item ::= SEQUENCE
OCUDUASN_CODE result_csi_rsrp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cgi_nr_present, 1));
  HANDLE_CODE(bref.pack(value_csi_rsrp_cell_present, 1));
  HANDLE_CODE(bref.pack(csi_rsrp_per_csi_rs.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, nr_pci, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(pack_integer(bref, nr_arfcn, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.pack(bref));
  }
  if (value_csi_rsrp_cell_present) {
    HANDLE_CODE(pack_integer(bref, value_csi_rsrp_cell, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (csi_rsrp_per_csi_rs.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, csi_rsrp_per_csi_rs, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_csi_rsrp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(cgi_nr_present, 1));
  HANDLE_CODE(bref.unpack(value_csi_rsrp_cell_present, 1));
  bool csi_rsrp_per_csi_rs_present;
  HANDLE_CODE(bref.unpack(csi_rsrp_per_csi_rs_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(nr_pci, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(unpack_integer(nr_arfcn, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.unpack(bref));
  }
  if (value_csi_rsrp_cell_present) {
    HANDLE_CODE(unpack_integer(value_csi_rsrp_cell, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (csi_rsrp_per_csi_rs_present) {
    HANDLE_CODE(unpack_dyn_seq_of(csi_rsrp_per_csi_rs, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_csi_rsrp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("nR-PCI", nr_pci);
  j.write_int("nR-ARFCN", nr_arfcn);
  if (cgi_nr_present) {
    j.write_fieldname("cGI-NR");
    cgi_nr.to_json(j);
  }
  if (value_csi_rsrp_cell_present) {
    j.write_int("valueCSI-RSRP-Cell", value_csi_rsrp_cell);
  }
  if (csi_rsrp_per_csi_rs.size() > 0) {
    j.start_array("cSI-RSRP-PerCSI-RS");
    for (const auto& e1 : csi_rsrp_per_csi_rs) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultCSI-RSRQ-PerCSI-RS-Item ::= SEQUENCE
OCUDUASN_CODE result_csi_rsrq_per_csi_rs_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, csi_rs_idx, (uint8_t)0u, (uint8_t)95u, false, true));
  HANDLE_CODE(pack_integer(bref, value_csi_rsrq, (uint8_t)0u, (uint8_t)127u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_csi_rsrq_per_csi_rs_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(csi_rs_idx, bref, (uint8_t)0u, (uint8_t)95u, false, true));
  HANDLE_CODE(unpack_integer(value_csi_rsrq, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_csi_rsrq_per_csi_rs_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("cSI-RS-Index", csi_rs_idx);
  j.write_int("valueCSI-RSRQ", value_csi_rsrq);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultCSI-RSRQ-Item ::= SEQUENCE
OCUDUASN_CODE result_csi_rsrq_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cgi_nr_present, 1));
  HANDLE_CODE(bref.pack(value_csi_rsrq_cell_present, 1));
  HANDLE_CODE(bref.pack(csi_rsrq_per_csi_rs.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, nr_pci, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(pack_integer(bref, nr_arfcn, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.pack(bref));
  }
  if (value_csi_rsrq_cell_present) {
    HANDLE_CODE(pack_integer(bref, value_csi_rsrq_cell, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (csi_rsrq_per_csi_rs.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, csi_rsrq_per_csi_rs, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_csi_rsrq_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(cgi_nr_present, 1));
  HANDLE_CODE(bref.unpack(value_csi_rsrq_cell_present, 1));
  bool csi_rsrq_per_csi_rs_present;
  HANDLE_CODE(bref.unpack(csi_rsrq_per_csi_rs_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(nr_pci, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(unpack_integer(nr_arfcn, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.unpack(bref));
  }
  if (value_csi_rsrq_cell_present) {
    HANDLE_CODE(unpack_integer(value_csi_rsrq_cell, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (csi_rsrq_per_csi_rs_present) {
    HANDLE_CODE(unpack_dyn_seq_of(csi_rsrq_per_csi_rs, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_csi_rsrq_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("nR-PCI", nr_pci);
  j.write_int("nR-ARFCN", nr_arfcn);
  if (cgi_nr_present) {
    j.write_fieldname("cGI-NR");
    cgi_nr.to_json(j);
  }
  if (value_csi_rsrq_cell_present) {
    j.write_int("valueCSI-RSRQ-Cell", value_csi_rsrq_cell);
  }
  if (csi_rsrq_per_csi_rs.size() > 0) {
    j.start_array("cSI-RSRQ-PerCSI-RS");
    for (const auto& e1 : csi_rsrq_per_csi_rs) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MeasuredResultsValue-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t measured_results_value_ext_ie_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {32, 33, 34, 35, 36, 94, 118};
  return map_enum_number(names, 7, idx, "id");
}
bool measured_results_value_ext_ie_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {32, 33, 34, 35, 36, 94, 118};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e measured_results_value_ext_ie_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 32:
      return crit_e::ignore;
    case 33:
      return crit_e::ignore;
    case 34:
      return crit_e::ignore;
    case 35:
      return crit_e::ignore;
    case 36:
      return crit_e::ignore;
    case 94:
      return crit_e::ignore;
    case 118:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
measured_results_value_ext_ie_o::value_c measured_results_value_ext_ie_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 32:
      ret.set(value_c::types::result_ss_rsrp);
      break;
    case 33:
      ret.set(value_c::types::result_ss_rsrq);
      break;
    case 34:
      ret.set(value_c::types::result_csi_rsrp);
      break;
    case 35:
      ret.set(value_c::types::result_csi_rsrq);
      break;
    case 36:
      ret.set(value_c::types::angle_of_arrival_nr);
      break;
    case 94:
      ret.set(value_c::types::nr_tadv);
      break;
    case 118:
      ret.set(value_c::types::ue_rx_tx_time_diff);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e measured_results_value_ext_ie_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 32:
      return presence_e::mandatory;
    case 33:
      return presence_e::mandatory;
    case 34:
      return presence_e::mandatory;
    case 35:
      return presence_e::mandatory;
    case 36:
      return presence_e::mandatory;
    case 94:
      return presence_e::mandatory;
    case 118:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void measured_results_value_ext_ie_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::result_ss_rsrp:
      c = result_ss_rsrp_l{};
      break;
    case types::result_ss_rsrq:
      c = result_ss_rsrq_l{};
      break;
    case types::result_csi_rsrp:
      c = result_csi_rsrp_l{};
      break;
    case types::result_csi_rsrq:
      c = result_csi_rsrq_l{};
      break;
    case types::angle_of_arrival_nr:
      c = ul_ao_a_s{};
      break;
    case types::nr_tadv:
      c = uint16_t{};
      break;
    case types::ue_rx_tx_time_diff:
      c = uint16_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_ext_ie_o::value_c");
  }
}
result_ss_rsrp_l& measured_results_value_ext_ie_o::value_c::result_ss_rsrp()
{
  assert_choice_type(types::result_ss_rsrp, type_, "Value");
  return c.get<result_ss_rsrp_l>();
}
result_ss_rsrq_l& measured_results_value_ext_ie_o::value_c::result_ss_rsrq()
{
  assert_choice_type(types::result_ss_rsrq, type_, "Value");
  return c.get<result_ss_rsrq_l>();
}
result_csi_rsrp_l& measured_results_value_ext_ie_o::value_c::result_csi_rsrp()
{
  assert_choice_type(types::result_csi_rsrp, type_, "Value");
  return c.get<result_csi_rsrp_l>();
}
result_csi_rsrq_l& measured_results_value_ext_ie_o::value_c::result_csi_rsrq()
{
  assert_choice_type(types::result_csi_rsrq, type_, "Value");
  return c.get<result_csi_rsrq_l>();
}
ul_ao_a_s& measured_results_value_ext_ie_o::value_c::angle_of_arrival_nr()
{
  assert_choice_type(types::angle_of_arrival_nr, type_, "Value");
  return c.get<ul_ao_a_s>();
}
uint16_t& measured_results_value_ext_ie_o::value_c::nr_tadv()
{
  assert_choice_type(types::nr_tadv, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& measured_results_value_ext_ie_o::value_c::ue_rx_tx_time_diff()
{
  assert_choice_type(types::ue_rx_tx_time_diff, type_, "Value");
  return c.get<uint16_t>();
}
const result_ss_rsrp_l& measured_results_value_ext_ie_o::value_c::result_ss_rsrp() const
{
  assert_choice_type(types::result_ss_rsrp, type_, "Value");
  return c.get<result_ss_rsrp_l>();
}
const result_ss_rsrq_l& measured_results_value_ext_ie_o::value_c::result_ss_rsrq() const
{
  assert_choice_type(types::result_ss_rsrq, type_, "Value");
  return c.get<result_ss_rsrq_l>();
}
const result_csi_rsrp_l& measured_results_value_ext_ie_o::value_c::result_csi_rsrp() const
{
  assert_choice_type(types::result_csi_rsrp, type_, "Value");
  return c.get<result_csi_rsrp_l>();
}
const result_csi_rsrq_l& measured_results_value_ext_ie_o::value_c::result_csi_rsrq() const
{
  assert_choice_type(types::result_csi_rsrq, type_, "Value");
  return c.get<result_csi_rsrq_l>();
}
const ul_ao_a_s& measured_results_value_ext_ie_o::value_c::angle_of_arrival_nr() const
{
  assert_choice_type(types::angle_of_arrival_nr, type_, "Value");
  return c.get<ul_ao_a_s>();
}
const uint16_t& measured_results_value_ext_ie_o::value_c::nr_tadv() const
{
  assert_choice_type(types::nr_tadv, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& measured_results_value_ext_ie_o::value_c::ue_rx_tx_time_diff() const
{
  assert_choice_type(types::ue_rx_tx_time_diff, type_, "Value");
  return c.get<uint16_t>();
}
void measured_results_value_ext_ie_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::result_ss_rsrp:
      j.start_array("");
      for (const auto& e1 : c.get<result_ss_rsrp_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::result_ss_rsrq:
      j.start_array("");
      for (const auto& e1 : c.get<result_ss_rsrq_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::result_csi_rsrp:
      j.start_array("");
      for (const auto& e1 : c.get<result_csi_rsrp_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::result_csi_rsrq:
      j.start_array("");
      for (const auto& e1 : c.get<result_csi_rsrq_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::angle_of_arrival_nr:
      c.get<ul_ao_a_s>().to_json(j);
      break;
    case types::nr_tadv:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ue_rx_tx_time_diff:
      j.write_int(c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_ext_ie_o::value_c");
  }
}
OCUDUASN_CODE measured_results_value_ext_ie_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::result_ss_rsrp:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_ss_rsrp_l>(), 1, 9, true));
      break;
    case types::result_ss_rsrq:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_ss_rsrq_l>(), 1, 9, true));
      break;
    case types::result_csi_rsrp:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_csi_rsrp_l>(), 1, 9, true));
      break;
    case types::result_csi_rsrq:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_csi_rsrq_l>(), 1, 9, true));
      break;
    case types::angle_of_arrival_nr:
      HANDLE_CODE(c.get<ul_ao_a_s>().pack(bref));
      break;
    case types::nr_tadv:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)7690u, false, true));
      break;
    case types::ue_rx_tx_time_diff:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)61565u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_ext_ie_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE measured_results_value_ext_ie_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::result_ss_rsrp:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_ss_rsrp_l>(), bref, 1, 9, true));
      break;
    case types::result_ss_rsrq:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_ss_rsrq_l>(), bref, 1, 9, true));
      break;
    case types::result_csi_rsrp:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_csi_rsrp_l>(), bref, 1, 9, true));
      break;
    case types::result_csi_rsrq:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_csi_rsrq_l>(), bref, 1, 9, true));
      break;
    case types::angle_of_arrival_nr:
      HANDLE_CODE(c.get<ul_ao_a_s>().unpack(bref));
      break;
    case types::nr_tadv:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)7690u, false, true));
      break;
    case types::ue_rx_tx_time_diff:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)61565u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_ext_ie_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* measured_results_value_ext_ie_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"ResultSS-RSRP",
                                "ResultSS-RSRQ",
                                "ResultCSI-RSRP",
                                "ResultCSI-RSRQ",
                                "UL-AoA",
                                "INTEGER (0..7690)",
                                "INTEGER (0..61565)"};
  return convert_enum_idx(names, 7, value, "measured_results_value_ext_ie_o::value_c::types");
}

// MeasuredResultsValue ::= CHOICE
void measured_results_value_c::destroy_()
{
  switch (type_) {
    case types::result_rsrp_eutra:
      c.destroy<result_rsrp_eutra_l>();
      break;
    case types::result_rsrq_eutra:
      c.destroy<result_rsrq_eutra_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>();
      break;
    default:
      break;
  }
}
void measured_results_value_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::value_angle_of_arrival_eutra:
      break;
    case types::value_timing_advance_type1_eutra:
      break;
    case types::value_timing_advance_type2_eutra:
      break;
    case types::result_rsrp_eutra:
      c.init<result_rsrp_eutra_l>();
      break;
    case types::result_rsrq_eutra:
      c.init<result_rsrq_eutra_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_c");
  }
}
measured_results_value_c::measured_results_value_c(const measured_results_value_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::value_angle_of_arrival_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::value_timing_advance_type1_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::value_timing_advance_type2_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::result_rsrp_eutra:
      c.init(other.c.get<result_rsrp_eutra_l>());
      break;
    case types::result_rsrq_eutra:
      c.init(other.c.get<result_rsrq_eutra_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_c");
  }
}
measured_results_value_c& measured_results_value_c::operator=(const measured_results_value_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::value_angle_of_arrival_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::value_timing_advance_type1_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::value_timing_advance_type2_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::result_rsrp_eutra:
      c.set(other.c.get<result_rsrp_eutra_l>());
      break;
    case types::result_rsrq_eutra:
      c.set(other.c.get<result_rsrq_eutra_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_c");
  }

  return *this;
}
uint16_t& measured_results_value_c::set_value_angle_of_arrival_eutra()
{
  set(types::value_angle_of_arrival_eutra);
  return c.get<uint16_t>();
}
uint16_t& measured_results_value_c::set_value_timing_advance_type1_eutra()
{
  set(types::value_timing_advance_type1_eutra);
  return c.get<uint16_t>();
}
uint16_t& measured_results_value_c::set_value_timing_advance_type2_eutra()
{
  set(types::value_timing_advance_type2_eutra);
  return c.get<uint16_t>();
}
result_rsrp_eutra_l& measured_results_value_c::set_result_rsrp_eutra()
{
  set(types::result_rsrp_eutra);
  return c.get<result_rsrp_eutra_l>();
}
result_rsrq_eutra_l& measured_results_value_c::set_result_rsrq_eutra()
{
  set(types::result_rsrq_eutra);
  return c.get<result_rsrq_eutra_l>();
}
protocol_ie_single_container_s<measured_results_value_ext_ie_o>& measured_results_value_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>();
}
void measured_results_value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::value_angle_of_arrival_eutra:
      j.write_int("valueAngleOfArrival-EUTRA", c.get<uint16_t>());
      break;
    case types::value_timing_advance_type1_eutra:
      j.write_int("valueTimingAdvanceType1-EUTRA", c.get<uint16_t>());
      break;
    case types::value_timing_advance_type2_eutra:
      j.write_int("valueTimingAdvanceType2-EUTRA", c.get<uint16_t>());
      break;
    case types::result_rsrp_eutra:
      j.start_array("resultRSRP-EUTRA");
      for (const auto& e1 : c.get<result_rsrp_eutra_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::result_rsrq_eutra:
      j.start_array("resultRSRQ-EUTRA");
      for (const auto& e1 : c.get<result_rsrq_eutra_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE measured_results_value_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::value_angle_of_arrival_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)719u, false, true));
      break;
    case types::value_timing_advance_type1_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)7690u, false, true));
      break;
    case types::value_timing_advance_type2_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)7690u, false, true));
      break;
    case types::result_rsrp_eutra:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_rsrp_eutra_l>(), 1, 9, true));
      break;
    case types::result_rsrq_eutra:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_rsrq_eutra_l>(), 1, 9, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE measured_results_value_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::value_angle_of_arrival_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)719u, false, true));
      break;
    case types::value_timing_advance_type1_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)7690u, false, true));
      break;
    case types::value_timing_advance_type2_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)7690u, false, true));
      break;
    case types::result_rsrp_eutra:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_rsrp_eutra_l>(), bref, 1, 9, true));
      break;
    case types::result_rsrq_eutra:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_rsrq_eutra_l>(), bref, 1, 9, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "measured_results_value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* measured_results_value_c::types_opts::to_string() const
{
  static const char* names[] = {"valueAngleOfArrival-EUTRA",
                                "valueTimingAdvanceType1-EUTRA",
                                "valueTimingAdvanceType2-EUTRA",
                                "resultRSRP-EUTRA",
                                "resultRSRQ-EUTRA",
                                "choice-Extension"};
  return convert_enum_idx(names, 6, value, "measured_results_value_c::types");
}
uint8_t measured_results_value_c::types_opts::to_number() const
{
  switch (value) {
    case value_timing_advance_type1_eutra:
      return 1;
    case value_timing_advance_type2_eutra:
      return 2;
    default:
      invalid_enum_number(value, "measured_results_value_c::types");
  }
  return 0;
}

// NGRANHighAccuracyAccessPointPosition ::= SEQUENCE
OCUDUASN_CODE ngran_high_accuracy_access_point_position_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, latitude, (int64_t)-2147483648, (int64_t)2147483647, false, true));
  HANDLE_CODE(pack_integer(bref, longitude, (int64_t)-2147483648, (int64_t)2147483647, false, true));
  HANDLE_CODE(pack_integer(bref, altitude, (int32_t)-64000, (int32_t)1280000, false, true));
  HANDLE_CODE(pack_integer(bref, uncertainty_semi_major, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_integer(bref, uncertainty_semi_minor, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_integer(bref, orientation_of_major_axis, (uint8_t)0u, (uint8_t)179u, false, true));
  HANDLE_CODE(pack_integer(bref, horizontal_confidence, (uint8_t)0u, (uint8_t)100u, false, true));
  HANDLE_CODE(pack_integer(bref, uncertainty_altitude, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_integer(bref, vertical_confidence, (uint8_t)0u, (uint8_t)100u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ngran_high_accuracy_access_point_position_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(latitude, bref, (int64_t)-2147483648, (int64_t)2147483647, false, true));
  HANDLE_CODE(unpack_integer(longitude, bref, (int64_t)-2147483648, (int64_t)2147483647, false, true));
  HANDLE_CODE(unpack_integer(altitude, bref, (int32_t)-64000, (int32_t)1280000, false, true));
  HANDLE_CODE(unpack_integer(uncertainty_semi_major, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_integer(uncertainty_semi_minor, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_integer(orientation_of_major_axis, bref, (uint8_t)0u, (uint8_t)179u, false, true));
  HANDLE_CODE(unpack_integer(horizontal_confidence, bref, (uint8_t)0u, (uint8_t)100u, false, true));
  HANDLE_CODE(unpack_integer(uncertainty_altitude, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_integer(vertical_confidence, bref, (uint8_t)0u, (uint8_t)100u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ngran_high_accuracy_access_point_position_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("latitude", latitude);
  j.write_int("longitude", longitude);
  j.write_int("altitude", altitude);
  j.write_int("uncertaintySemi-major", uncertainty_semi_major);
  j.write_int("uncertaintySemi-minor", uncertainty_semi_minor);
  j.write_int("orientationOfMajorAxis", orientation_of_major_axis);
  j.write_int("horizontalConfidence", horizontal_confidence);
  j.write_int("uncertaintyAltitude", uncertainty_altitude);
  j.write_int("verticalConfidence", vertical_confidence);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRPPositionDirectAccuracy ::= CHOICE
void trp_position_direct_accuracy_c::destroy_()
{
  switch (type_) {
    case types::trp_position:
      c.destroy<ng_ran_access_point_position_s>();
      break;
    case types::trph_aposition:
      c.destroy<ngran_high_accuracy_access_point_position_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void trp_position_direct_accuracy_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::trp_position:
      c.init<ng_ran_access_point_position_s>();
      break;
    case types::trph_aposition:
      c.init<ngran_high_accuracy_access_point_position_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_direct_accuracy_c");
  }
}
trp_position_direct_accuracy_c::trp_position_direct_accuracy_c(const trp_position_direct_accuracy_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::trp_position:
      c.init(other.c.get<ng_ran_access_point_position_s>());
      break;
    case types::trph_aposition:
      c.init(other.c.get<ngran_high_accuracy_access_point_position_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_direct_accuracy_c");
  }
}
trp_position_direct_accuracy_c& trp_position_direct_accuracy_c::operator=(const trp_position_direct_accuracy_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::trp_position:
      c.set(other.c.get<ng_ran_access_point_position_s>());
      break;
    case types::trph_aposition:
      c.set(other.c.get<ngran_high_accuracy_access_point_position_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_direct_accuracy_c");
  }

  return *this;
}
ng_ran_access_point_position_s& trp_position_direct_accuracy_c::set_trp_position()
{
  set(types::trp_position);
  return c.get<ng_ran_access_point_position_s>();
}
ngran_high_accuracy_access_point_position_s& trp_position_direct_accuracy_c::set_trph_aposition()
{
  set(types::trph_aposition);
  return c.get<ngran_high_accuracy_access_point_position_s>();
}
protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>& trp_position_direct_accuracy_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>();
}
void trp_position_direct_accuracy_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::trp_position:
      j.write_fieldname("tRPPosition");
      c.get<ng_ran_access_point_position_s>().to_json(j);
      break;
    case types::trph_aposition:
      j.write_fieldname("tRPHAposition");
      c.get<ngran_high_accuracy_access_point_position_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_direct_accuracy_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_position_direct_accuracy_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::trp_position:
      HANDLE_CODE(c.get<ng_ran_access_point_position_s>().pack(bref));
      break;
    case types::trph_aposition:
      HANDLE_CODE(c.get<ngran_high_accuracy_access_point_position_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_direct_accuracy_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_position_direct_accuracy_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::trp_position:
      HANDLE_CODE(c.get<ng_ran_access_point_position_s>().unpack(bref));
      break;
    case types::trph_aposition:
      HANDLE_CODE(c.get<ngran_high_accuracy_access_point_position_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_direct_accuracy_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_position_direct_accuracy_c::types_opts::to_string() const
{
  static const char* names[] = {"tRPPosition", "tRPHAposition", "choice-extension"};
  return convert_enum_idx(names, 3, value, "trp_position_direct_accuracy_c::types");
}

// TRPPositionDirect ::= SEQUENCE
OCUDUASN_CODE trp_position_direct_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(accuracy.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_position_direct_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(accuracy.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_position_direct_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("accuracy");
  accuracy.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// LocalOrigin ::= SEQUENCE
OCUDUASN_CODE local_origin_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ref_point_coordinate_ha_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, relative_coordinate_id, (uint16_t)0u, (uint16_t)511u, true, true));
  HANDLE_CODE(pack_integer(bref, horizontal_axes_orientation, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (ref_point_coordinate_ha_present) {
    HANDLE_CODE(ref_point_coordinate_ha.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE local_origin_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ref_point_coordinate_ha_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(relative_coordinate_id, bref, (uint16_t)0u, (uint16_t)511u, true, true));
  HANDLE_CODE(unpack_integer(horizontal_axes_orientation, bref, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (ref_point_coordinate_ha_present) {
    HANDLE_CODE(ref_point_coordinate_ha.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void local_origin_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("relativeCoordinateID", relative_coordinate_id);
  j.write_int("horizontalAxesOrientation", horizontal_axes_orientation);
  if (ref_point_coordinate_ha_present) {
    j.write_fieldname("referencePointCoordinateHA");
    ref_point_coordinate_ha.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ReferencePoint-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t ref_point_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {162};
  return map_enum_number(names, 1, idx, "id");
}
bool ref_point_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 162 == id;
}
crit_e ref_point_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 162) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ref_point_ext_ies_o::value_c ref_point_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 162) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ref_point_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 162) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ref_point_ext_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ref_point_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ref_point_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ref_point_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"LocalOrigin"};
  return convert_enum_idx(names, 1, value, "ref_point_ext_ies_o::value_c::types");
}

// ReferencePoint ::= CHOICE
void ref_point_c::destroy_()
{
  switch (type_) {
    case types::ref_point_coordinate:
      c.destroy<ng_ran_access_point_position_s>();
      break;
    case types::ref_point_coordinate_ha:
      c.destroy<ngran_high_accuracy_access_point_position_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<ref_point_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void ref_point_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::relative_coordinate_id:
      break;
    case types::ref_point_coordinate:
      c.init<ng_ran_access_point_position_s>();
      break;
    case types::ref_point_coordinate_ha:
      c.init<ngran_high_accuracy_access_point_position_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<ref_point_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ref_point_c");
  }
}
ref_point_c::ref_point_c(const ref_point_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::relative_coordinate_id:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ref_point_coordinate:
      c.init(other.c.get<ng_ran_access_point_position_s>());
      break;
    case types::ref_point_coordinate_ha:
      c.init(other.c.get<ngran_high_accuracy_access_point_position_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<ref_point_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ref_point_c");
  }
}
ref_point_c& ref_point_c::operator=(const ref_point_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::relative_coordinate_id:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ref_point_coordinate:
      c.set(other.c.get<ng_ran_access_point_position_s>());
      break;
    case types::ref_point_coordinate_ha:
      c.set(other.c.get<ngran_high_accuracy_access_point_position_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<ref_point_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ref_point_c");
  }

  return *this;
}
uint16_t& ref_point_c::set_relative_coordinate_id()
{
  set(types::relative_coordinate_id);
  return c.get<uint16_t>();
}
ng_ran_access_point_position_s& ref_point_c::set_ref_point_coordinate()
{
  set(types::ref_point_coordinate);
  return c.get<ng_ran_access_point_position_s>();
}
ngran_high_accuracy_access_point_position_s& ref_point_c::set_ref_point_coordinate_ha()
{
  set(types::ref_point_coordinate_ha);
  return c.get<ngran_high_accuracy_access_point_position_s>();
}
protocol_ie_single_container_s<ref_point_ext_ies_o>& ref_point_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<ref_point_ext_ies_o>>();
}
void ref_point_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::relative_coordinate_id:
      j.write_int("relativeCoordinateID", c.get<uint16_t>());
      break;
    case types::ref_point_coordinate:
      j.write_fieldname("referencePointCoordinate");
      c.get<ng_ran_access_point_position_s>().to_json(j);
      break;
    case types::ref_point_coordinate_ha:
      j.write_fieldname("referencePointCoordinateHA");
      c.get<ngran_high_accuracy_access_point_position_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<ref_point_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ref_point_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ref_point_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::relative_coordinate_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)511u, true, true));
      break;
    case types::ref_point_coordinate:
      HANDLE_CODE(c.get<ng_ran_access_point_position_s>().pack(bref));
      break;
    case types::ref_point_coordinate_ha:
      HANDLE_CODE(c.get<ngran_high_accuracy_access_point_position_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ref_point_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ref_point_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ref_point_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::relative_coordinate_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)511u, true, true));
      break;
    case types::ref_point_coordinate:
      HANDLE_CODE(c.get<ng_ran_access_point_position_s>().unpack(bref));
      break;
    case types::ref_point_coordinate_ha:
      HANDLE_CODE(c.get<ngran_high_accuracy_access_point_position_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ref_point_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ref_point_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ref_point_c::types_opts::to_string() const
{
  static const char* names[] = {
      "relativeCoordinateID", "referencePointCoordinate", "referencePointCoordinateHA", "choice-Extension"};
  return convert_enum_idx(names, 4, value, "ref_point_c::types");
}

// TRPReferencePointType ::= CHOICE
void trp_ref_point_type_c::destroy_()
{
  switch (type_) {
    case types::trp_position_relative_geodetic:
      c.destroy<relative_geodetic_location_s>();
      break;
    case types::trp_position_relative_cartesian:
      c.destroy<relative_cartesian_location_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void trp_ref_point_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::trp_position_relative_geodetic:
      c.init<relative_geodetic_location_s>();
      break;
    case types::trp_position_relative_cartesian:
      c.init<relative_cartesian_location_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_ref_point_type_c");
  }
}
trp_ref_point_type_c::trp_ref_point_type_c(const trp_ref_point_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::trp_position_relative_geodetic:
      c.init(other.c.get<relative_geodetic_location_s>());
      break;
    case types::trp_position_relative_cartesian:
      c.init(other.c.get<relative_cartesian_location_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_ref_point_type_c");
  }
}
trp_ref_point_type_c& trp_ref_point_type_c::operator=(const trp_ref_point_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::trp_position_relative_geodetic:
      c.set(other.c.get<relative_geodetic_location_s>());
      break;
    case types::trp_position_relative_cartesian:
      c.set(other.c.get<relative_cartesian_location_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_ref_point_type_c");
  }

  return *this;
}
relative_geodetic_location_s& trp_ref_point_type_c::set_trp_position_relative_geodetic()
{
  set(types::trp_position_relative_geodetic);
  return c.get<relative_geodetic_location_s>();
}
relative_cartesian_location_s& trp_ref_point_type_c::set_trp_position_relative_cartesian()
{
  set(types::trp_position_relative_cartesian);
  return c.get<relative_cartesian_location_s>();
}
protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>& trp_ref_point_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>();
}
void trp_ref_point_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::trp_position_relative_geodetic:
      j.write_fieldname("tRPPositionRelativeGeodetic");
      c.get<relative_geodetic_location_s>().to_json(j);
      break;
    case types::trp_position_relative_cartesian:
      j.write_fieldname("tRPPositionRelativeCartesian");
      c.get<relative_cartesian_location_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_ref_point_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_ref_point_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::trp_position_relative_geodetic:
      HANDLE_CODE(c.get<relative_geodetic_location_s>().pack(bref));
      break;
    case types::trp_position_relative_cartesian:
      HANDLE_CODE(c.get<relative_cartesian_location_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_ref_point_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_ref_point_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::trp_position_relative_geodetic:
      HANDLE_CODE(c.get<relative_geodetic_location_s>().unpack(bref));
      break;
    case types::trp_position_relative_cartesian:
      HANDLE_CODE(c.get<relative_cartesian_location_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_ref_point_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_ref_point_type_c::types_opts::to_string() const
{
  static const char* names[] = {"tRPPositionRelativeGeodetic", "tRPPositionRelativeCartesian", "choice-extension"};
  return convert_enum_idx(names, 3, value, "trp_ref_point_type_c::types");
}

// TRPPositionReferenced ::= SEQUENCE
OCUDUASN_CODE trp_position_refd_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ref_point.pack(bref));
  HANDLE_CODE(ref_point_type.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_position_refd_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ref_point.unpack(bref));
  HANDLE_CODE(ref_point_type.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_position_refd_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("referencePoint");
  ref_point.to_json(j);
  j.write_fieldname("referencePointType");
  ref_point_type.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRPPositionDefinitionType ::= CHOICE
void trp_position_definition_type_c::destroy_()
{
  switch (type_) {
    case types::direct:
      c.destroy<trp_position_direct_s>();
      break;
    case types::refd:
      c.destroy<trp_position_refd_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void trp_position_definition_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::direct:
      c.init<trp_position_direct_s>();
      break;
    case types::refd:
      c.init<trp_position_refd_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_definition_type_c");
  }
}
trp_position_definition_type_c::trp_position_definition_type_c(const trp_position_definition_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::direct:
      c.init(other.c.get<trp_position_direct_s>());
      break;
    case types::refd:
      c.init(other.c.get<trp_position_refd_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_definition_type_c");
  }
}
trp_position_definition_type_c& trp_position_definition_type_c::operator=(const trp_position_definition_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::direct:
      c.set(other.c.get<trp_position_direct_s>());
      break;
    case types::refd:
      c.set(other.c.get<trp_position_refd_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_definition_type_c");
  }

  return *this;
}
trp_position_direct_s& trp_position_definition_type_c::set_direct()
{
  set(types::direct);
  return c.get<trp_position_direct_s>();
}
trp_position_refd_s& trp_position_definition_type_c::set_refd()
{
  set(types::refd);
  return c.get<trp_position_refd_s>();
}
protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>& trp_position_definition_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>();
}
void trp_position_definition_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::direct:
      j.write_fieldname("direct");
      c.get<trp_position_direct_s>().to_json(j);
      break;
    case types::refd:
      j.write_fieldname("referenced");
      c.get<trp_position_refd_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_definition_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_position_definition_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::direct:
      HANDLE_CODE(c.get<trp_position_direct_s>().pack(bref));
      break;
    case types::refd:
      HANDLE_CODE(c.get<trp_position_refd_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_definition_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_position_definition_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::direct:
      HANDLE_CODE(c.get<trp_position_direct_s>().unpack(bref));
      break;
    case types::refd:
      HANDLE_CODE(c.get<trp_position_refd_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_position_definition_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_position_definition_type_c::types_opts::to_string() const
{
  static const char* names[] = {"direct", "referenced", "choice-extension"};
  return convert_enum_idx(names, 3, value, "trp_position_definition_type_c::types");
}

// GeographicalCoordinates-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t geographical_coordinates_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78};
  return map_enum_number(names, 1, idx, "id");
}
bool geographical_coordinates_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 78 == id;
}
crit_e geographical_coordinates_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 78) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
geographical_coordinates_ext_ies_o::ext_c geographical_coordinates_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 78) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e geographical_coordinates_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 78) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void geographical_coordinates_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.start_array("");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
}
OCUDUASN_CODE geographical_coordinates_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 16, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE geographical_coordinates_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 16, true));
  return OCUDUASN_SUCCESS;
}

const char* geographical_coordinates_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"ARPLocationInformation"};
  return convert_enum_idx(names, 1, value, "geographical_coordinates_ext_ies_o::ext_c::types");
}

// GeographicalCoordinates ::= SEQUENCE
OCUDUASN_CODE geographical_coordinates_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(dl_prs_res_coordinates_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(trp_position_definition_type.pack(bref));
  if (dl_prs_res_coordinates_present) {
    HANDLE_CODE(dl_prs_res_coordinates.pack(bref));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE geographical_coordinates_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(dl_prs_res_coordinates_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(trp_position_definition_type.unpack(bref));
  if (dl_prs_res_coordinates_present) {
    HANDLE_CODE(dl_prs_res_coordinates.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void geographical_coordinates_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tRPPositionDefinitionType");
  trp_position_definition_type.to_json(j);
  if (dl_prs_res_coordinates_present) {
    j.write_fieldname("dLPRSResourceCoordinates");
    dl_prs_res_coordinates.to_json(j);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TimeStampSlotIndex-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t time_stamp_slot_idx_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {119, 120};
  return map_enum_number(names, 2, idx, "id");
}
bool time_stamp_slot_idx_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {119, 120};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e time_stamp_slot_idx_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 119:
      return crit_e::reject;
    case 120:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
time_stamp_slot_idx_ext_ies_o::value_c time_stamp_slot_idx_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 119:
      ret.set(value_c::types::scs_480);
      break;
    case 120:
      ret.set(value_c::types::scs_960);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e time_stamp_slot_idx_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 119:
      return presence_e::mandatory;
    case 120:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void time_stamp_slot_idx_ext_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::scs_480:
      c = uint16_t{};
      break;
    case types::scs_960:
      c = uint16_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_ext_ies_o::value_c");
  }
}
uint16_t& time_stamp_slot_idx_ext_ies_o::value_c::scs_480()
{
  assert_choice_type(types::scs_480, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& time_stamp_slot_idx_ext_ies_o::value_c::scs_960()
{
  assert_choice_type(types::scs_960, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& time_stamp_slot_idx_ext_ies_o::value_c::scs_480() const
{
  assert_choice_type(types::scs_480, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& time_stamp_slot_idx_ext_ies_o::value_c::scs_960() const
{
  assert_choice_type(types::scs_960, type_, "Value");
  return c.get<uint16_t>();
}
void time_stamp_slot_idx_ext_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::scs_480:
      j.write_int(c.get<uint16_t>());
      break;
    case types::scs_960:
      j.write_int(c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_ext_ies_o::value_c");
  }
}
OCUDUASN_CODE time_stamp_slot_idx_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::scs_480:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)319u, false, true));
      break;
    case types::scs_960:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)639u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_ext_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_stamp_slot_idx_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::scs_480:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)319u, false, true));
      break;
    case types::scs_960:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)639u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_ext_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* time_stamp_slot_idx_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..319)", "INTEGER (0..639)"};
  return convert_enum_idx(names, 2, value, "time_stamp_slot_idx_ext_ies_o::value_c::types");
}

// TimeStampSlotIndex ::= CHOICE
void time_stamp_slot_idx_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void time_stamp_slot_idx_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::scs_15:
      break;
    case types::scs_30:
      break;
    case types::scs_60:
      break;
    case types::scs_120:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_c");
  }
}
time_stamp_slot_idx_c::time_stamp_slot_idx_c(const time_stamp_slot_idx_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::scs_15:
      c.init(other.c.get<uint8_t>());
      break;
    case types::scs_30:
      c.init(other.c.get<uint8_t>());
      break;
    case types::scs_60:
      c.init(other.c.get<uint8_t>());
      break;
    case types::scs_120:
      c.init(other.c.get<uint8_t>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_c");
  }
}
time_stamp_slot_idx_c& time_stamp_slot_idx_c::operator=(const time_stamp_slot_idx_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::scs_15:
      c.set(other.c.get<uint8_t>());
      break;
    case types::scs_30:
      c.set(other.c.get<uint8_t>());
      break;
    case types::scs_60:
      c.set(other.c.get<uint8_t>());
      break;
    case types::scs_120:
      c.set(other.c.get<uint8_t>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_c");
  }

  return *this;
}
uint8_t& time_stamp_slot_idx_c::set_scs_15()
{
  set(types::scs_15);
  return c.get<uint8_t>();
}
uint8_t& time_stamp_slot_idx_c::set_scs_30()
{
  set(types::scs_30);
  return c.get<uint8_t>();
}
uint8_t& time_stamp_slot_idx_c::set_scs_60()
{
  set(types::scs_60);
  return c.get<uint8_t>();
}
uint8_t& time_stamp_slot_idx_c::set_scs_120()
{
  set(types::scs_120);
  return c.get<uint8_t>();
}
protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>& time_stamp_slot_idx_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>();
}
void time_stamp_slot_idx_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::scs_15:
      j.write_int("sCS-15", c.get<uint8_t>());
      break;
    case types::scs_30:
      j.write_int("sCS-30", c.get<uint8_t>());
      break;
    case types::scs_60:
      j.write_int("sCS-60", c.get<uint8_t>());
      break;
    case types::scs_120:
      j.write_int("sCS-120", c.get<uint8_t>());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_c");
  }
  j.end_obj();
}
OCUDUASN_CODE time_stamp_slot_idx_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::scs_15:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)9u, false, true));
      break;
    case types::scs_30:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)19u, false, true));
      break;
    case types::scs_60:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)39u, false, true));
      break;
    case types::scs_120:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)79u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_stamp_slot_idx_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::scs_15:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)9u, false, true));
      break;
    case types::scs_30:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)19u, false, true));
      break;
    case types::scs_60:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)39u, false, true));
      break;
    case types::scs_120:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)79u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "time_stamp_slot_idx_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* time_stamp_slot_idx_c::types_opts::to_string() const
{
  static const char* names[] = {"sCS-15", "sCS-30", "sCS-60", "sCS-120", "choice-extension"};
  return convert_enum_idx(names, 5, value, "time_stamp_slot_idx_c::types");
}
int8_t time_stamp_slot_idx_c::types_opts::to_number() const
{
  static const int8_t numbers[] = {-15, -30, -60, -120};
  return map_enum_number(numbers, 4, value, "time_stamp_slot_idx_c::types");
}

// TimeStamp-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t time_stamp_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {127};
  return map_enum_number(names, 1, idx, "id");
}
bool time_stamp_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 127 == id;
}
crit_e time_stamp_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 127) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
time_stamp_ext_ies_o::ext_c time_stamp_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 127) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e time_stamp_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 127) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void time_stamp_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_int(c);
}
OCUDUASN_CODE time_stamp_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_integer(bref, c, (uint8_t)0u, (uint8_t)13u, false, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_stamp_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_integer(c, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  return OCUDUASN_SUCCESS;
}

const char* time_stamp_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..13)"};
  return convert_enum_idx(names, 1, value, "time_stamp_ext_ies_o::ext_c::types");
}
uint8_t time_stamp_ext_ies_o::ext_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "time_stamp_ext_ies_o::ext_c::types");
}

// TimeStamp ::= SEQUENCE
OCUDUASN_CODE time_stamp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(meas_time_present, 1));
  HANDLE_CODE(bref.pack(ie_ext.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true));
  HANDLE_CODE(slot_idx.pack(bref));
  if (meas_time_present) {
    HANDLE_CODE(meas_time.pack(bref));
  }
  if (ie_ext.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_ext, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_stamp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(meas_time_present, 1));
  bool ie_ext_present;
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true));
  HANDLE_CODE(slot_idx.unpack(bref));
  if (meas_time_present) {
    HANDLE_CODE(meas_time.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_ext, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void time_stamp_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("systemFrameNumber", sys_frame_num);
  j.write_fieldname("slotIndex");
  slot_idx.to_json(j);
  if (meas_time_present) {
    j.write_fieldname("measurementTime");
    meas_time.to_json(j);
  }
  if (ie_ext.size() > 0) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// Mobile-TRP-LocationInformation ::= SEQUENCE
OCUDUASN_CODE mobile_trp_location_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(velocity_info.size() > 0, 1));
  HANDLE_CODE(bref.pack(location_time_stamp_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(location_info.pack(bref));
  if (velocity_info.size() > 0) {
    HANDLE_CODE(velocity_info.pack(bref));
  }
  if (location_time_stamp_present) {
    HANDLE_CODE(location_time_stamp.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mobile_trp_location_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool velocity_info_present;
  HANDLE_CODE(bref.unpack(velocity_info_present, 1));
  HANDLE_CODE(bref.unpack(location_time_stamp_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(location_info.unpack(bref));
  if (velocity_info_present) {
    HANDLE_CODE(velocity_info.unpack(bref));
  }
  if (location_time_stamp_present) {
    HANDLE_CODE(location_time_stamp.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mobile_trp_location_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("location-Information", location_info.to_string());
  if (velocity_info.size() > 0) {
    j.write_str("velocity-Information", velocity_info.to_string());
  }
  if (location_time_stamp_present) {
    j.write_fieldname("location-time-stamp");
    location_time_stamp.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MeasuredResultsAssociatedInfoItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t measured_results_associated_info_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {164};
  return map_enum_number(names, 1, idx, "id");
}
bool measured_results_associated_info_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 164 == id;
}
crit_e measured_results_associated_info_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 164) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
measured_results_associated_info_item_ext_ies_o::ext_c
measured_results_associated_info_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 164) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e measured_results_associated_info_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 164) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void measured_results_associated_info_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_str(c.to_string());
}
OCUDUASN_CODE measured_results_associated_info_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE measured_results_associated_info_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* measured_results_associated_info_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"DL-reference-signal-UERxTx-TD"};
  return convert_enum_idx(names, 1, value, "measured_results_associated_info_item_ext_ies_o::ext_c::types");
}

// MeasuredResultsAssociatedInfoItem ::= SEQUENCE
OCUDUASN_CODE measured_results_associated_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(time_stamp_present, 1));
  HANDLE_CODE(bref.pack(meas_quality_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  if (time_stamp_present) {
    HANDLE_CODE(time_stamp.pack(bref));
  }
  if (meas_quality_present) {
    HANDLE_CODE(meas_quality.pack(bref));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE measured_results_associated_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(time_stamp_present, 1));
  HANDLE_CODE(bref.unpack(meas_quality_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (time_stamp_present) {
    HANDLE_CODE(time_stamp.unpack(bref));
  }
  if (meas_quality_present) {
    HANDLE_CODE(meas_quality.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void measured_results_associated_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (time_stamp_present) {
    j.write_fieldname("timeStamp");
    time_stamp.to_json(j);
  }
  if (meas_quality_present) {
    j.write_fieldname("measurementQuality");
    meas_quality.to_json(j);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// E-CID-MeasurementResult-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t e_c_id_meas_result_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {37, 116, 153};
  return map_enum_number(names, 3, idx, "id");
}
bool e_c_id_meas_result_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {37, 116, 153};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_c_id_meas_result_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 37:
      return crit_e::ignore;
    case 116:
      return crit_e::ignore;
    case 153:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e_c_id_meas_result_ext_ies_o::ext_c e_c_id_meas_result_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 37:
      ret.set(ext_c::types::geographical_coordinates);
      break;
    case 116:
      ret.set(ext_c::types::mobile_access_point_location);
      break;
    case 153:
      ret.set(ext_c::types::measured_results_associated_info_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e_c_id_meas_result_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 37:
      return presence_e::optional;
    case 116:
      return presence_e::optional;
    case 153:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void e_c_id_meas_result_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::geographical_coordinates:
      c = geographical_coordinates_s{};
      break;
    case types::mobile_access_point_location:
      c = mobile_trp_location_info_s{};
      break;
    case types::measured_results_associated_info_list:
      c = measured_results_associated_info_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_result_ext_ies_o::ext_c");
  }
}
geographical_coordinates_s& e_c_id_meas_result_ext_ies_o::ext_c::geographical_coordinates()
{
  assert_choice_type(types::geographical_coordinates, type_, "Extension");
  return c.get<geographical_coordinates_s>();
}
mobile_trp_location_info_s& e_c_id_meas_result_ext_ies_o::ext_c::mobile_access_point_location()
{
  assert_choice_type(types::mobile_access_point_location, type_, "Extension");
  return c.get<mobile_trp_location_info_s>();
}
measured_results_associated_info_list_l& e_c_id_meas_result_ext_ies_o::ext_c::measured_results_associated_info_list()
{
  assert_choice_type(types::measured_results_associated_info_list, type_, "Extension");
  return c.get<measured_results_associated_info_list_l>();
}
const geographical_coordinates_s& e_c_id_meas_result_ext_ies_o::ext_c::geographical_coordinates() const
{
  assert_choice_type(types::geographical_coordinates, type_, "Extension");
  return c.get<geographical_coordinates_s>();
}
const mobile_trp_location_info_s& e_c_id_meas_result_ext_ies_o::ext_c::mobile_access_point_location() const
{
  assert_choice_type(types::mobile_access_point_location, type_, "Extension");
  return c.get<mobile_trp_location_info_s>();
}
const measured_results_associated_info_list_l&
e_c_id_meas_result_ext_ies_o::ext_c::measured_results_associated_info_list() const
{
  assert_choice_type(types::measured_results_associated_info_list, type_, "Extension");
  return c.get<measured_results_associated_info_list_l>();
}
void e_c_id_meas_result_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::geographical_coordinates:
      c.get<geographical_coordinates_s>().to_json(j);
      break;
    case types::mobile_access_point_location:
      c.get<mobile_trp_location_info_s>().to_json(j);
      break;
    case types::measured_results_associated_info_list:
      j.start_array("");
      for (const auto& e1 : c.get<measured_results_associated_info_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_result_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE e_c_id_meas_result_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::geographical_coordinates:
      HANDLE_CODE(c.get<geographical_coordinates_s>().pack(bref));
      break;
    case types::mobile_access_point_location:
      HANDLE_CODE(c.get<mobile_trp_location_info_s>().pack(bref));
      break;
    case types::measured_results_associated_info_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<measured_results_associated_info_list_l>(), 1, 64, true));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_result_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_result_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::geographical_coordinates:
      HANDLE_CODE(c.get<geographical_coordinates_s>().unpack(bref));
      break;
    case types::mobile_access_point_location:
      HANDLE_CODE(c.get<mobile_trp_location_info_s>().unpack(bref));
      break;
    case types::measured_results_associated_info_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<measured_results_associated_info_list_l>(), bref, 1, 64, true));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_result_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e_c_id_meas_result_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {
      "GeographicalCoordinates", "Mobile-TRP-LocationInformation", "MeasuredResultsAssociatedInfoList"};
  return convert_enum_idx(names, 3, value, "e_c_id_meas_result_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<e_c_id_meas_result_ext_ies_o>;

OCUDUASN_CODE e_c_id_meas_result_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += geographical_coordinates_present ? 1 : 0;
  nof_ies += mobile_access_point_location_present ? 1 : 0;
  nof_ies += measured_results_associated_info_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (geographical_coordinates_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)37, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(geographical_coordinates.pack(bref));
  }
  if (mobile_access_point_location_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)116, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mobile_access_point_location.pack(bref));
  }
  if (measured_results_associated_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)153, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, measured_results_associated_info_list, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_result_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_result_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 37: {
        geographical_coordinates_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(geographical_coordinates.unpack(bref));
        break;
      }
      case 116: {
        mobile_access_point_location_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mobile_access_point_location.unpack(bref));
        break;
      }
      case 153: {
        measured_results_associated_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(measured_results_associated_info_list, bref, 1, 64, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void e_c_id_meas_result_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (geographical_coordinates_present) {
    j.start_obj();
    j.write_int("id", 37);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    geographical_coordinates.to_json(j);
    j.end_obj();
  }
  if (mobile_access_point_location_present) {
    j.start_obj();
    j.write_int("id", 116);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mobile_access_point_location.to_json(j);
    j.end_obj();
  }
  if (measured_results_associated_info_list_present) {
    j.start_obj();
    j.write_int("id", 153);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : measured_results_associated_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// E-CID-MeasurementResult ::= SEQUENCE
OCUDUASN_CODE e_c_id_meas_result_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ng_ran_access_point_position_present, 1));
  HANDLE_CODE(bref.pack(measured_results.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(serving_cell_id.pack(bref));
  HANDLE_CODE(serving_cell_tac.pack(bref));
  if (ng_ran_access_point_position_present) {
    HANDLE_CODE(ng_ran_access_point_position.pack(bref));
  }
  if (measured_results.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, measured_results, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_result_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ng_ran_access_point_position_present, 1));
  bool measured_results_present;
  HANDLE_CODE(bref.unpack(measured_results_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(serving_cell_id.unpack(bref));
  HANDLE_CODE(serving_cell_tac.unpack(bref));
  if (ng_ran_access_point_position_present) {
    HANDLE_CODE(ng_ran_access_point_position.unpack(bref));
  }
  if (measured_results_present) {
    HANDLE_CODE(unpack_dyn_seq_of(measured_results, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e_c_id_meas_result_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("servingCell-ID");
  serving_cell_id.to_json(j);
  j.write_str("servingCellTAC", serving_cell_tac.to_string());
  if (ng_ran_access_point_position_present) {
    j.write_fieldname("nG-RANAccessPointPosition");
    ng_ran_access_point_position.to_json(j);
  }
  if (measured_results.size() > 0) {
    j.start_array("measuredResults");
    for (const auto& e1 : measured_results) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ExtendedAdditionalPathList-Item ::= SEQUENCE
OCUDUASN_CODE extended_add_path_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(path_quality_present, 1));
  HANDLE_CODE(bref.pack(multiple_ul_ao_a_present, 1));
  HANDLE_CODE(bref.pack(path_pwr_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(relative_time_of_path.pack(bref));
  if (path_quality_present) {
    HANDLE_CODE(path_quality.pack(bref));
  }
  if (multiple_ul_ao_a_present) {
    HANDLE_CODE(multiple_ul_ao_a.pack(bref));
  }
  if (path_pwr_present) {
    HANDLE_CODE(path_pwr.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE extended_add_path_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(path_quality_present, 1));
  HANDLE_CODE(bref.unpack(multiple_ul_ao_a_present, 1));
  HANDLE_CODE(bref.unpack(path_pwr_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(relative_time_of_path.unpack(bref));
  if (path_quality_present) {
    HANDLE_CODE(path_quality.unpack(bref));
  }
  if (multiple_ul_ao_a_present) {
    HANDLE_CODE(multiple_ul_ao_a.unpack(bref));
  }
  if (path_pwr_present) {
    HANDLE_CODE(path_pwr.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void extended_add_path_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("relativeTimeOfPath");
  relative_time_of_path.to_json(j);
  if (path_quality_present) {
    j.write_fieldname("pathQuality");
    path_quality.to_json(j);
  }
  if (multiple_ul_ao_a_present) {
    j.write_fieldname("multipleULAoA");
    multiple_ul_ao_a.to_json(j);
  }
  if (path_pwr_present) {
    j.write_fieldname("pathPower");
    path_pwr.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GNBRxTxTimeDiffMeas-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t gnb_rx_tx_time_diff_meas_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {141, 142, 143, 144, 145, 146};
  return map_enum_number(names, 6, idx, "id");
}
bool gnb_rx_tx_time_diff_meas_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {141, 142, 143, 144, 145, 146};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_rx_tx_time_diff_meas_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 141:
      return crit_e::ignore;
    case 142:
      return crit_e::ignore;
    case 143:
      return crit_e::ignore;
    case 144:
      return crit_e::ignore;
    case 145:
      return crit_e::ignore;
    case 146:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
gnb_rx_tx_time_diff_meas_ext_ies_o::value_c gnb_rx_tx_time_diff_meas_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 141:
      ret.set(value_c::types::report_granularitykminus1);
      break;
    case 142:
      ret.set(value_c::types::report_granularitykminus2);
      break;
    case 143:
      ret.set(value_c::types::report_granularitykminus3);
      break;
    case 144:
      ret.set(value_c::types::report_granularitykminus4);
      break;
    case 145:
      ret.set(value_c::types::report_granularitykminus5);
      break;
    case 146:
      ret.set(value_c::types::report_granularitykminus6);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e gnb_rx_tx_time_diff_meas_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 141:
      return presence_e::mandatory;
    case 142:
      return presence_e::mandatory;
    case 143:
      return presence_e::mandatory;
    case 144:
      return presence_e::mandatory;
    case 145:
      return presence_e::mandatory;
    case 146:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::report_granularitykminus1:
      c = uint32_t{};
      break;
    case types::report_granularitykminus2:
      c = uint32_t{};
      break;
    case types::report_granularitykminus3:
      c = uint32_t{};
      break;
    case types::report_granularitykminus4:
      c = uint32_t{};
      break;
    case types::report_granularitykminus5:
      c = uint32_t{};
      break;
    case types::report_granularitykminus6:
      c = uint32_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_ext_ies_o::value_c");
  }
}
uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus1()
{
  assert_choice_type(types::report_granularitykminus1, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus2()
{
  assert_choice_type(types::report_granularitykminus2, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus3()
{
  assert_choice_type(types::report_granularitykminus3, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus4()
{
  assert_choice_type(types::report_granularitykminus4, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus5()
{
  assert_choice_type(types::report_granularitykminus5, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus6()
{
  assert_choice_type(types::report_granularitykminus6, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus1() const
{
  assert_choice_type(types::report_granularitykminus1, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus2() const
{
  assert_choice_type(types::report_granularitykminus2, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus3() const
{
  assert_choice_type(types::report_granularitykminus3, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus4() const
{
  assert_choice_type(types::report_granularitykminus4, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus5() const
{
  assert_choice_type(types::report_granularitykminus5, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::report_granularitykminus6() const
{
  assert_choice_type(types::report_granularitykminus6, type_, "Value");
  return c.get<uint32_t>();
}
void gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::report_granularitykminus1:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus2:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus3:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus4:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus5:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus6:
      j.write_int(c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_ext_ies_o::value_c");
  }
}
OCUDUASN_CODE gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::report_granularitykminus1:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)3940097u, false, true));
      break;
    case types::report_granularitykminus2:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)7880193u, false, true));
      break;
    case types::report_granularitykminus3:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)15760385u, false, true));
      break;
    case types::report_granularitykminus4:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)31520769u, false, true));
      break;
    case types::report_granularitykminus5:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)63041537u, false, true));
      break;
    case types::report_granularitykminus6:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)126083073u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_ext_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::report_granularitykminus1:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)3940097u, false, true));
      break;
    case types::report_granularitykminus2:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)7880193u, false, true));
      break;
    case types::report_granularitykminus3:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)15760385u, false, true));
      break;
    case types::report_granularitykminus4:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)31520769u, false, true));
      break;
    case types::report_granularitykminus5:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)63041537u, false, true));
      break;
    case types::report_granularitykminus6:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)126083073u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_ext_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..3940097)",
                                "INTEGER (0..7880193)",
                                "INTEGER (0..15760385)",
                                "INTEGER (0..31520769)",
                                "INTEGER (0..63041537)",
                                "INTEGER (0..126083073)"};
  return convert_enum_idx(names, 6, value, "gnb_rx_tx_time_diff_meas_ext_ies_o::value_c::types");
}

// GNBRxTxTimeDiffMeas ::= CHOICE
void gnb_rx_tx_time_diff_meas_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void gnb_rx_tx_time_diff_meas_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::k0:
      break;
    case types::k1:
      break;
    case types::k2:
      break;
    case types::k3:
      break;
    case types::k4:
      break;
    case types::k5:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_c");
  }
}
gnb_rx_tx_time_diff_meas_c::gnb_rx_tx_time_diff_meas_c(const gnb_rx_tx_time_diff_meas_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::k0:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k1:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k2:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k3:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k4:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k5:
      c.init(other.c.get<uint16_t>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_c");
  }
}
gnb_rx_tx_time_diff_meas_c& gnb_rx_tx_time_diff_meas_c::operator=(const gnb_rx_tx_time_diff_meas_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::k0:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k1:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k2:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k3:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k4:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k5:
      c.set(other.c.get<uint16_t>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_c");
  }

  return *this;
}
uint32_t& gnb_rx_tx_time_diff_meas_c::set_k0()
{
  set(types::k0);
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_c::set_k1()
{
  set(types::k1);
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_c::set_k2()
{
  set(types::k2);
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_c::set_k3()
{
  set(types::k3);
  return c.get<uint32_t>();
}
uint32_t& gnb_rx_tx_time_diff_meas_c::set_k4()
{
  set(types::k4);
  return c.get<uint32_t>();
}
uint16_t& gnb_rx_tx_time_diff_meas_c::set_k5()
{
  set(types::k5);
  return c.get<uint16_t>();
}
protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>& gnb_rx_tx_time_diff_meas_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>();
}
void gnb_rx_tx_time_diff_meas_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::k0:
      j.write_int("k0", c.get<uint32_t>());
      break;
    case types::k1:
      j.write_int("k1", c.get<uint32_t>());
      break;
    case types::k2:
      j.write_int("k2", c.get<uint32_t>());
      break;
    case types::k3:
      j.write_int("k3", c.get<uint32_t>());
      break;
    case types::k4:
      j.write_int("k4", c.get<uint32_t>());
      break;
    case types::k5:
      j.write_int("k5", c.get<uint16_t>());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_c");
  }
  j.end_obj();
}
OCUDUASN_CODE gnb_rx_tx_time_diff_meas_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::k0:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)1970049u, false, true));
      break;
    case types::k1:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)985025u, false, true));
      break;
    case types::k2:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)492513u, false, true));
      break;
    case types::k3:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)246257u, false, true));
      break;
    case types::k4:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)123129u, false, true));
      break;
    case types::k5:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)61565u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_rx_tx_time_diff_meas_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::k0:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)1970049u, false, true));
      break;
    case types::k1:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)985025u, false, true));
      break;
    case types::k2:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)492513u, false, true));
      break;
    case types::k3:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)246257u, false, true));
      break;
    case types::k4:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)123129u, false, true));
      break;
    case types::k5:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)61565u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_meas_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* gnb_rx_tx_time_diff_meas_c::types_opts::to_string() const
{
  static const char* names[] = {"k0", "k1", "k2", "k3", "k4", "k5", "choice-extension"};
  return convert_enum_idx(names, 7, value, "gnb_rx_tx_time_diff_meas_c::types");
}
uint8_t gnb_rx_tx_time_diff_meas_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1, 2, 3, 4, 5};
  return map_enum_number(numbers, 6, value, "gnb_rx_tx_time_diff_meas_c::types");
}

// RxTxTimingErrorMargin ::= ENUMERATED
const char* rx_tx_timing_error_margin_opts::to_string() const
{
  static const char* names[] = {"tc0dot5",
                                "tc1",
                                "tc2",
                                "tc4",
                                "tc8",
                                "tc12",
                                "tc16",
                                "tc20",
                                "tc24",
                                "tc32",
                                "tc40",
                                "tc48",
                                "tc64",
                                "tc80",
                                "tc96",
                                "tc128"};
  return convert_enum_idx(names, 16, value, "rx_tx_timing_error_margin_e");
}
float rx_tx_timing_error_margin_opts::to_number() const
{
  static const float numbers[] = {
      0.5, 1.0, 2.0, 4.0, 8.0, 12.0, 16.0, 20.0, 24.0, 32.0, 40.0, 48.0, 64.0, 80.0, 96.0, 128.0};
  return map_enum_number(numbers, 16, value, "rx_tx_timing_error_margin_e");
}
const char* rx_tx_timing_error_margin_opts::to_number_string() const
{
  static const char* number_strs[] = {
      "0.5", "1", "2", "4", "8", "12", "16", "20", "24", "32", "40", "48", "64", "80", "96", "128"};
  return convert_enum_idx(number_strs, 16, value, "rx_tx_timing_error_margin_e");
}

// TRP-RxTx-TEGInformation ::= SEQUENCE
OCUDUASN_CODE trp_rx_tx_teg_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_rx_tx_teg_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(trp_rx_tx_timing_error_margin.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_rx_tx_teg_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_rx_tx_teg_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(trp_rx_tx_timing_error_margin.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_rx_tx_teg_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-RxTx-TEGID", trp_rx_tx_teg_id);
  j.write_str("tRP-RxTx-TimingErrorMargin", trp_rx_tx_timing_error_margin.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TimingErrorMargin ::= ENUMERATED
const char* timing_error_margin_opts::to_string() const
{
  static const char* names[] = {"tc0",
                                "tc2",
                                "tc4",
                                "tc6",
                                "tc8",
                                "tc12",
                                "tc16",
                                "tc20",
                                "tc24",
                                "tc32",
                                "tc40",
                                "tc48",
                                "tc56",
                                "tc64",
                                "tc72",
                                "tc80"};
  return convert_enum_idx(names, 16, value, "timing_error_margin_e");
}
uint8_t timing_error_margin_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 2, 4, 6, 8, 12, 16, 20, 24, 32, 40, 48, 56, 64, 72, 80};
  return map_enum_number(numbers, 16, value, "timing_error_margin_e");
}

// TRP-Tx-TEGInformation ::= SEQUENCE
OCUDUASN_CODE trp_tx_teg_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_tx_teg_id, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(trp_tx_timing_error_margin.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_tx_teg_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_tx_teg_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(trp_tx_timing_error_margin.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_tx_teg_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-Tx-TEGID", trp_tx_teg_id);
  j.write_str("tRP-Tx-TimingErrorMargin", trp_tx_timing_error_margin.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RxTxTEG ::= SEQUENCE
OCUDUASN_CODE rx_tx_teg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(trp_tx_teg_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(trp_rx_tx_teg_info.pack(bref));
  if (trp_tx_teg_info_present) {
    HANDLE_CODE(trp_tx_teg_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rx_tx_teg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(trp_tx_teg_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(trp_rx_tx_teg_info.unpack(bref));
  if (trp_tx_teg_info_present) {
    HANDLE_CODE(trp_tx_teg_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void rx_tx_teg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tRP-RxTx-TEGInformation");
  trp_rx_tx_teg_info.to_json(j);
  if (trp_tx_teg_info_present) {
    j.write_fieldname("tRP-Tx-TEGInformation");
    trp_tx_teg_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRP-Rx-TEGInformation ::= SEQUENCE
OCUDUASN_CODE trp_rx_teg_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_rx_teg_id, (uint8_t)0u, (uint8_t)31u, false, true));
  HANDLE_CODE(trp_rx_timing_error_margin.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_rx_teg_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_rx_teg_id, bref, (uint8_t)0u, (uint8_t)31u, false, true));
  HANDLE_CODE(trp_rx_timing_error_margin.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_rx_teg_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-Rx-TEGID", trp_rx_teg_id);
  j.write_str("tRP-Rx-TimingErrorMargin", trp_rx_timing_error_margin.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RxTEG ::= SEQUENCE
OCUDUASN_CODE rx_teg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(trp_rx_teg_info.pack(bref));
  HANDLE_CODE(trp_tx_teg_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rx_teg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(trp_rx_teg_info.unpack(bref));
  HANDLE_CODE(trp_tx_teg_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void rx_teg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tRP-Rx-TEGInformation");
  trp_rx_teg_info.to_json(j);
  j.write_fieldname("tRP-Tx-TEGInformation");
  trp_tx_teg_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRPTEGInformation ::= CHOICE
void trpteg_info_c::destroy_()
{
  switch (type_) {
    case types::rx_tx_teg:
      c.destroy<rx_tx_teg_s>();
      break;
    case types::rx_teg:
      c.destroy<rx_teg_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void trpteg_info_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::rx_tx_teg:
      c.init<rx_tx_teg_s>();
      break;
    case types::rx_teg:
      c.init<rx_teg_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trpteg_info_c");
  }
}
trpteg_info_c::trpteg_info_c(const trpteg_info_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::rx_tx_teg:
      c.init(other.c.get<rx_tx_teg_s>());
      break;
    case types::rx_teg:
      c.init(other.c.get<rx_teg_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trpteg_info_c");
  }
}
trpteg_info_c& trpteg_info_c::operator=(const trpteg_info_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::rx_tx_teg:
      c.set(other.c.get<rx_tx_teg_s>());
      break;
    case types::rx_teg:
      c.set(other.c.get<rx_teg_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trpteg_info_c");
  }

  return *this;
}
rx_tx_teg_s& trpteg_info_c::set_rx_tx_teg()
{
  set(types::rx_tx_teg);
  return c.get<rx_tx_teg_s>();
}
rx_teg_s& trpteg_info_c::set_rx_teg()
{
  set(types::rx_teg);
  return c.get<rx_teg_s>();
}
protocol_ie_single_container_s<trpteg_info_ext_ies_o>& trpteg_info_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>();
}
void trpteg_info_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::rx_tx_teg:
      j.write_fieldname("rxTx-TEG");
      c.get<rx_tx_teg_s>().to_json(j);
      break;
    case types::rx_teg:
      j.write_fieldname("rx-TEG");
      c.get<rx_teg_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trpteg_info_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trpteg_info_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::rx_tx_teg:
      HANDLE_CODE(c.get<rx_tx_teg_s>().pack(bref));
      break;
    case types::rx_teg:
      HANDLE_CODE(c.get<rx_teg_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trpteg_info_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trpteg_info_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::rx_tx_teg:
      HANDLE_CODE(c.get<rx_tx_teg_s>().unpack(bref));
      break;
    case types::rx_teg:
      HANDLE_CODE(c.get<rx_teg_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trpteg_info_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trpteg_info_c::types_opts::to_string() const
{
  static const char* names[] = {"rxTx-TEG", "rx-TEG", "choice-extension"};
  return convert_enum_idx(names, 3, value, "trpteg_info_c::types");
}

// GNB-RxTxTimeDiff-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t gnb_rx_tx_time_diff_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {77, 85};
  return map_enum_number(names, 2, idx, "id");
}
bool gnb_rx_tx_time_diff_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {77, 85};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_rx_tx_time_diff_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 77:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
gnb_rx_tx_time_diff_ext_ies_o::ext_c gnb_rx_tx_time_diff_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 77:
      ret.set(ext_c::types::extended_add_path_list);
      break;
    case 85:
      ret.set(ext_c::types::trpteg_info);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e gnb_rx_tx_time_diff_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 77:
      return presence_e::optional;
    case 85:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void gnb_rx_tx_time_diff_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::extended_add_path_list:
      c = extended_add_path_list_l{};
      break;
    case types::trpteg_info:
      c = trpteg_info_c{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_ext_ies_o::ext_c");
  }
}
extended_add_path_list_l& gnb_rx_tx_time_diff_ext_ies_o::ext_c::extended_add_path_list()
{
  assert_choice_type(types::extended_add_path_list, type_, "Extension");
  return c.get<extended_add_path_list_l>();
}
trpteg_info_c& gnb_rx_tx_time_diff_ext_ies_o::ext_c::trpteg_info()
{
  assert_choice_type(types::trpteg_info, type_, "Extension");
  return c.get<trpteg_info_c>();
}
const extended_add_path_list_l& gnb_rx_tx_time_diff_ext_ies_o::ext_c::extended_add_path_list() const
{
  assert_choice_type(types::extended_add_path_list, type_, "Extension");
  return c.get<extended_add_path_list_l>();
}
const trpteg_info_c& gnb_rx_tx_time_diff_ext_ies_o::ext_c::trpteg_info() const
{
  assert_choice_type(types::trpteg_info, type_, "Extension");
  return c.get<trpteg_info_c>();
}
void gnb_rx_tx_time_diff_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::extended_add_path_list:
      j.start_array("");
      for (const auto& e1 : c.get<extended_add_path_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::trpteg_info:
      c.get<trpteg_info_c>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE gnb_rx_tx_time_diff_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::extended_add_path_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<extended_add_path_list_l>(), 1, 8, true));
      break;
    case types::trpteg_info:
      HANDLE_CODE(c.get<trpteg_info_c>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_rx_tx_time_diff_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::extended_add_path_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<extended_add_path_list_l>(), bref, 1, 8, true));
      break;
    case types::trpteg_info:
      HANDLE_CODE(c.get<trpteg_info_c>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "gnb_rx_tx_time_diff_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* gnb_rx_tx_time_diff_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"ExtendedAdditionalPathList", "TRPTEGInformation"};
  return convert_enum_idx(names, 2, value, "gnb_rx_tx_time_diff_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<gnb_rx_tx_time_diff_ext_ies_o>;

OCUDUASN_CODE gnb_rx_tx_time_diff_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += extended_add_path_list_present ? 1 : 0;
  nof_ies += trpteg_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (extended_add_path_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, extended_add_path_list, 1, 8, true));
  }
  if (trpteg_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)85, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(trpteg_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_rx_tx_time_diff_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_rx_tx_time_diff_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 77: {
        extended_add_path_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(extended_add_path_list, bref, 1, 8, true));
        break;
      }
      case 85: {
        trpteg_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(trpteg_info.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void gnb_rx_tx_time_diff_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (extended_add_path_list_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : extended_add_path_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (trpteg_info_present) {
    j.start_obj();
    j.write_int("id", 85);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    trpteg_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNB-RxTxTimeDiff ::= SEQUENCE
OCUDUASN_CODE gnb_rx_tx_time_diff_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(add_path_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(rx_tx_time_diff.pack(bref));
  if (add_path_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, add_path_list, 1, 2, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_rx_tx_time_diff_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool add_path_list_present;
  HANDLE_CODE(bref.unpack(add_path_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(rx_tx_time_diff.unpack(bref));
  if (add_path_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(add_path_list, bref, 1, 2, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void gnb_rx_tx_time_diff_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("rxTxTimeDiff");
  rx_tx_time_diff.to_json(j);
  if (add_path_list.size() > 0) {
    j.start_array("additionalPathList");
    for (const auto& e1 : add_path_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// LCS-to-GCS-TranslationItem ::= SEQUENCE
OCUDUASN_CODE lcs_to_gcs_translation_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(alpha_fine_present, 1));
  HANDLE_CODE(bref.pack(beta_fine_present, 1));
  HANDLE_CODE(bref.pack(gamma_fine_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, alpha, (uint16_t)0u, (uint16_t)359u, false, true));
  if (alpha_fine_present) {
    HANDLE_CODE(pack_integer(bref, alpha_fine, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  HANDLE_CODE(pack_integer(bref, beta, (uint16_t)0u, (uint16_t)359u, false, true));
  if (beta_fine_present) {
    HANDLE_CODE(pack_integer(bref, beta_fine, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  HANDLE_CODE(pack_integer(bref, gamma, (uint16_t)0u, (uint16_t)359u, false, true));
  if (gamma_fine_present) {
    HANDLE_CODE(pack_integer(bref, gamma_fine, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE lcs_to_gcs_translation_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(alpha_fine_present, 1));
  HANDLE_CODE(bref.unpack(beta_fine_present, 1));
  HANDLE_CODE(bref.unpack(gamma_fine_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(alpha, bref, (uint16_t)0u, (uint16_t)359u, false, true));
  if (alpha_fine_present) {
    HANDLE_CODE(unpack_integer(alpha_fine, bref, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  HANDLE_CODE(unpack_integer(beta, bref, (uint16_t)0u, (uint16_t)359u, false, true));
  if (beta_fine_present) {
    HANDLE_CODE(unpack_integer(beta_fine, bref, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  HANDLE_CODE(unpack_integer(gamma, bref, (uint16_t)0u, (uint16_t)359u, false, true));
  if (gamma_fine_present) {
    HANDLE_CODE(unpack_integer(gamma_fine, bref, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void lcs_to_gcs_translation_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("alpha", alpha);
  if (alpha_fine_present) {
    j.write_int("alphaFine", alpha_fine);
  }
  j.write_int("beta", beta);
  if (beta_fine_present) {
    j.write_int("betaFine", beta_fine);
  }
  j.write_int("gamma", gamma);
  if (gamma_fine_present) {
    j.write_int("gammaFine", gamma_fine);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// LoS-NLoSIndicatorHard ::= ENUMERATED
const char* lo_s_n_lo_si_ndicator_hard_opts::to_string() const
{
  static const char* names[] = {"nlos", "los"};
  return convert_enum_idx(names, 2, value, "lo_s_n_lo_si_ndicator_hard_e");
}

// LoS-NLoSInformation ::= CHOICE
void lo_s_n_lo_si_nformation_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void lo_s_n_lo_si_nformation_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::lo_s_n_lo_si_ndicator_soft:
      break;
    case types::lo_s_n_lo_si_ndicator_hard:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "lo_s_n_lo_si_nformation_c");
  }
}
lo_s_n_lo_si_nformation_c::lo_s_n_lo_si_nformation_c(const lo_s_n_lo_si_nformation_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::lo_s_n_lo_si_ndicator_soft:
      c.init(other.c.get<uint8_t>());
      break;
    case types::lo_s_n_lo_si_ndicator_hard:
      c.init(other.c.get<lo_s_n_lo_si_ndicator_hard_e>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "lo_s_n_lo_si_nformation_c");
  }
}
lo_s_n_lo_si_nformation_c& lo_s_n_lo_si_nformation_c::operator=(const lo_s_n_lo_si_nformation_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::lo_s_n_lo_si_ndicator_soft:
      c.set(other.c.get<uint8_t>());
      break;
    case types::lo_s_n_lo_si_ndicator_hard:
      c.set(other.c.get<lo_s_n_lo_si_ndicator_hard_e>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "lo_s_n_lo_si_nformation_c");
  }

  return *this;
}
uint8_t& lo_s_n_lo_si_nformation_c::set_lo_s_n_lo_si_ndicator_soft()
{
  set(types::lo_s_n_lo_si_ndicator_soft);
  return c.get<uint8_t>();
}
lo_s_n_lo_si_ndicator_hard_e& lo_s_n_lo_si_nformation_c::set_lo_s_n_lo_si_ndicator_hard()
{
  set(types::lo_s_n_lo_si_ndicator_hard);
  return c.get<lo_s_n_lo_si_ndicator_hard_e>();
}
protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>& lo_s_n_lo_si_nformation_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>();
}
void lo_s_n_lo_si_nformation_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lo_s_n_lo_si_ndicator_soft:
      j.write_int("loS-NLoSIndicatorSoft", c.get<uint8_t>());
      break;
    case types::lo_s_n_lo_si_ndicator_hard:
      j.write_str("loS-NLoSIndicatorHard", c.get<lo_s_n_lo_si_ndicator_hard_e>().to_string());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "lo_s_n_lo_si_nformation_c");
  }
  j.end_obj();
}
OCUDUASN_CODE lo_s_n_lo_si_nformation_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::lo_s_n_lo_si_ndicator_soft:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)10u, false, true));
      break;
    case types::lo_s_n_lo_si_ndicator_hard:
      HANDLE_CODE(c.get<lo_s_n_lo_si_ndicator_hard_e>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "lo_s_n_lo_si_nformation_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE lo_s_n_lo_si_nformation_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::lo_s_n_lo_si_ndicator_soft:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)10u, false, true));
      break;
    case types::lo_s_n_lo_si_ndicator_hard:
      HANDLE_CODE(c.get<lo_s_n_lo_si_ndicator_hard_e>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "lo_s_n_lo_si_nformation_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* lo_s_n_lo_si_nformation_c::types_opts::to_string() const
{
  static const char* names[] = {"loS-NLoSIndicatorSoft", "loS-NLoSIndicatorHard", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "lo_s_n_lo_si_nformation_c::types");
}

// MeasBasedOnAggregatedResources ::= ENUMERATED
const char* meas_based_on_aggr_res_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "meas_based_on_aggr_res_e");
}

// MeasuredFrequencyHops ::= ENUMERATED
const char* measured_freq_hops_opts::to_string() const
{
  static const char* names[] = {"singleHop", "multiHop"};
  return convert_enum_idx(names, 2, value, "measured_freq_hops_e");
}

// MeasurementAmount ::= ENUMERATED
const char* meas_amount_opts::to_string() const
{
  static const char* names[] = {"ma0", "ma1", "ma2", "ma4", "ma8", "ma16", "ma32", "ma64"};
  return convert_enum_idx(names, 8, value, "meas_amount_e");
}
uint8_t meas_amount_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1, 2, 4, 8, 16, 32, 64};
  return map_enum_number(numbers, 8, value, "meas_amount_e");
}

// MeasurementBeamInfo ::= SEQUENCE
OCUDUASN_CODE meas_beam_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(prs_res_id_present, 1));
  HANDLE_CODE(bref.pack(prs_res_set_id_present, 1));
  HANDLE_CODE(bref.pack(ssb_idx_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (prs_res_id_present) {
    HANDLE_CODE(pack_integer(bref, prs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (prs_res_set_id_present) {
    HANDLE_CODE(pack_integer(bref, prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  }
  if (ssb_idx_present) {
    HANDLE_CODE(pack_integer(bref, ssb_idx, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_beam_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(prs_res_id_present, 1));
  HANDLE_CODE(bref.unpack(prs_res_set_id_present, 1));
  HANDLE_CODE(bref.unpack(ssb_idx_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (prs_res_id_present) {
    HANDLE_CODE(unpack_integer(prs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (prs_res_set_id_present) {
    HANDLE_CODE(unpack_integer(prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  }
  if (ssb_idx_present) {
    HANDLE_CODE(unpack_integer(ssb_idx, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void meas_beam_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (prs_res_id_present) {
    j.write_int("pRS-Resource-ID", prs_res_id);
  }
  if (prs_res_set_id_present) {
    j.write_int("pRS-Resource-Set-ID", prs_res_set_id);
  }
  if (ssb_idx_present) {
    j.write_int("sSB-Index", ssb_idx);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MeasurementBeamInfoRequest ::= ENUMERATED
const char* meas_beam_info_request_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "meas_beam_info_request_e");
}

// MeasurementPeriodicity ::= ENUMERATED
const char* meas_periodicity_opts::to_string() const
{
  static const char* names[] = {"ms120",
                                "ms240",
                                "ms480",
                                "ms640",
                                "ms1024",
                                "ms2048",
                                "ms5120",
                                "ms10240",
                                "min1",
                                "min6",
                                "min12",
                                "min30",
                                "min60",
                                "ms20480",
                                "ms40960",
                                "extended"};
  return convert_enum_idx(names, 16, value, "meas_periodicity_e");
}
uint16_t meas_periodicity_opts::to_number() const
{
  static const uint16_t numbers[] = {120, 240, 480, 640, 1024, 2048, 5120, 10240, 1, 6, 12, 30, 60, 20480, 40960};
  return map_enum_number(numbers, 15, value, "meas_periodicity_e");
}

// MeasurementPeriodicityExtended ::= ENUMERATED
const char* meas_periodicity_extended_opts::to_string() const
{
  static const char* names[] = {
      "ms160", "ms320", "ms1280", "ms2560", "ms61440", "ms81920", "ms368640", "ms737280", "ms1843200"};
  return convert_enum_idx(names, 9, value, "meas_periodicity_extended_e");
}
uint32_t meas_periodicity_extended_opts::to_number() const
{
  static const uint32_t numbers[] = {160, 320, 1280, 2560, 61440, 81920, 368640, 737280, 1843200};
  return map_enum_number(numbers, 9, value, "meas_periodicity_extended_e");
}

// MeasurementPeriodicityNR-AoA ::= ENUMERATED
const char* meas_periodicity_nr_ao_a_opts::to_string() const
{
  static const char* names[] = {"ms160",
                                "ms320",
                                "ms640",
                                "ms1280",
                                "ms2560",
                                "ms5120",
                                "ms10240",
                                "ms20480",
                                "ms40960",
                                "ms61440",
                                "ms81920",
                                "ms368640",
                                "ms737280",
                                "ms1843200"};
  return convert_enum_idx(names, 14, value, "meas_periodicity_nr_ao_a_e");
}
uint32_t meas_periodicity_nr_ao_a_opts::to_number() const
{
  static const uint32_t numbers[] = {
      160, 320, 640, 1280, 2560, 5120, 10240, 20480, 40960, 61440, 81920, 368640, 737280, 1843200};
  return map_enum_number(numbers, 14, value, "meas_periodicity_nr_ao_a_e");
}

// MeasurementQuantitiesValue ::= ENUMERATED
const char* meas_quantities_value_opts::to_string() const
{
  static const char* names[] = {"cell-ID",
                                "angleOfArrival",
                                "timingAdvanceType1",
                                "timingAdvanceType2",
                                "rSRP",
                                "rSRQ",
                                "sS-RSRP",
                                "sS-RSRQ",
                                "cSI-RSRP",
                                "cSI-RSRQ",
                                "angleOfArrivalNR",
                                "timingAdvanceNR",
                                "uE-Rx-Tx-Time-Diff"};
  return convert_enum_idx(names, 13, value, "meas_quantities_value_e");
}
uint8_t meas_quantities_value_opts::to_number() const
{
  switch (value) {
    case timing_advance_type1:
      return 1;
    case timing_advance_type2:
      return 2;
    default:
      invalid_enum_number(value, "meas_quantities_value_e");
  }
  return 0;
}

// MeasurementQuantities-Item ::= SEQUENCE
OCUDUASN_CODE meas_quantities_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(meas_quantities_value.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_quantities_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(meas_quantities_value.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void meas_quantities_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("measurementQuantitiesValue", meas_quantities_value.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MeasurementQuantities-ItemIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_quantities_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {11};
  return map_enum_number(names, 1, idx, "id");
}
bool meas_quantities_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 11 == id;
}
crit_e meas_quantities_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 11) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
meas_quantities_item_ies_o::value_c meas_quantities_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 11) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_quantities_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 11) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void meas_quantities_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE meas_quantities_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_quantities_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* meas_quantities_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"MeasurementQuantities-Item"};
  return convert_enum_idx(names, 1, value, "meas_quantities_item_ies_o::value_c::types");
}

// MeasurementTimeOccasion ::= ENUMERATED
const char* meas_time_occasion_opts::to_string() const
{
  static const char* names[] = {"o1", "o4"};
  return convert_enum_idx(names, 2, value, "meas_time_occasion_e");
}
uint8_t meas_time_occasion_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 4};
  return map_enum_number(numbers, 2, value, "meas_time_occasion_e");
}

// PRSAngleItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t prs_angle_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {65};
  return map_enum_number(names, 1, idx, "id");
}
bool prs_angle_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 65 == id;
}
crit_e prs_angle_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 65) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
prs_angle_item_ext_ies_o::ext_c prs_angle_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 65) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e prs_angle_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 65) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void prs_angle_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_int(c);
}
OCUDUASN_CODE prs_angle_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_integer(bref, c, (uint8_t)0u, (uint8_t)63u, false, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_angle_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_integer(c, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  return OCUDUASN_SUCCESS;
}

const char* prs_angle_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..63)"};
  return convert_enum_idx(names, 1, value, "prs_angle_item_ext_ies_o::ext_c::types");
}
uint8_t prs_angle_item_ext_ies_o::ext_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "prs_angle_item_ext_ies_o::ext_c::types");
}

// PRSAngleItem ::= SEQUENCE
OCUDUASN_CODE prs_angle_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(nr_prs_azimuth_fine_present, 1));
  HANDLE_CODE(bref.pack(nr_prs_elevation_present, 1));
  HANDLE_CODE(bref.pack(nr_prs_elevation_fine_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, nr_prs_azimuth, (uint16_t)0u, (uint16_t)359u, false, true));
  if (nr_prs_azimuth_fine_present) {
    HANDLE_CODE(pack_integer(bref, nr_prs_azimuth_fine, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  if (nr_prs_elevation_present) {
    HANDLE_CODE(pack_integer(bref, nr_prs_elevation, (uint8_t)0u, (uint8_t)180u, false, true));
  }
  if (nr_prs_elevation_fine_present) {
    HANDLE_CODE(pack_integer(bref, nr_prs_elevation_fine, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_angle_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(nr_prs_azimuth_fine_present, 1));
  HANDLE_CODE(bref.unpack(nr_prs_elevation_present, 1));
  HANDLE_CODE(bref.unpack(nr_prs_elevation_fine_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(nr_prs_azimuth, bref, (uint16_t)0u, (uint16_t)359u, false, true));
  if (nr_prs_azimuth_fine_present) {
    HANDLE_CODE(unpack_integer(nr_prs_azimuth_fine, bref, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  if (nr_prs_elevation_present) {
    HANDLE_CODE(unpack_integer(nr_prs_elevation, bref, (uint8_t)0u, (uint8_t)180u, false, true));
  }
  if (nr_prs_elevation_fine_present) {
    HANDLE_CODE(unpack_integer(nr_prs_elevation_fine, bref, (uint8_t)0u, (uint8_t)9u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_angle_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("nRPRSAzimuth", nr_prs_azimuth);
  if (nr_prs_azimuth_fine_present) {
    j.write_int("nRPRSAzimuthFine", nr_prs_azimuth_fine);
  }
  if (nr_prs_elevation_present) {
    j.write_int("nRPRSElevation", nr_prs_elevation);
  }
  if (nr_prs_elevation_fine_present) {
    j.write_int("nRPRSElevationFine", nr_prs_elevation_fine);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NR-PRS-Beam-InformationItem ::= SEQUENCE
OCUDUASN_CODE nr_prs_beam_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pr_sres_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, prs_angle, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE nr_prs_beam_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pr_sres_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(prs_angle, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void nr_prs_beam_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pRSresourceSetID", pr_sres_set_id);
  j.start_array("pRSAngle");
  for (const auto& e1 : prs_angle) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NR-PRS-Beam-Information ::= SEQUENCE
OCUDUASN_CODE nr_prs_beam_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(lcs_to_gcs_translation_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, nr_prs_beam_info_list, 1, 2, true));
  if (lcs_to_gcs_translation_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, lcs_to_gcs_translation_list, 1, 3, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE nr_prs_beam_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool lcs_to_gcs_translation_list_present;
  HANDLE_CODE(bref.unpack(lcs_to_gcs_translation_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(nr_prs_beam_info_list, bref, 1, 2, true));
  if (lcs_to_gcs_translation_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(lcs_to_gcs_translation_list, bref, 1, 3, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void nr_prs_beam_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("nR-PRS-Beam-InformationList");
  for (const auto& e1 : nr_prs_beam_info_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (lcs_to_gcs_translation_list.size() > 0) {
    j.start_array("lCS-to-GCS-TranslationList");
    for (const auto& e1 : lcs_to_gcs_translation_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NumberOfAntennaPorts-EUTRA ::= ENUMERATED
const char* nof_ant_ports_eutra_opts::to_string() const
{
  static const char* names[] = {"n1-or-n2", "n4"};
  return convert_enum_idx(names, 2, value, "nof_ant_ports_eutra_e");
}
uint8_t nof_ant_ports_eutra_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 4};
  return map_enum_number(numbers, 2, value, "nof_ant_ports_eutra_e");
}

// NumberOfDlFrames-EUTRA ::= ENUMERATED
const char* nof_dl_frames_eutra_opts::to_string() const
{
  static const char* names[] = {"sf1", "sf2", "sf4", "sf6"};
  return convert_enum_idx(names, 4, value, "nof_dl_frames_eutra_e");
}
uint8_t nof_dl_frames_eutra_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 6};
  return map_enum_number(numbers, 4, value, "nof_dl_frames_eutra_e");
}

// NumberOfFrequencyHoppingBands ::= ENUMERATED
const char* nof_freq_hop_bands_opts::to_string() const
{
  static const char* names[] = {"twobands", "fourbands"};
  return convert_enum_idx(names, 2, value, "nof_freq_hop_bands_e");
}
uint8_t nof_freq_hop_bands_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4};
  return map_enum_number(numbers, 2, value, "nof_freq_hop_bands_e");
}

// NumberOfTRPRxTEG ::= ENUMERATED
const char* nof_trp_rx_teg_opts::to_string() const
{
  static const char* names[] = {"two", "three", "four", "six", "eight"};
  return convert_enum_idx(names, 5, value, "nof_trp_rx_teg_e");
}
uint8_t nof_trp_rx_teg_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 3, 4, 6, 8};
  return map_enum_number(numbers, 5, value, "nof_trp_rx_teg_e");
}

// NumberOfTRPRxTxTEG ::= ENUMERATED
const char* nof_trp_rx_tx_teg_opts::to_string() const
{
  static const char* names[] = {"two", "three", "four", "six", "eight"};
  return convert_enum_idx(names, 5, value, "nof_trp_rx_tx_teg_e");
}
uint8_t nof_trp_rx_tx_teg_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 3, 4, 6, 8};
  return map_enum_number(numbers, 5, value, "nof_trp_rx_tx_teg_e");
}

// OTDOA-Information-Item ::= ENUMERATED
const char* otdoa_info_item_opts::to_string() const
{
  static const char* names[] = {"pci",
                                "cGI",
                                "tac",
                                "earfcn",
                                "prsBandwidth",
                                "prsConfigIndex",
                                "cpLength",
                                "noDlFrames",
                                "noAntennaPorts",
                                "sFNInitTime",
                                "nG-RANAccessPointPosition",
                                "prsmutingconfiguration",
                                "prsid",
                                "tpid",
                                "tpType",
                                "crsCPlength",
                                "dlBandwidth",
                                "multipleprsConfigurationsperCell",
                                "prsOccasionGroup",
                                "prsFrequencyHoppingConfiguration",
                                "tddConfig"};
  return convert_enum_idx(names, 21, value, "otdoa_info_item_e");
}

// PRS-Bandwidth-EUTRA ::= ENUMERATED
const char* prs_bw_eutra_opts::to_string() const
{
  static const char* names[] = {"bw6", "bw15", "bw25", "bw50", "bw75", "bw100"};
  return convert_enum_idx(names, 6, value, "prs_bw_eutra_e");
}
uint8_t prs_bw_eutra_opts::to_number() const
{
  static const uint8_t numbers[] = {6, 15, 25, 50, 75, 100};
  return map_enum_number(numbers, 6, value, "prs_bw_eutra_e");
}

// PRSMutingConfiguration-EUTRA ::= CHOICE
void prs_muting_cfg_eutra_c::destroy_()
{
  switch (type_) {
    case types::two:
      c.destroy<fixed_bitstring<2, false, true>>();
      break;
    case types::four:
      c.destroy<fixed_bitstring<4, false, true>>();
      break;
    case types::eight:
      c.destroy<fixed_bitstring<8, false, true>>();
      break;
    case types::sixteen:
      c.destroy<fixed_bitstring<16, false, true>>();
      break;
    case types::thirty_two:
      c.destroy<fixed_bitstring<32, false, true>>();
      break;
    case types::sixty_four:
      c.destroy<fixed_bitstring<64, false, true>>();
      break;
    case types::one_hundred_and_twenty_eight:
      c.destroy<fixed_bitstring<128, false, true>>();
      break;
    case types::two_hundred_and_fifty_six:
      c.destroy<fixed_bitstring<256, false, true>>();
      break;
    case types::five_hundred_and_twelve:
      c.destroy<fixed_bitstring<512, false, true>>();
      break;
    case types::one_thousand_and_twenty_four:
      c.destroy<fixed_bitstring<1024, false, true>>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>();
      break;
    default:
      break;
  }
}
void prs_muting_cfg_eutra_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::two:
      c.init<fixed_bitstring<2, false, true>>();
      break;
    case types::four:
      c.init<fixed_bitstring<4, false, true>>();
      break;
    case types::eight:
      c.init<fixed_bitstring<8, false, true>>();
      break;
    case types::sixteen:
      c.init<fixed_bitstring<16, false, true>>();
      break;
    case types::thirty_two:
      c.init<fixed_bitstring<32, false, true>>();
      break;
    case types::sixty_four:
      c.init<fixed_bitstring<64, false, true>>();
      break;
    case types::one_hundred_and_twenty_eight:
      c.init<fixed_bitstring<128, false, true>>();
      break;
    case types::two_hundred_and_fifty_six:
      c.init<fixed_bitstring<256, false, true>>();
      break;
    case types::five_hundred_and_twelve:
      c.init<fixed_bitstring<512, false, true>>();
      break;
    case types::one_thousand_and_twenty_four:
      c.init<fixed_bitstring<1024, false, true>>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_muting_cfg_eutra_c");
  }
}
prs_muting_cfg_eutra_c::prs_muting_cfg_eutra_c(const prs_muting_cfg_eutra_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::two:
      c.init(other.c.get<fixed_bitstring<2, false, true>>());
      break;
    case types::four:
      c.init(other.c.get<fixed_bitstring<4, false, true>>());
      break;
    case types::eight:
      c.init(other.c.get<fixed_bitstring<8, false, true>>());
      break;
    case types::sixteen:
      c.init(other.c.get<fixed_bitstring<16, false, true>>());
      break;
    case types::thirty_two:
      c.init(other.c.get<fixed_bitstring<32, false, true>>());
      break;
    case types::sixty_four:
      c.init(other.c.get<fixed_bitstring<64, false, true>>());
      break;
    case types::one_hundred_and_twenty_eight:
      c.init(other.c.get<fixed_bitstring<128, false, true>>());
      break;
    case types::two_hundred_and_fifty_six:
      c.init(other.c.get<fixed_bitstring<256, false, true>>());
      break;
    case types::five_hundred_and_twelve:
      c.init(other.c.get<fixed_bitstring<512, false, true>>());
      break;
    case types::one_thousand_and_twenty_four:
      c.init(other.c.get<fixed_bitstring<1024, false, true>>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_muting_cfg_eutra_c");
  }
}
prs_muting_cfg_eutra_c& prs_muting_cfg_eutra_c::operator=(const prs_muting_cfg_eutra_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::two:
      c.set(other.c.get<fixed_bitstring<2, false, true>>());
      break;
    case types::four:
      c.set(other.c.get<fixed_bitstring<4, false, true>>());
      break;
    case types::eight:
      c.set(other.c.get<fixed_bitstring<8, false, true>>());
      break;
    case types::sixteen:
      c.set(other.c.get<fixed_bitstring<16, false, true>>());
      break;
    case types::thirty_two:
      c.set(other.c.get<fixed_bitstring<32, false, true>>());
      break;
    case types::sixty_four:
      c.set(other.c.get<fixed_bitstring<64, false, true>>());
      break;
    case types::one_hundred_and_twenty_eight:
      c.set(other.c.get<fixed_bitstring<128, false, true>>());
      break;
    case types::two_hundred_and_fifty_six:
      c.set(other.c.get<fixed_bitstring<256, false, true>>());
      break;
    case types::five_hundred_and_twelve:
      c.set(other.c.get<fixed_bitstring<512, false, true>>());
      break;
    case types::one_thousand_and_twenty_four:
      c.set(other.c.get<fixed_bitstring<1024, false, true>>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_muting_cfg_eutra_c");
  }

  return *this;
}
fixed_bitstring<2, false, true>& prs_muting_cfg_eutra_c::set_two()
{
  set(types::two);
  return c.get<fixed_bitstring<2, false, true>>();
}
fixed_bitstring<4, false, true>& prs_muting_cfg_eutra_c::set_four()
{
  set(types::four);
  return c.get<fixed_bitstring<4, false, true>>();
}
fixed_bitstring<8, false, true>& prs_muting_cfg_eutra_c::set_eight()
{
  set(types::eight);
  return c.get<fixed_bitstring<8, false, true>>();
}
fixed_bitstring<16, false, true>& prs_muting_cfg_eutra_c::set_sixteen()
{
  set(types::sixteen);
  return c.get<fixed_bitstring<16, false, true>>();
}
fixed_bitstring<32, false, true>& prs_muting_cfg_eutra_c::set_thirty_two()
{
  set(types::thirty_two);
  return c.get<fixed_bitstring<32, false, true>>();
}
fixed_bitstring<64, false, true>& prs_muting_cfg_eutra_c::set_sixty_four()
{
  set(types::sixty_four);
  return c.get<fixed_bitstring<64, false, true>>();
}
fixed_bitstring<128, false, true>& prs_muting_cfg_eutra_c::set_one_hundred_and_twenty_eight()
{
  set(types::one_hundred_and_twenty_eight);
  return c.get<fixed_bitstring<128, false, true>>();
}
fixed_bitstring<256, false, true>& prs_muting_cfg_eutra_c::set_two_hundred_and_fifty_six()
{
  set(types::two_hundred_and_fifty_six);
  return c.get<fixed_bitstring<256, false, true>>();
}
fixed_bitstring<512, false, true>& prs_muting_cfg_eutra_c::set_five_hundred_and_twelve()
{
  set(types::five_hundred_and_twelve);
  return c.get<fixed_bitstring<512, false, true>>();
}
fixed_bitstring<1024, false, true>& prs_muting_cfg_eutra_c::set_one_thousand_and_twenty_four()
{
  set(types::one_thousand_and_twenty_four);
  return c.get<fixed_bitstring<1024, false, true>>();
}
protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>& prs_muting_cfg_eutra_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>();
}
void prs_muting_cfg_eutra_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::two:
      j.write_fieldname("two");
      c.get<fixed_bitstring<2, false, true>>().to_json(j);
      break;
    case types::four:
      j.write_fieldname("four");
      c.get<fixed_bitstring<4, false, true>>().to_json(j);
      break;
    case types::eight:
      j.write_fieldname("eight");
      c.get<fixed_bitstring<8, false, true>>().to_json(j);
      break;
    case types::sixteen:
      j.write_fieldname("sixteen");
      c.get<fixed_bitstring<16, false, true>>().to_json(j);
      break;
    case types::thirty_two:
      j.write_fieldname("thirty-two");
      c.get<fixed_bitstring<32, false, true>>().to_json(j);
      break;
    case types::sixty_four:
      j.write_fieldname("sixty-four");
      c.get<fixed_bitstring<64, false, true>>().to_json(j);
      break;
    case types::one_hundred_and_twenty_eight:
      j.write_fieldname("one-hundred-and-twenty-eight");
      c.get<fixed_bitstring<128, false, true>>().to_json(j);
      break;
    case types::two_hundred_and_fifty_six:
      j.write_fieldname("two-hundred-and-fifty-six");
      c.get<fixed_bitstring<256, false, true>>().to_json(j);
      break;
    case types::five_hundred_and_twelve:
      j.write_fieldname("five-hundred-and-twelve");
      c.get<fixed_bitstring<512, false, true>>().to_json(j);
      break;
    case types::one_thousand_and_twenty_four:
      j.write_fieldname("one-thousand-and-twenty-four");
      c.get<fixed_bitstring<1024, false, true>>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "prs_muting_cfg_eutra_c");
  }
  j.end_obj();
}
OCUDUASN_CODE prs_muting_cfg_eutra_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::two:
      HANDLE_CODE((c.get<fixed_bitstring<2, false, true>>().pack(bref)));
      break;
    case types::four:
      HANDLE_CODE((c.get<fixed_bitstring<4, false, true>>().pack(bref)));
      break;
    case types::eight:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().pack(bref)));
      break;
    case types::sixteen:
      HANDLE_CODE((c.get<fixed_bitstring<16, false, true>>().pack(bref)));
      break;
    case types::thirty_two:
      HANDLE_CODE((c.get<fixed_bitstring<32, false, true>>().pack(bref)));
      break;
    case types::sixty_four:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    case types::one_hundred_and_twenty_eight:
      HANDLE_CODE((c.get<fixed_bitstring<128, false, true>>().pack(bref)));
      break;
    case types::two_hundred_and_fifty_six:
      HANDLE_CODE((c.get<fixed_bitstring<256, false, true>>().pack(bref)));
      break;
    case types::five_hundred_and_twelve:
      HANDLE_CODE((c.get<fixed_bitstring<512, false, true>>().pack(bref)));
      break;
    case types::one_thousand_and_twenty_four:
      HANDLE_CODE((c.get<fixed_bitstring<1024, false, true>>().pack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_muting_cfg_eutra_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_muting_cfg_eutra_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::two:
      HANDLE_CODE((c.get<fixed_bitstring<2, false, true>>().unpack(bref)));
      break;
    case types::four:
      HANDLE_CODE((c.get<fixed_bitstring<4, false, true>>().unpack(bref)));
      break;
    case types::eight:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().unpack(bref)));
      break;
    case types::sixteen:
      HANDLE_CODE((c.get<fixed_bitstring<16, false, true>>().unpack(bref)));
      break;
    case types::thirty_two:
      HANDLE_CODE((c.get<fixed_bitstring<32, false, true>>().unpack(bref)));
      break;
    case types::sixty_four:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    case types::one_hundred_and_twenty_eight:
      HANDLE_CODE((c.get<fixed_bitstring<128, false, true>>().unpack(bref)));
      break;
    case types::two_hundred_and_fifty_six:
      HANDLE_CODE((c.get<fixed_bitstring<256, false, true>>().unpack(bref)));
      break;
    case types::five_hundred_and_twelve:
      HANDLE_CODE((c.get<fixed_bitstring<512, false, true>>().unpack(bref)));
      break;
    case types::one_thousand_and_twenty_four:
      HANDLE_CODE((c.get<fixed_bitstring<1024, false, true>>().unpack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_muting_cfg_eutra_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* prs_muting_cfg_eutra_c::types_opts::to_string() const
{
  static const char* names[] = {"two",
                                "four",
                                "eight",
                                "sixteen",
                                "thirty-two",
                                "sixty-four",
                                "one-hundred-and-twenty-eight",
                                "two-hundred-and-fifty-six",
                                "five-hundred-and-twelve",
                                "one-thousand-and-twenty-four",
                                "choice-Extension"};
  return convert_enum_idx(names, 11, value, "prs_muting_cfg_eutra_c::types");
}

// TP-Type-EUTRA ::= ENUMERATED
const char* tp_type_eutra_opts::to_string() const
{
  static const char* names[] = {"prs-only-tp"};
  return convert_enum_idx(names, 1, value, "tp_type_eutra_e");
}

// PRSOccasionGroup-EUTRA ::= ENUMERATED
const char* prs_occasion_group_eutra_opts::to_string() const
{
  static const char* names[] = {"og2", "og4", "og8", "og16", "og32", "og64", "og128"};
  return convert_enum_idx(names, 7, value, "prs_occasion_group_eutra_e");
}
uint8_t prs_occasion_group_eutra_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 8, 16, 32, 64, 128};
  return map_enum_number(numbers, 7, value, "prs_occasion_group_eutra_e");
}

// PRSFrequencyHoppingConfiguration-EUTRA ::= SEQUENCE
OCUDUASN_CODE prs_freq_hop_cfg_eutra_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(no_of_freq_hop_bands.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, band_positions, 1, 7, integer_packer<uint8_t>(0, 15, true, true), true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_freq_hop_cfg_eutra_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(no_of_freq_hop_bands.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(band_positions, bref, 1, 7, integer_packer<uint8_t>(0, 15, true, true), true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_freq_hop_cfg_eutra_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("noOfFreqHoppingBands", no_of_freq_hop_bands.to_string());
  j.start_array("bandPositions");
  for (const auto& e1 : band_positions) {
    j.write_int(e1);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TDD-Config-EUTRA-Item ::= SEQUENCE
OCUDUASN_CODE tdd_cfg_eutra_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(sf_assign.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tdd_cfg_eutra_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(sf_assign.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void tdd_cfg_eutra_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("subframeAssignment", sf_assign.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* tdd_cfg_eutra_item_s::sf_assign_opts::to_string() const
{
  static const char* names[] = {"sa0", "sa1", "sa2", "sa3", "sa4", "sa5", "sa6"};
  return convert_enum_idx(names, 7, value, "tdd_cfg_eutra_item_s::sf_assign_e_");
}
uint8_t tdd_cfg_eutra_item_s::sf_assign_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1, 2, 3, 4, 5, 6};
  return map_enum_number(numbers, 7, value, "tdd_cfg_eutra_item_s::sf_assign_e_");
}

// OTDOACell-Information-Item-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t otdoa_cell_info_item_ext_ie_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {22, 58, 59};
  return map_enum_number(names, 3, idx, "id");
}
bool otdoa_cell_info_item_ext_ie_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {22, 58, 59};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e otdoa_cell_info_item_ext_ie_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 22:
      return crit_e::ignore;
    case 58:
      return crit_e::ignore;
    case 59:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
otdoa_cell_info_item_ext_ie_o::value_c otdoa_cell_info_item_ext_ie_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 22:
      ret.set(value_c::types::tdd_cfg_eutra_item);
      break;
    case 58:
      ret.set(value_c::types::cgi_nr);
      break;
    case 59:
      ret.set(value_c::types::sfn_initisation_time_nr);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e otdoa_cell_info_item_ext_ie_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 22:
      return presence_e::mandatory;
    case 58:
      return presence_e::mandatory;
    case 59:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void otdoa_cell_info_item_ext_ie_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::tdd_cfg_eutra_item:
      c = tdd_cfg_eutra_item_s{};
      break;
    case types::cgi_nr:
      c = cgi_nr_s{};
      break;
    case types::sfn_initisation_time_nr:
      c = fixed_bitstring<64, false, true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_ext_ie_o::value_c");
  }
}
tdd_cfg_eutra_item_s& otdoa_cell_info_item_ext_ie_o::value_c::tdd_cfg_eutra_item()
{
  assert_choice_type(types::tdd_cfg_eutra_item, type_, "Value");
  return c.get<tdd_cfg_eutra_item_s>();
}
cgi_nr_s& otdoa_cell_info_item_ext_ie_o::value_c::cgi_nr()
{
  assert_choice_type(types::cgi_nr, type_, "Value");
  return c.get<cgi_nr_s>();
}
fixed_bitstring<64, false, true>& otdoa_cell_info_item_ext_ie_o::value_c::sfn_initisation_time_nr()
{
  assert_choice_type(types::sfn_initisation_time_nr, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
const tdd_cfg_eutra_item_s& otdoa_cell_info_item_ext_ie_o::value_c::tdd_cfg_eutra_item() const
{
  assert_choice_type(types::tdd_cfg_eutra_item, type_, "Value");
  return c.get<tdd_cfg_eutra_item_s>();
}
const cgi_nr_s& otdoa_cell_info_item_ext_ie_o::value_c::cgi_nr() const
{
  assert_choice_type(types::cgi_nr, type_, "Value");
  return c.get<cgi_nr_s>();
}
const fixed_bitstring<64, false, true>& otdoa_cell_info_item_ext_ie_o::value_c::sfn_initisation_time_nr() const
{
  assert_choice_type(types::sfn_initisation_time_nr, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
void otdoa_cell_info_item_ext_ie_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::tdd_cfg_eutra_item:
      c.get<tdd_cfg_eutra_item_s>().to_json(j);
      break;
    case types::cgi_nr:
      c.get<cgi_nr_s>().to_json(j);
      break;
    case types::sfn_initisation_time_nr:
      c.get<fixed_bitstring<64, false, true>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_ext_ie_o::value_c");
  }
}
OCUDUASN_CODE otdoa_cell_info_item_ext_ie_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::tdd_cfg_eutra_item:
      HANDLE_CODE(c.get<tdd_cfg_eutra_item_s>().pack(bref));
      break;
    case types::cgi_nr:
      HANDLE_CODE(c.get<cgi_nr_s>().pack(bref));
      break;
    case types::sfn_initisation_time_nr:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_ext_ie_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_cell_info_item_ext_ie_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::tdd_cfg_eutra_item:
      HANDLE_CODE(c.get<tdd_cfg_eutra_item_s>().unpack(bref));
      break;
    case types::cgi_nr:
      HANDLE_CODE(c.get<cgi_nr_s>().unpack(bref));
      break;
    case types::sfn_initisation_time_nr:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_ext_ie_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* otdoa_cell_info_item_ext_ie_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TDD-Config-EUTRA-Item", "CGI-NR", "BIT STRING"};
  return convert_enum_idx(names, 3, value, "otdoa_cell_info_item_ext_ie_o::value_c::types");
}

// OTDOACell-Information-Item ::= CHOICE
void otdoa_cell_info_item_c::destroy_()
{
  switch (type_) {
    case types::cgi_eutra:
      c.destroy<cgi_eutra_s>();
      break;
    case types::tac:
      c.destroy<fixed_octstring<3, true>>();
      break;
    case types::sfn_initisation_time_eutra:
      c.destroy<fixed_bitstring<64, false, true>>();
      break;
    case types::ng_ran_access_point_position:
      c.destroy<ng_ran_access_point_position_s>();
      break;
    case types::prs_muting_cfg_eutra:
      c.destroy<prs_muting_cfg_eutra_c>();
      break;
    case types::prs_freq_hop_cfg_eutra:
      c.destroy<prs_freq_hop_cfg_eutra_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>();
      break;
    default:
      break;
  }
}
void otdoa_cell_info_item_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::pci_eutra:
      break;
    case types::cgi_eutra:
      c.init<cgi_eutra_s>();
      break;
    case types::tac:
      c.init<fixed_octstring<3, true>>();
      break;
    case types::earfcn:
      break;
    case types::prs_bw_eutra:
      break;
    case types::prs_cfg_idx_eutra:
      break;
    case types::cp_len_eutra:
      break;
    case types::nof_dl_frames_eutra:
      break;
    case types::nof_ant_ports_eutra:
      break;
    case types::sfn_initisation_time_eutra:
      c.init<fixed_bitstring<64, false, true>>();
      break;
    case types::ng_ran_access_point_position:
      c.init<ng_ran_access_point_position_s>();
      break;
    case types::prs_muting_cfg_eutra:
      c.init<prs_muting_cfg_eutra_c>();
      break;
    case types::prsid_eutra:
      break;
    case types::tpid_eutra:
      break;
    case types::tp_type_eutra:
      break;
    case types::nof_dl_frames_extended_eutra:
      break;
    case types::crs_cp_len_eutra:
      break;
    case types::dl_bw_eutra:
      break;
    case types::prs_occasion_group_eutra:
      break;
    case types::prs_freq_hop_cfg_eutra:
      c.init<prs_freq_hop_cfg_eutra_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_c");
  }
}
otdoa_cell_info_item_c::otdoa_cell_info_item_c(const otdoa_cell_info_item_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::pci_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::cgi_eutra:
      c.init(other.c.get<cgi_eutra_s>());
      break;
    case types::tac:
      c.init(other.c.get<fixed_octstring<3, true>>());
      break;
    case types::earfcn:
      c.init(other.c.get<uint32_t>());
      break;
    case types::prs_bw_eutra:
      c.init(other.c.get<prs_bw_eutra_e>());
      break;
    case types::prs_cfg_idx_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::cp_len_eutra:
      c.init(other.c.get<cp_len_eutra_e>());
      break;
    case types::nof_dl_frames_eutra:
      c.init(other.c.get<nof_dl_frames_eutra_e>());
      break;
    case types::nof_ant_ports_eutra:
      c.init(other.c.get<nof_ant_ports_eutra_e>());
      break;
    case types::sfn_initisation_time_eutra:
      c.init(other.c.get<fixed_bitstring<64, false, true>>());
      break;
    case types::ng_ran_access_point_position:
      c.init(other.c.get<ng_ran_access_point_position_s>());
      break;
    case types::prs_muting_cfg_eutra:
      c.init(other.c.get<prs_muting_cfg_eutra_c>());
      break;
    case types::prsid_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::tpid_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::tp_type_eutra:
      c.init(other.c.get<tp_type_eutra_e>());
      break;
    case types::nof_dl_frames_extended_eutra:
      c.init(other.c.get<uint16_t>());
      break;
    case types::crs_cp_len_eutra:
      c.init(other.c.get<cp_len_eutra_e>());
      break;
    case types::dl_bw_eutra:
      c.init(other.c.get<dl_bw_eutra_e>());
      break;
    case types::prs_occasion_group_eutra:
      c.init(other.c.get<prs_occasion_group_eutra_e>());
      break;
    case types::prs_freq_hop_cfg_eutra:
      c.init(other.c.get<prs_freq_hop_cfg_eutra_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_c");
  }
}
otdoa_cell_info_item_c& otdoa_cell_info_item_c::operator=(const otdoa_cell_info_item_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::pci_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::cgi_eutra:
      c.set(other.c.get<cgi_eutra_s>());
      break;
    case types::tac:
      c.set(other.c.get<fixed_octstring<3, true>>());
      break;
    case types::earfcn:
      c.set(other.c.get<uint32_t>());
      break;
    case types::prs_bw_eutra:
      c.set(other.c.get<prs_bw_eutra_e>());
      break;
    case types::prs_cfg_idx_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::cp_len_eutra:
      c.set(other.c.get<cp_len_eutra_e>());
      break;
    case types::nof_dl_frames_eutra:
      c.set(other.c.get<nof_dl_frames_eutra_e>());
      break;
    case types::nof_ant_ports_eutra:
      c.set(other.c.get<nof_ant_ports_eutra_e>());
      break;
    case types::sfn_initisation_time_eutra:
      c.set(other.c.get<fixed_bitstring<64, false, true>>());
      break;
    case types::ng_ran_access_point_position:
      c.set(other.c.get<ng_ran_access_point_position_s>());
      break;
    case types::prs_muting_cfg_eutra:
      c.set(other.c.get<prs_muting_cfg_eutra_c>());
      break;
    case types::prsid_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::tpid_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::tp_type_eutra:
      c.set(other.c.get<tp_type_eutra_e>());
      break;
    case types::nof_dl_frames_extended_eutra:
      c.set(other.c.get<uint16_t>());
      break;
    case types::crs_cp_len_eutra:
      c.set(other.c.get<cp_len_eutra_e>());
      break;
    case types::dl_bw_eutra:
      c.set(other.c.get<dl_bw_eutra_e>());
      break;
    case types::prs_occasion_group_eutra:
      c.set(other.c.get<prs_occasion_group_eutra_e>());
      break;
    case types::prs_freq_hop_cfg_eutra:
      c.set(other.c.get<prs_freq_hop_cfg_eutra_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_c");
  }

  return *this;
}
uint16_t& otdoa_cell_info_item_c::set_pci_eutra()
{
  set(types::pci_eutra);
  return c.get<uint16_t>();
}
cgi_eutra_s& otdoa_cell_info_item_c::set_cgi_eutra()
{
  set(types::cgi_eutra);
  return c.get<cgi_eutra_s>();
}
fixed_octstring<3, true>& otdoa_cell_info_item_c::set_tac()
{
  set(types::tac);
  return c.get<fixed_octstring<3, true>>();
}
uint32_t& otdoa_cell_info_item_c::set_earfcn()
{
  set(types::earfcn);
  return c.get<uint32_t>();
}
prs_bw_eutra_e& otdoa_cell_info_item_c::set_prs_bw_eutra()
{
  set(types::prs_bw_eutra);
  return c.get<prs_bw_eutra_e>();
}
uint16_t& otdoa_cell_info_item_c::set_prs_cfg_idx_eutra()
{
  set(types::prs_cfg_idx_eutra);
  return c.get<uint16_t>();
}
cp_len_eutra_e& otdoa_cell_info_item_c::set_cp_len_eutra()
{
  set(types::cp_len_eutra);
  return c.get<cp_len_eutra_e>();
}
nof_dl_frames_eutra_e& otdoa_cell_info_item_c::set_nof_dl_frames_eutra()
{
  set(types::nof_dl_frames_eutra);
  return c.get<nof_dl_frames_eutra_e>();
}
nof_ant_ports_eutra_e& otdoa_cell_info_item_c::set_nof_ant_ports_eutra()
{
  set(types::nof_ant_ports_eutra);
  return c.get<nof_ant_ports_eutra_e>();
}
fixed_bitstring<64, false, true>& otdoa_cell_info_item_c::set_sfn_initisation_time_eutra()
{
  set(types::sfn_initisation_time_eutra);
  return c.get<fixed_bitstring<64, false, true>>();
}
ng_ran_access_point_position_s& otdoa_cell_info_item_c::set_ng_ran_access_point_position()
{
  set(types::ng_ran_access_point_position);
  return c.get<ng_ran_access_point_position_s>();
}
prs_muting_cfg_eutra_c& otdoa_cell_info_item_c::set_prs_muting_cfg_eutra()
{
  set(types::prs_muting_cfg_eutra);
  return c.get<prs_muting_cfg_eutra_c>();
}
uint16_t& otdoa_cell_info_item_c::set_prsid_eutra()
{
  set(types::prsid_eutra);
  return c.get<uint16_t>();
}
uint16_t& otdoa_cell_info_item_c::set_tpid_eutra()
{
  set(types::tpid_eutra);
  return c.get<uint16_t>();
}
tp_type_eutra_e& otdoa_cell_info_item_c::set_tp_type_eutra()
{
  set(types::tp_type_eutra);
  return c.get<tp_type_eutra_e>();
}
uint16_t& otdoa_cell_info_item_c::set_nof_dl_frames_extended_eutra()
{
  set(types::nof_dl_frames_extended_eutra);
  return c.get<uint16_t>();
}
cp_len_eutra_e& otdoa_cell_info_item_c::set_crs_cp_len_eutra()
{
  set(types::crs_cp_len_eutra);
  return c.get<cp_len_eutra_e>();
}
dl_bw_eutra_e& otdoa_cell_info_item_c::set_dl_bw_eutra()
{
  set(types::dl_bw_eutra);
  return c.get<dl_bw_eutra_e>();
}
prs_occasion_group_eutra_e& otdoa_cell_info_item_c::set_prs_occasion_group_eutra()
{
  set(types::prs_occasion_group_eutra);
  return c.get<prs_occasion_group_eutra_e>();
}
prs_freq_hop_cfg_eutra_s& otdoa_cell_info_item_c::set_prs_freq_hop_cfg_eutra()
{
  set(types::prs_freq_hop_cfg_eutra);
  return c.get<prs_freq_hop_cfg_eutra_s>();
}
protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>& otdoa_cell_info_item_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>();
}
void otdoa_cell_info_item_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::pci_eutra:
      j.write_int("pCI-EUTRA", c.get<uint16_t>());
      break;
    case types::cgi_eutra:
      j.write_fieldname("cGI-EUTRA");
      c.get<cgi_eutra_s>().to_json(j);
      break;
    case types::tac:
      j.write_str("tAC", c.get<fixed_octstring<3, true>>().to_string());
      break;
    case types::earfcn:
      j.write_int("eARFCN", c.get<uint32_t>());
      break;
    case types::prs_bw_eutra:
      j.write_str("pRS-Bandwidth-EUTRA", c.get<prs_bw_eutra_e>().to_string());
      break;
    case types::prs_cfg_idx_eutra:
      j.write_int("pRS-ConfigurationIndex-EUTRA", c.get<uint16_t>());
      break;
    case types::cp_len_eutra:
      j.write_str("cPLength-EUTRA", c.get<cp_len_eutra_e>().to_string());
      break;
    case types::nof_dl_frames_eutra:
      j.write_str("numberOfDlFrames-EUTRA", c.get<nof_dl_frames_eutra_e>().to_string());
      break;
    case types::nof_ant_ports_eutra:
      j.write_str("numberOfAntennaPorts-EUTRA", c.get<nof_ant_ports_eutra_e>().to_string());
      break;
    case types::sfn_initisation_time_eutra:
      j.write_fieldname("sFNInitialisationTime-EUTRA");
      c.get<fixed_bitstring<64, false, true>>().to_json(j);
      break;
    case types::ng_ran_access_point_position:
      j.write_fieldname("nG-RANAccessPointPosition");
      c.get<ng_ran_access_point_position_s>().to_json(j);
      break;
    case types::prs_muting_cfg_eutra:
      j.write_fieldname("pRSMutingConfiguration-EUTRA");
      c.get<prs_muting_cfg_eutra_c>().to_json(j);
      break;
    case types::prsid_eutra:
      j.write_int("prsid-EUTRA", c.get<uint16_t>());
      break;
    case types::tpid_eutra:
      j.write_int("tpid-EUTRA", c.get<uint16_t>());
      break;
    case types::tp_type_eutra:
      j.write_str("tpType-EUTRA", "prs-only-tp");
      break;
    case types::nof_dl_frames_extended_eutra:
      j.write_int("numberOfDlFrames-Extended-EUTRA", c.get<uint16_t>());
      break;
    case types::crs_cp_len_eutra:
      j.write_str("crsCPlength-EUTRA", c.get<cp_len_eutra_e>().to_string());
      break;
    case types::dl_bw_eutra:
      j.write_str("dL-Bandwidth-EUTRA", c.get<dl_bw_eutra_e>().to_string());
      break;
    case types::prs_occasion_group_eutra:
      j.write_str("pRSOccasionGroup-EUTRA", c.get<prs_occasion_group_eutra_e>().to_string());
      break;
    case types::prs_freq_hop_cfg_eutra:
      j.write_fieldname("pRSFrequencyHoppingConfiguration-EUTRA");
      c.get<prs_freq_hop_cfg_eutra_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_c");
  }
  j.end_obj();
}
OCUDUASN_CODE otdoa_cell_info_item_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::pci_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)503u, true, true));
      break;
    case types::cgi_eutra:
      HANDLE_CODE(c.get<cgi_eutra_s>().pack(bref));
      break;
    case types::tac:
      HANDLE_CODE((c.get<fixed_octstring<3, true>>().pack(bref)));
      break;
    case types::earfcn:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)262143u, true, true));
      break;
    case types::prs_bw_eutra:
      HANDLE_CODE(c.get<prs_bw_eutra_e>().pack(bref));
      break;
    case types::prs_cfg_idx_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    case types::cp_len_eutra:
      HANDLE_CODE(c.get<cp_len_eutra_e>().pack(bref));
      break;
    case types::nof_dl_frames_eutra:
      HANDLE_CODE(c.get<nof_dl_frames_eutra_e>().pack(bref));
      break;
    case types::nof_ant_ports_eutra:
      HANDLE_CODE(c.get<nof_ant_ports_eutra_e>().pack(bref));
      break;
    case types::sfn_initisation_time_eutra:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    case types::ng_ran_access_point_position:
      HANDLE_CODE(c.get<ng_ran_access_point_position_s>().pack(bref));
      break;
    case types::prs_muting_cfg_eutra:
      HANDLE_CODE(c.get<prs_muting_cfg_eutra_c>().pack(bref));
      break;
    case types::prsid_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    case types::tpid_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    case types::tp_type_eutra:
      HANDLE_CODE(c.get<tp_type_eutra_e>().pack(bref));
      break;
    case types::nof_dl_frames_extended_eutra:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)160u, true, true));
      break;
    case types::crs_cp_len_eutra:
      HANDLE_CODE(c.get<cp_len_eutra_e>().pack(bref));
      break;
    case types::dl_bw_eutra:
      HANDLE_CODE(c.get<dl_bw_eutra_e>().pack(bref));
      break;
    case types::prs_occasion_group_eutra:
      HANDLE_CODE(c.get<prs_occasion_group_eutra_e>().pack(bref));
      break;
    case types::prs_freq_hop_cfg_eutra:
      HANDLE_CODE(c.get<prs_freq_hop_cfg_eutra_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_cell_info_item_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::pci_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)503u, true, true));
      break;
    case types::cgi_eutra:
      HANDLE_CODE(c.get<cgi_eutra_s>().unpack(bref));
      break;
    case types::tac:
      HANDLE_CODE((c.get<fixed_octstring<3, true>>().unpack(bref)));
      break;
    case types::earfcn:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)262143u, true, true));
      break;
    case types::prs_bw_eutra:
      HANDLE_CODE(c.get<prs_bw_eutra_e>().unpack(bref));
      break;
    case types::prs_cfg_idx_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    case types::cp_len_eutra:
      HANDLE_CODE(c.get<cp_len_eutra_e>().unpack(bref));
      break;
    case types::nof_dl_frames_eutra:
      HANDLE_CODE(c.get<nof_dl_frames_eutra_e>().unpack(bref));
      break;
    case types::nof_ant_ports_eutra:
      HANDLE_CODE(c.get<nof_ant_ports_eutra_e>().unpack(bref));
      break;
    case types::sfn_initisation_time_eutra:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    case types::ng_ran_access_point_position:
      HANDLE_CODE(c.get<ng_ran_access_point_position_s>().unpack(bref));
      break;
    case types::prs_muting_cfg_eutra:
      HANDLE_CODE(c.get<prs_muting_cfg_eutra_c>().unpack(bref));
      break;
    case types::prsid_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    case types::tpid_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    case types::tp_type_eutra:
      HANDLE_CODE(c.get<tp_type_eutra_e>().unpack(bref));
      break;
    case types::nof_dl_frames_extended_eutra:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)160u, true, true));
      break;
    case types::crs_cp_len_eutra:
      HANDLE_CODE(c.get<cp_len_eutra_e>().unpack(bref));
      break;
    case types::dl_bw_eutra:
      HANDLE_CODE(c.get<dl_bw_eutra_e>().unpack(bref));
      break;
    case types::prs_occasion_group_eutra:
      HANDLE_CODE(c.get<prs_occasion_group_eutra_e>().unpack(bref));
      break;
    case types::prs_freq_hop_cfg_eutra:
      HANDLE_CODE(c.get<prs_freq_hop_cfg_eutra_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_cell_info_item_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* otdoa_cell_info_item_c::types_opts::to_string() const
{
  static const char* names[] = {"pCI-EUTRA",
                                "cGI-EUTRA",
                                "tAC",
                                "eARFCN",
                                "pRS-Bandwidth-EUTRA",
                                "pRS-ConfigurationIndex-EUTRA",
                                "cPLength-EUTRA",
                                "numberOfDlFrames-EUTRA",
                                "numberOfAntennaPorts-EUTRA",
                                "sFNInitialisationTime-EUTRA",
                                "nG-RANAccessPointPosition",
                                "pRSMutingConfiguration-EUTRA",
                                "prsid-EUTRA",
                                "tpid-EUTRA",
                                "tpType-EUTRA",
                                "numberOfDlFrames-Extended-EUTRA",
                                "crsCPlength-EUTRA",
                                "dL-Bandwidth-EUTRA",
                                "pRSOccasionGroup-EUTRA",
                                "pRSFrequencyHoppingConfiguration-EUTRA",
                                "choice-Extension"};
  return convert_enum_idx(names, 21, value, "otdoa_cell_info_item_c::types");
}

OCUDUASN_CODE otdoa_cells_item_s_::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, otdoa_cell_info, 1, 63, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_cells_item_s_::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(otdoa_cell_info, bref, 1, 63, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void otdoa_cells_item_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("oTDOACellInfo");
  for (const auto& e1 : otdoa_cell_info) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// OnDemandPRS-Info ::= SEQUENCE
OCUDUASN_CODE on_demand_prs_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(allowed_res_set_periodicity_values_present, 1));
  HANDLE_CODE(bref.pack(allowed_prs_bw_values_present, 1));
  HANDLE_CODE(bref.pack(allowed_res_repeat_factor_values_present, 1));
  HANDLE_CODE(bref.pack(allowed_res_nof_symbols_values_present, 1));
  HANDLE_CODE(bref.pack(allowed_comb_size_values_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(on_demand_prs_request_allowed.pack(bref));
  if (allowed_res_set_periodicity_values_present) {
    HANDLE_CODE(allowed_res_set_periodicity_values.pack(bref));
  }
  if (allowed_prs_bw_values_present) {
    HANDLE_CODE(allowed_prs_bw_values.pack(bref));
  }
  if (allowed_res_repeat_factor_values_present) {
    HANDLE_CODE(allowed_res_repeat_factor_values.pack(bref));
  }
  if (allowed_res_nof_symbols_values_present) {
    HANDLE_CODE(allowed_res_nof_symbols_values.pack(bref));
  }
  if (allowed_comb_size_values_present) {
    HANDLE_CODE(allowed_comb_size_values.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE on_demand_prs_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(allowed_res_set_periodicity_values_present, 1));
  HANDLE_CODE(bref.unpack(allowed_prs_bw_values_present, 1));
  HANDLE_CODE(bref.unpack(allowed_res_repeat_factor_values_present, 1));
  HANDLE_CODE(bref.unpack(allowed_res_nof_symbols_values_present, 1));
  HANDLE_CODE(bref.unpack(allowed_comb_size_values_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(on_demand_prs_request_allowed.unpack(bref));
  if (allowed_res_set_periodicity_values_present) {
    HANDLE_CODE(allowed_res_set_periodicity_values.unpack(bref));
  }
  if (allowed_prs_bw_values_present) {
    HANDLE_CODE(allowed_prs_bw_values.unpack(bref));
  }
  if (allowed_res_repeat_factor_values_present) {
    HANDLE_CODE(allowed_res_repeat_factor_values.unpack(bref));
  }
  if (allowed_res_nof_symbols_values_present) {
    HANDLE_CODE(allowed_res_nof_symbols_values.unpack(bref));
  }
  if (allowed_comb_size_values_present) {
    HANDLE_CODE(allowed_comb_size_values.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void on_demand_prs_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("onDemandPRSRequestAllowed");
  on_demand_prs_request_allowed.to_json(j);
  if (allowed_res_set_periodicity_values_present) {
    j.write_fieldname("allowedResourceSetPeriodicityValues");
    allowed_res_set_periodicity_values.to_json(j);
  }
  if (allowed_prs_bw_values_present) {
    j.write_fieldname("allowedPRSBandwidthValues");
    allowed_prs_bw_values.to_json(j);
  }
  if (allowed_res_repeat_factor_values_present) {
    j.write_fieldname("allowedResourceRepetitionFactorValues");
    allowed_res_repeat_factor_values.to_json(j);
  }
  if (allowed_res_nof_symbols_values_present) {
    j.write_fieldname("allowedResourceNumberOfSymbolsValues");
    allowed_res_nof_symbols_values.to_json(j);
  }
  if (allowed_comb_size_values_present) {
    j.write_fieldname("allowedCombSizeValues");
    allowed_comb_size_values.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultGERAN-Item ::= SEQUENCE
OCUDUASN_CODE result_geran_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, bcch, (uint16_t)0u, (uint16_t)1023u, true, true));
  HANDLE_CODE(pack_integer(bref, pci_geran, (uint8_t)0u, (uint8_t)63u, true, true));
  HANDLE_CODE(pack_integer(bref, rssi, (uint8_t)0u, (uint8_t)63u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_geran_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(bcch, bref, (uint16_t)0u, (uint16_t)1023u, true, true));
  HANDLE_CODE(unpack_integer(pci_geran, bref, (uint8_t)0u, (uint8_t)63u, true, true));
  HANDLE_CODE(unpack_integer(rssi, bref, (uint8_t)0u, (uint8_t)63u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_geran_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("bCCH", bcch);
  j.write_int("physCellIDGERAN", pci_geran);
  j.write_int("rSSI", rssi);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultUTRAN-Item ::= SEQUENCE
OCUDUASN_CODE result_utran_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(utra_rs_cp_present, 1));
  HANDLE_CODE(bref.pack(utra_ec_n0_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, uarfcn, (uint16_t)0u, (uint16_t)16383u, true, true));
  HANDLE_CODE(pci_utran.pack(bref));
  if (utra_rs_cp_present) {
    HANDLE_CODE(pack_integer(bref, utra_rs_cp, (int16_t)-5, (int16_t)91, true, true));
  }
  if (utra_ec_n0_present) {
    HANDLE_CODE(pack_integer(bref, utra_ec_n0, (uint8_t)0u, (uint8_t)49u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_utran_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(utra_rs_cp_present, 1));
  HANDLE_CODE(bref.unpack(utra_ec_n0_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(uarfcn, bref, (uint16_t)0u, (uint16_t)16383u, true, true));
  HANDLE_CODE(pci_utran.unpack(bref));
  if (utra_rs_cp_present) {
    HANDLE_CODE(unpack_integer(utra_rs_cp, bref, (int16_t)-5, (int16_t)91, true, true));
  }
  if (utra_ec_n0_present) {
    HANDLE_CODE(unpack_integer(utra_ec_n0, bref, (uint8_t)0u, (uint8_t)49u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_utran_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("uARFCN", uarfcn);
  j.write_fieldname("physCellIDUTRAN");
  pci_utran.to_json(j);
  if (utra_rs_cp_present) {
    j.write_int("uTRA-RSCP", utra_rs_cp);
  }
  if (utra_ec_n0_present) {
    j.write_int("uTRA-EcN0", utra_ec_n0);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

void result_utran_item_s::pci_utran_c_::destroy_() {}
void result_utran_item_s::pci_utran_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
result_utran_item_s::pci_utran_c_::pci_utran_c_(const result_utran_item_s::pci_utran_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::phys_cell_i_du_tra_fdd:
      c.init(other.c.get<uint16_t>());
      break;
    case types::phys_cell_i_du_tra_tdd:
      c.init(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "result_utran_item_s::pci_utran_c_");
  }
}
result_utran_item_s::pci_utran_c_&
result_utran_item_s::pci_utran_c_::operator=(const result_utran_item_s::pci_utran_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::phys_cell_i_du_tra_fdd:
      c.set(other.c.get<uint16_t>());
      break;
    case types::phys_cell_i_du_tra_tdd:
      c.set(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "result_utran_item_s::pci_utran_c_");
  }

  return *this;
}
uint16_t& result_utran_item_s::pci_utran_c_::set_phys_cell_i_du_tra_fdd()
{
  set(types::phys_cell_i_du_tra_fdd);
  return c.get<uint16_t>();
}
uint16_t& result_utran_item_s::pci_utran_c_::set_phys_cell_i_du_tra_tdd()
{
  set(types::phys_cell_i_du_tra_tdd);
  return c.get<uint16_t>();
}
void result_utran_item_s::pci_utran_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::phys_cell_i_du_tra_fdd:
      j.write_int("physCellIDUTRA-FDD", c.get<uint16_t>());
      break;
    case types::phys_cell_i_du_tra_tdd:
      j.write_int("physCellIDUTRA-TDD", c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "result_utran_item_s::pci_utran_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE result_utran_item_s::pci_utran_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::phys_cell_i_du_tra_fdd:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)511u, true, true));
      break;
    case types::phys_cell_i_du_tra_tdd:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)127u, true, true));
      break;
    default:
      log_invalid_choice_id(type_, "result_utran_item_s::pci_utran_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_utran_item_s::pci_utran_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::phys_cell_i_du_tra_fdd:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)511u, true, true));
      break;
    case types::phys_cell_i_du_tra_tdd:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)127u, true, true));
      break;
    default:
      log_invalid_choice_id(type_, "result_utran_item_s::pci_utran_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* result_utran_item_s::pci_utran_c_::types_opts::to_string() const
{
  static const char* names[] = {"physCellIDUTRA-FDD", "physCellIDUTRA-TDD"};
  return convert_enum_idx(names, 2, value, "result_utran_item_s::pci_utran_c_::types");
}

// ResultNR-Item ::= SEQUENCE
OCUDUASN_CODE result_nr_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(value_ss_rsrp_cell_present, 1));
  HANDLE_CODE(bref.pack(value_ss_rsrq_cell_present, 1));
  HANDLE_CODE(bref.pack(ss_rsrp_per_ssb.size() > 0, 1));
  HANDLE_CODE(bref.pack(ss_rsrq_per_ssb.size() > 0, 1));
  HANDLE_CODE(bref.pack(cgi_nr_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, nr_pci, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(pack_integer(bref, nr_arfcn, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (value_ss_rsrp_cell_present) {
    HANDLE_CODE(pack_integer(bref, value_ss_rsrp_cell, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (value_ss_rsrq_cell_present) {
    HANDLE_CODE(pack_integer(bref, value_ss_rsrq_cell, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (ss_rsrp_per_ssb.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ss_rsrp_per_ssb, 1, 64, true));
  }
  if (ss_rsrq_per_ssb.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ss_rsrq_per_ssb, 1, 64, true));
  }
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_nr_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(value_ss_rsrp_cell_present, 1));
  HANDLE_CODE(bref.unpack(value_ss_rsrq_cell_present, 1));
  bool ss_rsrp_per_ssb_present;
  HANDLE_CODE(bref.unpack(ss_rsrp_per_ssb_present, 1));
  bool ss_rsrq_per_ssb_present;
  HANDLE_CODE(bref.unpack(ss_rsrq_per_ssb_present, 1));
  HANDLE_CODE(bref.unpack(cgi_nr_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(nr_pci, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  HANDLE_CODE(unpack_integer(nr_arfcn, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  if (value_ss_rsrp_cell_present) {
    HANDLE_CODE(unpack_integer(value_ss_rsrp_cell, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (value_ss_rsrq_cell_present) {
    HANDLE_CODE(unpack_integer(value_ss_rsrq_cell, bref, (uint8_t)0u, (uint8_t)127u, false, true));
  }
  if (ss_rsrp_per_ssb_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ss_rsrp_per_ssb, bref, 1, 64, true));
  }
  if (ss_rsrq_per_ssb_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ss_rsrq_per_ssb, bref, 1, 64, true));
  }
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_nr_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("nR-PCI", nr_pci);
  j.write_int("nR-ARFCN", nr_arfcn);
  if (value_ss_rsrp_cell_present) {
    j.write_int("valueSS-RSRP-Cell", value_ss_rsrp_cell);
  }
  if (value_ss_rsrq_cell_present) {
    j.write_int("valueSS-RSRQ-Cell", value_ss_rsrq_cell);
  }
  if (ss_rsrp_per_ssb.size() > 0) {
    j.start_array("sS-RSRP-PerSSB");
    for (const auto& e1 : ss_rsrp_per_ssb) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ss_rsrq_per_ssb.size() > 0) {
    j.start_array("sS-RSRQ-PerSSB");
    for (const auto& e1 : ss_rsrq_per_ssb) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cgi_nr_present) {
    j.write_fieldname("cGI-NR");
    cgi_nr.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResultEUTRA-Item ::= SEQUENCE
OCUDUASN_CODE result_eutra_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(value_rsrp_eutra_present, 1));
  HANDLE_CODE(bref.pack(value_rsrq_eutra_present, 1));
  HANDLE_CODE(bref.pack(cgi_eutra_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pci_eutra, (uint16_t)0u, (uint16_t)503u, true, true));
  HANDLE_CODE(pack_integer(bref, earfcn, (uint32_t)0u, (uint32_t)262143u, true, true));
  if (value_rsrp_eutra_present) {
    HANDLE_CODE(pack_integer(bref, value_rsrp_eutra, (uint16_t)0u, (uint16_t)97u, true, true));
  }
  if (value_rsrq_eutra_present) {
    HANDLE_CODE(pack_integer(bref, value_rsrq_eutra, (uint8_t)0u, (uint8_t)34u, true, true));
  }
  if (cgi_eutra_present) {
    HANDLE_CODE(cgi_eutra.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE result_eutra_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(value_rsrp_eutra_present, 1));
  HANDLE_CODE(bref.unpack(value_rsrq_eutra_present, 1));
  HANDLE_CODE(bref.unpack(cgi_eutra_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pci_eutra, bref, (uint16_t)0u, (uint16_t)503u, true, true));
  HANDLE_CODE(unpack_integer(earfcn, bref, (uint32_t)0u, (uint32_t)262143u, true, true));
  if (value_rsrp_eutra_present) {
    HANDLE_CODE(unpack_integer(value_rsrp_eutra, bref, (uint16_t)0u, (uint16_t)97u, true, true));
  }
  if (value_rsrq_eutra_present) {
    HANDLE_CODE(unpack_integer(value_rsrq_eutra, bref, (uint8_t)0u, (uint8_t)34u, true, true));
  }
  if (cgi_eutra_present) {
    HANDLE_CODE(cgi_eutra.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void result_eutra_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pCI-EUTRA", pci_eutra);
  j.write_int("eARFCN", earfcn);
  if (value_rsrp_eutra_present) {
    j.write_int("valueRSRP-EUTRA", value_rsrp_eutra);
  }
  if (value_rsrq_eutra_present) {
    j.write_int("valueRSRQ-EUTRA", value_rsrq_eutra);
  }
  if (cgi_eutra_present) {
    j.write_fieldname("cGI-EUTRA");
    cgi_eutra.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// OtherRATMeasuredResultsValue-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t other_rat_measured_results_value_ext_ie_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {55, 56};
  return map_enum_number(names, 2, idx, "id");
}
bool other_rat_measured_results_value_ext_ie_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {55, 56};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e other_rat_measured_results_value_ext_ie_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 55:
      return crit_e::ignore;
    case 56:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
other_rat_measured_results_value_ext_ie_o::value_c
other_rat_measured_results_value_ext_ie_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 55:
      ret.set(value_c::types::result_nr);
      break;
    case 56:
      ret.set(value_c::types::result_eutra);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e other_rat_measured_results_value_ext_ie_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 55:
      return presence_e::mandatory;
    case 56:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void other_rat_measured_results_value_ext_ie_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::result_nr:
      c = result_nr_l{};
      break;
    case types::result_eutra:
      c = result_eutra_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_ext_ie_o::value_c");
  }
}
result_nr_l& other_rat_measured_results_value_ext_ie_o::value_c::result_nr()
{
  assert_choice_type(types::result_nr, type_, "Value");
  return c.get<result_nr_l>();
}
result_eutra_l& other_rat_measured_results_value_ext_ie_o::value_c::result_eutra()
{
  assert_choice_type(types::result_eutra, type_, "Value");
  return c.get<result_eutra_l>();
}
const result_nr_l& other_rat_measured_results_value_ext_ie_o::value_c::result_nr() const
{
  assert_choice_type(types::result_nr, type_, "Value");
  return c.get<result_nr_l>();
}
const result_eutra_l& other_rat_measured_results_value_ext_ie_o::value_c::result_eutra() const
{
  assert_choice_type(types::result_eutra, type_, "Value");
  return c.get<result_eutra_l>();
}
void other_rat_measured_results_value_ext_ie_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::result_nr:
      j.start_array("");
      for (const auto& e1 : c.get<result_nr_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::result_eutra:
      j.start_array("");
      for (const auto& e1 : c.get<result_eutra_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_ext_ie_o::value_c");
  }
}
OCUDUASN_CODE other_rat_measured_results_value_ext_ie_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::result_nr:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_nr_l>(), 1, 8, true));
      break;
    case types::result_eutra:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_eutra_l>(), 1, 8, true));
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_ext_ie_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE other_rat_measured_results_value_ext_ie_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::result_nr:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_nr_l>(), bref, 1, 8, true));
      break;
    case types::result_eutra:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_eutra_l>(), bref, 1, 8, true));
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_ext_ie_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* other_rat_measured_results_value_ext_ie_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"ResultNR", "ResultEUTRA"};
  return convert_enum_idx(names, 2, value, "other_rat_measured_results_value_ext_ie_o::value_c::types");
}

// OtherRATMeasuredResultsValue ::= CHOICE
void other_rat_measured_results_value_c::destroy_()
{
  switch (type_) {
    case types::result_geran:
      c.destroy<result_geran_l>();
      break;
    case types::result_utran:
      c.destroy<result_utran_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>();
      break;
    default:
      break;
  }
}
void other_rat_measured_results_value_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::result_geran:
      c.init<result_geran_l>();
      break;
    case types::result_utran:
      c.init<result_utran_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_c");
  }
}
other_rat_measured_results_value_c::other_rat_measured_results_value_c(const other_rat_measured_results_value_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::result_geran:
      c.init(other.c.get<result_geran_l>());
      break;
    case types::result_utran:
      c.init(other.c.get<result_utran_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_c");
  }
}
other_rat_measured_results_value_c&
other_rat_measured_results_value_c::operator=(const other_rat_measured_results_value_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::result_geran:
      c.set(other.c.get<result_geran_l>());
      break;
    case types::result_utran:
      c.set(other.c.get<result_utran_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_c");
  }

  return *this;
}
result_geran_l& other_rat_measured_results_value_c::set_result_geran()
{
  set(types::result_geran);
  return c.get<result_geran_l>();
}
result_utran_l& other_rat_measured_results_value_c::set_result_utran()
{
  set(types::result_utran);
  return c.get<result_utran_l>();
}
protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>&
other_rat_measured_results_value_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>();
}
void other_rat_measured_results_value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::result_geran:
      j.start_array("resultGERAN");
      for (const auto& e1 : c.get<result_geran_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::result_utran:
      j.start_array("resultUTRAN");
      for (const auto& e1 : c.get<result_utran_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE other_rat_measured_results_value_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::result_geran:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_geran_l>(), 1, 8, true));
      break;
    case types::result_utran:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<result_utran_l>(), 1, 8, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE other_rat_measured_results_value_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::result_geran:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_geran_l>(), bref, 1, 8, true));
      break;
    case types::result_utran:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<result_utran_l>(), bref, 1, 8, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "other_rat_measured_results_value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* other_rat_measured_results_value_c::types_opts::to_string() const
{
  static const char* names[] = {"resultGERAN", "resultUTRAN", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "other_rat_measured_results_value_c::types");
}

// OtherRATMeasurementQuantitiesValue ::= ENUMERATED
const char* other_rat_meas_quantities_value_opts::to_string() const
{
  static const char* names[] = {"geran", "utran", "nR", "eUTRA"};
  return convert_enum_idx(names, 4, value, "other_rat_meas_quantities_value_e");
}

// OtherRATMeasurementQuantities-Item ::= SEQUENCE
OCUDUASN_CODE other_rat_meas_quantities_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(other_rat_meas_quantities_value.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE other_rat_meas_quantities_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(other_rat_meas_quantities_value.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void other_rat_meas_quantities_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("otherRATMeasurementQuantitiesValue", other_rat_meas_quantities_value.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// OtherRATMeasurementQuantities-ItemIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t other_rat_meas_quantities_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {16};
  return map_enum_number(names, 1, idx, "id");
}
bool other_rat_meas_quantities_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 16 == id;
}
crit_e other_rat_meas_quantities_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 16) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
other_rat_meas_quantities_item_ies_o::value_c other_rat_meas_quantities_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 16) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e other_rat_meas_quantities_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 16) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void other_rat_meas_quantities_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE other_rat_meas_quantities_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE other_rat_meas_quantities_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* other_rat_meas_quantities_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"OtherRATMeasurementQuantities-Item"};
  return convert_enum_idx(names, 1, value, "other_rat_meas_quantities_item_ies_o::value_c::types");
}

// PRS-Measurements-Info-List-Item ::= SEQUENCE
OCUDUASN_CODE prs_meass_info_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, point_a, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(meas_prs_periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, meas_prs_offset, (uint16_t)0u, (uint16_t)159u, true, true));
  HANDLE_CODE(meas_prs_len.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_meass_info_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(point_a, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(meas_prs_periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(meas_prs_offset, bref, (uint16_t)0u, (uint16_t)159u, true, true));
  HANDLE_CODE(meas_prs_len.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_meass_info_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pointA", point_a);
  j.write_str("measPRSPeriodicity", meas_prs_periodicity.to_string());
  j.write_int("measPRSOffset", meas_prs_offset);
  j.write_str("measurementPRSLength", meas_prs_len.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* prs_meass_info_list_item_s::meas_prs_periodicity_opts::to_string() const
{
  static const char* names[] = {"ms20", "ms40", "ms80", "ms160"};
  return convert_enum_idx(names, 4, value, "prs_meass_info_list_item_s::meas_prs_periodicity_e_");
}
uint8_t prs_meass_info_list_item_s::meas_prs_periodicity_opts::to_number() const
{
  static const uint8_t numbers[] = {20, 40, 80, 160};
  return map_enum_number(numbers, 4, value, "prs_meass_info_list_item_s::meas_prs_periodicity_e_");
}

const char* prs_meass_info_list_item_s::meas_prs_len_opts::to_string() const
{
  static const char* names[] = {"ms1dot5", "ms3", "ms3dot5", "ms4", "ms5dot5", "ms6", "ms10", "ms20"};
  return convert_enum_idx(names, 8, value, "prs_meass_info_list_item_s::meas_prs_len_e_");
}
float prs_meass_info_list_item_s::meas_prs_len_opts::to_number() const
{
  static const float numbers[] = {1.5, 3.0, 3.5, 4.0, 5.5, 6.0, 10.0, 20.0};
  return map_enum_number(numbers, 8, value, "prs_meass_info_list_item_s::meas_prs_len_e_");
}
const char* prs_meass_info_list_item_s::meas_prs_len_opts::to_number_string() const
{
  static const char* number_strs[] = {"1.5", "3", "3.5", "4", "5.5", "6", "10", "20"};
  return convert_enum_idx(number_strs, 8, value, "prs_meass_info_list_item_s::meas_prs_len_e_");
}

// PRSBWAggregationRequestInfo-Item ::= SEQUENCE
OCUDUASN_CODE prsbw_aggregation_request_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, dl_prsbw_aggregation_request_info_list, 2, 3, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prsbw_aggregation_request_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(dl_prsbw_aggregation_request_info_list, bref, 2, 3, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prsbw_aggregation_request_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("dl-PRSBWAggregationRequestInfo-List");
  for (const auto& e1 : dl_prsbw_aggregation_request_info_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSConfigRequestType ::= ENUMERATED
const char* prs_cfg_request_type_opts::to_string() const
{
  static const char* names[] = {"configure", "off"};
  return convert_enum_idx(names, 2, value, "prs_cfg_request_type_e");
}

// PRSMutingOption1 ::= SEQUENCE
OCUDUASN_CODE prs_muting_option1_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(muting_pattern.pack(bref));
  HANDLE_CODE(muting_bit_repeat_factor.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_muting_option1_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(muting_pattern.unpack(bref));
  HANDLE_CODE(muting_bit_repeat_factor.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_muting_option1_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mutingPattern");
  muting_pattern.to_json(j);
  j.write_str("mutingBitRepetitionFactor", muting_bit_repeat_factor.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* prs_muting_option1_s::muting_bit_repeat_factor_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4", "n8"};
  return convert_enum_idx(names, 4, value, "prs_muting_option1_s::muting_bit_repeat_factor_e_");
}
uint8_t prs_muting_option1_s::muting_bit_repeat_factor_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 8};
  return map_enum_number(numbers, 4, value, "prs_muting_option1_s::muting_bit_repeat_factor_e_");
}

// PRSMutingOption2 ::= SEQUENCE
OCUDUASN_CODE prs_muting_option2_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(muting_pattern.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_muting_option2_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(muting_pattern.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_muting_option2_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mutingPattern");
  muting_pattern.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSMuting ::= SEQUENCE
OCUDUASN_CODE prs_muting_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(prs_muting_option1_present, 1));
  HANDLE_CODE(bref.pack(prs_muting_option2_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (prs_muting_option1_present) {
    HANDLE_CODE(prs_muting_option1.pack(bref));
  }
  if (prs_muting_option2_present) {
    HANDLE_CODE(prs_muting_option2.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_muting_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(prs_muting_option1_present, 1));
  HANDLE_CODE(bref.unpack(prs_muting_option2_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (prs_muting_option1_present) {
    HANDLE_CODE(prs_muting_option1.unpack(bref));
  }
  if (prs_muting_option2_present) {
    HANDLE_CODE(prs_muting_option2.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_muting_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (prs_muting_option1_present) {
    j.write_fieldname("pRSMutingOption1");
    prs_muting_option1.to_json(j);
  }
  if (prs_muting_option2_present) {
    j.write_fieldname("pRSMutingOption2");
    prs_muting_option2.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSResource-QCLSourceSSB ::= SEQUENCE
OCUDUASN_CODE prs_res_qcl_source_ssb_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ssb_idx_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pci_nr, (uint16_t)0u, (uint16_t)1007u, false, true));
  if (ssb_idx_present) {
    HANDLE_CODE(pack_integer(bref, ssb_idx, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_res_qcl_source_ssb_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ssb_idx_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pci_nr, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  if (ssb_idx_present) {
    HANDLE_CODE(unpack_integer(ssb_idx, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_res_qcl_source_ssb_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pCI-NR", pci_nr);
  if (ssb_idx_present) {
    j.write_int("sSB-Index", ssb_idx);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSResource-QCLSourcePRS ::= SEQUENCE
OCUDUASN_CODE prs_res_qcl_source_prs_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qcl_source_prs_res_id_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qcl_source_prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  if (qcl_source_prs_res_id_present) {
    HANDLE_CODE(pack_integer(bref, qcl_source_prs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_res_qcl_source_prs_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(qcl_source_prs_res_id_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qcl_source_prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  if (qcl_source_prs_res_id_present) {
    HANDLE_CODE(unpack_integer(qcl_source_prs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_res_qcl_source_prs_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qCLSourcePRSResourceSetID", qcl_source_prs_res_set_id);
  if (qcl_source_prs_res_id_present) {
    j.write_int("qCLSourcePRSResourceID", qcl_source_prs_res_id);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSResource-QCLInfo ::= CHOICE
void prs_res_qcl_info_c::destroy_()
{
  switch (type_) {
    case types::qcl_source_ssb:
      c.destroy<prs_res_qcl_source_ssb_s>();
      break;
    case types::qcl_source_prs:
      c.destroy<prs_res_qcl_source_prs_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void prs_res_qcl_info_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::qcl_source_ssb:
      c.init<prs_res_qcl_source_ssb_s>();
      break;
    case types::qcl_source_prs:
      c.init<prs_res_qcl_source_prs_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_res_qcl_info_c");
  }
}
prs_res_qcl_info_c::prs_res_qcl_info_c(const prs_res_qcl_info_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::qcl_source_ssb:
      c.init(other.c.get<prs_res_qcl_source_ssb_s>());
      break;
    case types::qcl_source_prs:
      c.init(other.c.get<prs_res_qcl_source_prs_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_res_qcl_info_c");
  }
}
prs_res_qcl_info_c& prs_res_qcl_info_c::operator=(const prs_res_qcl_info_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::qcl_source_ssb:
      c.set(other.c.get<prs_res_qcl_source_ssb_s>());
      break;
    case types::qcl_source_prs:
      c.set(other.c.get<prs_res_qcl_source_prs_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_res_qcl_info_c");
  }

  return *this;
}
prs_res_qcl_source_ssb_s& prs_res_qcl_info_c::set_qcl_source_ssb()
{
  set(types::qcl_source_ssb);
  return c.get<prs_res_qcl_source_ssb_s>();
}
prs_res_qcl_source_prs_s& prs_res_qcl_info_c::set_qcl_source_prs()
{
  set(types::qcl_source_prs);
  return c.get<prs_res_qcl_source_prs_s>();
}
protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>& prs_res_qcl_info_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>();
}
void prs_res_qcl_info_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::qcl_source_ssb:
      j.write_fieldname("qCLSourceSSB");
      c.get<prs_res_qcl_source_ssb_s>().to_json(j);
      break;
    case types::qcl_source_prs:
      j.write_fieldname("qCLSourcePRS");
      c.get<prs_res_qcl_source_prs_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "prs_res_qcl_info_c");
  }
  j.end_obj();
}
OCUDUASN_CODE prs_res_qcl_info_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::qcl_source_ssb:
      HANDLE_CODE(c.get<prs_res_qcl_source_ssb_s>().pack(bref));
      break;
    case types::qcl_source_prs:
      HANDLE_CODE(c.get<prs_res_qcl_source_prs_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_res_qcl_info_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_res_qcl_info_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::qcl_source_ssb:
      HANDLE_CODE(c.get<prs_res_qcl_source_ssb_s>().unpack(bref));
      break;
    case types::qcl_source_prs:
      HANDLE_CODE(c.get<prs_res_qcl_source_prs_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_res_qcl_info_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* prs_res_qcl_info_c::types_opts::to_string() const
{
  static const char* names[] = {"qCLSourceSSB", "qCLSourcePRS", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "prs_res_qcl_info_c::types");
}

// PRSResource-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t prs_res_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {112};
  return map_enum_number(names, 1, idx, "id");
}
bool prs_res_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 112 == id;
}
crit_e prs_res_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 112) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
prs_res_item_ext_ies_o::ext_c prs_res_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 112) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e prs_res_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 112) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void prs_res_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_int(c);
}
OCUDUASN_CODE prs_res_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_integer(bref, c, (uint8_t)0u, (uint8_t)13u, true, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_res_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_integer(c, bref, (uint8_t)0u, (uint8_t)13u, true, true));
  return OCUDUASN_SUCCESS;
}

const char* prs_res_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..13,...)"};
  return convert_enum_idx(names, 1, value, "prs_res_item_ext_ies_o::ext_c::types");
}
uint8_t prs_res_item_ext_ies_o::ext_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "prs_res_item_ext_ies_o::ext_c::types");
}

// PRSResource-Item ::= SEQUENCE
OCUDUASN_CODE prs_res_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qcl_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, prs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(pack_integer(bref, seq_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(pack_integer(bref, re_offset, (uint8_t)0u, (uint8_t)11u, true, true));
  HANDLE_CODE(pack_integer(bref, res_slot_offset, (uint16_t)0u, (uint16_t)511u, false, true));
  HANDLE_CODE(pack_integer(bref, res_symbol_offset, (uint8_t)0u, (uint8_t)12u, false, true));
  if (qcl_info_present) {
    HANDLE_CODE(qcl_info.pack(bref));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_res_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(qcl_info_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(unpack_integer(seq_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(unpack_integer(re_offset, bref, (uint8_t)0u, (uint8_t)11u, true, true));
  HANDLE_CODE(unpack_integer(res_slot_offset, bref, (uint16_t)0u, (uint16_t)511u, false, true));
  HANDLE_CODE(unpack_integer(res_symbol_offset, bref, (uint8_t)0u, (uint8_t)12u, false, true));
  if (qcl_info_present) {
    HANDLE_CODE(qcl_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_res_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pRSResourceID", prs_res_id);
  j.write_int("sequenceID", seq_id);
  j.write_int("rEOffset", re_offset);
  j.write_int("resourceSlotOffset", res_slot_offset);
  j.write_int("resourceSymbolOffset", res_symbol_offset);
  if (qcl_info_present) {
    j.write_fieldname("qCLInfo");
    qcl_info.to_json(j);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSResourceSet-Item ::= SEQUENCE
OCUDUASN_CODE prs_res_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(prs_muting_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(subcarrier_spacing.pack(bref));
  HANDLE_CODE(pack_integer(bref, pr_sbw, (uint8_t)1u, (uint8_t)63u, false, true));
  HANDLE_CODE(pack_integer(bref, start_prb, (uint16_t)0u, (uint16_t)2176u, false, true));
  HANDLE_CODE(pack_integer(bref, point_a, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(comb_size.pack(bref));
  HANDLE_CODE(cp_type.pack(bref));
  HANDLE_CODE(res_set_periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, res_set_slot_offset, (uint32_t)0u, (uint32_t)81919u, true, true));
  HANDLE_CODE(res_repeat_factor.pack(bref));
  HANDLE_CODE(res_time_gap.pack(bref));
  HANDLE_CODE(res_numof_symbols.pack(bref));
  if (prs_muting_present) {
    HANDLE_CODE(prs_muting.pack(bref));
  }
  HANDLE_CODE(pack_integer(bref, prs_res_tx_pwr, (int8_t)-60, (int8_t)50, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, prs_res_list, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_res_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(prs_muting_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(subcarrier_spacing.unpack(bref));
  HANDLE_CODE(unpack_integer(pr_sbw, bref, (uint8_t)1u, (uint8_t)63u, false, true));
  HANDLE_CODE(unpack_integer(start_prb, bref, (uint16_t)0u, (uint16_t)2176u, false, true));
  HANDLE_CODE(unpack_integer(point_a, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(comb_size.unpack(bref));
  HANDLE_CODE(cp_type.unpack(bref));
  HANDLE_CODE(res_set_periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(res_set_slot_offset, bref, (uint32_t)0u, (uint32_t)81919u, true, true));
  HANDLE_CODE(res_repeat_factor.unpack(bref));
  HANDLE_CODE(res_time_gap.unpack(bref));
  HANDLE_CODE(res_numof_symbols.unpack(bref));
  if (prs_muting_present) {
    HANDLE_CODE(prs_muting.unpack(bref));
  }
  HANDLE_CODE(unpack_integer(prs_res_tx_pwr, bref, (int8_t)-60, (int8_t)50, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(prs_res_list, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_res_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pRSResourceSetID", prs_res_set_id);
  j.write_str("subcarrierSpacing", subcarrier_spacing.to_string());
  j.write_int("pRSbandwidth", pr_sbw);
  j.write_int("startPRB", start_prb);
  j.write_int("pointA", point_a);
  j.write_str("combSize", comb_size.to_string());
  j.write_str("cPType", cp_type.to_string());
  j.write_str("resourceSetPeriodicity", res_set_periodicity.to_string());
  j.write_int("resourceSetSlotOffset", res_set_slot_offset);
  j.write_str("resourceRepetitionFactor", res_repeat_factor.to_string());
  j.write_str("resourceTimeGap", res_time_gap.to_string());
  j.write_str("resourceNumberofSymbols", res_numof_symbols.to_string());
  if (prs_muting_present) {
    j.write_fieldname("pRSMuting");
    prs_muting.to_json(j);
  }
  j.write_int("pRSResourceTransmitPower", prs_res_tx_pwr);
  j.start_array("pRSResource-List");
  for (const auto& e1 : prs_res_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* prs_res_set_item_s::subcarrier_spacing_opts::to_string() const
{
  static const char* names[] = {"kHz15", "kHz30", "kHz60", "kHz120"};
  return convert_enum_idx(names, 4, value, "prs_res_set_item_s::subcarrier_spacing_e_");
}
uint8_t prs_res_set_item_s::subcarrier_spacing_opts::to_number() const
{
  static const uint8_t numbers[] = {15, 30, 60, 120};
  return map_enum_number(numbers, 4, value, "prs_res_set_item_s::subcarrier_spacing_e_");
}

const char* prs_res_set_item_s::comb_size_opts::to_string() const
{
  static const char* names[] = {"n2", "n4", "n6", "n12"};
  return convert_enum_idx(names, 4, value, "prs_res_set_item_s::comb_size_e_");
}
uint8_t prs_res_set_item_s::comb_size_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 6, 12};
  return map_enum_number(numbers, 4, value, "prs_res_set_item_s::comb_size_e_");
}

const char* prs_res_set_item_s::cp_type_opts::to_string() const
{
  static const char* names[] = {"normal", "extended"};
  return convert_enum_idx(names, 2, value, "prs_res_set_item_s::cp_type_e_");
}

const char* prs_res_set_item_s::res_set_periodicity_opts::to_string() const
{
  static const char* names[] = {"n4",     "n5",     "n8",     "n10",    "n16",  "n20",   "n32",   "n40",
                                "n64",    "n80",    "n160",   "n320",   "n640", "n1280", "n2560", "n5120",
                                "n10240", "n20480", "n40960", "n81920", "n128", "n256",  "n512"};
  return convert_enum_idx(names, 23, value, "prs_res_set_item_s::res_set_periodicity_e_");
}
uint32_t prs_res_set_item_s::res_set_periodicity_opts::to_number() const
{
  static const uint32_t numbers[] = {4,   5,    8,    10,   16,    20,    32,    40,    64,  80,  160, 320,
                                     640, 1280, 2560, 5120, 10240, 20480, 40960, 81920, 128, 256, 512};
  return map_enum_number(numbers, 23, value, "prs_res_set_item_s::res_set_periodicity_e_");
}

const char* prs_res_set_item_s::res_repeat_factor_opts::to_string() const
{
  static const char* names[] = {"rf1", "rf2", "rf4", "rf6", "rf8", "rf16", "rf32"};
  return convert_enum_idx(names, 7, value, "prs_res_set_item_s::res_repeat_factor_e_");
}
uint8_t prs_res_set_item_s::res_repeat_factor_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 6, 8, 16, 32};
  return map_enum_number(numbers, 7, value, "prs_res_set_item_s::res_repeat_factor_e_");
}

const char* prs_res_set_item_s::res_time_gap_opts::to_string() const
{
  static const char* names[] = {"tg1", "tg2", "tg4", "tg8", "tg16", "tg32"};
  return convert_enum_idx(names, 6, value, "prs_res_set_item_s::res_time_gap_e_");
}
uint8_t prs_res_set_item_s::res_time_gap_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 8, 16, 32};
  return map_enum_number(numbers, 6, value, "prs_res_set_item_s::res_time_gap_e_");
}

const char* prs_res_set_item_s::res_numof_symbols_opts::to_string() const
{
  static const char* names[] = {"n2", "n4", "n6", "n12", "n1"};
  return convert_enum_idx(names, 5, value, "prs_res_set_item_s::res_numof_symbols_e_");
}
uint8_t prs_res_set_item_s::res_numof_symbols_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 6, 12, 1};
  return map_enum_number(numbers, 5, value, "prs_res_set_item_s::res_numof_symbols_e_");
}

// PRSConfiguration-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t prs_cfg_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {132};
  return map_enum_number(names, 1, idx, "id");
}
bool prs_cfg_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 132 == id;
}
crit_e prs_cfg_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 132) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
prs_cfg_ext_ies_o::ext_c prs_cfg_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 132) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e prs_cfg_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 132) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void prs_cfg_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.start_array("");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
}
OCUDUASN_CODE prs_cfg_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 2, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 2, true));
  return OCUDUASN_SUCCESS;
}

const char* prs_cfg_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"AggregatedPRSResourceSetList"};
  return convert_enum_idx(names, 1, value, "prs_cfg_ext_ies_o::ext_c::types");
}

// PRSConfiguration ::= SEQUENCE
OCUDUASN_CODE prs_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, prs_res_set_list, 1, 8, true));
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(prs_res_set_list, bref, 1, 8, true));
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("pRSResourceSet-List");
  for (const auto& e1 : prs_res_set_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RequestedDLPRSResource-Item ::= SEQUENCE
OCUDUASN_CODE requested_dl_prs_res_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qcl_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (qcl_info_present) {
    HANDLE_CODE(qcl_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE requested_dl_prs_res_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(qcl_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (qcl_info_present) {
    HANDLE_CODE(qcl_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void requested_dl_prs_res_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (qcl_info_present) {
    j.write_fieldname("qCLInfo");
    qcl_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// StartTimeAndDuration ::= SEQUENCE
OCUDUASN_CODE start_time_and_dur_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(start_time_present, 1));
  HANDLE_CODE(bref.pack(dur_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (start_time_present) {
    HANDLE_CODE(start_time.pack(bref));
  }
  if (dur_present) {
    HANDLE_CODE(pack_integer(bref, dur, (uint32_t)0u, (uint32_t)90060u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE start_time_and_dur_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(start_time_present, 1));
  HANDLE_CODE(bref.unpack(dur_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (start_time_present) {
    HANDLE_CODE(start_time.unpack(bref));
  }
  if (dur_present) {
    HANDLE_CODE(unpack_integer(dur, bref, (uint32_t)0u, (uint32_t)90060u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void start_time_and_dur_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (start_time_present) {
    j.write_fieldname("startTime");
    start_time.to_json(j);
  }
  if (dur_present) {
    j.write_int("duration", dur);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RequestedDLPRSResourceSet-Item ::= SEQUENCE
OCUDUASN_CODE requested_dl_prs_res_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pr_sbw_present, 1));
  HANDLE_CODE(bref.pack(comb_size_present, 1));
  HANDLE_CODE(bref.pack(res_set_periodicity_present, 1));
  HANDLE_CODE(bref.pack(res_repeat_factor_present, 1));
  HANDLE_CODE(bref.pack(res_numof_symbols_present, 1));
  HANDLE_CODE(bref.pack(requested_dl_prs_res_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(res_set_start_time_and_dur_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (pr_sbw_present) {
    HANDLE_CODE(pack_integer(bref, pr_sbw, (uint8_t)1u, (uint8_t)63u, false, true));
  }
  if (comb_size_present) {
    HANDLE_CODE(comb_size.pack(bref));
  }
  if (res_set_periodicity_present) {
    HANDLE_CODE(res_set_periodicity.pack(bref));
  }
  if (res_repeat_factor_present) {
    HANDLE_CODE(res_repeat_factor.pack(bref));
  }
  if (res_numof_symbols_present) {
    HANDLE_CODE(res_numof_symbols.pack(bref));
  }
  if (requested_dl_prs_res_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, requested_dl_prs_res_list, 1, 64, true));
  }
  if (res_set_start_time_and_dur_present) {
    HANDLE_CODE(res_set_start_time_and_dur.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE requested_dl_prs_res_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(pr_sbw_present, 1));
  HANDLE_CODE(bref.unpack(comb_size_present, 1));
  HANDLE_CODE(bref.unpack(res_set_periodicity_present, 1));
  HANDLE_CODE(bref.unpack(res_repeat_factor_present, 1));
  HANDLE_CODE(bref.unpack(res_numof_symbols_present, 1));
  bool requested_dl_prs_res_list_present;
  HANDLE_CODE(bref.unpack(requested_dl_prs_res_list_present, 1));
  HANDLE_CODE(bref.unpack(res_set_start_time_and_dur_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (pr_sbw_present) {
    HANDLE_CODE(unpack_integer(pr_sbw, bref, (uint8_t)1u, (uint8_t)63u, false, true));
  }
  if (comb_size_present) {
    HANDLE_CODE(comb_size.unpack(bref));
  }
  if (res_set_periodicity_present) {
    HANDLE_CODE(res_set_periodicity.unpack(bref));
  }
  if (res_repeat_factor_present) {
    HANDLE_CODE(res_repeat_factor.unpack(bref));
  }
  if (res_numof_symbols_present) {
    HANDLE_CODE(res_numof_symbols.unpack(bref));
  }
  if (requested_dl_prs_res_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(requested_dl_prs_res_list, bref, 1, 64, true));
  }
  if (res_set_start_time_and_dur_present) {
    HANDLE_CODE(res_set_start_time_and_dur.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void requested_dl_prs_res_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pr_sbw_present) {
    j.write_int("pRSbandwidth", pr_sbw);
  }
  if (comb_size_present) {
    j.write_str("combSize", comb_size.to_string());
  }
  if (res_set_periodicity_present) {
    j.write_str("resourceSetPeriodicity", res_set_periodicity.to_string());
  }
  if (res_repeat_factor_present) {
    j.write_str("resourceRepetitionFactor", res_repeat_factor.to_string());
  }
  if (res_numof_symbols_present) {
    j.write_str("resourceNumberofSymbols", res_numof_symbols.to_string());
  }
  if (requested_dl_prs_res_list.size() > 0) {
    j.start_array("requestedDLPRSResource-List");
    for (const auto& e1 : requested_dl_prs_res_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (res_set_start_time_and_dur_present) {
    j.write_fieldname("resourceSetStartTimeAndDuration");
    res_set_start_time_and_dur.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* requested_dl_prs_res_set_item_s::comb_size_opts::to_string() const
{
  static const char* names[] = {"n2", "n4", "n6", "n12"};
  return convert_enum_idx(names, 4, value, "requested_dl_prs_res_set_item_s::comb_size_e_");
}
uint8_t requested_dl_prs_res_set_item_s::comb_size_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 6, 12};
  return map_enum_number(numbers, 4, value, "requested_dl_prs_res_set_item_s::comb_size_e_");
}

const char* requested_dl_prs_res_set_item_s::res_set_periodicity_opts::to_string() const
{
  static const char* names[] = {"n4",     "n5",     "n8",     "n10",    "n16",  "n20",   "n32",   "n40",
                                "n64",    "n80",    "n160",   "n320",   "n640", "n1280", "n2560", "n5120",
                                "n10240", "n20480", "n40960", "n81920", "n128", "n256",  "n512"};
  return convert_enum_idx(names, 23, value, "requested_dl_prs_res_set_item_s::res_set_periodicity_e_");
}
uint32_t requested_dl_prs_res_set_item_s::res_set_periodicity_opts::to_number() const
{
  static const uint32_t numbers[] = {4,   5,    8,    10,   16,    20,    32,    40,    64,  80,  160, 320,
                                     640, 1280, 2560, 5120, 10240, 20480, 40960, 81920, 128, 256, 512};
  return map_enum_number(numbers, 23, value, "requested_dl_prs_res_set_item_s::res_set_periodicity_e_");
}

const char* requested_dl_prs_res_set_item_s::res_repeat_factor_opts::to_string() const
{
  static const char* names[] = {"rf1", "rf2", "rf4", "rf6", "rf8", "rf16", "rf32"};
  return convert_enum_idx(names, 7, value, "requested_dl_prs_res_set_item_s::res_repeat_factor_e_");
}
uint8_t requested_dl_prs_res_set_item_s::res_repeat_factor_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 6, 8, 16, 32};
  return map_enum_number(numbers, 7, value, "requested_dl_prs_res_set_item_s::res_repeat_factor_e_");
}

const char* requested_dl_prs_res_set_item_s::res_numof_symbols_opts::to_string() const
{
  static const char* names[] = {"n2", "n4", "n6", "n12", "n1"};
  return convert_enum_idx(names, 5, value, "requested_dl_prs_res_set_item_s::res_numof_symbols_e_");
}
uint8_t requested_dl_prs_res_set_item_s::res_numof_symbols_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 6, 12, 1};
  return map_enum_number(numbers, 5, value, "requested_dl_prs_res_set_item_s::res_numof_symbols_e_");
}

// RequestedDLPRSTransmissionCharacteristics-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t requested_dl_prs_tx_characteristics_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {130};
  return map_enum_number(names, 1, idx, "id");
}
bool requested_dl_prs_tx_characteristics_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 130 == id;
}
crit_e requested_dl_prs_tx_characteristics_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 130) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
requested_dl_prs_tx_characteristics_ext_ies_o::ext_c
requested_dl_prs_tx_characteristics_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 130) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e requested_dl_prs_tx_characteristics_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 130) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void requested_dl_prs_tx_characteristics_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.start_array("");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
}
OCUDUASN_CODE requested_dl_prs_tx_characteristics_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 2, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE requested_dl_prs_tx_characteristics_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 2, true));
  return OCUDUASN_SUCCESS;
}

const char* requested_dl_prs_tx_characteristics_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"PRSBWAggregationRequestInfoList"};
  return convert_enum_idx(names, 1, value, "requested_dl_prs_tx_characteristics_ext_ies_o::ext_c::types");
}

// RequestedDLPRSTransmissionCharacteristics ::= SEQUENCE
OCUDUASN_CODE requested_dl_prs_tx_characteristics_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(numof_freq_layers_present, 1));
  HANDLE_CODE(bref.pack(start_time_and_dur_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, requested_dl_prs_res_set_list, 1, 8, true));
  if (numof_freq_layers_present) {
    HANDLE_CODE(pack_integer(bref, numof_freq_layers, (uint8_t)1u, (uint8_t)4u, false, true));
  }
  if (start_time_and_dur_present) {
    HANDLE_CODE(start_time_and_dur.pack(bref));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE requested_dl_prs_tx_characteristics_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(numof_freq_layers_present, 1));
  HANDLE_CODE(bref.unpack(start_time_and_dur_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(requested_dl_prs_res_set_list, bref, 1, 8, true));
  if (numof_freq_layers_present) {
    HANDLE_CODE(unpack_integer(numof_freq_layers, bref, (uint8_t)1u, (uint8_t)4u, false, true));
  }
  if (start_time_and_dur_present) {
    HANDLE_CODE(start_time_and_dur.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void requested_dl_prs_tx_characteristics_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("requestedDLPRSResourceSet-List");
  for (const auto& e1 : requested_dl_prs_res_set_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (numof_freq_layers_present) {
    j.write_int("numberofFrequencyLayers", numof_freq_layers);
  }
  if (start_time_and_dur_present) {
    j.write_fieldname("startTimeAndDuration");
    start_time_and_dur.to_json(j);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSTransmissionOffPerResourceSet-Item ::= SEQUENCE
OCUDUASN_CODE prs_tx_off_per_res_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_tx_off_per_res_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_tx_off_per_res_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pRSResourceSetID", prs_res_set_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSTransmissionOffIndicationPerResource-Item ::= SEQUENCE
OCUDUASN_CODE prs_tx_off_ind_per_res_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, prs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_tx_off_ind_per_res_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_tx_off_ind_per_res_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pRSResourceID", prs_res_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSTransmissionOffPerResource-Item ::= SEQUENCE
OCUDUASN_CODE prs_tx_off_per_res_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, prs_tx_off_ind_per_res_list, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_tx_off_per_res_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(prs_tx_off_ind_per_res_list, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_tx_off_per_res_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pRSResourceSetID", prs_res_set_id);
  j.start_array("pRSTransmissionOffIndicationPerResourceList");
  for (const auto& e1 : prs_tx_off_ind_per_res_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSTransmissionOffIndication ::= CHOICE
void prs_tx_off_ind_c::destroy_()
{
  switch (type_) {
    case types::prs_tx_off_per_res_set:
      c.destroy<prs_tx_off_per_res_set_l>();
      break;
    case types::prs_tx_off_per_res:
      c.destroy<prs_tx_off_per_res_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void prs_tx_off_ind_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::prs_tx_off_per_trp:
      break;
    case types::prs_tx_off_per_res_set:
      c.init<prs_tx_off_per_res_set_l>();
      break;
    case types::prs_tx_off_per_res:
      c.init<prs_tx_off_per_res_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_tx_off_ind_c");
  }
}
prs_tx_off_ind_c::prs_tx_off_ind_c(const prs_tx_off_ind_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::prs_tx_off_per_trp:
      break;
    case types::prs_tx_off_per_res_set:
      c.init(other.c.get<prs_tx_off_per_res_set_l>());
      break;
    case types::prs_tx_off_per_res:
      c.init(other.c.get<prs_tx_off_per_res_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_tx_off_ind_c");
  }
}
prs_tx_off_ind_c& prs_tx_off_ind_c::operator=(const prs_tx_off_ind_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::prs_tx_off_per_trp:
      break;
    case types::prs_tx_off_per_res_set:
      c.set(other.c.get<prs_tx_off_per_res_set_l>());
      break;
    case types::prs_tx_off_per_res:
      c.set(other.c.get<prs_tx_off_per_res_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_tx_off_ind_c");
  }

  return *this;
}
void prs_tx_off_ind_c::set_prs_tx_off_per_trp()
{
  set(types::prs_tx_off_per_trp);
}
prs_tx_off_per_res_set_l& prs_tx_off_ind_c::set_prs_tx_off_per_res_set()
{
  set(types::prs_tx_off_per_res_set);
  return c.get<prs_tx_off_per_res_set_l>();
}
prs_tx_off_per_res_l& prs_tx_off_ind_c::set_prs_tx_off_per_res()
{
  set(types::prs_tx_off_per_res);
  return c.get<prs_tx_off_per_res_l>();
}
protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>& prs_tx_off_ind_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>();
}
void prs_tx_off_ind_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::prs_tx_off_per_trp:
      j.write_null("pRSTransmissionOffPerTRP");
      break;
    case types::prs_tx_off_per_res_set:
      j.start_array("pRSTransmissionOffPerResourceSet");
      for (const auto& e1 : c.get<prs_tx_off_per_res_set_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::prs_tx_off_per_res:
      j.start_array("pRSTransmissionOffPerResource");
      for (const auto& e1 : c.get<prs_tx_off_per_res_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "prs_tx_off_ind_c");
  }
  j.end_obj();
}
OCUDUASN_CODE prs_tx_off_ind_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::prs_tx_off_per_trp:
      break;
    case types::prs_tx_off_per_res_set:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<prs_tx_off_per_res_set_l>(), 1, 8, true));
      break;
    case types::prs_tx_off_per_res:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<prs_tx_off_per_res_l>(), 1, 8, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_tx_off_ind_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_tx_off_ind_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::prs_tx_off_per_trp:
      break;
    case types::prs_tx_off_per_res_set:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<prs_tx_off_per_res_set_l>(), bref, 1, 8, true));
      break;
    case types::prs_tx_off_per_res:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<prs_tx_off_per_res_l>(), bref, 1, 8, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_tx_off_ind_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* prs_tx_off_ind_c::types_opts::to_string() const
{
  static const char* names[] = {"pRSTransmissionOffPerTRP",
                                "pRSTransmissionOffPerResourceSet",
                                "pRSTransmissionOffPerResource",
                                "choice-Extension"};
  return convert_enum_idx(names, 4, value, "prs_tx_off_ind_c::types");
}

// PRSTransmissionOffInformation ::= SEQUENCE
OCUDUASN_CODE prs_tx_off_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(prs_tx_off_ind.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_tx_off_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(prs_tx_off_ind.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_tx_off_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("pRSTransmissionOffIndication");
  prs_tx_off_ind.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSTRPItem ::= SEQUENCE
OCUDUASN_CODE prstrp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(requested_dl_prs_tx_characteristics_present, 1));
  HANDLE_CODE(bref.pack(prs_tx_off_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_id, (uint32_t)1u, (uint32_t)65535u, true, true));
  if (requested_dl_prs_tx_characteristics_present) {
    HANDLE_CODE(requested_dl_prs_tx_characteristics.pack(bref));
  }
  if (prs_tx_off_info_present) {
    HANDLE_CODE(prs_tx_off_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prstrp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(requested_dl_prs_tx_characteristics_present, 1));
  HANDLE_CODE(bref.unpack(prs_tx_off_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_id, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  if (requested_dl_prs_tx_characteristics_present) {
    HANDLE_CODE(requested_dl_prs_tx_characteristics.unpack(bref));
  }
  if (prs_tx_off_info_present) {
    HANDLE_CODE(prs_tx_off_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prstrp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-ID", trp_id);
  if (requested_dl_prs_tx_characteristics_present) {
    j.write_fieldname("requestedDLPRSTransmissionCharacteristics");
    requested_dl_prs_tx_characteristics.to_json(j);
  }
  if (prs_tx_off_info_present) {
    j.write_fieldname("pRSTransmissionOffInformation");
    prs_tx_off_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PRSTransmissionTRPItem ::= SEQUENCE
OCUDUASN_CODE prs_tx_trp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_id, (uint32_t)1u, (uint32_t)65535u, true, true));
  HANDLE_CODE(prs_cfg.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_tx_trp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_id, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  HANDLE_CODE(prs_cfg.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void prs_tx_trp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-ID", trp_id);
  j.write_fieldname("pRSConfiguration");
  prs_cfg.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PathlossReferenceSignal ::= CHOICE
void pathloss_ref_sig_c::destroy_()
{
  switch (type_) {
    case types::ssb_ref:
      c.destroy<ssb_s>();
      break;
    case types::dl_prs_ref:
      c.destroy<dl_prs_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>();
      break;
    default:
      break;
  }
}
void pathloss_ref_sig_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ssb_ref:
      c.init<ssb_s>();
      break;
    case types::dl_prs_ref:
      c.init<dl_prs_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pathloss_ref_sig_c");
  }
}
pathloss_ref_sig_c::pathloss_ref_sig_c(const pathloss_ref_sig_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ssb_ref:
      c.init(other.c.get<ssb_s>());
      break;
    case types::dl_prs_ref:
      c.init(other.c.get<dl_prs_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pathloss_ref_sig_c");
  }
}
pathloss_ref_sig_c& pathloss_ref_sig_c::operator=(const pathloss_ref_sig_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ssb_ref:
      c.set(other.c.get<ssb_s>());
      break;
    case types::dl_prs_ref:
      c.set(other.c.get<dl_prs_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pathloss_ref_sig_c");
  }

  return *this;
}
ssb_s& pathloss_ref_sig_c::set_ssb_ref()
{
  set(types::ssb_ref);
  return c.get<ssb_s>();
}
dl_prs_s& pathloss_ref_sig_c::set_dl_prs_ref()
{
  set(types::dl_prs_ref);
  return c.get<dl_prs_s>();
}
protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>& pathloss_ref_sig_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>();
}
void pathloss_ref_sig_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ssb_ref:
      j.write_fieldname("sSB-Reference");
      c.get<ssb_s>().to_json(j);
      break;
    case types::dl_prs_ref:
      j.write_fieldname("dL-PRS-Reference");
      c.get<dl_prs_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "pathloss_ref_sig_c");
  }
  j.end_obj();
}
OCUDUASN_CODE pathloss_ref_sig_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ssb_ref:
      HANDLE_CODE(c.get<ssb_s>().pack(bref));
      break;
    case types::dl_prs_ref:
      HANDLE_CODE(c.get<dl_prs_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "pathloss_ref_sig_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pathloss_ref_sig_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ssb_ref:
      HANDLE_CODE(c.get<ssb_s>().unpack(bref));
      break;
    case types::dl_prs_ref:
      HANDLE_CODE(c.get<dl_prs_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "pathloss_ref_sig_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* pathloss_ref_sig_c::types_opts::to_string() const
{
  static const char* names[] = {"sSB-Reference", "dL-PRS-Reference", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "pathloss_ref_sig_c::types");
}

// PathlossReferenceInformation ::= SEQUENCE
OCUDUASN_CODE pathloss_ref_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pathloss_ref_sig.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pathloss_ref_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(pathloss_ref_sig.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pathloss_ref_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("pathlossReferenceSignal");
  pathloss_ref_sig.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PeriodicityItem ::= ENUMERATED
const char* periodicity_item_opts::to_string() const
{
  static const char* names[] = {"ms0dot125", "ms0dot25", "ms0dot5", "ms0dot625", "ms1",   "ms1dot25", "ms2",
                                "ms2dot5",   "ms4dot",   "ms5",     "ms8",       "ms10",  "ms16",     "ms20",
                                "ms32",      "ms40",     "ms64",    "ms80m",     "ms160", "ms320",    "ms640m",
                                "ms1280",    "ms2560",   "ms5120",  "ms10240"};
  return convert_enum_idx(names, 25, value, "periodicity_item_e");
}
float periodicity_item_opts::to_number() const
{
  static const float numbers[] = {0.125, 0.25,  0.5,   0.625,  1.0,    1.25,   2.0,    2.5,  4.0,
                                  5.0,   8.0,   10.0,  16.0,   20.0,   32.0,   40.0,   64.0, 80.0,
                                  160.0, 320.0, 640.0, 1280.0, 2560.0, 5120.0, 10240.0};
  return map_enum_number(numbers, 25, value, "periodicity_item_e");
}
const char* periodicity_item_opts::to_number_string() const
{
  static const char* number_strs[] = {"0.125", "0.25", "0.5", "0.625", "1",    "1.25", "2",    "2.5", "4",
                                      "5",     "8",    "10",  "16",    "20",   "32",   "40",   "64",  "80",
                                      "160",   "320",  "640", "1280",  "2560", "5120", "10240"};
  return convert_enum_idx(number_strs, 25, value, "periodicity_item_e");
}

// PosSRSInfo ::= SEQUENCE
OCUDUASN_CODE pos_srs_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, pos_srs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_srs_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(pos_srs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_srs_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("posSRSResourceID", pos_srs_res_id);
  j.end_obj();
}

// PosValidityAreaCell-Item ::= SEQUENCE
OCUDUASN_CODE pos_validity_area_cell_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(nr_pci_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.pack(bref));
  if (nr_pci_present) {
    HANDLE_CODE(pack_integer(bref, nr_pci, (uint16_t)0u, (uint16_t)1007u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_validity_area_cell_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(nr_pci_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.unpack(bref));
  if (nr_pci_present) {
    HANDLE_CODE(unpack_integer(nr_pci, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pos_validity_area_cell_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("nR-CGI");
  nr_cgi.to_json(j);
  if (nr_pci_present) {
    j.write_int("nR-PCI", nr_pci);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ReferenceSignal ::= CHOICE
void ref_sig_c::destroy_()
{
  switch (type_) {
    case types::ssb:
      c.destroy<ssb_s>();
      break;
    case types::dl_prs:
      c.destroy<dl_prs_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<ref_sig_ext_ie_o>>();
      break;
    default:
      break;
  }
}
void ref_sig_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::nzp_csi_rs:
      break;
    case types::ssb:
      c.init<ssb_s>();
      break;
    case types::srs:
      break;
    case types::positioning_srs:
      break;
    case types::dl_prs:
      c.init<dl_prs_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<ref_sig_ext_ie_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ref_sig_c");
  }
}
ref_sig_c::ref_sig_c(const ref_sig_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::nzp_csi_rs:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ssb:
      c.init(other.c.get<ssb_s>());
      break;
    case types::srs:
      c.init(other.c.get<uint8_t>());
      break;
    case types::positioning_srs:
      c.init(other.c.get<uint8_t>());
      break;
    case types::dl_prs:
      c.init(other.c.get<dl_prs_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<ref_sig_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ref_sig_c");
  }
}
ref_sig_c& ref_sig_c::operator=(const ref_sig_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::nzp_csi_rs:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ssb:
      c.set(other.c.get<ssb_s>());
      break;
    case types::srs:
      c.set(other.c.get<uint8_t>());
      break;
    case types::positioning_srs:
      c.set(other.c.get<uint8_t>());
      break;
    case types::dl_prs:
      c.set(other.c.get<dl_prs_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<ref_sig_ext_ie_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ref_sig_c");
  }

  return *this;
}
uint8_t& ref_sig_c::set_nzp_csi_rs()
{
  set(types::nzp_csi_rs);
  return c.get<uint8_t>();
}
ssb_s& ref_sig_c::set_ssb()
{
  set(types::ssb);
  return c.get<ssb_s>();
}
uint8_t& ref_sig_c::set_srs()
{
  set(types::srs);
  return c.get<uint8_t>();
}
uint8_t& ref_sig_c::set_positioning_srs()
{
  set(types::positioning_srs);
  return c.get<uint8_t>();
}
dl_prs_s& ref_sig_c::set_dl_prs()
{
  set(types::dl_prs);
  return c.get<dl_prs_s>();
}
protocol_ie_single_container_s<ref_sig_ext_ie_o>& ref_sig_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<ref_sig_ext_ie_o>>();
}
void ref_sig_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::nzp_csi_rs:
      j.write_int("nZP-CSI-RS", c.get<uint8_t>());
      break;
    case types::ssb:
      j.write_fieldname("sSB");
      c.get<ssb_s>().to_json(j);
      break;
    case types::srs:
      j.write_int("sRS", c.get<uint8_t>());
      break;
    case types::positioning_srs:
      j.write_int("positioningSRS", c.get<uint8_t>());
      break;
    case types::dl_prs:
      j.write_fieldname("dL-PRS");
      c.get<dl_prs_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<ref_sig_ext_ie_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ref_sig_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ref_sig_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::nzp_csi_rs:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)191u, false, true));
      break;
    case types::ssb:
      HANDLE_CODE(c.get<ssb_s>().pack(bref));
      break;
    case types::srs:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)63u, false, true));
      break;
    case types::positioning_srs:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)63u, false, true));
      break;
    case types::dl_prs:
      HANDLE_CODE(c.get<dl_prs_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ref_sig_ext_ie_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ref_sig_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ref_sig_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::nzp_csi_rs:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)191u, false, true));
      break;
    case types::ssb:
      HANDLE_CODE(c.get<ssb_s>().unpack(bref));
      break;
    case types::srs:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)63u, false, true));
      break;
    case types::positioning_srs:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)63u, false, true));
      break;
    case types::dl_prs:
      HANDLE_CODE(c.get<dl_prs_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ref_sig_ext_ie_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ref_sig_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ref_sig_c::types_opts::to_string() const
{
  static const char* names[] = {"nZP-CSI-RS", "sSB", "sRS", "positioningSRS", "dL-PRS", "choice-Extension"};
  return convert_enum_idx(names, 6, value, "ref_sig_c::types");
}

// Remote-UE-Indication ::= ENUMERATED
const char* remote_ue_ind_opts::to_string() const
{
  static const char* names[] = {"l2-u2n-remote-ue"};
  return convert_enum_idx(names, 1, value, "remote_ue_ind_e");
}
uint8_t remote_ue_ind_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "remote_ue_ind_e");
}

// Remote-UE-Indication-Request ::= ENUMERATED
const char* remote_ue_ind_request_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "remote_ue_ind_request_e");
}

// Remote-UE-Status ::= ENUMERATED
const char* remote_ue_status_opts::to_string() const
{
  static const char* names[] = {"l2-u2n-remote-ue", "no"};
  return convert_enum_idx(names, 2, value, "remote_ue_status_e");
}
uint8_t remote_ue_status_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "remote_ue_status_e");
}

// ReportCharacteristics ::= ENUMERATED
const char* report_characteristics_opts::to_string() const
{
  static const char* names[] = {"onDemand", "periodic"};
  return convert_enum_idx(names, 2, value, "report_characteristics_e");
}

// RequestType ::= ENUMERATED
const char* request_type_opts::to_string() const
{
  static const char* names[] = {"activate", "deactivate"};
  return convert_enum_idx(names, 2, value, "request_type_e");
}

// SpatialRelationforResourceIDItem ::= SEQUENCE
OCUDUASN_CODE spatial_relationfor_res_id_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ref_sig.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE spatial_relationfor_res_id_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ref_sig.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void spatial_relationfor_res_id_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("referenceSignal");
  ref_sig.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SpatialRelationInfo ::= SEQUENCE
OCUDUASN_CODE spatial_relation_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, spatial_relationfor_res_id, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE spatial_relation_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(spatial_relationfor_res_id, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void spatial_relation_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("spatialRelationforResourceID");
  for (const auto& e1 : spatial_relationfor_res_id) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SpatialRelationPerSRSResourceItem ::= SEQUENCE
OCUDUASN_CODE spatial_relation_per_srs_res_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ref_sig.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE spatial_relation_per_srs_res_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ref_sig.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void spatial_relation_per_srs_res_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("referenceSignal");
  ref_sig.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SpatialRelationPerSRSResource ::= SEQUENCE
OCUDUASN_CODE spatial_relation_per_srs_res_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, spatial_relation_per_srs_res_list, 1, 16, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE spatial_relation_per_srs_res_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(spatial_relation_per_srs_res_list, bref, 1, 16, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void spatial_relation_per_srs_res_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("spatialRelationPerSRSResource-List");
  for (const auto& e1 : spatial_relation_per_srs_res_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t srs_res_set_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {63};
  return map_enum_number(names, 1, idx, "id");
}
bool srs_res_set_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 63 == id;
}
crit_e srs_res_set_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 63) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
srs_res_set_item_ext_ies_o::ext_c srs_res_set_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 63) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e srs_res_set_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 63) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void srs_res_set_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE srs_res_set_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_set_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* srs_res_set_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"SpatialRelationPerSRSResource"};
  return convert_enum_idx(names, 1, value, "srs_res_set_item_ext_ies_o::ext_c::types");
}

// SRSResourceSet-Item ::= SEQUENCE
OCUDUASN_CODE srs_res_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(nof_srs_res_per_set_present, 1));
  HANDLE_CODE(bref.pack(periodicity_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(spatial_relation_info_present, 1));
  HANDLE_CODE(bref.pack(pathloss_ref_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  if (nof_srs_res_per_set_present) {
    HANDLE_CODE(pack_integer(bref, nof_srs_res_per_set, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  if (periodicity_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, periodicity_list, 1, 16, true));
  }
  if (spatial_relation_info_present) {
    HANDLE_CODE(spatial_relation_info.pack(bref));
  }
  if (pathloss_ref_info_present) {
    HANDLE_CODE(pathloss_ref_info.pack(bref));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(nof_srs_res_per_set_present, 1));
  bool periodicity_list_present;
  HANDLE_CODE(bref.unpack(periodicity_list_present, 1));
  HANDLE_CODE(bref.unpack(spatial_relation_info_present, 1));
  HANDLE_CODE(bref.unpack(pathloss_ref_info_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (nof_srs_res_per_set_present) {
    HANDLE_CODE(unpack_integer(nof_srs_res_per_set, bref, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  if (periodicity_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(periodicity_list, bref, 1, 16, true));
  }
  if (spatial_relation_info_present) {
    HANDLE_CODE(spatial_relation_info.unpack(bref));
  }
  if (pathloss_ref_info_present) {
    HANDLE_CODE(pathloss_ref_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_res_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (nof_srs_res_per_set_present) {
    j.write_int("numberOfSRSResourcePerSet", nof_srs_res_per_set);
  }
  if (periodicity_list.size() > 0) {
    j.start_array("periodicityList");
    for (const auto& e1 : periodicity_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
  }
  if (spatial_relation_info_present) {
    j.write_fieldname("spatialRelationInformation");
    spatial_relation_info.to_json(j);
  }
  if (pathloss_ref_info_present) {
    j.write_fieldname("pathlossReferenceInformation");
    pathloss_ref_info.to_json(j);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SSBBurstPosition ::= CHOICE
void ssb_burst_position_c::destroy_()
{
  switch (type_) {
    case types::short_bitmap:
      c.destroy<fixed_bitstring<4, false, true>>();
      break;
    case types::medium_bitmap:
      c.destroy<fixed_bitstring<8, false, true>>();
      break;
    case types::long_bitmap:
      c.destroy<fixed_bitstring<64, false, true>>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void ssb_burst_position_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::short_bitmap:
      c.init<fixed_bitstring<4, false, true>>();
      break;
    case types::medium_bitmap:
      c.init<fixed_bitstring<8, false, true>>();
      break;
    case types::long_bitmap:
      c.init<fixed_bitstring<64, false, true>>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ssb_burst_position_c");
  }
}
ssb_burst_position_c::ssb_burst_position_c(const ssb_burst_position_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::short_bitmap:
      c.init(other.c.get<fixed_bitstring<4, false, true>>());
      break;
    case types::medium_bitmap:
      c.init(other.c.get<fixed_bitstring<8, false, true>>());
      break;
    case types::long_bitmap:
      c.init(other.c.get<fixed_bitstring<64, false, true>>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ssb_burst_position_c");
  }
}
ssb_burst_position_c& ssb_burst_position_c::operator=(const ssb_burst_position_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::short_bitmap:
      c.set(other.c.get<fixed_bitstring<4, false, true>>());
      break;
    case types::medium_bitmap:
      c.set(other.c.get<fixed_bitstring<8, false, true>>());
      break;
    case types::long_bitmap:
      c.set(other.c.get<fixed_bitstring<64, false, true>>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ssb_burst_position_c");
  }

  return *this;
}
fixed_bitstring<4, false, true>& ssb_burst_position_c::set_short_bitmap()
{
  set(types::short_bitmap);
  return c.get<fixed_bitstring<4, false, true>>();
}
fixed_bitstring<8, false, true>& ssb_burst_position_c::set_medium_bitmap()
{
  set(types::medium_bitmap);
  return c.get<fixed_bitstring<8, false, true>>();
}
fixed_bitstring<64, false, true>& ssb_burst_position_c::set_long_bitmap()
{
  set(types::long_bitmap);
  return c.get<fixed_bitstring<64, false, true>>();
}
protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>& ssb_burst_position_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>();
}
void ssb_burst_position_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::short_bitmap:
      j.write_fieldname("shortBitmap");
      c.get<fixed_bitstring<4, false, true>>().to_json(j);
      break;
    case types::medium_bitmap:
      j.write_fieldname("mediumBitmap");
      c.get<fixed_bitstring<8, false, true>>().to_json(j);
      break;
    case types::long_bitmap:
      j.write_fieldname("longBitmap");
      c.get<fixed_bitstring<64, false, true>>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ssb_burst_position_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ssb_burst_position_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::short_bitmap:
      HANDLE_CODE((c.get<fixed_bitstring<4, false, true>>().pack(bref)));
      break;
    case types::medium_bitmap:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().pack(bref)));
      break;
    case types::long_bitmap:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ssb_burst_position_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ssb_burst_position_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::short_bitmap:
      HANDLE_CODE((c.get<fixed_bitstring<4, false, true>>().unpack(bref)));
      break;
    case types::medium_bitmap:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().unpack(bref)));
      break;
    case types::long_bitmap:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ssb_burst_position_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ssb_burst_position_c::types_opts::to_string() const
{
  static const char* names[] = {"shortBitmap", "mediumBitmap", "longBitmap", "choice-extension"};
  return convert_enum_idx(names, 4, value, "ssb_burst_position_c::types");
}

// TF-Configuration ::= SEQUENCE
OCUDUASN_CODE tf_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ssb_burst_position_present, 1));
  HANDLE_CODE(bref.pack(sfn_initisation_time_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, ssb_freq, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(ssb_subcarrier_spacing.pack(bref));
  HANDLE_CODE(pack_integer(bref, ssb_tx_pwr, (int8_t)-60, (int8_t)50, false, true));
  HANDLE_CODE(ssb_periodicity.pack(bref));
  HANDLE_CODE(pack_integer(bref, ssb_half_frame_offset, (uint8_t)0u, (uint8_t)1u, false, true));
  HANDLE_CODE(pack_integer(bref, ssb_sfn_offset, (uint8_t)0u, (uint8_t)15u, false, true));
  if (ssb_burst_position_present) {
    HANDLE_CODE(ssb_burst_position.pack(bref));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(sfn_initisation_time.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tf_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ssb_burst_position_present, 1));
  HANDLE_CODE(bref.unpack(sfn_initisation_time_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ssb_freq, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(ssb_subcarrier_spacing.unpack(bref));
  HANDLE_CODE(unpack_integer(ssb_tx_pwr, bref, (int8_t)-60, (int8_t)50, false, true));
  HANDLE_CODE(ssb_periodicity.unpack(bref));
  HANDLE_CODE(unpack_integer(ssb_half_frame_offset, bref, (uint8_t)0u, (uint8_t)1u, false, true));
  HANDLE_CODE(unpack_integer(ssb_sfn_offset, bref, (uint8_t)0u, (uint8_t)15u, false, true));
  if (ssb_burst_position_present) {
    HANDLE_CODE(ssb_burst_position.unpack(bref));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(sfn_initisation_time.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void tf_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sSB-frequency", ssb_freq);
  j.write_str("sSB-subcarrier-spacing", ssb_subcarrier_spacing.to_string());
  j.write_int("sSB-Transmit-power", ssb_tx_pwr);
  j.write_str("sSB-periodicity", ssb_periodicity.to_string());
  j.write_int("sSB-half-frame-offset", ssb_half_frame_offset);
  j.write_int("sSB-SFN-offset", ssb_sfn_offset);
  if (ssb_burst_position_present) {
    j.write_fieldname("sSB-BurstPosition");
    ssb_burst_position.to_json(j);
  }
  if (sfn_initisation_time_present) {
    j.write_fieldname("sFN-initialisation-time");
    sfn_initisation_time.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* tf_cfg_s::ssb_subcarrier_spacing_opts::to_string() const
{
  static const char* names[] = {"kHz15", "kHz30", "kHz120", "kHz240", "kHz60", "kHz480", "kHz960"};
  return convert_enum_idx(names, 7, value, "tf_cfg_s::ssb_subcarrier_spacing_e_");
}
uint16_t tf_cfg_s::ssb_subcarrier_spacing_opts::to_number() const
{
  static const uint16_t numbers[] = {15, 30, 120, 240, 60, 480, 960};
  return map_enum_number(numbers, 7, value, "tf_cfg_s::ssb_subcarrier_spacing_e_");
}

const char* tf_cfg_s::ssb_periodicity_opts::to_string() const
{
  static const char* names[] = {"ms5", "ms10", "ms20", "ms40", "ms80", "ms160"};
  return convert_enum_idx(names, 6, value, "tf_cfg_s::ssb_periodicity_e_");
}
uint8_t tf_cfg_s::ssb_periodicity_opts::to_number() const
{
  static const uint8_t numbers[] = {5, 10, 20, 40, 80, 160};
  return map_enum_number(numbers, 6, value, "tf_cfg_s::ssb_periodicity_e_");
}

// SSBInfoItem ::= SEQUENCE
OCUDUASN_CODE ssb_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ssb_cfg.pack(bref));
  HANDLE_CODE(pack_integer(bref, pci_nr, (uint16_t)0u, (uint16_t)1007u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ssb_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ssb_cfg.unpack(bref));
  HANDLE_CODE(unpack_integer(pci_nr, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ssb_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sSB-Configuration");
  ssb_cfg.to_json(j);
  j.write_int("pCI-NR", pci_nr);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SSBInfo ::= SEQUENCE
OCUDUASN_CODE ssb_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, list_of_ssb_info, 1, 255, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ssb_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(list_of_ssb_info, bref, 1, 255, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ssb_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("listOfSSBInfo");
  for (const auto& e1 : list_of_ssb_info) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResourceMapping ::= SEQUENCE
OCUDUASN_CODE res_map_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, start_position, (uint8_t)0u, (uint8_t)13u, false, true));
  HANDLE_CODE(nrof_sumbols.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_map_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(start_position, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  HANDLE_CODE(nrof_sumbols.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_map_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("startPosition", start_position);
  j.write_str("nrofSumbols", nrof_sumbols.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* res_map_s::nrof_sumbols_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4", "n8", "n12"};
  return convert_enum_idx(names, 5, value, "res_map_s::nrof_sumbols_e_");
}
uint8_t res_map_s::nrof_sumbols_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 8, 12};
  return map_enum_number(numbers, 5, value, "res_map_s::nrof_sumbols_e_");
}

// ValidityAreaSpecificSRSInformation ::= SEQUENCE
OCUDUASN_CODE validity_area_specific_srs_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(tx_comb_pos_present, 1));
  HANDLE_CODE(bref.pack(res_map_present, 1));
  HANDLE_CODE(bref.pack(freq_domain_shift_present, 1));
  HANDLE_CODE(bref.pack(c_srs_present, 1));
  HANDLE_CODE(bref.pack(res_type_pos_present, 1));
  HANDLE_CODE(bref.pack(seq_id_pos_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (tx_comb_pos_present) {
    HANDLE_CODE(tx_comb_pos.pack(bref));
  }
  if (res_map_present) {
    HANDLE_CODE(res_map.pack(bref));
  }
  if (freq_domain_shift_present) {
    HANDLE_CODE(pack_integer(bref, freq_domain_shift, (uint16_t)0u, (uint16_t)268u, false, true));
  }
  if (c_srs_present) {
    HANDLE_CODE(pack_integer(bref, c_srs, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (res_type_pos_present) {
    HANDLE_CODE(res_type_pos.pack(bref));
  }
  if (seq_id_pos_present) {
    HANDLE_CODE(pack_integer(bref, seq_id_pos, (uint32_t)0u, (uint32_t)65535u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE validity_area_specific_srs_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(tx_comb_pos_present, 1));
  HANDLE_CODE(bref.unpack(res_map_present, 1));
  HANDLE_CODE(bref.unpack(freq_domain_shift_present, 1));
  HANDLE_CODE(bref.unpack(c_srs_present, 1));
  HANDLE_CODE(bref.unpack(res_type_pos_present, 1));
  HANDLE_CODE(bref.unpack(seq_id_pos_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (tx_comb_pos_present) {
    HANDLE_CODE(tx_comb_pos.unpack(bref));
  }
  if (res_map_present) {
    HANDLE_CODE(res_map.unpack(bref));
  }
  if (freq_domain_shift_present) {
    HANDLE_CODE(unpack_integer(freq_domain_shift, bref, (uint16_t)0u, (uint16_t)268u, false, true));
  }
  if (c_srs_present) {
    HANDLE_CODE(unpack_integer(c_srs, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  }
  if (res_type_pos_present) {
    HANDLE_CODE(res_type_pos.unpack(bref));
  }
  if (seq_id_pos_present) {
    HANDLE_CODE(unpack_integer(seq_id_pos, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void validity_area_specific_srs_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (tx_comb_pos_present) {
    j.write_fieldname("transmissionCombPos");
    tx_comb_pos.to_json(j);
  }
  if (res_map_present) {
    j.write_fieldname("resourceMapping");
    res_map.to_json(j);
  }
  if (freq_domain_shift_present) {
    j.write_int("freqDomainShift", freq_domain_shift);
  }
  if (c_srs_present) {
    j.write_int("c-SRS", c_srs);
  }
  if (res_type_pos_present) {
    j.write_fieldname("resourceTypePos");
    res_type_pos.to_json(j);
  }
  if (seq_id_pos_present) {
    j.write_int("sequenceIDPos", seq_id_pos);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ValidityAreaSpecificSRSInformationExtended ::= SEQUENCE
OCUDUASN_CODE validity_area_specific_srs_info_extended_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pos_srs_res_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(pos_srs_res_set_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (pos_srs_res_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_srs_res_list, 1, 64, true));
  }
  if (pos_srs_res_set_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_srs_res_set_list, 1, 16, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE validity_area_specific_srs_info_extended_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool pos_srs_res_list_present;
  HANDLE_CODE(bref.unpack(pos_srs_res_list_present, 1));
  bool pos_srs_res_set_list_present;
  HANDLE_CODE(bref.unpack(pos_srs_res_set_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (pos_srs_res_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pos_srs_res_list, bref, 1, 64, true));
  }
  if (pos_srs_res_set_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pos_srs_res_set_list, bref, 1, 16, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void validity_area_specific_srs_info_extended_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pos_srs_res_list.size() > 0) {
    j.start_array("posSRSResource-List");
    for (const auto& e1 : pos_srs_res_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pos_srs_res_set_list.size() > 0) {
    j.start_array("posSRSResourceSet-List");
    for (const auto& e1 : pos_srs_res_set_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RequestedSRSTransmissionCharacteristics-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t requested_srs_tx_characteristics_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {61, 121, 128, 135, 166};
  return map_enum_number(names, 5, idx, "id");
}
bool requested_srs_tx_characteristics_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {61, 121, 128, 135, 166};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e requested_srs_tx_characteristics_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 61:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    case 128:
      return crit_e::ignore;
    case 135:
      return crit_e::ignore;
    case 166:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
requested_srs_tx_characteristics_ext_ies_o::ext_c
requested_srs_tx_characteristics_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 61:
      ret.set(ext_c::types::srs_freq);
      break;
    case 121:
      ret.set(ext_c::types::bw_aggregation_request_ind);
      break;
    case 128:
      ret.set(ext_c::types::pos_validity_area_cell_list);
      break;
    case 135:
      ret.set(ext_c::types::validity_area_specific_srs_info);
      break;
    case 166:
      ret.set(ext_c::types::validity_area_specific_srs_info_extended);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e requested_srs_tx_characteristics_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 61:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    case 128:
      return presence_e::optional;
    case 135:
      return presence_e::optional;
    case 166:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void requested_srs_tx_characteristics_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::srs_freq:
      c = uint32_t{};
      break;
    case types::bw_aggregation_request_ind:
      c = bw_aggregation_request_ind_e{};
      break;
    case types::pos_validity_area_cell_list:
      c = pos_validity_area_cell_list_l{};
      break;
    case types::validity_area_specific_srs_info:
      c = validity_area_specific_srs_info_s{};
      break;
    case types::validity_area_specific_srs_info_extended:
      c = validity_area_specific_srs_info_extended_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "requested_srs_tx_characteristics_ext_ies_o::ext_c");
  }
}
uint32_t& requested_srs_tx_characteristics_ext_ies_o::ext_c::srs_freq()
{
  assert_choice_type(types::srs_freq, type_, "Extension");
  return c.get<uint32_t>();
}
bw_aggregation_request_ind_e& requested_srs_tx_characteristics_ext_ies_o::ext_c::bw_aggregation_request_ind()
{
  assert_choice_type(types::bw_aggregation_request_ind, type_, "Extension");
  return c.get<bw_aggregation_request_ind_e>();
}
pos_validity_area_cell_list_l& requested_srs_tx_characteristics_ext_ies_o::ext_c::pos_validity_area_cell_list()
{
  assert_choice_type(types::pos_validity_area_cell_list, type_, "Extension");
  return c.get<pos_validity_area_cell_list_l>();
}
validity_area_specific_srs_info_s& requested_srs_tx_characteristics_ext_ies_o::ext_c::validity_area_specific_srs_info()
{
  assert_choice_type(types::validity_area_specific_srs_info, type_, "Extension");
  return c.get<validity_area_specific_srs_info_s>();
}
validity_area_specific_srs_info_extended_s&
requested_srs_tx_characteristics_ext_ies_o::ext_c::validity_area_specific_srs_info_extended()
{
  assert_choice_type(types::validity_area_specific_srs_info_extended, type_, "Extension");
  return c.get<validity_area_specific_srs_info_extended_s>();
}
const uint32_t& requested_srs_tx_characteristics_ext_ies_o::ext_c::srs_freq() const
{
  assert_choice_type(types::srs_freq, type_, "Extension");
  return c.get<uint32_t>();
}
const bw_aggregation_request_ind_e&
requested_srs_tx_characteristics_ext_ies_o::ext_c::bw_aggregation_request_ind() const
{
  assert_choice_type(types::bw_aggregation_request_ind, type_, "Extension");
  return c.get<bw_aggregation_request_ind_e>();
}
const pos_validity_area_cell_list_l&
requested_srs_tx_characteristics_ext_ies_o::ext_c::pos_validity_area_cell_list() const
{
  assert_choice_type(types::pos_validity_area_cell_list, type_, "Extension");
  return c.get<pos_validity_area_cell_list_l>();
}
const validity_area_specific_srs_info_s&
requested_srs_tx_characteristics_ext_ies_o::ext_c::validity_area_specific_srs_info() const
{
  assert_choice_type(types::validity_area_specific_srs_info, type_, "Extension");
  return c.get<validity_area_specific_srs_info_s>();
}
const validity_area_specific_srs_info_extended_s&
requested_srs_tx_characteristics_ext_ies_o::ext_c::validity_area_specific_srs_info_extended() const
{
  assert_choice_type(types::validity_area_specific_srs_info_extended, type_, "Extension");
  return c.get<validity_area_specific_srs_info_extended_s>();
}
void requested_srs_tx_characteristics_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::srs_freq:
      j.write_int(c.get<uint32_t>());
      break;
    case types::bw_aggregation_request_ind:
      j.write_str("true");
      break;
    case types::pos_validity_area_cell_list:
      j.start_array("");
      for (const auto& e1 : c.get<pos_validity_area_cell_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::validity_area_specific_srs_info:
      c.get<validity_area_specific_srs_info_s>().to_json(j);
      break;
    case types::validity_area_specific_srs_info_extended:
      c.get<validity_area_specific_srs_info_extended_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "requested_srs_tx_characteristics_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE requested_srs_tx_characteristics_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_freq:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)3279165u, false, true));
      break;
    case types::bw_aggregation_request_ind:
      HANDLE_CODE(c.get<bw_aggregation_request_ind_e>().pack(bref));
      break;
    case types::pos_validity_area_cell_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<pos_validity_area_cell_list_l>(), 1, 32, true));
      break;
    case types::validity_area_specific_srs_info:
      HANDLE_CODE(c.get<validity_area_specific_srs_info_s>().pack(bref));
      break;
    case types::validity_area_specific_srs_info_extended:
      HANDLE_CODE(c.get<validity_area_specific_srs_info_extended_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "requested_srs_tx_characteristics_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE requested_srs_tx_characteristics_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_freq:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
      break;
    case types::bw_aggregation_request_ind:
      HANDLE_CODE(c.get<bw_aggregation_request_ind_e>().unpack(bref));
      break;
    case types::pos_validity_area_cell_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<pos_validity_area_cell_list_l>(), bref, 1, 32, true));
      break;
    case types::validity_area_specific_srs_info:
      HANDLE_CODE(c.get<validity_area_specific_srs_info_s>().unpack(bref));
      break;
    case types::validity_area_specific_srs_info_extended:
      HANDLE_CODE(c.get<validity_area_specific_srs_info_extended_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "requested_srs_tx_characteristics_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* requested_srs_tx_characteristics_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..3279165)",
                                "Bandwidth-Aggregation-Request-Indication",
                                "PosValidityAreaCellList",
                                "ValidityAreaSpecificSRSInformation",
                                "ValidityAreaSpecificSRSInformationExtended"};
  return convert_enum_idx(names, 5, value, "requested_srs_tx_characteristics_ext_ies_o::ext_c::types");
}
uint8_t requested_srs_tx_characteristics_ext_ies_o::ext_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "requested_srs_tx_characteristics_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<requested_srs_tx_characteristics_ext_ies_o>;

OCUDUASN_CODE requested_srs_tx_characteristics_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += srs_freq_present ? 1 : 0;
  nof_ies += bw_aggregation_request_ind_present ? 1 : 0;
  nof_ies += pos_validity_area_cell_list_present ? 1 : 0;
  nof_ies += validity_area_specific_srs_info_present ? 1 : 0;
  nof_ies += validity_area_specific_srs_info_extended_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (srs_freq_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)61, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, srs_freq, (uint32_t)0u, (uint32_t)3279165u, false, true));
  }
  if (bw_aggregation_request_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)121, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(bw_aggregation_request_ind.pack(bref));
  }
  if (pos_validity_area_cell_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)128, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_validity_area_cell_list, 1, 32, true));
  }
  if (validity_area_specific_srs_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)135, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(validity_area_specific_srs_info.pack(bref));
  }
  if (validity_area_specific_srs_info_extended_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)166, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(validity_area_specific_srs_info_extended.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE requested_srs_tx_characteristics_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != requested_srs_tx_characteristics_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 61: {
        srs_freq_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(srs_freq, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
        break;
      }
      case 121: {
        bw_aggregation_request_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(bw_aggregation_request_ind.unpack(bref));
        break;
      }
      case 128: {
        pos_validity_area_cell_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pos_validity_area_cell_list, bref, 1, 32, true));
        break;
      }
      case 135: {
        validity_area_specific_srs_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(validity_area_specific_srs_info.unpack(bref));
        break;
      }
      case 166: {
        validity_area_specific_srs_info_extended_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(validity_area_specific_srs_info_extended.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void requested_srs_tx_characteristics_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (srs_freq_present) {
    j.start_obj();
    j.write_int("id", 61);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", srs_freq);
    j.end_obj();
  }
  if (bw_aggregation_request_ind_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  if (pos_validity_area_cell_list_present) {
    j.start_obj();
    j.write_int("id", 128);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : pos_validity_area_cell_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (validity_area_specific_srs_info_present) {
    j.start_obj();
    j.write_int("id", 135);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    validity_area_specific_srs_info.to_json(j);
    j.end_obj();
  }
  if (validity_area_specific_srs_info_extended_present) {
    j.start_obj();
    j.write_int("id", 166);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    validity_area_specific_srs_info_extended.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RequestedSRSTransmissionCharacteristics ::= SEQUENCE
OCUDUASN_CODE requested_srs_tx_characteristics_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(nof_txs_present, 1));
  HANDLE_CODE(bref.pack(list_of_srs_res_set.size() > 0, 1));
  HANDLE_CODE(bref.pack(ssb_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (nof_txs_present) {
    HANDLE_CODE(pack_integer(bref, nof_txs, (uint16_t)0u, (uint16_t)500u, true, true));
  }
  HANDLE_CODE(res_type.pack(bref));
  HANDLE_CODE(bw.pack(bref));
  if (list_of_srs_res_set.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, list_of_srs_res_set, 1, 16, true));
  }
  if (ssb_info_present) {
    HANDLE_CODE(ssb_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE requested_srs_tx_characteristics_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(nof_txs_present, 1));
  bool list_of_srs_res_set_present;
  HANDLE_CODE(bref.unpack(list_of_srs_res_set_present, 1));
  HANDLE_CODE(bref.unpack(ssb_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (nof_txs_present) {
    HANDLE_CODE(unpack_integer(nof_txs, bref, (uint16_t)0u, (uint16_t)500u, true, true));
  }
  HANDLE_CODE(res_type.unpack(bref));
  HANDLE_CODE(bw.unpack(bref));
  if (list_of_srs_res_set_present) {
    HANDLE_CODE(unpack_dyn_seq_of(list_of_srs_res_set, bref, 1, 16, true));
  }
  if (ssb_info_present) {
    HANDLE_CODE(ssb_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void requested_srs_tx_characteristics_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (nof_txs_present) {
    j.write_int("numberOfTransmissions", nof_txs);
  }
  j.write_str("resourceType", res_type.to_string());
  j.write_fieldname("bandwidth");
  bw.to_json(j);
  if (list_of_srs_res_set.size() > 0) {
    j.start_array("listOfSRSResourceSet");
    for (const auto& e1 : list_of_srs_res_set) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ssb_info_present) {
    j.write_fieldname("sSBInformation");
    ssb_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* requested_srs_tx_characteristics_s::res_type_opts::to_string() const
{
  static const char* names[] = {"periodic", "semi-persistent", "aperiodic"};
  return convert_enum_idx(names, 3, value, "requested_srs_tx_characteristics_s::res_type_e_");
}

// RequestedSRSPreconfigurationCharacteristics-Item ::= SEQUENCE
OCUDUASN_CODE requested_srs_precfg_characteristics_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(requested_srs_tx_characteristics.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE requested_srs_precfg_characteristics_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(requested_srs_tx_characteristics.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void requested_srs_precfg_characteristics_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("requestedSRSTransmissionCharacteristics");
  requested_srs_tx_characteristics.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResponseTime ::= SEQUENCE
OCUDUASN_CODE resp_time_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, time, (uint16_t)1u, (uint16_t)128u, true, true));
  HANDLE_CODE(time_unit.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE resp_time_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(time, bref, (uint16_t)1u, (uint16_t)128u, true, true));
  HANDLE_CODE(time_unit.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void resp_time_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("time", time);
  j.write_str("timeUnit", time_unit.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* resp_time_s::time_unit_opts::to_string() const
{
  static const char* names[] = {"second", "ten-seconds", "ten-milliseconds"};
  return convert_enum_idx(names, 3, value, "resp_time_s::time_unit_e_");
}

// SRSCarrier-List-Item ::= SEQUENCE
OCUDUASN_CODE srs_carrier_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pci_nr_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, point_a, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, ul_ch_bw_per_scs_list, 1, 5, true));
  HANDLE_CODE(active_ul_bwp.pack(bref));
  if (pci_nr_present) {
    HANDLE_CODE(pack_integer(bref, pci_nr, (uint16_t)0u, (uint16_t)1007u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_carrier_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(pci_nr_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(point_a, bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(ul_ch_bw_per_scs_list, bref, 1, 5, true));
  HANDLE_CODE(active_ul_bwp.unpack(bref));
  if (pci_nr_present) {
    HANDLE_CODE(unpack_integer(pci_nr, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_carrier_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pointA", point_a);
  j.start_array("uplinkChannelBW-PerSCS-List");
  for (const auto& e1 : ul_ch_bw_per_scs_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.write_fieldname("activeULBWP");
  active_ul_bwp.to_json(j);
  if (pci_nr_present) {
    j.write_int("pCI-NR", pci_nr);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSConfiguration-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t srs_configuration_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {122};
  return map_enum_number(names, 1, idx, "id");
}
bool srs_configuration_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 122 == id;
}
crit_e srs_configuration_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 122) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
srs_configuration_ext_ies_o::ext_c srs_configuration_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 122) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e srs_configuration_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 122) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void srs_configuration_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.start_array("");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
}
OCUDUASN_CODE srs_configuration_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 32, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_configuration_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 32, true));
  return OCUDUASN_SUCCESS;
}

const char* srs_configuration_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"PosSRSResourceSet-Aggregation-List"};
  return convert_enum_idx(names, 1, value, "srs_configuration_ext_ies_o::ext_c::types");
}

// SRSConfiguration ::= SEQUENCE
OCUDUASN_CODE srs_configuration_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, srs_carrier_list, 1, 32, true));
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_configuration_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(srs_carrier_list, bref, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_configuration_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("sRSCarrier-List");
  for (const auto& e1 : srs_carrier_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSInfo ::= SEQUENCE
OCUDUASN_CODE srs_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, srs_res, (uint8_t)0u, (uint8_t)63u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(srs_res, bref, (uint8_t)0u, (uint8_t)63u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSResource", srs_res);
  j.end_obj();
}

// SRSPortIndex ::= ENUMERATED
const char* srs_port_idx_opts::to_string() const
{
  static const char* names[] = {"id1000", "id1001", "id1002", "id1003"};
  return convert_enum_idx(names, 4, value, "srs_port_idx_e");
}
uint16_t srs_port_idx_opts::to_number() const
{
  static const uint16_t numbers[] = {1000, 1001, 1002, 1003};
  return map_enum_number(numbers, 4, value, "srs_port_idx_e");
}

// SRSPreconfiguration-Item ::= SEQUENCE
OCUDUASN_CODE srs_precfg_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(srs_cfg.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, pos_validity_area_cell_list, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_precfg_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(srs_cfg.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(pos_validity_area_cell_list, bref, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_precfg_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sRSConfiguration");
  srs_cfg.to_json(j);
  j.start_array("posValidityAreaCellList");
  for (const auto& e1 : pos_validity_area_cell_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSReservationType ::= ENUMERATED
const char* srs_reserv_type_opts::to_string() const
{
  static const char* names[] = {"reserve", "release"};
  return convert_enum_idx(names, 2, value, "srs_reserv_type_e");
}

// SRSResourceID-Item ::= SEQUENCE
OCUDUASN_CODE srs_res_id_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, srs_res_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_id_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_res_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_res_id_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSResourceID", srs_res_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSResourceTrigger ::= SEQUENCE
OCUDUASN_CODE srs_res_trigger_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(
      pack_dyn_seq_of(bref, aperiodic_srs_res_trigger_list, 1, 3, integer_packer<uint8_t>(1, 3, false, true), true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_trigger_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(
      unpack_dyn_seq_of(aperiodic_srs_res_trigger_list, bref, 1, 3, integer_packer<uint8_t>(1, 3, false, true), true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_res_trigger_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("aperiodicSRSResourceTriggerList");
  for (const auto& e1 : aperiodic_srs_res_trigger_list) {
    j.write_int(e1);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSResourceTypeChoice ::= CHOICE
void srs_res_type_choice_c::destroy_()
{
  switch (type_) {
    case types::srs_res_info:
      c.destroy<srs_info_s>();
      break;
    case types::pos_srs_res_info:
      c.destroy<pos_srs_info_s>();
      break;
    default:
      break;
  }
}
void srs_res_type_choice_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::srs_res_info:
      c.init<srs_info_s>();
      break;
    case types::pos_srs_res_info:
      c.init<pos_srs_info_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_type_choice_c");
  }
}
srs_res_type_choice_c::srs_res_type_choice_c(const srs_res_type_choice_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::srs_res_info:
      c.init(other.c.get<srs_info_s>());
      break;
    case types::pos_srs_res_info:
      c.init(other.c.get<pos_srs_info_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_type_choice_c");
  }
}
srs_res_type_choice_c& srs_res_type_choice_c::operator=(const srs_res_type_choice_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::srs_res_info:
      c.set(other.c.get<srs_info_s>());
      break;
    case types::pos_srs_res_info:
      c.set(other.c.get<pos_srs_info_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_type_choice_c");
  }

  return *this;
}
srs_info_s& srs_res_type_choice_c::set_srs_res_info()
{
  set(types::srs_res_info);
  return c.get<srs_info_s>();
}
pos_srs_info_s& srs_res_type_choice_c::set_pos_srs_res_info()
{
  set(types::pos_srs_res_info);
  return c.get<pos_srs_info_s>();
}
void srs_res_type_choice_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srs_res_info:
      j.write_fieldname("sRSResourceInfo");
      c.get<srs_info_s>().to_json(j);
      break;
    case types::pos_srs_res_info:
      j.write_fieldname("posSRSResourceInfo");
      c.get<pos_srs_info_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_type_choice_c");
  }
  j.end_obj();
}
OCUDUASN_CODE srs_res_type_choice_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::srs_res_info:
      HANDLE_CODE(c.get<srs_info_s>().pack(bref));
      break;
    case types::pos_srs_res_info:
      HANDLE_CODE(c.get<pos_srs_info_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_type_choice_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_res_type_choice_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::srs_res_info:
      HANDLE_CODE(c.get<srs_info_s>().unpack(bref));
      break;
    case types::pos_srs_res_info:
      HANDLE_CODE(c.get<pos_srs_info_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "srs_res_type_choice_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* srs_res_type_choice_c::types_opts::to_string() const
{
  static const char* names[] = {"sRSResourceInfo", "posSRSResourceInfo"};
  return convert_enum_idx(names, 2, value, "srs_res_type_choice_c::types");
}

// SRSResourcetype-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t srs_restype_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {100};
  return map_enum_number(names, 1, idx, "id");
}
bool srs_restype_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 100 == id;
}
crit_e srs_restype_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 100) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
srs_restype_ext_ies_o::ext_c srs_restype_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 100) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e srs_restype_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 100) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void srs_restype_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_str(c.to_string());
}
OCUDUASN_CODE srs_restype_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_restype_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* srs_restype_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"SRSPortIndex"};
  return convert_enum_idx(names, 1, value, "srs_restype_ext_ies_o::ext_c::types");
}

// SRSResourcetype ::= SEQUENCE
OCUDUASN_CODE srs_restype_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(srs_res_type_choice.pack(bref));
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_restype_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(srs_res_type_choice.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void srs_restype_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sRSResourceTypeChoice");
  srs_res_type_choice.to_json(j);
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSTransmissionStatus ::= ENUMERATED
const char* srs_tx_status_opts::to_string() const
{
  static const char* names[] = {"stopped", "area-specific-SRS-activated"};
  return convert_enum_idx(names, 2, value, "srs_tx_status_e");
}

// Search-window-information ::= SEQUENCE
OCUDUASN_CODE search_win_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, expected_propagation_delay, (int16_t)-3841, (int16_t)3841, true, true));
  HANDLE_CODE(pack_integer(bref, delay_uncertainty, (uint16_t)1u, (uint16_t)246u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE search_win_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(expected_propagation_delay, bref, (int16_t)-3841, (int16_t)3841, true, true));
  HANDLE_CODE(unpack_integer(delay_uncertainty, bref, (uint16_t)1u, (uint16_t)246u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void search_win_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("expectedPropagationDelay", expected_propagation_delay);
  j.write_int("delayUncertainty", delay_uncertainty);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SpatialDirectionInformation ::= SEQUENCE
OCUDUASN_CODE spatial_direction_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(nr_prs_beam_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE spatial_direction_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(nr_prs_beam_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void spatial_direction_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("nR-PRS-Beam-Information");
  nr_prs_beam_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRP-MeasurementRequestItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t trp_meas_request_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {60, 69, 82, 83};
  return map_enum_number(names, 4, idx, "id");
}
bool trp_meas_request_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {60, 69, 82, 83};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trp_meas_request_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 60:
      return crit_e::ignore;
    case 69:
      return crit_e::ignore;
    case 82:
      return crit_e::ignore;
    case 83:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
trp_meas_request_item_ext_ies_o::ext_c trp_meas_request_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 60:
      ret.set(ext_c::types::cell_id);
      break;
    case 69:
      ret.set(ext_c::types::ao_a_search_win);
      break;
    case 82:
      ret.set(ext_c::types::nof_trp_rx_teg);
      break;
    case 83:
      ret.set(ext_c::types::nof_trp_rx_tx_teg);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_meas_request_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 60:
      return presence_e::optional;
    case 69:
      return presence_e::optional;
    case 82:
      return presence_e::optional;
    case 83:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void trp_meas_request_item_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::cell_id:
      c = cgi_nr_s{};
      break;
    case types::ao_a_search_win:
      c = ao_a_assist_info_s{};
      break;
    case types::nof_trp_rx_teg:
      c = nof_trp_rx_teg_e{};
      break;
    case types::nof_trp_rx_tx_teg:
      c = nof_trp_rx_tx_teg_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_request_item_ext_ies_o::ext_c");
  }
}
cgi_nr_s& trp_meas_request_item_ext_ies_o::ext_c::cell_id()
{
  assert_choice_type(types::cell_id, type_, "Extension");
  return c.get<cgi_nr_s>();
}
ao_a_assist_info_s& trp_meas_request_item_ext_ies_o::ext_c::ao_a_search_win()
{
  assert_choice_type(types::ao_a_search_win, type_, "Extension");
  return c.get<ao_a_assist_info_s>();
}
nof_trp_rx_teg_e& trp_meas_request_item_ext_ies_o::ext_c::nof_trp_rx_teg()
{
  assert_choice_type(types::nof_trp_rx_teg, type_, "Extension");
  return c.get<nof_trp_rx_teg_e>();
}
nof_trp_rx_tx_teg_e& trp_meas_request_item_ext_ies_o::ext_c::nof_trp_rx_tx_teg()
{
  assert_choice_type(types::nof_trp_rx_tx_teg, type_, "Extension");
  return c.get<nof_trp_rx_tx_teg_e>();
}
const cgi_nr_s& trp_meas_request_item_ext_ies_o::ext_c::cell_id() const
{
  assert_choice_type(types::cell_id, type_, "Extension");
  return c.get<cgi_nr_s>();
}
const ao_a_assist_info_s& trp_meas_request_item_ext_ies_o::ext_c::ao_a_search_win() const
{
  assert_choice_type(types::ao_a_search_win, type_, "Extension");
  return c.get<ao_a_assist_info_s>();
}
const nof_trp_rx_teg_e& trp_meas_request_item_ext_ies_o::ext_c::nof_trp_rx_teg() const
{
  assert_choice_type(types::nof_trp_rx_teg, type_, "Extension");
  return c.get<nof_trp_rx_teg_e>();
}
const nof_trp_rx_tx_teg_e& trp_meas_request_item_ext_ies_o::ext_c::nof_trp_rx_tx_teg() const
{
  assert_choice_type(types::nof_trp_rx_tx_teg, type_, "Extension");
  return c.get<nof_trp_rx_tx_teg_e>();
}
void trp_meas_request_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::cell_id:
      c.get<cgi_nr_s>().to_json(j);
      break;
    case types::ao_a_search_win:
      c.get<ao_a_assist_info_s>().to_json(j);
      break;
    case types::nof_trp_rx_teg:
      j.write_str(c.get<nof_trp_rx_teg_e>().to_string());
      break;
    case types::nof_trp_rx_tx_teg:
      j.write_str(c.get<nof_trp_rx_tx_teg_e>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_request_item_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE trp_meas_request_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cell_id:
      HANDLE_CODE(c.get<cgi_nr_s>().pack(bref));
      break;
    case types::ao_a_search_win:
      HANDLE_CODE(c.get<ao_a_assist_info_s>().pack(bref));
      break;
    case types::nof_trp_rx_teg:
      HANDLE_CODE(c.get<nof_trp_rx_teg_e>().pack(bref));
      break;
    case types::nof_trp_rx_tx_teg:
      HANDLE_CODE(c.get<nof_trp_rx_tx_teg_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_request_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_request_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cell_id:
      HANDLE_CODE(c.get<cgi_nr_s>().unpack(bref));
      break;
    case types::ao_a_search_win:
      HANDLE_CODE(c.get<ao_a_assist_info_s>().unpack(bref));
      break;
    case types::nof_trp_rx_teg:
      HANDLE_CODE(c.get<nof_trp_rx_teg_e>().unpack(bref));
      break;
    case types::nof_trp_rx_tx_teg:
      HANDLE_CODE(c.get<nof_trp_rx_tx_teg_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_request_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_meas_request_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"CGI-NR", "AoA-AssistanceInfo", "NumberOfTRPRxTEG", "NumberOfTRPRxTxTEG"};
  return convert_enum_idx(names, 4, value, "trp_meas_request_item_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<trp_meas_request_item_ext_ies_o>;

OCUDUASN_CODE trp_meas_request_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += cell_id_present ? 1 : 0;
  nof_ies += ao_a_search_win_present ? 1 : 0;
  nof_ies += nof_trp_rx_teg_present ? 1 : 0;
  nof_ies += nof_trp_rx_tx_teg_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (cell_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)60, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cell_id.pack(bref));
  }
  if (ao_a_search_win_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)69, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ao_a_search_win.pack(bref));
  }
  if (nof_trp_rx_teg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)82, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nof_trp_rx_teg.pack(bref));
  }
  if (nof_trp_rx_tx_teg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)83, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nof_trp_rx_tx_teg.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_request_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_meas_request_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 60: {
        cell_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cell_id.unpack(bref));
        break;
      }
      case 69: {
        ao_a_search_win_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ao_a_search_win.unpack(bref));
        break;
      }
      case 82: {
        nof_trp_rx_teg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nof_trp_rx_teg.unpack(bref));
        break;
      }
      case 83: {
        nof_trp_rx_tx_teg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nof_trp_rx_tx_teg.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void trp_meas_request_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (cell_id_present) {
    j.start_obj();
    j.write_int("id", 60);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    cell_id.to_json(j);
    j.end_obj();
  }
  if (ao_a_search_win_present) {
    j.start_obj();
    j.write_int("id", 69);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    ao_a_search_win.to_json(j);
    j.end_obj();
  }
  if (nof_trp_rx_teg_present) {
    j.start_obj();
    j.write_int("id", 82);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", nof_trp_rx_teg.to_string());
    j.end_obj();
  }
  if (nof_trp_rx_tx_teg_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", nof_trp_rx_tx_teg.to_string());
    j.end_obj();
  }
  j.end_array();
}

// TRP-MeasurementRequestItem ::= SEQUENCE
OCUDUASN_CODE trp_meas_request_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(search_win_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_id, (uint32_t)1u, (uint32_t)65535u, true, true));
  if (search_win_info_present) {
    HANDLE_CODE(search_win_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_request_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(search_win_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_id, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  if (search_win_info_present) {
    HANDLE_CODE(search_win_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_meas_request_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-ID", trp_id);
  if (search_win_info_present) {
    j.write_fieldname("search-window-information");
    search_win_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ULRTOAMeas-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t ul_rtoa_meas_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {141, 142, 143, 144, 145, 146};
  return map_enum_number(names, 6, idx, "id");
}
bool ul_rtoa_meas_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {141, 142, 143, 144, 145, 146};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_rtoa_meas_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 141:
      return crit_e::ignore;
    case 142:
      return crit_e::ignore;
    case 143:
      return crit_e::ignore;
    case 144:
      return crit_e::ignore;
    case 145:
      return crit_e::ignore;
    case 146:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ul_rtoa_meas_ext_ies_o::value_c ul_rtoa_meas_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 141:
      ret.set(value_c::types::report_granularitykminus1);
      break;
    case 142:
      ret.set(value_c::types::report_granularitykminus2);
      break;
    case 143:
      ret.set(value_c::types::report_granularitykminus3);
      break;
    case 144:
      ret.set(value_c::types::report_granularitykminus4);
      break;
    case 145:
      ret.set(value_c::types::report_granularitykminus5);
      break;
    case 146:
      ret.set(value_c::types::report_granularitykminus6);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ul_rtoa_meas_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 141:
      return presence_e::mandatory;
    case 142:
      return presence_e::mandatory;
    case 143:
      return presence_e::mandatory;
    case 144:
      return presence_e::mandatory;
    case 145:
      return presence_e::mandatory;
    case 146:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ul_rtoa_meas_ext_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::report_granularitykminus1:
      c = uint32_t{};
      break;
    case types::report_granularitykminus2:
      c = uint32_t{};
      break;
    case types::report_granularitykminus3:
      c = uint32_t{};
      break;
    case types::report_granularitykminus4:
      c = uint32_t{};
      break;
    case types::report_granularitykminus5:
      c = uint32_t{};
      break;
    case types::report_granularitykminus6:
      c = uint32_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_ext_ies_o::value_c");
  }
}
uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus1()
{
  assert_choice_type(types::report_granularitykminus1, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus2()
{
  assert_choice_type(types::report_granularitykminus2, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus3()
{
  assert_choice_type(types::report_granularitykminus3, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus4()
{
  assert_choice_type(types::report_granularitykminus4, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus5()
{
  assert_choice_type(types::report_granularitykminus5, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus6()
{
  assert_choice_type(types::report_granularitykminus6, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus1() const
{
  assert_choice_type(types::report_granularitykminus1, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus2() const
{
  assert_choice_type(types::report_granularitykminus2, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus3() const
{
  assert_choice_type(types::report_granularitykminus3, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus4() const
{
  assert_choice_type(types::report_granularitykminus4, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus5() const
{
  assert_choice_type(types::report_granularitykminus5, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& ul_rtoa_meas_ext_ies_o::value_c::report_granularitykminus6() const
{
  assert_choice_type(types::report_granularitykminus6, type_, "Value");
  return c.get<uint32_t>();
}
void ul_rtoa_meas_ext_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::report_granularitykminus1:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus2:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus3:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus4:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus5:
      j.write_int(c.get<uint32_t>());
      break;
    case types::report_granularitykminus6:
      j.write_int(c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_ext_ies_o::value_c");
  }
}
OCUDUASN_CODE ul_rtoa_meas_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::report_granularitykminus1:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)3940097u, false, true));
      break;
    case types::report_granularitykminus2:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)7880193u, false, true));
      break;
    case types::report_granularitykminus3:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)15760385u, false, true));
      break;
    case types::report_granularitykminus4:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)31520769u, false, true));
      break;
    case types::report_granularitykminus5:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)63041537u, false, true));
      break;
    case types::report_granularitykminus6:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)126083073u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_ext_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_rtoa_meas_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::report_granularitykminus1:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)3940097u, false, true));
      break;
    case types::report_granularitykminus2:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)7880193u, false, true));
      break;
    case types::report_granularitykminus3:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)15760385u, false, true));
      break;
    case types::report_granularitykminus4:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)31520769u, false, true));
      break;
    case types::report_granularitykminus5:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)63041537u, false, true));
      break;
    case types::report_granularitykminus6:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)126083073u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_ext_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ul_rtoa_meas_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..3940097)",
                                "INTEGER (0..7880193)",
                                "INTEGER (0..15760385)",
                                "INTEGER (0..31520769)",
                                "INTEGER (0..63041537)",
                                "INTEGER (0..126083073)"};
  return convert_enum_idx(names, 6, value, "ul_rtoa_meas_ext_ies_o::value_c::types");
}

// ULRTOAMeas ::= CHOICE
void ul_rtoa_meas_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void ul_rtoa_meas_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::k0:
      break;
    case types::k1:
      break;
    case types::k2:
      break;
    case types::k3:
      break;
    case types::k4:
      break;
    case types::k5:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_c");
  }
}
ul_rtoa_meas_c::ul_rtoa_meas_c(const ul_rtoa_meas_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::k0:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k1:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k2:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k3:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k4:
      c.init(other.c.get<uint32_t>());
      break;
    case types::k5:
      c.init(other.c.get<uint16_t>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_c");
  }
}
ul_rtoa_meas_c& ul_rtoa_meas_c::operator=(const ul_rtoa_meas_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::k0:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k1:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k2:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k3:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k4:
      c.set(other.c.get<uint32_t>());
      break;
    case types::k5:
      c.set(other.c.get<uint16_t>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_c");
  }

  return *this;
}
uint32_t& ul_rtoa_meas_c::set_k0()
{
  set(types::k0);
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_c::set_k1()
{
  set(types::k1);
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_c::set_k2()
{
  set(types::k2);
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_c::set_k3()
{
  set(types::k3);
  return c.get<uint32_t>();
}
uint32_t& ul_rtoa_meas_c::set_k4()
{
  set(types::k4);
  return c.get<uint32_t>();
}
uint16_t& ul_rtoa_meas_c::set_k5()
{
  set(types::k5);
  return c.get<uint16_t>();
}
protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>& ul_rtoa_meas_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>();
}
void ul_rtoa_meas_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::k0:
      j.write_int("k0", c.get<uint32_t>());
      break;
    case types::k1:
      j.write_int("k1", c.get<uint32_t>());
      break;
    case types::k2:
      j.write_int("k2", c.get<uint32_t>());
      break;
    case types::k3:
      j.write_int("k3", c.get<uint32_t>());
      break;
    case types::k4:
      j.write_int("k4", c.get<uint32_t>());
      break;
    case types::k5:
      j.write_int("k5", c.get<uint16_t>());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ul_rtoa_meas_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::k0:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)1970049u, false, true));
      break;
    case types::k1:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)985025u, false, true));
      break;
    case types::k2:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)492513u, false, true));
      break;
    case types::k3:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)246257u, false, true));
      break;
    case types::k4:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)123129u, false, true));
      break;
    case types::k5:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)61565u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_rtoa_meas_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::k0:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)1970049u, false, true));
      break;
    case types::k1:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)985025u, false, true));
      break;
    case types::k2:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)492513u, false, true));
      break;
    case types::k3:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)246257u, false, true));
      break;
    case types::k4:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)123129u, false, true));
      break;
    case types::k5:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)61565u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_meas_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ul_rtoa_meas_c::types_opts::to_string() const
{
  static const char* names[] = {"k0", "k1", "k2", "k3", "k4", "k5", "choice-extension"};
  return convert_enum_idx(names, 7, value, "ul_rtoa_meas_c::types");
}
uint8_t ul_rtoa_meas_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1, 2, 3, 4, 5};
  return map_enum_number(numbers, 6, value, "ul_rtoa_meas_c::types");
}

// UL-RTOAMeasurement-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t ul_rtoa_measurement_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {77, 86};
  return map_enum_number(names, 2, idx, "id");
}
bool ul_rtoa_measurement_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {77, 86};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_rtoa_measurement_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 77:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ul_rtoa_measurement_ext_ies_o::ext_c ul_rtoa_measurement_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 77:
      ret.set(ext_c::types::extended_add_path_list);
      break;
    case 86:
      ret.set(ext_c::types::trp_rx_teg_info);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ul_rtoa_measurement_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 77:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void ul_rtoa_measurement_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::extended_add_path_list:
      c = extended_add_path_list_l{};
      break;
    case types::trp_rx_teg_info:
      c = trp_rx_teg_info_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_measurement_ext_ies_o::ext_c");
  }
}
extended_add_path_list_l& ul_rtoa_measurement_ext_ies_o::ext_c::extended_add_path_list()
{
  assert_choice_type(types::extended_add_path_list, type_, "Extension");
  return c.get<extended_add_path_list_l>();
}
trp_rx_teg_info_s& ul_rtoa_measurement_ext_ies_o::ext_c::trp_rx_teg_info()
{
  assert_choice_type(types::trp_rx_teg_info, type_, "Extension");
  return c.get<trp_rx_teg_info_s>();
}
const extended_add_path_list_l& ul_rtoa_measurement_ext_ies_o::ext_c::extended_add_path_list() const
{
  assert_choice_type(types::extended_add_path_list, type_, "Extension");
  return c.get<extended_add_path_list_l>();
}
const trp_rx_teg_info_s& ul_rtoa_measurement_ext_ies_o::ext_c::trp_rx_teg_info() const
{
  assert_choice_type(types::trp_rx_teg_info, type_, "Extension");
  return c.get<trp_rx_teg_info_s>();
}
void ul_rtoa_measurement_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::extended_add_path_list:
      j.start_array("");
      for (const auto& e1 : c.get<extended_add_path_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::trp_rx_teg_info:
      c.get<trp_rx_teg_info_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_measurement_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE ul_rtoa_measurement_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::extended_add_path_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<extended_add_path_list_l>(), 1, 8, true));
      break;
    case types::trp_rx_teg_info:
      HANDLE_CODE(c.get<trp_rx_teg_info_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_measurement_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_rtoa_measurement_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::extended_add_path_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<extended_add_path_list_l>(), bref, 1, 8, true));
      break;
    case types::trp_rx_teg_info:
      HANDLE_CODE(c.get<trp_rx_teg_info_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ul_rtoa_measurement_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ul_rtoa_measurement_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"ExtendedAdditionalPathList", "TRP-Rx-TEGInformation"};
  return convert_enum_idx(names, 2, value, "ul_rtoa_measurement_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<ul_rtoa_measurement_ext_ies_o>;

OCUDUASN_CODE ul_rtoa_measurement_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += extended_add_path_list_present ? 1 : 0;
  nof_ies += trp_rx_teg_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (extended_add_path_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, extended_add_path_list, 1, 8, true));
  }
  if (trp_rx_teg_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)86, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(trp_rx_teg_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_rtoa_measurement_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ul_rtoa_measurement_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 77: {
        extended_add_path_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(extended_add_path_list, bref, 1, 8, true));
        break;
      }
      case 86: {
        trp_rx_teg_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(trp_rx_teg_info.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void ul_rtoa_measurement_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (extended_add_path_list_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : extended_add_path_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (trp_rx_teg_info_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    trp_rx_teg_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UL-RTOAMeasurement ::= SEQUENCE
OCUDUASN_CODE ul_rtoa_measurement_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(add_path_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ul_rto_ameas.pack(bref));
  if (add_path_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, add_path_list, 1, 2, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_rtoa_measurement_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool add_path_list_present;
  HANDLE_CODE(bref.unpack(add_path_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ul_rto_ameas.unpack(bref));
  if (add_path_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(add_path_list, bref, 1, 2, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ul_rtoa_measurement_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("uLRTOAmeas");
  ul_rto_ameas.to_json(j);
  if (add_path_list.size() > 0) {
    j.start_array("additionalPathList");
    for (const auto& e1 : add_path_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// UL-RSCPMeas ::= SEQUENCE
OCUDUASN_CODE ul_rs_cp_meas_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, ul_rs_cp, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_rs_cp_meas_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ul_rs_cp, bref, (uint16_t)0u, (uint16_t)3599u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ul_rs_cp_meas_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("uLRSCP", ul_rs_cp);
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TrpMeasuredResultsValue-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t trp_measured_results_value_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {71, 74, 75, 126};
  return map_enum_number(names, 4, idx, "id");
}
bool trp_measured_results_value_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {71, 74, 75, 126};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trp_measured_results_value_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 71:
      return crit_e::reject;
    case 74:
      return crit_e::reject;
    case 75:
      return crit_e::reject;
    case 126:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
trp_measured_results_value_ext_ies_o::value_c trp_measured_results_value_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 71:
      ret.set(value_c::types::zo_a);
      break;
    case 74:
      ret.set(value_c::types::multiple_ul_ao_a);
      break;
    case 75:
      ret.set(value_c::types::ul_srs_rsrp_p);
      break;
    case 126:
      ret.set(value_c::types::ul_rs_cp_meas);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_measured_results_value_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 71:
      return presence_e::mandatory;
    case 74:
      return presence_e::mandatory;
    case 75:
      return presence_e::mandatory;
    case 126:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void trp_measured_results_value_ext_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::zo_a:
      c = zo_a_s{};
      break;
    case types::multiple_ul_ao_a:
      c = multiple_ul_ao_a_s{};
      break;
    case types::ul_srs_rsrp_p:
      c = ul_srs_rsrp_p_s{};
      break;
    case types::ul_rs_cp_meas:
      c = ul_rs_cp_meas_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_ext_ies_o::value_c");
  }
}
zo_a_s& trp_measured_results_value_ext_ies_o::value_c::zo_a()
{
  assert_choice_type(types::zo_a, type_, "Value");
  return c.get<zo_a_s>();
}
multiple_ul_ao_a_s& trp_measured_results_value_ext_ies_o::value_c::multiple_ul_ao_a()
{
  assert_choice_type(types::multiple_ul_ao_a, type_, "Value");
  return c.get<multiple_ul_ao_a_s>();
}
ul_srs_rsrp_p_s& trp_measured_results_value_ext_ies_o::value_c::ul_srs_rsrp_p()
{
  assert_choice_type(types::ul_srs_rsrp_p, type_, "Value");
  return c.get<ul_srs_rsrp_p_s>();
}
ul_rs_cp_meas_s& trp_measured_results_value_ext_ies_o::value_c::ul_rs_cp_meas()
{
  assert_choice_type(types::ul_rs_cp_meas, type_, "Value");
  return c.get<ul_rs_cp_meas_s>();
}
const zo_a_s& trp_measured_results_value_ext_ies_o::value_c::zo_a() const
{
  assert_choice_type(types::zo_a, type_, "Value");
  return c.get<zo_a_s>();
}
const multiple_ul_ao_a_s& trp_measured_results_value_ext_ies_o::value_c::multiple_ul_ao_a() const
{
  assert_choice_type(types::multiple_ul_ao_a, type_, "Value");
  return c.get<multiple_ul_ao_a_s>();
}
const ul_srs_rsrp_p_s& trp_measured_results_value_ext_ies_o::value_c::ul_srs_rsrp_p() const
{
  assert_choice_type(types::ul_srs_rsrp_p, type_, "Value");
  return c.get<ul_srs_rsrp_p_s>();
}
const ul_rs_cp_meas_s& trp_measured_results_value_ext_ies_o::value_c::ul_rs_cp_meas() const
{
  assert_choice_type(types::ul_rs_cp_meas, type_, "Value");
  return c.get<ul_rs_cp_meas_s>();
}
void trp_measured_results_value_ext_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::zo_a:
      c.get<zo_a_s>().to_json(j);
      break;
    case types::multiple_ul_ao_a:
      c.get<multiple_ul_ao_a_s>().to_json(j);
      break;
    case types::ul_srs_rsrp_p:
      c.get<ul_srs_rsrp_p_s>().to_json(j);
      break;
    case types::ul_rs_cp_meas:
      c.get<ul_rs_cp_meas_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_ext_ies_o::value_c");
  }
}
OCUDUASN_CODE trp_measured_results_value_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::zo_a:
      HANDLE_CODE(c.get<zo_a_s>().pack(bref));
      break;
    case types::multiple_ul_ao_a:
      HANDLE_CODE(c.get<multiple_ul_ao_a_s>().pack(bref));
      break;
    case types::ul_srs_rsrp_p:
      HANDLE_CODE(c.get<ul_srs_rsrp_p_s>().pack(bref));
      break;
    case types::ul_rs_cp_meas:
      HANDLE_CODE(c.get<ul_rs_cp_meas_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_ext_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_measured_results_value_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::zo_a:
      HANDLE_CODE(c.get<zo_a_s>().unpack(bref));
      break;
    case types::multiple_ul_ao_a:
      HANDLE_CODE(c.get<multiple_ul_ao_a_s>().unpack(bref));
      break;
    case types::ul_srs_rsrp_p:
      HANDLE_CODE(c.get<ul_srs_rsrp_p_s>().unpack(bref));
      break;
    case types::ul_rs_cp_meas:
      HANDLE_CODE(c.get<ul_rs_cp_meas_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_ext_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_measured_results_value_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"ZoA", "MultipleULAoA", "UL-SRS-RSRPP", "UL-RSCPMeas"};
  return convert_enum_idx(names, 4, value, "trp_measured_results_value_ext_ies_o::value_c::types");
}

// TrpMeasuredResultsValue ::= CHOICE
void trp_measured_results_value_c::destroy_()
{
  switch (type_) {
    case types::ul_angle_of_arrival:
      c.destroy<ul_ao_a_s>();
      break;
    case types::ul_rtoa:
      c.destroy<ul_rtoa_measurement_s>();
      break;
    case types::gnb_rx_tx_time_diff:
      c.destroy<gnb_rx_tx_time_diff_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void trp_measured_results_value_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ul_angle_of_arrival:
      c.init<ul_ao_a_s>();
      break;
    case types::ul_srs_rsrp:
      break;
    case types::ul_rtoa:
      c.init<ul_rtoa_measurement_s>();
      break;
    case types::gnb_rx_tx_time_diff:
      c.init<gnb_rx_tx_time_diff_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_c");
  }
}
trp_measured_results_value_c::trp_measured_results_value_c(const trp_measured_results_value_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ul_angle_of_arrival:
      c.init(other.c.get<ul_ao_a_s>());
      break;
    case types::ul_srs_rsrp:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ul_rtoa:
      c.init(other.c.get<ul_rtoa_measurement_s>());
      break;
    case types::gnb_rx_tx_time_diff:
      c.init(other.c.get<gnb_rx_tx_time_diff_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_c");
  }
}
trp_measured_results_value_c& trp_measured_results_value_c::operator=(const trp_measured_results_value_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ul_angle_of_arrival:
      c.set(other.c.get<ul_ao_a_s>());
      break;
    case types::ul_srs_rsrp:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ul_rtoa:
      c.set(other.c.get<ul_rtoa_measurement_s>());
      break;
    case types::gnb_rx_tx_time_diff:
      c.set(other.c.get<gnb_rx_tx_time_diff_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_c");
  }

  return *this;
}
ul_ao_a_s& trp_measured_results_value_c::set_ul_angle_of_arrival()
{
  set(types::ul_angle_of_arrival);
  return c.get<ul_ao_a_s>();
}
uint8_t& trp_measured_results_value_c::set_ul_srs_rsrp()
{
  set(types::ul_srs_rsrp);
  return c.get<uint8_t>();
}
ul_rtoa_measurement_s& trp_measured_results_value_c::set_ul_rtoa()
{
  set(types::ul_rtoa);
  return c.get<ul_rtoa_measurement_s>();
}
gnb_rx_tx_time_diff_s& trp_measured_results_value_c::set_gnb_rx_tx_time_diff()
{
  set(types::gnb_rx_tx_time_diff);
  return c.get<gnb_rx_tx_time_diff_s>();
}
protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>& trp_measured_results_value_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>();
}
void trp_measured_results_value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ul_angle_of_arrival:
      j.write_fieldname("uL-AngleOfArrival");
      c.get<ul_ao_a_s>().to_json(j);
      break;
    case types::ul_srs_rsrp:
      j.write_int("uL-SRS-RSRP", c.get<uint8_t>());
      break;
    case types::ul_rtoa:
      j.write_fieldname("uL-RTOA");
      c.get<ul_rtoa_measurement_s>().to_json(j);
      break;
    case types::gnb_rx_tx_time_diff:
      j.write_fieldname("gNB-RxTxTimeDiff");
      c.get<gnb_rx_tx_time_diff_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_measured_results_value_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ul_angle_of_arrival:
      HANDLE_CODE(c.get<ul_ao_a_s>().pack(bref));
      break;
    case types::ul_srs_rsrp:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)126u, false, true));
      break;
    case types::ul_rtoa:
      HANDLE_CODE(c.get<ul_rtoa_measurement_s>().pack(bref));
      break;
    case types::gnb_rx_tx_time_diff:
      HANDLE_CODE(c.get<gnb_rx_tx_time_diff_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_measured_results_value_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ul_angle_of_arrival:
      HANDLE_CODE(c.get<ul_ao_a_s>().unpack(bref));
      break;
    case types::ul_srs_rsrp:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)126u, false, true));
      break;
    case types::ul_rtoa:
      HANDLE_CODE(c.get<ul_rtoa_measurement_s>().unpack(bref));
      break;
    case types::gnb_rx_tx_time_diff:
      HANDLE_CODE(c.get<gnb_rx_tx_time_diff_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_measured_results_value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_measured_results_value_c::types_opts::to_string() const
{
  static const char* names[] = {"uL-AngleOfArrival", "uL-SRS-RSRP", "uL-RTOA", "gNB-RxTxTimeDiff", "choice-extension"};
  return convert_enum_idx(names, 5, value, "trp_measured_results_value_c::types");
}

// TrpMeasurementResultItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t trp_meas_result_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {76, 79, 80, 114, 140, 131, 157};
  return map_enum_number(names, 7, idx, "id");
}
bool trp_meas_result_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {76, 79, 80, 114, 140, 131, 157};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trp_meas_result_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 76:
      return crit_e::ignore;
    case 79:
      return crit_e::ignore;
    case 80:
      return crit_e::ignore;
    case 114:
      return crit_e::ignore;
    case 140:
      return crit_e::ignore;
    case 131:
      return crit_e::ignore;
    case 157:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
trp_meas_result_item_ext_ies_o::ext_c trp_meas_result_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 76:
      ret.set(ext_c::types::srs_restype);
      break;
    case 79:
      ret.set(ext_c::types::arp_id);
      break;
    case 80:
      ret.set(ext_c::types::lo_s_n_lo_si_nformation);
      break;
    case 114:
      ret.set(ext_c::types::mobile_trp_location_info);
      break;
    case 140:
      ret.set(ext_c::types::measured_freq_hops);
      break;
    case 131:
      ret.set(ext_c::types::aggr_pos_srs_res_id_list);
      break;
    case 157:
      ret.set(ext_c::types::meas_based_on_aggr_res);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_meas_result_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 76:
      return presence_e::optional;
    case 79:
      return presence_e::optional;
    case 80:
      return presence_e::optional;
    case 114:
      return presence_e::optional;
    case 140:
      return presence_e::optional;
    case 131:
      return presence_e::optional;
    case 157:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void trp_meas_result_item_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::srs_restype:
      c = srs_restype_s{};
      break;
    case types::arp_id:
      c = uint8_t{};
      break;
    case types::lo_s_n_lo_si_nformation:
      c = lo_s_n_lo_si_nformation_c{};
      break;
    case types::mobile_trp_location_info:
      c = mobile_trp_location_info_s{};
      break;
    case types::measured_freq_hops:
      c = measured_freq_hops_e{};
      break;
    case types::aggr_pos_srs_res_id_list:
      c = aggr_pos_srs_res_id_list_l{};
      break;
    case types::meas_based_on_aggr_res:
      c = meas_based_on_aggr_res_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_result_item_ext_ies_o::ext_c");
  }
}
srs_restype_s& trp_meas_result_item_ext_ies_o::ext_c::srs_restype()
{
  assert_choice_type(types::srs_restype, type_, "Extension");
  return c.get<srs_restype_s>();
}
uint8_t& trp_meas_result_item_ext_ies_o::ext_c::arp_id()
{
  assert_choice_type(types::arp_id, type_, "Extension");
  return c.get<uint8_t>();
}
lo_s_n_lo_si_nformation_c& trp_meas_result_item_ext_ies_o::ext_c::lo_s_n_lo_si_nformation()
{
  assert_choice_type(types::lo_s_n_lo_si_nformation, type_, "Extension");
  return c.get<lo_s_n_lo_si_nformation_c>();
}
mobile_trp_location_info_s& trp_meas_result_item_ext_ies_o::ext_c::mobile_trp_location_info()
{
  assert_choice_type(types::mobile_trp_location_info, type_, "Extension");
  return c.get<mobile_trp_location_info_s>();
}
measured_freq_hops_e& trp_meas_result_item_ext_ies_o::ext_c::measured_freq_hops()
{
  assert_choice_type(types::measured_freq_hops, type_, "Extension");
  return c.get<measured_freq_hops_e>();
}
aggr_pos_srs_res_id_list_l& trp_meas_result_item_ext_ies_o::ext_c::aggr_pos_srs_res_id_list()
{
  assert_choice_type(types::aggr_pos_srs_res_id_list, type_, "Extension");
  return c.get<aggr_pos_srs_res_id_list_l>();
}
meas_based_on_aggr_res_e& trp_meas_result_item_ext_ies_o::ext_c::meas_based_on_aggr_res()
{
  assert_choice_type(types::meas_based_on_aggr_res, type_, "Extension");
  return c.get<meas_based_on_aggr_res_e>();
}
const srs_restype_s& trp_meas_result_item_ext_ies_o::ext_c::srs_restype() const
{
  assert_choice_type(types::srs_restype, type_, "Extension");
  return c.get<srs_restype_s>();
}
const uint8_t& trp_meas_result_item_ext_ies_o::ext_c::arp_id() const
{
  assert_choice_type(types::arp_id, type_, "Extension");
  return c.get<uint8_t>();
}
const lo_s_n_lo_si_nformation_c& trp_meas_result_item_ext_ies_o::ext_c::lo_s_n_lo_si_nformation() const
{
  assert_choice_type(types::lo_s_n_lo_si_nformation, type_, "Extension");
  return c.get<lo_s_n_lo_si_nformation_c>();
}
const mobile_trp_location_info_s& trp_meas_result_item_ext_ies_o::ext_c::mobile_trp_location_info() const
{
  assert_choice_type(types::mobile_trp_location_info, type_, "Extension");
  return c.get<mobile_trp_location_info_s>();
}
const measured_freq_hops_e& trp_meas_result_item_ext_ies_o::ext_c::measured_freq_hops() const
{
  assert_choice_type(types::measured_freq_hops, type_, "Extension");
  return c.get<measured_freq_hops_e>();
}
const aggr_pos_srs_res_id_list_l& trp_meas_result_item_ext_ies_o::ext_c::aggr_pos_srs_res_id_list() const
{
  assert_choice_type(types::aggr_pos_srs_res_id_list, type_, "Extension");
  return c.get<aggr_pos_srs_res_id_list_l>();
}
const meas_based_on_aggr_res_e& trp_meas_result_item_ext_ies_o::ext_c::meas_based_on_aggr_res() const
{
  assert_choice_type(types::meas_based_on_aggr_res, type_, "Extension");
  return c.get<meas_based_on_aggr_res_e>();
}
void trp_meas_result_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::srs_restype:
      c.get<srs_restype_s>().to_json(j);
      break;
    case types::arp_id:
      j.write_int(c.get<uint8_t>());
      break;
    case types::lo_s_n_lo_si_nformation:
      c.get<lo_s_n_lo_si_nformation_c>().to_json(j);
      break;
    case types::mobile_trp_location_info:
      c.get<mobile_trp_location_info_s>().to_json(j);
      break;
    case types::measured_freq_hops:
      j.write_str(c.get<measured_freq_hops_e>().to_string());
      break;
    case types::aggr_pos_srs_res_id_list:
      j.start_array("");
      for (const auto& e1 : c.get<aggr_pos_srs_res_id_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::meas_based_on_aggr_res:
      j.write_str("true");
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_result_item_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE trp_meas_result_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_restype:
      HANDLE_CODE(c.get<srs_restype_s>().pack(bref));
      break;
    case types::arp_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)16u, true, true));
      break;
    case types::lo_s_n_lo_si_nformation:
      HANDLE_CODE(c.get<lo_s_n_lo_si_nformation_c>().pack(bref));
      break;
    case types::mobile_trp_location_info:
      HANDLE_CODE(c.get<mobile_trp_location_info_s>().pack(bref));
      break;
    case types::measured_freq_hops:
      HANDLE_CODE(c.get<measured_freq_hops_e>().pack(bref));
      break;
    case types::aggr_pos_srs_res_id_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<aggr_pos_srs_res_id_list_l>(), 2, 3, true));
      break;
    case types::meas_based_on_aggr_res:
      HANDLE_CODE(c.get<meas_based_on_aggr_res_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_result_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_result_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_restype:
      HANDLE_CODE(c.get<srs_restype_s>().unpack(bref));
      break;
    case types::arp_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)16u, true, true));
      break;
    case types::lo_s_n_lo_si_nformation:
      HANDLE_CODE(c.get<lo_s_n_lo_si_nformation_c>().unpack(bref));
      break;
    case types::mobile_trp_location_info:
      HANDLE_CODE(c.get<mobile_trp_location_info_s>().unpack(bref));
      break;
    case types::measured_freq_hops:
      HANDLE_CODE(c.get<measured_freq_hops_e>().unpack(bref));
      break;
    case types::aggr_pos_srs_res_id_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<aggr_pos_srs_res_id_list_l>(), bref, 2, 3, true));
      break;
    case types::meas_based_on_aggr_res:
      HANDLE_CODE(c.get<meas_based_on_aggr_res_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_result_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_meas_result_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"SRSResourcetype",
                                "INTEGER (1..16,...)",
                                "LoS-NLoSInformation",
                                "Mobile-TRP-LocationInformation",
                                "MeasuredFrequencyHops",
                                "AggregatedPosSRSResourceID-List",
                                "MeasBasedOnAggregatedResources"};
  return convert_enum_idx(names, 7, value, "trp_meas_result_item_ext_ies_o::ext_c::types");
}
uint8_t trp_meas_result_item_ext_ies_o::ext_c::types_opts::to_number() const
{
  if (value == arp_id) {
    return 1;
  }
  invalid_enum_number(value, "trp_meas_result_item_ext_ies_o::ext_c::types");
  return 0;
}

template struct asn1::protocol_ext_field_s<trp_meas_result_item_ext_ies_o>;

OCUDUASN_CODE trp_meas_result_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += srs_restype_present ? 1 : 0;
  nof_ies += arp_id_present ? 1 : 0;
  nof_ies += lo_s_n_lo_si_nformation_present ? 1 : 0;
  nof_ies += mobile_trp_location_info_present ? 1 : 0;
  nof_ies += measured_freq_hops_present ? 1 : 0;
  nof_ies += aggr_pos_srs_res_id_list_present ? 1 : 0;
  nof_ies += meas_based_on_aggr_res_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (srs_restype_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)76, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_restype.pack(bref));
  }
  if (arp_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)79, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, arp_id, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  if (lo_s_n_lo_si_nformation_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)80, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(lo_s_n_lo_si_nformation.pack(bref));
  }
  if (mobile_trp_location_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)114, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mobile_trp_location_info.pack(bref));
  }
  if (measured_freq_hops_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)140, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(measured_freq_hops.pack(bref));
  }
  if (aggr_pos_srs_res_id_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)131, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, aggr_pos_srs_res_id_list, 2, 3, true));
  }
  if (meas_based_on_aggr_res_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)157, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_based_on_aggr_res.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_result_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_meas_result_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 76: {
        srs_restype_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_restype.unpack(bref));
        break;
      }
      case 79: {
        arp_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(arp_id, bref, (uint8_t)1u, (uint8_t)16u, true, true));
        break;
      }
      case 80: {
        lo_s_n_lo_si_nformation_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(lo_s_n_lo_si_nformation.unpack(bref));
        break;
      }
      case 114: {
        mobile_trp_location_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mobile_trp_location_info.unpack(bref));
        break;
      }
      case 140: {
        measured_freq_hops_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(measured_freq_hops.unpack(bref));
        break;
      }
      case 131: {
        aggr_pos_srs_res_id_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(aggr_pos_srs_res_id_list, bref, 2, 3, true));
        break;
      }
      case 157: {
        meas_based_on_aggr_res_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_based_on_aggr_res.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void trp_meas_result_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (srs_restype_present) {
    j.start_obj();
    j.write_int("id", 76);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    srs_restype.to_json(j);
    j.end_obj();
  }
  if (arp_id_present) {
    j.start_obj();
    j.write_int("id", 79);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", arp_id);
    j.end_obj();
  }
  if (lo_s_n_lo_si_nformation_present) {
    j.start_obj();
    j.write_int("id", 80);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    lo_s_n_lo_si_nformation.to_json(j);
    j.end_obj();
  }
  if (mobile_trp_location_info_present) {
    j.start_obj();
    j.write_int("id", 114);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mobile_trp_location_info.to_json(j);
    j.end_obj();
  }
  if (measured_freq_hops_present) {
    j.start_obj();
    j.write_int("id", 140);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", measured_freq_hops.to_string());
    j.end_obj();
  }
  if (aggr_pos_srs_res_id_list_present) {
    j.start_obj();
    j.write_int("id", 131);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : aggr_pos_srs_res_id_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (meas_based_on_aggr_res_present) {
    j.start_obj();
    j.write_int("id", 157);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  j.end_array();
}

// TrpMeasurementResultItem ::= SEQUENCE
OCUDUASN_CODE trp_meas_result_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(meas_quality_present, 1));
  HANDLE_CODE(bref.pack(meas_beam_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(measured_results_value.pack(bref));
  HANDLE_CODE(time_stamp.pack(bref));
  if (meas_quality_present) {
    HANDLE_CODE(meas_quality.pack(bref));
  }
  if (meas_beam_info_present) {
    HANDLE_CODE(meas_beam_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_result_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(meas_quality_present, 1));
  HANDLE_CODE(bref.unpack(meas_beam_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(measured_results_value.unpack(bref));
  HANDLE_CODE(time_stamp.unpack(bref));
  if (meas_quality_present) {
    HANDLE_CODE(meas_quality.unpack(bref));
  }
  if (meas_beam_info_present) {
    HANDLE_CODE(meas_beam_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_meas_result_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("measuredResultsValue");
  measured_results_value.to_json(j);
  j.write_fieldname("timeStamp");
  time_stamp.to_json(j);
  if (meas_quality_present) {
    j.write_fieldname("measurementQuality");
    meas_quality.to_json(j);
  }
  if (meas_beam_info_present) {
    j.write_fieldname("measurementBeamInfo");
    meas_beam_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRP-MeasurementResponseItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t trp_meas_resp_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {60};
  return map_enum_number(names, 1, idx, "id");
}
bool trp_meas_resp_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 60 == id;
}
crit_e trp_meas_resp_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 60) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
trp_meas_resp_item_ext_ies_o::ext_c trp_meas_resp_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 60) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_meas_resp_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 60) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void trp_meas_resp_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE trp_meas_resp_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_resp_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* trp_meas_resp_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"CGI-NR"};
  return convert_enum_idx(names, 1, value, "trp_meas_resp_item_ext_ies_o::ext_c::types");
}

// TRP-MeasurementResponseItem ::= SEQUENCE
OCUDUASN_CODE trp_meas_resp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, trp_id, (uint32_t)1u, (uint32_t)65535u, true, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, meas_result, 1, 16384, true));
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_resp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_id, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  HANDLE_CODE(unpack_dyn_seq_of(meas_result, bref, 1, 16384, true));
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_meas_resp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-ID", trp_id);
  j.start_array("measurementResult");
  for (const auto& e1 : meas_result) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRP-MeasurementUpdateItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t trp_meas_upd_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {82, 83};
  return map_enum_number(names, 2, idx, "id");
}
bool trp_meas_upd_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {82, 83};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trp_meas_upd_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 82:
      return crit_e::ignore;
    case 83:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
trp_meas_upd_item_ext_ies_o::ext_c trp_meas_upd_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 82:
      ret.set(ext_c::types::nof_trp_rx_teg);
      break;
    case 83:
      ret.set(ext_c::types::nof_trp_rx_tx_teg);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_meas_upd_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 82:
      return presence_e::optional;
    case 83:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void trp_meas_upd_item_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::nof_trp_rx_teg:
      c = nof_trp_rx_teg_e{};
      break;
    case types::nof_trp_rx_tx_teg:
      c = nof_trp_rx_tx_teg_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_upd_item_ext_ies_o::ext_c");
  }
}
nof_trp_rx_teg_e& trp_meas_upd_item_ext_ies_o::ext_c::nof_trp_rx_teg()
{
  assert_choice_type(types::nof_trp_rx_teg, type_, "Extension");
  return c.get<nof_trp_rx_teg_e>();
}
nof_trp_rx_tx_teg_e& trp_meas_upd_item_ext_ies_o::ext_c::nof_trp_rx_tx_teg()
{
  assert_choice_type(types::nof_trp_rx_tx_teg, type_, "Extension");
  return c.get<nof_trp_rx_tx_teg_e>();
}
const nof_trp_rx_teg_e& trp_meas_upd_item_ext_ies_o::ext_c::nof_trp_rx_teg() const
{
  assert_choice_type(types::nof_trp_rx_teg, type_, "Extension");
  return c.get<nof_trp_rx_teg_e>();
}
const nof_trp_rx_tx_teg_e& trp_meas_upd_item_ext_ies_o::ext_c::nof_trp_rx_tx_teg() const
{
  assert_choice_type(types::nof_trp_rx_tx_teg, type_, "Extension");
  return c.get<nof_trp_rx_tx_teg_e>();
}
void trp_meas_upd_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::nof_trp_rx_teg:
      j.write_str(c.get<nof_trp_rx_teg_e>().to_string());
      break;
    case types::nof_trp_rx_tx_teg:
      j.write_str(c.get<nof_trp_rx_tx_teg_e>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_upd_item_ext_ies_o::ext_c");
  }
}
OCUDUASN_CODE trp_meas_upd_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::nof_trp_rx_teg:
      HANDLE_CODE(c.get<nof_trp_rx_teg_e>().pack(bref));
      break;
    case types::nof_trp_rx_tx_teg:
      HANDLE_CODE(c.get<nof_trp_rx_tx_teg_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_upd_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_upd_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::nof_trp_rx_teg:
      HANDLE_CODE(c.get<nof_trp_rx_teg_e>().unpack(bref));
      break;
    case types::nof_trp_rx_tx_teg:
      HANDLE_CODE(c.get<nof_trp_rx_tx_teg_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_meas_upd_item_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_meas_upd_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"NumberOfTRPRxTEG", "NumberOfTRPRxTxTEG"};
  return convert_enum_idx(names, 2, value, "trp_meas_upd_item_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<trp_meas_upd_item_ext_ies_o>;

OCUDUASN_CODE trp_meas_upd_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += nof_trp_rx_teg_present ? 1 : 0;
  nof_ies += nof_trp_rx_tx_teg_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (nof_trp_rx_teg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)82, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nof_trp_rx_teg.pack(bref));
  }
  if (nof_trp_rx_tx_teg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)83, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nof_trp_rx_tx_teg.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_upd_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_meas_upd_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 82: {
        nof_trp_rx_teg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nof_trp_rx_teg.unpack(bref));
        break;
      }
      case 83: {
        nof_trp_rx_tx_teg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nof_trp_rx_tx_teg.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void trp_meas_upd_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (nof_trp_rx_teg_present) {
    j.start_obj();
    j.write_int("id", 82);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", nof_trp_rx_teg.to_string());
    j.end_obj();
  }
  if (nof_trp_rx_tx_teg_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", nof_trp_rx_tx_teg.to_string());
    j.end_obj();
  }
  j.end_array();
}

// TRP-MeasurementUpdateItem ::= SEQUENCE
OCUDUASN_CODE trp_meas_upd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ao_a_win_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_id, (uint32_t)1u, (uint32_t)65535u, true, true));
  if (ao_a_win_info_present) {
    HANDLE_CODE(ao_a_win_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_upd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ao_a_win_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_id, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  if (ao_a_win_info_present) {
    HANDLE_CODE(ao_a_win_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_meas_upd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-ID", trp_id);
  if (ao_a_win_info_present) {
    j.write_fieldname("aoA-window-information");
    ao_a_win_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRP-PRS-Information-List-Item ::= SEQUENCE
OCUDUASN_CODE trp_prs_info_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cgi_nr_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_id, (uint32_t)1u, (uint32_t)65535u, true, true));
  HANDLE_CODE(pack_integer(bref, nr_pci, (uint16_t)0u, (uint16_t)1007u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.pack(bref));
  }
  HANDLE_CODE(prs_cfg.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_prs_info_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(cgi_nr_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_id, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  HANDLE_CODE(unpack_integer(nr_pci, bref, (uint16_t)0u, (uint16_t)1007u, false, true));
  if (cgi_nr_present) {
    HANDLE_CODE(cgi_nr.unpack(bref));
  }
  HANDLE_CODE(prs_cfg.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_prs_info_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-ID", trp_id);
  j.write_int("nR-PCI", nr_pci);
  if (cgi_nr_present) {
    j.write_fieldname("cGI-NR");
    cgi_nr.to_json(j);
  }
  j.write_fieldname("pRSConfiguration");
  prs_cfg.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRPBeamAntennaInformation ::= SEQUENCE
OCUDUASN_CODE trp_beam_ant_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(choice_trp_beam_ant_info_item.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_beam_ant_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(choice_trp_beam_ant_info_item.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_beam_ant_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("choice-TRP-Beam-Antenna-Info-Item");
  choice_trp_beam_ant_info_item.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRPType ::= ENUMERATED
const char* trp_type_opts::to_string() const
{
  static const char* names[] = {"prsOnlyTP", "srsOnlyRP", "tp", "rp", "trp", "mobile-trp"};
  return convert_enum_idx(names, 6, value, "trp_type_e");
}

// TRPTEGItem ::= SEQUENCE
OCUDUASN_CODE trpteg_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(dl_prs_res_id_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(trp_tx_teg_info.pack(bref));
  HANDLE_CODE(pack_integer(bref, dl_prs_res_set_id, (uint8_t)0u, (uint8_t)7u, false, true));
  if (dl_prs_res_id_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, dl_prs_res_id_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trpteg_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool dl_prs_res_id_list_present;
  HANDLE_CODE(bref.unpack(dl_prs_res_id_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(trp_tx_teg_info.unpack(bref));
  HANDLE_CODE(unpack_integer(dl_prs_res_set_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  if (dl_prs_res_id_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(dl_prs_res_id_list, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trpteg_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tRP-Tx-TEGInformation");
  trp_tx_teg_info.to_json(j);
  j.write_int("dl-PRSResourceSetID", dl_prs_res_set_id);
  if (dl_prs_res_id_list.size() > 0) {
    j.start_array("dl-PRSResourceID-List");
    for (const auto& e1 : dl_prs_res_id_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRPInformationTypeResponseItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t trp_info_type_resp_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {62, 68, 84, 93, 114, 117};
  return map_enum_number(names, 6, idx, "id");
}
bool trp_info_type_resp_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {62, 68, 84, 93, 114, 117};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trp_info_type_resp_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 62:
      return crit_e::reject;
    case 68:
      return crit_e::reject;
    case 84:
      return crit_e::reject;
    case 93:
      return crit_e::reject;
    case 114:
      return crit_e::ignore;
    case 117:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
trp_info_type_resp_item_ext_ies_o::value_c trp_info_type_resp_item_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 62:
      ret.set(value_c::types::trp_type);
      break;
    case 68:
      ret.set(value_c::types::on_demand_prs);
      break;
    case 84:
      ret.set(value_c::types::trp_tx_teg_assoc);
      break;
    case 93:
      ret.set(value_c::types::trp_beam_ant_info);
      break;
    case 114:
      ret.set(value_c::types::mobile_trp_location_info);
      break;
    case 117:
      ret.set(value_c::types::common_ta_params);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_info_type_resp_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 62:
      return presence_e::mandatory;
    case 68:
      return presence_e::mandatory;
    case 84:
      return presence_e::mandatory;
    case 93:
      return presence_e::mandatory;
    case 114:
      return presence_e::mandatory;
    case 117:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void trp_info_type_resp_item_ext_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::trp_type:
      c = trp_type_e{};
      break;
    case types::on_demand_prs:
      c = on_demand_prs_info_s{};
      break;
    case types::trp_tx_teg_assoc:
      c = trp_tx_teg_assoc_l{};
      break;
    case types::trp_beam_ant_info:
      c = trp_beam_ant_info_s{};
      break;
    case types::mobile_trp_location_info:
      c = mobile_trp_location_info_s{};
      break;
    case types::common_ta_params:
      c = common_ta_params_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_ext_ies_o::value_c");
  }
}
trp_type_e& trp_info_type_resp_item_ext_ies_o::value_c::trp_type()
{
  assert_choice_type(types::trp_type, type_, "Value");
  return c.get<trp_type_e>();
}
on_demand_prs_info_s& trp_info_type_resp_item_ext_ies_o::value_c::on_demand_prs()
{
  assert_choice_type(types::on_demand_prs, type_, "Value");
  return c.get<on_demand_prs_info_s>();
}
trp_tx_teg_assoc_l& trp_info_type_resp_item_ext_ies_o::value_c::trp_tx_teg_assoc()
{
  assert_choice_type(types::trp_tx_teg_assoc, type_, "Value");
  return c.get<trp_tx_teg_assoc_l>();
}
trp_beam_ant_info_s& trp_info_type_resp_item_ext_ies_o::value_c::trp_beam_ant_info()
{
  assert_choice_type(types::trp_beam_ant_info, type_, "Value");
  return c.get<trp_beam_ant_info_s>();
}
mobile_trp_location_info_s& trp_info_type_resp_item_ext_ies_o::value_c::mobile_trp_location_info()
{
  assert_choice_type(types::mobile_trp_location_info, type_, "Value");
  return c.get<mobile_trp_location_info_s>();
}
common_ta_params_s& trp_info_type_resp_item_ext_ies_o::value_c::common_ta_params()
{
  assert_choice_type(types::common_ta_params, type_, "Value");
  return c.get<common_ta_params_s>();
}
const trp_type_e& trp_info_type_resp_item_ext_ies_o::value_c::trp_type() const
{
  assert_choice_type(types::trp_type, type_, "Value");
  return c.get<trp_type_e>();
}
const on_demand_prs_info_s& trp_info_type_resp_item_ext_ies_o::value_c::on_demand_prs() const
{
  assert_choice_type(types::on_demand_prs, type_, "Value");
  return c.get<on_demand_prs_info_s>();
}
const trp_tx_teg_assoc_l& trp_info_type_resp_item_ext_ies_o::value_c::trp_tx_teg_assoc() const
{
  assert_choice_type(types::trp_tx_teg_assoc, type_, "Value");
  return c.get<trp_tx_teg_assoc_l>();
}
const trp_beam_ant_info_s& trp_info_type_resp_item_ext_ies_o::value_c::trp_beam_ant_info() const
{
  assert_choice_type(types::trp_beam_ant_info, type_, "Value");
  return c.get<trp_beam_ant_info_s>();
}
const mobile_trp_location_info_s& trp_info_type_resp_item_ext_ies_o::value_c::mobile_trp_location_info() const
{
  assert_choice_type(types::mobile_trp_location_info, type_, "Value");
  return c.get<mobile_trp_location_info_s>();
}
const common_ta_params_s& trp_info_type_resp_item_ext_ies_o::value_c::common_ta_params() const
{
  assert_choice_type(types::common_ta_params, type_, "Value");
  return c.get<common_ta_params_s>();
}
void trp_info_type_resp_item_ext_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::trp_type:
      j.write_str(c.get<trp_type_e>().to_string());
      break;
    case types::on_demand_prs:
      c.get<on_demand_prs_info_s>().to_json(j);
      break;
    case types::trp_tx_teg_assoc:
      j.start_array("");
      for (const auto& e1 : c.get<trp_tx_teg_assoc_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::trp_beam_ant_info:
      c.get<trp_beam_ant_info_s>().to_json(j);
      break;
    case types::mobile_trp_location_info:
      c.get<mobile_trp_location_info_s>().to_json(j);
      break;
    case types::common_ta_params:
      c.get<common_ta_params_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_ext_ies_o::value_c");
  }
}
OCUDUASN_CODE trp_info_type_resp_item_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::trp_type:
      HANDLE_CODE(c.get<trp_type_e>().pack(bref));
      break;
    case types::on_demand_prs:
      HANDLE_CODE(c.get<on_demand_prs_info_s>().pack(bref));
      break;
    case types::trp_tx_teg_assoc:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_tx_teg_assoc_l>(), 1, 8, true));
      break;
    case types::trp_beam_ant_info:
      HANDLE_CODE(c.get<trp_beam_ant_info_s>().pack(bref));
      break;
    case types::mobile_trp_location_info:
      HANDLE_CODE(c.get<mobile_trp_location_info_s>().pack(bref));
      break;
    case types::common_ta_params:
      HANDLE_CODE(c.get<common_ta_params_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_ext_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_type_resp_item_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::trp_type:
      HANDLE_CODE(c.get<trp_type_e>().unpack(bref));
      break;
    case types::on_demand_prs:
      HANDLE_CODE(c.get<on_demand_prs_info_s>().unpack(bref));
      break;
    case types::trp_tx_teg_assoc:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_tx_teg_assoc_l>(), bref, 1, 8, true));
      break;
    case types::trp_beam_ant_info:
      HANDLE_CODE(c.get<trp_beam_ant_info_s>().unpack(bref));
      break;
    case types::mobile_trp_location_info:
      HANDLE_CODE(c.get<mobile_trp_location_info_s>().unpack(bref));
      break;
    case types::common_ta_params:
      HANDLE_CODE(c.get<common_ta_params_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_ext_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_info_type_resp_item_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TRPType",
                                "OnDemandPRS-Info",
                                "TRPTxTEGAssociation",
                                "TRPBeamAntennaInformation",
                                "Mobile-TRP-LocationInformation",
                                "CommonTAParameters"};
  return convert_enum_idx(names, 6, value, "trp_info_type_resp_item_ext_ies_o::value_c::types");
}

// TRPInformationTypeResponseItem ::= CHOICE
void trp_info_type_resp_item_c::destroy_()
{
  switch (type_) {
    case types::cgi_nr:
      c.destroy<cgi_nr_s>();
      break;
    case types::prs_cfg:
      c.destroy<prs_cfg_s>();
      break;
    case types::ss_binfo:
      c.destroy<ssb_info_s>();
      break;
    case types::sfn_initisation_time:
      c.destroy<fixed_bitstring<64, false, true>>();
      break;
    case types::spatial_direction_info:
      c.destroy<spatial_direction_info_s>();
      break;
    case types::geographical_coordinates:
      c.destroy<geographical_coordinates_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void trp_info_type_resp_item_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::pci_nr:
      break;
    case types::cgi_nr:
      c.init<cgi_nr_s>();
      break;
    case types::arfcn:
      break;
    case types::prs_cfg:
      c.init<prs_cfg_s>();
      break;
    case types::ss_binfo:
      c.init<ssb_info_s>();
      break;
    case types::sfn_initisation_time:
      c.init<fixed_bitstring<64, false, true>>();
      break;
    case types::spatial_direction_info:
      c.init<spatial_direction_info_s>();
      break;
    case types::geographical_coordinates:
      c.init<geographical_coordinates_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_c");
  }
}
trp_info_type_resp_item_c::trp_info_type_resp_item_c(const trp_info_type_resp_item_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::pci_nr:
      c.init(other.c.get<uint16_t>());
      break;
    case types::cgi_nr:
      c.init(other.c.get<cgi_nr_s>());
      break;
    case types::arfcn:
      c.init(other.c.get<uint32_t>());
      break;
    case types::prs_cfg:
      c.init(other.c.get<prs_cfg_s>());
      break;
    case types::ss_binfo:
      c.init(other.c.get<ssb_info_s>());
      break;
    case types::sfn_initisation_time:
      c.init(other.c.get<fixed_bitstring<64, false, true>>());
      break;
    case types::spatial_direction_info:
      c.init(other.c.get<spatial_direction_info_s>());
      break;
    case types::geographical_coordinates:
      c.init(other.c.get<geographical_coordinates_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_c");
  }
}
trp_info_type_resp_item_c& trp_info_type_resp_item_c::operator=(const trp_info_type_resp_item_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::pci_nr:
      c.set(other.c.get<uint16_t>());
      break;
    case types::cgi_nr:
      c.set(other.c.get<cgi_nr_s>());
      break;
    case types::arfcn:
      c.set(other.c.get<uint32_t>());
      break;
    case types::prs_cfg:
      c.set(other.c.get<prs_cfg_s>());
      break;
    case types::ss_binfo:
      c.set(other.c.get<ssb_info_s>());
      break;
    case types::sfn_initisation_time:
      c.set(other.c.get<fixed_bitstring<64, false, true>>());
      break;
    case types::spatial_direction_info:
      c.set(other.c.get<spatial_direction_info_s>());
      break;
    case types::geographical_coordinates:
      c.set(other.c.get<geographical_coordinates_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_c");
  }

  return *this;
}
uint16_t& trp_info_type_resp_item_c::set_pci_nr()
{
  set(types::pci_nr);
  return c.get<uint16_t>();
}
cgi_nr_s& trp_info_type_resp_item_c::set_cgi_nr()
{
  set(types::cgi_nr);
  return c.get<cgi_nr_s>();
}
uint32_t& trp_info_type_resp_item_c::set_arfcn()
{
  set(types::arfcn);
  return c.get<uint32_t>();
}
prs_cfg_s& trp_info_type_resp_item_c::set_prs_cfg()
{
  set(types::prs_cfg);
  return c.get<prs_cfg_s>();
}
ssb_info_s& trp_info_type_resp_item_c::set_ss_binfo()
{
  set(types::ss_binfo);
  return c.get<ssb_info_s>();
}
fixed_bitstring<64, false, true>& trp_info_type_resp_item_c::set_sfn_initisation_time()
{
  set(types::sfn_initisation_time);
  return c.get<fixed_bitstring<64, false, true>>();
}
spatial_direction_info_s& trp_info_type_resp_item_c::set_spatial_direction_info()
{
  set(types::spatial_direction_info);
  return c.get<spatial_direction_info_s>();
}
geographical_coordinates_s& trp_info_type_resp_item_c::set_geographical_coordinates()
{
  set(types::geographical_coordinates);
  return c.get<geographical_coordinates_s>();
}
protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>& trp_info_type_resp_item_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>();
}
void trp_info_type_resp_item_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::pci_nr:
      j.write_int("pCI-NR", c.get<uint16_t>());
      break;
    case types::cgi_nr:
      j.write_fieldname("cGI-NR");
      c.get<cgi_nr_s>().to_json(j);
      break;
    case types::arfcn:
      j.write_int("aRFCN", c.get<uint32_t>());
      break;
    case types::prs_cfg:
      j.write_fieldname("pRSConfiguration");
      c.get<prs_cfg_s>().to_json(j);
      break;
    case types::ss_binfo:
      j.write_fieldname("sSBinformation");
      c.get<ssb_info_s>().to_json(j);
      break;
    case types::sfn_initisation_time:
      j.write_fieldname("sFNInitialisationTime");
      c.get<fixed_bitstring<64, false, true>>().to_json(j);
      break;
    case types::spatial_direction_info:
      j.write_fieldname("spatialDirectionInformation");
      c.get<spatial_direction_info_s>().to_json(j);
      break;
    case types::geographical_coordinates:
      j.write_fieldname("geographicalCoordinates");
      c.get<geographical_coordinates_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_info_type_resp_item_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::pci_nr:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1007u, false, true));
      break;
    case types::cgi_nr:
      HANDLE_CODE(c.get<cgi_nr_s>().pack(bref));
      break;
    case types::arfcn:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)3279165u, false, true));
      break;
    case types::prs_cfg:
      HANDLE_CODE(c.get<prs_cfg_s>().pack(bref));
      break;
    case types::ss_binfo:
      HANDLE_CODE(c.get<ssb_info_s>().pack(bref));
      break;
    case types::sfn_initisation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    case types::spatial_direction_info:
      HANDLE_CODE(c.get<spatial_direction_info_s>().pack(bref));
      break;
    case types::geographical_coordinates:
      HANDLE_CODE(c.get<geographical_coordinates_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_type_resp_item_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::pci_nr:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1007u, false, true));
      break;
    case types::cgi_nr:
      HANDLE_CODE(c.get<cgi_nr_s>().unpack(bref));
      break;
    case types::arfcn:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)3279165u, false, true));
      break;
    case types::prs_cfg:
      HANDLE_CODE(c.get<prs_cfg_s>().unpack(bref));
      break;
    case types::ss_binfo:
      HANDLE_CODE(c.get<ssb_info_s>().unpack(bref));
      break;
    case types::sfn_initisation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    case types::spatial_direction_info:
      HANDLE_CODE(c.get<spatial_direction_info_s>().unpack(bref));
      break;
    case types::geographical_coordinates:
      HANDLE_CODE(c.get<geographical_coordinates_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_type_resp_item_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_info_type_resp_item_c::types_opts::to_string() const
{
  static const char* names[] = {"pCI-NR",
                                "cGI-NR",
                                "aRFCN",
                                "pRSConfiguration",
                                "sSBinformation",
                                "sFNInitialisationTime",
                                "spatialDirectionInformation",
                                "geographicalCoordinates",
                                "choice-extension"};
  return convert_enum_idx(names, 9, value, "trp_info_type_resp_item_c::types");
}

// TRPInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t trp_info_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {115};
  return map_enum_number(names, 1, idx, "id");
}
bool trp_info_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 115 == id;
}
crit_e trp_info_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 115) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
trp_info_ext_ies_o::ext_c trp_info_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 115) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_info_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 115) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void trp_info_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_str(c.to_string());
}
OCUDUASN_CODE trp_info_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* trp_info_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"OCTET STRING"};
  return convert_enum_idx(names, 1, value, "trp_info_ext_ies_o::ext_c::types");
}

// TRPInformation ::= SEQUENCE
OCUDUASN_CODE trp_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, trp_id, (uint32_t)1u, (uint32_t)65535u, true, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, trp_info_type_resp_list, 1, 64, true));
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_id, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  HANDLE_CODE(unpack_dyn_seq_of(trp_info_type_resp_list, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-ID", trp_id);
  j.start_array("tRPInformationTypeResponseList");
  for (const auto& e1 : trp_info_type_resp_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

OCUDUASN_CODE trp_info_list_trp_resp_item_s_::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(trp_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_list_trp_resp_item_s_::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(trp_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_info_list_trp_resp_item_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tRPInformation");
  trp_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRPInformationTypeItem ::= ENUMERATED
const char* trp_info_type_item_opts::to_string() const
{
  static const char* names[] = {"nrPCI",
                                "nG-RAN-CGI",
                                "arfcn",
                                "pRSConfig",
                                "sSBInfo",
                                "sFNInitTime",
                                "spatialDirectInfo",
                                "geoCoord",
                                "trp-type",
                                "ondemandPRSInfo",
                                "trpTxTeg",
                                "beam-antenna-info",
                                "mobile-trp-location-info",
                                "commonTA"};
  return convert_enum_idx(names, 14, value, "trp_info_type_item_e");
}

// TRPInformationTypeItemTRPReq ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t trp_info_type_item_trp_req_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57};
  return map_enum_number(names, 1, idx, "id");
}
bool trp_info_type_item_trp_req_o::is_id_valid(const uint32_t& id)
{
  return 57 == id;
}
crit_e trp_info_type_item_trp_req_o::get_crit(const uint32_t& id)
{
  if (id == 57) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
trp_info_type_item_trp_req_o::value_c trp_info_type_item_trp_req_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 57) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_info_type_item_trp_req_o::get_presence(const uint32_t& id)
{
  if (id == 57) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void trp_info_type_item_trp_req_o::value_c::to_json(json_writer& j) const
{
  j.write_str(c.to_string());
}
OCUDUASN_CODE trp_info_type_item_trp_req_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_type_item_trp_req_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* trp_info_type_item_trp_req_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TRPInformationTypeItem"};
  return convert_enum_idx(names, 1, value, "trp_info_type_item_trp_req_o::value_c::types");
}

// TRPItem ::= SEQUENCE
OCUDUASN_CODE trp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, trp_id, (uint32_t)1u, (uint32_t)65535u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(trp_id, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tRP-ID", trp_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TRPMeasurementType ::= ENUMERATED
const char* trp_meas_type_opts::to_string() const
{
  static const char* names[] = {
      "gNB-RxTxTimeDiff", "uL-SRS-RSRP", "uL-AoA", "uL-RTOA", "multiple-UL-AoA", "uL-SRS-RSRPP", "ul-RSCP"};
  return convert_enum_idx(names, 7, value, "trp_meas_type_e");
}

// TRPMeasurementQuantitiesList-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t trp_meas_quantities_list_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {123};
  return map_enum_number(names, 1, idx, "id");
}
bool trp_meas_quantities_list_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 123 == id;
}
crit_e trp_meas_quantities_list_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 123) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
trp_meas_quantities_list_item_ext_ies_o::ext_c trp_meas_quantities_list_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 123) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_meas_quantities_list_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 123) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void trp_meas_quantities_list_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_int(c);
}
OCUDUASN_CODE trp_meas_quantities_list_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_integer(bref, c, (int8_t)-6, (int8_t)-1, true, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_quantities_list_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_integer(c, bref, (int8_t)-6, (int8_t)-1, true, true));
  return OCUDUASN_SUCCESS;
}

const char* trp_meas_quantities_list_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (-6..-1,...)"};
  return convert_enum_idx(names, 1, value, "trp_meas_quantities_list_item_ext_ies_o::ext_c::types");
}
int8_t trp_meas_quantities_list_item_ext_ies_o::ext_c::types_opts::to_number() const
{
  static const int8_t numbers[] = {-6};
  return map_enum_number(numbers, 1, value, "trp_meas_quantities_list_item_ext_ies_o::ext_c::types");
}

// TRPMeasurementQuantitiesList-Item ::= SEQUENCE
OCUDUASN_CODE trp_meas_quantities_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(timing_report_granularity_factor_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(trp_meas_quantities_item.pack(bref));
  if (timing_report_granularity_factor_present) {
    HANDLE_CODE(pack_integer(bref, timing_report_granularity_factor, (uint8_t)0u, (uint8_t)5u, false, true));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_meas_quantities_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(timing_report_granularity_factor_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(trp_meas_quantities_item.unpack(bref));
  if (timing_report_granularity_factor_present) {
    HANDLE_CODE(unpack_integer(timing_report_granularity_factor, bref, (uint8_t)0u, (uint8_t)5u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trp_meas_quantities_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("tRPMeasurementQuantities-Item", trp_meas_quantities_item.to_string());
  if (timing_report_granularity_factor_present) {
    j.write_int("timingReportingGranularityFactor", timing_report_granularity_factor);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TimeWindowDurationMeasurement ::= CHOICE
void time_win_dur_meas_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void time_win_dur_meas_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::dur_slots:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_meas_c");
  }
}
time_win_dur_meas_c::time_win_dur_meas_c(const time_win_dur_meas_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::dur_slots:
      c.init(other.c.get<dur_slots_e_>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_meas_c");
  }
}
time_win_dur_meas_c& time_win_dur_meas_c::operator=(const time_win_dur_meas_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::dur_slots:
      c.set(other.c.get<dur_slots_e_>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_meas_c");
  }

  return *this;
}
time_win_dur_meas_c::dur_slots_e_& time_win_dur_meas_c::set_dur_slots()
{
  set(types::dur_slots);
  return c.get<dur_slots_e_>();
}
protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>& time_win_dur_meas_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>();
}
void time_win_dur_meas_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::dur_slots:
      j.write_str("durationSlots", c.get<dur_slots_e_>().to_string());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_meas_c");
  }
  j.end_obj();
}
OCUDUASN_CODE time_win_dur_meas_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::dur_slots:
      HANDLE_CODE(c.get<dur_slots_e_>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_meas_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_win_dur_meas_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::dur_slots:
      HANDLE_CODE(c.get<dur_slots_e_>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_meas_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* time_win_dur_meas_c::dur_slots_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4", "n6", "n8", "n12", "n16"};
  return convert_enum_idx(names, 7, value, "time_win_dur_meas_c::dur_slots_e_");
}
uint8_t time_win_dur_meas_c::dur_slots_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 6, 8, 12, 16};
  return map_enum_number(numbers, 7, value, "time_win_dur_meas_c::dur_slots_e_");
}

const char* time_win_dur_meas_c::types_opts::to_string() const
{
  static const char* names[] = {"durationSlots", "choice-extension"};
  return convert_enum_idx(names, 2, value, "time_win_dur_meas_c::types");
}

// TimeWindowDurationSRS ::= CHOICE
void time_win_dur_srs_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void time_win_dur_srs_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::dur_symbols:
      break;
    case types::dur_slots:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_srs_c");
  }
}
time_win_dur_srs_c::time_win_dur_srs_c(const time_win_dur_srs_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::dur_symbols:
      c.init(other.c.get<dur_symbols_e_>());
      break;
    case types::dur_slots:
      c.init(other.c.get<dur_slots_e_>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_srs_c");
  }
}
time_win_dur_srs_c& time_win_dur_srs_c::operator=(const time_win_dur_srs_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::dur_symbols:
      c.set(other.c.get<dur_symbols_e_>());
      break;
    case types::dur_slots:
      c.set(other.c.get<dur_slots_e_>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_srs_c");
  }

  return *this;
}
time_win_dur_srs_c::dur_symbols_e_& time_win_dur_srs_c::set_dur_symbols()
{
  set(types::dur_symbols);
  return c.get<dur_symbols_e_>();
}
time_win_dur_srs_c::dur_slots_e_& time_win_dur_srs_c::set_dur_slots()
{
  set(types::dur_slots);
  return c.get<dur_slots_e_>();
}
protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>& time_win_dur_srs_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>();
}
void time_win_dur_srs_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::dur_symbols:
      j.write_str("durationSymbols", c.get<dur_symbols_e_>().to_string());
      break;
    case types::dur_slots:
      j.write_str("durationSlots", c.get<dur_slots_e_>().to_string());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_srs_c");
  }
  j.end_obj();
}
OCUDUASN_CODE time_win_dur_srs_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::dur_symbols:
      HANDLE_CODE(c.get<dur_symbols_e_>().pack(bref));
      break;
    case types::dur_slots:
      HANDLE_CODE(c.get<dur_slots_e_>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_srs_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_win_dur_srs_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::dur_symbols:
      HANDLE_CODE(c.get<dur_symbols_e_>().unpack(bref));
      break;
    case types::dur_slots:
      HANDLE_CODE(c.get<dur_slots_e_>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "time_win_dur_srs_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* time_win_dur_srs_c::dur_symbols_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4", "n8", "n12"};
  return convert_enum_idx(names, 5, value, "time_win_dur_srs_c::dur_symbols_e_");
}
uint8_t time_win_dur_srs_c::dur_symbols_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 8, 12};
  return map_enum_number(numbers, 5, value, "time_win_dur_srs_c::dur_symbols_e_");
}

const char* time_win_dur_srs_c::dur_slots_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4", "n6", "n8", "n12", "n16"};
  return convert_enum_idx(names, 7, value, "time_win_dur_srs_c::dur_slots_e_");
}
uint8_t time_win_dur_srs_c::dur_slots_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 6, 8, 12, 16};
  return map_enum_number(numbers, 7, value, "time_win_dur_srs_c::dur_slots_e_");
}

const char* time_win_dur_srs_c::types_opts::to_string() const
{
  static const char* names[] = {"durationSymbols", "durationSlots", "choice-extension"};
  return convert_enum_idx(names, 3, value, "time_win_dur_srs_c::types");
}

// TimeWindowPeriodicityMeasurement ::= ENUMERATED
const char* time_win_periodicity_meas_opts::to_string() const
{
  static const char* names[] = {"ms160",
                                "ms320",
                                "ms640",
                                "ms1280",
                                "ms2560",
                                "ms5120",
                                "ms10240",
                                "ms20480",
                                "ms40960",
                                "ms61440",
                                "ms81920",
                                "ms368640",
                                "ms737280",
                                "ms1843200"};
  return convert_enum_idx(names, 14, value, "time_win_periodicity_meas_e");
}
uint32_t time_win_periodicity_meas_opts::to_number() const
{
  static const uint32_t numbers[] = {
      160, 320, 640, 1280, 2560, 5120, 10240, 20480, 40960, 61440, 81920, 368640, 737280, 1843200};
  return map_enum_number(numbers, 14, value, "time_win_periodicity_meas_e");
}

// TimeWindowStart ::= SEQUENCE
OCUDUASN_CODE time_win_start_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true));
  HANDLE_CODE(pack_integer(bref, slot_num, (uint8_t)0u, (uint8_t)79u, false, true));
  HANDLE_CODE(pack_integer(bref, symbol_idx, (uint8_t)0u, (uint8_t)13u, false, true));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_win_start_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true));
  HANDLE_CODE(unpack_integer(slot_num, bref, (uint8_t)0u, (uint8_t)79u, false, true));
  HANDLE_CODE(unpack_integer(symbol_idx, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void time_win_start_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("systemFrameNumber", sys_frame_num);
  j.write_int("slotNumber", slot_num);
  j.write_int("symbolIndex", symbol_idx);
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// TimeWindowInformation-Measurement-Item ::= SEQUENCE
OCUDUASN_CODE time_win_info_meas_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(time_win_periodicity_meas_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(time_win_dur_meas.pack(bref));
  HANDLE_CODE(time_win_type.pack(bref));
  if (time_win_periodicity_meas_present) {
    HANDLE_CODE(time_win_periodicity_meas.pack(bref));
  }
  HANDLE_CODE(time_win_start.pack(bref));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_win_info_meas_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(time_win_periodicity_meas_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(time_win_dur_meas.unpack(bref));
  HANDLE_CODE(time_win_type.unpack(bref));
  if (time_win_periodicity_meas_present) {
    HANDLE_CODE(time_win_periodicity_meas.unpack(bref));
  }
  HANDLE_CODE(time_win_start.unpack(bref));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void time_win_info_meas_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("timeWindowDurationMeasurement");
  time_win_dur_meas.to_json(j);
  j.write_str("timeWindowType", time_win_type.to_string());
  if (time_win_periodicity_meas_present) {
    j.write_str("timeWindowPeriodicityMeasurement", time_win_periodicity_meas.to_string());
  }
  j.write_fieldname("timeWindowStart");
  time_win_start.to_json(j);
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

const char* time_win_info_meas_item_s::time_win_type_opts::to_string() const
{
  static const char* names[] = {"single", "periodic"};
  return convert_enum_idx(names, 2, value, "time_win_info_meas_item_s::time_win_type_e_");
}

// TimeWindowStartSRS ::= SEQUENCE
OCUDUASN_CODE time_win_start_srs_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true));
  HANDLE_CODE(pack_integer(bref, slot_num, (uint8_t)0u, (uint8_t)79u, false, true));
  HANDLE_CODE(pack_integer(bref, symbol_idx, (uint8_t)0u, (uint8_t)13u, false, true));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_win_start_srs_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true));
  HANDLE_CODE(unpack_integer(slot_num, bref, (uint8_t)0u, (uint8_t)79u, false, true));
  HANDLE_CODE(unpack_integer(symbol_idx, bref, (uint8_t)0u, (uint8_t)13u, false, true));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void time_win_start_srs_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("systemFrameNumber", sys_frame_num);
  j.write_int("slotNumber", slot_num);
  j.write_int("symbolIndex", symbol_idx);
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// TimeWindowPeriodicitySRS ::= ENUMERATED
const char* time_win_periodicity_srs_opts::to_string() const
{
  static const char* names[] = {"ms0dot125", "ms0dot25", "ms0dot5", "ms0dot625", "ms1",   "ms1dot25", "ms2",
                                "ms2dot5",   "ms4",      "ms5",     "ms8",       "ms10",  "ms16",     "ms20",
                                "ms32",      "ms40",     "ms64",    "ms80",      "ms160", "ms320",    "ms640",
                                "ms1280",    "ms2560",   "ms5120",  "ms10240"};
  return convert_enum_idx(names, 25, value, "time_win_periodicity_srs_e");
}
float time_win_periodicity_srs_opts::to_number() const
{
  static const float numbers[] = {0.125, 0.25,  0.5,   0.625,  1.0,    1.25,   2.0,    2.5,  4.0,
                                  5.0,   8.0,   10.0,  16.0,   20.0,   32.0,   40.0,   64.0, 80.0,
                                  160.0, 320.0, 640.0, 1280.0, 2560.0, 5120.0, 10240.0};
  return map_enum_number(numbers, 25, value, "time_win_periodicity_srs_e");
}
const char* time_win_periodicity_srs_opts::to_number_string() const
{
  static const char* number_strs[] = {"0.125", "0.25", "0.5", "0.625", "1",    "1.25", "2",    "2.5", "4",
                                      "5",     "8",    "10",  "16",    "20",   "32",   "40",   "64",  "80",
                                      "160",   "320",  "640", "1280",  "2560", "5120", "10240"};
  return convert_enum_idx(number_strs, 25, value, "time_win_periodicity_srs_e");
}

// TimeWindowInformation-SRS-Item ::= SEQUENCE
OCUDUASN_CODE time_win_info_srs_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(time_win_periodicity_srs_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(time_win_start_srs.pack(bref));
  HANDLE_CODE(time_win_dur_srs.pack(bref));
  HANDLE_CODE(time_win_type.pack(bref));
  if (time_win_periodicity_srs_present) {
    HANDLE_CODE(time_win_periodicity_srs.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE time_win_info_srs_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(time_win_periodicity_srs_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(time_win_start_srs.unpack(bref));
  HANDLE_CODE(time_win_dur_srs.unpack(bref));
  HANDLE_CODE(time_win_type.unpack(bref));
  if (time_win_periodicity_srs_present) {
    HANDLE_CODE(time_win_periodicity_srs.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void time_win_info_srs_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("timeWindowStartSRS");
  time_win_start_srs.to_json(j);
  j.write_fieldname("timeWindowDurationSRS");
  time_win_dur_srs.to_json(j);
  j.write_str("timeWindowType", time_win_type.to_string());
  if (time_win_periodicity_srs_present) {
    j.write_str("timeWindowPeriodicitySRS", time_win_periodicity_srs.to_string());
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

const char* time_win_info_srs_item_s::time_win_type_opts::to_string() const
{
  static const char* names[] = {"single", "periodic"};
  return convert_enum_idx(names, 2, value, "time_win_info_srs_item_s::time_win_type_e_");
}

// UE-TEG-Info-Request ::= ENUMERATED
const char* ue_teg_info_request_opts::to_string() const
{
  static const char* names[] = {"onDemand", "periodic", "stop"};
  return convert_enum_idx(names, 3, value, "ue_teg_info_request_e");
}

// UE-TEG-ReportingPeriodicity ::= ENUMERATED
const char* ue_teg_report_periodicity_opts::to_string() const
{
  static const char* names[] = {"ms160", "ms320", "ms1280", "ms2560", "ms61440", "ms81920", "ms368640", "ms737280"};
  return convert_enum_idx(names, 8, value, "ue_teg_report_periodicity_e");
}
uint32_t ue_teg_report_periodicity_opts::to_number() const
{
  static const uint32_t numbers[] = {160, 320, 1280, 2560, 61440, 81920, 368640, 737280};
  return map_enum_number(numbers, 8, value, "ue_teg_report_periodicity_e");
}

// UEReportingInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t ue_report_info_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {158};
  return map_enum_number(names, 1, idx, "id");
}
bool ue_report_info_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 158 == id;
}
crit_e ue_report_info_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 158) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ue_report_info_ext_ies_o::ext_c ue_report_info_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 158) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ue_report_info_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 158) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void ue_report_info_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_int(c);
}
OCUDUASN_CODE ue_report_info_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_integer(bref, c, (uint16_t)1u, (uint16_t)999u, false, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_report_info_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_integer(c, bref, (uint16_t)1u, (uint16_t)999u, false, true));
  return OCUDUASN_SUCCESS;
}

const char* ue_report_info_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..999)"};
  return convert_enum_idx(names, 1, value, "ue_report_info_ext_ies_o::ext_c::types");
}
uint8_t ue_report_info_ext_ies_o::ext_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ue_report_info_ext_ies_o::ext_c::types");
}

// UEReportingInformation ::= SEQUENCE
OCUDUASN_CODE ue_report_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(report_amount.pack(bref));
  HANDLE_CODE(report_interv.pack(bref));
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_report_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(report_amount.unpack(bref));
  HANDLE_CODE(report_interv.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_report_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("reportingAmount", report_amount.to_string());
  j.write_str("reportingInterval", report_interv.to_string());
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* ue_report_info_s::report_amount_opts::to_string() const
{
  static const char* names[] = {"ma0", "ma1", "ma2", "ma4", "ma8", "ma16", "ma32", "ma64"};
  return convert_enum_idx(names, 8, value, "ue_report_info_s::report_amount_e_");
}
uint8_t ue_report_info_s::report_amount_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1, 2, 4, 8, 16, 32, 64};
  return map_enum_number(numbers, 8, value, "ue_report_info_s::report_amount_e_");
}

const char* ue_report_info_s::report_interv_opts::to_string() const
{
  static const char* names[] = {
      "none", "one", "two", "four", "eight", "ten", "sixteen", "twenty", "thirty-two", "sixty-four"};
  return convert_enum_idx(names, 10, value, "ue_report_info_s::report_interv_e_");
}

// UETxTEGAssociationItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t ue_tx_teg_assoc_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {104};
  return map_enum_number(names, 1, idx, "id");
}
bool ue_tx_teg_assoc_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 104 == id;
}
crit_e ue_tx_teg_assoc_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 104) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ue_tx_teg_assoc_item_ext_ies_o::ext_c ue_tx_teg_assoc_item_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 104) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ue_tx_teg_assoc_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 104) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void ue_tx_teg_assoc_item_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.write_str(c.to_string());
}
OCUDUASN_CODE ue_tx_teg_assoc_item_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_tx_teg_assoc_item_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ue_tx_teg_assoc_item_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"TimingErrorMargin"};
  return convert_enum_idx(names, 1, value, "ue_tx_teg_assoc_item_ext_ies_o::ext_c::types");
}

// UETxTEGAssociationItem ::= SEQUENCE
OCUDUASN_CODE ue_tx_teg_assoc_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(carrier_freq_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, ue_tx_teg_id, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, pos_srs_res_id_list, 1, 64, integer_packer<uint8_t>(0, 63, false, true), true));
  HANDLE_CODE(time_stamp.pack(bref));
  if (carrier_freq_present) {
    HANDLE_CODE(carrier_freq.pack(bref));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_tx_teg_assoc_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(carrier_freq_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ue_tx_teg_id, bref, (uint8_t)0u, (uint8_t)7u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(pos_srs_res_id_list, bref, 1, 64, integer_packer<uint8_t>(0, 63, false, true), true));
  HANDLE_CODE(time_stamp.unpack(bref));
  if (carrier_freq_present) {
    HANDLE_CODE(carrier_freq.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_tx_teg_assoc_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("uE-Tx-TEG-ID", ue_tx_teg_id);
  j.start_array("posSRSResourceID-List");
  for (const auto& e1 : pos_srs_res_id_list) {
    j.write_int(e1);
  }
  j.end_array();
  j.write_fieldname("timeStamp");
  time_stamp.to_json(j);
  if (carrier_freq_present) {
    j.write_fieldname("carrierFreq");
    carrier_freq.to_json(j);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// WLANBand ::= ENUMERATED
const char* wlan_band_opts::to_string() const
{
  static const char* names[] = {"band2dot4", "band5"};
  return convert_enum_idx(names, 2, value, "wlan_band_e");
}
float wlan_band_opts::to_number() const
{
  static const float numbers[] = {2.4, 5.0};
  return map_enum_number(numbers, 2, value, "wlan_band_e");
}
const char* wlan_band_opts::to_number_string() const
{
  static const char* number_strs[] = {"2.4", "5"};
  return convert_enum_idx(number_strs, 2, value, "wlan_band_e");
}

// WLANCountryCode ::= ENUMERATED
const char* wlan_country_code_opts::to_string() const
{
  static const char* names[] = {"unitedStates", "europe", "japan", "global"};
  return convert_enum_idx(names, 4, value, "wlan_country_code_e");
}

// WLANMeasurementQuantitiesValue ::= ENUMERATED
const char* wlan_meas_quantities_value_opts::to_string() const
{
  static const char* names[] = {"wlan"};
  return convert_enum_idx(names, 1, value, "wlan_meas_quantities_value_e");
}

// WLANMeasurementQuantities-Item ::= SEQUENCE
OCUDUASN_CODE wlan_meas_quantities_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(wlan_meas_quantities_value.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE wlan_meas_quantities_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(wlan_meas_quantities_value.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void wlan_meas_quantities_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("wLANMeasurementQuantitiesValue", "wlan");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// WLANMeasurementQuantities-ItemIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t wlan_meas_quantities_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {20};
  return map_enum_number(names, 1, idx, "id");
}
bool wlan_meas_quantities_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 20 == id;
}
crit_e wlan_meas_quantities_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 20) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
wlan_meas_quantities_item_ies_o::value_c wlan_meas_quantities_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 20) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e wlan_meas_quantities_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 20) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void wlan_meas_quantities_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE wlan_meas_quantities_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE wlan_meas_quantities_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* wlan_meas_quantities_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"WLANMeasurementQuantities-Item"};
  return convert_enum_idx(names, 1, value, "wlan_meas_quantities_item_ies_o::value_c::types");
}

// WLANMeasurementResult-Item ::= SEQUENCE
OCUDUASN_CODE wlan_meas_result_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ssi_d.size() > 0, 1));
  HANDLE_CODE(bref.pack(bs_si_d_present, 1));
  HANDLE_CODE(bref.pack(hes_si_d_present, 1));
  HANDLE_CODE(bref.pack(operating_class_present, 1));
  HANDLE_CODE(bref.pack(country_code_present, 1));
  HANDLE_CODE(bref.pack(wlan_ch_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(wlan_band_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, wlan_rssi, (uint16_t)0u, (uint16_t)141u, true, true));
  if (ssi_d.size() > 0) {
    HANDLE_CODE(ssi_d.pack(bref));
  }
  if (bs_si_d_present) {
    HANDLE_CODE(bs_si_d.pack(bref));
  }
  if (hes_si_d_present) {
    HANDLE_CODE(hes_si_d.pack(bref));
  }
  if (operating_class_present) {
    HANDLE_CODE(pack_integer(bref, operating_class, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (country_code_present) {
    HANDLE_CODE(country_code.pack(bref));
  }
  if (wlan_ch_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, wlan_ch_list, 1, 16, integer_packer<uint16_t>(0, 255, false, true), true));
  }
  if (wlan_band_present) {
    HANDLE_CODE(wlan_band.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE wlan_meas_result_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ssi_d_present;
  HANDLE_CODE(bref.unpack(ssi_d_present, 1));
  HANDLE_CODE(bref.unpack(bs_si_d_present, 1));
  HANDLE_CODE(bref.unpack(hes_si_d_present, 1));
  HANDLE_CODE(bref.unpack(operating_class_present, 1));
  HANDLE_CODE(bref.unpack(country_code_present, 1));
  bool wlan_ch_list_present;
  HANDLE_CODE(bref.unpack(wlan_ch_list_present, 1));
  HANDLE_CODE(bref.unpack(wlan_band_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(wlan_rssi, bref, (uint16_t)0u, (uint16_t)141u, true, true));
  if (ssi_d_present) {
    HANDLE_CODE(ssi_d.unpack(bref));
  }
  if (bs_si_d_present) {
    HANDLE_CODE(bs_si_d.unpack(bref));
  }
  if (hes_si_d_present) {
    HANDLE_CODE(hes_si_d.unpack(bref));
  }
  if (operating_class_present) {
    HANDLE_CODE(unpack_integer(operating_class, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (country_code_present) {
    HANDLE_CODE(country_code.unpack(bref));
  }
  if (wlan_ch_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(wlan_ch_list, bref, 1, 16, integer_packer<uint16_t>(0, 255, false, true), true));
  }
  if (wlan_band_present) {
    HANDLE_CODE(wlan_band.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void wlan_meas_result_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("wLAN-RSSI", wlan_rssi);
  if (ssi_d.size() > 0) {
    j.write_str("sSID", ssi_d.to_string());
  }
  if (bs_si_d_present) {
    j.write_str("bSSID", bs_si_d.to_string());
  }
  if (hes_si_d_present) {
    j.write_str("hESSID", hes_si_d.to_string());
  }
  if (operating_class_present) {
    j.write_int("operatingClass", operating_class);
  }
  if (country_code_present) {
    j.write_str("countryCode", country_code.to_string());
  }
  if (wlan_ch_list.size() > 0) {
    j.start_array("wLANChannelList");
    for (const auto& e1 : wlan_ch_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (wlan_band_present) {
    j.write_str("wLANBand", wlan_band.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}
