// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/*******************************************************************************
 *
 *                     3GPP TS ASN1 F1AP v18.8.0 (2025-12)
 *
 ******************************************************************************/

#pragma once

#include "ocudu/asn1/asn1_ap_utils.h"

namespace asn1 {
namespace f1ap {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// TriggeringMessage ::= ENUMERATED
struct trigger_msg_opts {
  enum options { init_msg, successful_outcome, unsuccessful_outcome, nulltype } value;

  const char* to_string() const;
};
using trigger_msg_e = enumerated<trigger_msg_opts>;

// LocationUncertainty-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using location_uncertainty_ext_ies_o = protocol_ext_empty_o;

using location_uncertainty_ext_ies_container = protocol_ext_container_empty_l;

// LocationUncertainty ::= SEQUENCE
struct location_uncertainty_s {
  bool                                   ie_exts_present        = false;
  uint16_t                               horizontal_uncertainty = 0;
  uint8_t                                horizontal_confidence  = 0;
  uint16_t                               vertical_uncertainty   = 0;
  uint8_t                                vertical_confidence    = 0;
  location_uncertainty_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RelativeGeodeticLocation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using relative_geodetic_location_ext_ies_o = protocol_ext_empty_o;

using relative_geodetic_location_ext_ies_container = protocol_ext_container_empty_l;

// RelativeGeodeticLocation ::= SEQUENCE
struct relative_geodetic_location_s {
  struct milli_arc_second_units_opts {
    enum options { zerodot03, zerodot3, three, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using milli_arc_second_units_e_ = enumerated<milli_arc_second_units_opts, true>;
  struct height_units_opts {
    enum options { mm, cm, m, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using height_units_e_ = enumerated<height_units_opts, true>;

  // member variables
  bool                                         ie_exts_present = false;
  milli_arc_second_units_e_                    milli_arc_second_units;
  height_units_e_                              height_units;
  int16_t                                      delta_latitude  = -1024;
  int16_t                                      delta_longitude = -1024;
  int16_t                                      delta_height    = -1024;
  location_uncertainty_s                       location_uncertainty;
  relative_geodetic_location_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RelativeCartesianLocation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using relative_cartesian_location_ext_ies_o = protocol_ext_empty_o;

using relative_cartesian_location_ext_ies_container = protocol_ext_container_empty_l;

// RelativeCartesianLocation ::= SEQUENCE
struct relative_cartesian_location_s {
  struct xy_zunit_opts {
    enum options { mm, cm, dm, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using xy_zunit_e_ = enumerated<xy_zunit_opts, true>;

  // member variables
  bool                                          ie_exts_present = false;
  xy_zunit_e_                                   xy_zunit;
  int32_t                                       xvalue = -65536;
  int32_t                                       yvalue = -65536;
  int32_t                                       zvalue = -32768;
  location_uncertainty_s                        location_uncertainty;
  relative_cartesian_location_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ARPLocationType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using arp_location_type_ext_ies_o = protocol_ies_empty_o;

// ARPLocationType ::= CHOICE
struct arp_location_type_c {
  struct types_opts {
    enum options { arp_position_relative_geodetic, arp_position_relative_cartesian, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  arp_location_type_c() = default;
  arp_location_type_c(const arp_location_type_c& other);
  arp_location_type_c& operator=(const arp_location_type_c& other);
  ~arp_location_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  relative_geodetic_location_s& arp_position_relative_geodetic()
  {
    assert_choice_type(types::arp_position_relative_geodetic, type_, "ARPLocationType");
    return c.get<relative_geodetic_location_s>();
  }
  relative_cartesian_location_s& arp_position_relative_cartesian()
  {
    assert_choice_type(types::arp_position_relative_cartesian, type_, "ARPLocationType");
    return c.get<relative_cartesian_location_s>();
  }
  protocol_ie_single_container_s<arp_location_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ARPLocationType");
    return c.get<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>();
  }
  const relative_geodetic_location_s& arp_position_relative_geodetic() const
  {
    assert_choice_type(types::arp_position_relative_geodetic, type_, "ARPLocationType");
    return c.get<relative_geodetic_location_s>();
  }
  const relative_cartesian_location_s& arp_position_relative_cartesian() const
  {
    assert_choice_type(types::arp_position_relative_cartesian, type_, "ARPLocationType");
    return c.get<relative_cartesian_location_s>();
  }
  const protocol_ie_single_container_s<arp_location_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ARPLocationType");
    return c.get<protocol_ie_single_container_s<arp_location_type_ext_ies_o>>();
  }
  relative_geodetic_location_s&                                set_arp_position_relative_geodetic();
  relative_cartesian_location_s&                               set_arp_position_relative_cartesian();
  protocol_ie_single_container_s<arp_location_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<arp_location_type_ext_ies_o>,
                  relative_cartesian_location_s,
                  relative_geodetic_location_s>
      c;

  void destroy_();
};

// ARPLocationInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using arp_location_info_ext_ies_o = protocol_ext_empty_o;

using arp_location_info_ext_ies_container = protocol_ext_container_empty_l;

// ARPLocationInformation-Item ::= SEQUENCE
struct arp_location_info_item_s {
  bool                                ie_exts_present = false;
  uint8_t                             arp_id          = 1;
  arp_location_type_c                 arp_location_type;
  arp_location_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ARPLocationInformation ::= SEQUENCE (SIZE (1..16)) OF ARPLocationInformation-Item
using arp_location_info_l = dyn_array<arp_location_info_item_s>;

// SCS-SpecificCarrier-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using scs_specific_carrier_ext_ies_o = protocol_ext_empty_o;

using scs_specific_carrier_ext_ies_container = protocol_ext_container_empty_l;

// SCS-SpecificCarrier ::= SEQUENCE
struct scs_specific_carrier_s {
  struct subcarrier_spacing_opts {
    enum options { khz15, khz30, khz60, khz120, /*...*/ khz480, khz960, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using subcarrier_spacing_e_ = enumerated<subcarrier_spacing_opts, true, 2>;

  // member variables
  bool                                   ie_exts_present   = false;
  uint16_t                               offset_to_carrier = 0;
  subcarrier_spacing_e_                  subcarrier_spacing;
  uint16_t                               carrier_bw = 1;
  scs_specific_carrier_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Combined-PosSRSResourceSet-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using combined_pos_srs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using combined_pos_srs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// Combined-PosSRSResourceSet-Item ::= SEQUENCE
struct combined_pos_srs_res_set_item_s {
  bool                                            nr_pci_present     = false;
  bool                                            ie_exts_present    = false;
  uint32_t                                        point_a            = 0;
  uint16_t                                        nr_pci             = 0;
  uint8_t                                         pos_srs_res_set_id = 0;
  scs_specific_carrier_s                          scs_specific_carrier;
  combined_pos_srs_res_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Combined-PosSRSResourceSet-List ::= SEQUENCE (SIZE (2..3)) OF Combined-PosSRSResourceSet-Item
using combined_pos_srs_res_set_list_l = dyn_array<combined_pos_srs_res_set_item_s>;

// AggregatedPosSRSResourceSet-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using aggr_pos_srs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using aggr_pos_srs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// AggregatedPosSRSResourceSet-Item ::= SEQUENCE
struct aggr_pos_srs_res_set_item_s {
  bool                                        ie_exts_present = false;
  combined_pos_srs_res_set_list_l             combined_pos_srs_res_set_list;
  aggr_pos_srs_res_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AggregatedPosSRSResourceSetList ::= SEQUENCE (SIZE (1..32)) OF AggregatedPosSRSResourceSet-Item
using aggr_pos_srs_res_set_list_l = dyn_array<aggr_pos_srs_res_set_item_s>;

// AbortTransmission-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct abort_tx_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { aggr_pos_srs_res_set_list, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::aggr_pos_srs_res_set_list; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    aggr_pos_srs_res_set_list_l&       aggr_pos_srs_res_set_list() { return c; }
    const aggr_pos_srs_res_set_list_l& aggr_pos_srs_res_set_list() const { return c; }

  private:
    aggr_pos_srs_res_set_list_l c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// AbortTransmission ::= CHOICE
struct abort_tx_c {
  struct types_opts {
    enum options { srs_res_set_id, release_all, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  abort_tx_c() = default;
  abort_tx_c(const abort_tx_c& other);
  abort_tx_c& operator=(const abort_tx_c& other);
  ~abort_tx_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& srs_res_set_id()
  {
    assert_choice_type(types::srs_res_set_id, type_, "AbortTransmission");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<abort_tx_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "AbortTransmission");
    return c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>();
  }
  const uint8_t& srs_res_set_id() const
  {
    assert_choice_type(types::srs_res_set_id, type_, "AbortTransmission");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<abort_tx_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "AbortTransmission");
    return c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>();
  }
  uint8_t&                                            set_srs_res_set_id();
  void                                                set_release_all();
  protocol_ie_single_container_s<abort_tx_ext_ies_o>& set_choice_ext();

private:
  types                                                               type_;
  choice_buffer_t<protocol_ie_single_container_s<abort_tx_ext_ies_o>> c;

  void destroy_();
};

// AccessPointPosition-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using access_point_position_ext_ies_o = protocol_ext_empty_o;

using access_point_position_ext_ies_container = protocol_ext_container_empty_l;

// AccessPointPosition ::= SEQUENCE
struct access_point_position_s {
  struct latitude_sign_opts {
    enum options { north, south, nulltype } value;

    const char* to_string() const;
  };
  using latitude_sign_e_ = enumerated<latitude_sign_opts>;
  struct direction_of_altitude_opts {
    enum options { height, depth, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using direction_of_altitude_e_ = enumerated<direction_of_altitude_opts>;

  // member variables
  bool                                    ie_exts_present = false;
  latitude_sign_e_                        latitude_sign;
  uint32_t                                latitude  = 0;
  int32_t                                 longitude = -8388608;
  direction_of_altitude_e_                direction_of_altitude;
  uint16_t                                altitude                  = 0;
  uint8_t                                 uncertainty_semi_major    = 0;
  uint8_t                                 uncertainty_semi_minor    = 0;
  uint8_t                                 orientation_of_major_axis = 0;
  uint8_t                                 uncertainty_altitude      = 0;
  uint8_t                                 confidence                = 0;
  access_point_position_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRCGI-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_cgi_ext_ies_o = protocol_ext_empty_o;

using nr_cgi_ext_ies_container = protocol_ext_container_empty_l;

// NRCGI ::= SEQUENCE
struct nr_cgi_s {
  bool                             ie_exts_present = false;
  fixed_octstring<3, true>         plmn_id;
  fixed_bitstring<36, false, true> nr_cell_id;
  nr_cgi_ext_ies_container         ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Activated-Cells-Mapping-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using activ_cells_map_list_item_ext_ies_o = protocol_ext_empty_o;

using activ_cells_map_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Activated-Cells-Mapping-List-Item ::= SEQUENCE
struct activ_cells_map_list_item_s {
  bool                                        ie_exts_present = false;
  nr_cgi_s                                    nr_cgi_for_target_lc_du;
  nr_cgi_s                                    nr_cgi_for_source_lc_du;
  activ_cells_map_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SubcarrierSpacing ::= ENUMERATED
struct subcarrier_spacing_opts {
  enum options { khz15, khz30, khz60, khz120, khz240, spare3, spare2, spare1, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using subcarrier_spacing_e = enumerated<subcarrier_spacing_opts, true>;

// DUFTransmissionPeriodicity ::= ENUMERATED
struct du_f_tx_periodicity_opts {
  enum options { ms0p5, ms0p625, ms1, ms1p25, ms2, ms2p5, ms5, ms10, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_f_tx_periodicity_e = enumerated<du_f_tx_periodicity_opts, true>;

// Permutation ::= ENUMERATED
struct permutation_opts {
  enum options { dfu, ufd, /*...*/ nulltype } value;

  const char* to_string() const;
};
using permutation_e = enumerated<permutation_opts, true>;

// ExplicitFormat-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using explicit_format_ext_ies_o = protocol_ext_empty_o;

using explicit_format_ext_ies_container = protocol_ext_container_empty_l;

// ExplicitFormat ::= SEQUENCE
struct explicit_format_s {
  bool                              noof_dl_symbols_present = false;
  bool                              noof_ul_symbols_present = false;
  bool                              ie_exts_present         = false;
  permutation_e                     permutation;
  uint8_t                           noof_dl_symbols = 0;
  uint8_t                           noof_ul_symbols = 0;
  explicit_format_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ImplicitFormat-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using implicit_format_ext_ies_o = protocol_ext_empty_o;

using implicit_format_ext_ies_container = protocol_ext_container_empty_l;

// ImplicitFormat ::= SEQUENCE
struct implicit_format_s {
  bool                              ie_exts_present     = false;
  uint8_t                           du_f_slotformat_idx = 0;
  implicit_format_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DUF-Slot-Config-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using du_f_slot_cfg_item_ext_ies_o = protocol_ies_empty_o;

// DUF-Slot-Config-Item ::= CHOICE
struct du_f_slot_cfg_item_c {
  struct types_opts {
    enum options { explicit_format, implicit_format, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  du_f_slot_cfg_item_c() = default;
  du_f_slot_cfg_item_c(const du_f_slot_cfg_item_c& other);
  du_f_slot_cfg_item_c& operator=(const du_f_slot_cfg_item_c& other);
  ~du_f_slot_cfg_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  explicit_format_s& explicit_format()
  {
    assert_choice_type(types::explicit_format, type_, "DUF-Slot-Config-Item");
    return c.get<explicit_format_s>();
  }
  implicit_format_s& implicit_format()
  {
    assert_choice_type(types::implicit_format, type_, "DUF-Slot-Config-Item");
    return c.get<implicit_format_s>();
  }
  protocol_ie_single_container_s<du_f_slot_cfg_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "DUF-Slot-Config-Item");
    return c.get<protocol_ie_single_container_s<du_f_slot_cfg_item_ext_ies_o>>();
  }
  const explicit_format_s& explicit_format() const
  {
    assert_choice_type(types::explicit_format, type_, "DUF-Slot-Config-Item");
    return c.get<explicit_format_s>();
  }
  const implicit_format_s& implicit_format() const
  {
    assert_choice_type(types::implicit_format, type_, "DUF-Slot-Config-Item");
    return c.get<implicit_format_s>();
  }
  const protocol_ie_single_container_s<du_f_slot_cfg_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "DUF-Slot-Config-Item");
    return c.get<protocol_ie_single_container_s<du_f_slot_cfg_item_ext_ies_o>>();
  }
  explicit_format_s&                                            set_explicit_format();
  implicit_format_s&                                            set_implicit_format();
  protocol_ie_single_container_s<du_f_slot_cfg_item_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<explicit_format_s, implicit_format_s, protocol_ie_single_container_s<du_f_slot_cfg_item_ext_ies_o>> c;

  void destroy_();
};

// DUF-Slot-Config-List ::= SEQUENCE (SIZE (1..320)) OF DUF-Slot-Config-Item
using du_f_slot_cfg_list_l = dyn_array<du_f_slot_cfg_item_c>;

// HSNATransmissionPeriodicity ::= ENUMERATED
struct h_sn_a_tx_periodicity_opts {
  enum options { ms0p5, ms0p625, ms1, ms1p25, ms2, ms2p5, ms5, ms10, ms20, ms40, ms80, ms160, /*...*/ nulltype } value;

  const char* to_string() const;
};
using h_sn_a_tx_periodicity_e = enumerated<h_sn_a_tx_periodicity_opts, true>;

// HSNADownlink ::= ENUMERATED
struct h_sn_a_dl_opts {
  enum options { hard, soft, notavailable, nulltype } value;

  const char* to_string() const;
};
using h_sn_a_dl_e = enumerated<h_sn_a_dl_opts>;

// HSNAUplink ::= ENUMERATED
struct h_sn_a_ul_opts {
  enum options { hard, soft, notavailable, nulltype } value;

  const char* to_string() const;
};
using h_sn_a_ul_e = enumerated<h_sn_a_ul_opts>;

// HSNAFlexible ::= ENUMERATED
struct h_sn_a_flex_opts {
  enum options { hard, soft, notavailable, nulltype } value;

  const char* to_string() const;
};
using h_sn_a_flex_e = enumerated<h_sn_a_flex_opts>;

// HSNASlotConfigItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using hs_nas_lot_cfg_item_ext_ies_o = protocol_ext_empty_o;

using hs_nas_lot_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// HSNASlotConfigItem ::= SEQUENCE
struct hs_nas_lot_cfg_item_s {
  bool                                  hsn_a_dl_present   = false;
  bool                                  hsn_a_ul_present   = false;
  bool                                  hsn_a_flex_present = false;
  bool                                  ie_exts_present    = false;
  h_sn_a_dl_e                           hsn_a_dl;
  h_sn_a_ul_e                           hsn_a_ul;
  h_sn_a_flex_e                         hsn_a_flex;
  hs_nas_lot_cfg_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HSNASlotConfigList ::= SEQUENCE (SIZE (1..5120)) OF HSNASlotConfigItem
using hs_nas_lot_cfg_list_l = dyn_array<hs_nas_lot_cfg_item_s>;

// RBSetSize ::= ENUMERATED
struct rb_set_size_opts {
  enum options { rb2, rb4, rb8, rb16, rb32, rb64, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using rb_set_size_e = enumerated<rb_set_size_opts>;

// RBSetConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rb_set_cfg_ext_ies_o = protocol_ext_empty_o;

using rb_set_cfg_ext_ies_container = protocol_ext_container_empty_l;

// RBSetConfiguration ::= SEQUENCE
struct rb_set_cfg_s {
  bool                         ie_exts_present = false;
  subcarrier_spacing_e         subcarrier_spacing;
  rb_set_size_e                rb_set_size;
  uint8_t                      num_r_bsets = 1;
  rb_set_cfg_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Frequency-Domain-HSNA-Slot-Configuration-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using freq_domain_h_sn_a_slot_cfg_item_ext_ies_o = protocol_ext_empty_o;

using freq_domain_h_sn_a_slot_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// Frequency-Domain-HSNA-Slot-Configuration-Item ::= SEQUENCE
struct freq_domain_h_sn_a_slot_cfg_item_s {
  bool                                               slot_idx_present   = false;
  bool                                               hsn_a_dl_present   = false;
  bool                                               hsn_a_ul_present   = false;
  bool                                               hsn_a_flex_present = false;
  bool                                               ie_exts_present    = false;
  uint16_t                                           slot_idx           = 0;
  h_sn_a_dl_e                                        hsn_a_dl;
  h_sn_a_ul_e                                        hsn_a_ul;
  h_sn_a_flex_e                                      hsn_a_flex;
  freq_domain_h_sn_a_slot_cfg_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Frequency-Domain-HSNA-Slot-Configuration-List ::= SEQUENCE (SIZE (1..5120)) OF
// Frequency-Domain-HSNA-Slot-Configuration-Item
using freq_domain_h_sn_a_slot_cfg_list_l = dyn_array<freq_domain_h_sn_a_slot_cfg_item_s>;

// Frequency-Domain-HSNA-Configuration-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using freq_domain_h_sn_a_cfg_item_ext_ies_o = protocol_ext_empty_o;

using freq_domain_h_sn_a_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// Frequency-Domain-HSNA-Configuration-Item ::= SEQUENCE
struct freq_domain_h_sn_a_cfg_item_s {
  bool                                          ie_exts_present = false;
  uint8_t                                       rb_set_idx      = 0;
  freq_domain_h_sn_a_slot_cfg_list_l            freq_domain_h_sn_a_slot_cfg_list;
  freq_domain_h_sn_a_cfg_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Frequency-Domain-HSNA-Configuration-List ::= SEQUENCE (SIZE (1..8)) OF Frequency-Domain-HSNA-Configuration-Item
using freq_domain_h_sn_a_cfg_list_l = dyn_array<freq_domain_h_sn_a_cfg_item_s>;

// NADownlink ::= ENUMERATED
struct na_dl_opts {
  enum options { true_value, false_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using na_dl_e = enumerated<na_dl_opts, true>;

// NAUplink ::= ENUMERATED
struct na_ul_opts {
  enum options { true_value, false_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using na_ul_e = enumerated<na_ul_opts, true>;

// NAFlexible ::= ENUMERATED
struct na_flex_opts {
  enum options { true_value, false_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using na_flex_e = enumerated<na_flex_opts, true>;

// NA-Resource-Configuration-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using na_res_cfg_item_ext_ies_o = protocol_ext_empty_o;

using na_res_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// NA-Resource-Configuration-Item ::= SEQUENCE
struct na_res_cfg_item_s {
  bool                              na_dl_present   = false;
  bool                              na_ul_present   = false;
  bool                              na_flex_present = false;
  bool                              ie_exts_present = false;
  na_dl_e                           na_dl;
  na_ul_e                           na_ul;
  na_flex_e                         na_flex;
  na_res_cfg_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NA-Resource-Configuration-List ::= SEQUENCE (SIZE (1..5120)) OF NA-Resource-Configuration-Item
using na_res_cfg_list_l = dyn_array<na_res_cfg_item_s>;

// Child-IAB-Nodes-NA-Resource-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using child_iab_nodes_na_res_list_item_ext_ies_o = protocol_ext_empty_o;

using child_iab_nodes_na_res_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Child-IAB-Nodes-NA-Resource-List-Item ::= SEQUENCE
struct child_iab_nodes_na_res_list_item_s {
  bool                                               ie_exts_present   = false;
  uint64_t                                           gnb_cu_ue_f1ap_id = 0;
  uint64_t                                           gnb_du_ue_f1ap_id = 0;
  na_res_cfg_list_l                                  na_res_cfg_list;
  child_iab_nodes_na_res_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Child-IAB-Nodes-NA-Resource-List ::= SEQUENCE (SIZE (1..1024)) OF Child-IAB-Nodes-NA-Resource-List-Item
using child_iab_nodes_na_res_list_l = dyn_array<child_iab_nodes_na_res_list_item_s>;

// Parent-IAB-Nodes-NA-Resource-Configuration-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using parent_iab_nodes_na_res_cfg_item_ext_ies_o = protocol_ext_empty_o;

using parent_iab_nodes_na_res_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// Parent-IAB-Nodes-NA-Resource-Configuration-Item ::= SEQUENCE
struct parent_iab_nodes_na_res_cfg_item_s {
  bool                                               na_dl_present   = false;
  bool                                               na_ul_present   = false;
  bool                                               na_flex_present = false;
  bool                                               ie_exts_present = false;
  na_dl_e                                            na_dl;
  na_ul_e                                            na_ul;
  na_flex_e                                          na_flex;
  parent_iab_nodes_na_res_cfg_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Parent-IAB-Nodes-NA-Resource-Configuration-List ::= SEQUENCE (SIZE (1..5120)) OF
// Parent-IAB-Nodes-NA-Resource-Configuration-Item
using parent_iab_nodes_na_res_cfg_list_l = dyn_array<parent_iab_nodes_na_res_cfg_item_s>;

// GNB-DU-Cell-Resource-Configuration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct gnb_du_cell_res_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct gnb_du_cell_res_cfg_ext_ies_container {
  bool                               rb_set_cfg_present                       = false;
  bool                               freq_domain_h_sn_a_cfg_list_present      = false;
  bool                               child_iab_nodes_na_res_list_present      = false;
  bool                               parent_iab_nodes_na_res_cfg_list_present = false;
  rb_set_cfg_s                       rb_set_cfg;
  freq_domain_h_sn_a_cfg_list_l      freq_domain_h_sn_a_cfg_list;
  child_iab_nodes_na_res_list_l      child_iab_nodes_na_res_list;
  parent_iab_nodes_na_res_cfg_list_l parent_iab_nodes_na_res_cfg_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-DU-Cell-Resource-Configuration ::= SEQUENCE
struct gnb_du_cell_res_cfg_s {
  bool                                  du_f_tx_periodicity_present = false;
  bool                                  ie_exts_present             = false;
  subcarrier_spacing_e                  subcarrier_spacing;
  du_f_tx_periodicity_e                 du_f_tx_periodicity;
  du_f_slot_cfg_list_l                  du_f_slot_cfg_list;
  h_sn_a_tx_periodicity_e               hsn_a_tx_periodicity;
  hs_nas_lot_cfg_list_l                 hs_nsa_slot_cfg_list;
  gnb_du_cell_res_cfg_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRSCS ::= ENUMERATED
struct nr_scs_opts {
  enum options { scs15, scs30, scs60, scs120, /*...*/ scs480, scs960, nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using nr_scs_e = enumerated<nr_scs_opts, true, 2>;

// NRNRB ::= ENUMERATED
struct nr_nrb_opts {
  enum options {
    nrb11,
    nrb18,
    nrb24,
    nrb25,
    nrb31,
    nrb32,
    nrb38,
    nrb51,
    nrb52,
    nrb65,
    nrb66,
    nrb78,
    nrb79,
    nrb93,
    nrb106,
    nrb107,
    nrb121,
    nrb132,
    nrb133,
    nrb135,
    nrb160,
    nrb162,
    nrb189,
    nrb216,
    nrb217,
    nrb245,
    nrb264,
    nrb270,
    nrb273,
    // ...
    nrb33,
    nrb62,
    nrb124,
    nrb148,
    nrb248,
    nrb44,
    nrb58,
    nrb92,
    nrb119,
    nrb188,
    nrb242,
    nrb15,
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using nr_nrb_e = enumerated<nr_nrb_opts, true, 12>;

// Transmission-Bandwidth-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tx_bw_ext_ies_o = protocol_ext_empty_o;

using tx_bw_ext_ies_container = protocol_ext_container_empty_l;

// Transmission-Bandwidth ::= SEQUENCE
struct tx_bw_s {
  bool                    ie_exts_present = false;
  nr_scs_e                nr_scs;
  nr_nrb_e                nr_nrb;
  tx_bw_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRCarrierItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_carrier_item_ext_ies_o = protocol_ext_empty_o;

using nr_carrier_item_ext_ies_container = protocol_ext_container_empty_l;

// NRCarrierItem ::= SEQUENCE
struct nr_carrier_item_s {
  bool                              ie_ext_present = false;
  nr_scs_e                          carrier_scs;
  uint16_t                          offset_to_carrier = 0;
  uint16_t                          carrier_bw        = 0;
  nr_carrier_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRCarrierList ::= SEQUENCE (SIZE (1..5)) OF NRCarrierItem
using nr_carrier_list_l = dyn_array<nr_carrier_item_s>;

// FrequencyShift7p5khz ::= ENUMERATED
struct freq_shift7p5khz_opts {
  enum options { false_value, true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using freq_shift7p5khz_e = enumerated<freq_shift7p5khz_opts, true>;

// SUL-InformationExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct sul_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sul_info_ext_ies_container {
  bool               carrier_list_present     = false;
  bool               freq_shift7p5khz_present = false;
  nr_carrier_list_l  carrier_list;
  freq_shift7p5khz_e freq_shift7p5khz;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SUL-Information ::= SEQUENCE
struct sul_info_s {
  bool                       ie_exts_present = false;
  uint32_t                   sul_nr_arfcn    = 0;
  tx_bw_s                    sul_tx_bw;
  sul_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SupportedSULFreqBandItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using supported_sul_freq_band_item_ext_ies_o = protocol_ext_empty_o;

using supported_sul_freq_band_item_ext_ies_container = protocol_ext_container_empty_l;

// SupportedSULFreqBandItem ::= SEQUENCE
struct supported_sul_freq_band_item_s {
  bool                                           ie_exts_present  = false;
  uint16_t                                       freq_band_ind_nr = 1;
  supported_sul_freq_band_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FreqBandNrItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using freq_band_nr_item_ext_ies_o = protocol_ext_empty_o;

using freq_band_nr_item_ext_ies_container = protocol_ext_container_empty_l;

// FreqBandNrItem ::= SEQUENCE
struct freq_band_nr_item_s {
  using supported_sul_band_list_l_ = dyn_array<supported_sul_freq_band_item_s>;

  // member variables
  bool                                ie_exts_present  = false;
  uint16_t                            freq_band_ind_nr = 1;
  supported_sul_band_list_l_          supported_sul_band_list;
  freq_band_nr_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRFreqInfoExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct nr_freq_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct nr_freq_info_ext_ies_container {
  bool               freq_shift7p5khz_present = false;
  freq_shift7p5khz_e freq_shift7p5khz;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRFreqInfo ::= SEQUENCE
struct nr_freq_info_s {
  using freq_band_list_nr_l_ = dyn_array<freq_band_nr_item_s>;

  // member variables
  bool                           sul_info_present = false;
  bool                           ie_exts_present  = false;
  uint32_t                       nr_arfcn         = 0;
  sul_info_s                     sul_info;
  freq_band_list_nr_l_           freq_band_list_nr;
  nr_freq_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-DU-Cell-Resource-Configuration-FDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct iab_du_cell_res_cfg_fdd_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_du_cell_res_cfg_fdd_info_ext_ies_container {
  bool              ul_freq_info_present       = false;
  bool              ul_tx_bw_present           = false;
  bool              ul_nr_carrier_list_present = false;
  bool              dl_freq_info_present       = false;
  bool              dl_tx_bw_present           = false;
  bool              dl_nr_carrier_list_present = false;
  nr_freq_info_s    ul_freq_info;
  tx_bw_s           ul_tx_bw;
  nr_carrier_list_l ul_nr_carrier_list;
  nr_freq_info_s    dl_freq_info;
  tx_bw_s           dl_tx_bw;
  nr_carrier_list_l dl_nr_carrier_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-DU-Cell-Resource-Configuration-FDD-Info ::= SEQUENCE
struct iab_du_cell_res_cfg_fdd_info_s {
  bool                                           ie_exts_present = false;
  gnb_du_cell_res_cfg_s                          gnb_du_cell_res_cfg_fdd_ul;
  gnb_du_cell_res_cfg_s                          gnb_du_cell_res_cfg_fdd_dl;
  iab_du_cell_res_cfg_fdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-DU-Cell-Resource-Configuration-TDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct iab_du_cell_res_cfg_tdd_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_du_cell_res_cfg_tdd_info_ext_ies_container {
  bool              nr_freq_info_present    = false;
  bool              tx_bw_present           = false;
  bool              nr_carrier_list_present = false;
  nr_freq_info_s    nr_freq_info;
  tx_bw_s           tx_bw;
  nr_carrier_list_l nr_carrier_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-DU-Cell-Resource-Configuration-TDD-Info ::= SEQUENCE
struct iab_du_cell_res_cfg_tdd_info_s {
  bool                                           ie_exts_present = false;
  gnb_du_cell_res_cfg_s                          gnb_du_cell_resourc_cfg_tdd;
  iab_du_cell_res_cfg_tdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-DU-Cell-Resource-Configuration-Mode-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using iab_du_cell_res_cfg_mode_info_ext_ies_o = protocol_ies_empty_o;

// IAB-DU-Cell-Resource-Configuration-Mode-Info ::= CHOICE
struct iab_du_cell_res_cfg_mode_info_c {
  struct types_opts {
    enum options { fdd, tdd, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  iab_du_cell_res_cfg_mode_info_c() = default;
  iab_du_cell_res_cfg_mode_info_c(const iab_du_cell_res_cfg_mode_info_c& other);
  iab_du_cell_res_cfg_mode_info_c& operator=(const iab_du_cell_res_cfg_mode_info_c& other);
  ~iab_du_cell_res_cfg_mode_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  iab_du_cell_res_cfg_fdd_info_s& fdd()
  {
    assert_choice_type(types::fdd, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<iab_du_cell_res_cfg_fdd_info_s>();
  }
  iab_du_cell_res_cfg_tdd_info_s& tdd()
  {
    assert_choice_type(types::tdd, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<iab_du_cell_res_cfg_tdd_info_s>();
  }
  protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>>();
  }
  const iab_du_cell_res_cfg_fdd_info_s& fdd() const
  {
    assert_choice_type(types::fdd, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<iab_du_cell_res_cfg_fdd_info_s>();
  }
  const iab_du_cell_res_cfg_tdd_info_s& tdd() const
  {
    assert_choice_type(types::tdd, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<iab_du_cell_res_cfg_tdd_info_s>();
  }
  const protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>>();
  }
  iab_du_cell_res_cfg_fdd_info_s&                                          set_fdd();
  iab_du_cell_res_cfg_tdd_info_s&                                          set_tdd();
  protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<iab_du_cell_res_cfg_fdd_info_s,
                  iab_du_cell_res_cfg_tdd_info_s,
                  protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>>
      c;

  void destroy_();
};

// Activated-Cells-to-be-Updated-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using activ_cells_to_be_upd_list_item_ext_ies_o = protocol_ext_empty_o;

using activ_cells_to_be_upd_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Activated-Cells-to-be-Updated-List-Item ::= SEQUENCE
struct activ_cells_to_be_upd_list_item_s {
  bool                                              ie_exts_present = false;
  nr_cgi_s                                          nr_cgi;
  iab_du_cell_res_cfg_mode_info_c                   iab_du_cell_res_cfg_mode_info;
  activ_cells_to_be_upd_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Activated-Cells-to-be-Updated-List ::= SEQUENCE (SIZE (1..512)) OF Activated-Cells-to-be-Updated-List-Item
using activ_cells_to_be_upd_list_l = dyn_array<activ_cells_to_be_upd_list_item_s>;

// ActivationRequestType ::= ENUMERATED
struct activation_request_type_opts {
  enum options { activ, deactiv, /*...*/ nulltype } value;

  const char* to_string() const;
};
using activation_request_type_e = enumerated<activation_request_type_opts, true>;

// TransmissionCombn8-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tx_combn8_ext_ies_o = protocol_ext_empty_o;

using tx_combn8_ext_ies_container = protocol_ext_container_empty_l;

// TransmissionCombn8 ::= SEQUENCE
struct tx_combn8_s {
  bool                        ie_exts_present = false;
  uint8_t                     comb_offset_n8  = 0;
  uint8_t                     cyclic_shift_n8 = 0;
  tx_combn8_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TransmissionComb-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct tx_comb_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { tx_combn8, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::tx_combn8; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    tx_combn8_s&       tx_combn8() { return c; }
    const tx_combn8_s& tx_combn8() const { return c; }

  private:
    tx_combn8_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// TransmissionComb ::= CHOICE
struct tx_comb_c {
  struct n2_s_ {
    uint8_t comb_offset_n2  = 0;
    uint8_t cyclic_shift_n2 = 0;
  };
  struct n4_s_ {
    uint8_t comb_offset_n4  = 0;
    uint8_t cyclic_shift_n4 = 0;
  };
  struct types_opts {
    enum options { n2, n4, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  tx_comb_c() = default;
  tx_comb_c(const tx_comb_c& other);
  tx_comb_c& operator=(const tx_comb_c& other);
  ~tx_comb_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  n2_s_& n2()
  {
    assert_choice_type(types::n2, type_, "TransmissionComb");
    return c.get<n2_s_>();
  }
  n4_s_& n4()
  {
    assert_choice_type(types::n4, type_, "TransmissionComb");
    return c.get<n4_s_>();
  }
  protocol_ie_single_container_s<tx_comb_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TransmissionComb");
    return c.get<protocol_ie_single_container_s<tx_comb_ext_ies_o>>();
  }
  const n2_s_& n2() const
  {
    assert_choice_type(types::n2, type_, "TransmissionComb");
    return c.get<n2_s_>();
  }
  const n4_s_& n4() const
  {
    assert_choice_type(types::n4, type_, "TransmissionComb");
    return c.get<n4_s_>();
  }
  const protocol_ie_single_container_s<tx_comb_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TransmissionComb");
    return c.get<protocol_ie_single_container_s<tx_comb_ext_ies_o>>();
  }
  n2_s_&                                             set_n2();
  n4_s_&                                             set_n4();
  protocol_ie_single_container_s<tx_comb_ext_ies_o>& set_choice_ext();

private:
  types                                                                            type_;
  choice_buffer_t<n2_s_, n4_s_, protocol_ie_single_container_s<tx_comb_ext_ies_o>> c;

  void destroy_();
};

// ResourceTypePeriodic-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_type_periodic_ext_ies_o = protocol_ext_empty_o;

using res_type_periodic_ext_ies_container = protocol_ext_container_empty_l;

// ResourceTypePeriodic ::= SEQUENCE
struct res_type_periodic_s {
  struct periodicity_opts {
    enum options {
      slot1,
      slot2,
      slot4,
      slot5,
      slot8,
      slot10,
      slot16,
      slot20,
      slot32,
      slot40,
      slot64,
      slot80,
      slot160,
      slot320,
      slot640,
      slot1280,
      slot2560,
      // ...
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using periodicity_e_ = enumerated<periodicity_opts, true>;

  // member variables
  bool                                ie_exts_present = false;
  periodicity_e_                      periodicity;
  uint16_t                            offset = 0;
  res_type_periodic_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeSemi-persistent-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_type_semi_persistent_ext_ies_o = protocol_ext_empty_o;

using res_type_semi_persistent_ext_ies_container = protocol_ext_container_empty_l;

// ResourceTypeSemi-persistent ::= SEQUENCE
struct res_type_semi_persistent_s {
  struct periodicity_opts {
    enum options {
      slot1,
      slot2,
      slot4,
      slot5,
      slot8,
      slot10,
      slot16,
      slot20,
      slot32,
      slot40,
      slot64,
      slot80,
      slot160,
      slot320,
      slot640,
      slot1280,
      slot2560,
      // ...
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using periodicity_e_ = enumerated<periodicity_opts, true>;

  // member variables
  bool                                       ie_exts_present = false;
  periodicity_e_                             periodicity;
  uint16_t                                   offset = 0;
  res_type_semi_persistent_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeAperiodic-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_type_aperiodic_ext_ies_o = protocol_ext_empty_o;

using res_type_aperiodic_ext_ies_container = protocol_ext_container_empty_l;

// ResourceTypeAperiodic ::= SEQUENCE
struct res_type_aperiodic_s {
  struct aperiodic_res_type_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using aperiodic_res_type_e_ = enumerated<aperiodic_res_type_opts, true>;

  // member variables
  bool                                 ie_exts_present = false;
  aperiodic_res_type_e_                aperiodic_res_type;
  res_type_aperiodic_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using res_type_ext_ies_o = protocol_ies_empty_o;

// ResourceType ::= CHOICE
struct res_type_c {
  struct types_opts {
    enum options { periodic, semi_persistent, aperiodic, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  res_type_c() = default;
  res_type_c(const res_type_c& other);
  res_type_c& operator=(const res_type_c& other);
  ~res_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  res_type_periodic_s& periodic()
  {
    assert_choice_type(types::periodic, type_, "ResourceType");
    return c.get<res_type_periodic_s>();
  }
  res_type_semi_persistent_s& semi_persistent()
  {
    assert_choice_type(types::semi_persistent, type_, "ResourceType");
    return c.get<res_type_semi_persistent_s>();
  }
  res_type_aperiodic_s& aperiodic()
  {
    assert_choice_type(types::aperiodic, type_, "ResourceType");
    return c.get<res_type_aperiodic_s>();
  }
  protocol_ie_single_container_s<res_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ResourceType");
    return c.get<protocol_ie_single_container_s<res_type_ext_ies_o>>();
  }
  const res_type_periodic_s& periodic() const
  {
    assert_choice_type(types::periodic, type_, "ResourceType");
    return c.get<res_type_periodic_s>();
  }
  const res_type_semi_persistent_s& semi_persistent() const
  {
    assert_choice_type(types::semi_persistent, type_, "ResourceType");
    return c.get<res_type_semi_persistent_s>();
  }
  const res_type_aperiodic_s& aperiodic() const
  {
    assert_choice_type(types::aperiodic, type_, "ResourceType");
    return c.get<res_type_aperiodic_s>();
  }
  const protocol_ie_single_container_s<res_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ResourceType");
    return c.get<protocol_ie_single_container_s<res_type_ext_ies_o>>();
  }
  res_type_periodic_s&                                set_periodic();
  res_type_semi_persistent_s&                         set_semi_persistent();
  res_type_aperiodic_s&                               set_aperiodic();
  protocol_ie_single_container_s<res_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<res_type_ext_ies_o>,
                  res_type_aperiodic_s,
                  res_type_periodic_s,
                  res_type_semi_persistent_s>
      c;

  void destroy_();
};

// NrofSymbolsExtended ::= ENUMERATED
struct nrof_symbols_extended_opts {
  enum options { n8, n10, n12, n14, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nrof_symbols_extended_e = enumerated<nrof_symbols_extended_opts, true>;

// RepetitionFactorExtended ::= ENUMERATED
struct repeat_factor_extended_opts {
  enum options { n3, n5, n6, n7, n8, n10, n12, n14, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using repeat_factor_extended_e = enumerated<repeat_factor_extended_opts, true>;

// StartRBHopping ::= ENUMERATED
struct start_rb_hop_opts {
  enum options { enable, nulltype } value;

  const char* to_string() const;
};
using start_rb_hop_e = enumerated<start_rb_hop_opts>;

// StartRBIndex-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using start_rb_idx_ext_ies_o = protocol_ies_empty_o;

// StartRBIndex ::= CHOICE
struct start_rb_idx_c {
  struct types_opts {
    enum options { freq_scaling_factor2, freq_scaling_factor4, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  start_rb_idx_c() = default;
  start_rb_idx_c(const start_rb_idx_c& other);
  start_rb_idx_c& operator=(const start_rb_idx_c& other);
  ~start_rb_idx_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& freq_scaling_factor2()
  {
    assert_choice_type(types::freq_scaling_factor2, type_, "StartRBIndex");
    return c.get<uint8_t>();
  }
  uint8_t& freq_scaling_factor4()
  {
    assert_choice_type(types::freq_scaling_factor4, type_, "StartRBIndex");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<start_rb_idx_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "StartRBIndex");
    return c.get<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>();
  }
  const uint8_t& freq_scaling_factor2() const
  {
    assert_choice_type(types::freq_scaling_factor2, type_, "StartRBIndex");
    return c.get<uint8_t>();
  }
  const uint8_t& freq_scaling_factor4() const
  {
    assert_choice_type(types::freq_scaling_factor4, type_, "StartRBIndex");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<start_rb_idx_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "StartRBIndex");
    return c.get<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>>();
  }
  uint8_t&                                                set_freq_scaling_factor2();
  uint8_t&                                                set_freq_scaling_factor4();
  protocol_ie_single_container_s<start_rb_idx_ext_ies_o>& set_choice_ext();

private:
  types                                                                   type_;
  choice_buffer_t<protocol_ie_single_container_s<start_rb_idx_ext_ies_o>> c;

  void destroy_();
};

// SRSResource-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct srs_res_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct srs_res_ext_ies_container {
  bool                     nrof_symbols_extended_present  = false;
  bool                     repeat_factor_extended_present = false;
  bool                     start_rb_hop_present           = false;
  bool                     start_rb_idx_present           = false;
  nrof_symbols_extended_e  nrof_symbols_extended;
  repeat_factor_extended_e repeat_factor_extended;
  start_rb_hop_e           start_rb_hop;
  start_rb_idx_c           start_rb_idx;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResource ::= SEQUENCE
struct srs_res_s {
  struct nrof_srs_ports_opts {
    enum options { port1, ports2, ports4, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using nrof_srs_ports_e_ = enumerated<nrof_srs_ports_opts>;
  struct nrof_symbols_opts {
    enum options { n1, n2, n4, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using nrof_symbols_e_ = enumerated<nrof_symbols_opts>;
  struct repeat_factor_opts {
    enum options { n1, n2, n4, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using repeat_factor_e_ = enumerated<repeat_factor_opts>;
  struct group_or_seq_hop_opts {
    enum options { neither, group_hop, seq_hop, nulltype } value;

    const char* to_string() const;
  };
  using group_or_seq_hop_e_ = enumerated<group_or_seq_hop_opts>;

  // member variables
  bool                      ie_exts_present = false;
  uint8_t                   srs_res_id      = 0;
  nrof_srs_ports_e_         nrof_srs_ports;
  tx_comb_c                 tx_comb;
  uint8_t                   start_position = 0;
  nrof_symbols_e_           nrof_symbols;
  repeat_factor_e_          repeat_factor;
  uint8_t                   freq_domain_position = 0;
  uint16_t                  freq_domain_shift    = 0;
  uint8_t                   c_srs                = 0;
  uint8_t                   b_srs                = 0;
  uint8_t                   b_hop                = 0;
  group_or_seq_hop_e_       group_or_seq_hop;
  res_type_c                res_type;
  uint16_t                  seq_id = 0;
  srs_res_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResource-List ::= SEQUENCE (SIZE (1..64)) OF SRSResource
using srs_res_list_l = dyn_array<srs_res_s>;

// TransmissionCombPos-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using tx_comb_pos_ext_ies_o = protocol_ies_empty_o;

// TransmissionCombPos ::= CHOICE
struct tx_comb_pos_c {
  struct n2_s_ {
    uint8_t comb_offset_n2  = 0;
    uint8_t cyclic_shift_n2 = 0;
  };
  struct n4_s_ {
    uint8_t comb_offset_n4  = 0;
    uint8_t cyclic_shift_n4 = 0;
  };
  struct n8_s_ {
    uint8_t comb_offset_n8  = 0;
    uint8_t cyclic_shift_n8 = 0;
  };
  struct types_opts {
    enum options { n2, n4, n8, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  tx_comb_pos_c() = default;
  tx_comb_pos_c(const tx_comb_pos_c& other);
  tx_comb_pos_c& operator=(const tx_comb_pos_c& other);
  ~tx_comb_pos_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  n2_s_& n2()
  {
    assert_choice_type(types::n2, type_, "TransmissionCombPos");
    return c.get<n2_s_>();
  }
  n4_s_& n4()
  {
    assert_choice_type(types::n4, type_, "TransmissionCombPos");
    return c.get<n4_s_>();
  }
  n8_s_& n8()
  {
    assert_choice_type(types::n8, type_, "TransmissionCombPos");
    return c.get<n8_s_>();
  }
  protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TransmissionCombPos");
    return c.get<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>();
  }
  const n2_s_& n2() const
  {
    assert_choice_type(types::n2, type_, "TransmissionCombPos");
    return c.get<n2_s_>();
  }
  const n4_s_& n4() const
  {
    assert_choice_type(types::n4, type_, "TransmissionCombPos");
    return c.get<n4_s_>();
  }
  const n8_s_& n8() const
  {
    assert_choice_type(types::n8, type_, "TransmissionCombPos");
    return c.get<n8_s_>();
  }
  const protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TransmissionCombPos");
    return c.get<protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>>();
  }
  n2_s_&                                                 set_n2();
  n4_s_&                                                 set_n4();
  n8_s_&                                                 set_n8();
  protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>& set_choice_ext();

private:
  types                                                                                       type_;
  choice_buffer_t<n2_s_, n4_s_, n8_s_, protocol_ie_single_container_s<tx_comb_pos_ext_ies_o>> c;

  void destroy_();
};

// SRS-Periodicity ::= ENUMERATED
struct srs_periodicity_opts {
  enum options {
    slot1,
    slot2,
    slot4,
    slot5,
    slot8,
    slot10,
    slot16,
    slot20,
    slot32,
    slot40,
    slot64,
    slot80,
    slot160,
    slot320,
    slot640,
    slot1280,
    slot2560,
    slot5120,
    slot10240,
    slot40960,
    slot81920,
    // ...
    slot128,
    slot256,
    slot512,
    slot20480,
    nulltype
  } value;
  typedef uint32_t number_type;

  const char* to_string() const;
  uint32_t    to_number() const;
};
using srs_periodicity_e = enumerated<srs_periodicity_opts, true, 4>;

// SRSPosPeriodicConfigHyperSFNIndex ::= ENUMERATED
struct srs_pos_periodic_cfg_hyper_sfn_idx_opts {
  enum options { even0, odd1, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using srs_pos_periodic_cfg_hyper_sfn_idx_e = enumerated<srs_pos_periodic_cfg_hyper_sfn_idx_opts>;

// ResourceTypePeriodicPos-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct res_type_periodic_pos_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct res_type_periodic_pos_ext_ies_container {
  bool                                 srs_pos_periodic_cfg_hyper_sfn_idx_present = false;
  srs_pos_periodic_cfg_hyper_sfn_idx_e srs_pos_periodic_cfg_hyper_sfn_idx;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypePeriodicPos ::= SEQUENCE
struct res_type_periodic_pos_s {
  bool                                    ie_exts_present = false;
  srs_periodicity_e                       periodicity;
  uint32_t                                offset = 0;
  res_type_periodic_pos_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeSemi-persistentPos-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct res_type_semi_persistent_pos_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct res_type_semi_persistent_pos_ext_ies_container {
  bool                                 srs_pos_periodic_cfg_hyper_sfn_idx_present = false;
  srs_pos_periodic_cfg_hyper_sfn_idx_e srs_pos_periodic_cfg_hyper_sfn_idx;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeSemi-persistentPos ::= SEQUENCE
struct res_type_semi_persistent_pos_s {
  bool                                           ie_exts_present = false;
  srs_periodicity_e                              periodicity;
  uint32_t                                       offset = 0;
  res_type_semi_persistent_pos_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeAperiodicPos-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_type_aperiodic_pos_ext_ies_o = protocol_ext_empty_o;

using res_type_aperiodic_pos_ext_ies_container = protocol_ext_container_empty_l;

// ResourceTypeAperiodicPos ::= SEQUENCE
struct res_type_aperiodic_pos_s {
  bool                                     ie_exts_present = false;
  uint8_t                                  slot_offset     = 0;
  res_type_aperiodic_pos_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypePos-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using res_type_pos_ext_ies_o = protocol_ies_empty_o;

// ResourceTypePos ::= CHOICE
struct res_type_pos_c {
  struct types_opts {
    enum options { periodic, semi_persistent, aperiodic, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  res_type_pos_c() = default;
  res_type_pos_c(const res_type_pos_c& other);
  res_type_pos_c& operator=(const res_type_pos_c& other);
  ~res_type_pos_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  res_type_periodic_pos_s& periodic()
  {
    assert_choice_type(types::periodic, type_, "ResourceTypePos");
    return c.get<res_type_periodic_pos_s>();
  }
  res_type_semi_persistent_pos_s& semi_persistent()
  {
    assert_choice_type(types::semi_persistent, type_, "ResourceTypePos");
    return c.get<res_type_semi_persistent_pos_s>();
  }
  res_type_aperiodic_pos_s& aperiodic()
  {
    assert_choice_type(types::aperiodic, type_, "ResourceTypePos");
    return c.get<res_type_aperiodic_pos_s>();
  }
  protocol_ie_single_container_s<res_type_pos_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ResourceTypePos");
    return c.get<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>();
  }
  const res_type_periodic_pos_s& periodic() const
  {
    assert_choice_type(types::periodic, type_, "ResourceTypePos");
    return c.get<res_type_periodic_pos_s>();
  }
  const res_type_semi_persistent_pos_s& semi_persistent() const
  {
    assert_choice_type(types::semi_persistent, type_, "ResourceTypePos");
    return c.get<res_type_semi_persistent_pos_s>();
  }
  const res_type_aperiodic_pos_s& aperiodic() const
  {
    assert_choice_type(types::aperiodic, type_, "ResourceTypePos");
    return c.get<res_type_aperiodic_pos_s>();
  }
  const protocol_ie_single_container_s<res_type_pos_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ResourceTypePos");
    return c.get<protocol_ie_single_container_s<res_type_pos_ext_ies_o>>();
  }
  res_type_periodic_pos_s&                                set_periodic();
  res_type_semi_persistent_pos_s&                         set_semi_persistent();
  res_type_aperiodic_pos_s&                               set_aperiodic();
  protocol_ie_single_container_s<res_type_pos_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<res_type_pos_ext_ies_o>,
                  res_type_aperiodic_pos_s,
                  res_type_periodic_pos_s,
                  res_type_semi_persistent_pos_s>
      c;

  void destroy_();
};

// SSB-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_ext_ies_o = protocol_ext_empty_o;

using ssb_ext_ies_container = protocol_ext_container_empty_l;

// SSB ::= SEQUENCE
struct ssb_s {
  bool                  ssb_idx_present = false;
  bool                  ie_exts_present = false;
  uint16_t              pci_nr          = 0;
  uint8_t               ssb_idx         = 0;
  ssb_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSInformationPos-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pr_si_nformation_pos_ext_ies_o = protocol_ext_empty_o;

using pr_si_nformation_pos_ext_ies_container = protocol_ext_container_empty_l;

// PRSInformationPos ::= SEQUENCE
struct pr_si_nformation_pos_s {
  bool                                   prs_res_id_pos_present = false;
  bool                                   ie_exts_present        = false;
  uint16_t                               prs_id_pos             = 0;
  uint8_t                                prs_res_set_id_pos     = 0;
  uint8_t                                prs_res_id_pos         = 0;
  pr_si_nformation_pos_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialInformationPos-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using spatial_info_pos_ext_ies_o = protocol_ies_empty_o;

// SpatialRelationPos ::= CHOICE
struct spatial_relation_pos_c {
  struct types_opts {
    enum options { ssb_pos, pr_si_nformation_pos, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  spatial_relation_pos_c() = default;
  spatial_relation_pos_c(const spatial_relation_pos_c& other);
  spatial_relation_pos_c& operator=(const spatial_relation_pos_c& other);
  ~spatial_relation_pos_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ssb_s& ssb_pos()
  {
    assert_choice_type(types::ssb_pos, type_, "SpatialRelationPos");
    return c.get<ssb_s>();
  }
  pr_si_nformation_pos_s& pr_si_nformation_pos()
  {
    assert_choice_type(types::pr_si_nformation_pos, type_, "SpatialRelationPos");
    return c.get<pr_si_nformation_pos_s>();
  }
  protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SpatialRelationPos");
    return c.get<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>();
  }
  const ssb_s& ssb_pos() const
  {
    assert_choice_type(types::ssb_pos, type_, "SpatialRelationPos");
    return c.get<ssb_s>();
  }
  const pr_si_nformation_pos_s& pr_si_nformation_pos() const
  {
    assert_choice_type(types::pr_si_nformation_pos, type_, "SpatialRelationPos");
    return c.get<pr_si_nformation_pos_s>();
  }
  const protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SpatialRelationPos");
    return c.get<protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>>();
  }
  ssb_s&                                                      set_ssb_pos();
  pr_si_nformation_pos_s&                                     set_pr_si_nformation_pos();
  protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                      type_;
  choice_buffer_t<pr_si_nformation_pos_s, protocol_ie_single_container_s<spatial_info_pos_ext_ies_o>, ssb_s> c;

  void destroy_();
};

// SlotOffsetRemainingHopsAperiodic-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slot_offset_remaining_hops_aperiodic_ext_ies_o = protocol_ext_empty_o;

using slot_offset_remaining_hops_aperiodic_ext_ies_container = protocol_ext_container_empty_l;

// SlotOffsetRemainingHopsAperiodic ::= SEQUENCE
struct slot_offset_remaining_hops_aperiodic_s {
  bool                                                   slot_offset_present    = false;
  bool                                                   start_position_present = false;
  bool                                                   ie_exts_present        = false;
  uint8_t                                                slot_offset            = 1;
  uint8_t                                                start_position         = 0;
  slot_offset_remaining_hops_aperiodic_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SlotOffsetRemainingHopsSemiPersistent-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slot_offset_remaining_hops_semi_persistent_ext_ies_o = protocol_ext_empty_o;

using slot_offset_remaining_hops_semi_persistent_ext_ies_container = protocol_ext_container_empty_l;

// SlotOffsetRemainingHopsSemiPersistent ::= SEQUENCE
struct slot_offset_remaining_hops_semi_persistent_s {
  bool                                                         start_position_present = false;
  bool                                                         ie_exts_present        = false;
  srs_periodicity_e                                            srs_periodicity;
  uint32_t                                                     offset         = 0;
  uint8_t                                                      start_position = 0;
  slot_offset_remaining_hops_semi_persistent_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SlotOffsetRemainingHopsSemiPeriodic-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slot_offset_remaining_hops_semi_periodic_ext_ies_o = protocol_ext_empty_o;

using slot_offset_remaining_hops_semi_periodic_ext_ies_container = protocol_ext_container_empty_l;

// SlotOffsetRemainingHopsPeriodic ::= SEQUENCE
struct slot_offset_remaining_hops_periodic_s {
  bool                                                       start_position_present = false;
  bool                                                       ie_exts_present        = false;
  srs_periodicity_e                                          srs_periodicity;
  uint32_t                                                   offset         = 0;
  uint8_t                                                    start_position = 0;
  slot_offset_remaining_hops_semi_periodic_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SlotOffsetRemainingHops-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using slot_offset_remaining_hops_ext_ies_o = protocol_ies_empty_o;

// SlotOffsetRemainingHops ::= CHOICE
struct slot_offset_remaining_hops_c {
  struct types_opts {
    enum options { aperiodic, semi_persistent, periodic, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  slot_offset_remaining_hops_c() = default;
  slot_offset_remaining_hops_c(const slot_offset_remaining_hops_c& other);
  slot_offset_remaining_hops_c& operator=(const slot_offset_remaining_hops_c& other);
  ~slot_offset_remaining_hops_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  slot_offset_remaining_hops_aperiodic_s& aperiodic()
  {
    assert_choice_type(types::aperiodic, type_, "SlotOffsetRemainingHops");
    return c.get<slot_offset_remaining_hops_aperiodic_s>();
  }
  slot_offset_remaining_hops_semi_persistent_s& semi_persistent()
  {
    assert_choice_type(types::semi_persistent, type_, "SlotOffsetRemainingHops");
    return c.get<slot_offset_remaining_hops_semi_persistent_s>();
  }
  slot_offset_remaining_hops_periodic_s& periodic()
  {
    assert_choice_type(types::periodic, type_, "SlotOffsetRemainingHops");
    return c.get<slot_offset_remaining_hops_periodic_s>();
  }
  protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SlotOffsetRemainingHops");
    return c.get<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>();
  }
  const slot_offset_remaining_hops_aperiodic_s& aperiodic() const
  {
    assert_choice_type(types::aperiodic, type_, "SlotOffsetRemainingHops");
    return c.get<slot_offset_remaining_hops_aperiodic_s>();
  }
  const slot_offset_remaining_hops_semi_persistent_s& semi_persistent() const
  {
    assert_choice_type(types::semi_persistent, type_, "SlotOffsetRemainingHops");
    return c.get<slot_offset_remaining_hops_semi_persistent_s>();
  }
  const slot_offset_remaining_hops_periodic_s& periodic() const
  {
    assert_choice_type(types::periodic, type_, "SlotOffsetRemainingHops");
    return c.get<slot_offset_remaining_hops_periodic_s>();
  }
  const protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SlotOffsetRemainingHops");
    return c.get<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>>();
  }
  slot_offset_remaining_hops_aperiodic_s&                               set_aperiodic();
  slot_offset_remaining_hops_semi_persistent_s&                         set_semi_persistent();
  slot_offset_remaining_hops_periodic_s&                                set_periodic();
  protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<slot_offset_remaining_hops_ext_ies_o>,
                  slot_offset_remaining_hops_aperiodic_s,
                  slot_offset_remaining_hops_periodic_s,
                  slot_offset_remaining_hops_semi_persistent_s>
      c;

  void destroy_();
};

// SlotOffsetForRemainingHopsItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slot_offset_for_remaining_hops_item_ext_ies_o = protocol_ext_empty_o;

using slot_offset_for_remaining_hops_item_ext_ies_container = protocol_ext_container_empty_l;

// SlotOffsetForRemainingHopsItem ::= SEQUENCE
struct slot_offset_for_remaining_hops_item_s {
  bool                                                  ie_exts_present = false;
  slot_offset_remaining_hops_c                          slot_offset_remaining_hops;
  slot_offset_for_remaining_hops_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SlotOffsetForRemainingHopsList ::= SEQUENCE (SIZE (1..5)) OF SlotOffsetForRemainingHopsItem
using slot_offset_for_remaining_hops_list_l = dyn_array<slot_offset_for_remaining_hops_item_s>;

// TxHoppingConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tx_hop_cfg_ext_ies_o = protocol_ext_empty_o;

using tx_hop_cfg_ext_ies_container = protocol_ext_container_empty_l;

// TxHoppingConfiguration ::= SEQUENCE
struct tx_hop_cfg_s {
  struct overlap_value_opts {
    enum options { rb0, rb1, rb2, rb4, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using overlap_value_e_ = enumerated<overlap_value_opts>;

  // member variables
  bool                                  ie_exts_present = false;
  overlap_value_e_                      overlap_value;
  uint8_t                               nof_hops = 2;
  slot_offset_for_remaining_hops_list_l slot_offset_for_remaining_hops_list;
  tx_hop_cfg_ext_ies_container          ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSResource-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct pos_srs_res_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pos_srs_res_item_ext_ies_container {
  bool         tx_hop_cfg_present = false;
  tx_hop_cfg_s tx_hop_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSResource-Item ::= SEQUENCE
struct pos_srs_res_item_s {
  struct nrof_symbols_opts {
    enum options { n1, n2, n4, n8, n12, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using nrof_symbols_e_ = enumerated<nrof_symbols_opts>;
  struct group_or_seq_hop_opts {
    enum options { neither, group_hop, seq_hop, nulltype } value;

    const char* to_string() const;
  };
  using group_or_seq_hop_e_ = enumerated<group_or_seq_hop_opts>;

  // member variables
  bool                               spatial_relation_pos_present = false;
  bool                               ie_exts_present              = false;
  uint8_t                            srs_pos_res_id               = 0;
  tx_comb_pos_c                      tx_comb_pos;
  uint8_t                            start_position = 0;
  nrof_symbols_e_                    nrof_symbols;
  uint16_t                           freq_domain_shift = 0;
  uint8_t                            c_srs             = 0;
  group_or_seq_hop_e_                group_or_seq_hop;
  res_type_pos_c                     res_type_pos;
  uint32_t                           seq_id = 0;
  spatial_relation_pos_c             spatial_relation_pos;
  pos_srs_res_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSResource-List ::= SEQUENCE (SIZE (1..64)) OF PosSRSResource-Item
using pos_srs_res_list_l = dyn_array<pos_srs_res_item_s>;

// SRSResourceID-List ::= SEQUENCE (SIZE (1..16)) OF INTEGER (0..63)
using srs_res_id_list_l = bounded_array<uint8_t, 16>;

// ResourceSetTypePeriodic-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_set_type_periodic_ext_ies_o = protocol_ext_empty_o;

using res_set_type_periodic_ext_ies_container = protocol_ext_container_empty_l;

// ResourceSetTypePeriodic ::= SEQUENCE
struct res_set_type_periodic_s {
  struct periodic_set_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using periodic_set_e_ = enumerated<periodic_set_opts, true>;

  // member variables
  bool                                    ie_exts_present = false;
  periodic_set_e_                         periodic_set;
  res_set_type_periodic_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceSetTypeSemi-persistent-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_set_type_semi_persistent_ext_ies_o = protocol_ext_empty_o;

using res_set_type_semi_persistent_ext_ies_container = protocol_ext_container_empty_l;

// ResourceSetTypeSemi-persistent ::= SEQUENCE
struct res_set_type_semi_persistent_s {
  struct semi_persistent_set_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using semi_persistent_set_e_ = enumerated<semi_persistent_set_opts, true>;

  // member variables
  bool                                           ie_exts_present = false;
  semi_persistent_set_e_                         semi_persistent_set;
  res_set_type_semi_persistent_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceSetTypeAperiodic-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_set_type_aperiodic_ext_ies_o = protocol_ext_empty_o;

using res_set_type_aperiodic_ext_ies_container = protocol_ext_container_empty_l;

// ResourceSetTypeAperiodic ::= SEQUENCE
struct res_set_type_aperiodic_s {
  bool                                     ie_exts_present      = false;
  uint8_t                                  srs_res_trigger_list = 1;
  uint8_t                                  slotoffset           = 0;
  res_set_type_aperiodic_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceSetType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using res_set_type_ext_ies_o = protocol_ies_empty_o;

// ResourceSetType ::= CHOICE
struct res_set_type_c {
  struct types_opts {
    enum options { periodic, semi_persistent, aperiodic, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  res_set_type_c() = default;
  res_set_type_c(const res_set_type_c& other);
  res_set_type_c& operator=(const res_set_type_c& other);
  ~res_set_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  res_set_type_periodic_s& periodic()
  {
    assert_choice_type(types::periodic, type_, "ResourceSetType");
    return c.get<res_set_type_periodic_s>();
  }
  res_set_type_semi_persistent_s& semi_persistent()
  {
    assert_choice_type(types::semi_persistent, type_, "ResourceSetType");
    return c.get<res_set_type_semi_persistent_s>();
  }
  res_set_type_aperiodic_s& aperiodic()
  {
    assert_choice_type(types::aperiodic, type_, "ResourceSetType");
    return c.get<res_set_type_aperiodic_s>();
  }
  protocol_ie_single_container_s<res_set_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ResourceSetType");
    return c.get<protocol_ie_single_container_s<res_set_type_ext_ies_o>>();
  }
  const res_set_type_periodic_s& periodic() const
  {
    assert_choice_type(types::periodic, type_, "ResourceSetType");
    return c.get<res_set_type_periodic_s>();
  }
  const res_set_type_semi_persistent_s& semi_persistent() const
  {
    assert_choice_type(types::semi_persistent, type_, "ResourceSetType");
    return c.get<res_set_type_semi_persistent_s>();
  }
  const res_set_type_aperiodic_s& aperiodic() const
  {
    assert_choice_type(types::aperiodic, type_, "ResourceSetType");
    return c.get<res_set_type_aperiodic_s>();
  }
  const protocol_ie_single_container_s<res_set_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ResourceSetType");
    return c.get<protocol_ie_single_container_s<res_set_type_ext_ies_o>>();
  }
  res_set_type_periodic_s&                                set_periodic();
  res_set_type_semi_persistent_s&                         set_semi_persistent();
  res_set_type_aperiodic_s&                               set_aperiodic();
  protocol_ie_single_container_s<res_set_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<res_set_type_ext_ies_o>,
                  res_set_type_aperiodic_s,
                  res_set_type_periodic_s,
                  res_set_type_semi_persistent_s>
      c;

  void destroy_();
};

// SRSResourceSet-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srs_res_set_ext_ies_o = protocol_ext_empty_o;

using srs_res_set_ext_ies_container = protocol_ext_container_empty_l;

// SRSResourceSet ::= SEQUENCE
struct srs_res_set_s {
  bool                          ie_exts_present = false;
  uint8_t                       srs_res_set_id  = 0;
  srs_res_id_list_l             srs_res_id_list;
  res_set_type_c                res_set_type;
  srs_res_set_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResourceSet-List ::= SEQUENCE (SIZE (1..16)) OF SRSResourceSet
using srs_res_set_list2_l = dyn_array<srs_res_set_s>;

// PosSRSResourceID-List ::= SEQUENCE (SIZE (1..16)) OF INTEGER (0..63)
using pos_srs_res_id_list_l = bounded_array<uint8_t, 16>;

// PosResourceSetTypePR-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pos_res_set_type_pr_ext_ies_o = protocol_ext_empty_o;

using pos_res_set_type_pr_ext_ies_container = protocol_ext_container_empty_l;

// PosResourceSetTypePR ::= SEQUENCE
struct pos_res_set_type_pr_s {
  struct posperiodic_set_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using posperiodic_set_e_ = enumerated<posperiodic_set_opts, true>;

  // member variables
  bool                                  ie_exts_present = false;
  posperiodic_set_e_                    posperiodic_set;
  pos_res_set_type_pr_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosResourceSetTypeSP-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pos_res_set_type_sp_ext_ies_o = protocol_ext_empty_o;

using pos_res_set_type_sp_ext_ies_container = protocol_ext_container_empty_l;

// PosResourceSetTypeSP ::= SEQUENCE
struct pos_res_set_type_sp_s {
  struct possemi_persistent_set_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using possemi_persistent_set_e_ = enumerated<possemi_persistent_set_opts, true>;

  // member variables
  bool                                  ie_exts_present = false;
  possemi_persistent_set_e_             possemi_persistent_set;
  pos_res_set_type_sp_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosResourceSetTypeAP-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pos_res_set_type_ap_ext_ies_o = protocol_ext_empty_o;

using pos_res_set_type_ap_ext_ies_container = protocol_ext_container_empty_l;

// PosResourceSetTypeAP ::= SEQUENCE
struct pos_res_set_type_ap_s {
  bool                                  ie_exts_present      = false;
  uint8_t                               srs_res_trigger_list = 1;
  pos_res_set_type_ap_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosResourceSetType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using pos_res_set_type_ext_ies_o = protocol_ies_empty_o;

// PosResourceSetType ::= CHOICE
struct pos_res_set_type_c {
  struct types_opts {
    enum options { periodic, semi_persistent, aperiodic, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  pos_res_set_type_c() = default;
  pos_res_set_type_c(const pos_res_set_type_c& other);
  pos_res_set_type_c& operator=(const pos_res_set_type_c& other);
  ~pos_res_set_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  pos_res_set_type_pr_s& periodic()
  {
    assert_choice_type(types::periodic, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_pr_s>();
  }
  pos_res_set_type_sp_s& semi_persistent()
  {
    assert_choice_type(types::semi_persistent, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_sp_s>();
  }
  pos_res_set_type_ap_s& aperiodic()
  {
    assert_choice_type(types::aperiodic, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_ap_s>();
  }
  protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PosResourceSetType");
    return c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>();
  }
  const pos_res_set_type_pr_s& periodic() const
  {
    assert_choice_type(types::periodic, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_pr_s>();
  }
  const pos_res_set_type_sp_s& semi_persistent() const
  {
    assert_choice_type(types::semi_persistent, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_sp_s>();
  }
  const pos_res_set_type_ap_s& aperiodic() const
  {
    assert_choice_type(types::aperiodic, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_ap_s>();
  }
  const protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PosResourceSetType");
    return c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>();
  }
  pos_res_set_type_pr_s&                                      set_periodic();
  pos_res_set_type_sp_s&                                      set_semi_persistent();
  pos_res_set_type_ap_s&                                      set_aperiodic();
  protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<pos_res_set_type_ap_s,
                  pos_res_set_type_pr_s,
                  pos_res_set_type_sp_s,
                  protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>
      c;

  void destroy_();
};

// PosSRSResourceSet-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pos_srs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using pos_srs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// PosSRSResourceSet-Item ::= SEQUENCE
struct pos_srs_res_set_item_s {
  bool                                   ie_exts_present   = false;
  uint8_t                                possrs_res_set_id = 0;
  pos_srs_res_id_list_l                  poss_rs_res_id_list;
  pos_res_set_type_c                     posres_set_type;
  pos_srs_res_set_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSResourceSet-List ::= SEQUENCE (SIZE (1..16)) OF PosSRSResourceSet-Item
using pos_srs_res_set_list_l = dyn_array<pos_srs_res_set_item_s>;

// SRSConfig-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srs_cfg_ext_ies_o = protocol_ext_empty_o;

using srs_cfg_ext_ies_container = protocol_ext_container_empty_l;

// SRSConfig ::= SEQUENCE
struct srs_cfg_s {
  bool                      ie_exts_present = false;
  srs_res_list_l            srs_res_list;
  pos_srs_res_list_l        pos_srs_res_list;
  srs_res_set_list2_l       srs_res_set_list;
  pos_srs_res_set_list_l    pos_srs_res_set_list;
  srs_cfg_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ActiveULBWP-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using active_ul_bwp_ext_ies_o = protocol_ext_empty_o;

using active_ul_bwp_ext_ies_container = protocol_ext_container_empty_l;

// ActiveULBWP ::= SEQUENCE
struct active_ul_bwp_s {
  struct subcarrier_spacing_opts {
    enum options { khz15, khz30, khz60, khz120, /*...*/ khz480, khz960, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using subcarrier_spacing_e_ = enumerated<subcarrier_spacing_opts, true, 2>;
  struct cp_opts {
    enum options { normal, extended, nulltype } value;

    const char* to_string() const;
  };
  using cp_e_ = enumerated<cp_opts>;
  struct shift7dot5k_hz_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using shift7dot5k_hz_e_ = enumerated<shift7dot5k_hz_opts, true>;

  // member variables
  bool                            shift7dot5k_hz_present = false;
  bool                            ie_exts_present        = false;
  uint32_t                        location_and_bw        = 0;
  subcarrier_spacing_e_           subcarrier_spacing;
  cp_e_                           cp;
  uint16_t                        tx_direct_current_location = 0;
  shift7dot5k_hz_e_               shift7dot5k_hz;
  srs_cfg_s                       srs_cfg;
  active_ul_bwp_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalDuplicationIndication ::= ENUMERATED
struct add_dupl_ind_opts {
  enum options { three, four, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using add_dupl_ind_e = enumerated<add_dupl_ind_opts, true>;

// GTPTunnel-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gtp_tunnel_ext_ies_o = protocol_ext_empty_o;

using gtp_tunnel_ext_ies_container = protocol_ext_container_empty_l;

// GTPTunnel ::= SEQUENCE
struct gtp_tunnel_s {
  bool                                  ie_exts_present = false;
  bounded_bitstring<1, 160, true, true> transport_layer_address;
  fixed_octstring<4, true>              gtp_teid;
  gtp_tunnel_ext_ies_container          ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UPTransportLayerInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using up_transport_layer_info_ext_ies_o = protocol_ies_empty_o;

// UPTransportLayerInformation ::= CHOICE
struct up_transport_layer_info_c {
  struct types_opts {
    enum options { gtp_tunnel, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  up_transport_layer_info_c() = default;
  up_transport_layer_info_c(const up_transport_layer_info_c& other);
  up_transport_layer_info_c& operator=(const up_transport_layer_info_c& other);
  ~up_transport_layer_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  gtp_tunnel_s& gtp_tunnel()
  {
    assert_choice_type(types::gtp_tunnel, type_, "UPTransportLayerInformation");
    return c.get<gtp_tunnel_s>();
  }
  protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UPTransportLayerInformation");
    return c.get<protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>>();
  }
  const gtp_tunnel_s& gtp_tunnel() const
  {
    assert_choice_type(types::gtp_tunnel, type_, "UPTransportLayerInformation");
    return c.get<gtp_tunnel_s>();
  }
  const protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UPTransportLayerInformation");
    return c.get<protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>>();
  }
  gtp_tunnel_s&                                                      set_gtp_tunnel();
  protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                            type_;
  choice_buffer_t<gtp_tunnel_s, protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>> c;

  void destroy_();
};

// BAPRoutingIDExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bap_routing_id_ext_ies_o = protocol_ext_empty_o;

using bap_routing_id_ext_ies_container = protocol_ext_container_empty_l;

// BAPRoutingID ::= SEQUENCE
struct bap_routing_id_s {
  bool                             ie_exts_present = false;
  fixed_bitstring<10, false, true> bap_address;
  fixed_bitstring<10, false, true> bap_path_id;
  bap_routing_id_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EgressBHRLCCHItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using egress_bh_rlc_ch_item_ext_ies_o = protocol_ext_empty_o;

using egress_bh_rlc_ch_item_ext_ies_container = protocol_ext_container_empty_l;

// EgressBHRLCCHItem ::= SEQUENCE
struct egress_bh_rlc_ch_item_s {
  bool                                    ie_exts_present = false;
  fixed_bitstring<10, false, true>        next_hop_bap_address;
  fixed_bitstring<16, false, true>        bh_rlc_ch_id;
  egress_bh_rlc_ch_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EgressBHRLCCHList ::= SEQUENCE (SIZE (1..2)) OF EgressBHRLCCHItem
using egress_bh_rlc_ch_list_l = dyn_array<egress_bh_rlc_ch_item_s>;

// NonF1terminatingTopologyIndicator ::= ENUMERATED
struct non_f1terminating_topology_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using non_f1terminating_topology_ind_e = enumerated<non_f1terminating_topology_ind_opts, true>;

// BHInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct bh_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct bh_info_ext_ies_container {
  bool                             non_f1terminating_topology_ind_present = false;
  non_f1terminating_topology_ind_e non_f1terminating_topology_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHInfo ::= SEQUENCE
struct bh_info_s {
  bool                      ba_prouting_id_present = false;
  bool                      ie_exts_present        = false;
  bap_routing_id_s          ba_prouting_id;
  egress_bh_rlc_ch_list_l   egress_bh_rlc_ch_list;
  bh_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalPDCPDuplicationTNL-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct add_pdcp_dupl_tnl_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct add_pdcp_dupl_tnl_item_ext_ies_container {
  bool      bh_info_present = false;
  bh_info_s bh_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalPDCPDuplicationTNL-Item ::= SEQUENCE
struct add_pdcp_dupl_tnl_item_s {
  bool                                     ie_exts_present = false;
  up_transport_layer_info_c                add_pdcp_dupl_up_tnl_info;
  add_pdcp_dupl_tnl_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalPDCPDuplicationTNL-List ::= SEQUENCE (SIZE (1..2)) OF AdditionalPDCPDuplicationTNL-Item
using add_pdcp_dupl_tnl_list_l = dyn_array<add_pdcp_dupl_tnl_item_s>;

// RelativePathDelay-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct relative_path_delay_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options {
        report_granularitykminus1addpath,
        report_granularitykminus2addpath,
        report_granularitykminus3addpath,
        report_granularitykminus4addpath,
        report_granularitykminus5addpath,
        report_granularitykminus6addpath,
        nulltype
      } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    value_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint16_t&       report_granularitykminus1addpath();
    uint16_t&       report_granularitykminus2addpath();
    uint32_t&       report_granularitykminus3addpath();
    uint32_t&       report_granularitykminus4addpath();
    uint32_t&       report_granularitykminus5addpath();
    uint32_t&       report_granularitykminus6addpath();
    const uint16_t& report_granularitykminus1addpath() const;
    const uint16_t& report_granularitykminus2addpath() const;
    const uint32_t& report_granularitykminus3addpath() const;
    const uint32_t& report_granularitykminus4addpath() const;
    const uint32_t& report_granularitykminus5addpath() const;
    const uint32_t& report_granularitykminus6addpath() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RelativePathDelay ::= CHOICE
struct relative_path_delay_c {
  struct types_opts {
    enum options { k0, k1, k2, k3, k4, k5, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  relative_path_delay_c() = default;
  relative_path_delay_c(const relative_path_delay_c& other);
  relative_path_delay_c& operator=(const relative_path_delay_c& other);
  ~relative_path_delay_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint16_t& k0()
  {
    assert_choice_type(types::k0, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  uint16_t& k1()
  {
    assert_choice_type(types::k1, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  uint16_t& k2()
  {
    assert_choice_type(types::k2, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  uint16_t& k3()
  {
    assert_choice_type(types::k3, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  uint16_t& k4()
  {
    assert_choice_type(types::k4, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  uint16_t& k5()
  {
    assert_choice_type(types::k5, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  protocol_ie_single_container_s<relative_path_delay_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "RelativePathDelay");
    return c.get<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>();
  }
  const uint16_t& k0() const
  {
    assert_choice_type(types::k0, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  const uint16_t& k1() const
  {
    assert_choice_type(types::k1, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  const uint16_t& k2() const
  {
    assert_choice_type(types::k2, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  const uint16_t& k3() const
  {
    assert_choice_type(types::k3, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  const uint16_t& k4() const
  {
    assert_choice_type(types::k4, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  const uint16_t& k5() const
  {
    assert_choice_type(types::k5, type_, "RelativePathDelay");
    return c.get<uint16_t>();
  }
  const protocol_ie_single_container_s<relative_path_delay_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "RelativePathDelay");
    return c.get<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>>();
  }
  uint16_t&                                                      set_k0();
  uint16_t&                                                      set_k1();
  uint16_t&                                                      set_k2();
  uint16_t&                                                      set_k3();
  uint16_t&                                                      set_k4();
  uint16_t&                                                      set_k5();
  protocol_ie_single_container_s<relative_path_delay_ext_ies_o>& set_choice_ext();

private:
  types                                                                          type_;
  choice_buffer_t<protocol_ie_single_container_s<relative_path_delay_ext_ies_o>> c;

  void destroy_();
};

// TimingMeasurementQuality-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using timing_meas_quality_ext_ies_o = protocol_ext_empty_o;

using timing_meas_quality_ext_ies_container = protocol_ext_container_empty_l;

// TimingMeasurementQuality ::= SEQUENCE
struct timing_meas_quality_s {
  struct resolution_opts {
    enum options { m0dot1, m1, m10, m30, /*...*/ nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using resolution_e_ = enumerated<resolution_opts, true>;

  // member variables
  bool                                  ie_exts_present = false;
  uint8_t                               meas_quality    = 0;
  resolution_e_                         resolution;
  timing_meas_quality_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AngleMeasurementQuality-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using angle_meas_quality_ext_ies_o = protocol_ext_empty_o;

using angle_meas_quality_ext_ies_container = protocol_ext_container_empty_l;

// AngleMeasurementQuality ::= SEQUENCE
struct angle_meas_quality_s {
  struct resolution_opts {
    enum options { deg0dot1, /*...*/ nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using resolution_e_ = enumerated<resolution_opts, true>;

  // member variables
  bool                                 zenith_quality_present = false;
  bool                                 ie_exts_present        = false;
  uint16_t                             azimuth_quality        = 0;
  uint16_t                             zenith_quality         = 0;
  resolution_e_                        resolution;
  angle_meas_quality_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PhaseQuality-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using phase_quality_ext_ies_o = protocol_ext_empty_o;

using phase_quality_ext_ies_container = protocol_ext_container_empty_l;

// PhaseQuality ::= SEQUENCE
struct phase_quality_s {
  struct phase_quality_resolution_opts {
    enum options { deg0dot1, deg1, /*...*/ nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using phase_quality_resolution_e_ = enumerated<phase_quality_resolution_opts, true>;

  // member variables
  bool                            ie_exts_present   = false;
  uint8_t                         phase_quality_idx = 0;
  phase_quality_resolution_e_     phase_quality_resolution;
  phase_quality_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPMeasurementQuality-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct trp_meas_quality_item_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { phase_quality, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::phase_quality; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    phase_quality_s&       phase_quality() { return c; }
    const phase_quality_s& phase_quality() const { return c; }

  private:
    phase_quality_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// TRPMeasurementQuality-Item ::= CHOICE
struct trp_meas_quality_item_c {
  struct types_opts {
    enum options { timing_meas_quality, angle_meas_quality, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  trp_meas_quality_item_c() = default;
  trp_meas_quality_item_c(const trp_meas_quality_item_c& other);
  trp_meas_quality_item_c& operator=(const trp_meas_quality_item_c& other);
  ~trp_meas_quality_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  timing_meas_quality_s& timing_meas_quality()
  {
    assert_choice_type(types::timing_meas_quality, type_, "TRPMeasurementQuality-Item");
    return c.get<timing_meas_quality_s>();
  }
  angle_meas_quality_s& angle_meas_quality()
  {
    assert_choice_type(types::angle_meas_quality, type_, "TRPMeasurementQuality-Item");
    return c.get<angle_meas_quality_s>();
  }
  protocol_ie_single_container_s<trp_meas_quality_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TRPMeasurementQuality-Item");
    return c.get<protocol_ie_single_container_s<trp_meas_quality_item_ext_ies_o>>();
  }
  const timing_meas_quality_s& timing_meas_quality() const
  {
    assert_choice_type(types::timing_meas_quality, type_, "TRPMeasurementQuality-Item");
    return c.get<timing_meas_quality_s>();
  }
  const angle_meas_quality_s& angle_meas_quality() const
  {
    assert_choice_type(types::angle_meas_quality, type_, "TRPMeasurementQuality-Item");
    return c.get<angle_meas_quality_s>();
  }
  const protocol_ie_single_container_s<trp_meas_quality_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TRPMeasurementQuality-Item");
    return c.get<protocol_ie_single_container_s<trp_meas_quality_item_ext_ies_o>>();
  }
  timing_meas_quality_s&                                           set_timing_meas_quality();
  angle_meas_quality_s&                                            set_angle_meas_quality();
  protocol_ie_single_container_s<trp_meas_quality_item_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<angle_meas_quality_s,
                  protocol_ie_single_container_s<trp_meas_quality_item_ext_ies_o>,
                  timing_meas_quality_s>
      c;

  void destroy_();
};

// TRPMeasurementQuality-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_meas_quality_ext_ies_o = protocol_ext_empty_o;

using trp_meas_quality_ext_ies_container = protocol_ext_container_empty_l;

// TRPMeasurementQuality ::= SEQUENCE
struct trp_meas_quality_s {
  bool                               ie_exts_present = false;
  trp_meas_quality_item_c            tr_pmeas_quality_item;
  trp_meas_quality_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LCS-to-GCS-Translation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using lcs_to_gcs_translation_ext_ies_o = protocol_ext_empty_o;

using lcs_to_gcs_translation_ext_ies_container = protocol_ext_container_empty_l;

// LCS-to-GCS-Translation ::= SEQUENCE
struct lcs_to_gcs_translation_s {
  bool                                     ie_exts_present = false;
  uint16_t                                 alpha           = 0;
  uint16_t                                 beta            = 0;
  uint16_t                                 gamma           = 0;
  lcs_to_gcs_translation_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-AoA-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ul_ao_a_ext_ies_o = protocol_ext_empty_o;

using ul_ao_a_ext_ies_container = protocol_ext_container_empty_l;

// UL-AoA ::= SEQUENCE
struct ul_ao_a_s {
  bool                      zenith_ao_a_present            = false;
  bool                      lcs_to_gcs_translation_present = false;
  bool                      ie_exts_present                = false;
  uint16_t                  azimuth_ao_a                   = 0;
  uint16_t                  zenith_ao_a                    = 0;
  lcs_to_gcs_translation_s  lcs_to_gcs_translation;
  ul_ao_a_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ZoAInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using zo_a_info_ext_ies_o = protocol_ext_empty_o;

using zo_a_info_ext_ies_container = protocol_ext_container_empty_l;

// ZoAInformation ::= SEQUENCE
struct zo_a_info_s {
  bool                        lcs_to_gcs_translation_present = false;
  bool                        ie_exts_present                = false;
  uint16_t                    zenith_ao_a                    = 0;
  lcs_to_gcs_translation_s    lcs_to_gcs_translation;
  zo_a_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MultipleULAoA-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using multiple_ul_ao_a_item_ext_ies_o = protocol_ies_empty_o;

// MultipleULAoA-Item ::= CHOICE
struct multiple_ul_ao_a_item_c {
  struct types_opts {
    enum options { ul_ao_a, ul_zo_a, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  multiple_ul_ao_a_item_c() = default;
  multiple_ul_ao_a_item_c(const multiple_ul_ao_a_item_c& other);
  multiple_ul_ao_a_item_c& operator=(const multiple_ul_ao_a_item_c& other);
  ~multiple_ul_ao_a_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ul_ao_a_s& ul_ao_a()
  {
    assert_choice_type(types::ul_ao_a, type_, "MultipleULAoA-Item");
    return c.get<ul_ao_a_s>();
  }
  zo_a_info_s& ul_zo_a()
  {
    assert_choice_type(types::ul_zo_a, type_, "MultipleULAoA-Item");
    return c.get<zo_a_info_s>();
  }
  protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MultipleULAoA-Item");
    return c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>();
  }
  const ul_ao_a_s& ul_ao_a() const
  {
    assert_choice_type(types::ul_ao_a, type_, "MultipleULAoA-Item");
    return c.get<ul_ao_a_s>();
  }
  const zo_a_info_s& ul_zo_a() const
  {
    assert_choice_type(types::ul_zo_a, type_, "MultipleULAoA-Item");
    return c.get<zo_a_info_s>();
  }
  const protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MultipleULAoA-Item");
    return c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>();
  }
  ul_ao_a_s&                                                       set_ul_ao_a();
  zo_a_info_s&                                                     set_ul_zo_a();
  protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                    type_;
  choice_buffer_t<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>, ul_ao_a_s, zo_a_info_s> c;

  void destroy_();
};

// MultipleULAoA-List ::= SEQUENCE (SIZE (1..8)) OF MultipleULAoA-Item
using multiple_ul_ao_a_list_l = dyn_array<multiple_ul_ao_a_item_c>;

// MultipleULAoA-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multiple_ul_ao_a_ext_ies_o = protocol_ext_empty_o;

using multiple_ul_ao_a_ext_ies_container = protocol_ext_container_empty_l;

// MultipleULAoA ::= SEQUENCE
struct multiple_ul_ao_a_s {
  bool                               ie_exts_present = false;
  multiple_ul_ao_a_list_l            multiple_ul_ao_a;
  multiple_ul_ao_a_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-SRS-RSRPP-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ul_srs_rsrp_p_ext_ies_o = protocol_ext_empty_o;

using ul_srs_rsrp_p_ext_ies_container = protocol_ext_container_empty_l;

// UL-SRS-RSRPP ::= SEQUENCE
struct ul_srs_rsrp_p_s {
  bool                            ie_exts_present   = false;
  uint8_t                         first_path_rsrp_p = 0;
  ul_srs_rsrp_p_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalPath-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct add_path_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct add_path_item_ext_ies_container {
  bool               multiple_ul_ao_a_present = false;
  bool               path_pwr_present         = false;
  multiple_ul_ao_a_s multiple_ul_ao_a;
  ul_srs_rsrp_p_s    path_pwr;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalPath-Item ::= SEQUENCE
struct add_path_item_s {
  bool                            path_quality_present = false;
  bool                            ie_exts_present      = false;
  relative_path_delay_c           relative_path_delay;
  trp_meas_quality_s              path_quality;
  add_path_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalPath-List ::= SEQUENCE (SIZE (1..2)) OF AdditionalPath-Item
using add_path_list_l = dyn_array<add_path_item_s>;

// AdditionalSIBMessageList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using add_sib_msg_list_item_ext_ies_o = protocol_ext_empty_o;

using add_sib_msg_list_item_ext_ies_container = protocol_ext_container_empty_l;

// AdditionalSIBMessageList-Item ::= SEQUENCE
struct add_sib_msg_list_item_s {
  bool                                    ie_exts_present = false;
  unbounded_octstring<true>               add_sib;
  add_sib_msg_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalSIBMessageList ::= SEQUENCE (SIZE (1..63)) OF AdditionalSIBMessageList-Item
using add_sib_msg_list_l = dyn_array<add_sib_msg_list_item_s>;

// AerialUE ::= ENUMERATED
struct aerial_ue_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using aerial_ue_e = enumerated<aerial_ue_opts, true>;

// AffectedSSB-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using affected_ssb_item_ext_ies_o = protocol_ext_empty_o;

using affected_ssb_item_ext_ies_container = protocol_ext_container_empty_l;

// AffectedSSB-Item ::= SEQUENCE
struct affected_ssb_item_s {
  bool                                ie_exts_present = false;
  uint8_t                             ssb_idx         = 0;
  affected_ssb_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AffectedSSB-List ::= SEQUENCE (SIZE (1..64)) OF AffectedSSB-Item
using affected_ssb_list_l = dyn_array<affected_ssb_item_s>;

// AffectedCellsAndBeams-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using affected_cells_and_beams_item_ext_ies_o = protocol_ext_empty_o;

using affected_cells_and_beams_item_ext_ies_container = protocol_ext_container_empty_l;

// AffectedCellsAndBeams-Item ::= SEQUENCE
struct affected_cells_and_beams_item_s {
  bool                                            ie_exts_present = false;
  nr_cgi_s                                        nr_cgi;
  affected_ssb_list_l                             affected_ssb_list;
  affected_cells_and_beams_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AffectedCellsAndBeams-List ::= SEQUENCE (SIZE (1..32)) OF AffectedCellsAndBeams-Item
using affected_cells_and_beams_list_l = dyn_array<affected_cells_and_beams_item_s>;

// Aggregated-PosSRS-Resource-ID-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct aggr_pos_srs_res_id_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct aggr_pos_srs_res_id_item_ext_ies_container {
  bool                   nr_pci_present = false;
  uint32_t               point_a;
  scs_specific_carrier_s scs_specific_carrier;
  uint16_t               nr_pci;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Aggregated-PosSRS-Resource-ID-Item ::= SEQUENCE
struct aggr_pos_srs_res_id_item_s {
  bool                                       ie_exts_present = false;
  uint8_t                                    positioning_srs = 0;
  aggr_pos_srs_res_id_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DL-PRS-ResourceSet-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_prs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using dl_prs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// DL-PRS-ResourceSet-Item ::= SEQUENCE
struct dl_prs_res_set_item_s {
  bool                                  ie_exts_present    = false;
  uint8_t                               dl_prs_res_set_idx = 1;
  dl_prs_res_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DL-PRS-ResourceSet-List ::= SEQUENCE (SIZE (1..3)) OF DL-PRS-ResourceSet-Item
using dl_prs_res_set_list_l = dyn_array<dl_prs_res_set_item_s>;

// AggregatedPRSResourceSet-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using aggr_prs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using aggr_prs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// AggregatedPRSResourceSet-Item ::= SEQUENCE
struct aggr_prs_res_set_item_s {
  bool                                    ie_exts_present = false;
  dl_prs_res_set_list_l                   dl_prs_res_set_list;
  aggr_prs_res_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AggregatedPRSResourceSetList ::= SEQUENCE (SIZE (1..2)) OF AggregatedPRSResourceSet-Item
using aggr_prs_res_set_list_l = dyn_array<aggr_prs_res_set_item_s>;

// AggregatedPosSRSResourceIDList ::= SEQUENCE (SIZE (2..3)) OF Aggregated-PosSRS-Resource-ID-Item
using aggr_pos_srs_res_id_list_l = dyn_array<aggr_pos_srs_res_id_item_s>;

// AggressorCellList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using aggressor_cell_list_item_ext_ies_o = protocol_ext_empty_o;

using aggressor_cell_list_item_ext_ies_container = protocol_ext_container_empty_l;

// AggressorCellList-Item ::= SEQUENCE
struct aggressor_cell_list_item_s {
  bool                                       ie_exts_present = false;
  nr_cgi_s                                   aggressor_cell_id;
  aggressor_cell_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AggressorCellList ::= SEQUENCE (SIZE (1..512)) OF AggressorCellList-Item
using aggressor_cell_list_l = dyn_array<aggressor_cell_list_item_s>;

// AggressorgNBSetID-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using aggressor_gnb_set_id_ext_ies_o = protocol_ext_empty_o;

using aggressor_gnb_set_id_ext_ies_container = protocol_ext_container_empty_l;

// AggressorgNBSetID ::= SEQUENCE
struct aggressor_gnb_set_id_s {
  bool                                   ie_exts_present = false;
  fixed_bitstring<22, false, true>       aggressor_gnb_set_id;
  aggressor_gnb_set_id_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Pre-emptionCapability ::= ENUMERATED
struct pre_emption_cap_opts {
  enum options { shall_not_trigger_pre_emption, may_trigger_pre_emption, nulltype } value;

  const char* to_string() const;
};
using pre_emption_cap_e = enumerated<pre_emption_cap_opts>;

// Pre-emptionVulnerability ::= ENUMERATED
struct pre_emption_vulnerability_opts {
  enum options { not_pre_emptable, pre_emptable, nulltype } value;

  const char* to_string() const;
};
using pre_emption_vulnerability_e = enumerated<pre_emption_vulnerability_opts>;

// AllocationAndRetentionPriority-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using alloc_and_retention_prio_ext_ies_o = protocol_ext_empty_o;

using alloc_and_retention_prio_ext_ies_container = protocol_ext_container_empty_l;

// AllocationAndRetentionPriority ::= SEQUENCE
struct alloc_and_retention_prio_s {
  bool                                       ie_exts_present = false;
  uint8_t                                    prio_level      = 0;
  pre_emption_cap_e                          pre_emption_cap;
  pre_emption_vulnerability_e                pre_emption_vulnerability;
  alloc_and_retention_prio_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AllowedPeriodicityList ::= SEQUENCE (SIZE (1..8)) OF INTEGER (0..640000,...)
using allowed_periodicity_list_l = bounded_array<uint32_t, 8>;

// PacketErrorRate-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using packet_error_rate_ext_ies_o = protocol_ext_empty_o;

using packet_error_rate_ext_ies_container = protocol_ext_container_empty_l;

// PacketErrorRate ::= SEQUENCE
struct packet_error_rate_s {
  bool                                ie_exts_present = false;
  uint8_t                             per_scalar      = 0;
  uint8_t                             per_exponent    = 0;
  packet_error_rate_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AlternativeQoSParaSetItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct alt_qos_para_set_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct alt_qos_para_set_item_ext_ies_container {
  bool     max_data_burst_volume_present = false;
  uint16_t max_data_burst_volume;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AlternativeQoSParaSetItem ::= SEQUENCE
struct alt_qos_para_set_item_s {
  bool                                    guaranteed_flow_bit_rate_dl_present = false;
  bool                                    guaranteed_flow_bit_rate_ul_present = false;
  bool                                    packet_delay_budget_present         = false;
  bool                                    packet_error_rate_present           = false;
  bool                                    ie_exts_present                     = false;
  uint8_t                                 alt_qos_para_set_idx                = 1;
  uint64_t                                guaranteed_flow_bit_rate_dl         = 0;
  uint64_t                                guaranteed_flow_bit_rate_ul         = 0;
  uint16_t                                packet_delay_budget                 = 0;
  packet_error_rate_s                     packet_error_rate;
  alt_qos_para_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AlternativeQoSParaSetList ::= SEQUENCE (SIZE (1..8)) OF AlternativeQoSParaSetItem
using alt_qos_para_set_list_l = dyn_array<alt_qos_para_set_item_s>;

// Expected-Azimuth-AoA-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using expected_azimuth_ao_a_ext_ies_o = protocol_ext_empty_o;

using expected_azimuth_ao_a_ext_ies_container = protocol_ext_container_empty_l;

// Expected-Azimuth-AoA ::= SEQUENCE
struct expected_azimuth_ao_a_s {
  bool                                    ie_exts_present                   = false;
  uint16_t                                expected_azimuth_ao_a_value       = 0;
  uint16_t                                expected_azimuth_ao_a_uncertainty = 0;
  expected_azimuth_ao_a_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Expected-Zenith-AoA-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using expected_zenith_ao_a_ext_ies_o = protocol_ext_empty_o;

using expected_zenith_ao_a_ext_ies_container = protocol_ext_container_empty_l;

// Expected-Zenith-AoA ::= SEQUENCE
struct expected_zenith_ao_a_s {
  bool                                   ie_exts_present                  = false;
  uint16_t                               expected_zenith_ao_a_value       = 0;
  uint16_t                               expected_zenith_ao_a_uncertainty = 0;
  expected_zenith_ao_a_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Expected-UL-AoA-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using expected_ul_ao_a_ext_ies_o = protocol_ext_empty_o;

using expected_ul_ao_a_ext_ies_container = protocol_ext_container_empty_l;

// Expected-UL-AoA ::= SEQUENCE
struct expected_ul_ao_a_s {
  bool                               expected_zenith_ao_a_present = false;
  bool                               ie_exts_present              = false;
  expected_azimuth_ao_a_s            expected_azimuth_ao_a;
  expected_zenith_ao_a_s             expected_zenith_ao_a;
  expected_ul_ao_a_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Expected-ZoA-only-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using expected_zo_a_only_ext_ies_o = protocol_ext_empty_o;

using expected_zo_a_only_ext_ies_container = protocol_ext_container_empty_l;

// Expected-ZoA-only ::= SEQUENCE
struct expected_zo_a_only_s {
  bool                                 ie_exts_present = false;
  expected_zenith_ao_a_s               expected_zo_a_only;
  expected_zo_a_only_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AngleMeasurementType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using angle_meas_type_ext_ies_o = protocol_ies_empty_o;

// AngleMeasurementType ::= CHOICE
struct angle_meas_type_c {
  struct types_opts {
    enum options { expected_ul_ao_a, expected_zo_a, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  angle_meas_type_c() = default;
  angle_meas_type_c(const angle_meas_type_c& other);
  angle_meas_type_c& operator=(const angle_meas_type_c& other);
  ~angle_meas_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  expected_ul_ao_a_s& expected_ul_ao_a()
  {
    assert_choice_type(types::expected_ul_ao_a, type_, "AngleMeasurementType");
    return c.get<expected_ul_ao_a_s>();
  }
  expected_zo_a_only_s& expected_zo_a()
  {
    assert_choice_type(types::expected_zo_a, type_, "AngleMeasurementType");
    return c.get<expected_zo_a_only_s>();
  }
  protocol_ie_single_container_s<angle_meas_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "AngleMeasurementType");
    return c.get<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>();
  }
  const expected_ul_ao_a_s& expected_ul_ao_a() const
  {
    assert_choice_type(types::expected_ul_ao_a, type_, "AngleMeasurementType");
    return c.get<expected_ul_ao_a_s>();
  }
  const expected_zo_a_only_s& expected_zo_a() const
  {
    assert_choice_type(types::expected_zo_a, type_, "AngleMeasurementType");
    return c.get<expected_zo_a_only_s>();
  }
  const protocol_ie_single_container_s<angle_meas_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "AngleMeasurementType");
    return c.get<protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>();
  }
  expected_ul_ao_a_s&                                        set_expected_ul_ao_a();
  expected_zo_a_only_s&                                      set_expected_zo_a();
  protocol_ie_single_container_s<angle_meas_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<expected_ul_ao_a_s, expected_zo_a_only_s, protocol_ie_single_container_s<angle_meas_type_ext_ies_o>>
      c;

  void destroy_();
};

// AoA-AssistanceInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ao_a_assist_info_ext_ies_o = protocol_ext_empty_o;

using ao_a_assist_info_ext_ies_container = protocol_ext_container_empty_l;

// AoA-AssistanceInfo ::= SEQUENCE
struct ao_a_assist_info_s {
  bool                               lcs_to_gcs_translation_present = false;
  bool                               ie_exts_present                = false;
  angle_meas_type_c                  angle_meas;
  lcs_to_gcs_translation_s           lcs_to_gcs_translation;
  ao_a_assist_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AperiodicSRSResourceTriggerList ::= SEQUENCE (SIZE (1..3)) OF INTEGER (1..3)
using aperiodic_srs_res_trigger_list_l = bounded_array<uint8_t, 3>;

// AreaScope ::= ENUMERATED
struct area_scope_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using area_scope_e = enumerated<area_scope_opts, true>;

// DL-PRS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_prs_ext_ies_o = protocol_ext_empty_o;

using dl_prs_ext_ies_container = protocol_ext_container_empty_l;

// DL-PRS ::= SEQUENCE
struct dl_prs_s {
  bool                     dl_prs_res_id_present = false;
  bool                     ie_exts_present       = false;
  uint16_t                 prsid                 = 0;
  uint8_t                  dl_prs_res_set_id     = 0;
  uint8_t                  dl_prs_res_id         = 0;
  dl_prs_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ReferenceSignal-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using ref_sig_ext_ies_o = protocol_ies_empty_o;

// ReferenceSignal ::= CHOICE
struct ref_sig_c {
  struct types_opts {
    enum options { nzp_csi_rs, ssb, srs, positioning_srs, dl_prs, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ref_sig_c() = default;
  ref_sig_c(const ref_sig_c& other);
  ref_sig_c& operator=(const ref_sig_c& other);
  ~ref_sig_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& nzp_csi_rs()
  {
    assert_choice_type(types::nzp_csi_rs, type_, "ReferenceSignal");
    return c.get<uint8_t>();
  }
  ssb_s& ssb()
  {
    assert_choice_type(types::ssb, type_, "ReferenceSignal");
    return c.get<ssb_s>();
  }
  uint8_t& srs()
  {
    assert_choice_type(types::srs, type_, "ReferenceSignal");
    return c.get<uint8_t>();
  }
  uint8_t& positioning_srs()
  {
    assert_choice_type(types::positioning_srs, type_, "ReferenceSignal");
    return c.get<uint8_t>();
  }
  dl_prs_s& dl_prs()
  {
    assert_choice_type(types::dl_prs, type_, "ReferenceSignal");
    return c.get<dl_prs_s>();
  }
  protocol_ie_single_container_s<ref_sig_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ReferenceSignal");
    return c.get<protocol_ie_single_container_s<ref_sig_ext_ies_o>>();
  }
  const uint8_t& nzp_csi_rs() const
  {
    assert_choice_type(types::nzp_csi_rs, type_, "ReferenceSignal");
    return c.get<uint8_t>();
  }
  const ssb_s& ssb() const
  {
    assert_choice_type(types::ssb, type_, "ReferenceSignal");
    return c.get<ssb_s>();
  }
  const uint8_t& srs() const
  {
    assert_choice_type(types::srs, type_, "ReferenceSignal");
    return c.get<uint8_t>();
  }
  const uint8_t& positioning_srs() const
  {
    assert_choice_type(types::positioning_srs, type_, "ReferenceSignal");
    return c.get<uint8_t>();
  }
  const dl_prs_s& dl_prs() const
  {
    assert_choice_type(types::dl_prs, type_, "ReferenceSignal");
    return c.get<dl_prs_s>();
  }
  const protocol_ie_single_container_s<ref_sig_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ReferenceSignal");
    return c.get<protocol_ie_single_container_s<ref_sig_ext_ies_o>>();
  }
  uint8_t&                                           set_nzp_csi_rs();
  ssb_s&                                             set_ssb();
  uint8_t&                                           set_srs();
  uint8_t&                                           set_positioning_srs();
  dl_prs_s&                                          set_dl_prs();
  protocol_ie_single_container_s<ref_sig_ext_ies_o>& set_choice_ext();

private:
  types                                                                               type_;
  choice_buffer_t<dl_prs_s, protocol_ie_single_container_s<ref_sig_ext_ies_o>, ssb_s> c;

  void destroy_();
};

// SpatialRelationforResourceIDItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using spatial_relationfor_res_id_item_ext_ies_o = protocol_ext_empty_o;

using spatial_relationfor_res_id_item_ext_ies_container = protocol_ext_container_empty_l;

// SpatialRelationforResourceIDItem ::= SEQUENCE
struct spatial_relationfor_res_id_item_s {
  bool                                              ie_exts_present = false;
  ref_sig_c                                         ref_sig;
  spatial_relationfor_res_id_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialRelationforResourceID ::= SEQUENCE (SIZE (1..64)) OF SpatialRelationforResourceIDItem
using spatial_relationfor_res_id_l = dyn_array<spatial_relationfor_res_id_item_s>;

// SpatialRelationInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using spatial_relation_info_ext_ies_o = protocol_ext_empty_o;

using spatial_relation_info_ext_ies_container = protocol_ext_container_empty_l;

// SpatialRelationInfo ::= SEQUENCE
struct spatial_relation_info_s {
  bool                                    ie_exts_present = false;
  spatial_relationfor_res_id_l            spatial_relationfor_res_id;
  spatial_relation_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialRelationPerSRSResourceItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using spatial_relation_per_srs_res_item_ext_ies_o = protocol_ext_empty_o;

using spatial_relation_per_srs_res_item_ext_ies_container = protocol_ext_container_empty_l;

// SpatialRelationPerSRSResourceItem ::= SEQUENCE
struct spatial_relation_per_srs_res_item_s {
  bool                                                ie_exts_present = false;
  ref_sig_c                                           ref_sig;
  spatial_relation_per_srs_res_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialRelationPerSRSResource-List ::= SEQUENCE (SIZE (1..16)) OF SpatialRelationPerSRSResourceItem
using spatial_relation_per_srs_res_list_l = dyn_array<spatial_relation_per_srs_res_item_s>;

// SpatialRelationPerSRSResource-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using spatial_relation_per_srs_res_ext_ies_o = protocol_ext_empty_o;

using spatial_relation_per_srs_res_ext_ies_container = protocol_ext_container_empty_l;

// SpatialRelationPerSRSResource ::= SEQUENCE
struct spatial_relation_per_srs_res_s {
  bool                                           ie_exts_present = false;
  spatial_relation_per_srs_res_list_l            spatial_relation_per_srs_res_list;
  spatial_relation_per_srs_res_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AreaSpecificSemiPersistentSRSPosInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using area_specific_semi_persistent_srs_pos_info_ext_ies_o = protocol_ext_empty_o;

using area_specific_semi_persistent_srs_pos_info_ext_ies_container = protocol_ext_container_empty_l;

// AreaSpecificSemiPersistentSRSPosInfo ::= SEQUENCE
struct area_specific_semi_persistent_srs_pos_info_s {
  struct srs_tx_type_opts {
    enum options { activ, deactiv, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using srs_tx_type_e_ = enumerated<srs_tx_type_opts, true>;

  // member variables
  bool                                                         spatial_relation_info_present        = false;
  bool                                                         spatial_relation_per_srs_res_present = false;
  bool                                                         ie_exts_present                      = false;
  srs_tx_type_e_                                               srs_tx_type;
  uint8_t                                                      srs_res_set_id = 0;
  spatial_relation_info_s                                      spatial_relation_info;
  spatial_relation_per_srs_res_s                               spatial_relation_per_srs_res;
  area_specific_semi_persistent_srs_pos_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Associated-SCell-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using associated_scell_item_ext_ies_o = protocol_ext_empty_o;

using associated_scell_item_ext_ies_container = protocol_ext_container_empty_l;

// Associated-SCell-Item ::= SEQUENCE
struct associated_scell_item_s {
  bool                                    ie_exts_present = false;
  nr_cgi_s                                scell_id;
  associated_scell_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AvailablePLMNList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using available_plmn_list_item_ext_ies_o = protocol_ext_empty_o;

using available_plmn_list_item_ext_ies_container = protocol_ext_container_empty_l;

// AvailablePLMNList-Item ::= SEQUENCE
struct available_plmn_list_item_s {
  bool                                       ie_exts_present = false;
  fixed_octstring<3, true>                   plmn_id;
  available_plmn_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AvailablePLMNList ::= SEQUENCE (SIZE (1..6)) OF AvailablePLMNList-Item
using available_plmn_list_l = dyn_array<available_plmn_list_item_s>;

// BroadcastNIDList ::= SEQUENCE (SIZE (1..12)) OF BIT STRING (SIZE (44))
using broadcast_n_id_list_l = bounded_array<fixed_bitstring<44, false, true>, 12>;

// AvailableSNPN-ID-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using available_sn_pn_id_list_item_ext_ies_o = protocol_ext_empty_o;

using available_sn_pn_id_list_item_ext_ies_container = protocol_ext_container_empty_l;

// AvailableSNPN-ID-List-Item ::= SEQUENCE
struct available_sn_pn_id_list_item_s {
  bool                                           ie_exts_present = false;
  fixed_octstring<3, true>                       plmn_id;
  broadcast_n_id_list_l                          available_n_id_list;
  available_sn_pn_id_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AvailableSNPN-ID-List ::= SEQUENCE (SIZE (1..12)) OF AvailableSNPN-ID-List-Item
using available_sn_pn_id_list_l = dyn_array<available_sn_pn_id_list_item_s>;

// BAP-Header-Rewriting-Added-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bap_hdr_rewriting_added_list_item_ext_ies_o = protocol_ext_empty_o;

using bap_hdr_rewriting_added_list_item_ext_ies_container = protocol_ext_container_empty_l;

// BAP-Header-Rewriting-Added-List-Item ::= SEQUENCE
struct bap_hdr_rewriting_added_list_item_s {
  bool                                                non_f1terminating_topology_ind_present = false;
  bool                                                ie_exts_present                        = false;
  bap_routing_id_s                                    ingress_bap_routing_id;
  bap_routing_id_s                                    egress_bap_routing_id;
  non_f1terminating_topology_ind_e                    non_f1terminating_topology_ind;
  bap_hdr_rewriting_added_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BAP-Header-Rewriting-Removed-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bap_hdr_rewriting_remd_list_item_ext_ies_o = protocol_ext_empty_o;

using bap_hdr_rewriting_remd_list_item_ext_ies_container = protocol_ext_container_empty_l;

// BAP-Header-Rewriting-Removed-List-Item ::= SEQUENCE
struct bap_hdr_rewriting_remd_list_item_s {
  bool                                               ie_exts_present = false;
  bap_routing_id_s                                   ingress_bap_routing_id;
  bap_hdr_rewriting_remd_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BAPCtrlPDUChannel ::= ENUMERATED
struct bap_ctrl_pdu_ch_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using bap_ctrl_pdu_ch_e = enumerated<bap_ctrl_pdu_ch_opts, true>;

// IngressNonF1terminatingTopologyIndicator ::= ENUMERATED
struct ingress_non_f1terminating_topology_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ingress_non_f1terminating_topology_ind_e = enumerated<ingress_non_f1terminating_topology_ind_opts, true>;

// EgressNonF1terminatingTopologyIndicator ::= ENUMERATED
struct egress_non_f1terminating_topology_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using egress_non_f1terminating_topology_ind_e = enumerated<egress_non_f1terminating_topology_ind_opts, true>;

// BAPlayerBHRLCchannelMappingInfo-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct ba_player_bh_rlc_ch_map_info_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ba_player_bh_rlc_ch_map_info_item_ext_ies_container {
  bool                                     ingress_non_f1terminating_topology_ind_present = false;
  bool                                     egress_non_f1terminating_topology_ind_present  = false;
  ingress_non_f1terminating_topology_ind_e ingress_non_f1terminating_topology_ind;
  egress_non_f1terminating_topology_ind_e  egress_non_f1terminating_topology_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BAPlayerBHRLCchannelMappingInfo-Item ::= SEQUENCE
struct ba_player_bh_rlc_ch_map_info_item_s {
  bool                                                prior_hop_bap_address_present = false;
  bool                                                ingressb_h_rlc_ch_id_present  = false;
  bool                                                next_hop_bap_address_present  = false;
  bool                                                egressb_h_rlc_ch_id_present   = false;
  bool                                                ie_exts_present               = false;
  fixed_bitstring<26, false, true>                    map_info_idx;
  fixed_bitstring<10, false, true>                    prior_hop_bap_address;
  fixed_bitstring<16, false, true>                    ingressb_h_rlc_ch_id;
  fixed_bitstring<10, false, true>                    next_hop_bap_address;
  fixed_bitstring<16, false, true>                    egressb_h_rlc_ch_id;
  ba_player_bh_rlc_ch_map_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BAPlayerBHRLCchannelMappingInfoList ::= SEQUENCE (SIZE (1..67108864)) OF BAPlayerBHRLCchannelMappingInfo-Item
using ba_player_bh_rlc_ch_map_info_list_l = dyn_array<ba_player_bh_rlc_ch_map_info_item_s>;

// MappingInformationtoRemove ::= SEQUENCE (SIZE (1..67108864)) OF BIT STRING (SIZE (26))
using map_infoto_rem_l = dyn_array<fixed_bitstring<26, false, true>>;

// BAPlayerBHRLCchannelMappingInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ba_player_bh_rlc_ch_map_info_ext_ies_o = protocol_ext_empty_o;

using ba_player_bh_rlc_ch_map_info_ext_ies_container = protocol_ext_container_empty_l;

// BAPlayerBHRLCchannelMappingInfo ::= SEQUENCE
struct ba_player_bh_rlc_ch_map_info_s {
  bool                                           ie_exts_present = false;
  ba_player_bh_rlc_ch_map_info_list_l            ba_player_bh_rlc_ch_map_info_to_add;
  map_infoto_rem_l                               ba_player_bh_rlc_ch_map_info_to_rem;
  ba_player_bh_rlc_ch_map_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// F1UTunnelNotEstablished ::= ENUMERATED
struct f1_u_tunnel_not_established_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using f1_u_tunnel_not_established_e = enumerated<f1_u_tunnel_not_established_opts, true>;

// MBSF1UInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct mbsf1_u_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct mbsf1_u_info_ext_ies_container {
  bool                          f1_u_tunnel_not_established_present = false;
  f1_u_tunnel_not_established_e f1_u_tunnel_not_established;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBSF1UInformation ::= SEQUENCE
struct mbsf1_u_info_s {
  bool                           ie_exts_present = false;
  up_transport_layer_info_c      mbs_f1u_info;
  mbsf1_u_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LocationDependentMBSF1UInformation-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct location_dependent_mbsf1_u_info_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct location_dependent_mbsf1_u_info_item_ext_ies_container {
  bool                          f1_u_tunnel_not_established_present = false;
  f1_u_tunnel_not_established_e f1_u_tunnel_not_established;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LocationDependentMBSF1UInformation-Item ::= SEQUENCE
struct location_dependent_mbsf1_u_info_item_s {
  bool                                                   ie_exts_present     = false;
  uint32_t                                               mbs_area_session_id = 0;
  up_transport_layer_info_c                              mbs_f1u_info_at_cu;
  location_dependent_mbsf1_u_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LocationDependentMBSF1UInformation ::= SEQUENCE (SIZE (1..256)) OF LocationDependentMBSF1UInformation-Item
using location_dependent_mbsf1_u_info_l = dyn_array<location_dependent_mbsf1_u_info_item_s>;

// BCBearerContextF1U-TNLInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using bc_bearer_context_f1_u_tnl_info_ext_ies_o = protocol_ies_empty_o;

// BCBearerContextF1U-TNLInfo ::= CHOICE
struct bc_bearer_context_f1_u_tnl_info_c {
  struct types_opts {
    enum options { locationindpendent, locationdependent, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  bc_bearer_context_f1_u_tnl_info_c() = default;
  bc_bearer_context_f1_u_tnl_info_c(const bc_bearer_context_f1_u_tnl_info_c& other);
  bc_bearer_context_f1_u_tnl_info_c& operator=(const bc_bearer_context_f1_u_tnl_info_c& other);
  ~bc_bearer_context_f1_u_tnl_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  mbsf1_u_info_s& locationindpendent()
  {
    assert_choice_type(types::locationindpendent, type_, "BCBearerContextF1U-TNLInfo");
    return c.get<mbsf1_u_info_s>();
  }
  location_dependent_mbsf1_u_info_l& locationdependent()
  {
    assert_choice_type(types::locationdependent, type_, "BCBearerContextF1U-TNLInfo");
    return c.get<location_dependent_mbsf1_u_info_l>();
  }
  protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "BCBearerContextF1U-TNLInfo");
    return c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_info_ext_ies_o>>();
  }
  const mbsf1_u_info_s& locationindpendent() const
  {
    assert_choice_type(types::locationindpendent, type_, "BCBearerContextF1U-TNLInfo");
    return c.get<mbsf1_u_info_s>();
  }
  const location_dependent_mbsf1_u_info_l& locationdependent() const
  {
    assert_choice_type(types::locationdependent, type_, "BCBearerContextF1U-TNLInfo");
    return c.get<location_dependent_mbsf1_u_info_l>();
  }
  const protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "BCBearerContextF1U-TNLInfo");
    return c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_info_ext_ies_o>>();
  }
  mbsf1_u_info_s&                                                            set_locationindpendent();
  location_dependent_mbsf1_u_info_l&                                         set_locationdependent();
  protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<location_dependent_mbsf1_u_info_l,
                  mbsf1_u_info_s,
                  protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_info_ext_ies_o>>
      c;

  void destroy_();
};

// BH-Routing-Information-Added-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct bh_routing_info_added_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct bh_routing_info_added_list_item_ext_ies_container {
  bool                             non_f1terminating_topology_ind_present = false;
  non_f1terminating_topology_ind_e non_f1terminating_topology_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BH-Routing-Information-Added-List-Item ::= SEQUENCE
struct bh_routing_info_added_list_item_s {
  bool                                              ie_exts_present = false;
  bap_routing_id_s                                  bap_routing_id;
  fixed_bitstring<10, false, true>                  next_hop_bap_address;
  bh_routing_info_added_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BH-Routing-Information-Removed-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_routing_info_remd_list_item_ext_ies_o = protocol_ext_empty_o;

using bh_routing_info_remd_list_item_ext_ies_container = protocol_ext_container_empty_l;

// BH-Routing-Information-Removed-List-Item ::= SEQUENCE
struct bh_routing_info_remd_list_item_s {
  bool                                             ie_exts_present = false;
  bap_routing_id_s                                 bap_routing_id;
  bh_routing_info_remd_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CauseRadioNetwork ::= ENUMERATED
struct cause_radio_network_opts {
  enum options {
    unspecified,
    rl_fail_rlc,
    unknown_or_already_allocated_gnb_cu_ue_f1ap_id,
    unknown_or_already_allocated_gnb_du_ue_f1ap_id,
    unknown_or_inconsistent_pair_of_ue_f1ap_id,
    interaction_with_other_proc,
    not_supported_qci_value,
    action_desirable_for_radio_reasons,
    no_radio_res_available,
    proc_cancelled,
    normal_release,
    // ...
    cell_not_available,
    rl_fail_others,
    ue_rejection,
    res_not_available_for_the_slice,
    amf_initiated_abnormal_release,
    release_due_to_pre_emption,
    plmn_not_served_by_the_gnb_cu,
    multiple_drb_id_instances,
    unknown_drb_id,
    multiple_bh_rlc_ch_id_instances,
    unknown_bh_rlc_ch_id,
    cho_cpc_res_tobechanged,
    npn_not_supported,
    npn_access_denied,
    gnb_cu_cell_capacity_exceeded,
    report_characteristics_empty,
    existing_meas_id,
    meas_temporarily_not_available,
    meas_not_supported_for_the_obj,
    unknown_bh_address,
    unknown_bap_routing_id,
    insufficient_ue_cap,
    scg_activation_deactivation_fail,
    scg_deactivation_fail_due_to_data_tx,
    requested_item_not_supported_on_time,
    unknown_or_already_allocated_gnb_cu_mbs_f1ap_id,
    unknown_or_already_allocated_gnb_du_mbs_f1ap_id,
    unknown_or_inconsistent_pair_of_mbs_f1ap_id,
    unknown_or_inconsistent_mrb_id,
    tat_sdt_expiry,
    ltm_cmd_triggered,
    ssb_not_available,
    nulltype
  } value;

  const char* to_string() const;
};
using cause_radio_network_e = enumerated<cause_radio_network_opts, true, 32>;

// CauseTransport ::= ENUMERATED
struct cause_transport_opts {
  enum options {
    unspecified,
    transport_res_unavailable,
    // ...
    unknown_tnl_address_for_iab,
    unknown_up_tnl_info_for_iab,
    nulltype
  } value;

  const char* to_string() const;
};
using cause_transport_e = enumerated<cause_transport_opts, true, 2>;

// CauseProtocol ::= ENUMERATED
struct cause_protocol_opts {
  enum options {
    transfer_syntax_error,
    abstract_syntax_error_reject,
    abstract_syntax_error_ignore_and_notify,
    msg_not_compatible_with_receiver_state,
    semantic_error,
    abstract_syntax_error_falsely_constructed_msg,
    unspecified,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using cause_protocol_e = enumerated<cause_protocol_opts, true>;

// CauseMisc ::= ENUMERATED
struct cause_misc_opts {
  enum options {
    ctrl_processing_overload,
    not_enough_user_plane_processing_res,
    hardware_fail,
    om_intervention,
    unspecified,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using cause_misc_e = enumerated<cause_misc_opts, true>;

// Cause-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using cause_ext_ies_o = protocol_ies_empty_o;

// Cause ::= CHOICE
struct cause_c {
  struct types_opts {
    enum options { radio_network, transport, protocol, misc, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cause_c() = default;
  cause_c(const cause_c& other);
  cause_c& operator=(const cause_c& other);
  ~cause_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  cause_radio_network_e& radio_network()
  {
    assert_choice_type(types::radio_network, type_, "Cause");
    return c.get<cause_radio_network_e>();
  }
  cause_transport_e& transport()
  {
    assert_choice_type(types::transport, type_, "Cause");
    return c.get<cause_transport_e>();
  }
  cause_protocol_e& protocol()
  {
    assert_choice_type(types::protocol, type_, "Cause");
    return c.get<cause_protocol_e>();
  }
  cause_misc_e& misc()
  {
    assert_choice_type(types::misc, type_, "Cause");
    return c.get<cause_misc_e>();
  }
  protocol_ie_single_container_s<cause_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "Cause");
    return c.get<protocol_ie_single_container_s<cause_ext_ies_o>>();
  }
  const cause_radio_network_e& radio_network() const
  {
    assert_choice_type(types::radio_network, type_, "Cause");
    return c.get<cause_radio_network_e>();
  }
  const cause_transport_e& transport() const
  {
    assert_choice_type(types::transport, type_, "Cause");
    return c.get<cause_transport_e>();
  }
  const cause_protocol_e& protocol() const
  {
    assert_choice_type(types::protocol, type_, "Cause");
    return c.get<cause_protocol_e>();
  }
  const cause_misc_e& misc() const
  {
    assert_choice_type(types::misc, type_, "Cause");
    return c.get<cause_misc_e>();
  }
  const protocol_ie_single_container_s<cause_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "Cause");
    return c.get<protocol_ie_single_container_s<cause_ext_ies_o>>();
  }
  cause_radio_network_e&                           set_radio_network();
  cause_transport_e&                               set_transport();
  cause_protocol_e&                                set_protocol();
  cause_misc_e&                                    set_misc();
  protocol_ie_single_container_s<cause_ext_ies_o>& set_choice_ext();

private:
  types                                                            type_;
  choice_buffer_t<protocol_ie_single_container_s<cause_ext_ies_o>> c;

  void destroy_();
};

// BHChannels-FailedToBeModified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_failed_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_failed_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-FailedToBeModified-Item ::= SEQUENCE
struct bh_chs_failed_to_be_modified_item_s {
  bool                                                cause_present   = false;
  bool                                                ie_exts_present = false;
  fixed_bitstring<16, false, true>                    bh_rlc_ch_id;
  cause_c                                             cause;
  bh_chs_failed_to_be_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-FailedToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-FailedToBeSetup-Item ::= SEQUENCE
struct bh_chs_failed_to_be_setup_item_s {
  bool                                             cause_present   = false;
  bool                                             ie_exts_present = false;
  fixed_bitstring<16, false, true>                 bh_rlc_ch_id;
  cause_c                                          cause;
  bh_chs_failed_to_be_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-FailedToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_failed_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_failed_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-FailedToBeSetupMod-Item ::= SEQUENCE
struct bh_chs_failed_to_be_setup_mod_item_s {
  bool                                                 cause_present   = false;
  bool                                                 ie_exts_present = false;
  fixed_bitstring<16, false, true>                     bh_rlc_ch_id;
  cause_c                                              cause;
  bh_chs_failed_to_be_setup_mod_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-Modified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_modified_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-Modified-Item ::= SEQUENCE
struct bh_chs_modified_item_s {
  bool                                   ie_exts_present = false;
  fixed_bitstring<16, false, true>       bh_rlc_ch_id;
  bh_chs_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-Required-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_required_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_required_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-Required-ToBeReleased-Item ::= SEQUENCE
struct bh_chs_required_to_be_released_item_s {
  bool                                                  ie_exts_present = false;
  fixed_bitstring<16, false, true>                      bh_rlc_ch_id;
  bh_chs_required_to_be_released_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-Setup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_setup_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-Setup-Item ::= SEQUENCE
struct bh_chs_setup_item_s {
  bool                                ie_exts_present = false;
  fixed_bitstring<16, false, true>    bh_rlc_ch_id;
  bh_chs_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-SetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-SetupMod-Item ::= SEQUENCE
struct bh_chs_setup_mod_item_s {
  bool                                    ie_exts_present = false;
  fixed_bitstring<16, false, true>        bh_rlc_ch_id;
  bh_chs_setup_mod_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NonDynamic5QIDescriptor-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct non_dyn_5qi_descriptor_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct non_dyn_5qi_descriptor_ext_ies_container {
  bool     cn_packet_delay_budget_dl_present = false;
  bool     cn_packet_delay_budget_ul_present = false;
  uint32_t cn_packet_delay_budget_dl;
  uint32_t cn_packet_delay_budget_ul;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NonDynamic5QIDescriptor ::= SEQUENCE
struct non_dyn_5qi_descriptor_s {
  bool                                     qos_prio_level_present        = false;
  bool                                     averaging_win_present         = false;
  bool                                     max_data_burst_volume_present = false;
  bool                                     ie_exts_present               = false;
  uint16_t                                 five_qi                       = 0;
  uint8_t                                  qos_prio_level                = 1;
  uint16_t                                 averaging_win                 = 0;
  uint16_t                                 max_data_burst_volume         = 0;
  non_dyn_5qi_descriptor_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Dynamic5QIDescriptor-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct dyn_5qi_descriptor_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct dyn_5qi_descriptor_ext_ies_container {
  bool     extended_packet_delay_budget_present = false;
  bool     cn_packet_delay_budget_dl_present    = false;
  bool     cn_packet_delay_budget_ul_present    = false;
  uint32_t extended_packet_delay_budget;
  uint32_t cn_packet_delay_budget_dl;
  uint32_t cn_packet_delay_budget_ul;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Dynamic5QIDescriptor ::= SEQUENCE
struct dyn_5qi_descriptor_s {
  struct delay_crit_opts {
    enum options { delay_crit, non_delay_crit, nulltype } value;

    const char* to_string() const;
  };
  using delay_crit_e_ = enumerated<delay_crit_opts>;

  // member variables
  bool                                 five_qi_present               = false;
  bool                                 delay_crit_present            = false;
  bool                                 averaging_win_present         = false;
  bool                                 max_data_burst_volume_present = false;
  bool                                 ie_exts_present               = false;
  uint8_t                              qos_prio_level                = 1;
  uint16_t                             packet_delay_budget           = 0;
  packet_error_rate_s                  packet_error_rate;
  uint16_t                             five_qi = 0;
  delay_crit_e_                        delay_crit;
  uint16_t                             averaging_win         = 0;
  uint16_t                             max_data_burst_volume = 0;
  dyn_5qi_descriptor_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoS-Characteristics-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using qos_characteristics_ext_ies_o = protocol_ies_empty_o;

// QoS-Characteristics ::= CHOICE
struct qos_characteristics_c {
  struct types_opts {
    enum options { non_dyn_5qi, dyn_5qi, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  qos_characteristics_c() = default;
  qos_characteristics_c(const qos_characteristics_c& other);
  qos_characteristics_c& operator=(const qos_characteristics_c& other);
  ~qos_characteristics_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  non_dyn_5qi_descriptor_s& non_dyn_5qi()
  {
    assert_choice_type(types::non_dyn_5qi, type_, "QoS-Characteristics");
    return c.get<non_dyn_5qi_descriptor_s>();
  }
  dyn_5qi_descriptor_s& dyn_5qi()
  {
    assert_choice_type(types::dyn_5qi, type_, "QoS-Characteristics");
    return c.get<dyn_5qi_descriptor_s>();
  }
  protocol_ie_single_container_s<qos_characteristics_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "QoS-Characteristics");
    return c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>();
  }
  const non_dyn_5qi_descriptor_s& non_dyn_5qi() const
  {
    assert_choice_type(types::non_dyn_5qi, type_, "QoS-Characteristics");
    return c.get<non_dyn_5qi_descriptor_s>();
  }
  const dyn_5qi_descriptor_s& dyn_5qi() const
  {
    assert_choice_type(types::dyn_5qi, type_, "QoS-Characteristics");
    return c.get<dyn_5qi_descriptor_s>();
  }
  const protocol_ie_single_container_s<qos_characteristics_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "QoS-Characteristics");
    return c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>();
  }
  non_dyn_5qi_descriptor_s&                                      set_non_dyn_5qi();
  dyn_5qi_descriptor_s&                                          set_dyn_5qi();
  protocol_ie_single_container_s<qos_characteristics_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<dyn_5qi_descriptor_s,
                  non_dyn_5qi_descriptor_s,
                  protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>
      c;

  void destroy_();
};

// NGRANAllocationAndRetentionPriority-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ngran_alloc_and_retention_prio_ext_ies_o = protocol_ext_empty_o;

using ngran_alloc_and_retention_prio_ext_ies_container = protocol_ext_container_empty_l;

// NGRANAllocationAndRetentionPriority ::= SEQUENCE
struct ngran_alloc_and_retention_prio_s {
  bool                                             ie_exts_present = false;
  uint8_t                                          prio_level      = 0;
  pre_emption_cap_e                                pre_emption_cap;
  pre_emption_vulnerability_e                      pre_emption_vulnerability;
  ngran_alloc_and_retention_prio_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GBR-QosFlowInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct gbr_qos_flow_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct gbr_qos_flow_info_ext_ies_container {
  bool                    alt_qos_para_set_list_present = false;
  alt_qos_para_set_list_l alt_qos_para_set_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GBR-QoSFlowInformation ::= SEQUENCE
struct gbr_qos_flow_info_s {
  bool                                max_packet_loss_rate_dl_present = false;
  bool                                max_packet_loss_rate_ul_present = false;
  bool                                ie_exts_present                 = false;
  uint64_t                            max_flow_bit_rate_dl            = 0;
  uint64_t                            max_flow_bit_rate_ul            = 0;
  uint64_t                            guaranteed_flow_bit_rate_dl     = 0;
  uint64_t                            guaranteed_flow_bit_rate_ul     = 0;
  uint16_t                            max_packet_loss_rate_dl         = 0;
  uint16_t                            max_packet_loss_rate_ul         = 0;
  gbr_qos_flow_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QosMonitoringRequest ::= ENUMERATED
struct qos_monitoring_request_opts {
  enum options { ul, dl, both, /*...*/ stop, nulltype } value;

  const char* to_string() const;
};
using qos_monitoring_request_e = enumerated<qos_monitoring_request_opts, true, 1>;

// PDUSetQoSInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pdu_set_qos_info_ext_ies_o = protocol_ext_empty_o;

using pdu_set_qos_info_ext_ies_container = protocol_ext_container_empty_l;

// PDUSetQoSInformation ::= SEQUENCE
struct pdu_set_qos_info_s {
  struct pdu_set_integrated_handling_info_opts {
    enum options { true_value, false_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using pdu_set_integrated_handling_info_e_ = enumerated<pdu_set_integrated_handling_info_opts, true>;

  // member variables
  bool                                pdu_set_delay_budget_present             = false;
  bool                                pdu_set_error_rate_present               = false;
  bool                                pdu_set_integrated_handling_info_present = false;
  bool                                ie_exts_present                          = false;
  uint32_t                            pdu_set_delay_budget                     = 1;
  packet_error_rate_s                 pdu_set_error_rate;
  pdu_set_integrated_handling_info_e_ pdu_set_integrated_handling_info;
  pdu_set_qos_info_ext_ies_container  ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSetQoSParameters-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pdu_set_qos_params_ext_ies_o = protocol_ext_empty_o;

using pdu_set_qos_params_ext_ies_container = protocol_ext_container_empty_l;

// PDUSetQoSParameters ::= SEQUENCE
struct pdu_set_qos_params_s {
  bool                                 ul_pdu_set_qos_info_present = false;
  bool                                 dl_pdu_set_qos_info_present = false;
  bool                                 ie_exts_present             = false;
  pdu_set_qos_info_s                   ul_pdu_set_qos_info;
  pdu_set_qos_info_s                   dl_pdu_set_qos_info;
  pdu_set_qos_params_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowLevelQoSParameters-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct qos_flow_level_qos_params_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flow_level_qos_params_ext_ies_container {
  bool                                  pdu_session_id_present                         = false;
  bool                                  ul_pdu_session_aggr_max_bit_rate_present       = false;
  bool                                  qos_monitoring_request_present                 = false;
  bool                                  pdcp_terminating_node_dl_tnl_addr_info_present = false;
  bool                                  pdu_set_qos_params_present                     = false;
  uint16_t                              pdu_session_id;
  uint64_t                              ul_pdu_session_aggr_max_bit_rate;
  qos_monitoring_request_e              qos_monitoring_request;
  bounded_bitstring<1, 160, true, true> pdcp_terminating_node_dl_tnl_addr_info;
  pdu_set_qos_params_s                  pdu_set_qos_params;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowLevelQoSParameters ::= SEQUENCE
struct qos_flow_level_qos_params_s {
  struct reflective_qos_attribute_opts {
    enum options { subject_to, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using reflective_qos_attribute_e_ = enumerated<reflective_qos_attribute_opts, true>;

  // member variables
  bool                                        gbr_qos_flow_info_present        = false;
  bool                                        reflective_qos_attribute_present = false;
  bool                                        ie_exts_present                  = false;
  qos_characteristics_c                       qos_characteristics;
  ngran_alloc_and_retention_prio_s            ngran_alloc_retention_prio;
  gbr_qos_flow_info_s                         gbr_qos_flow_info;
  reflective_qos_attribute_e_                 reflective_qos_attribute;
  qos_flow_level_qos_params_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GBR-QosInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gbr_qos_info_ext_ies_o = protocol_ext_empty_o;

using gbr_qos_info_ext_ies_container = protocol_ext_container_empty_l;

// GBR-QosInformation ::= SEQUENCE
struct gbr_qos_info_s {
  bool                           ie_exts_present            = false;
  uint64_t                       erab_max_bitrate_dl        = 0;
  uint64_t                       erab_max_bitrate_ul        = 0;
  uint64_t                       erab_guaranteed_bitrate_dl = 0;
  uint64_t                       erab_guaranteed_bitrate_ul = 0;
  gbr_qos_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRANQoS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct eutran_qos_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct eutran_qos_ext_ies_container {
  bool                                  enb_dl_tnl_address_present = false;
  bounded_bitstring<1, 160, true, true> enb_dl_tnl_address;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRANQoS ::= SEQUENCE
struct eutran_qos_s {
  bool                         gbr_qos_info_present = false;
  bool                         ie_exts_present      = false;
  uint16_t                     qci                  = 0;
  alloc_and_retention_prio_s   alloc_and_retention_prio;
  gbr_qos_info_s               gbr_qos_info;
  eutran_qos_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHQoSInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using bh_qos_info_ext_ies_o = protocol_ies_empty_o;

// BHQoSInformation ::= CHOICE
struct bh_qos_info_c {
  struct types_opts {
    enum options { bh_rlc_ch_qos, eutran_bh_rlc_ch_qos, cp_traffic_type, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  bh_qos_info_c() = default;
  bh_qos_info_c(const bh_qos_info_c& other);
  bh_qos_info_c& operator=(const bh_qos_info_c& other);
  ~bh_qos_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  qos_flow_level_qos_params_s& bh_rlc_ch_qos()
  {
    assert_choice_type(types::bh_rlc_ch_qos, type_, "BHQoSInformation");
    return c.get<qos_flow_level_qos_params_s>();
  }
  eutran_qos_s& eutran_bh_rlc_ch_qos()
  {
    assert_choice_type(types::eutran_bh_rlc_ch_qos, type_, "BHQoSInformation");
    return c.get<eutran_qos_s>();
  }
  uint8_t& cp_traffic_type()
  {
    assert_choice_type(types::cp_traffic_type, type_, "BHQoSInformation");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<bh_qos_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "BHQoSInformation");
    return c.get<protocol_ie_single_container_s<bh_qos_info_ext_ies_o>>();
  }
  const qos_flow_level_qos_params_s& bh_rlc_ch_qos() const
  {
    assert_choice_type(types::bh_rlc_ch_qos, type_, "BHQoSInformation");
    return c.get<qos_flow_level_qos_params_s>();
  }
  const eutran_qos_s& eutran_bh_rlc_ch_qos() const
  {
    assert_choice_type(types::eutran_bh_rlc_ch_qos, type_, "BHQoSInformation");
    return c.get<eutran_qos_s>();
  }
  const uint8_t& cp_traffic_type() const
  {
    assert_choice_type(types::cp_traffic_type, type_, "BHQoSInformation");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<bh_qos_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "BHQoSInformation");
    return c.get<protocol_ie_single_container_s<bh_qos_info_ext_ies_o>>();
  }
  qos_flow_level_qos_params_s&                           set_bh_rlc_ch_qos();
  eutran_qos_s&                                          set_eutran_bh_rlc_ch_qos();
  uint8_t&                                               set_cp_traffic_type();
  protocol_ie_single_container_s<bh_qos_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<eutran_qos_s, protocol_ie_single_container_s<bh_qos_info_ext_ies_o>, qos_flow_level_qos_params_s> c;

  void destroy_();
};

// RLCMode ::= ENUMERATED
struct rlc_mode_opts {
  enum options {
    rlc_am,
    rlc_um_bidirectional,
    rlc_um_unidirectional_ul,
    rlc_um_unidirectional_dl,
    /*...*/ nulltype
  } value;

  const char* to_string() const;
};
using rlc_mode_e = enumerated<rlc_mode_opts, true>;

// IABTNLAddress-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using iab_tnl_address_ext_ies_o = protocol_ies_empty_o;

// IABTNLAddress ::= CHOICE
struct iab_tnl_address_c {
  struct types_opts {
    enum options { ip_v4_address, ip_v6_address, ip_v6_prefix, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  iab_tnl_address_c() = default;
  iab_tnl_address_c(const iab_tnl_address_c& other);
  iab_tnl_address_c& operator=(const iab_tnl_address_c& other);
  ~iab_tnl_address_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<32, false, true>& ip_v4_address()
  {
    assert_choice_type(types::ip_v4_address, type_, "IABTNLAddress");
    return c.get<fixed_bitstring<32, false, true>>();
  }
  fixed_bitstring<128, false, true>& ip_v6_address()
  {
    assert_choice_type(types::ip_v6_address, type_, "IABTNLAddress");
    return c.get<fixed_bitstring<128, false, true>>();
  }
  fixed_bitstring<64, false, true>& ip_v6_prefix()
  {
    assert_choice_type(types::ip_v6_prefix, type_, "IABTNLAddress");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  protocol_ie_single_container_s<iab_tnl_address_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "IABTNLAddress");
    return c.get<protocol_ie_single_container_s<iab_tnl_address_ext_ies_o>>();
  }
  const fixed_bitstring<32, false, true>& ip_v4_address() const
  {
    assert_choice_type(types::ip_v4_address, type_, "IABTNLAddress");
    return c.get<fixed_bitstring<32, false, true>>();
  }
  const fixed_bitstring<128, false, true>& ip_v6_address() const
  {
    assert_choice_type(types::ip_v6_address, type_, "IABTNLAddress");
    return c.get<fixed_bitstring<128, false, true>>();
  }
  const fixed_bitstring<64, false, true>& ip_v6_prefix() const
  {
    assert_choice_type(types::ip_v6_prefix, type_, "IABTNLAddress");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  const protocol_ie_single_container_s<iab_tnl_address_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "IABTNLAddress");
    return c.get<protocol_ie_single_container_s<iab_tnl_address_ext_ies_o>>();
  }
  fixed_bitstring<32, false, true>&                          set_ip_v4_address();
  fixed_bitstring<128, false, true>&                         set_ip_v6_address();
  fixed_bitstring<64, false, true>&                          set_ip_v6_prefix();
  protocol_ie_single_container_s<iab_tnl_address_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                         type_;
  choice_buffer_t<fixed_bitstring<128, false, true>, protocol_ie_single_container_s<iab_tnl_address_ext_ies_o>> c;

  void destroy_();
};

// DSInformationList ::= SEQUENCE (SIZE (0..64)) OF BIT STRING (SIZE (6))
using d_si_nformation_list_l = dyn_array<fixed_bitstring<6, false, true>>;

// IPHeaderInformation-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ip_hdr_info_item_ext_ies_o = protocol_ext_empty_o;

using ip_hdr_info_item_ext_ies_container = protocol_ext_container_empty_l;

// IPHeaderInformation ::= SEQUENCE
struct ip_hdr_info_s {
  bool                               ds_info_list_present     = false;
  bool                               ip_v6_flow_label_present = false;
  bool                               ie_exts_present          = false;
  iab_tnl_address_c                  dest_iab_tnl_address;
  d_si_nformation_list_l             ds_info_list;
  fixed_bitstring<20, false, true>   ip_v6_flow_label;
  ip_hdr_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IPtolayer2TrafficMappingInfo-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ip_tolayer2_traffic_map_info_item_ext_ies_o = protocol_ext_empty_o;

using ip_tolayer2_traffic_map_info_item_ext_ies_container = protocol_ext_container_empty_l;

// IPtolayer2TrafficMappingInfo-Item ::= SEQUENCE
struct ip_tolayer2_traffic_map_info_item_s {
  bool                                                ie_exts_present = false;
  fixed_bitstring<26, false, true>                    map_info_idx;
  ip_hdr_info_s                                       ip_hdr_info;
  bh_info_s                                           bh_info;
  ip_tolayer2_traffic_map_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IPtolayer2TrafficMappingInfoList ::= SEQUENCE (SIZE (1..67108864)) OF IPtolayer2TrafficMappingInfo-Item
using ip_tolayer2_traffic_map_info_list_l = dyn_array<ip_tolayer2_traffic_map_info_item_s>;

// IPtolayer2TrafficMappingInfo ::= SEQUENCE
struct ip_tolayer2_traffic_map_info_s {
  bool                                                ie_exts_present = false;
  ip_tolayer2_traffic_map_info_list_l                 ip_tolayer2_traffic_map_info_to_add;
  map_infoto_rem_l                                    ip_tolayer2_traffic_map_info_to_rem;
  ip_tolayer2_traffic_map_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficMappingInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using traffic_map_info_ext_ies_o = protocol_ies_empty_o;

// TrafficMappingInfo ::= CHOICE
struct traffic_map_info_c {
  struct types_opts {
    enum options { ip_tolayer2_traffic_map_info, ba_player_bh_rlc_ch_map_info, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  traffic_map_info_c() = default;
  traffic_map_info_c(const traffic_map_info_c& other);
  traffic_map_info_c& operator=(const traffic_map_info_c& other);
  ~traffic_map_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ip_tolayer2_traffic_map_info_s& ip_tolayer2_traffic_map_info()
  {
    assert_choice_type(types::ip_tolayer2_traffic_map_info, type_, "TrafficMappingInfo");
    return c.get<ip_tolayer2_traffic_map_info_s>();
  }
  ba_player_bh_rlc_ch_map_info_s& ba_player_bh_rlc_ch_map_info()
  {
    assert_choice_type(types::ba_player_bh_rlc_ch_map_info, type_, "TrafficMappingInfo");
    return c.get<ba_player_bh_rlc_ch_map_info_s>();
  }
  protocol_ie_single_container_s<traffic_map_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TrafficMappingInfo");
    return c.get<protocol_ie_single_container_s<traffic_map_info_ext_ies_o>>();
  }
  const ip_tolayer2_traffic_map_info_s& ip_tolayer2_traffic_map_info() const
  {
    assert_choice_type(types::ip_tolayer2_traffic_map_info, type_, "TrafficMappingInfo");
    return c.get<ip_tolayer2_traffic_map_info_s>();
  }
  const ba_player_bh_rlc_ch_map_info_s& ba_player_bh_rlc_ch_map_info() const
  {
    assert_choice_type(types::ba_player_bh_rlc_ch_map_info, type_, "TrafficMappingInfo");
    return c.get<ba_player_bh_rlc_ch_map_info_s>();
  }
  const protocol_ie_single_container_s<traffic_map_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TrafficMappingInfo");
    return c.get<protocol_ie_single_container_s<traffic_map_info_ext_ies_o>>();
  }
  ip_tolayer2_traffic_map_info_s&                             set_ip_tolayer2_traffic_map_info();
  ba_player_bh_rlc_ch_map_info_s&                             set_ba_player_bh_rlc_ch_map_info();
  protocol_ie_single_container_s<traffic_map_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<ba_player_bh_rlc_ch_map_info_s,
                  ip_tolayer2_traffic_map_info_s,
                  protocol_ie_single_container_s<traffic_map_info_ext_ies_o>>
      c;

  void destroy_();
};

// BHChannels-ToBeModified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-ToBeModified-Item ::= SEQUENCE
struct bh_chs_to_be_modified_item_s {
  bool                                         rlc_mode_present         = false;
  bool                                         bap_ctrl_pdu_ch_present  = false;
  bool                                         traffic_map_info_present = false;
  bool                                         ie_exts_present          = false;
  fixed_bitstring<16, false, true>             bh_rlc_ch_id;
  bh_qos_info_c                                bh_qos_info;
  rlc_mode_e                                   rlc_mode;
  bap_ctrl_pdu_ch_e                            bap_ctrl_pdu_ch;
  traffic_map_info_c                           traffic_map_info;
  bh_chs_to_be_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-ToBeReleased-Item ::= SEQUENCE
struct bh_chs_to_be_released_item_s {
  bool                                         ie_exts_present = false;
  fixed_bitstring<16, false, true>             bh_rlc_ch_id;
  bh_chs_to_be_released_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-ToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-ToBeSetup-Item ::= SEQUENCE
struct bh_chs_to_be_setup_item_s {
  bool                                      bap_ctrl_pdu_ch_present  = false;
  bool                                      traffic_map_info_present = false;
  bool                                      ie_exts_present          = false;
  fixed_bitstring<16, false, true>          bh_rlc_ch_id;
  bh_qos_info_c                             bh_qos_info;
  rlc_mode_e                                rlc_mode;
  bap_ctrl_pdu_ch_e                         bap_ctrl_pdu_ch;
  traffic_map_info_c                        traffic_map_info;
  bh_chs_to_be_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHChannels-ToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_chs_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using bh_chs_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// BHChannels-ToBeSetupMod-Item ::= SEQUENCE
struct bh_chs_to_be_setup_mod_item_s {
  bool                                          bap_ctrl_pdu_ch_present  = false;
  bool                                          traffic_map_info_present = false;
  bool                                          ie_exts_present          = false;
  fixed_bitstring<16, false, true>              bh_rlc_ch_id;
  bh_qos_info_c                                 bh_qos_info;
  rlc_mode_e                                    rlc_mode;
  bap_ctrl_pdu_ch_e                             bap_ctrl_pdu_ch;
  traffic_map_info_c                            traffic_map_info;
  bh_chs_to_be_setup_mod_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHRLCCHItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using bh_rlc_ch_item_ext_ies_o = protocol_ext_empty_o;

using bh_rlc_ch_item_ext_ies_container = protocol_ext_container_empty_l;

// BHRLCCHItem ::= SEQUENCE
struct bh_rlc_ch_item_s {
  bool                             ie_exts_present = false;
  fixed_bitstring<16, false, true> bh_rlc_ch_id;
  bh_rlc_ch_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHRLCCHList ::= SEQUENCE (SIZE (1..65536)) OF BHRLCCHItem
using bh_rlc_ch_list_l = dyn_array<bh_rlc_ch_item_s>;

// ExtendedAvailablePLMN-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using extended_available_plmn_item_ext_ies_o = protocol_ext_empty_o;

using extended_available_plmn_item_ext_ies_container = protocol_ext_container_empty_l;

// ExtendedAvailablePLMN-Item ::= SEQUENCE
struct extended_available_plmn_item_s {
  bool                                           ie_exts_present = false;
  fixed_octstring<3, true>                       plmn_id;
  extended_available_plmn_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExtendedAvailablePLMN-List ::= SEQUENCE (SIZE (1..6)) OF ExtendedAvailablePLMN-Item
using extended_available_plmn_list_l = dyn_array<extended_available_plmn_item_s>;

// ConfiguredTACIndication ::= ENUMERATED
struct cfg_tac_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cfg_tac_ind_e = enumerated<cfg_tac_ind_opts, true>;

// BroadcastSNPN-ID-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_sn_pn_id_list_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_sn_pn_id_list_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastSNPN-ID-List-Item ::= SEQUENCE
struct broadcast_sn_pn_id_list_item_s {
  bool                                           ie_exts_present = false;
  fixed_octstring<3, true>                       plmn_id;
  broadcast_n_id_list_l                          broadcast_n_id_list;
  broadcast_sn_pn_id_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastSNPN-ID-List ::= SEQUENCE (SIZE (1..12)) OF BroadcastSNPN-ID-List-Item
using broadcast_sn_pn_id_list_l = dyn_array<broadcast_sn_pn_id_list_item_s>;

// NPN-Broadcast-Information-SNPN-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using npn_broadcast_info_sn_pn_ext_ies_o = protocol_ext_empty_o;

using npn_broadcast_info_sn_pn_ext_ies_container = protocol_ext_container_empty_l;

// NPN-Broadcast-Information-SNPN ::= SEQUENCE
struct npn_broadcast_info_sn_pn_s {
  bool                                       ie_ext_present = false;
  broadcast_sn_pn_id_list_l                  broadcast_sn_pn_id_list;
  npn_broadcast_info_sn_pn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastCAGList ::= SEQUENCE (SIZE (1..12)) OF BIT STRING (SIZE (32))
using broadcast_cag_list_l = bounded_array<fixed_bitstring<32, false, true>, 12>;

// BroadcastPNI-NPN-ID-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_pni_npn_id_list_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_pni_npn_id_list_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastPNI-NPN-ID-List-Item ::= SEQUENCE
struct broadcast_pni_npn_id_list_item_s {
  bool                                             ie_exts_present = false;
  fixed_octstring<3, true>                         plmn_id;
  broadcast_cag_list_l                             broadcast_cag_list;
  broadcast_pni_npn_id_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastPNI-NPN-ID-List ::= SEQUENCE (SIZE (1..12)) OF BroadcastPNI-NPN-ID-List-Item
using broadcast_pni_npn_id_list_l = dyn_array<broadcast_pni_npn_id_list_item_s>;

// NPN-Broadcast-Information-PNI-NPN-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using npn_broadcast_info_pni_npn_ext_ies_o = protocol_ext_empty_o;

using npn_broadcast_info_pni_npn_ext_ies_container = protocol_ext_container_empty_l;

// NPN-Broadcast-Information-PNI-NPN ::= SEQUENCE
struct npn_broadcast_info_pni_npn_s {
  bool                                         ie_ext_present = false;
  broadcast_pni_npn_id_list_l                  broadcast_pni_npn_id_info;
  npn_broadcast_info_pni_npn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPNBroadcastInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using npn_broadcast_info_ext_ies_o = protocol_ies_empty_o;

// NPNBroadcastInformation ::= CHOICE
struct npn_broadcast_info_c {
  struct types_opts {
    enum options { sn_pn_broadcast_info, pni_npn_broadcast_info, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  npn_broadcast_info_c() = default;
  npn_broadcast_info_c(const npn_broadcast_info_c& other);
  npn_broadcast_info_c& operator=(const npn_broadcast_info_c& other);
  ~npn_broadcast_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  npn_broadcast_info_sn_pn_s& sn_pn_broadcast_info()
  {
    assert_choice_type(types::sn_pn_broadcast_info, type_, "NPNBroadcastInformation");
    return c.get<npn_broadcast_info_sn_pn_s>();
  }
  npn_broadcast_info_pni_npn_s& pni_npn_broadcast_info()
  {
    assert_choice_type(types::pni_npn_broadcast_info, type_, "NPNBroadcastInformation");
    return c.get<npn_broadcast_info_pni_npn_s>();
  }
  protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NPNBroadcastInformation");
    return c.get<protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>>();
  }
  const npn_broadcast_info_sn_pn_s& sn_pn_broadcast_info() const
  {
    assert_choice_type(types::sn_pn_broadcast_info, type_, "NPNBroadcastInformation");
    return c.get<npn_broadcast_info_sn_pn_s>();
  }
  const npn_broadcast_info_pni_npn_s& pni_npn_broadcast_info() const
  {
    assert_choice_type(types::pni_npn_broadcast_info, type_, "NPNBroadcastInformation");
    return c.get<npn_broadcast_info_pni_npn_s>();
  }
  const protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NPNBroadcastInformation");
    return c.get<protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>>();
  }
  npn_broadcast_info_sn_pn_s&                                   set_sn_pn_broadcast_info();
  npn_broadcast_info_pni_npn_s&                                 set_pni_npn_broadcast_info();
  protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<npn_broadcast_info_pni_npn_s,
                  npn_broadcast_info_sn_pn_s,
                  protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>>
      c;

  void destroy_();
};

// BPLMN-ID-Info-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct bplmn_id_info_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct bplmn_id_info_item_ext_ies_container {
  bool                 cfg_tac_ind_present        = false;
  bool                 npn_broadcast_info_present = false;
  cfg_tac_ind_e        cfg_tac_ind;
  npn_broadcast_info_c npn_broadcast_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BPLMN-ID-Info-Item ::= SEQUENCE
struct bplmn_id_info_item_s {
  bool                                 five_gs_tac_present = false;
  bool                                 ranac_present       = false;
  bool                                 ie_exts_present     = false;
  available_plmn_list_l                plmn_id_list;
  extended_available_plmn_list_l       extended_plmn_id_list;
  fixed_octstring<3, true>             five_gs_tac;
  fixed_bitstring<36, false, true>     nr_cell_id;
  uint16_t                             ranac = 0;
  bplmn_id_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BPLMN-ID-Info-List ::= SEQUENCE (SIZE (1..12)) OF BPLMN-ID-Info-Item
using bplmn_id_info_list_l = dyn_array<bplmn_id_info_item_s>;

// BW-Aggregation-Request-Indication ::= ENUMERATED
struct bw_aggregation_request_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using bw_aggregation_request_ind_e = enumerated<bw_aggregation_request_ind_opts, true>;

// FR1-Bandwidth ::= ENUMERATED
struct fr1_bw_opts {
  enum options {
    bw5,
    bw10,
    bw20,
    bw40,
    bw50,
    bw80,
    bw100,
    // ...
    bw160,
    bw200,
    bw15,
    bw25,
    bw30,
    bw60,
    bw35,
    bw45,
    bw70,
    bw90,
    nulltype
  } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using fr1_bw_e = enumerated<fr1_bw_opts, true, 10>;

// FR2-Bandwidth ::= ENUMERATED
struct fr2_bw_opts {
  enum options { bw50, bw100, bw200, bw400, /*...*/ bw800, bw1600, bw2000, bw600, nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using fr2_bw_e = enumerated<fr2_bw_opts, true, 4>;

// BandwidthSRS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using bw_srs_ext_ies_o = protocol_ies_empty_o;

// BandwidthSRS ::= CHOICE
struct bw_srs_c {
  struct types_opts {
    enum options { fr1, fr2, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  bw_srs_c() = default;
  bw_srs_c(const bw_srs_c& other);
  bw_srs_c& operator=(const bw_srs_c& other);
  ~bw_srs_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fr1_bw_e& fr1()
  {
    assert_choice_type(types::fr1, type_, "BandwidthSRS");
    return c.get<fr1_bw_e>();
  }
  fr2_bw_e& fr2()
  {
    assert_choice_type(types::fr2, type_, "BandwidthSRS");
    return c.get<fr2_bw_e>();
  }
  protocol_ie_single_container_s<bw_srs_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "BandwidthSRS");
    return c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>();
  }
  const fr1_bw_e& fr1() const
  {
    assert_choice_type(types::fr1, type_, "BandwidthSRS");
    return c.get<fr1_bw_e>();
  }
  const fr2_bw_e& fr2() const
  {
    assert_choice_type(types::fr2, type_, "BandwidthSRS");
    return c.get<fr2_bw_e>();
  }
  const protocol_ie_single_container_s<bw_srs_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "BandwidthSRS");
    return c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>();
  }
  fr1_bw_e&                                         set_fr1();
  fr2_bw_e&                                         set_fr2();
  protocol_ie_single_container_s<bw_srs_ext_ies_o>& set_choice_ext();

private:
  types                                                             type_;
  choice_buffer_t<protocol_ie_single_container_s<bw_srs_ext_ies_o>> c;

  void destroy_();
};

// BarringExemptionforEmerCallInfo ::= ENUMERATED
struct barr_exemptionfor_emer_call_info_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using barr_exemptionfor_emer_call_info_e = enumerated<barr_exemptionfor_emer_call_info_opts, true>;

// BearerTypeChange ::= ENUMERATED
struct bearer_type_change_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using bearer_type_change_e = enumerated<bearer_type_change_opts, true>;

// Broadcast-Cell-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_cell_list_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_cell_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Broadcast-Cell-List-Item ::= SEQUENCE
struct broadcast_cell_list_item_s {
  bool                                       ie_exts_present = false;
  nr_cgi_s                                   cell_id;
  broadcast_cell_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Broadcast-MRBs-Transport-Request-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_transport_request_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_transport_request_item_ext_ies_container = protocol_ext_container_empty_l;

// Broadcast-MRBs-Transport-Request-Item ::= SEQUENCE
struct broadcast_m_rbs_transport_request_item_s {
  bool                                                     ie_exts_present = false;
  uint16_t                                                 mrb_id          = 1;
  bc_bearer_context_f1_u_tnl_info_c                        bc_bearer_ctxt_f1_u_tnl_infoat_du;
  broadcast_m_rbs_transport_request_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Broadcast-To-Be-Cancelled-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_to_be_cancelled_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_to_be_cancelled_item_ext_ies_container = protocol_ext_container_empty_l;

// Broadcast-To-Be-Cancelled-Item ::= SEQUENCE
struct broadcast_to_be_cancelled_item_s {
  bool                                             ie_exts_present = false;
  nr_cgi_s                                         nr_cgi;
  broadcast_to_be_cancelled_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastCellList ::= SEQUENCE (SIZE (1..512)) OF Broadcast-Cell-List-Item
using broadcast_cell_list_l = dyn_array<broadcast_cell_list_item_s>;

// PartialSuccessCell-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using partial_success_cell_ext_ies_o = protocol_ext_empty_o;

using partial_success_cell_ext_ies_container = protocol_ext_container_empty_l;

// PartialSuccessCell ::= SEQUENCE
struct partial_success_cell_s {
  bool                                   ie_exts_present = false;
  broadcast_cell_list_l                  broadcast_cell_list;
  partial_success_cell_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastAreaScope-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using broadcast_area_scope_ext_ies_o = protocol_ies_empty_o;

// BroadcastAreaScope ::= CHOICE
struct broadcast_area_scope_c {
  struct types_opts {
    enum options { complete_success, partial_success, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  broadcast_area_scope_c() = default;
  broadcast_area_scope_c(const broadcast_area_scope_c& other);
  broadcast_area_scope_c& operator=(const broadcast_area_scope_c& other);
  ~broadcast_area_scope_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  partial_success_cell_s& partial_success()
  {
    assert_choice_type(types::partial_success, type_, "BroadcastAreaScope");
    return c.get<partial_success_cell_s>();
  }
  protocol_ie_single_container_s<broadcast_area_scope_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "BroadcastAreaScope");
    return c.get<protocol_ie_single_container_s<broadcast_area_scope_ext_ies_o>>();
  }
  const partial_success_cell_s& partial_success() const
  {
    assert_choice_type(types::partial_success, type_, "BroadcastAreaScope");
    return c.get<partial_success_cell_s>();
  }
  const protocol_ie_single_container_s<broadcast_area_scope_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "BroadcastAreaScope");
    return c.get<protocol_ie_single_container_s<broadcast_area_scope_ext_ies_o>>();
  }
  void                                                            set_complete_success();
  partial_success_cell_s&                                         set_partial_success();
  protocol_ie_single_container_s<broadcast_area_scope_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                   type_;
  choice_buffer_t<partial_success_cell_s, protocol_ie_single_container_s<broadcast_area_scope_ext_ies_o>> c;

  void destroy_();
};

// BroadcastMRBs-FailedtoBeModified-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_failedto_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_failedto_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-FailedToBeModified-Item ::= SEQUENCE
struct broadcast_m_rbs_failed_to_be_modified_item_s {
  bool                                                        cause_present   = false;
  bool                                                        ie_exts_present = false;
  uint16_t                                                    mrb_id          = 1;
  cause_c                                                     cause;
  broadcast_m_rbs_failedto_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastMRBs-FailedToBeSetup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-FailedToBeSetup-Item ::= SEQUENCE
struct broadcast_m_rbs_failed_to_be_setup_item_s {
  bool                                                      cause_present   = false;
  bool                                                      ie_exts_present = false;
  uint16_t                                                  mrb_id          = 1;
  cause_c                                                   cause;
  broadcast_m_rbs_failed_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastMRBs-FailedToBeSetupMod-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_failed_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_failed_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-FailedToBeSetupMod-Item ::= SEQUENCE
struct broadcast_m_rbs_failed_to_be_setup_mod_item_s {
  bool                                                          cause_present   = false;
  bool                                                          ie_exts_present = false;
  uint16_t                                                      mrb_id          = 1;
  cause_c                                                       cause;
  broadcast_m_rbs_failed_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastMRBs-Modified-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_modified_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-Modified-Item ::= SEQUENCE
struct broadcast_m_rbs_modified_item_s {
  bool                                            bc_bearer_ctxt_f1_u_tnl_infoat_du_present = false;
  bool                                            ie_exts_present                           = false;
  uint16_t                                        mrb_id                                    = 1;
  bc_bearer_context_f1_u_tnl_info_c               bc_bearer_ctxt_f1_u_tnl_infoat_du;
  broadcast_m_rbs_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastMRBs-Setup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_setup_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-Setup-Item ::= SEQUENCE
struct broadcast_m_rbs_setup_item_s {
  bool                                         ie_exts_present = false;
  uint16_t                                     mrb_id          = 1;
  bc_bearer_context_f1_u_tnl_info_c            bc_bearer_ctxt_f1_u_tnl_infoat_du;
  broadcast_m_rbs_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastMRBs-SetupMod-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-SetupMod-Item ::= SEQUENCE
struct broadcast_m_rbs_setup_mod_item_s {
  bool                                             ie_exts_present = false;
  uint16_t                                         mrb_id          = 1;
  bc_bearer_context_f1_u_tnl_info_c                bc_bearer_ctxt_f1_u_tnl_infoat_du;
  broadcast_m_rbs_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Flows-Mapped-To-MRB-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_flows_mapped_to_mrb_item_ext_ies_o = protocol_ext_empty_o;

using mbs_flows_mapped_to_mrb_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-Flows-Mapped-To-MRB-Item ::= SEQUENCE
struct mbs_flows_mapped_to_mrb_item_s {
  bool                                           ie_exts_present = false;
  uint8_t                                        mbs_qos_flow_id = 0;
  qos_flow_level_qos_params_s                    mbs_qos_flow_level_qos_params;
  mbs_flows_mapped_to_mrb_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Flows-Mapped-To-MRB-List ::= SEQUENCE (SIZE (1..64)) OF MBS-Flows-Mapped-To-MRB-Item
using mbs_flows_mapped_to_mrb_list_l = dyn_array<mbs_flows_mapped_to_mrb_item_s>;

// BroadcastMRBs-ToBeModified-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-ToBeModified-Item ::= SEQUENCE
struct broadcast_m_rbs_to_be_modified_item_s {
  bool                                                  mrb_qos_info_present                      = false;
  bool                                                  bc_bearer_ctxt_f1_u_tnl_infoat_cu_present = false;
  bool                                                  ie_exts_present                           = false;
  uint16_t                                              mrb_id                                    = 1;
  qos_flow_level_qos_params_s                           mrb_qos_info;
  mbs_flows_mapped_to_mrb_list_l                        mbs_flows_mapped_to_mrb_list;
  bc_bearer_context_f1_u_tnl_info_c                     bc_bearer_ctxt_f1_u_tnl_infoat_cu;
  broadcast_m_rbs_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastMRBs-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-ToBeReleased-Item ::= SEQUENCE
struct broadcast_m_rbs_to_be_released_item_s {
  bool                                                  ie_exts_present = false;
  uint16_t                                              mrb_id          = 1;
  broadcast_m_rbs_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastMRBs-ToBeSetup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-ToBeSetup-Item ::= SEQUENCE
struct broadcast_m_rbs_to_be_setup_item_s {
  bool                                               ie_exts_present = false;
  uint16_t                                           mrb_id          = 1;
  qos_flow_level_qos_params_s                        mrb_qos_info;
  mbs_flows_mapped_to_mrb_list_l                     mbs_flows_mapped_to_mrb_list;
  bc_bearer_context_f1_u_tnl_info_c                  bc_bearer_ctxt_f1_u_tnl_infoat_cu;
  broadcast_m_rbs_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastMRBs-ToBeSetupMod-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using broadcast_m_rbs_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_m_rbs_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastMRBs-ToBeSetupMod-Item ::= SEQUENCE
struct broadcast_m_rbs_to_be_setup_mod_item_s {
  bool                                                   ie_exts_present = false;
  uint16_t                                               mrb_id          = 1;
  qos_flow_level_qos_params_s                            mrb_qos_info;
  mbs_flows_mapped_to_mrb_list_l                         mbs_flows_mapped_to_mrb_list;
  bc_bearer_context_f1_u_tnl_info_c                      bc_bearer_ctxt_f1_u_tnl_infoat_cu;
  broadcast_m_rbs_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BurstArrivalTimeWindow-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using burst_arrival_time_win_ext_ies_o = protocol_ext_empty_o;

using burst_arrival_time_win_ext_ies_container = protocol_ext_container_empty_l;

// BurstArrivalTimeWindow ::= SEQUENCE
struct burst_arrival_time_win_s {
  bool                                     ie_ext_present               = false;
  uint32_t                                 burst_arrival_time_win_start = 0;
  uint32_t                                 burst_arrival_time_win_end   = 0;
  burst_arrival_time_win_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CCO-issue-detection ::= ENUMERATED
struct cco_issue_detection_opts {
  enum options { coverage, cell_edge_capacity, /*...*/ network_energy_saving, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using cco_issue_detection_e = enumerated<cco_issue_detection_opts, true, 1>;

// CCO-Assistance-Information-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cco_assist_info_ext_ies_o = protocol_ext_empty_o;

using cco_assist_info_ext_ies_container = protocol_ext_container_empty_l;

// CCO-Assistance-Information ::= SEQUENCE
struct cco_assist_info_s {
  bool                              cco_issue_detection_present = false;
  bool                              ie_exts_present             = false;
  cco_issue_detection_e             cco_issue_detection;
  affected_cells_and_beams_list_l   affected_cells_and_beams_list;
  cco_assist_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CG-SDTKeptIndicator ::= ENUMERATED
struct cg_sdt_kept_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cg_sdt_kept_ind_e = enumerated<cg_sdt_kept_ind_opts, true>;

// CG-SDTQueryIndication ::= ENUMERATED
struct cg_sdt_query_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cg_sdt_query_ind_e = enumerated<cg_sdt_query_ind_opts, true>;

// CG-SDTSessionInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cg_sdt_session_info_ext_ies_o = protocol_ext_empty_o;

using cg_sdt_session_info_ext_ies_container = protocol_ext_container_empty_l;

// CG-SDTSessionInfo ::= SEQUENCE
struct cg_sdt_session_info_s {
  bool                                  ie_exts_present   = false;
  uint64_t                              gnb_cu_ue_f1ap_id = 0;
  uint64_t                              gnb_du_ue_f1ap_id = 0;
  cg_sdt_session_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CG-SDTindicatorMod ::= ENUMERATED
struct cg_sd_tind_mod_opts {
  enum options { true_value, false_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cg_sd_tind_mod_e = enumerated<cg_sd_tind_mod_opts, true>;

// CG-SDTindicatorSetup ::= ENUMERATED
struct cg_sd_tind_setup_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cg_sd_tind_setup_e = enumerated<cg_sd_tind_setup_opts, true>;

// CHOtrigger-InterDU ::= ENUMERATED
struct ch_otrigger_inter_du_opts {
  enum options { cho_initiation, cho_replace, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ch_otrigger_inter_du_e = enumerated<ch_otrigger_inter_du_opts, true>;

// CHOtrigger-IntraDU ::= ENUMERATED
struct ch_otrigger_intra_du_opts {
  enum options { cho_initiation, cho_replace, cho_cancel, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ch_otrigger_intra_du_e = enumerated<ch_otrigger_intra_du_opts, true>;

// CNUEPagingIdentity-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using cn_ue_paging_id_ext_ies_o = protocol_ies_empty_o;

// CNUEPagingIdentity ::= CHOICE
struct cn_ue_paging_id_c {
  struct types_opts {
    enum options { five_g_s_tmsi, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cn_ue_paging_id_c() = default;
  cn_ue_paging_id_c(const cn_ue_paging_id_c& other);
  cn_ue_paging_id_c& operator=(const cn_ue_paging_id_c& other);
  ~cn_ue_paging_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<48, false, true>& five_g_s_tmsi()
  {
    assert_choice_type(types::five_g_s_tmsi, type_, "CNUEPagingIdentity");
    return c.get<fixed_bitstring<48, false, true>>();
  }
  protocol_ie_single_container_s<cn_ue_paging_id_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "CNUEPagingIdentity");
    return c.get<protocol_ie_single_container_s<cn_ue_paging_id_ext_ies_o>>();
  }
  const fixed_bitstring<48, false, true>& five_g_s_tmsi() const
  {
    assert_choice_type(types::five_g_s_tmsi, type_, "CNUEPagingIdentity");
    return c.get<fixed_bitstring<48, false, true>>();
  }
  const protocol_ie_single_container_s<cn_ue_paging_id_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "CNUEPagingIdentity");
    return c.get<protocol_ie_single_container_s<cn_ue_paging_id_ext_ies_o>>();
  }
  fixed_bitstring<48, false, true>&                          set_five_g_s_tmsi();
  protocol_ie_single_container_s<cn_ue_paging_id_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                        type_;
  choice_buffer_t<fixed_bitstring<48, false, true>, protocol_ie_single_container_s<cn_ue_paging_id_ext_ies_o>> c;

  void destroy_();
};

// Endpoint-IP-address-and-port-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct endpoint_ip_address_and_port_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct endpoint_ip_address_and_port_ext_ies_container {
  bool                             port_num_present = false;
  fixed_bitstring<16, false, true> port_num;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Endpoint-IP-address-and-port ::= SEQUENCE
struct endpoint_ip_address_and_port_s {
  bool                                           ie_exts_present = false;
  bounded_bitstring<1, 160, true, true>          endpoint_ip_address;
  endpoint_ip_address_and_port_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CP-TransportLayerAddress-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using cp_transport_layer_address_ext_ies_o = protocol_ies_empty_o;

// CP-TransportLayerAddress ::= CHOICE
struct cp_transport_layer_address_c {
  struct types_opts {
    enum options { endpoint_ip_address, endpoint_ip_address_and_port, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cp_transport_layer_address_c() = default;
  cp_transport_layer_address_c(const cp_transport_layer_address_c& other);
  cp_transport_layer_address_c& operator=(const cp_transport_layer_address_c& other);
  ~cp_transport_layer_address_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  bounded_bitstring<1, 160, true, true>& endpoint_ip_address()
  {
    assert_choice_type(types::endpoint_ip_address, type_, "CP-TransportLayerAddress");
    return c.get<bounded_bitstring<1, 160, true, true>>();
  }
  endpoint_ip_address_and_port_s& endpoint_ip_address_and_port()
  {
    assert_choice_type(types::endpoint_ip_address_and_port, type_, "CP-TransportLayerAddress");
    return c.get<endpoint_ip_address_and_port_s>();
  }
  protocol_ie_single_container_s<cp_transport_layer_address_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "CP-TransportLayerAddress");
    return c.get<protocol_ie_single_container_s<cp_transport_layer_address_ext_ies_o>>();
  }
  const bounded_bitstring<1, 160, true, true>& endpoint_ip_address() const
  {
    assert_choice_type(types::endpoint_ip_address, type_, "CP-TransportLayerAddress");
    return c.get<bounded_bitstring<1, 160, true, true>>();
  }
  const endpoint_ip_address_and_port_s& endpoint_ip_address_and_port() const
  {
    assert_choice_type(types::endpoint_ip_address_and_port, type_, "CP-TransportLayerAddress");
    return c.get<endpoint_ip_address_and_port_s>();
  }
  const protocol_ie_single_container_s<cp_transport_layer_address_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "CP-TransportLayerAddress");
    return c.get<protocol_ie_single_container_s<cp_transport_layer_address_ext_ies_o>>();
  }
  bounded_bitstring<1, 160, true, true>&                                set_endpoint_ip_address();
  endpoint_ip_address_and_port_s&                                       set_endpoint_ip_address_and_port();
  protocol_ie_single_container_s<cp_transport_layer_address_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<bounded_bitstring<1, 160, true, true>,
                  endpoint_ip_address_and_port_s,
                  protocol_ie_single_container_s<cp_transport_layer_address_ext_ies_o>>
      c;

  void destroy_();
};

// CPAC-trigger ::= ENUMERATED
struct cp_ac_trigger_opts {
  enum options { cpac_prep, cpac_executed, /*...*/ cpac_cancel, nulltype } value;

  const char* to_string() const;
};
using cp_ac_trigger_e = enumerated<cp_ac_trigger_opts, true, 1>;

// PSCellList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pscell_list_item_ext_ies_o = protocol_ext_empty_o;

using pscell_list_item_ext_ies_container = protocol_ext_container_empty_l;

// PSCellList-Item ::= SEQUENCE
struct pscell_list_item_s {
  bool                               ie_exts_present = false;
  nr_cgi_s                           pscell;
  pscell_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PSCellList ::= SEQUENCE (SIZE (1..8)) OF PSCellList-Item
using pscell_list_l = dyn_array<pscell_list_item_s>;

// CPACMCGInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct cp_acmcg_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cp_acmcg_info_ext_ies_container {
  bool          candidate_p_scells_to_cancel_present = false;
  pscell_list_l candidate_p_scells_to_cancel;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPACMCGInformation ::= SEQUENCE
struct cp_acmcg_info_s {
  bool                            ie_exts_present = false;
  cp_ac_trigger_e                 cpac_trigger;
  nr_cgi_s                        pscellid;
  cp_acmcg_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CSIResourceConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using csi_res_cfg_ext_ies_o = protocol_ext_empty_o;

using csi_res_cfg_ext_ies_container = protocol_ext_container_empty_l;

// CSIResourceConfiguration ::= SEQUENCE
struct csi_res_cfg_s {
  bool                          ie_exts_present = false;
  unbounded_octstring<true>     csi_res_cfg_to_add_mod_list;
  unbounded_octstring<true>     csi_res_cfg_to_release_list;
  csi_res_cfg_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RIMRSDetectionStatus ::= ENUMERATED
struct rimrs_detection_status_opts {
  enum options { rs_detected, rs_disappeared, /*...*/ nulltype } value;

  const char* to_string() const;
};
using rimrs_detection_status_e = enumerated<rimrs_detection_status_opts, true>;

// CUDURIMInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cu_du_rim_info_ext_ies_o = protocol_ext_empty_o;

using cu_du_rim_info_ext_ies_container = protocol_ext_container_empty_l;

// CUDURIMInformation ::= SEQUENCE
struct cu_du_rim_info_s {
  bool                             ie_exts_present = false;
  fixed_bitstring<22, false, true> victim_gnb_set_id;
  rimrs_detection_status_e         rimrs_detection_status;
  cu_du_rim_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CUDURadioInformationType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using cu_du_radio_info_type_ext_ies_o = protocol_ies_empty_o;

// CUDURadioInformationType ::= CHOICE
struct cu_du_radio_info_type_c {
  struct types_opts {
    enum options { rim, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cu_du_radio_info_type_c() = default;
  cu_du_radio_info_type_c(const cu_du_radio_info_type_c& other);
  cu_du_radio_info_type_c& operator=(const cu_du_radio_info_type_c& other);
  ~cu_du_radio_info_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  cu_du_rim_info_s& rim()
  {
    assert_choice_type(types::rim, type_, "CUDURadioInformationType");
    return c.get<cu_du_rim_info_s>();
  }
  protocol_ie_single_container_s<cu_du_radio_info_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "CUDURadioInformationType");
    return c.get<protocol_ie_single_container_s<cu_du_radio_info_type_ext_ies_o>>();
  }
  const cu_du_rim_info_s& rim() const
  {
    assert_choice_type(types::rim, type_, "CUDURadioInformationType");
    return c.get<cu_du_rim_info_s>();
  }
  const protocol_ie_single_container_s<cu_du_radio_info_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "CUDURadioInformationType");
    return c.get<protocol_ie_single_container_s<cu_du_radio_info_type_ext_ies_o>>();
  }
  cu_du_rim_info_s&                                                set_rim();
  protocol_ie_single_container_s<cu_du_radio_info_type_ext_ies_o>& set_choice_ext();

private:
  types                                                                                              type_;
  choice_buffer_t<cu_du_rim_info_s, protocol_ie_single_container_s<cu_du_radio_info_type_ext_ies_o>> c;

  void destroy_();
};

// Preconfigured-measurement-GAP-Request ::= ENUMERATED
struct precfg_meas_gap_request_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using precfg_meas_gap_request_e = enumerated<precfg_meas_gap_request_opts, true>;

// MusimCapabilityRestrictionIndication ::= ENUMERATED
struct musim_cap_restrict_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using musim_cap_restrict_ind_e = enumerated<musim_cap_restrict_ind_opts, true>;

// CUtoDURRCInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct cu_to_du_rrc_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cu_to_du_rrc_info_ext_ies_container {
  bool                      ho_prep_info_present                  = false;
  bool                      cell_group_cfg_present                = false;
  bool                      meas_timing_cfg_present               = false;
  bool                      ue_assist_info_present                = false;
  bool                      cg_cfg_present                        = false;
  bool                      ue_assist_info_eutra_present          = false;
  bool                      location_meas_info_present            = false;
  bool                      mu_si_m_gap_cfg_present               = false;
  bool                      sdt_mac_phy_cg_cfg_present            = false;
  bool                      mb_si_nterest_ind_present             = false;
  bool                      need_for_gaps_info_nr_present         = false;
  bool                      need_for_gap_ncsg_info_nr_present     = false;
  bool                      need_for_gap_ncsg_info_eutra_present  = false;
  bool                      cfg_restrict_info_daps_present        = false;
  bool                      precfg_meas_gap_request_present       = false;
  bool                      need_for_interruption_info_nr_present = false;
  bool                      musim_cap_restrict_ind_present        = false;
  bool                      musim_candidate_band_list_present     = false;
  unbounded_octstring<true> ho_prep_info;
  unbounded_octstring<true> cell_group_cfg;
  unbounded_octstring<true> meas_timing_cfg;
  unbounded_octstring<true> ue_assist_info;
  unbounded_octstring<true> cg_cfg;
  unbounded_octstring<true> ue_assist_info_eutra;
  unbounded_octstring<true> location_meas_info;
  unbounded_octstring<true> mu_si_m_gap_cfg;
  unbounded_octstring<true> sdt_mac_phy_cg_cfg;
  unbounded_octstring<true> mb_si_nterest_ind;
  unbounded_octstring<true> need_for_gaps_info_nr;
  unbounded_octstring<true> need_for_gap_ncsg_info_nr;
  unbounded_octstring<true> need_for_gap_ncsg_info_eutra;
  unbounded_octstring<true> cfg_restrict_info_daps;
  precfg_meas_gap_request_e precfg_meas_gap_request;
  unbounded_octstring<true> need_for_interruption_info_nr;
  musim_cap_restrict_ind_e  musim_cap_restrict_ind;
  unbounded_octstring<true> musim_candidate_band_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CUtoDURRCInformation ::= SEQUENCE
struct cu_to_du_rrc_info_s {
  bool                                ie_exts_present = false;
  unbounded_octstring<true>           cg_cfg_info;
  unbounded_octstring<true>           ue_cap_rat_container_list;
  unbounded_octstring<true>           meas_cfg;
  cu_to_du_rrc_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CUtoDUTAInformation-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cu_to_du_tai_nformation_item_ext_ies_o = protocol_ext_empty_o;

using cu_to_du_tai_nformation_item_ext_ies_container = protocol_ext_container_empty_l;

// CUtoDUTAInformation-Item ::= SEQUENCE
struct cu_to_du_tai_nformation_item_s {
  bool                                           ie_exts_present = false;
  nr_cgi_s                                       nr_cgi;
  uint16_t                                       ta_value   = 0;
  uint8_t                                        preamb_idx = 0;
  uint32_t                                       ra_rnti    = 0;
  unbounded_octstring<true>                      tag_id_pointer;
  cu_to_du_tai_nformation_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CUtoDUTAInformation-List ::= SEQUENCE (SIZE (1..8)) OF CUtoDUTAInformation-Item
using cu_to_du_tai_nformation_list_l = dyn_array<cu_to_du_tai_nformation_item_s>;

// Cancel-all-Warning-Messages-Indicator ::= ENUMERATED
struct cancel_all_warning_msgs_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cancel_all_warning_msgs_ind_e = enumerated<cancel_all_warning_msgs_ind_opts, true>;

// Candidate-SpCell-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using candidate_sp_cell_item_ext_ies_o = protocol_ext_empty_o;

using candidate_sp_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// Candidate-SpCell-Item ::= SEQUENCE
struct candidate_sp_cell_item_s {
  bool                                     ie_exts_present = false;
  nr_cgi_s                                 candidate_sp_cell_id;
  candidate_sp_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CandidateCellwithBeamInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using candidate_cellwith_beam_info_ext_ies_o = protocol_ext_empty_o;

using candidate_cellwith_beam_info_ext_ies_container = protocol_ext_container_empty_l;

// CandidateCellwithBeamInfo ::= SEQUENCE
struct candidate_cellwith_beam_info_s {
  bool                                           ie_exts_present = false;
  nr_cgi_s                                       candidate_cell_id;
  uint8_t                                        ssb_idx = 0;
  candidate_cellwith_beam_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBIndex-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_idx_item_ext_ies_o = protocol_ext_empty_o;

using ssb_idx_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBIndex-Item ::= SEQUENCE
struct ssb_idx_item_s {
  bool                           ie_exts_present = false;
  uint8_t                        ssb_idx         = 0;
  ssb_idx_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBIndexList ::= SEQUENCE (SIZE (1..64)) OF SSBIndex-Item
using ssb_idx_list_l = dyn_array<ssb_idx_item_s>;

// CandidateCellwithBeamInfo-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using candidate_cellwith_beam_info_item_ext_ies_o = protocol_ext_empty_o;

using candidate_cellwith_beam_info_item_ext_ies_container = protocol_ext_container_empty_l;

// CandidateCellwithBeamInfo-Item ::= SEQUENCE
struct candidate_cellwith_beam_info_item_s {
  bool                                                ie_exts_present = false;
  nr_cgi_s                                            candidate_cell_id;
  ssb_idx_list_l                                      ssb_idx_list;
  candidate_cellwith_beam_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CandidateCellwithBeamInfoList ::= SEQUENCE (SIZE (1..8)) OF CandidateCellwithBeamInfo-Item
using candidate_cellwith_beam_info_list_l = dyn_array<candidate_cellwith_beam_info_item_s>;

// SelectedMeasurementQuantities-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sel_meas_quantities_ext_ies_o = protocol_ext_empty_o;

using sel_meas_quantities_ext_ies_container = protocol_ext_container_empty_l;

// SelectedMeasurementQuantities ::= SEQUENCE
struct sel_meas_quantities_s {
  bool                                  sinr_present    = false;
  bool                                  ie_exts_present = false;
  uint8_t                               rsrp            = 0;
  uint8_t                               rsrq            = 0;
  uint8_t                               sinr            = 0;
  sel_meas_quantities_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBIndexwithMeasurements-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_idxwith_meass_item_ext_ies_o = protocol_ext_empty_o;

using ssb_idxwith_meass_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBIndexwithMeasurements-Item ::= SEQUENCE
struct ssb_idxwith_meass_item_s {
  bool                                     ie_exts_present = false;
  uint8_t                                  ssb_idx         = 0;
  sel_meas_quantities_s                    sel_meas_quantities;
  ssb_idxwith_meass_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBIndexwithMeasurementsList ::= SEQUENCE (SIZE (1..64)) OF SSBIndexwithMeasurements-Item
using ssb_idxwith_meass_list_l = dyn_array<ssb_idxwith_meass_item_s>;

// CandidateCellwithMeasurements-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using candidate_cellwith_meass_item_ext_ies_o = protocol_ext_empty_o;

using candidate_cellwith_meass_item_ext_ies_container = protocol_ext_container_empty_l;

// CandidateCellwithMeasurements-Item ::= SEQUENCE
struct candidate_cellwith_meass_item_s {
  bool                                            ie_exts_present = false;
  nr_cgi_s                                        candidate_cell_id;
  ssb_idxwith_meass_list_l                        ssb_idxwith_meass_list;
  candidate_cellwith_meass_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CandidateCellwithMeasurementsList ::= SEQUENCE (SIZE (1..8)) OF CandidateCellwithMeasurements-Item
using candidate_cellwith_meass_list_l = dyn_array<candidate_cellwith_meass_item_s>;

// SSBAreaCapacityValueItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_area_capacity_value_item_ext_ies_o = protocol_ext_empty_o;

using ssb_area_capacity_value_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBAreaCapacityValueItem ::= SEQUENCE
struct ssb_area_capacity_value_item_s {
  bool                                           ie_exts_present         = false;
  uint8_t                                        ssb_idx                 = 0;
  uint8_t                                        ssb_area_capacity_value = 0;
  ssb_area_capacity_value_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBAreaCapacityValueList ::= SEQUENCE (SIZE (1..64)) OF SSBAreaCapacityValueItem
using ssb_area_capacity_value_list_l = dyn_array<ssb_area_capacity_value_item_s>;

// CapacityValue-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using capacity_value_ext_ies_o = protocol_ext_empty_o;

using capacity_value_ext_ies_container = protocol_ext_container_empty_l;

// CapacityValue ::= SEQUENCE
struct capacity_value_s {
  bool                             ie_exts_present = false;
  uint8_t                          capacity_value  = 0;
  ssb_area_capacity_value_list_l   ssb_area_capacity_value_list;
  capacity_value_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cell-Direction ::= ENUMERATED
struct cell_direction_opts {
  enum options { dl_only, ul_only, nulltype } value;

  const char* to_string() const;
};
using cell_direction_e = enumerated<cell_direction_opts>;

// CellBarred ::= ENUMERATED
struct cell_barred_opts {
  enum options { barred, not_barred, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cell_barred_e = enumerated<cell_barred_opts, true>;

// SSBAreaRadioResourceStatusItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_area_rr_status_item_ext_ies_o = protocol_ext_empty_o;

using ssb_area_rr_status_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBAreaRadioResourceStatusItem ::= SEQUENCE
struct ssb_area_rr_status_item_s {
  bool                                      dl_sched_pdcch_cc_eusage_present = false;
  bool                                      ul_sched_pdcch_cc_eusage_present = false;
  bool                                      ie_exts_present                  = false;
  uint8_t                                   ssb_idx                          = 0;
  uint8_t                                   ssb_area_dl_gbr_prb_usage        = 0;
  uint8_t                                   ssb_area_ul_gbr_prb_usage        = 0;
  uint8_t                                   ssb_area_dl_non_gbr_prb_usage    = 0;
  uint8_t                                   ssb_area_ul_non_gbr_prb_usage    = 0;
  uint8_t                                   ssb_area_dl_total_prb_usage      = 0;
  uint8_t                                   ssb_area_ul_total_prb_usage      = 0;
  uint8_t                                   dl_sched_pdcch_cc_eusage         = 0;
  uint8_t                                   ul_sched_pdcch_cc_eusage         = 0;
  ssb_area_rr_status_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBAreaRadioResourceStatusList ::= SEQUENCE (SIZE (1..64)) OF SSBAreaRadioResourceStatusItem
using ssb_area_rr_status_list_l = dyn_array<ssb_area_rr_status_item_s>;

// SNSSAI-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using snssai_ext_ies_o = protocol_ext_empty_o;

using snssai_ext_ies_container = protocol_ext_container_empty_l;

// SNSSAI ::= SEQUENCE
struct snssai_s {
  bool                     sd_present      = false;
  bool                     ie_exts_present = false;
  fixed_octstring<1, true> sst;
  fixed_octstring<3, true> sd;
  snssai_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNSSAIRadioResourceStatus-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using snssai_rr_status_item_ext_ies_o = protocol_ext_empty_o;

using snssai_rr_status_item_ext_ies_container = protocol_ext_container_empty_l;

// SNSSAIRadioResourceStatus-Item ::= SEQUENCE
struct snssai_rr_status_item_s {
  bool                                    ie_exts_present = false;
  snssai_s                                snssai;
  uint8_t                                 snssai_dl_gbr_prb_usage     = 0;
  uint8_t                                 snssai_ul_gbr_prb_usage     = 0;
  uint8_t                                 snssai_dl_non_gbr_prb_usage = 0;
  uint8_t                                 snssai_ul_non_gbr_prb_usage = 0;
  uint8_t                                 snssai_dl_total_prb_alloc   = 0;
  uint8_t                                 snssai_ul_total_prb_alloc   = 0;
  snssai_rr_status_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNSSAIRadioResourceStatus-List ::= SEQUENCE (SIZE (1..1024)) OF SNSSAIRadioResourceStatus-Item
using snssai_rr_status_list_l = dyn_array<snssai_rr_status_item_s>;

// SliceRadioResourceStatus-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slice_rr_status_item_ext_ies_o = protocol_ext_empty_o;

using slice_rr_status_item_ext_ies_container = protocol_ext_container_empty_l;

// SliceRadioResourceStatus-Item ::= SEQUENCE
struct slice_rr_status_item_s {
  bool                                   ie_exts_present = false;
  fixed_octstring<3, true>               plmn_id;
  snssai_rr_status_list_l                snssai_rr_status_list;
  slice_rr_status_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceRadioResourceStatus-List ::= SEQUENCE (SIZE (1..12)) OF SliceRadioResourceStatus-Item
using slice_rr_status_list_l = dyn_array<slice_rr_status_item_s>;

// SliceRadioResourceStatus-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slice_rr_status_ext_ies_o = protocol_ext_empty_o;

using slice_rr_status_ext_ies_container = protocol_ext_container_empty_l;

// SliceRadioResourceStatus ::= SEQUENCE
struct slice_rr_status_s {
  bool                              ie_exts_present = false;
  slice_rr_status_list_l            slice_rr_status;
  slice_rr_status_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MIMOPRBusageInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mimo_prb_usage_info_ext_ies_o = protocol_ext_empty_o;

using mimo_prb_usage_info_ext_ies_container = protocol_ext_container_empty_l;

// MIMOPRBusageInformation ::= SEQUENCE
struct mimo_prb_usage_info_s {
  bool                                  ie_exts_present               = false;
  uint8_t                               dl_gbr_prb_usage_for_mimo     = 0;
  uint8_t                               ul_gbr_prb_usage_for_mimo     = 0;
  uint8_t                               dl_non_gbr_prb_usage_for_mimo = 0;
  uint8_t                               ul_non_gbr_prb_usage_for_mimo = 0;
  uint8_t                               dl_total_prb_usage_for_mimo   = 0;
  uint8_t                               ul_total_prb_usage_for_mimo   = 0;
  mimo_prb_usage_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RadioResourceStatus-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct rr_status_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct rr_status_ext_ies_container {
  bool                  slice_rr_status_present     = false;
  bool                  mimo_prb_usage_info_present = false;
  slice_rr_status_s     slice_rr_status;
  mimo_prb_usage_info_s mimo_prb_usage_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RadioResourceStatus ::= SEQUENCE
struct rr_status_s {
  bool                        ie_exts_present = false;
  ssb_area_rr_status_list_l   ssb_area_rr_status_list;
  rr_status_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CompositeAvailableCapacity-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using composite_available_capacity_ext_ies_o = protocol_ext_empty_o;

using composite_available_capacity_ext_ies_container = protocol_ext_container_empty_l;

// CompositeAvailableCapacity ::= SEQUENCE
struct composite_available_capacity_s {
  bool                                           cell_capacity_class_value_present = false;
  bool                                           ie_exts_present                   = false;
  uint16_t                                       cell_capacity_class_value         = 1;
  capacity_value_s                               capacity_value;
  composite_available_capacity_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CompositeAvailableCapacityGroup-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct composite_available_capacity_group_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct composite_available_capacity_group_ext_ies_container {
  bool                           composite_available_capacity_sul_present = false;
  composite_available_capacity_s composite_available_capacity_sul;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CompositeAvailableCapacityGroup ::= SEQUENCE
struct composite_available_capacity_group_s {
  bool                                                 ie_exts_present = false;
  composite_available_capacity_s                       composite_available_capacity_dl;
  composite_available_capacity_s                       composite_available_capacity_ul;
  composite_available_capacity_group_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNSSAIAvailableCapacity-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using snssai_available_capacity_item_ext_ies_o = protocol_ext_empty_o;

using snssai_available_capacity_item_ext_ies_container = protocol_ext_container_empty_l;

// SNSSAIAvailableCapacity-Item ::= SEQUENCE
struct snssai_available_capacity_item_s {
  bool                                             slice_available_capacity_value_dl_present = false;
  bool                                             slice_available_capacity_value_ul_present = false;
  bool                                             ie_exts_present                           = false;
  snssai_s                                         snssai;
  uint8_t                                          slice_available_capacity_value_dl = 0;
  uint8_t                                          slice_available_capacity_value_ul = 0;
  snssai_available_capacity_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNSSAIAvailableCapacity-List ::= SEQUENCE (SIZE (1..1024)) OF SNSSAIAvailableCapacity-Item
using snssai_available_capacity_list_l = dyn_array<snssai_available_capacity_item_s>;

// SliceAvailableCapacityItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slice_available_capacity_item_ext_ies_o = protocol_ext_empty_o;

using slice_available_capacity_item_ext_ies_container = protocol_ext_container_empty_l;

// SliceAvailableCapacityItem ::= SEQUENCE
struct slice_available_capacity_item_s {
  bool                                            ie_exts_present = false;
  fixed_octstring<3, true>                        plmn_id;
  snssai_available_capacity_list_l                snssai_available_capacity_list;
  slice_available_capacity_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceAvailableCapacityList ::= SEQUENCE (SIZE (1..12)) OF SliceAvailableCapacityItem
using slice_available_capacity_list_l = dyn_array<slice_available_capacity_item_s>;

// SliceAvailableCapacity-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slice_available_capacity_ext_ies_o = protocol_ext_empty_o;

using slice_available_capacity_ext_ies_container = protocol_ext_container_empty_l;

// SliceAvailableCapacity ::= SEQUENCE
struct slice_available_capacity_s {
  bool                                       ie_exts_present = false;
  slice_available_capacity_list_l            slice_available_capacity_list;
  slice_available_capacity_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RadioResourceStatusNR-U-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rr_status_nr_u_ext_ies_o = protocol_ext_empty_o;

using rr_status_nr_u_ext_ies_container = protocol_ext_container_empty_l;

// RadioResourceStatusNR-U ::= SEQUENCE
struct rr_status_nr_u_s {
  bool                             ie_exts_present    = false;
  uint8_t                          dl_total_prb_usage = 0;
  uint8_t                          ul_total_prb_usage = 0;
  rr_status_nr_u_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-Channel-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct nr_u_ch_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct nr_u_ch_item_ext_ies_container {
  bool             ch_occupancy_time_percentage_ul_present = false;
  bool             rr_status_nr_u_present                  = false;
  uint16_t         ch_occupancy_time_percentage_ul;
  rr_status_nr_u_s rr_status_nr_u;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-Channel-Item ::= SEQUENCE
struct nr_u_ch_item_s {
  bool                           ie_exts_present                 = false;
  uint8_t                        nr_u_ch_id                      = 1;
  uint16_t                       ch_occupancy_time_percentage_dl = 0;
  int16_t                        energy_detection_thres          = -100;
  nr_u_ch_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-Channel-List ::= SEQUENCE (SIZE (1..16)) OF NR-U-Channel-Item
using nr_u_ch_list_l = dyn_array<nr_u_ch_item_s>;

// CellMeasurementResultItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct cell_meas_result_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cell_meas_result_item_ext_ies_container {
  bool           nr_u_ch_list_present = false;
  nr_u_ch_list_l nr_u_ch_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellMeasurementResultItem ::= SEQUENCE
struct cell_meas_result_item_s {
  bool                                    rr_status_present                          = false;
  bool                                    composite_available_capacity_group_present = false;
  bool                                    slice_available_capacity_present           = false;
  bool                                    numof_active_ues_present                   = false;
  bool                                    ie_exts_present                            = false;
  nr_cgi_s                                cell_id;
  rr_status_s                             rr_status;
  composite_available_capacity_group_s    composite_available_capacity_group;
  slice_available_capacity_s              slice_available_capacity;
  uint32_t                                numof_active_ues = 0;
  cell_meas_result_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellMeasurementResultList ::= SEQUENCE (SIZE (1..512)) OF CellMeasurementResultItem
using cell_meas_result_list_l = dyn_array<cell_meas_result_item_s>;

// CellSize ::= ENUMERATED
struct cell_size_opts {
  enum options { verysmall, small, medium, large, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cell_size_e = enumerated<cell_size_opts, true>;

// SSBToReportItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_to_report_item_ext_ies_o = protocol_ext_empty_o;

using ssb_to_report_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBToReportItem ::= SEQUENCE
struct ssb_to_report_item_s {
  bool                                 ie_exts_present = false;
  uint8_t                              ssb_idx         = 0;
  ssb_to_report_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBToReportList ::= SEQUENCE (SIZE (1..64)) OF SSBToReportItem
using ssb_to_report_list_l = dyn_array<ssb_to_report_item_s>;

// SNSSAI-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using snssai_item_ext_ies_o = protocol_ext_empty_o;

using snssai_item_ext_ies_container = protocol_ext_container_empty_l;

// SNSSAI-Item ::= SEQUENCE
struct snssai_item_s {
  bool                          ie_exts_present = false;
  snssai_s                      snssai;
  snssai_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNSSAI-list ::= SEQUENCE (SIZE (1..1024)) OF SNSSAI-Item
using snssai_list_l = dyn_array<snssai_item_s>;

// SliceToReportItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slice_to_report_item_ext_ies_o = protocol_ext_empty_o;

using slice_to_report_item_ext_ies_container = protocol_ext_container_empty_l;

// SliceToReportItem ::= SEQUENCE
struct slice_to_report_item_s {
  bool                                   ie_exts_present = false;
  fixed_octstring<3, true>               plmn_id;
  snssai_list_l                          snssai_list;
  slice_to_report_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceToReportList ::= SEQUENCE (SIZE (1..12)) OF SliceToReportItem
using slice_to_report_list_l = dyn_array<slice_to_report_item_s>;

// CellToReportItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cell_to_report_item_ext_ies_o = protocol_ext_empty_o;

using cell_to_report_item_ext_ies_container = protocol_ext_container_empty_l;

// CellToReportItem ::= SEQUENCE
struct cell_to_report_item_s {
  bool                                  ie_exts_present = false;
  nr_cgi_s                              cell_id;
  ssb_to_report_list_l                  ssb_to_report_list;
  slice_to_report_list_l                slice_to_report_list;
  cell_to_report_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellToReportList ::= SEQUENCE (SIZE (1..512)) OF CellToReportItem
using cell_to_report_list_l = dyn_array<cell_to_report_item_s>;

// CellType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cell_type_ext_ies_o = protocol_ext_empty_o;

using cell_type_ext_ies_container = protocol_ext_container_empty_l;

// CellType ::= SEQUENCE
struct cell_type_s {
  bool                        ie_exts_present = false;
  cell_size_e                 cell_size;
  cell_type_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellULConfigured ::= ENUMERATED
struct cell_ul_cfg_opts {
  enum options { none, ul, sul, ul_and_sul, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using cell_ul_cfg_e = enumerated<cell_ul_cfg_opts, true>;

// Cells-Allowed-to-be-Deactivated-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_allowed_to_be_deactiv_list_item_ext_ies_o = protocol_ext_empty_o;

using cells_allowed_to_be_deactiv_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Cells-Allowed-to-be-Deactivated-List-Item ::= SEQUENCE
struct cells_allowed_to_be_deactiv_list_item_s {
  bool                                                    ie_exts_present = false;
  nr_cgi_s                                                nr_cgi;
  cells_allowed_to_be_deactiv_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-Broadcast-Cancelled-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_broadcast_cancelled_item_ext_ies_o = protocol_ext_empty_o;

using cells_broadcast_cancelled_item_ext_ies_container = protocol_ext_container_empty_l;

// Cells-Broadcast-Cancelled-Item ::= SEQUENCE
struct cells_broadcast_cancelled_item_s {
  bool                                             ie_exts_present = false;
  nr_cgi_s                                         nr_cgi;
  uint32_t                                         nof_broadcasts = 0;
  cells_broadcast_cancelled_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-Broadcast-Completed-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_broadcast_completed_item_ext_ies_o = protocol_ext_empty_o;

using cells_broadcast_completed_item_ext_ies_container = protocol_ext_container_empty_l;

// Cells-Broadcast-Completed-Item ::= SEQUENCE
struct cells_broadcast_completed_item_s {
  bool                                             ie_exts_present = false;
  nr_cgi_s                                         nr_cgi;
  cells_broadcast_completed_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-Failed-to-be-Activated-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_failed_to_be_activ_list_item_ext_ies_o = protocol_ext_empty_o;

using cells_failed_to_be_activ_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Cells-Failed-to-be-Activated-List-Item ::= SEQUENCE
struct cells_failed_to_be_activ_list_item_s {
  bool                                                 ie_exts_present = false;
  nr_cgi_s                                             nr_cgi;
  cause_c                                              cause;
  cells_failed_to_be_activ_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Service-State ::= ENUMERATED
struct service_state_opts {
  enum options { in_service, out_of_service, /*...*/ nulltype } value;

  const char* to_string() const;
};
using service_state_e = enumerated<service_state_opts, true>;

// Service-Status-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using service_status_ext_ies_o = protocol_ext_empty_o;

using service_status_ext_ies_container = protocol_ext_container_empty_l;

// Service-Status ::= SEQUENCE
struct service_status_s {
  struct switching_off_ongoing_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using switching_off_ongoing_e_ = enumerated<switching_off_ongoing_opts, true>;

  // member variables
  bool                             switching_off_ongoing_present = false;
  bool                             ie_exts_present               = false;
  service_state_e                  service_state;
  switching_off_ongoing_e_         switching_off_ongoing;
  service_status_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-Status-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_status_item_ext_ies_o = protocol_ext_empty_o;

using cells_status_item_ext_ies_container = protocol_ext_container_empty_l;

// Cells-Status-Item ::= SEQUENCE
struct cells_status_item_s {
  bool                                ie_exts_present = false;
  nr_cgi_s                            nr_cgi;
  service_status_s                    service_status;
  cells_status_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-To-Be-Broadcast-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_to_be_broadcast_item_ext_ies_o = protocol_ext_empty_o;

using cells_to_be_broadcast_item_ext_ies_container = protocol_ext_container_empty_l;

// Cells-To-Be-Broadcast-Item ::= SEQUENCE
struct cells_to_be_broadcast_item_s {
  bool                                         ie_exts_present = false;
  nr_cgi_s                                     nr_cgi;
  cells_to_be_broadcast_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBs-activated-List ::= SEQUENCE (SIZE (1..64)) OF INTEGER (0..63)
using ss_bs_activ_list_l = dyn_array<uint8_t>;

// Cells-With-SSBs-Activated-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_with_ss_bs_activ_list_item_ext_ies_o = protocol_ext_empty_o;

using cells_with_ss_bs_activ_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Cells-With-SSBs-Activated-List-Item ::= SEQUENCE
struct cells_with_ss_bs_activ_list_item_s {
  bool                                               ie_exts_present = false;
  nr_cgi_s                                           nr_cgi;
  ss_bs_activ_list_l                                 ss_bs_activ_list;
  cells_with_ss_bs_activ_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-With-SSBs-Activated-List ::= SEQUENCE (SIZE (1..512)) OF Cells-With-SSBs-Activated-List-Item
using cells_with_ss_bs_activ_list_l = dyn_array<cells_with_ss_bs_activ_list_item_s>;

// SibtypetobeupdatedListItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct sibtypetobeupd_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sibtypetobeupd_list_item_ext_ies_container {
  bool         area_scope_present = false;
  area_scope_e area_scope;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SibtypetobeupdatedListItem ::= SEQUENCE
struct sibtypetobeupd_list_item_s {
  bool                                       ie_exts_present = false;
  uint8_t                                    sib_type        = 2;
  unbounded_octstring<true>                  sib_msg;
  uint8_t                                    value_tag = 0;
  sibtypetobeupd_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-CUSystemInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct gnb_cu_sys_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct gnb_cu_sys_info_ext_ies_container {
  bool                             sys_info_area_id_present = false;
  fixed_bitstring<24, false, true> sys_info_area_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-CUSystemInformation ::= SEQUENCE
struct gnb_cu_sys_info_s {
  using sibtypetobeupdlist_l_ = dyn_array<sibtypetobeupd_list_item_s>;

  // member variables
  bool                              ie_exts_present = false;
  sibtypetobeupdlist_l_             sibtypetobeupdlist;
  gnb_cu_sys_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSB-subcarrierSpacing ::= ENUMERATED
struct ssb_subcarrier_spacing_opts {
  enum options { khz15, khz30, khz120, khz240, spare3, spare2, spare1, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using ssb_subcarrier_spacing_e = enumerated<ssb_subcarrier_spacing_opts, true>;

// SSB-transmissionPeriodicity ::= ENUMERATED
struct ssb_tx_periodicity_opts {
  enum options { sf10, sf20, sf40, sf80, sf160, sf320, sf640, /*...*/ sf5, nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using ssb_tx_periodicity_e = enumerated<ssb_tx_periodicity_opts, true, 1>;

// SSB-transmisisonBitmap-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using ssb_transmisison_bitmap_ext_ies_o = protocol_ies_empty_o;

// SSB-transmissionBitmap ::= CHOICE
struct ssb_tx_bitmap_c {
  struct types_opts {
    enum options { short_bitmap, medium_bitmap, long_bitmap, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ssb_tx_bitmap_c() = default;
  ssb_tx_bitmap_c(const ssb_tx_bitmap_c& other);
  ssb_tx_bitmap_c& operator=(const ssb_tx_bitmap_c& other);
  ~ssb_tx_bitmap_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<4, false, true>& short_bitmap()
  {
    assert_choice_type(types::short_bitmap, type_, "SSB-transmissionBitmap");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  fixed_bitstring<8, false, true>& medium_bitmap()
  {
    assert_choice_type(types::medium_bitmap, type_, "SSB-transmissionBitmap");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  fixed_bitstring<64, false, true>& long_bitmap()
  {
    assert_choice_type(types::long_bitmap, type_, "SSB-transmissionBitmap");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  protocol_ie_single_container_s<ssb_transmisison_bitmap_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SSB-transmissionBitmap");
    return c.get<protocol_ie_single_container_s<ssb_transmisison_bitmap_ext_ies_o>>();
  }
  const fixed_bitstring<4, false, true>& short_bitmap() const
  {
    assert_choice_type(types::short_bitmap, type_, "SSB-transmissionBitmap");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  const fixed_bitstring<8, false, true>& medium_bitmap() const
  {
    assert_choice_type(types::medium_bitmap, type_, "SSB-transmissionBitmap");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  const fixed_bitstring<64, false, true>& long_bitmap() const
  {
    assert_choice_type(types::long_bitmap, type_, "SSB-transmissionBitmap");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  const protocol_ie_single_container_s<ssb_transmisison_bitmap_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SSB-transmissionBitmap");
    return c.get<protocol_ie_single_container_s<ssb_transmisison_bitmap_ext_ies_o>>();
  }
  fixed_bitstring<4, false, true>&                                   set_short_bitmap();
  fixed_bitstring<8, false, true>&                                   set_medium_bitmap();
  fixed_bitstring<64, false, true>&                                  set_long_bitmap();
  protocol_ie_single_container_s<ssb_transmisison_bitmap_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<64, false, true>, protocol_ie_single_container_s<ssb_transmisison_bitmap_ext_ies_o>>
      c;

  void destroy_();
};

// IAB-STC-Info-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_stc_info_item_ext_ies_o = protocol_ext_empty_o;

using iab_stc_info_item_ext_ies_container = protocol_ext_container_empty_l;

// IAB-STC-Info-Item ::= SEQUENCE
struct iab_stc_info_item_s {
  bool                                ie_exts_present = false;
  uint32_t                            ssb_freq_info   = 0;
  ssb_subcarrier_spacing_e            ssb_subcarrier_spacing;
  ssb_tx_periodicity_e                ssb_tx_periodicity;
  uint16_t                            ssb_tx_timing_offset = 0;
  ssb_tx_bitmap_c                     ssb_tx_bitmap;
  iab_stc_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-STC-Info-List ::= SEQUENCE (SIZE (1..45)) OF IAB-STC-Info-Item
using iab_stc_info_list_l = dyn_array<iab_stc_info_item_s>;

// IAB-STC-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_stc_info_ext_ies_o = protocol_ext_empty_o;

using iab_stc_info_ext_ies_container = protocol_ext_container_empty_l;

// IAB-STC-Info ::= SEQUENCE
struct iab_stc_info_s {
  bool                           ie_exts_present = false;
  iab_stc_info_list_l            iab_stc_info_list;
  iab_stc_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-Info-IAB-donor-CU-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_info_iab_donor_cu_ext_ies_o = protocol_ext_empty_o;

using iab_info_iab_donor_cu_ext_ies_container = protocol_ext_container_empty_l;

// IAB-Info-IAB-donor-CU ::= SEQUENCE
struct iab_info_iab_donor_cu_s {
  bool                                    iab_stc_info_present = false;
  bool                                    ie_exts_present      = false;
  iab_stc_info_s                          iab_stc_info;
  iab_info_iab_donor_cu_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBs-toBeActivated-List ::= SEQUENCE (SIZE (1..64)) OF INTEGER (0..63)
using ss_bs_to_be_activ_list_l = dyn_array<uint8_t>;

// Cells-to-be-Activated-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct cells_to_be_activ_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cells_to_be_activ_list_item_ext_ies_container {
  bool                           gnb_cu_sys_info_present                       = false;
  bool                           available_plmn_list_present                   = false;
  bool                           extended_available_plmn_list_present          = false;
  bool                           iab_info_iab_donor_cu_present                 = false;
  bool                           available_sn_pn_id_list_present               = false;
  bool                           mbs_broadcast_neighbour_cell_list_present     = false;
  bool                           ss_bs_within_the_cell_tobe_activ_list_present = false;
  gnb_cu_sys_info_s              gnb_cu_sys_info;
  available_plmn_list_l          available_plmn_list;
  extended_available_plmn_list_l extended_available_plmn_list;
  iab_info_iab_donor_cu_s        iab_info_iab_donor_cu;
  available_sn_pn_id_list_l      available_sn_pn_id_list;
  unbounded_octstring<true>      mbs_broadcast_neighbour_cell_list;
  ss_bs_to_be_activ_list_l       ss_bs_within_the_cell_tobe_activ_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-to-be-Activated-List-Item ::= SEQUENCE
struct cells_to_be_activ_list_item_s {
  bool                                          nr_pci_present  = false;
  bool                                          ie_exts_present = false;
  nr_cgi_s                                      nr_cgi;
  uint16_t                                      nr_pci = 0;
  cells_to_be_activ_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-Barred ::= ENUMERATED
struct iab_barred_opts {
  enum options { barred, not_barred, /*...*/ nulltype } value;

  const char* to_string() const;
};
using iab_barred_e = enumerated<iab_barred_opts, true>;

// MobileIAB-Barred ::= ENUMERATED
struct mobile_iab_barred_opts {
  enum options { barred, not_barred, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mobile_iab_barred_e = enumerated<mobile_iab_barred_opts, true>;

// Cells-to-be-Barred-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct cells_to_be_barred_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cells_to_be_barred_item_ext_ies_container {
  bool                iab_barred_present        = false;
  bool                mobile_iab_barred_present = false;
  iab_barred_e        iab_barred;
  mobile_iab_barred_e mobile_iab_barred;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-to-be-Barred-Item ::= SEQUENCE
struct cells_to_be_barred_item_s {
  bool                                      ie_exts_present = false;
  nr_cgi_s                                  nr_cgi;
  cell_barred_e                             cell_barred;
  cells_to_be_barred_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cells-to-be-Deactivated-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_to_be_deactiv_list_item_ext_ies_o = protocol_ext_empty_o;

using cells_to_be_deactiv_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Cells-to-be-Deactivated-List-Item ::= SEQUENCE
struct cells_to_be_deactiv_list_item_s {
  bool                                            ie_exts_present = false;
  nr_cgi_s                                        nr_cgi;
  cells_to_be_deactiv_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FreqInfoRel16-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using freq_info_rel16_ext_ies_o = protocol_ext_empty_o;

using freq_info_rel16_ext_ies_container = protocol_ext_container_empty_l;

// FreqInfoRel16 ::= SEQUENCE
struct freq_info_rel16_s {
  bool                              nr_arfcn_present         = false;
  bool                              freq_shift7p5khz_present = false;
  bool                              ie_exts_present          = false;
  uint32_t                          nr_arfcn                 = 0;
  freq_shift7p5khz_e                freq_shift7p5khz;
  nr_carrier_list_l                 carrier_list;
  freq_info_rel16_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FDD-InfoRel16-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using fdd_info_rel16_ext_ies_o = protocol_ext_empty_o;

using fdd_info_rel16_ext_ies_container = protocol_ext_container_empty_l;

// FDD-InfoRel16 ::= SEQUENCE
struct fdd_info_rel16_s {
  bool                             ul_freq_info_present  = false;
  bool                             sul_freq_info_present = false;
  bool                             ie_exts_present       = false;
  freq_info_rel16_s                ul_freq_info;
  freq_info_rel16_s                sul_freq_info;
  fdd_info_rel16_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TDD-InfoRel16-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tdd_info_rel16_ext_ies_o = protocol_ext_empty_o;

using tdd_info_rel16_ext_ies_container = protocol_ext_container_empty_l;

// TDD-InfoRel16 ::= SEQUENCE
struct tdd_info_rel16_s {
  bool                             tdd_freq_info_present = false;
  bool                             sul_freq_info_present = false;
  bool                             ie_exts_present       = false;
  freq_info_rel16_s                tdd_freq_info;
  freq_info_rel16_s                sul_freq_info;
  unbounded_octstring<true>        tdd_ul_dl_cfg_common_nr;
  tdd_info_rel16_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-ModeInfoRel16-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using nr_mode_info_rel16_ext_ies_o = protocol_ies_empty_o;

// NR-ModeInfoRel16 ::= CHOICE
struct nr_mode_info_rel16_c {
  struct types_opts {
    enum options { fdd, tdd, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  nr_mode_info_rel16_c() = default;
  nr_mode_info_rel16_c(const nr_mode_info_rel16_c& other);
  nr_mode_info_rel16_c& operator=(const nr_mode_info_rel16_c& other);
  ~nr_mode_info_rel16_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fdd_info_rel16_s& fdd()
  {
    assert_choice_type(types::fdd, type_, "NR-ModeInfoRel16");
    return c.get<fdd_info_rel16_s>();
  }
  tdd_info_rel16_s& tdd()
  {
    assert_choice_type(types::tdd, type_, "NR-ModeInfoRel16");
    return c.get<tdd_info_rel16_s>();
  }
  protocol_ie_single_container_s<nr_mode_info_rel16_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NR-ModeInfoRel16");
    return c.get<protocol_ie_single_container_s<nr_mode_info_rel16_ext_ies_o>>();
  }
  const fdd_info_rel16_s& fdd() const
  {
    assert_choice_type(types::fdd, type_, "NR-ModeInfoRel16");
    return c.get<fdd_info_rel16_s>();
  }
  const tdd_info_rel16_s& tdd() const
  {
    assert_choice_type(types::tdd, type_, "NR-ModeInfoRel16");
    return c.get<tdd_info_rel16_s>();
  }
  const protocol_ie_single_container_s<nr_mode_info_rel16_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NR-ModeInfoRel16");
    return c.get<protocol_ie_single_container_s<nr_mode_info_rel16_ext_ies_o>>();
  }
  fdd_info_rel16_s&                                             set_fdd();
  tdd_info_rel16_s&                                             set_tdd();
  protocol_ie_single_container_s<nr_mode_info_rel16_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fdd_info_rel16_s, protocol_ie_single_container_s<nr_mode_info_rel16_ext_ies_o>, tdd_info_rel16_s> c;

  void destroy_();
};

// SSB-PositionsInBurst-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using ssb_positions_in_burst_ext_ies_o = protocol_ies_empty_o;

// SSB-PositionsInBurst ::= CHOICE
struct ssb_positions_in_burst_c {
  struct types_opts {
    enum options { short_bitmap, medium_bitmap, long_bitmap, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ssb_positions_in_burst_c() = default;
  ssb_positions_in_burst_c(const ssb_positions_in_burst_c& other);
  ssb_positions_in_burst_c& operator=(const ssb_positions_in_burst_c& other);
  ~ssb_positions_in_burst_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<4, false, true>& short_bitmap()
  {
    assert_choice_type(types::short_bitmap, type_, "SSB-PositionsInBurst");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  fixed_bitstring<8, false, true>& medium_bitmap()
  {
    assert_choice_type(types::medium_bitmap, type_, "SSB-PositionsInBurst");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  fixed_bitstring<64, false, true>& long_bitmap()
  {
    assert_choice_type(types::long_bitmap, type_, "SSB-PositionsInBurst");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  protocol_ie_single_container_s<ssb_positions_in_burst_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SSB-PositionsInBurst");
    return c.get<protocol_ie_single_container_s<ssb_positions_in_burst_ext_ies_o>>();
  }
  const fixed_bitstring<4, false, true>& short_bitmap() const
  {
    assert_choice_type(types::short_bitmap, type_, "SSB-PositionsInBurst");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  const fixed_bitstring<8, false, true>& medium_bitmap() const
  {
    assert_choice_type(types::medium_bitmap, type_, "SSB-PositionsInBurst");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  const fixed_bitstring<64, false, true>& long_bitmap() const
  {
    assert_choice_type(types::long_bitmap, type_, "SSB-PositionsInBurst");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  const protocol_ie_single_container_s<ssb_positions_in_burst_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SSB-PositionsInBurst");
    return c.get<protocol_ie_single_container_s<ssb_positions_in_burst_ext_ies_o>>();
  }
  fixed_bitstring<4, false, true>&                                  set_short_bitmap();
  fixed_bitstring<8, false, true>&                                  set_medium_bitmap();
  fixed_bitstring<64, false, true>&                                 set_long_bitmap();
  protocol_ie_single_container_s<ssb_positions_in_burst_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<64, false, true>, protocol_ie_single_container_s<ssb_positions_in_burst_ext_ies_o>> c;

  void destroy_();
};

// L839Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using l839_info_ext_ies_o = protocol_ext_empty_o;

using l839_info_ext_ies_container = protocol_ext_container_empty_l;

// L839Info ::= SEQUENCE
struct l839_info_s {
  struct restricted_set_cfg_opts {
    enum options { unrestricted_set, restricted_set_type_a, restricted_set_type_b, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using restricted_set_cfg_e_ = enumerated<restricted_set_cfg_opts, true>;

  // member variables
  bool                        ie_ext_present = false;
  uint16_t                    root_seq_idx   = 0;
  restricted_set_cfg_e_       restricted_set_cfg;
  l839_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// L139Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using l139_info_ext_ies_o = protocol_ext_empty_o;

using l139_info_ext_ies_container = protocol_ext_container_empty_l;

// L139Info ::= SEQUENCE
struct l139_info_s {
  struct prach_scs_opts {
    enum options { scs15, scs30, scs60, scs120, /*...*/ scs480, scs960, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using prach_scs_e_ = enumerated<prach_scs_opts, true, 2>;

  // member variables
  bool                        root_seq_idx_present = false;
  bool                        ie_ext_present       = false;
  prach_scs_e_                prach_scs;
  uint8_t                     root_seq_idx = 0;
  l139_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// L571Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using l571_info_ext_ies_o = protocol_ext_empty_o;

using l571_info_ext_ies_container = protocol_ext_container_empty_l;

// L571Info ::= SEQUENCE
struct l571_info_s {
  struct prach_scs_for_l571_opts {
    enum options { scs30, scs120, /*...*/ scs480, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using prach_scs_for_l571_e_ = enumerated<prach_scs_for_l571_opts, true, 1>;

  // member variables
  bool                        ie_ext_present = false;
  prach_scs_for_l571_e_       prach_scs_for_l571;
  uint16_t                    root_seq_idx = 0;
  l571_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// L1151Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using l1151_info_ext_ies_o = protocol_ext_empty_o;

using l1151_info_ext_ies_container = protocol_ext_container_empty_l;

// L1151Info ::= SEQUENCE
struct l1151_info_s {
  struct prach_scs_for_l1151_opts {
    enum options { scs15, scs120, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using prach_scs_for_l1151_e_ = enumerated<prach_scs_for_l1151_opts, true>;

  // member variables
  bool                         ie_ext_present = false;
  prach_scs_for_l1151_e_       prach_scs_for_l1151;
  uint16_t                     root_seq_idx = 0;
  l1151_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FreqDomainLength-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct freq_domain_len_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { l571_info, l1151_info, nulltype } value;
      typedef uint16_t number_type;

      const char* to_string() const;
      uint16_t    to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    value_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    l571_info_s&        l571_info();
    l1151_info_s&       l1151_info();
    const l571_info_s&  l571_info() const;
    const l1151_info_s& l1151_info() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// FreqDomainLength ::= CHOICE
struct freq_domain_len_c {
  struct types_opts {
    enum options { l839, l139, choice_ext, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  freq_domain_len_c() = default;
  freq_domain_len_c(const freq_domain_len_c& other);
  freq_domain_len_c& operator=(const freq_domain_len_c& other);
  ~freq_domain_len_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  l839_info_s& l839()
  {
    assert_choice_type(types::l839, type_, "FreqDomainLength");
    return c.get<l839_info_s>();
  }
  l139_info_s& l139()
  {
    assert_choice_type(types::l139, type_, "FreqDomainLength");
    return c.get<l139_info_s>();
  }
  protocol_ie_single_container_s<freq_domain_len_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "FreqDomainLength");
    return c.get<protocol_ie_single_container_s<freq_domain_len_ext_ies_o>>();
  }
  const l839_info_s& l839() const
  {
    assert_choice_type(types::l839, type_, "FreqDomainLength");
    return c.get<l839_info_s>();
  }
  const l139_info_s& l139() const
  {
    assert_choice_type(types::l139, type_, "FreqDomainLength");
    return c.get<l139_info_s>();
  }
  const protocol_ie_single_container_s<freq_domain_len_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "FreqDomainLength");
    return c.get<protocol_ie_single_container_s<freq_domain_len_ext_ies_o>>();
  }
  l839_info_s&                                               set_l839();
  l139_info_s&                                               set_l139();
  protocol_ie_single_container_s<freq_domain_len_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                type_;
  choice_buffer_t<l139_info_s, l839_info_s, protocol_ie_single_container_s<freq_domain_len_ext_ies_o>> c;

  void destroy_();
};

// NRPRACHConfigItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_prach_cfg_item_ext_ies_o = protocol_ext_empty_o;

using nr_prach_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// NRPRACHConfigItem ::= SEQUENCE
struct nr_prach_cfg_item_s {
  struct prach_fdm_opts {
    enum options { one, two, four, eight, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using prach_fdm_e_ = enumerated<prach_fdm_opts, true>;
  struct ssb_per_rach_occasion_opts {
    enum options { one_eighth, one_fourth, one_half, one, two, four, eight, sixteen, /*...*/ nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using ssb_per_rach_occasion_e_ = enumerated<ssb_per_rach_occasion_opts, true>;

  // member variables
  bool                                ie_ext_present = false;
  nr_scs_e                            nr_scs;
  uint16_t                            prach_freq_startfrom_carrier = 0;
  prach_fdm_e_                        prach_fdm;
  uint16_t                            prach_cfg_idx = 0;
  ssb_per_rach_occasion_e_            ssb_per_rach_occasion;
  freq_domain_len_c                   freq_domain_len;
  uint8_t                             zero_correl_zone_cfg = 0;
  nr_prach_cfg_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRPRACHConfigList ::= SEQUENCE (SIZE (0..16)) OF NRPRACHConfigItem
using nr_prach_cfg_list_l = dyn_array<nr_prach_cfg_item_s>;

// NRPRACHConfig-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_prach_cfg_ext_ies_o = protocol_ext_empty_o;

using nr_prach_cfg_ext_ies_container = protocol_ext_container_empty_l;

// NRPRACHConfig ::= SEQUENCE
struct nr_prach_cfg_s {
  bool                           ul_prach_cfg_list_present  = false;
  bool                           sul_prach_cfg_list_present = false;
  bool                           ie_ext_present             = false;
  nr_prach_cfg_list_l            ul_prach_cfg_list;
  nr_prach_cfg_list_l            sul_prach_cfg_list;
  nr_prach_cfg_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NeighbourNR-CellsForSON-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using neighbour_nr_cells_for_son_item_ext_ies_o = protocol_ext_empty_o;

using neighbour_nr_cells_for_son_item_ext_ies_container = protocol_ext_container_empty_l;

// NeighbourNR-CellsForSON-Item ::= SEQUENCE
struct neighbour_nr_cells_for_son_item_s {
  bool                                              nr_mode_info_rel16_present     = false;
  bool                                              ssb_positions_in_burst_present = false;
  bool                                              nr_prach_cfg_present           = false;
  bool                                              ie_exts_present                = false;
  nr_cgi_s                                          nr_cgi;
  nr_mode_info_rel16_c                              nr_mode_info_rel16;
  ssb_positions_in_burst_c                          ssb_positions_in_burst;
  nr_prach_cfg_s                                    nr_prach_cfg;
  neighbour_nr_cells_for_son_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NeighbourNR-CellsForSON-List ::= SEQUENCE (SIZE (1..32)) OF NeighbourNR-CellsForSON-Item
using neighbour_nr_cells_for_son_list_l = dyn_array<neighbour_nr_cells_for_son_item_s>;

// CellsForSON-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cells_for_son_item_ext_ies_o = protocol_ext_empty_o;

using cells_for_son_item_ext_ies_container = protocol_ext_container_empty_l;

// CellsForSON-Item ::= SEQUENCE
struct cells_for_son_item_s {
  bool                                 ie_exts_present = false;
  nr_cgi_s                             nr_cgi;
  neighbour_nr_cells_for_son_list_l    neighbour_nr_cells_for_son_list;
  cells_for_son_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellsForSON-List ::= SEQUENCE (SIZE (1..256)) OF CellsForSON-Item
using cells_for_son_list_l = dyn_array<cells_for_son_item_s>;

// DU-RX-MT-RX ::= ENUMERATED
struct du_rx_mt_rx_opts {
  enum options { supported, not_supported, nulltype } value;

  const char* to_string() const;
};
using du_rx_mt_rx_e = enumerated<du_rx_mt_rx_opts>;

// DU-TX-MT-TX ::= ENUMERATED
struct du_tx_mt_tx_opts {
  enum options { supported, not_supported, nulltype } value;

  const char* to_string() const;
};
using du_tx_mt_tx_e = enumerated<du_tx_mt_tx_opts>;

// DU-RX-MT-TX ::= ENUMERATED
struct du_rx_mt_tx_opts {
  enum options { supported, not_supported, nulltype } value;

  const char* to_string() const;
};
using du_rx_mt_tx_e = enumerated<du_rx_mt_tx_opts>;

// DU-TX-MT-RX ::= ENUMERATED
struct du_tx_mt_rx_opts {
  enum options { supported, not_supported, nulltype } value;

  const char* to_string() const;
};
using du_tx_mt_rx_e = enumerated<du_tx_mt_rx_opts>;

// DU-RX-MT-RX-Extend ::= ENUMERATED
struct du_rx_mt_rx_extend_opts {
  enum options { supported, not_supported, supported_and_fdm_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_rx_mt_rx_extend_e = enumerated<du_rx_mt_rx_extend_opts, true>;

// DU-TX-MT-TX-Extend ::= ENUMERATED
struct du_tx_mt_tx_extend_opts {
  enum options { supported, not_supported, supported_and_fdm_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_tx_mt_tx_extend_e = enumerated<du_tx_mt_tx_extend_opts, true>;

// DU-RX-MT-TX-Extend ::= ENUMERATED
struct du_rx_mt_tx_extend_opts {
  enum options { supported, not_supported, supported_and_fdm_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_rx_mt_tx_extend_e = enumerated<du_rx_mt_tx_extend_opts, true>;

// DU-TX-MT-RX-Extend ::= ENUMERATED
struct du_tx_mt_rx_extend_opts {
  enum options { supported, not_supported, supported_and_fdm_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_tx_mt_rx_extend_e = enumerated<du_tx_mt_rx_extend_opts, true>;

// IAB-MT-Cell-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct iab_mt_cell_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_mt_cell_list_item_ext_ies_container {
  bool                 du_rx_mt_rx_extend_present = false;
  bool                 du_tx_mt_tx_extend_present = false;
  bool                 du_rx_mt_tx_extend_present = false;
  bool                 du_tx_mt_rx_extend_present = false;
  du_rx_mt_rx_extend_e du_rx_mt_rx_extend;
  du_tx_mt_tx_extend_e du_tx_mt_tx_extend;
  du_rx_mt_tx_extend_e du_rx_mt_tx_extend;
  du_tx_mt_rx_extend_e du_tx_mt_rx_extend;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-MT-Cell-List-Item ::= SEQUENCE
struct iab_mt_cell_list_item_s {
  bool                                    ie_exts_present = false;
  fixed_bitstring<36, false, true>        nr_cell_id;
  du_rx_mt_rx_e                           du_rx_mt_rx;
  du_tx_mt_tx_e                           du_tx_mt_tx;
  du_rx_mt_tx_e                           du_rx_mt_tx;
  du_tx_mt_rx_e                           du_tx_mt_rx;
  iab_mt_cell_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-MT-Cell-List ::= SEQUENCE (SIZE (1..32)) OF IAB-MT-Cell-List-Item
using iab_mt_cell_list_l = dyn_array<iab_mt_cell_list_item_s>;

// MultiplexingInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mux_info_ext_ies_o = protocol_ext_empty_o;

using mux_info_ext_ies_container = protocol_ext_container_empty_l;

// MultiplexingInfo ::= SEQUENCE
struct mux_info_s {
  bool                       ie_exts_present = false;
  iab_mt_cell_list_l         iab_mt_cell_list;
  mux_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Child-Node-Cells-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using child_node_cells_list_item_ext_ies_o = protocol_ext_empty_o;

using child_node_cells_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Child-Node-Cells-List-Item ::= SEQUENCE
struct child_node_cells_list_item_s {
  bool                                         iab_du_cell_res_cfg_mode_info_present = false;
  bool                                         iab_stc_info_present                  = false;
  bool                                         mux_info_present                      = false;
  bool                                         ie_exts_present                       = false;
  nr_cgi_s                                     nr_cgi;
  iab_du_cell_res_cfg_mode_info_c              iab_du_cell_res_cfg_mode_info;
  iab_stc_info_s                               iab_stc_info;
  unbounded_octstring<true>                    rach_cfg_common;
  unbounded_octstring<true>                    rach_cfg_common_iab;
  unbounded_octstring<true>                    csi_rs_cfg;
  unbounded_octstring<true>                    sr_cfg;
  unbounded_octstring<true>                    pdcch_cfg_sib1;
  unbounded_octstring<true>                    scs_common;
  mux_info_s                                   mux_info;
  child_node_cells_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Child-Node-Cells-List ::= SEQUENCE (SIZE (1..1024)) OF Child-Node-Cells-List-Item
using child_node_cells_list_l = dyn_array<child_node_cells_list_item_s>;

// Child-Nodes-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using child_nodes_list_item_ext_ies_o = protocol_ext_empty_o;

using child_nodes_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Child-Nodes-List-Item ::= SEQUENCE
struct child_nodes_list_item_s {
  bool                                    ie_exts_present   = false;
  uint64_t                                gnb_cu_ue_f1ap_id = 0;
  uint64_t                                gnb_du_ue_f1ap_id = 0;
  child_node_cells_list_l                 child_node_cells_list;
  child_nodes_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Child-Nodes-List ::= SEQUENCE (SIZE (1..1024)) OF Child-Nodes-List-Item
using child_nodes_list_l = dyn_array<child_nodes_list_item_s>;

// TRP-Beam-Power-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_beam_pwr_item_ext_ies_o = protocol_ext_empty_o;

using trp_beam_pwr_item_ext_ies_container = protocol_ext_container_empty_l;

// TRP-Beam-Power-Item ::= SEQUENCE
struct trp_beam_pwr_item_s {
  bool                                prs_res_set_id_present    = false;
  bool                                relative_pwr_fine_present = false;
  bool                                ie_exts_present           = false;
  uint8_t                             prs_res_set_id            = 0;
  uint8_t                             prs_res_id                = 0;
  uint8_t                             relative_pwr              = 0;
  uint8_t                             relative_pwr_fine         = 0;
  trp_beam_pwr_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-ElevationAngleList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_elevation_angle_list_item_ext_ies_o = protocol_ext_empty_o;

using trp_elevation_angle_list_item_ext_ies_container = protocol_ext_container_empty_l;

// TRP-ElevationAngleList-Item ::= SEQUENCE
struct trp_elevation_angle_list_item_s {
  using trp_beam_pwr_list_l_ = dyn_array<trp_beam_pwr_item_s>;

  // member variables
  bool                                            trp_elevation_angle_fine_present = false;
  bool                                            ie_exts_present                  = false;
  uint8_t                                         trp_elevation_angle              = 0;
  uint8_t                                         trp_elevation_angle_fine         = 0;
  trp_beam_pwr_list_l_                            trp_beam_pwr_list;
  trp_elevation_angle_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-BeamAntennaAnglesList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_beam_ant_angles_list_item_ext_ies_o = protocol_ext_empty_o;

using trp_beam_ant_angles_list_item_ext_ies_container = protocol_ext_container_empty_l;

// TRP-BeamAntennaAnglesList-Item ::= SEQUENCE
struct trp_beam_ant_angles_list_item_s {
  using trp_elevation_angle_list_l_ = dyn_array<trp_elevation_angle_list_item_s>;

  // member variables
  bool                                            trp_azimuth_angle_fine_present = false;
  bool                                            ie_exts_present                = false;
  uint16_t                                        trp_azimuth_angle              = 0;
  uint8_t                                         trp_azimuth_angle_fine         = 0;
  trp_elevation_angle_list_l_                     trp_elevation_angle_list;
  trp_beam_ant_angles_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-BeamAntennaAngles ::= SEQUENCE (SIZE (1..3600)) OF TRP-BeamAntennaAnglesList-Item
using trp_beam_ant_angles_l = dyn_array<trp_beam_ant_angles_list_item_s>;

// TRP-BeamAntennaExplicitInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_beam_ant_explicit_info_ext_ies_o = protocol_ext_empty_o;

using trp_beam_ant_explicit_info_ext_ies_container = protocol_ext_container_empty_l;

// TRP-BeamAntennaExplicitInformation ::= SEQUENCE
struct trp_beam_ant_explicit_info_s {
  bool                                         lcs_to_gcs_translation_present = false;
  bool                                         ie_exts_present                = false;
  trp_beam_ant_angles_l                        trp_beam_ant_angles;
  lcs_to_gcs_translation_s                     lcs_to_gcs_translation;
  trp_beam_ant_explicit_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Choice-TRP-Beam-Info-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using choice_trp_beam_info_item_ext_ies_o = protocol_ies_empty_o;

// Choice-TRP-Beam-Antenna-Info-Item ::= CHOICE
struct choice_trp_beam_ant_info_item_c {
  struct types_opts {
    enum options { ref, explicit_value, no_change, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  choice_trp_beam_ant_info_item_c() = default;
  choice_trp_beam_ant_info_item_c(const choice_trp_beam_ant_info_item_c& other);
  choice_trp_beam_ant_info_item_c& operator=(const choice_trp_beam_ant_info_item_c& other);
  ~choice_trp_beam_ant_info_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint32_t& ref()
  {
    assert_choice_type(types::ref, type_, "Choice-TRP-Beam-Antenna-Info-Item");
    return c.get<uint32_t>();
  }
  trp_beam_ant_explicit_info_s& explicit_value()
  {
    assert_choice_type(types::explicit_value, type_, "Choice-TRP-Beam-Antenna-Info-Item");
    return c.get<trp_beam_ant_explicit_info_s>();
  }
  protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "Choice-TRP-Beam-Antenna-Info-Item");
    return c.get<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>();
  }
  const uint32_t& ref() const
  {
    assert_choice_type(types::ref, type_, "Choice-TRP-Beam-Antenna-Info-Item");
    return c.get<uint32_t>();
  }
  const trp_beam_ant_explicit_info_s& explicit_value() const
  {
    assert_choice_type(types::explicit_value, type_, "Choice-TRP-Beam-Antenna-Info-Item");
    return c.get<trp_beam_ant_explicit_info_s>();
  }
  const protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "Choice-TRP-Beam-Antenna-Info-Item");
    return c.get<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>>();
  }
  uint32_t&                                                            set_ref();
  trp_beam_ant_explicit_info_s&                                        set_explicit_value();
  void                                                                 set_no_change();
  protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<choice_trp_beam_info_item_ext_ies_o>, trp_beam_ant_explicit_info_s> c;

  void destroy_();
};

// ClockAccuracy-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using clock_accuracy_ext_ies_o = protocol_ies_empty_o;

// ClockAccuracy ::= CHOICE
struct clock_accuracy_c {
  struct types_opts {
    enum options { clock_accuracy_value, clock_accuracy_idx, choice_exts, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  clock_accuracy_c() = default;
  clock_accuracy_c(const clock_accuracy_c& other);
  clock_accuracy_c& operator=(const clock_accuracy_c& other);
  ~clock_accuracy_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint32_t& clock_accuracy_value()
  {
    assert_choice_type(types::clock_accuracy_value, type_, "ClockAccuracy");
    return c.get<uint32_t>();
  }
  uint8_t& clock_accuracy_idx()
  {
    assert_choice_type(types::clock_accuracy_idx, type_, "ClockAccuracy");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<clock_accuracy_ext_ies_o>& choice_exts()
  {
    assert_choice_type(types::choice_exts, type_, "ClockAccuracy");
    return c.get<protocol_ie_single_container_s<clock_accuracy_ext_ies_o>>();
  }
  const uint32_t& clock_accuracy_value() const
  {
    assert_choice_type(types::clock_accuracy_value, type_, "ClockAccuracy");
    return c.get<uint32_t>();
  }
  const uint8_t& clock_accuracy_idx() const
  {
    assert_choice_type(types::clock_accuracy_idx, type_, "ClockAccuracy");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<clock_accuracy_ext_ies_o>& choice_exts() const
  {
    assert_choice_type(types::choice_exts, type_, "ClockAccuracy");
    return c.get<protocol_ie_single_container_s<clock_accuracy_ext_ies_o>>();
  }
  uint32_t&                                                 set_clock_accuracy_value();
  uint8_t&                                                  set_clock_accuracy_idx();
  protocol_ie_single_container_s<clock_accuracy_ext_ies_o>& set_choice_exts();

private:
  types                                                                     type_;
  choice_buffer_t<protocol_ie_single_container_s<clock_accuracy_ext_ies_o>> c;

  void destroy_();
};

// CompleteCandidateConfigurationIndicator ::= ENUMERATED
struct complete_candidate_cfg_ind_opts {
  enum options { complete, /*...*/ nulltype } value;

  const char* to_string() const;
};
using complete_candidate_cfg_ind_e = enumerated<complete_candidate_cfg_ind_opts, true>;

// SCPAC-Request ::= ENUMERATED
struct s_cp_ac_request_opts {
  enum options { initiation, /*...*/ nulltype } value;

  const char* to_string() const;
};
using s_cp_ac_request_e = enumerated<s_cp_ac_request_opts, true>;

// S-CPACLowerLayerReferenceConfigRequest ::= ENUMERATED
struct s_cp_ac_lower_layer_ref_cfg_request_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using s_cp_ac_lower_layer_ref_cfg_request_e = enumerated<s_cp_ac_lower_layer_ref_cfg_request_opts, true>;

// ConditionalInterDUMobilityInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct conditional_inter_du_mob_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct conditional_inter_du_mob_info_ext_ies_container {
  bool                                  estimated_arrival_probability_present       = false;
  bool                                  s_cp_ac_request_present                     = false;
  bool                                  s_cp_ac_lower_layer_ref_cfg_request_present = false;
  uint8_t                               estimated_arrival_probability;
  s_cp_ac_request_e                     s_cp_ac_request;
  s_cp_ac_lower_layer_ref_cfg_request_e s_cp_ac_lower_layer_ref_cfg_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ConditionalInterDUMobilityInformation ::= SEQUENCE
struct conditional_inter_du_mob_info_s {
  bool                                            target_gnb_du_ue_f1ap_id_present = false;
  bool                                            ie_exts_present                  = false;
  ch_otrigger_inter_du_e                          cho_trigger;
  uint64_t                                        target_gnb_du_ue_f1ap_id = 0;
  conditional_inter_du_mob_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TargetCellList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using target_cell_list_item_ext_ies_o = protocol_ext_empty_o;

using target_cell_list_item_ext_ies_container = protocol_ext_container_empty_l;

// TargetCellList-Item ::= SEQUENCE
struct target_cell_list_item_s {
  bool                                    ie_exts_present = false;
  nr_cgi_s                                target_cell;
  target_cell_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TargetCellList ::= SEQUENCE (SIZE (1..8)) OF TargetCellList-Item
using target_cell_list_l = dyn_array<target_cell_list_item_s>;

// ConditionalIntraDUMobilityInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct conditional_intra_du_mob_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct conditional_intra_du_mob_info_ext_ies_container {
  bool                                  estimated_arrival_probability_present       = false;
  bool                                  s_cp_ac_request_present                     = false;
  bool                                  s_cp_ac_lower_layer_ref_cfg_request_present = false;
  uint8_t                               estimated_arrival_probability;
  s_cp_ac_request_e                     s_cp_ac_request;
  s_cp_ac_lower_layer_ref_cfg_request_e s_cp_ac_lower_layer_ref_cfg_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ConditionalIntraDUMobilityInformation ::= SEQUENCE
struct conditional_intra_du_mob_info_s {
  bool                                            ie_exts_present = false;
  ch_otrigger_intra_du_e                          cho_trigger;
  target_cell_list_l                              target_cells_tocancel;
  conditional_intra_du_mob_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Configured-BWP-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using cfg_bwp_item_ext_ies_o = protocol_ext_empty_o;

using cfg_bwp_item_ext_ies_container = protocol_ext_container_empty_l;

// Configured-BWP-Item ::= SEQUENCE
struct cfg_bwp_item_s {
  bool                           ie_exts_present     = false;
  uint8_t                        bwp_id              = 0;
  uint16_t                       bwp_location_and_bw = 0;
  cfg_bwp_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Configured-BWP-List ::= SEQUENCE (SIZE (1..8)) OF Configured-BWP-Item
using cfg_bwp_list_l = dyn_array<cfg_bwp_item_s>;

// CongestionInformationRequest ::= ENUMERATED
struct congestion_info_request_opts {
  enum options { ul, dl, both, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using congestion_info_request_e = enumerated<congestion_info_request_opts, true>;

// ControllerUE ::= ENUMERATED
struct ctrller_ue_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ctrller_ue_e = enumerated<ctrller_ue_opts, true>;

// SSBCoverageModification-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_coverage_mod_item_ext_ies_o = protocol_ext_empty_o;

using ssb_coverage_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBCoverageModification-Item ::= SEQUENCE
struct ssb_coverage_mod_item_s {
  bool                                    ie_exts_present    = false;
  uint8_t                                 ssb_idx            = 0;
  uint8_t                                 ssb_coverage_state = 0;
  ssb_coverage_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBCoverageModification-List ::= SEQUENCE (SIZE (1..64)) OF SSBCoverageModification-Item
using ssb_coverage_mod_list_l = dyn_array<ssb_coverage_mod_item_s>;

// Coverage-Modification-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct coverage_mod_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct coverage_mod_item_ext_ies_container {
  bool                  coverage_mod_cause_present = false;
  cco_issue_detection_e coverage_mod_cause;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Coverage-Modification-Item ::= SEQUENCE
struct coverage_mod_item_s {
  bool                                ie_ext_present = false;
  nr_cgi_s                            nr_cgi;
  uint8_t                             cell_coverage_state = 0;
  ssb_coverage_mod_list_l             ssb_coverage_mod_list;
  coverage_mod_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Coverage-Modification-List ::= SEQUENCE (SIZE (1..512)) OF Coverage-Modification-Item
using coverage_mod_list_l = dyn_array<coverage_mod_item_s>;

// Coverage-Modification-Notification-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using coverage_mod_notif_ext_ies_o = protocol_ext_empty_o;

using coverage_mod_notif_ext_ies_container = protocol_ext_container_empty_l;

// Coverage-Modification-Notification ::= SEQUENCE
struct coverage_mod_notif_s {
  bool                                 ie_exts_present = false;
  coverage_mod_list_l                  coverage_mod_list;
  coverage_mod_notif_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TypeOfError ::= ENUMERATED
struct type_of_error_opts {
  enum options { not_understood, missing, /*...*/ nulltype } value;

  const char* to_string() const;
};
using type_of_error_e = enumerated<type_of_error_opts, true>;

// CriticalityDiagnostics-IE-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using crit_diagnostics_ie_item_ext_ies_o = protocol_ext_empty_o;

using crit_diagnostics_ie_item_ext_ies_container = protocol_ext_container_empty_l;

// CriticalityDiagnostics-IE-Item ::= SEQUENCE
struct crit_diagnostics_ie_item_s {
  bool                                       ie_exts_present = false;
  crit_e                                     ie_crit;
  uint32_t                                   ie_id = 0;
  type_of_error_e                            type_of_error;
  crit_diagnostics_ie_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CriticalityDiagnostics-IE-List ::= SEQUENCE (SIZE (1..256)) OF CriticalityDiagnostics-IE-Item
using crit_diagnostics_ie_list_l = dyn_array<crit_diagnostics_ie_item_s>;

// CriticalityDiagnostics-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using crit_diagnostics_ext_ies_o = protocol_ext_empty_o;

using crit_diagnostics_ext_ies_container = protocol_ext_container_empty_l;

// CriticalityDiagnostics ::= SEQUENCE
struct crit_diagnostics_s {
  bool                               proc_code_present      = false;
  bool                               trigger_msg_present    = false;
  bool                               proc_crit_present      = false;
  bool                               transaction_id_present = false;
  bool                               ie_exts_present        = false;
  uint16_t                           proc_code              = 0;
  trigger_msg_e                      trigger_msg;
  crit_e                             proc_crit;
  uint16_t                           transaction_id = 0;
  crit_diagnostics_ie_list_l         ies_crit_diagnostics;
  crit_diagnostics_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DAPS-HO-Status ::= ENUMERATED
struct daps_ho_status_opts {
  enum options { initiation, /*...*/ nulltype } value;

  const char* to_string() const;
};
using daps_ho_status_e = enumerated<daps_ho_status_opts, true>;

// DCBasedDuplicationConfigured ::= ENUMERATED
struct dc_based_dupl_cfg_opts {
  enum options { true_value, /*...*/ false_value, nulltype } value;

  const char* to_string() const;
};
using dc_based_dupl_cfg_e = enumerated<dc_based_dupl_cfg_opts, true, 1>;

// DL-PRSBWAggregationRequestInfo-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_prsbw_aggregation_request_info_item_ext_ies_o = protocol_ext_empty_o;

using dl_prsbw_aggregation_request_info_item_ext_ies_container = protocol_ext_container_empty_l;

// DL-PRSBWAggregationRequestInfo-Item ::= SEQUENCE
struct dl_prsbw_aggregation_request_info_item_s {
  bool                                                     ie_exts_present    = false;
  uint8_t                                                  dl_prs_res_set_idx = 1;
  dl_prsbw_aggregation_request_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DL-PRSBWAggregationRequestInfo-List ::= SEQUENCE (SIZE (2..3)) OF DL-PRSBWAggregationRequestInfo-Item
using dl_prsbw_aggregation_request_info_list_l = dyn_array<dl_prsbw_aggregation_request_info_item_s>;

// DL-PRSMutingPattern-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using dl_prs_muting_pattern_ext_ies_o = protocol_ies_empty_o;

// DL-PRSMutingPattern ::= CHOICE
struct dl_prs_muting_pattern_c {
  struct types_opts {
    enum options { two, four, six, eight, sixteen, thirty_two, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  dl_prs_muting_pattern_c() = default;
  dl_prs_muting_pattern_c(const dl_prs_muting_pattern_c& other);
  dl_prs_muting_pattern_c& operator=(const dl_prs_muting_pattern_c& other);
  ~dl_prs_muting_pattern_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<2, false, true>& two()
  {
    assert_choice_type(types::two, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<2, false, true>>();
  }
  fixed_bitstring<4, false, true>& four()
  {
    assert_choice_type(types::four, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  fixed_bitstring<6, false, true>& six()
  {
    assert_choice_type(types::six, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<6, false, true>>();
  }
  fixed_bitstring<8, false, true>& eight()
  {
    assert_choice_type(types::eight, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  fixed_bitstring<16, false, true>& sixteen()
  {
    assert_choice_type(types::sixteen, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<16, false, true>>();
  }
  fixed_bitstring<32, false, true>& thirty_two()
  {
    assert_choice_type(types::thirty_two, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<32, false, true>>();
  }
  protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "DL-PRSMutingPattern");
    return c.get<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>();
  }
  const fixed_bitstring<2, false, true>& two() const
  {
    assert_choice_type(types::two, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<2, false, true>>();
  }
  const fixed_bitstring<4, false, true>& four() const
  {
    assert_choice_type(types::four, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  const fixed_bitstring<6, false, true>& six() const
  {
    assert_choice_type(types::six, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<6, false, true>>();
  }
  const fixed_bitstring<8, false, true>& eight() const
  {
    assert_choice_type(types::eight, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  const fixed_bitstring<16, false, true>& sixteen() const
  {
    assert_choice_type(types::sixteen, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<16, false, true>>();
  }
  const fixed_bitstring<32, false, true>& thirty_two() const
  {
    assert_choice_type(types::thirty_two, type_, "DL-PRSMutingPattern");
    return c.get<fixed_bitstring<32, false, true>>();
  }
  const protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "DL-PRSMutingPattern");
    return c.get<protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>>();
  }
  fixed_bitstring<2, false, true>&                                 set_two();
  fixed_bitstring<4, false, true>&                                 set_four();
  fixed_bitstring<6, false, true>&                                 set_six();
  fixed_bitstring<8, false, true>&                                 set_eight();
  fixed_bitstring<16, false, true>&                                set_sixteen();
  fixed_bitstring<32, false, true>&                                set_thirty_two();
  protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<32, false, true>, protocol_ie_single_container_s<dl_prs_muting_pattern_ext_ies_o>> c;

  void destroy_();
};

// DL-PRSResourceARPLocation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using dl_prs_res_arp_location_ext_ies_o = protocol_ies_empty_o;

// DL-PRSResourceARPLocation ::= CHOICE
struct dl_prs_res_arp_location_c {
  struct types_opts {
    enum options { relative_geodetic_location, relative_cartesian_location, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  dl_prs_res_arp_location_c() = default;
  dl_prs_res_arp_location_c(const dl_prs_res_arp_location_c& other);
  dl_prs_res_arp_location_c& operator=(const dl_prs_res_arp_location_c& other);
  ~dl_prs_res_arp_location_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  relative_geodetic_location_s& relative_geodetic_location()
  {
    assert_choice_type(types::relative_geodetic_location, type_, "DL-PRSResourceARPLocation");
    return c.get<relative_geodetic_location_s>();
  }
  relative_cartesian_location_s& relative_cartesian_location()
  {
    assert_choice_type(types::relative_cartesian_location, type_, "DL-PRSResourceARPLocation");
    return c.get<relative_cartesian_location_s>();
  }
  protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "DL-PRSResourceARPLocation");
    return c.get<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>();
  }
  const relative_geodetic_location_s& relative_geodetic_location() const
  {
    assert_choice_type(types::relative_geodetic_location, type_, "DL-PRSResourceARPLocation");
    return c.get<relative_geodetic_location_s>();
  }
  const relative_cartesian_location_s& relative_cartesian_location() const
  {
    assert_choice_type(types::relative_cartesian_location, type_, "DL-PRSResourceARPLocation");
    return c.get<relative_cartesian_location_s>();
  }
  const protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "DL-PRSResourceARPLocation");
    return c.get<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>>();
  }
  relative_geodetic_location_s&                                      set_relative_geodetic_location();
  relative_cartesian_location_s&                                     set_relative_cartesian_location();
  protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<dl_prs_res_arp_location_ext_ies_o>,
                  relative_cartesian_location_s,
                  relative_geodetic_location_s>
      c;

  void destroy_();
};

// DL-PRSResourceSetARPLocation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using dl_prs_res_set_arp_location_ext_ies_o = protocol_ies_empty_o;

// DL-PRSResourceSetARPLocation ::= CHOICE
struct dl_prs_res_set_arp_location_c {
  struct types_opts {
    enum options { relative_geodetic_location, relative_cartesian_location, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  dl_prs_res_set_arp_location_c() = default;
  dl_prs_res_set_arp_location_c(const dl_prs_res_set_arp_location_c& other);
  dl_prs_res_set_arp_location_c& operator=(const dl_prs_res_set_arp_location_c& other);
  ~dl_prs_res_set_arp_location_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  relative_geodetic_location_s& relative_geodetic_location()
  {
    assert_choice_type(types::relative_geodetic_location, type_, "DL-PRSResourceSetARPLocation");
    return c.get<relative_geodetic_location_s>();
  }
  relative_cartesian_location_s& relative_cartesian_location()
  {
    assert_choice_type(types::relative_cartesian_location, type_, "DL-PRSResourceSetARPLocation");
    return c.get<relative_cartesian_location_s>();
  }
  protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "DL-PRSResourceSetARPLocation");
    return c.get<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>();
  }
  const relative_geodetic_location_s& relative_geodetic_location() const
  {
    assert_choice_type(types::relative_geodetic_location, type_, "DL-PRSResourceSetARPLocation");
    return c.get<relative_geodetic_location_s>();
  }
  const relative_cartesian_location_s& relative_cartesian_location() const
  {
    assert_choice_type(types::relative_cartesian_location, type_, "DL-PRSResourceSetARPLocation");
    return c.get<relative_cartesian_location_s>();
  }
  const protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "DL-PRSResourceSetARPLocation");
    return c.get<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>>();
  }
  relative_geodetic_location_s&                                          set_relative_geodetic_location();
  relative_cartesian_location_s&                                         set_relative_cartesian_location();
  protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<dl_prs_res_set_arp_location_ext_ies_o>,
                  relative_cartesian_location_s,
                  relative_geodetic_location_s>
      c;

  void destroy_();
};

// DL-UP-TNL-Address-to-Update-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_up_tnl_address_to_upd_list_item_ext_ies_o = protocol_ext_empty_o;

using dl_up_tnl_address_to_upd_list_item_ext_ies_container = protocol_ext_container_empty_l;

// DL-UP-TNL-Address-to-Update-List-Item ::= SEQUENCE
struct dl_up_tnl_address_to_upd_list_item_s {
  bool                                                 ie_exts_present = false;
  bounded_bitstring<1, 160, true, true>                old_ip_adress;
  bounded_bitstring<1, 160, true, true>                new_ip_adress;
  dl_up_tnl_address_to_upd_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLLBTFailureInformationList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_lbt_fail_info_list_item_ext_ies_o = protocol_ext_empty_o;

using dl_lbt_fail_info_list_item_ext_ies_container = protocol_ext_container_empty_l;

// DLLBTFailureInformationList-Item ::= SEQUENCE
struct dl_lbt_fail_info_list_item_s {
  bool                                         nof_dl_lbt_fails_present = false;
  bool                                         ie_exts_present          = false;
  uint64_t                                     ue_assistant_id          = 0;
  uint16_t                                     nof_dl_lbt_fails         = 1;
  dl_lbt_fail_info_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLLBTFailureInformationList ::= SEQUENCE (SIZE (1..64)) OF DLLBTFailureInformationList-Item
using dl_lbt_fail_info_list_l = dyn_array<dl_lbt_fail_info_list_item_s>;

// DLLBTFailureInformationRequest ::= ENUMERATED
struct dl_lbt_fail_info_request_opts {
  enum options { inquiry, /*...*/ nulltype } value;

  const char* to_string() const;
};
using dl_lbt_fail_info_request_e = enumerated<dl_lbt_fail_info_request_opts, true>;

// DLPRSResource-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_prs_res_item_ext_ies_o = protocol_ext_empty_o;

// DLPRSResourceARP-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_prs_res_arp_ext_ies_o = protocol_ext_empty_o;

using dl_prs_res_arp_ext_ies_container = protocol_ext_container_empty_l;

// DLPRSResourceARP ::= SEQUENCE
struct dl_prs_res_arp_s {
  bool                             ie_exts_present = false;
  uint8_t                          dl_prs_res_id   = 0;
  dl_prs_res_arp_location_c        dl_prs_res_arp_location;
  dl_prs_res_arp_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLPRSResourceSetARP-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_prs_res_set_arp_ext_ies_o = protocol_ext_empty_o;

using dl_prs_res_set_arp_ext_ies_container = protocol_ext_container_empty_l;

// DLPRSResourceSetARP ::= SEQUENCE
struct dl_prs_res_set_arp_s {
  using listof_dl_prs_res_arp_l_ = dyn_array<dl_prs_res_arp_s>;

  // member variables
  bool                                 ie_exts_present   = false;
  uint8_t                              dl_prs_res_set_id = 0;
  dl_prs_res_set_arp_location_c        dl_prs_res_set_arp_location;
  listof_dl_prs_res_arp_l_             listof_dl_prs_res_arp;
  dl_prs_res_set_arp_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLPRSResourceCoordinates-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_prs_res_coordinates_ext_ies_o = protocol_ext_empty_o;

using dl_prs_res_coordinates_ext_ies_container = protocol_ext_container_empty_l;

// DLPRSResourceCoordinates ::= SEQUENCE
struct dl_prs_res_coordinates_s {
  using listof_dl_prs_res_set_arp_l_ = dyn_array<dl_prs_res_set_arp_s>;

  // member variables
  bool                                     ie_exts_present = false;
  listof_dl_prs_res_set_arp_l_             listof_dl_prs_res_set_arp;
  dl_prs_res_coordinates_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

using dl_prs_res_item_ext_ies_container = protocol_ext_container_empty_l;

// DLPRSResourceID-Item ::= SEQUENCE
struct dl_prs_res_id_item_s {
  bool                              ie_exts_present = false;
  uint8_t                           dl_prs_res_id   = 0;
  dl_prs_res_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLUPTNLInformation-ToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dl_up_tnl_info_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using dl_up_tnl_info_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// DLUPTNLInformation-ToBeSetup-Item ::= SEQUENCE
struct dl_up_tnl_info_to_be_setup_item_s {
  bool                                              ie_exts_present = false;
  up_transport_layer_info_c                         dl_up_tnl_info;
  dl_up_tnl_info_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLUPTNLInformation-ToBeSetup-List ::= SEQUENCE (SIZE (1..2)) OF DLUPTNLInformation-ToBeSetup-Item
using dl_up_tnl_info_to_be_setup_list_l = dyn_array<dl_up_tnl_info_to_be_setup_item_s>;

// DRB-Activity ::= ENUMERATED
struct drb_activity_opts {
  enum options { active, not_active, nulltype } value;

  const char* to_string() const;
};
using drb_activity_e = enumerated<drb_activity_opts>;

// DRB-Activity-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using drb_activity_item_ext_ies_o = protocol_ext_empty_o;

using drb_activity_item_ext_ies_container = protocol_ext_container_empty_l;

// DRB-Activity-Item ::= SEQUENCE
struct drb_activity_item_s {
  bool                                drb_activity_present = false;
  bool                                ie_exts_present      = false;
  uint8_t                             drb_id               = 1;
  drb_activity_e                      drb_activity;
  drb_activity_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NotificationControl ::= ENUMERATED
struct notif_ctrl_opts {
  enum options { active, not_active, /*...*/ nulltype } value;

  const char* to_string() const;
};
using notif_ctrl_e = enumerated<notif_ctrl_opts, true>;

// QoSFlowMappingIndication ::= ENUMERATED
struct qos_flow_map_ind_opts {
  enum options { ul, dl, /*...*/ nulltype } value;

  const char* to_string() const;
};
using qos_flow_map_ind_e = enumerated<qos_flow_map_ind_opts, true>;

// PeriodicityBound-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using periodicity_bound_ext_ies_o = protocol_ext_empty_o;

using periodicity_bound_ext_ies_container = protocol_ext_container_empty_l;

// PeriodicityBound ::= SEQUENCE
struct periodicity_bound_s {
  bool                                ie_exts_present         = false;
  uint32_t                            periodicity_lower_bound = 0;
  uint32_t                            periodicity_upper_bound = 0;
  periodicity_bound_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PeriodicityRange-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using periodicity_range_ext_ies_o = protocol_ies_empty_o;

// PeriodicityRange ::= CHOICE
struct periodicity_range_c {
  struct types_opts {
    enum options { periodicity_bound, periodicity_list, choice_exts, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  periodicity_range_c() = default;
  periodicity_range_c(const periodicity_range_c& other);
  periodicity_range_c& operator=(const periodicity_range_c& other);
  ~periodicity_range_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  periodicity_bound_s& periodicity_bound()
  {
    assert_choice_type(types::periodicity_bound, type_, "PeriodicityRange");
    return c.get<periodicity_bound_s>();
  }
  allowed_periodicity_list_l& periodicity_list()
  {
    assert_choice_type(types::periodicity_list, type_, "PeriodicityRange");
    return c.get<allowed_periodicity_list_l>();
  }
  protocol_ie_single_container_s<periodicity_range_ext_ies_o>& choice_exts()
  {
    assert_choice_type(types::choice_exts, type_, "PeriodicityRange");
    return c.get<protocol_ie_single_container_s<periodicity_range_ext_ies_o>>();
  }
  const periodicity_bound_s& periodicity_bound() const
  {
    assert_choice_type(types::periodicity_bound, type_, "PeriodicityRange");
    return c.get<periodicity_bound_s>();
  }
  const allowed_periodicity_list_l& periodicity_list() const
  {
    assert_choice_type(types::periodicity_list, type_, "PeriodicityRange");
    return c.get<allowed_periodicity_list_l>();
  }
  const protocol_ie_single_container_s<periodicity_range_ext_ies_o>& choice_exts() const
  {
    assert_choice_type(types::choice_exts, type_, "PeriodicityRange");
    return c.get<protocol_ie_single_container_s<periodicity_range_ext_ies_o>>();
  }
  periodicity_bound_s&                                         set_periodicity_bound();
  allowed_periodicity_list_l&                                  set_periodicity_list();
  protocol_ie_single_container_s<periodicity_range_ext_ies_o>& set_choice_exts();

private:
  types type_;
  choice_buffer_t<allowed_periodicity_list_l,
                  periodicity_bound_s,
                  protocol_ie_single_container_s<periodicity_range_ext_ies_o>>
      c;

  void destroy_();
};

// RANfeedbacktype-proactive-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ra_nfeedbacktype_proactive_ext_ies_o = protocol_ext_empty_o;

using ra_nfeedbacktype_proactive_ext_ies_container = protocol_ext_container_empty_l;

// RANfeedbacktype-proactive ::= SEQUENCE
struct ra_nfeedbacktype_proactive_s {
  bool                                         periodicity_range_present = false;
  bool                                         ie_ext_present            = false;
  burst_arrival_time_win_s                     burst_arrival_time_win;
  periodicity_range_c                          periodicity_range;
  ra_nfeedbacktype_proactive_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfeedbacktype-reactive-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ra_nfeedbacktype_reactive_ext_ies_o = protocol_ext_empty_o;

using ra_nfeedbacktype_reactive_ext_ies_container = protocol_ext_container_empty_l;

// RANfeedbacktype-reactive ::= SEQUENCE
struct ra_nfeedbacktype_reactive_s {
  struct cap_for_bat_adaptation_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using cap_for_bat_adaptation_e_ = enumerated<cap_for_bat_adaptation_opts, true>;

  // member variables
  bool                                        ie_ext_present = false;
  cap_for_bat_adaptation_e_                   cap_for_bat_adaptation;
  ra_nfeedbacktype_reactive_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfeedbacktype-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using ra_nfeedbacktype_ext_ies_o = protocol_ies_empty_o;

// RANfeedbacktype ::= CHOICE
struct ra_nfeedbacktype_c {
  struct types_opts {
    enum options { proactive, reactive, choice_exts, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ra_nfeedbacktype_c() = default;
  ra_nfeedbacktype_c(const ra_nfeedbacktype_c& other);
  ra_nfeedbacktype_c& operator=(const ra_nfeedbacktype_c& other);
  ~ra_nfeedbacktype_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ra_nfeedbacktype_proactive_s& proactive()
  {
    assert_choice_type(types::proactive, type_, "RANfeedbacktype");
    return c.get<ra_nfeedbacktype_proactive_s>();
  }
  ra_nfeedbacktype_reactive_s& reactive()
  {
    assert_choice_type(types::reactive, type_, "RANfeedbacktype");
    return c.get<ra_nfeedbacktype_reactive_s>();
  }
  protocol_ie_single_container_s<ra_nfeedbacktype_ext_ies_o>& choice_exts()
  {
    assert_choice_type(types::choice_exts, type_, "RANfeedbacktype");
    return c.get<protocol_ie_single_container_s<ra_nfeedbacktype_ext_ies_o>>();
  }
  const ra_nfeedbacktype_proactive_s& proactive() const
  {
    assert_choice_type(types::proactive, type_, "RANfeedbacktype");
    return c.get<ra_nfeedbacktype_proactive_s>();
  }
  const ra_nfeedbacktype_reactive_s& reactive() const
  {
    assert_choice_type(types::reactive, type_, "RANfeedbacktype");
    return c.get<ra_nfeedbacktype_reactive_s>();
  }
  const protocol_ie_single_container_s<ra_nfeedbacktype_ext_ies_o>& choice_exts() const
  {
    assert_choice_type(types::choice_exts, type_, "RANfeedbacktype");
    return c.get<protocol_ie_single_container_s<ra_nfeedbacktype_ext_ies_o>>();
  }
  ra_nfeedbacktype_proactive_s&                               set_proactive();
  ra_nfeedbacktype_reactive_s&                                set_reactive();
  protocol_ie_single_container_s<ra_nfeedbacktype_ext_ies_o>& set_choice_exts();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<ra_nfeedbacktype_ext_ies_o>,
                  ra_nfeedbacktype_proactive_s,
                  ra_nfeedbacktype_reactive_s>
      c;

  void destroy_();
};

// N6JitterInformationExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using n6_jitter_info_ext_ies_o = protocol_ext_empty_o;

using n6_jitter_info_ext_ies_container = protocol_ext_container_empty_l;

// N6JitterInformation ::= SEQUENCE
struct n6_jitter_info_s {
  bool                             ie_exts_present       = false;
  int16_t                          n6_jitter_lower_bound = -127;
  int16_t                          n6_jitter_upper_bound = -127;
  n6_jitter_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TSCAssistanceInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct tsc_assist_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct tsc_assist_info_ext_ies_container {
  bool               survival_time_present    = false;
  bool               ra_nfeedbacktype_present = false;
  bool               n6_jitter_info_present   = false;
  uint32_t           survival_time;
  ra_nfeedbacktype_c ra_nfeedbacktype;
  n6_jitter_info_s   n6_jitter_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TSCAssistanceInformation ::= SEQUENCE
struct tsc_assist_info_s {
  bool                              ie_exts_present = false;
  uint32_t                          periodicity     = 0;
  unbounded_octstring<true>         burst_arrival_time;
  tsc_assist_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TSCTrafficCharacteristics-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tsc_traffic_characteristics_ext_ies_o = protocol_ext_empty_o;

using tsc_traffic_characteristics_ext_ies_container = protocol_ext_container_empty_l;

// TSCTrafficCharacteristics ::= SEQUENCE
struct tsc_traffic_characteristics_s {
  bool                                          tsc_assist_info_dl_present = false;
  bool                                          tsc_assist_info_ul_present = false;
  bool                                          ie_exts_present            = false;
  tsc_assist_info_s                             tsc_assist_info_dl;
  tsc_assist_info_s                             tsc_assist_info_ul;
  tsc_traffic_characteristics_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Flows-Mapped-To-DRB-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct flows_mapped_to_drb_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct flows_mapped_to_drb_item_ext_ies_container {
  bool                          qos_flow_map_ind_present            = false;
  bool                          tsc_traffic_characteristics_present = false;
  qos_flow_map_ind_e            qos_flow_map_ind;
  tsc_traffic_characteristics_s tsc_traffic_characteristics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Flows-Mapped-To-DRB-Item ::= SEQUENCE
struct flows_mapped_to_drb_item_s {
  bool                                       ie_exts_present = false;
  uint8_t                                    qos_flow_id     = 0;
  qos_flow_level_qos_params_s                qos_flow_level_qos_params;
  flows_mapped_to_drb_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Flows-Mapped-To-DRB-List ::= SEQUENCE (SIZE (1..64)) OF Flows-Mapped-To-DRB-Item
using flows_mapped_to_drb_list_l = dyn_array<flows_mapped_to_drb_item_s>;

// ECNmarkingRequest ::= ENUMERATED
struct ec_nmarking_request_opts {
  enum options { ul, dl, both, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ec_nmarking_request_e = enumerated<ec_nmarking_request_opts, true>;

// ECNMarkingorCongestionInformationReportingRequest-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using ecn_markingor_congestion_info_report_request_ext_ies_o = protocol_ies_empty_o;

// ECNMarkingorCongestionInformationReportingRequest ::= CHOICE
struct ecn_markingor_congestion_info_report_request_c {
  struct types_opts {
    enum options { ecn_marking, congestion_info, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ecn_markingor_congestion_info_report_request_c() = default;
  ecn_markingor_congestion_info_report_request_c(const ecn_markingor_congestion_info_report_request_c& other);
  ecn_markingor_congestion_info_report_request_c&
  operator=(const ecn_markingor_congestion_info_report_request_c& other);
  ~ecn_markingor_congestion_info_report_request_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ec_nmarking_request_e& ecn_marking()
  {
    assert_choice_type(types::ecn_marking, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<ec_nmarking_request_e>();
  }
  congestion_info_request_e& congestion_info()
  {
    assert_choice_type(types::congestion_info, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<congestion_info_request_e>();
  }
  protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>();
  }
  const ec_nmarking_request_e& ecn_marking() const
  {
    assert_choice_type(types::ecn_marking, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<ec_nmarking_request_e>();
  }
  const congestion_info_request_e& congestion_info() const
  {
    assert_choice_type(types::congestion_info, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<congestion_info_request_e>();
  }
  const protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>();
  }
  ec_nmarking_request_e&                                                                  set_ecn_marking();
  congestion_info_request_e&                                                              set_congestion_info();
  protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                   type_;
  choice_buffer_t<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>> c;

  void destroy_();
};

// PSIbasedSDUdiscardUL ::= ENUMERATED
struct psi_based_sdu_discard_ul_opts {
  enum options { start, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using psi_based_sdu_discard_ul_e = enumerated<psi_based_sdu_discard_ul_opts, true>;

// DRB-Information-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drb_info_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drb_info_item_ext_ies_container {
  bool                                           ecn_markingor_congestion_info_report_request_present = false;
  bool                                           psi_based_sdu_discard_ul_present                     = false;
  ecn_markingor_congestion_info_report_request_c ecn_markingor_congestion_info_report_request;
  psi_based_sdu_discard_ul_e                     psi_based_sdu_discard_ul;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRB-Information ::= SEQUENCE
struct drb_info_s {
  bool                            notif_ctrl_present = false;
  bool                            ie_exts_present    = false;
  qos_flow_level_qos_params_s     drb_qos;
  snssai_s                        snssai;
  notif_ctrl_e                    notif_ctrl;
  flows_mapped_to_drb_list_l      flows_mapped_to_drb_list;
  drb_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRB-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using drb_list_item_ext_ies_o = protocol_ext_empty_o;

using drb_list_item_ext_ies_container = protocol_ext_container_empty_l;

// DRB-List-Item ::= SEQUENCE
struct drb_list_item_s {
  bool                            ie_exts_present = false;
  uint8_t                         drb_id          = 1;
  drb_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRB-List ::= SEQUENCE (SIZE (1..64)) OF DRB-List-Item
using drb_list_l = dyn_array<drb_list_item_s>;

// Notification-Cause ::= ENUMERATED
struct notif_cause_opts {
  enum options { fulfilled, not_fulfilled, /*...*/ nulltype } value;

  const char* to_string() const;
};
using notif_cause_e = enumerated<notif_cause_opts, true>;

// TSCFeedbackInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tsc_feedback_info_ext_ies_o = protocol_ext_empty_o;

using tsc_feedback_info_ext_ies_container = protocol_ext_container_empty_l;

// TSCFeedbackInformation ::= SEQUENCE
struct tsc_feedback_info_s {
  bool                                adjusted_periodicity_present = false;
  bool                                ie_exts_present              = false;
  int32_t                             burst_arrival_time_offset    = -640000;
  uint32_t                            adjusted_periodicity         = 0;
  tsc_feedback_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TSCTrafficCharacteristicsFeedback-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tsc_traffic_characteristics_feedback_ext_ies_o = protocol_ext_empty_o;

using tsc_traffic_characteristics_feedback_ext_ies_container = protocol_ext_container_empty_l;

// TSCTrafficCharacteristicsFeedback ::= SEQUENCE
struct tsc_traffic_characteristics_feedback_s {
  bool                                                   tsc_feedback_info_dl_present = false;
  bool                                                   tsc_feedback_info_ul_present = false;
  bool                                                   ie_exts_present              = false;
  tsc_feedback_info_s                                    tsc_feedback_info_dl;
  tsc_feedback_info_s                                    tsc_feedback_info_ul;
  tsc_traffic_characteristics_feedback_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRB-Notify-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drb_notify_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drb_notify_item_ext_ies_container {
  bool                                   current_qos_para_set_idx_present             = false;
  bool                                   tsc_traffic_characteristics_feedback_present = false;
  uint8_t                                current_qos_para_set_idx;
  tsc_traffic_characteristics_feedback_s tsc_traffic_characteristics_feedback;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRB-Notify-Item ::= SEQUENCE
struct drb_notify_item_s {
  bool                              ie_exts_present = false;
  uint8_t                           drb_id          = 1;
  notif_cause_e                     notif_cause;
  drb_notify_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-FailedToBeModified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using drbs_failed_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using drbs_failed_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// DRBs-FailedToBeModified-Item ::= SEQUENCE
struct drbs_failed_to_be_modified_item_s {
  bool                                              cause_present   = false;
  bool                                              ie_exts_present = false;
  uint8_t                                           drb_id          = 1;
  cause_c                                           cause;
  drbs_failed_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-FailedToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using drbs_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using drbs_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// DRBs-FailedToBeSetup-Item ::= SEQUENCE
struct drbs_failed_to_be_setup_item_s {
  bool                                           cause_present   = false;
  bool                                           ie_exts_present = false;
  uint8_t                                        drb_id          = 1;
  cause_c                                        cause;
  drbs_failed_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-FailedToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using drbs_failed_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using drbs_failed_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// DRBs-FailedToBeSetupMod-Item ::= SEQUENCE
struct drbs_failed_to_be_setup_mod_item_s {
  bool                                               cause_present   = false;
  bool                                               ie_exts_present = false;
  uint8_t                                            drb_id          = 1;
  cause_c                                            cause;
  drbs_failed_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Reestablishment-Indication ::= ENUMERATED
struct reest_ind_opts {
  enum options { reestablished, /*...*/ nulltype } value;

  const char* to_string() const;
};
using reest_ind_e = enumerated<reest_ind_opts, true>;

// RLC-Status-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rlc_status_ext_ies_o = protocol_ext_empty_o;

using rlc_status_ext_ies_container = protocol_ext_container_empty_l;

// RLC-Status ::= SEQUENCE
struct rlc_status_s {
  bool                         ie_exts_present = false;
  reest_ind_e                  reest_ind;
  rlc_status_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ECNMarkingorCongestionInformationReportingStatus ::= ENUMERATED
struct ecn_markingor_congestion_info_report_status_opts {
  enum options { active, not_active, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ecn_markingor_congestion_info_report_status_e =
    enumerated<ecn_markingor_congestion_info_report_status_opts, true>;

// DRBs-Modified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drbs_modified_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_modified_item_ext_ies_container {
  bool                                          rlc_status_present                                  = false;
  bool                                          add_pdcp_dupl_tnl_list_present                      = false;
  bool                                          current_qos_para_set_idx_present                    = false;
  bool                                          tsc_traffic_characteristics_feedback_present        = false;
  bool                                          ecn_markingor_congestion_info_report_status_present = false;
  rlc_status_s                                  rlc_status;
  add_pdcp_dupl_tnl_list_l                      add_pdcp_dupl_tnl_list;
  uint8_t                                       current_qos_para_set_idx;
  tsc_traffic_characteristics_feedback_s        tsc_traffic_characteristics_feedback;
  ecn_markingor_congestion_info_report_status_e ecn_markingor_congestion_info_report_status;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-Modified-Item ::= SEQUENCE
struct drbs_modified_item_s {
  bool                                 lcid_present    = false;
  bool                                 ie_exts_present = false;
  uint8_t                              drb_id          = 1;
  uint8_t                              lcid            = 1;
  dl_up_tnl_info_to_be_setup_list_l    dl_up_tnl_info_to_be_setup_list;
  drbs_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ULUPTNLInformation-ToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct ul_up_tnl_info_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ul_up_tnl_info_to_be_setup_item_ext_ies_container {
  bool      bh_info_present      = false;
  bool      drb_map_info_present = false;
  bh_info_s bh_info;
  uint8_t   drb_map_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ULUPTNLInformation-ToBeSetup-Item ::= SEQUENCE
struct ul_up_tnl_info_to_be_setup_item_s {
  bool                                              ie_exts_present = false;
  up_transport_layer_info_c                         ul_up_tnl_info;
  ul_up_tnl_info_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ULUPTNLInformation-ToBeSetup-List ::= SEQUENCE (SIZE (1..2)) OF ULUPTNLInformation-ToBeSetup-Item
using ul_up_tnl_info_to_be_setup_list_l = dyn_array<ul_up_tnl_info_to_be_setup_item_s>;

// DRBs-ModifiedConf-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drbs_modified_conf_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_modified_conf_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-ModifiedConf-Item ::= SEQUENCE
struct drbs_modified_conf_item_s {
  bool                                      ie_exts_present = false;
  uint8_t                                   drb_id          = 1;
  ul_up_tnl_info_to_be_setup_list_l         ul_up_tnl_info_to_be_setup_list;
  drbs_modified_conf_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-Required-ToBeModified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drbs_required_to_be_modified_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_required_to_be_modified_item_ext_ies_container {
  bool                     rlc_status_present             = false;
  bool                     add_pdcp_dupl_tnl_list_present = false;
  rlc_status_s             rlc_status;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-Required-ToBeModified-Item ::= SEQUENCE
struct drbs_required_to_be_modified_item_s {
  bool                                                ie_exts_present = false;
  uint8_t                                             drb_id          = 1;
  dl_up_tnl_info_to_be_setup_list_l                   dl_up_tnl_info_to_be_setup_list;
  drbs_required_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-Required-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using drbs_required_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using drbs_required_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// DRBs-Required-ToBeReleased-Item ::= SEQUENCE
struct drbs_required_to_be_released_item_s {
  bool                                                ie_exts_present = false;
  uint8_t                                             drb_id          = 1;
  drbs_required_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-Setup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drbs_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_setup_item_ext_ies_container {
  bool                                          add_pdcp_dupl_tnl_list_present                      = false;
  bool                                          current_qos_para_set_idx_present                    = false;
  bool                                          tsc_traffic_characteristics_feedback_present        = false;
  bool                                          ecn_markingor_congestion_info_report_status_present = false;
  add_pdcp_dupl_tnl_list_l                      add_pdcp_dupl_tnl_list;
  uint8_t                                       current_qos_para_set_idx;
  tsc_traffic_characteristics_feedback_s        tsc_traffic_characteristics_feedback;
  ecn_markingor_congestion_info_report_status_e ecn_markingor_congestion_info_report_status;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-Setup-Item ::= SEQUENCE
struct drbs_setup_item_s {
  bool                              lcid_present    = false;
  bool                              ie_exts_present = false;
  uint8_t                           drb_id          = 1;
  uint8_t                           lcid            = 1;
  dl_up_tnl_info_to_be_setup_list_l dl_up_tnl_info_to_be_setup_list;
  drbs_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-SetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drbs_setup_mod_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_setup_mod_item_ext_ies_container {
  bool                                          add_pdcp_dupl_tnl_list_present                      = false;
  bool                                          current_qos_para_set_idx_present                    = false;
  bool                                          tsc_traffic_characteristics_feedback_present        = false;
  bool                                          ecn_markingor_congestion_info_report_status_present = false;
  add_pdcp_dupl_tnl_list_l                      add_pdcp_dupl_tnl_list;
  uint8_t                                       current_qos_para_set_idx;
  tsc_traffic_characteristics_feedback_s        tsc_traffic_characteristics_feedback;
  ecn_markingor_congestion_info_report_status_e ecn_markingor_congestion_info_report_status;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-SetupMod-Item ::= SEQUENCE
struct drbs_setup_mod_item_s {
  bool                                  lcid_present    = false;
  bool                                  ie_exts_present = false;
  uint8_t                               drb_id          = 1;
  uint8_t                               lcid            = 1;
  dl_up_tnl_info_to_be_setup_list_l     dl_up_tnl_info_to_be_setup_list;
  drbs_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct qos_info_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { drb_info, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::drb_info; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    drb_info_s&       drb_info() { return c; }
    const drb_info_s& drb_info() const { return c; }

  private:
    drb_info_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// QoSInformation ::= CHOICE
struct qos_info_c {
  struct types_opts {
    enum options { eutran_qos, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  qos_info_c() = default;
  qos_info_c(const qos_info_c& other);
  qos_info_c& operator=(const qos_info_c& other);
  ~qos_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  eutran_qos_s& eutran_qos()
  {
    assert_choice_type(types::eutran_qos, type_, "QoSInformation");
    return c.get<eutran_qos_s>();
  }
  protocol_ie_single_container_s<qos_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "QoSInformation");
    return c.get<protocol_ie_single_container_s<qos_info_ext_ies_o>>();
  }
  const eutran_qos_s& eutran_qos() const
  {
    assert_choice_type(types::eutran_qos, type_, "QoSInformation");
    return c.get<eutran_qos_s>();
  }
  const protocol_ie_single_container_s<qos_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "QoSInformation");
    return c.get<protocol_ie_single_container_s<qos_info_ext_ies_o>>();
  }
  eutran_qos_s&                                       set_eutran_qos();
  protocol_ie_single_container_s<qos_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                             type_;
  choice_buffer_t<eutran_qos_s, protocol_ie_single_container_s<qos_info_ext_ies_o>> c;

  void destroy_();
};

// ULUEConfiguration ::= ENUMERATED
struct ul_ue_cfg_opts {
  enum options { no_data, shared, only, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ul_ue_cfg_e = enumerated<ul_ue_cfg_opts, true>;

// ULConfigurationExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ul_cfg_ext_ies_o = protocol_ext_empty_o;

using ul_cfg_ext_ies_container = protocol_ext_container_empty_l;

// ULConfiguration ::= SEQUENCE
struct ul_cfg_s {
  bool                     ie_exts_present = false;
  ul_ue_cfg_e              ul_ue_cfg;
  ul_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDCPSNLength ::= ENUMERATED
struct pdcp_sn_len_opts {
  enum options { twelve_bits, eighteen_bits, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using pdcp_sn_len_e = enumerated<pdcp_sn_len_opts, true>;

// DuplicationActivation ::= ENUMERATED
struct dupl_activation_opts {
  enum options { active, inactive, /*...*/ nulltype } value;

  const char* to_string() const;
};
using dupl_activation_e = enumerated<dupl_activation_opts, true>;

// DuplicationState ::= ENUMERATED
struct dupl_state_opts {
  enum options { active, inactive, /*...*/ nulltype } value;

  const char* to_string() const;
};
using dupl_state_e = enumerated<dupl_state_opts, true>;

// RLCDuplicationState-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rlc_dupl_state_item_ext_ies_o = protocol_ext_empty_o;

using rlc_dupl_state_item_ext_ies_container = protocol_ext_container_empty_l;

// RLCDuplicationState-Item ::= SEQUENCE
struct rlc_dupl_state_item_s {
  bool                                  ie_exts_present = false;
  dupl_state_e                          dupl_state;
  rlc_dupl_state_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RLCDuplicationStateList ::= SEQUENCE (SIZE (1..3)) OF RLCDuplicationState-Item
using rlc_dupl_state_list_l = dyn_array<rlc_dupl_state_item_s>;

// PrimaryPathIndication ::= ENUMERATED
struct primary_path_ind_opts {
  enum options { true_value, false_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using primary_path_ind_e = enumerated<primary_path_ind_opts, true>;

// RLCDuplicationInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rlc_dupl_info_ext_ies_o = protocol_ext_empty_o;

using rlc_dupl_info_ext_ies_container = protocol_ext_container_empty_l;

// RLCDuplicationInformation ::= SEQUENCE
struct rlc_dupl_info_s {
  bool                            primary_path_ind_present = false;
  bool                            ie_exts_present          = false;
  rlc_dupl_state_list_l           rlc_dupl_state_list;
  primary_path_ind_e              primary_path_ind;
  rlc_dupl_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TransmissionStopIndicator ::= ENUMERATED
struct tx_stop_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using tx_stop_ind_e = enumerated<tx_stop_ind_opts, true>;

// DRBs-ToBeModified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drbs_to_be_modified_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_modified_item_ext_ies_container {
  bool                     dl_pdcp_sn_len_present           = false;
  bool                     ul_pdcp_sn_len_present           = false;
  bool                     bearer_type_change_present       = false;
  bool                     rlc_mode_present                 = false;
  bool                     dupl_activation_present          = false;
  bool                     dc_based_dupl_cfg_present        = false;
  bool                     dc_based_dupl_activation_present = false;
  bool                     add_pdcp_dupl_tnl_list_present   = false;
  bool                     rlc_dupl_info_present            = false;
  bool                     tx_stop_ind_present              = false;
  bool                     cg_sd_tind_mod_present           = false;
  pdcp_sn_len_e            dl_pdcp_sn_len;
  pdcp_sn_len_e            ul_pdcp_sn_len;
  bearer_type_change_e     bearer_type_change;
  rlc_mode_e               rlc_mode;
  dupl_activation_e        dupl_activation;
  dc_based_dupl_cfg_e      dc_based_dupl_cfg;
  dupl_activation_e        dc_based_dupl_activation;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s          rlc_dupl_info;
  tx_stop_ind_e            tx_stop_ind;
  cg_sd_tind_mod_e         cg_sd_tind_mod;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-ToBeModified-Item ::= SEQUENCE
struct drbs_to_be_modified_item_s {
  bool                                       qos_info_present = false;
  bool                                       ul_cfg_present   = false;
  bool                                       ie_exts_present  = false;
  uint8_t                                    drb_id           = 1;
  qos_info_c                                 qos_info;
  ul_up_tnl_info_to_be_setup_list_l          ul_up_tnl_info_to_be_setup_list;
  ul_cfg_s                                   ul_cfg;
  drbs_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using drbs_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using drbs_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// DRBs-ToBeReleased-Item ::= SEQUENCE
struct drbs_to_be_released_item_s {
  bool                                       ie_exts_present = false;
  uint8_t                                    drb_id          = 1;
  drbs_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-ToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drbs_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_setup_item_ext_ies_container {
  bool                      dc_based_dupl_cfg_present        = false;
  bool                      dc_based_dupl_activation_present = false;
  bool                      ul_pdcp_sn_len_present           = false;
  bool                      add_pdcp_dupl_tnl_list_present   = false;
  bool                      rlc_dupl_info_present            = false;
  bool                      sdt_rlc_bearer_cfg_present       = false;
  dc_based_dupl_cfg_e       dc_based_dupl_cfg;
  dupl_activation_e         dc_based_dupl_activation;
  pdcp_sn_len_e             dl_pdcp_sn_len;
  pdcp_sn_len_e             ul_pdcp_sn_len;
  add_pdcp_dupl_tnl_list_l  add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s           rlc_dupl_info;
  unbounded_octstring<true> sdt_rlc_bearer_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-ToBeSetup-Item ::= SEQUENCE
struct drbs_to_be_setup_item_s {
  bool                                    ul_cfg_present          = false;
  bool                                    dupl_activation_present = false;
  bool                                    ie_exts_present         = false;
  uint8_t                                 drb_id                  = 1;
  qos_info_c                              qos_info;
  ul_up_tnl_info_to_be_setup_list_l       ul_up_tnl_info_to_be_setup_list;
  rlc_mode_e                              rlc_mode;
  ul_cfg_s                                ul_cfg;
  dupl_activation_e                       dupl_activation;
  drbs_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-ToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct drbs_to_be_setup_mod_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_setup_mod_item_ext_ies_container {
  bool                     dc_based_dupl_cfg_present        = false;
  bool                     dc_based_dupl_activation_present = false;
  bool                     dl_pdcp_sn_len_present           = false;
  bool                     ul_pdcp_sn_len_present           = false;
  bool                     add_pdcp_dupl_tnl_list_present   = false;
  bool                     rlc_dupl_info_present            = false;
  bool                     cg_sd_tind_setup_present         = false;
  dc_based_dupl_cfg_e      dc_based_dupl_cfg;
  dupl_activation_e        dc_based_dupl_activation;
  pdcp_sn_len_e            dl_pdcp_sn_len;
  pdcp_sn_len_e            ul_pdcp_sn_len;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s          rlc_dupl_info;
  cg_sd_tind_setup_e       cg_sd_tind_setup;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBs-ToBeSetupMod-Item ::= SEQUENCE
struct drbs_to_be_setup_mod_item_s {
  bool                                        ul_cfg_present          = false;
  bool                                        dupl_activation_present = false;
  bool                                        ie_exts_present         = false;
  uint8_t                                     drb_id                  = 1;
  qos_info_c                                  qos_info;
  ul_up_tnl_info_to_be_setup_list_l           ul_up_tnl_info_to_be_setup_list;
  rlc_mode_e                                  rlc_mode;
  ul_cfg_s                                    ul_cfg;
  dupl_activation_e                           dupl_activation;
  drbs_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRXConfigurationIndicator ::= ENUMERATED
struct drx_cfg_ind_opts {
  enum options { release, /*...*/ nulltype } value;

  const char* to_string() const;
};
using drx_cfg_ind_e = enumerated<drx_cfg_ind_opts, true>;

// LongDRXCycleLength ::= ENUMERATED
struct long_drx_cycle_len_opts {
  enum options {
    ms10,
    ms20,
    ms32,
    ms40,
    ms60,
    ms64,
    ms70,
    ms80,
    ms128,
    ms160,
    ms256,
    ms320,
    ms512,
    ms640,
    ms1024,
    ms1280,
    ms2048,
    ms2560,
    ms5120,
    ms10240,
    // ...
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using long_drx_cycle_len_e = enumerated<long_drx_cycle_len_opts, true>;

// ShortDRXCycleLength ::= ENUMERATED
struct short_drx_cycle_len_opts {
  enum options {
    ms2,
    ms3,
    ms4,
    ms5,
    ms6,
    ms7,
    ms8,
    ms10,
    ms14,
    ms16,
    ms20,
    ms30,
    ms32,
    ms35,
    ms40,
    ms64,
    ms80,
    ms128,
    ms160,
    ms256,
    ms320,
    ms512,
    ms640,
    // ...
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using short_drx_cycle_len_e = enumerated<short_drx_cycle_len_opts, true>;

// DRXCycle-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using drx_cycle_ext_ies_o = protocol_ext_empty_o;

using drx_cycle_ext_ies_container = protocol_ext_container_empty_l;

// DRXCycle ::= SEQUENCE
struct drx_cycle_s {
  bool                        short_drx_cycle_len_present   = false;
  bool                        short_drx_cycle_timer_present = false;
  bool                        ie_exts_present               = false;
  long_drx_cycle_len_e        long_drx_cycle_len;
  short_drx_cycle_len_e       short_drx_cycle_len;
  uint8_t                     short_drx_cycle_timer = 1;
  drx_cycle_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DUCURIMInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using du_cu_rim_info_ext_ies_o = protocol_ext_empty_o;

using du_cu_rim_info_ext_ies_container = protocol_ext_container_empty_l;

// DUCURIMInformation ::= SEQUENCE
struct du_cu_rim_info_s {
  bool                             ie_exts_present = false;
  fixed_bitstring<22, false, true> victim_gnb_set_id;
  rimrs_detection_status_e         rimrs_detection_status;
  aggressor_cell_list_l            aggressor_cell_list;
  du_cu_rim_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DUCURadioInformationType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using du_cu_radio_info_type_ext_ies_o = protocol_ies_empty_o;

// DUCURadioInformationType ::= CHOICE
struct du_cu_radio_info_type_c {
  struct types_opts {
    enum options { rim, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  du_cu_radio_info_type_c() = default;
  du_cu_radio_info_type_c(const du_cu_radio_info_type_c& other);
  du_cu_radio_info_type_c& operator=(const du_cu_radio_info_type_c& other);
  ~du_cu_radio_info_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  du_cu_rim_info_s& rim()
  {
    assert_choice_type(types::rim, type_, "DUCURadioInformationType");
    return c.get<du_cu_rim_info_s>();
  }
  protocol_ie_single_container_s<du_cu_radio_info_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "DUCURadioInformationType");
    return c.get<protocol_ie_single_container_s<du_cu_radio_info_type_ext_ies_o>>();
  }
  const du_cu_rim_info_s& rim() const
  {
    assert_choice_type(types::rim, type_, "DUCURadioInformationType");
    return c.get<du_cu_rim_info_s>();
  }
  const protocol_ie_single_container_s<du_cu_radio_info_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "DUCURadioInformationType");
    return c.get<protocol_ie_single_container_s<du_cu_radio_info_type_ext_ies_o>>();
  }
  du_cu_rim_info_s&                                                set_rim();
  protocol_ie_single_container_s<du_cu_radio_info_type_ext_ies_o>& set_choice_ext();

private:
  types                                                                                              type_;
  choice_buffer_t<du_cu_rim_info_s, protocol_ie_single_container_s<du_cu_radio_info_type_ext_ies_o>> c;

  void destroy_();
};

// InterFrequencyConfig-NoGap ::= ENUMERATED
struct interfreq_cfg_no_gap_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using interfreq_cfg_no_gap_e = enumerated<interfreq_cfg_no_gap_opts, true>;

// TwoPHRModeMCG ::= ENUMERATED
struct two_phr_mode_mcg_opts {
  enum options { enabled, /*...*/ nulltype } value;

  const char* to_string() const;
};
using two_phr_mode_mcg_e = enumerated<two_phr_mode_mcg_opts, true>;

// TwoPHRModeSCG ::= ENUMERATED
struct two_phr_mode_scg_opts {
  enum options { enabled, /*...*/ nulltype } value;

  const char* to_string() const;
};
using two_phr_mode_scg_e = enumerated<two_phr_mode_scg_opts, true>;

// DUtoCURRCInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct du_to_cu_rrc_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct du_to_cu_rrc_info_ext_ies_container {
  bool                      drx_long_cycle_start_offset_present         = false;
  bool                      sel_band_combination_idx_present            = false;
  bool                      sel_feature_set_entry_idx_present           = false;
  bool                      ph_info_scg_present                         = false;
  bool                      requested_band_combination_idx_present      = false;
  bool                      requested_feature_set_entry_idx_present     = false;
  bool                      drx_cfg_present                             = false;
  bool                      pdcch_blind_detection_scg_present           = false;
  bool                      requested_pdcch_blind_detection_scg_present = false;
  bool                      ph_info_mcg_present                         = false;
  bool                      meas_gap_sharing_cfg_present                = false;
  bool                      sl_phy_mac_rlc_cfg_present                  = false;
  bool                      sl_cfg_ded_eutra_info_present               = false;
  bool                      requested_p_max_fr2_present                 = false;
  bool                      sdt_mac_phy_cg_cfg_present                  = false;
  bool                      mu_si_m_gap_cfg_present                     = false;
  bool                      sl_rlc_ch_to_add_mod_list_present           = false;
  bool                      interfreq_cfg_no_gap_present                = false;
  bool                      ul_gap_fr2_cfg_present                      = false;
  bool                      two_phr_mode_mcg_present                    = false;
  bool                      two_phr_mode_scg_present                    = false;
  bool                      ncd_ssb_red_cap_init_bwp_sdt_present        = false;
  bool                      serv_cell_info_list_present                 = false;
  bool                      sl_phy_mac_rlc_cfg_ext_present              = false;
  uint16_t                  drx_long_cycle_start_offset;
  unbounded_octstring<true> sel_band_combination_idx;
  unbounded_octstring<true> sel_feature_set_entry_idx;
  unbounded_octstring<true> ph_info_scg;
  unbounded_octstring<true> requested_band_combination_idx;
  unbounded_octstring<true> requested_feature_set_entry_idx;
  unbounded_octstring<true> drx_cfg;
  unbounded_octstring<true> pdcch_blind_detection_scg;
  unbounded_octstring<true> requested_pdcch_blind_detection_scg;
  unbounded_octstring<true> ph_info_mcg;
  unbounded_octstring<true> meas_gap_sharing_cfg;
  unbounded_octstring<true> sl_phy_mac_rlc_cfg;
  unbounded_octstring<true> sl_cfg_ded_eutra_info;
  unbounded_octstring<true> requested_p_max_fr2;
  unbounded_octstring<true> sdt_mac_phy_cg_cfg;
  unbounded_octstring<true> mu_si_m_gap_cfg;
  unbounded_octstring<true> sl_rlc_ch_to_add_mod_list;
  interfreq_cfg_no_gap_e    interfreq_cfg_no_gap;
  unbounded_octstring<true> ul_gap_fr2_cfg;
  two_phr_mode_mcg_e        two_phr_mode_mcg;
  two_phr_mode_scg_e        two_phr_mode_scg;
  unbounded_octstring<true> ncd_ssb_red_cap_init_bwp_sdt;
  unbounded_octstring<true> serv_cell_info_list;
  unbounded_octstring<true> sl_phy_mac_rlc_cfg_ext;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DUtoCURRCInformation ::= SEQUENCE
struct du_to_cu_rrc_info_s {
  bool                                ie_exts_present = false;
  unbounded_octstring<true>           cell_group_cfg;
  unbounded_octstring<true>           meas_gap_cfg;
  unbounded_octstring<true>           requested_p_max_fr1;
  du_to_cu_rrc_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DUtoCUTAInformation-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using du_to_cu_tai_nformation_item_ext_ies_o = protocol_ext_empty_o;

using du_to_cu_tai_nformation_item_ext_ies_container = protocol_ext_container_empty_l;

// DUtoCUTAInformation-Item ::= SEQUENCE
struct du_to_cu_tai_nformation_item_s {
  bool                                           ie_exts_present = false;
  nr_cgi_s                                       nr_cgi;
  uint16_t                                       ta_value         = 0;
  uint8_t                                        preamb_idx       = 0;
  uint32_t                                       ra_rnti          = 0;
  uint64_t                                       source_gnb_du_id = 0;
  unbounded_octstring<true>                      tag_id_pointer;
  du_to_cu_tai_nformation_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DUtoCUTAInformation-List ::= SEQUENCE (SIZE (1..8)) OF DUtoCUTAInformation-Item
using du_to_cu_tai_nformation_list_l = dyn_array<du_to_cu_tai_nformation_item_s>;

// DeactivationIndicationList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using deactivation_ind_list_item_ext_ies_o = protocol_ext_empty_o;

using deactivation_ind_list_item_ext_ies_container = protocol_ext_container_empty_l;

// DeactivationIndicationList-Item ::= SEQUENCE
struct deactivation_ind_list_item_s {
  bool                                         ie_exts_present   = false;
  uint64_t                                     gnb_cu_ue_f1ap_id = 0;
  uint64_t                                     gnb_du_ue_f1ap_id = 0;
  deactivation_ind_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DeactivationIndicationList ::= SEQUENCE (SIZE (1..512)) OF DeactivationIndicationList-Item
using deactivation_ind_list_l = dyn_array<deactivation_ind_list_item_s>;

// DeactivationIndication-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using deactivation_ind_ext_ies_o = protocol_ies_empty_o;

// DeactivationIndication ::= CHOICE
struct deactivation_ind_c {
  struct types_opts {
    enum options { per_ue, deactiv_all, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  deactivation_ind_c() = default;
  deactivation_ind_c(const deactivation_ind_c& other);
  deactivation_ind_c& operator=(const deactivation_ind_c& other);
  ~deactivation_ind_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  deactivation_ind_list_l& per_ue()
  {
    assert_choice_type(types::per_ue, type_, "DeactivationIndication");
    return c.get<deactivation_ind_list_l>();
  }
  protocol_ie_single_container_s<deactivation_ind_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "DeactivationIndication");
    return c.get<protocol_ie_single_container_s<deactivation_ind_ext_ies_o>>();
  }
  const deactivation_ind_list_l& per_ue() const
  {
    assert_choice_type(types::per_ue, type_, "DeactivationIndication");
    return c.get<deactivation_ind_list_l>();
  }
  const protocol_ie_single_container_s<deactivation_ind_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "DeactivationIndication");
    return c.get<protocol_ie_single_container_s<deactivation_ind_ext_ies_o>>();
  }
  deactivation_ind_list_l&                                    set_per_ue();
  void                                                        set_deactiv_all();
  protocol_ie_single_container_s<deactivation_ind_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                type_;
  choice_buffer_t<deactivation_ind_list_l, protocol_ie_single_container_s<deactivation_ind_ext_ies_o>> c;

  void destroy_();
};

// DedicatedSIDeliveryNeededUE-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ded_si_delivery_needed_ue_item_ext_ies_o = protocol_ext_empty_o;

using ded_si_delivery_needed_ue_item_ext_ies_container = protocol_ext_container_empty_l;

// Dedicated-SIDelivery-NeededUE-Item ::= SEQUENCE
struct ded_si_delivery_needed_ue_item_s {
  bool                                             ie_exts_present   = false;
  uint64_t                                         gnb_cu_ue_f1ap_id = 0;
  nr_cgi_s                                         nr_cgi;
  ded_si_delivery_needed_ue_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DedicatedSIDeliveryIndication ::= ENUMERATED
struct ded_si_delivery_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ded_si_delivery_ind_e = enumerated<ded_si_delivery_ind_opts, true>;

// DuplicationIndication ::= ENUMERATED
struct dupl_ind_opts {
  enum options { true_value, /*...*/ false_value, nulltype } value;

  const char* to_string() const;
};
using dupl_ind_e = enumerated<dupl_ind_opts, true, 1>;

// DynamicPQIDescriptor-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using dyn_pq_id_escriptor_ext_ies_o = protocol_ext_empty_o;

using dyn_pq_id_escriptor_ext_ies_container = protocol_ext_container_empty_l;

// DynamicPQIDescriptor ::= SEQUENCE
struct dyn_pq_id_escriptor_s {
  struct res_type_opts {
    enum options { gbr, non_gbr, delay_crit_grb, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using res_type_e_ = enumerated<res_type_opts, true>;

  // member variables
  bool                                  res_type_present              = false;
  bool                                  averaging_win_present         = false;
  bool                                  max_data_burst_volume_present = false;
  bool                                  ie_exts_present               = false;
  res_type_e_                           res_type;
  uint8_t                               qos_prio_level      = 1;
  uint16_t                              packet_delay_budget = 0;
  packet_error_rate_s                   packet_error_rate;
  uint16_t                              averaging_win         = 0;
  uint16_t                              max_data_burst_volume = 0;
  dyn_pq_id_escriptor_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-MeasuredResults-Value-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct e_c_id_measured_results_value_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { nr_tadv, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::nr_tadv; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint16_t&       nr_tadv() { return c; }
    const uint16_t& nr_tadv() const { return c; }

  private:
    uint16_t c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E-CID-MeasuredResults-Value ::= CHOICE
struct e_c_id_measured_results_value_c {
  struct types_opts {
    enum options { value_angleof_arrival_nr, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  e_c_id_measured_results_value_c() = default;
  e_c_id_measured_results_value_c(const e_c_id_measured_results_value_c& other);
  e_c_id_measured_results_value_c& operator=(const e_c_id_measured_results_value_c& other);
  ~e_c_id_measured_results_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ul_ao_a_s& value_angleof_arrival_nr()
  {
    assert_choice_type(types::value_angleof_arrival_nr, type_, "E-CID-MeasuredResults-Value");
    return c.get<ul_ao_a_s>();
  }
  protocol_ie_single_container_s<e_c_id_measured_results_value_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "E-CID-MeasuredResults-Value");
    return c.get<protocol_ie_single_container_s<e_c_id_measured_results_value_ext_ies_o>>();
  }
  const ul_ao_a_s& value_angleof_arrival_nr() const
  {
    assert_choice_type(types::value_angleof_arrival_nr, type_, "E-CID-MeasuredResults-Value");
    return c.get<ul_ao_a_s>();
  }
  const protocol_ie_single_container_s<e_c_id_measured_results_value_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "E-CID-MeasuredResults-Value");
    return c.get<protocol_ie_single_container_s<e_c_id_measured_results_value_ext_ies_o>>();
  }
  ul_ao_a_s&                                                               set_value_angleof_arrival_nr();
  protocol_ie_single_container_s<e_c_id_measured_results_value_ext_ies_o>& set_choice_ext();

private:
  types                                                                                               type_;
  choice_buffer_t<protocol_ie_single_container_s<e_c_id_measured_results_value_ext_ies_o>, ul_ao_a_s> c;

  void destroy_();
};

// E-CID-MeasuredResults-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using e_c_id_measured_results_item_ext_ies_o = protocol_ext_empty_o;

using e_c_id_measured_results_item_ext_ies_container = protocol_ext_container_empty_l;

// E-CID-MeasuredResults-Item ::= SEQUENCE
struct e_c_id_measured_results_item_s {
  bool                                           ie_exts_present = false;
  e_c_id_measured_results_value_c                e_c_id_measured_results_value;
  e_c_id_measured_results_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-MeasuredResults-List ::= SEQUENCE (SIZE (1..64)) OF E-CID-MeasuredResults-Item
using e_c_id_measured_results_list_l = dyn_array<e_c_id_measured_results_item_s>;

// TimeStampSlotIndex-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct time_stamp_slot_idx_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { scs_480, scs_960, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    value_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint16_t&       scs_480();
    uint16_t&       scs_960();
    const uint16_t& scs_480() const;
    const uint16_t& scs_960() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// TimeStampSlotIndex ::= CHOICE
struct time_stamp_slot_idx_c {
  struct types_opts {
    enum options { scs_15, scs_30, scs_60, scs_120, choice_ext, nulltype } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  time_stamp_slot_idx_c() = default;
  time_stamp_slot_idx_c(const time_stamp_slot_idx_c& other);
  time_stamp_slot_idx_c& operator=(const time_stamp_slot_idx_c& other);
  ~time_stamp_slot_idx_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& scs_15()
  {
    assert_choice_type(types::scs_15, type_, "TimeStampSlotIndex");
    return c.get<uint8_t>();
  }
  uint8_t& scs_30()
  {
    assert_choice_type(types::scs_30, type_, "TimeStampSlotIndex");
    return c.get<uint8_t>();
  }
  uint8_t& scs_60()
  {
    assert_choice_type(types::scs_60, type_, "TimeStampSlotIndex");
    return c.get<uint8_t>();
  }
  uint8_t& scs_120()
  {
    assert_choice_type(types::scs_120, type_, "TimeStampSlotIndex");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TimeStampSlotIndex");
    return c.get<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>();
  }
  const uint8_t& scs_15() const
  {
    assert_choice_type(types::scs_15, type_, "TimeStampSlotIndex");
    return c.get<uint8_t>();
  }
  const uint8_t& scs_30() const
  {
    assert_choice_type(types::scs_30, type_, "TimeStampSlotIndex");
    return c.get<uint8_t>();
  }
  const uint8_t& scs_60() const
  {
    assert_choice_type(types::scs_60, type_, "TimeStampSlotIndex");
    return c.get<uint8_t>();
  }
  const uint8_t& scs_120() const
  {
    assert_choice_type(types::scs_120, type_, "TimeStampSlotIndex");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TimeStampSlotIndex");
    return c.get<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>>();
  }
  uint8_t&                                                       set_scs_15();
  uint8_t&                                                       set_scs_30();
  uint8_t&                                                       set_scs_60();
  uint8_t&                                                       set_scs_120();
  protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>& set_choice_ext();

private:
  types                                                                          type_;
  choice_buffer_t<protocol_ie_single_container_s<time_stamp_slot_idx_ext_ies_o>> c;

  void destroy_();
};

// TimeStamp-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct time_stamp_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct time_stamp_ext_ies_container {
  bool    symbol_idx_present = false;
  uint8_t symbol_idx;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeStamp ::= SEQUENCE
struct time_stamp_s {
  bool                             meas_time_present = false;
  bool                             ie_ext_present    = false;
  uint16_t                         sys_frame_num     = 0;
  time_stamp_slot_idx_c            slot_idx;
  fixed_bitstring<64, false, true> meas_time;
  time_stamp_ext_ies_container     ie_ext;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-MeasuredResultsAssociatedInfoItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using e_c_id_measured_results_associated_info_item_ext_ies_o = protocol_ext_empty_o;

using e_c_id_measured_results_associated_info_item_ext_ies_container = protocol_ext_container_empty_l;

// E-CID-MeasuredResultsAssociatedInfoItem ::= SEQUENCE
struct e_c_id_measured_results_associated_info_item_s {
  bool                                                           time_stamp_present   = false;
  bool                                                           meas_quality_present = false;
  bool                                                           ie_exts_present      = false;
  time_stamp_s                                                   time_stamp;
  trp_meas_quality_s                                             meas_quality;
  e_c_id_measured_results_associated_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-MeasuredResultsAssociatedInfoList ::= SEQUENCE (SIZE (1..64)) OF E-CID-MeasuredResultsAssociatedInfoItem
using e_c_id_measured_results_associated_info_list_l = dyn_array<e_c_id_measured_results_associated_info_item_s>;

// E-CID-MeasurementQuantitiesValue ::= ENUMERATED
struct e_c_id_meas_quantities_value_opts {
  enum options { default_value, angle_of_arrival_nr, /*...*/ timing_advance_nr, nulltype } value;

  const char* to_string() const;
};
using e_c_id_meas_quantities_value_e = enumerated<e_c_id_meas_quantities_value_opts, true, 1>;

// E-CID-MeasurementQuantitiesValue-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using e_c_id_meas_quantities_value_ext_ies_o = protocol_ext_empty_o;

using e_c_id_meas_quantities_value_ext_ies_container = protocol_ext_container_empty_l;

// E-CID-MeasurementQuantities-Item ::= SEQUENCE
struct e_c_id_meas_quantities_item_s {
  bool                                           ie_exts_present = false;
  e_c_id_meas_quantities_value_e                 e_c_id_meas_quantities_value;
  e_c_id_meas_quantities_value_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-MeasurementQuantities-ItemIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct e_c_id_meas_quantities_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e_c_id_meas_quantities_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e_c_id_meas_quantities_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e_c_id_meas_quantities_item_s&       e_c_id_meas_quantities_item() { return c; }
    const e_c_id_meas_quantities_item_s& e_c_id_meas_quantities_item() const { return c; }

  private:
    e_c_id_meas_quantities_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E-CID-MeasurementQuantities ::= SEQUENCE (SIZE (1..64)) OF ProtocolIE-Field{F1AP-PROTOCOL-IES : IEsSetParam}
using e_c_id_meas_quantities_l = dyn_array<protocol_ie_single_container_s<e_c_id_meas_quantities_item_ies_o>>;

// NGRANHighAccuracyAccessPointPosition-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ngran_high_accuracy_access_point_position_ext_ies_o = protocol_ext_empty_o;

using ngran_high_accuracy_access_point_position_ext_ies_container = protocol_ext_container_empty_l;

// NGRANHighAccuracyAccessPointPosition ::= SEQUENCE
struct ngran_high_accuracy_access_point_position_s {
  bool                                                        ie_exts_present           = false;
  int64_t                                                     latitude                  = -2147483648;
  int64_t                                                     longitude                 = -2147483648;
  int32_t                                                     altitude                  = -64000;
  uint16_t                                                    uncertainty_semi_major    = 0;
  uint16_t                                                    uncertainty_semi_minor    = 0;
  uint8_t                                                     orientation_of_major_axis = 0;
  uint8_t                                                     horizontal_confidence     = 0;
  uint16_t                                                    uncertainty_altitude      = 0;
  uint8_t                                                     vertical_confidence       = 0;
  ngran_high_accuracy_access_point_position_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPPositionDirectAccuracy-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using trp_position_direct_accuracy_ext_ies_o = protocol_ies_empty_o;

// TRPPositionDirectAccuracy ::= CHOICE
struct trp_position_direct_accuracy_c {
  struct types_opts {
    enum options { trp_position, trph_aposition, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  trp_position_direct_accuracy_c() = default;
  trp_position_direct_accuracy_c(const trp_position_direct_accuracy_c& other);
  trp_position_direct_accuracy_c& operator=(const trp_position_direct_accuracy_c& other);
  ~trp_position_direct_accuracy_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  access_point_position_s& trp_position()
  {
    assert_choice_type(types::trp_position, type_, "TRPPositionDirectAccuracy");
    return c.get<access_point_position_s>();
  }
  ngran_high_accuracy_access_point_position_s& trph_aposition()
  {
    assert_choice_type(types::trph_aposition, type_, "TRPPositionDirectAccuracy");
    return c.get<ngran_high_accuracy_access_point_position_s>();
  }
  protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TRPPositionDirectAccuracy");
    return c.get<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>();
  }
  const access_point_position_s& trp_position() const
  {
    assert_choice_type(types::trp_position, type_, "TRPPositionDirectAccuracy");
    return c.get<access_point_position_s>();
  }
  const ngran_high_accuracy_access_point_position_s& trph_aposition() const
  {
    assert_choice_type(types::trph_aposition, type_, "TRPPositionDirectAccuracy");
    return c.get<ngran_high_accuracy_access_point_position_s>();
  }
  const protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TRPPositionDirectAccuracy");
    return c.get<protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>();
  }
  access_point_position_s&                                                set_trp_position();
  ngran_high_accuracy_access_point_position_s&                            set_trph_aposition();
  protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<access_point_position_s,
                  ngran_high_accuracy_access_point_position_s,
                  protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>
      c;

  void destroy_();
};

// TRPPositionDirect-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_position_direct_ext_ies_o = protocol_ext_empty_o;

using trp_position_direct_ext_ies_container = protocol_ext_container_empty_l;

// TRPPositionDirect ::= SEQUENCE
struct trp_position_direct_s {
  bool                                  ie_exts_present = false;
  trp_position_direct_accuracy_c        accuracy;
  trp_position_direct_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LocalOrigin-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using local_origin_ext_ies_o = protocol_ext_empty_o;

using local_origin_ext_ies_container = protocol_ext_container_empty_l;

// LocalOrigin ::= SEQUENCE
struct local_origin_s {
  bool                                        ref_point_coordinate_ha_present = false;
  bool                                        ie_exts_present                 = false;
  uint16_t                                    relative_coordinate_id          = 0;
  uint16_t                                    horizontal_axes_orientation     = 0;
  ngran_high_accuracy_access_point_position_s ref_point_coordinate_ha;
  local_origin_ext_ies_container              ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ReferencePoint-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ref_point_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { local_origin, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::local_origin; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    local_origin_s&       local_origin() { return c; }
    const local_origin_s& local_origin() const { return c; }

  private:
    local_origin_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// ReferencePoint ::= CHOICE
struct ref_point_c {
  struct types_opts {
    enum options { coordinate_id, ref_point_coordinate, ref_point_coordinate_ha, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ref_point_c() = default;
  ref_point_c(const ref_point_c& other);
  ref_point_c& operator=(const ref_point_c& other);
  ~ref_point_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint16_t& coordinate_id()
  {
    assert_choice_type(types::coordinate_id, type_, "ReferencePoint");
    return c.get<uint16_t>();
  }
  access_point_position_s& ref_point_coordinate()
  {
    assert_choice_type(types::ref_point_coordinate, type_, "ReferencePoint");
    return c.get<access_point_position_s>();
  }
  ngran_high_accuracy_access_point_position_s& ref_point_coordinate_ha()
  {
    assert_choice_type(types::ref_point_coordinate_ha, type_, "ReferencePoint");
    return c.get<ngran_high_accuracy_access_point_position_s>();
  }
  protocol_ie_single_container_s<ref_point_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ReferencePoint");
    return c.get<protocol_ie_single_container_s<ref_point_ext_ies_o>>();
  }
  const uint16_t& coordinate_id() const
  {
    assert_choice_type(types::coordinate_id, type_, "ReferencePoint");
    return c.get<uint16_t>();
  }
  const access_point_position_s& ref_point_coordinate() const
  {
    assert_choice_type(types::ref_point_coordinate, type_, "ReferencePoint");
    return c.get<access_point_position_s>();
  }
  const ngran_high_accuracy_access_point_position_s& ref_point_coordinate_ha() const
  {
    assert_choice_type(types::ref_point_coordinate_ha, type_, "ReferencePoint");
    return c.get<ngran_high_accuracy_access_point_position_s>();
  }
  const protocol_ie_single_container_s<ref_point_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ReferencePoint");
    return c.get<protocol_ie_single_container_s<ref_point_ext_ies_o>>();
  }
  uint16_t&                                            set_coordinate_id();
  access_point_position_s&                             set_ref_point_coordinate();
  ngran_high_accuracy_access_point_position_s&         set_ref_point_coordinate_ha();
  protocol_ie_single_container_s<ref_point_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<access_point_position_s,
                  ngran_high_accuracy_access_point_position_s,
                  protocol_ie_single_container_s<ref_point_ext_ies_o>>
      c;

  void destroy_();
};

// TRPReferencePointType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using trp_ref_point_type_ext_ies_o = protocol_ies_empty_o;

// TRPReferencePointType ::= CHOICE
struct trp_ref_point_type_c {
  struct types_opts {
    enum options { trp_position_relative_geodetic, trp_position_relative_cartesian, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  trp_ref_point_type_c() = default;
  trp_ref_point_type_c(const trp_ref_point_type_c& other);
  trp_ref_point_type_c& operator=(const trp_ref_point_type_c& other);
  ~trp_ref_point_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  relative_geodetic_location_s& trp_position_relative_geodetic()
  {
    assert_choice_type(types::trp_position_relative_geodetic, type_, "TRPReferencePointType");
    return c.get<relative_geodetic_location_s>();
  }
  relative_cartesian_location_s& trp_position_relative_cartesian()
  {
    assert_choice_type(types::trp_position_relative_cartesian, type_, "TRPReferencePointType");
    return c.get<relative_cartesian_location_s>();
  }
  protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TRPReferencePointType");
    return c.get<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>();
  }
  const relative_geodetic_location_s& trp_position_relative_geodetic() const
  {
    assert_choice_type(types::trp_position_relative_geodetic, type_, "TRPReferencePointType");
    return c.get<relative_geodetic_location_s>();
  }
  const relative_cartesian_location_s& trp_position_relative_cartesian() const
  {
    assert_choice_type(types::trp_position_relative_cartesian, type_, "TRPReferencePointType");
    return c.get<relative_cartesian_location_s>();
  }
  const protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TRPReferencePointType");
    return c.get<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>>();
  }
  relative_geodetic_location_s&                                 set_trp_position_relative_geodetic();
  relative_cartesian_location_s&                                set_trp_position_relative_cartesian();
  protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<trp_ref_point_type_ext_ies_o>,
                  relative_cartesian_location_s,
                  relative_geodetic_location_s>
      c;

  void destroy_();
};

// TRPPositionReferenced-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_position_refd_ext_ies_o = protocol_ext_empty_o;

using trp_position_refd_ext_ies_container = protocol_ext_container_empty_l;

// TRPPositionReferenced ::= SEQUENCE
struct trp_position_refd_s {
  bool                                ie_exts_present = false;
  ref_point_c                         ref_point;
  trp_ref_point_type_c                ref_point_type;
  trp_position_refd_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPPositionDefinitionType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using trp_position_definition_type_ext_ies_o = protocol_ies_empty_o;

// TRPPositionDefinitionType ::= CHOICE
struct trp_position_definition_type_c {
  struct types_opts {
    enum options { direct, refd, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  trp_position_definition_type_c() = default;
  trp_position_definition_type_c(const trp_position_definition_type_c& other);
  trp_position_definition_type_c& operator=(const trp_position_definition_type_c& other);
  ~trp_position_definition_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  trp_position_direct_s& direct()
  {
    assert_choice_type(types::direct, type_, "TRPPositionDefinitionType");
    return c.get<trp_position_direct_s>();
  }
  trp_position_refd_s& refd()
  {
    assert_choice_type(types::refd, type_, "TRPPositionDefinitionType");
    return c.get<trp_position_refd_s>();
  }
  protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TRPPositionDefinitionType");
    return c.get<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>();
  }
  const trp_position_direct_s& direct() const
  {
    assert_choice_type(types::direct, type_, "TRPPositionDefinitionType");
    return c.get<trp_position_direct_s>();
  }
  const trp_position_refd_s& refd() const
  {
    assert_choice_type(types::refd, type_, "TRPPositionDefinitionType");
    return c.get<trp_position_refd_s>();
  }
  const protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TRPPositionDefinitionType");
    return c.get<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>>();
  }
  trp_position_direct_s&                                                  set_direct();
  trp_position_refd_s&                                                    set_refd();
  protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<trp_position_definition_type_ext_ies_o>,
                  trp_position_direct_s,
                  trp_position_refd_s>
      c;

  void destroy_();
};

// GeographicalCoordinates-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct geographical_coordinates_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct geographical_coordinates_ext_ies_container {
  bool                arp_location_info_present = false;
  arp_location_info_l arp_location_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GeographicalCoordinates ::= SEQUENCE
struct geographical_coordinates_s {
  bool                                       dl_prs_res_coordinates_present = false;
  bool                                       ie_exts_present                = false;
  trp_position_definition_type_c             trp_position_definition_type;
  dl_prs_res_coordinates_s                   dl_prs_res_coordinates;
  geographical_coordinates_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Mobile-TRP-LocationInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mobile_trp_location_info_ext_ies_o = protocol_ext_empty_o;

using mobile_trp_location_info_ext_ies_container = protocol_ext_container_empty_l;

// Mobile-TRP-LocationInformation ::= SEQUENCE
struct mobile_trp_location_info_s {
  bool                                       location_time_stamp_present = false;
  bool                                       ie_exts_present             = false;
  unbounded_octstring<true>                  location_info;
  unbounded_octstring<true>                  velocity_info;
  time_stamp_s                               location_time_stamp;
  mobile_trp_location_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-MeasurementResult-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct e_c_id_meas_result_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_c_id_meas_result_ext_ies_container {
  bool                                           mobile_access_point_location_present                 = false;
  bool                                           e_c_id_measured_results_associated_info_list_present = false;
  mobile_trp_location_info_s                     mobile_access_point_location;
  e_c_id_measured_results_associated_info_list_l e_c_id_measured_results_associated_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-MeasurementResult ::= SEQUENCE
struct e_c_id_meas_result_s {
  bool                                 geographical_coordinates_present = false;
  bool                                 ie_exts_present                  = false;
  geographical_coordinates_s           geographical_coordinates;
  e_c_id_measured_results_list_l       measured_results_list;
  e_c_id_meas_result_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-ReportCharacteristics ::= ENUMERATED
struct e_c_id_report_characteristics_opts {
  enum options { on_demand, periodic, /*...*/ nulltype } value;

  const char* to_string() const;
};
using e_c_id_report_characteristics_e = enumerated<e_c_id_report_characteristics_opts, true>;

// EUTRA-Transmission-Bandwidth ::= ENUMERATED
struct eutra_tx_bw_opts {
  enum options { bw6, bw15, bw25, bw50, bw75, bw100, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using eutra_tx_bw_e = enumerated<eutra_tx_bw_opts, true>;

// EUTRA-Coex-FDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using eutra_coex_fdd_info_ext_ies_o = protocol_ext_empty_o;

using eutra_coex_fdd_info_ext_ies_container = protocol_ext_container_empty_l;

// EUTRA-Coex-FDD-Info ::= SEQUENCE
struct eutra_coex_fdd_info_s {
  bool                                  ul_earfcn_present = false;
  bool                                  ul_tx_bw_present  = false;
  bool                                  ie_exts_present   = false;
  uint32_t                              ul_earfcn         = 0;
  uint32_t                              dl_earfcn         = 0;
  eutra_tx_bw_e                         ul_tx_bw;
  eutra_tx_bw_e                         dl_tx_bw;
  eutra_coex_fdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRA-SubframeAssignment ::= ENUMERATED
struct eutra_sf_assign_opts {
  enum options { sa0, sa1, sa2, sa3, sa4, sa5, sa6, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using eutra_sf_assign_e = enumerated<eutra_sf_assign_opts, true>;

// EUTRA-SpecialSubframePatterns ::= ENUMERATED
struct eutra_special_sf_patterns_opts {
  enum options { ssp0, ssp1, ssp2, ssp3, ssp4, ssp5, ssp6, ssp7, ssp8, ssp9, ssp10, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using eutra_special_sf_patterns_e = enumerated<eutra_special_sf_patterns_opts, true>;

// EUTRA-CyclicPrefixDL ::= ENUMERATED
struct eutra_cp_dl_opts {
  enum options { normal, extended, /*...*/ nulltype } value;

  const char* to_string() const;
};
using eutra_cp_dl_e = enumerated<eutra_cp_dl_opts, true>;

// EUTRA-CyclicPrefixUL ::= ENUMERATED
struct eutra_cp_ul_opts {
  enum options { normal, extended, /*...*/ nulltype } value;

  const char* to_string() const;
};
using eutra_cp_ul_e = enumerated<eutra_cp_ul_opts, true>;

// EUTRA-SpecialSubframe-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using eutra_special_sf_info_ext_ies_o = protocol_ext_empty_o;

using eutra_special_sf_info_ext_ies_container = protocol_ext_container_empty_l;

// EUTRA-SpecialSubframe-Info ::= SEQUENCE
struct eutra_special_sf_info_s {
  bool                                    ie_exts_present = false;
  eutra_special_sf_patterns_e             special_sf_patterns;
  eutra_cp_dl_e                           cp_dl;
  eutra_cp_ul_e                           cp_ul;
  eutra_special_sf_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRA-Coex-TDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using eutra_coex_tdd_info_ext_ies_o = protocol_ext_empty_o;

using eutra_coex_tdd_info_ext_ies_container = protocol_ext_container_empty_l;

// EUTRA-Coex-TDD-Info ::= SEQUENCE
struct eutra_coex_tdd_info_s {
  bool                                  ie_exts_present = false;
  uint32_t                              earfcn          = 0;
  eutra_tx_bw_e                         tx_bw;
  eutra_sf_assign_e                     sf_assign;
  eutra_special_sf_info_s               special_sf_info;
  eutra_coex_tdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRA-Coex-Mode-Info ::= CHOICE
struct eutra_coex_mode_info_c {
  struct types_opts {
    enum options { fdd, tdd, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  eutra_coex_mode_info_c() = default;
  eutra_coex_mode_info_c(const eutra_coex_mode_info_c& other);
  eutra_coex_mode_info_c& operator=(const eutra_coex_mode_info_c& other);
  ~eutra_coex_mode_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  eutra_coex_fdd_info_s& fdd()
  {
    assert_choice_type(types::fdd, type_, "EUTRA-Coex-Mode-Info");
    return c.get<eutra_coex_fdd_info_s>();
  }
  eutra_coex_tdd_info_s& tdd()
  {
    assert_choice_type(types::tdd, type_, "EUTRA-Coex-Mode-Info");
    return c.get<eutra_coex_tdd_info_s>();
  }
  const eutra_coex_fdd_info_s& fdd() const
  {
    assert_choice_type(types::fdd, type_, "EUTRA-Coex-Mode-Info");
    return c.get<eutra_coex_fdd_info_s>();
  }
  const eutra_coex_tdd_info_s& tdd() const
  {
    assert_choice_type(types::tdd, type_, "EUTRA-Coex-Mode-Info");
    return c.get<eutra_coex_tdd_info_s>();
  }
  eutra_coex_fdd_info_s& set_fdd();
  eutra_coex_tdd_info_s& set_tdd();

private:
  types                                                         type_;
  choice_buffer_t<eutra_coex_fdd_info_s, eutra_coex_tdd_info_s> c;

  void destroy_();
};

// EUTRA-FDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using eutra_fdd_info_ext_ies_o = protocol_ext_empty_o;

using eutra_fdd_info_ext_ies_container = protocol_ext_container_empty_l;

// EUTRA-FDD-Info ::= SEQUENCE
struct eutra_fdd_info_s {
  bool                             ie_exts_present      = false;
  uint16_t                         ul_offset_to_point_a = 0;
  uint16_t                         dl_offset_to_point_a = 0;
  eutra_fdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRA-TDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using eutra_tdd_info_ext_ies_o = protocol_ext_empty_o;

using eutra_tdd_info_ext_ies_container = protocol_ext_container_empty_l;

// EUTRA-TDD-Info ::= SEQUENCE
struct eutra_tdd_info_s {
  bool                             ie_exts_present   = false;
  uint16_t                         offset_to_point_a = 0;
  eutra_tdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRA-Mode-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using eutra_mode_info_ext_ies_o = protocol_ies_empty_o;

// EUTRA-Mode-Info ::= CHOICE
struct eutra_mode_info_c {
  struct types_opts {
    enum options { eutra_fdd, eutra_tdd, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  eutra_mode_info_c() = default;
  eutra_mode_info_c(const eutra_mode_info_c& other);
  eutra_mode_info_c& operator=(const eutra_mode_info_c& other);
  ~eutra_mode_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  eutra_fdd_info_s& eutra_fdd()
  {
    assert_choice_type(types::eutra_fdd, type_, "EUTRA-Mode-Info");
    return c.get<eutra_fdd_info_s>();
  }
  eutra_tdd_info_s& eutra_tdd()
  {
    assert_choice_type(types::eutra_tdd, type_, "EUTRA-Mode-Info");
    return c.get<eutra_tdd_info_s>();
  }
  protocol_ie_single_container_s<eutra_mode_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "EUTRA-Mode-Info");
    return c.get<protocol_ie_single_container_s<eutra_mode_info_ext_ies_o>>();
  }
  const eutra_fdd_info_s& eutra_fdd() const
  {
    assert_choice_type(types::eutra_fdd, type_, "EUTRA-Mode-Info");
    return c.get<eutra_fdd_info_s>();
  }
  const eutra_tdd_info_s& eutra_tdd() const
  {
    assert_choice_type(types::eutra_tdd, type_, "EUTRA-Mode-Info");
    return c.get<eutra_tdd_info_s>();
  }
  const protocol_ie_single_container_s<eutra_mode_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "EUTRA-Mode-Info");
    return c.get<protocol_ie_single_container_s<eutra_mode_info_ext_ies_o>>();
  }
  eutra_fdd_info_s&                                          set_eutra_fdd();
  eutra_tdd_info_s&                                          set_eutra_tdd();
  protocol_ie_single_container_s<eutra_mode_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                          type_;
  choice_buffer_t<eutra_fdd_info_s, eutra_tdd_info_s, protocol_ie_single_container_s<eutra_mode_info_ext_ies_o>> c;

  void destroy_();
};

// EUTRA-PRACH-Configuration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using eutra_prach_cfg_ext_ies_o = protocol_ext_empty_o;

using eutra_prach_cfg_ext_ies_container = protocol_ext_container_empty_l;

// EUTRA-PRACH-Configuration ::= SEQUENCE
struct eutra_prach_cfg_s {
  bool                              prach_cfg_idx_present = false;
  bool                              ie_exts_present       = false;
  uint16_t                          root_seq_idx          = 0;
  uint8_t                           zero_correlation_idx  = 0;
  bool                              high_speed_flag       = false;
  uint8_t                           prach_freq_offset     = 0;
  uint8_t                           prach_cfg_idx         = 0;
  eutra_prach_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Served-EUTRA-Cell-Information-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using served_eutra_cell_info_ext_ies_o = protocol_ext_empty_o;

using served_eutra_cell_info_ext_ies_container = protocol_ext_container_empty_l;

// Served-EUTRA-Cells-Information ::= SEQUENCE
struct served_eutra_cells_info_s {
  bool                                     ie_exts_present = false;
  eutra_mode_info_c                        eutra_mode_info;
  unbounded_octstring<true>                protected_eutra_res_ind;
  served_eutra_cell_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRACells-List-itemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using eutra_cells_list_item_ext_ies_o = protocol_ext_empty_o;

using eutra_cells_list_item_ext_ies_container = protocol_ext_container_empty_l;

// EUTRACells-List-item ::= SEQUENCE
struct eutra_cells_list_item_s {
  bool                                    ie_exts_present = false;
  fixed_bitstring<28, false, true>        eutra_cell_id;
  served_eutra_cells_info_s               served_eutra_cells_info;
  eutra_cells_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EUTRACells-List ::= SEQUENCE (SIZE (1..256)) OF EUTRACells-List-item
using eutra_cells_list_l = dyn_array<eutra_cells_list_item_s>;

// PreambleIndex-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using preamb_idx_item_ext_ies_o = protocol_ext_empty_o;

using preamb_idx_item_ext_ies_container = protocol_ext_container_empty_l;

// PreambleIndexList-Item ::= SEQUENCE
struct preamb_idx_list_item_s {
  bool                              ie_exts_present = false;
  uint8_t                           preamb_idx      = 0;
  preamb_idx_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PreambleIndexList ::= SEQUENCE (SIZE (1..64)) OF PreambleIndexList-Item
using preamb_idx_list_l = dyn_array<preamb_idx_list_item_s>;

// LTMgNB-DU-IDs-PreambleIndex-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltm_gnb_du_ids_preamb_idx_item_ext_ies_o = protocol_ext_empty_o;

using ltm_gnb_du_ids_preamb_idx_item_ext_ies_container = protocol_ext_container_empty_l;

// LTMgNB-DU-IDs-PreambleIndex-Item ::= SEQUENCE
struct ltm_gnb_du_ids_preamb_idx_item_s {
  bool                                             ie_exts_present = false;
  uint64_t                                         ltm_gnb_du_id   = 0;
  preamb_idx_list_l                                preamb_idx_list;
  ltm_gnb_du_ids_preamb_idx_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMgNB-DU-IDs-PreambleIndexList ::= SEQUENCE (SIZE (1..8)) OF LTMgNB-DU-IDs-PreambleIndex-Item
using ltm_gnb_du_ids_preamb_idx_list_l = dyn_array<ltm_gnb_du_ids_preamb_idx_item_s>;

// EarlyULSyncConfig-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using early_ul_sync_cfg_ext_ies_o = protocol_ext_empty_o;

using early_ul_sync_cfg_ext_ies_container = protocol_ext_container_empty_l;

// EarlyULSyncConfig ::= SEQUENCE
struct early_ul_sync_cfg_s {
  bool                                ie_exts_present = false;
  unbounded_octstring<true>           rach;
  ltm_gnb_du_ids_preamb_idx_list_l    ltm_gnb_du_ids_preamb_idx_list;
  early_ul_sync_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAAssistanceInfo ::= ENUMERATED
struct ta_assist_info_opts {
  enum options { zero, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using ta_assist_info_e = enumerated<ta_assist_info_opts, true>;

// EarlySyncCandidateCellInformation-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct early_sync_candidate_cell_info_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct early_sync_candidate_cell_info_item_ext_ies_container {
  bool                     ssb_positions_in_burst_present = false;
  ssb_positions_in_burst_c ssb_positions_in_burst;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EarlySyncCandidateCellInformation-Item ::= SEQUENCE
struct early_sync_candidate_cell_info_item_s {
  bool                                                  early_ul_sync_cfg_present     = false;
  bool                                                  early_ul_sync_cfg_sul_present = false;
  bool                                                  ta_assist_info_present        = false;
  bool                                                  ie_exts_present               = false;
  nr_cgi_s                                              nr_cgi;
  unbounded_octstring<true>                             tci_states_cfgs_list;
  early_ul_sync_cfg_s                                   early_ul_sync_cfg;
  early_ul_sync_cfg_s                                   early_ul_sync_cfg_sul;
  ta_assist_info_e                                      ta_assist_info;
  unbounded_octstring<true>                             ue_based_t_ameas_cfg;
  early_sync_candidate_cell_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EarlySyncCandidateCellInformation-List ::= SEQUENCE (SIZE (1..8)) OF EarlySyncCandidateCellInformation-Item
using early_sync_candidate_cell_info_list_l = dyn_array<early_sync_candidate_cell_info_item_s>;

// EarlySyncInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using early_sync_info_ext_ies_o = protocol_ext_empty_o;

using early_sync_info_ext_ies_container = protocol_ext_container_empty_l;

// EarlySyncInformation ::= SEQUENCE
struct early_sync_info_s {
  bool                              early_ul_sync_cfg_present     = false;
  bool                              early_ul_sync_cfg_sul_present = false;
  bool                              ie_exts_present               = false;
  unbounded_octstring<true>         tci_states_cfgs_list;
  early_ul_sync_cfg_s               early_ul_sync_cfg;
  early_ul_sync_cfg_s               early_ul_sync_cfg_sul;
  early_sync_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestforRACHConfiguration ::= ENUMERATED
struct requestfor_rach_cfg_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using requestfor_rach_cfg_e = enumerated<requestfor_rach_cfg_opts, true>;

// LTMgNB-DU-IDs-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltm_gnb_du_ids_item_ext_ies_o = protocol_ext_empty_o;

using ltm_gnb_du_ids_item_ext_ies_container = protocol_ext_container_empty_l;

// LTMgNB-DU-IDs-Item ::= SEQUENCE
struct ltm_gnb_du_ids_item_s {
  bool                                  ie_exts_present = false;
  uint64_t                              ltm_gnb_du_id   = 0;
  ltm_gnb_du_ids_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMgNB-DU-IDsList ::= SEQUENCE (SIZE (1..8)) OF LTMgNB-DU-IDs-Item
using ltm_gnb_du_ids_list_l = dyn_array<ltm_gnb_du_ids_item_s>;

// EarlySyncInformation-Request-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using early_sync_info_request_ext_ies_o = protocol_ext_empty_o;

using early_sync_info_request_ext_ies_container = protocol_ext_container_empty_l;

// EarlySyncInformation-Request ::= SEQUENCE
struct early_sync_info_request_s {
  bool                                      ie_exts_present = false;
  requestfor_rach_cfg_e                     requestfor_rach_cfg;
  ltm_gnb_du_ids_list_l                     ltm_gnb_du_ids_list;
  early_sync_info_request_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EarlySyncServingCellInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using early_sync_serving_cell_info_ext_ies_o = protocol_ext_empty_o;

using early_sync_serving_cell_info_ext_ies_container = protocol_ext_container_empty_l;

// EarlySyncServingCellInformation ::= SEQUENCE
struct early_sync_serving_cell_info_s {
  bool                                           ie_exts_present = false;
  unbounded_octstring<true>                      ue_based_t_ameas_cfg;
  early_sync_serving_cell_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventType ::= ENUMERATED
struct event_type_opts {
  enum options { on_demand, periodic, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using event_type_e = enumerated<event_type_opts, true>;

// ExecuteDuplication ::= ENUMERATED
struct execute_dupl_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using execute_dupl_e = enumerated<execute_dupl_opts, true>;

// Extended-GNB-CU-Name-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using extended_gnb_cu_name_ext_ies_o = protocol_ext_empty_o;

using extended_gnb_cu_name_ext_ies_container = protocol_ext_container_empty_l;

// Extended-GNB-CU-Name ::= SEQUENCE
struct extended_gnb_cu_name_s {
  bool                                   gnb_cu_name_visible_string_present = false;
  bool                                   gnb_cu_name_utf8_string_present    = false;
  bool                                   ie_exts_present                    = false;
  visible_string<1, 150, true, true>     gnb_cu_name_visible_string;
  utf8_string<true>                      gnb_cu_name_utf8_string;
  extended_gnb_cu_name_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Extended-GNB-DU-Name-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using extended_gnb_du_name_ext_ies_o = protocol_ext_empty_o;

using extended_gnb_du_name_ext_ies_container = protocol_ext_container_empty_l;

// Extended-GNB-DU-Name ::= SEQUENCE
struct extended_gnb_du_name_s {
  bool                                   gnb_du_name_visible_string_present = false;
  bool                                   gnb_du_name_utf8_string_present    = false;
  bool                                   ie_exts_present                    = false;
  visible_string<1, 150, true, true>     gnb_du_name_visible_string;
  utf8_string<true>                      gnb_du_name_utf8_string;
  extended_gnb_du_name_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExtendedAdditionalPathList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using extended_add_path_list_item_ext_ies_o = protocol_ext_empty_o;

using extended_add_path_list_item_ext_ies_container = protocol_ext_container_empty_l;

// ExtendedAdditionalPathList-Item ::= SEQUENCE
struct extended_add_path_list_item_s {
  bool                                          path_quality_present     = false;
  bool                                          multiple_ul_ao_a_present = false;
  bool                                          path_pwr_present         = false;
  bool                                          ie_exts_present          = false;
  relative_path_delay_c                         relative_time_of_path;
  trp_meas_quality_s                            path_quality;
  multiple_ul_ao_a_s                            multiple_ul_ao_a;
  ul_srs_rsrp_p_s                               path_pwr;
  extended_add_path_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExtendedAdditionalPathList ::= SEQUENCE (SIZE (1..8)) OF ExtendedAdditionalPathList-Item
using extended_add_path_list_l = dyn_array<extended_add_path_list_item_s>;

// SliceSupportItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slice_support_item_ext_ies_o = protocol_ext_empty_o;

using slice_support_item_ext_ies_container = protocol_ext_container_empty_l;

// SliceSupportItem ::= SEQUENCE
struct slice_support_item_s {
  bool                                 ie_exts_present = false;
  snssai_s                             snssai;
  slice_support_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceSupportList ::= SEQUENCE (SIZE (1..1024)) OF SliceSupportItem
using slice_support_list_l = dyn_array<slice_support_item_s>;

// NPNSupportInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using npn_support_info_ext_ies_o = protocol_ies_empty_o;

// NPNSupportInfo ::= CHOICE
struct npn_support_info_c {
  struct types_opts {
    enum options { sn_pn_info, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  npn_support_info_c() = default;
  npn_support_info_c(const npn_support_info_c& other);
  npn_support_info_c& operator=(const npn_support_info_c& other);
  ~npn_support_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<44, false, true>& sn_pn_info()
  {
    assert_choice_type(types::sn_pn_info, type_, "NPNSupportInfo");
    return c.get<fixed_bitstring<44, false, true>>();
  }
  protocol_ie_single_container_s<npn_support_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NPNSupportInfo");
    return c.get<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>();
  }
  const fixed_bitstring<44, false, true>& sn_pn_info() const
  {
    assert_choice_type(types::sn_pn_info, type_, "NPNSupportInfo");
    return c.get<fixed_bitstring<44, false, true>>();
  }
  const protocol_ie_single_container_s<npn_support_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NPNSupportInfo");
    return c.get<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>();
  }
  fixed_bitstring<44, false, true>&                           set_sn_pn_info();
  protocol_ie_single_container_s<npn_support_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                         type_;
  choice_buffer_t<fixed_bitstring<44, false, true>, protocol_ie_single_container_s<npn_support_info_ext_ies_o>> c;

  void destroy_();
};

// ExtendedSliceSupportList ::= SEQUENCE (SIZE (1..65535)) OF SliceSupportItem
using extended_slice_support_list_l = dyn_array<slice_support_item_s>;

// NSAGSupportItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nsag_support_item_ext_ies_o = protocol_ext_empty_o;

using nsag_support_item_ext_ies_container = protocol_ext_container_empty_l;

// NSAGSupportItem ::= SEQUENCE
struct nsag_support_item_s {
  bool                                ie_exts_present = false;
  uint16_t                            nsag_id         = 0;
  extended_slice_support_list_l       nsag_slice_support;
  nsag_support_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NSAGSupportList ::= SEQUENCE (SIZE (1..256)) OF NSAGSupportItem
using nsag_support_list_l = dyn_array<nsag_support_item_s>;

// ExtendedServedPLMNs-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct extended_served_plmns_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct extended_served_plmns_item_ext_ies_container {
  bool                          npn_support_info_present                = false;
  bool                          extended_tai_slice_support_list_present = false;
  bool                          tai_nsag_support_list_present           = false;
  npn_support_info_c            npn_support_info;
  extended_slice_support_list_l extended_tai_slice_support_list;
  nsag_support_list_l           tai_nsag_support_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExtendedServedPLMNs-Item ::= SEQUENCE
struct extended_served_plmns_item_s {
  bool                                         ie_exts_present = false;
  fixed_octstring<3, true>                     plmn_id;
  slice_support_list_l                         tai_slice_support_list;
  extended_served_plmns_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExtendedServedPLMNs-List ::= SEQUENCE (SIZE (1..6)) OF ExtendedServedPLMNs-Item
using extended_served_plmns_list_l = dyn_array<extended_served_plmns_item_s>;

// F1CPathNRDC ::= ENUMERATED
struct f1_cp_ath_nr_dc_opts {
  enum options { mcg, scg, both, nulltype } value;

  const char* to_string() const;
};
using f1_cp_ath_nr_dc_e = enumerated<f1_cp_ath_nr_dc_opts>;

// F1CPathNSA ::= ENUMERATED
struct f1_cp_ath_nsa_opts {
  enum options { lte, nr, both, nulltype } value;

  const char* to_string() const;
};
using f1_cp_ath_nsa_e = enumerated<f1_cp_ath_nsa_opts>;

// F1CTransferPath-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using f1_c_transfer_path_ext_ies_o = protocol_ext_empty_o;

using f1_c_transfer_path_ext_ies_container = protocol_ext_container_empty_l;

// F1CTransferPath ::= SEQUENCE
struct f1_c_transfer_path_s {
  bool                                 ie_exts_present = false;
  f1_cp_ath_nsa_e                      f1_cp_ath_nsa;
  f1_c_transfer_path_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// F1CTransferPathNRDC-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using f1_c_transfer_path_nr_dc_ext_ies_o = protocol_ext_empty_o;

using f1_c_transfer_path_nr_dc_ext_ies_container = protocol_ext_container_empty_l;

// F1CTransferPathNRDC ::= SEQUENCE
struct f1_c_transfer_path_nr_dc_s {
  bool                                       ie_exts_present = false;
  f1_cp_ath_nr_dc_e                          f1_cp_ath_nr_dc;
  f1_c_transfer_path_nr_dc_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// F1U-PathFailure ::= ENUMERATED
struct f1_u_path_fail_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using f1_u_path_fail_e = enumerated<f1_u_path_fail_opts, true>;

// FDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct fdd_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct fdd_info_ext_ies_container {
  bool              ul_carrier_list_present = false;
  bool              dl_carrier_list_present = false;
  nr_carrier_list_l ul_carrier_list;
  nr_carrier_list_l dl_carrier_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FDD-Info ::= SEQUENCE
struct fdd_info_s {
  bool                       ie_exts_present = false;
  nr_freq_info_s             ul_nr_freq_info;
  nr_freq_info_s             dl_nr_freq_info;
  tx_bw_s                    ul_tx_bw;
  tx_bw_s                    dl_tx_bw;
  fdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FiveG-ProSeDirectDiscovery ::= ENUMERATED
struct five_g_pro_se_direct_discovery_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_direct_discovery_e = enumerated<five_g_pro_se_direct_discovery_opts, true>;

// FiveG-ProSeDirectCommunication ::= ENUMERATED
struct five_g_pro_se_direct_communication_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_direct_communication_e = enumerated<five_g_pro_se_direct_communication_opts, true>;

// FiveG-ProSeLayer2UEtoNetworkRelay ::= ENUMERATED
struct five_g_pro_se_layer2_ue_to_network_relay_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_ue_to_network_relay_e = enumerated<five_g_pro_se_layer2_ue_to_network_relay_opts, true>;

// FiveG-ProSeLayer3UEtoNetworkRelay ::= ENUMERATED
struct five_g_pro_se_layer3_ue_to_network_relay_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer3_ue_to_network_relay_e = enumerated<five_g_pro_se_layer3_ue_to_network_relay_opts, true>;

// FiveG-ProSeLayer2RemoteUE ::= ENUMERATED
struct five_g_pro_se_layer2_remote_ue_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_remote_ue_e = enumerated<five_g_pro_se_layer2_remote_ue_opts, true>;

// FiveG-ProSeLayer2Multipath ::= ENUMERATED
struct five_g_pro_se_layer2_multipath_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_multipath_e = enumerated<five_g_pro_se_layer2_multipath_opts, true>;

// FiveG-ProSeLayer2UEtoUERelay ::= ENUMERATED
struct five_g_pro_se_layer2_ue_to_ue_relay_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_ue_to_ue_relay_e = enumerated<five_g_pro_se_layer2_ue_to_ue_relay_opts, true>;

// FiveG-ProSeLayer2UEtoUERemote ::= ENUMERATED
struct five_g_pro_se_layer2_ue_to_ue_remote_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_ue_to_ue_remote_e = enumerated<five_g_pro_se_layer2_ue_to_ue_remote_opts, true>;

// FiveG-ProSeAuthorized-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct five_g_pro_se_authorized_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct five_g_pro_se_authorized_ext_ies_container {
  bool                                   five_g_pro_se_layer2_multipath_present       = false;
  bool                                   five_g_pro_se_layer2_ue_to_ue_relay_present  = false;
  bool                                   five_g_pro_se_layer2_ue_to_ue_remote_present = false;
  five_g_pro_se_layer2_multipath_e       five_g_pro_se_layer2_multipath;
  five_g_pro_se_layer2_ue_to_ue_relay_e  five_g_pro_se_layer2_ue_to_ue_relay;
  five_g_pro_se_layer2_ue_to_ue_remote_e five_g_pro_se_layer2_ue_to_ue_remote;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FiveG-ProSeAuthorized ::= SEQUENCE
struct five_g_pro_se_authorized_s {
  bool                                       five_g_pro_se_direct_discovery_present           = false;
  bool                                       five_g_pro_se_direct_communication_present       = false;
  bool                                       five_g_pro_se_layer2_ue_to_network_relay_present = false;
  bool                                       five_g_pro_se_layer3_ue_to_network_relay_present = false;
  bool                                       five_g_pro_se_layer2_remote_ue_present           = false;
  bool                                       ie_exts_present                                  = false;
  five_g_pro_se_direct_discovery_e           five_g_pro_se_direct_discovery;
  five_g_pro_se_direct_communication_e       five_g_pro_se_direct_communication;
  five_g_pro_se_layer2_ue_to_network_relay_e five_g_pro_se_layer2_ue_to_network_relay;
  five_g_pro_se_layer3_ue_to_network_relay_e five_g_pro_se_layer3_ue_to_network_relay;
  five_g_pro_se_layer2_remote_ue_e           five_g_pro_se_layer2_remote_ue;
  five_g_pro_se_authorized_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FlowsMappedToSLDRB-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using flows_mapped_to_sl_drb_item_ext_ies_o = protocol_ext_empty_o;

using flows_mapped_to_sl_drb_item_ext_ies_container = protocol_ext_container_empty_l;

// FlowsMappedToSLDRB-Item ::= SEQUENCE
struct flows_mapped_to_sl_drb_item_s {
  bool                                          ie_exts_present = false;
  uint16_t                                      pc5_qos_flow_id = 1;
  flows_mapped_to_sl_drb_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FlowsMappedToSLDRB-List ::= SEQUENCE (SIZE (1..2048)) OF FlowsMappedToSLDRB-Item
using flows_mapped_to_sl_drb_list_l = dyn_array<flows_mapped_to_sl_drb_item_s>;

// FullConfiguration ::= ENUMERATED
struct full_cfg_opts {
  enum options { full, /*...*/ nulltype } value;

  const char* to_string() const;
};
using full_cfg_e = enumerated<full_cfg_opts, true>;

// GNB-CU-TNL-Association-Failed-To-Setup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gnb_cu_tnl_assoc_failed_to_setup_item_ext_ies_o = protocol_ext_empty_o;

using gnb_cu_tnl_assoc_failed_to_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// GNB-CU-TNL-Association-Failed-To-Setup-Item ::= SEQUENCE
struct gnb_cu_tnl_assoc_failed_to_setup_item_s {
  bool                                                    ie_exts_present = false;
  cp_transport_layer_address_c                            tnl_assoc_transport_layer_address;
  cause_c                                                 cause;
  gnb_cu_tnl_assoc_failed_to_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-CU-TNL-Association-Setup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gnb_cu_tnl_assoc_setup_item_ext_ies_o = protocol_ext_empty_o;

using gnb_cu_tnl_assoc_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// GNB-CU-TNL-Association-Setup-Item ::= SEQUENCE
struct gnb_cu_tnl_assoc_setup_item_s {
  bool                                          ie_exts_present = false;
  cp_transport_layer_address_c                  tnl_assoc_transport_layer_address;
  gnb_cu_tnl_assoc_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TNLAssociationUsage ::= ENUMERATED
struct tnl_assoc_usage_opts {
  enum options { ue, non_ue, both, /*...*/ nulltype } value;

  const char* to_string() const;
};
using tnl_assoc_usage_e = enumerated<tnl_assoc_usage_opts, true>;

// GNB-CU-TNL-Association-To-Add-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gnb_cu_tnl_assoc_to_add_item_ext_ies_o = protocol_ext_empty_o;

using gnb_cu_tnl_assoc_to_add_item_ext_ies_container = protocol_ext_container_empty_l;

// GNB-CU-TNL-Association-To-Add-Item ::= SEQUENCE
struct gnb_cu_tnl_assoc_to_add_item_s {
  bool                                           ie_exts_present = false;
  cp_transport_layer_address_c                   tnl_assoc_transport_layer_address;
  tnl_assoc_usage_e                              tnl_assoc_usage;
  gnb_cu_tnl_assoc_to_add_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-CU-TNL-Association-To-Remove-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct gnb_cu_tnl_assoc_to_rem_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct gnb_cu_tnl_assoc_to_rem_item_ext_ies_container {
  bool                         tnl_assoc_transport_layer_address_gnb_du_present = false;
  cp_transport_layer_address_c tnl_assoc_transport_layer_address_gnb_du;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-CU-TNL-Association-To-Remove-Item ::= SEQUENCE
struct gnb_cu_tnl_assoc_to_rem_item_s {
  bool                                           ie_exts_present = false;
  cp_transport_layer_address_c                   tnl_assoc_transport_layer_address;
  gnb_cu_tnl_assoc_to_rem_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-CU-TNL-Association-To-Update-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gnb_cu_tnl_assoc_to_upd_item_ext_ies_o = protocol_ext_empty_o;

using gnb_cu_tnl_assoc_to_upd_item_ext_ies_container = protocol_ext_container_empty_l;

// GNB-CU-TNL-Association-To-Update-Item ::= SEQUENCE
struct gnb_cu_tnl_assoc_to_upd_item_s {
  bool                                           tnl_assoc_usage_present = false;
  bool                                           ie_exts_present         = false;
  cp_transport_layer_address_c                   tnl_assoc_transport_layer_address;
  tnl_assoc_usage_e                              tnl_assoc_usage;
  gnb_cu_tnl_assoc_to_upd_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedPLMNs-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct served_plmns_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct served_plmns_item_ext_ies_container {
  bool                          tai_slice_support_list_present          = false;
  bool                          npn_support_info_present                = false;
  bool                          extended_tai_slice_support_list_present = false;
  bool                          tai_nsag_support_list_present           = false;
  slice_support_list_l          tai_slice_support_list;
  npn_support_info_c            npn_support_info;
  extended_slice_support_list_l extended_tai_slice_support_list;
  nsag_support_list_l           tai_nsag_support_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedPLMNs-Item ::= SEQUENCE
struct served_plmns_item_s {
  bool                                ie_exts_present = false;
  fixed_octstring<3, true>            plmn_id;
  served_plmns_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedPLMNs-List ::= SEQUENCE (SIZE (1..6)) OF ServedPLMNs-Item
using served_plmns_list_l = dyn_array<served_plmns_item_s>;

// NumDLULSymbols-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct num_dl_ul_symbols_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct num_dl_ul_symbols_ext_ies_container {
  bool          permutation_present = false;
  permutation_e permutation;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NumDLULSymbols ::= SEQUENCE
struct num_dl_ul_symbols_s {
  bool                                ie_exts_present = false;
  uint8_t                             num_dl_symbols  = 0;
  uint8_t                             num_ul_symbols  = 0;
  num_dl_ul_symbols_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SymbolAllocInSlot-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using symbol_alloc_in_slot_ext_ies_o = protocol_ies_empty_o;

// SymbolAllocInSlot ::= CHOICE
struct symbol_alloc_in_slot_c {
  struct types_opts {
    enum options { all_dl, all_ul, both_dl_and_ul, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  symbol_alloc_in_slot_c() = default;
  symbol_alloc_in_slot_c(const symbol_alloc_in_slot_c& other);
  symbol_alloc_in_slot_c& operator=(const symbol_alloc_in_slot_c& other);
  ~symbol_alloc_in_slot_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  num_dl_ul_symbols_s& both_dl_and_ul()
  {
    assert_choice_type(types::both_dl_and_ul, type_, "SymbolAllocInSlot");
    return c.get<num_dl_ul_symbols_s>();
  }
  protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SymbolAllocInSlot");
    return c.get<protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>>();
  }
  const num_dl_ul_symbols_s& both_dl_and_ul() const
  {
    assert_choice_type(types::both_dl_and_ul, type_, "SymbolAllocInSlot");
    return c.get<num_dl_ul_symbols_s>();
  }
  const protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SymbolAllocInSlot");
    return c.get<protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>>();
  }
  void                                                            set_all_dl();
  void                                                            set_all_ul();
  num_dl_ul_symbols_s&                                            set_both_dl_and_ul();
  protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                type_;
  choice_buffer_t<num_dl_ul_symbols_s, protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>> c;

  void destroy_();
};

// Slot-Configuration-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using slot_cfg_item_ext_ies_o = protocol_ext_empty_o;

using slot_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// Slot-Configuration-Item ::= SEQUENCE
struct slot_cfg_item_s {
  bool                            ie_exts_present = false;
  uint16_t                        slot_idx        = 0;
  symbol_alloc_in_slot_c          symbol_alloc_in_slot;
  slot_cfg_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Slot-Configuration-List ::= SEQUENCE (SIZE (1..5120)) OF Slot-Configuration-Item
using slot_cfg_list_l = dyn_array<slot_cfg_item_s>;

// IntendedTDD-DL-ULConfig-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using intended_tdd_dl_ul_cfg_ext_ies_o = protocol_ext_empty_o;

using intended_tdd_dl_ul_cfg_ext_ies_container = protocol_ext_container_empty_l;

// IntendedTDD-DL-ULConfig ::= SEQUENCE
struct intended_tdd_dl_ul_cfg_s {
  struct nr_scs_opts {
    enum options { scs15, scs30, scs60, scs120, /*...*/ scs480, scs960, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using nr_scs_e_ = enumerated<nr_scs_opts, true, 2>;
  struct nr_cp_opts {
    enum options { normal, extended, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using nr_cp_e_ = enumerated<nr_cp_opts, true>;
  struct nr_dl_ul_tx_periodicity_opts {
    enum options {
      ms0p5,
      ms0p625,
      ms1,
      ms1p25,
      ms2,
      ms2p5,
      ms3,
      ms4,
      ms5,
      ms10,
      ms20,
      ms40,
      ms60,
      ms80,
      ms100,
      ms120,
      ms140,
      ms160,
      // ...
      nulltype
    } value;

    const char* to_string() const;
  };
  using nr_dl_ul_tx_periodicity_e_ = enumerated<nr_dl_ul_tx_periodicity_opts, true>;

  // member variables
  bool                                     ie_exts_present = false;
  nr_scs_e_                                nr_scs;
  nr_cp_e_                                 nr_cp;
  nr_dl_ul_tx_periodicity_e_               nr_dl_ul_tx_periodicity;
  slot_cfg_list_l                          slot_cfg_list;
  intended_tdd_dl_ul_cfg_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Transmission-Bandwidth-asymmetric-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tx_bw_asymmetric_ext_ies_o = protocol_ext_empty_o;

using tx_bw_asymmetric_ext_ies_container = protocol_ext_container_empty_l;

// Transmission-Bandwidth-asymmetric ::= SEQUENCE
struct tx_bw_asymmetric_s {
  bool                               ie_exts_present = false;
  tx_bw_s                            ul_tx_bw;
  tx_bw_s                            dl_tx_bw;
  tx_bw_asymmetric_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct tdd_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct tdd_info_ext_ies_container {
  bool                      intended_tdd_dl_ul_cfg_present  = false;
  bool                      tdd_ul_dl_cfg_common_nr_present = false;
  bool                      carrier_list_present            = false;
  bool                      tx_bw_asymmetric_present        = false;
  intended_tdd_dl_ul_cfg_s  intended_tdd_dl_ul_cfg;
  unbounded_octstring<true> tdd_ul_dl_cfg_common_nr;
  nr_carrier_list_l         carrier_list;
  tx_bw_asymmetric_s        tx_bw_asymmetric;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TDD-Info ::= SEQUENCE
struct tdd_info_s {
  bool                       ie_exts_present = false;
  nr_freq_info_s             nr_freq_info;
  tx_bw_s                    tx_bw;
  tdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-Channel-Info-List-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_u_ch_info_list_ext_ies_o = protocol_ext_empty_o;

using nr_u_ch_info_list_ext_ies_container = protocol_ext_container_empty_l;

// NR-U-Channel-Info-Item ::= SEQUENCE
struct nr_u_ch_info_item_s {
  struct bw_opts {
    enum options { mhz_neg10, mhz_neg20, mhz_neg40, mhz_neg60, mhz_neg80, /*...*/ mhz_neg100, nulltype } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using bw_e_ = enumerated<bw_opts, true, 1>;

  // member variables
  bool                                ie_exts_present = false;
  uint8_t                             nr_u_ch_id      = 1;
  uint32_t                            nr_arfcn        = 0;
  bw_e_                               bw;
  nr_u_ch_info_list_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-Channel-Info-List ::= SEQUENCE (SIZE (1..16)) OF NR-U-Channel-Info-Item
using nr_u_ch_info_list_l = dyn_array<nr_u_ch_info_item_s>;

// NR-Mode-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct nr_mode_info_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { nr_u, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::nr_u; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    nr_u_ch_info_list_l&       nr_u() { return c; }
    const nr_u_ch_info_list_l& nr_u() const { return c; }

  private:
    nr_u_ch_info_list_l c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// NR-Mode-Info ::= CHOICE
struct nr_mode_info_c {
  struct types_opts {
    enum options { fdd, tdd, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  nr_mode_info_c() = default;
  nr_mode_info_c(const nr_mode_info_c& other);
  nr_mode_info_c& operator=(const nr_mode_info_c& other);
  ~nr_mode_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fdd_info_s& fdd()
  {
    assert_choice_type(types::fdd, type_, "NR-Mode-Info");
    return c.get<fdd_info_s>();
  }
  tdd_info_s& tdd()
  {
    assert_choice_type(types::tdd, type_, "NR-Mode-Info");
    return c.get<tdd_info_s>();
  }
  protocol_ie_single_container_s<nr_mode_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NR-Mode-Info");
    return c.get<protocol_ie_single_container_s<nr_mode_info_ext_ies_o>>();
  }
  const fdd_info_s& fdd() const
  {
    assert_choice_type(types::fdd, type_, "NR-Mode-Info");
    return c.get<fdd_info_s>();
  }
  const tdd_info_s& tdd() const
  {
    assert_choice_type(types::tdd, type_, "NR-Mode-Info");
    return c.get<tdd_info_s>();
  }
  const protocol_ie_single_container_s<nr_mode_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NR-Mode-Info");
    return c.get<protocol_ie_single_container_s<nr_mode_info_ext_ies_o>>();
  }
  fdd_info_s&                                             set_fdd();
  tdd_info_s&                                             set_tdd();
  protocol_ie_single_container_s<nr_mode_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                           type_;
  choice_buffer_t<fdd_info_s, protocol_ie_single_container_s<nr_mode_info_ext_ies_o>, tdd_info_s> c;

  void destroy_();
};

// VictimgNBSetID-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using victim_gnb_set_id_ext_ies_o = protocol_ext_empty_o;

using victim_gnb_set_id_ext_ies_container = protocol_ext_container_empty_l;

// VictimgNBSetID ::= SEQUENCE
struct victim_gnb_set_id_s {
  bool                                ie_exts_present = false;
  fixed_bitstring<22, false, true>    victim_gnb_set_id;
  victim_gnb_set_id_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-Info-IAB-DU-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_info_iab_du_ext_ies_o = protocol_ext_empty_o;

using iab_info_iab_du_ext_ies_container = protocol_ext_container_empty_l;

// IAB-Info-IAB-DU ::= SEQUENCE
struct iab_info_iab_du_s {
  bool                              mux_info_present     = false;
  bool                              iab_stc_info_present = false;
  bool                              ie_exts_present      = false;
  mux_info_s                        mux_info;
  iab_stc_info_s                    iab_stc_info;
  iab_info_iab_du_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SFN-Offset-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sfn_offset_ext_ies_o = protocol_ext_empty_o;

using sfn_offset_ext_ies_container = protocol_ext_container_empty_l;

// SFN-Offset ::= SEQUENCE
struct sfn_offset_s {
  bool                             ie_exts_present = false;
  fixed_bitstring<24, false, true> sfn_time_offset;
  sfn_offset_ext_ies_container     ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Supported-MBS-FSA-ID-List ::= SEQUENCE (SIZE (1..256)) OF OCTET STRING (SIZE (3))
using supported_mbs_fsa_id_list_l = dyn_array<fixed_octstring<3, true>>;

// XR-Bcast-Information ::= ENUMERATED
struct xr_bcast_info_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using xr_bcast_info_e = enumerated<xr_bcast_info_opts, true>;

// Served-Cell-Information-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct served_cell_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct served_cell_info_ext_ies_container {
  bool                               ranac_present                            = false;
  bool                               extended_served_plmns_list_present       = false;
  bool                               cell_direction_present                   = false;
  bool                               bplmn_id_info_list_present               = false;
  bool                               cell_type_present                        = false;
  bool                               cfg_tac_ind_present                      = false;
  bool                               aggressor_gnb_set_id_present             = false;
  bool                               victim_gnb_set_id_present                = false;
  bool                               iab_info_iab_du_present                  = false;
  bool                               ssb_positions_in_burst_present           = false;
  bool                               nr_prach_cfg_present                     = false;
  bool                               sfn_offset_present                       = false;
  bool                               npn_broadcast_info_present               = false;
  bool                               supported_mbs_fsa_id_list_present        = false;
  bool                               redcap_bcast_info_present                = false;
  bool                               e_redcap_bcast_info_present              = false;
  bool                               xr_bcast_info_present                    = false;
  bool                               barr_exemptionfor_emer_call_info_present = false;
  uint16_t                           ranac;
  extended_served_plmns_list_l       extended_served_plmns_list;
  cell_direction_e                   cell_direction;
  bplmn_id_info_list_l               bplmn_id_info_list;
  cell_type_s                        cell_type;
  cfg_tac_ind_e                      cfg_tac_ind;
  aggressor_gnb_set_id_s             aggressor_gnb_set_id;
  victim_gnb_set_id_s                victim_gnb_set_id;
  iab_info_iab_du_s                  iab_info_iab_du;
  ssb_positions_in_burst_c           ssb_positions_in_burst;
  nr_prach_cfg_s                     nr_prach_cfg;
  sfn_offset_s                       sfn_offset;
  npn_broadcast_info_c               npn_broadcast_info;
  supported_mbs_fsa_id_list_l        supported_mbs_fsa_id_list;
  fixed_bitstring<8, false, true>    redcap_bcast_info;
  fixed_bitstring<8, false, true>    e_redcap_bcast_info;
  xr_bcast_info_e                    xr_bcast_info;
  barr_exemptionfor_emer_call_info_e barr_exemptionfor_emer_call_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Served-Cell-Information ::= SEQUENCE
struct served_cell_info_s {
  bool                               five_gs_tac_present = false;
  bool                               cfg_eps_tac_present = false;
  bool                               ie_exts_present     = false;
  nr_cgi_s                           nr_cgi;
  uint16_t                           nr_pci = 0;
  fixed_octstring<3, true>           five_gs_tac;
  fixed_octstring<2, true>           cfg_eps_tac;
  served_plmns_list_l                served_plmns;
  nr_mode_info_c                     nr_mode_info;
  unbounded_octstring<true>          meas_timing_cfg;
  served_cell_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-DU-System-Information-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct gnb_du_sys_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct gnb_du_sys_info_ext_ies_container {
  bool                      sib12_msg_present    = false;
  bool                      sib13_msg_present    = false;
  bool                      sib14_msg_present    = false;
  bool                      sib10_msg_present    = false;
  bool                      sib17_msg_present    = false;
  bool                      sib20_msg_present    = false;
  bool                      sib15_msg_present    = false;
  bool                      sib24_msg_present    = false;
  bool                      sib22_msg_present    = false;
  bool                      sib23_msg_present    = false;
  bool                      sib17bis_msg_present = false;
  unbounded_octstring<true> sib12_msg;
  unbounded_octstring<true> sib13_msg;
  unbounded_octstring<true> sib14_msg;
  unbounded_octstring<true> sib10_msg;
  unbounded_octstring<true> sib17_msg;
  unbounded_octstring<true> sib20_msg;
  unbounded_octstring<true> sib15_msg;
  unbounded_octstring<true> sib24_msg;
  unbounded_octstring<true> sib22_msg;
  unbounded_octstring<true> sib23_msg;
  unbounded_octstring<true> sib17bis_msg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-DU-System-Information ::= SEQUENCE
struct gnb_du_sys_info_s {
  bool                              ie_exts_present = false;
  unbounded_octstring<true>         mib_msg;
  unbounded_octstring<true>         sib1_msg;
  gnb_du_sys_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-DU-Served-Cells-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gnb_du_served_cells_item_ext_ies_o = protocol_ext_empty_o;

using gnb_du_served_cells_item_ext_ies_container = protocol_ext_container_empty_l;

// GNB-DU-Served-Cells-Item ::= SEQUENCE
struct gnb_du_served_cells_item_s {
  bool                                       gnb_du_sys_info_present = false;
  bool                                       ie_exts_present         = false;
  served_cell_info_s                         served_cell_info;
  gnb_du_sys_info_s                          gnb_du_sys_info;
  gnb_du_served_cells_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-DU-TNL-Association-To-Remove-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gnb_du_tnl_assoc_to_rem_item_ext_ies_o = protocol_ext_empty_o;

using gnb_du_tnl_assoc_to_rem_item_ext_ies_container = protocol_ext_container_empty_l;

// GNB-DU-TNL-Association-To-Remove-Item ::= SEQUENCE
struct gnb_du_tnl_assoc_to_rem_item_s {
  bool                                           tnl_assoc_transport_layer_address_gnb_cu_present = false;
  bool                                           ie_exts_present                                  = false;
  cp_transport_layer_address_c                   tnl_assoc_transport_layer_address;
  cp_transport_layer_address_c                   tnl_assoc_transport_layer_address_gnb_cu;
  gnb_du_tnl_assoc_to_rem_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-DUConfigurationQuery ::= ENUMERATED
struct gnb_du_cfg_query_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using gnb_du_cfg_query_e = enumerated<gnb_du_cfg_query_opts, true>;

// GNB-ID-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using gnb_id_ext_ies_o = protocol_ies_empty_o;

// GNB-ID ::= CHOICE
struct gnb_id_c {
  struct types_opts {
    enum options { gnb_id, choice_exts, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  gnb_id_c() = default;
  gnb_id_c(const gnb_id_c& other);
  gnb_id_c& operator=(const gnb_id_c& other);
  ~gnb_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  bounded_bitstring<22, 32, false, true>& gnb_id()
  {
    assert_choice_type(types::gnb_id, type_, "GNB-ID");
    return c.get<bounded_bitstring<22, 32, false, true>>();
  }
  protocol_ie_single_container_s<gnb_id_ext_ies_o>& choice_exts()
  {
    assert_choice_type(types::choice_exts, type_, "GNB-ID");
    return c.get<protocol_ie_single_container_s<gnb_id_ext_ies_o>>();
  }
  const bounded_bitstring<22, 32, false, true>& gnb_id() const
  {
    assert_choice_type(types::gnb_id, type_, "GNB-ID");
    return c.get<bounded_bitstring<22, 32, false, true>>();
  }
  const protocol_ie_single_container_s<gnb_id_ext_ies_o>& choice_exts() const
  {
    assert_choice_type(types::choice_exts, type_, "GNB-ID");
    return c.get<protocol_ie_single_container_s<gnb_id_ext_ies_o>>();
  }
  bounded_bitstring<22, 32, false, true>&           set_gnb_id();
  protocol_ie_single_container_s<gnb_id_ext_ies_o>& set_choice_exts();

private:
  types                                                                                                     type_;
  choice_buffer_t<bounded_bitstring<22, 32, false, true>, protocol_ie_single_container_s<gnb_id_ext_ies_o>> c;

  void destroy_();
};

// GNBRxTxTimeDiffMeas-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct gnb_rx_tx_time_diff_meas_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options {
        report_granularitykminus1,
        report_granularitykminus2,
        report_granularitykminus3,
        report_granularitykminus4,
        report_granularitykminus5,
        report_granularitykminus6,
        nulltype
      } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    value_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint32_t&       report_granularitykminus1();
    uint32_t&       report_granularitykminus2();
    uint32_t&       report_granularitykminus3();
    uint32_t&       report_granularitykminus4();
    uint32_t&       report_granularitykminus5();
    uint32_t&       report_granularitykminus6();
    const uint32_t& report_granularitykminus1() const;
    const uint32_t& report_granularitykminus2() const;
    const uint32_t& report_granularitykminus3() const;
    const uint32_t& report_granularitykminus4() const;
    const uint32_t& report_granularitykminus5() const;
    const uint32_t& report_granularitykminus6() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// GNBRxTxTimeDiffMeas ::= CHOICE
struct gnb_rx_tx_time_diff_meas_c {
  struct types_opts {
    enum options { k0, k1, k2, k3, k4, k5, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  gnb_rx_tx_time_diff_meas_c() = default;
  gnb_rx_tx_time_diff_meas_c(const gnb_rx_tx_time_diff_meas_c& other);
  gnb_rx_tx_time_diff_meas_c& operator=(const gnb_rx_tx_time_diff_meas_c& other);
  ~gnb_rx_tx_time_diff_meas_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint32_t& k0()
  {
    assert_choice_type(types::k0, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  uint32_t& k1()
  {
    assert_choice_type(types::k1, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  uint32_t& k2()
  {
    assert_choice_type(types::k2, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  uint32_t& k3()
  {
    assert_choice_type(types::k3, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  uint32_t& k4()
  {
    assert_choice_type(types::k4, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  uint16_t& k5()
  {
    assert_choice_type(types::k5, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint16_t>();
  }
  protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "GNBRxTxTimeDiffMeas");
    return c.get<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>();
  }
  const uint32_t& k0() const
  {
    assert_choice_type(types::k0, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  const uint32_t& k1() const
  {
    assert_choice_type(types::k1, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  const uint32_t& k2() const
  {
    assert_choice_type(types::k2, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  const uint32_t& k3() const
  {
    assert_choice_type(types::k3, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  const uint32_t& k4() const
  {
    assert_choice_type(types::k4, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint32_t>();
  }
  const uint16_t& k5() const
  {
    assert_choice_type(types::k5, type_, "GNBRxTxTimeDiffMeas");
    return c.get<uint16_t>();
  }
  const protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "GNBRxTxTimeDiffMeas");
    return c.get<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>>();
  }
  uint32_t&                                                           set_k0();
  uint32_t&                                                           set_k1();
  uint32_t&                                                           set_k2();
  uint32_t&                                                           set_k3();
  uint32_t&                                                           set_k4();
  uint16_t&                                                           set_k5();
  protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>& set_choice_ext();

private:
  types                                                                               type_;
  choice_buffer_t<protocol_ie_single_container_s<gnb_rx_tx_time_diff_meas_ext_ies_o>> c;

  void destroy_();
};

// RxTxTimingErrorMargin ::= ENUMERATED
struct rx_tx_timing_error_margin_opts {
  enum options {
    tc0dot5,
    tc1,
    tc2,
    tc4,
    tc8,
    tc12,
    tc16,
    tc20,
    tc24,
    tc32,
    tc40,
    tc48,
    tc64,
    tc80,
    tc96,
    tc128,
    // ...
    nulltype
  } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using rx_tx_timing_error_margin_e = enumerated<rx_tx_timing_error_margin_opts, true>;

// TRP-RxTx-TEGInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_rx_tx_teg_info_ext_ies_o = protocol_ext_empty_o;

using trp_rx_tx_teg_info_ext_ies_container = protocol_ext_container_empty_l;

// TRP-RxTx-TEGInformation ::= SEQUENCE
struct trp_rx_tx_teg_info_s {
  bool                                 ie_exts_present  = false;
  uint16_t                             trp_rx_tx_teg_id = 0;
  rx_tx_timing_error_margin_e          trp_rx_tx_timing_error_margin;
  trp_rx_tx_teg_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimingErrorMargin ::= ENUMERATED
struct timing_error_margin_opts {
  enum options {
    m0_tc,
    m2_tc,
    m4_tc,
    m6_tc,
    m8_tc,
    m12_tc,
    m16_tc,
    m20_tc,
    m24_tc,
    m32_tc,
    m40_tc,
    m48_tc,
    m56_tc,
    m64_tc,
    m72_tc,
    m80_tc,
    // ...
    nulltype
  } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using timing_error_margin_e = enumerated<timing_error_margin_opts, true>;

// TRP-Tx-TEGInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_tx_teg_info_ext_ies_o = protocol_ext_empty_o;

using trp_tx_teg_info_ext_ies_container = protocol_ext_container_empty_l;

// TRP-Tx-TEGInformation ::= SEQUENCE
struct trp_tx_teg_info_s {
  bool                              ie_exts_present = false;
  uint8_t                           trp_tx_teg_id   = 0;
  timing_error_margin_e             trp_tx_timing_error_margin;
  trp_tx_teg_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RxTxTEG-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rx_tx_teg_ext_ies_o = protocol_ext_empty_o;

using rx_tx_teg_ext_ies_container = protocol_ext_container_empty_l;

// RxTxTEG ::= SEQUENCE
struct rx_tx_teg_s {
  bool                        trp_tx_teg_info_present = false;
  bool                        ie_exts_present         = false;
  trp_rx_tx_teg_info_s        trp_rx_tx_teg_info;
  trp_tx_teg_info_s           trp_tx_teg_info;
  rx_tx_teg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-Rx-TEGInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_rx_teg_info_ext_ies_o = protocol_ext_empty_o;

using trp_rx_teg_info_ext_ies_container = protocol_ext_container_empty_l;

// TRP-Rx-TEGInformation ::= SEQUENCE
struct trp_rx_teg_info_s {
  bool                              ie_exts_present = false;
  uint8_t                           trp_rx_teg_id   = 0;
  timing_error_margin_e             trp_rx_timing_error_margin;
  trp_rx_teg_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RxTEG-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rx_teg_ext_ies_o = protocol_ext_empty_o;

using rx_teg_ext_ies_container = protocol_ext_container_empty_l;

// RxTEG ::= SEQUENCE
struct rx_teg_s {
  bool                     ie_exts_present = false;
  trp_rx_teg_info_s        trp_rx_teg_info;
  trp_tx_teg_info_s        trp_tx_teg_info;
  rx_teg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPTEGInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using trpteg_info_ext_ies_o = protocol_ies_empty_o;

// TRPTEGInformation ::= CHOICE
struct trpteg_info_c {
  struct types_opts {
    enum options { rx_tx_teg, rx_teg, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  trpteg_info_c() = default;
  trpteg_info_c(const trpteg_info_c& other);
  trpteg_info_c& operator=(const trpteg_info_c& other);
  ~trpteg_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  rx_tx_teg_s& rx_tx_teg()
  {
    assert_choice_type(types::rx_tx_teg, type_, "TRPTEGInformation");
    return c.get<rx_tx_teg_s>();
  }
  rx_teg_s& rx_teg()
  {
    assert_choice_type(types::rx_teg, type_, "TRPTEGInformation");
    return c.get<rx_teg_s>();
  }
  protocol_ie_single_container_s<trpteg_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TRPTEGInformation");
    return c.get<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>();
  }
  const rx_tx_teg_s& rx_tx_teg() const
  {
    assert_choice_type(types::rx_tx_teg, type_, "TRPTEGInformation");
    return c.get<rx_tx_teg_s>();
  }
  const rx_teg_s& rx_teg() const
  {
    assert_choice_type(types::rx_teg, type_, "TRPTEGInformation");
    return c.get<rx_teg_s>();
  }
  const protocol_ie_single_container_s<trpteg_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TRPTEGInformation");
    return c.get<protocol_ie_single_container_s<trpteg_info_ext_ies_o>>();
  }
  rx_tx_teg_s&                                           set_rx_tx_teg();
  rx_teg_s&                                              set_rx_teg();
  protocol_ie_single_container_s<trpteg_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                         type_;
  choice_buffer_t<protocol_ie_single_container_s<trpteg_info_ext_ies_o>, rx_teg_s, rx_tx_teg_s> c;

  void destroy_();
};

// GNB-RxTxTimeDiff-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct gnb_rx_tx_time_diff_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct gnb_rx_tx_time_diff_ext_ies_container {
  bool                     extended_add_path_list_present = false;
  bool                     trpteg_info_present            = false;
  extended_add_path_list_l extended_add_path_list;
  trpteg_info_c            trpteg_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-RxTxTimeDiff ::= SEQUENCE
struct gnb_rx_tx_time_diff_s {
  bool                                  ie_exts_present = false;
  gnb_rx_tx_time_diff_meas_c            rx_tx_time_diff;
  add_path_list_l                       add_path_list;
  gnb_rx_tx_time_diff_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNBDUOverloadInformation ::= ENUMERATED
struct gnb_du_overload_info_opts {
  enum options { overloaded, not_overloaded, nulltype } value;

  const char* to_string() const;
};
using gnb_du_overload_info_e = enumerated<gnb_du_overload_info_opts>;

// GNBDUUESliceMaximumBitRateItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gnb_du_ue_slice_max_bit_rate_item_ext_ies_o = protocol_ext_empty_o;

using gnb_du_ue_slice_max_bit_rate_item_ext_ies_container = protocol_ext_container_empty_l;

// GNBDUUESliceMaximumBitRateItem ::= SEQUENCE
struct gnb_du_ue_slice_max_bit_rate_item_s {
  bool                                                ie_exts_present = false;
  snssai_s                                            snssai;
  uint64_t                                            ue_slice_max_bit_rate_ul = 0;
  gnb_du_ue_slice_max_bit_rate_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNBDUUESliceMaximumBitRateList ::= SEQUENCE (SIZE (1..8)) OF GNBDUUESliceMaximumBitRateItem
using gnb_du_ue_slice_max_bit_rate_list_l = dyn_array<gnb_du_ue_slice_max_bit_rate_item_s>;

// GTPTLA-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using gtp_tla_item_ext_ies_o = protocol_ext_empty_o;

using gtp_tla_item_ext_ies_container = protocol_ext_container_empty_l;

// GTPTLA-Item ::= SEQUENCE
struct gtp_tla_item_s {
  bool                                  ie_exts_present = false;
  bounded_bitstring<1, 160, true, true> gtp_transport_layer_address;
  gtp_tla_item_ext_ies_container        ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GTPTLAs ::= SEQUENCE (SIZE (1..16)) OF GTPTLA-Item
using gtp_tlas_l = dyn_array<gtp_tla_item_s>;

// GlobalGNB-ID-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using global_gnb_id_ext_ies_o = protocol_ext_empty_o;

using global_gnb_id_ext_ies_container = protocol_ext_container_empty_l;

// GlobalGNB-ID ::= SEQUENCE
struct global_gnb_id_s {
  bool                            ie_exts_present = false;
  fixed_octstring<3, true>        plmn_id;
  gnb_id_c                        gnb_id;
  global_gnb_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HardwareLoadIndicator-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using hardware_load_ind_ext_ies_o = protocol_ext_empty_o;

using hardware_load_ind_ext_ies_container = protocol_ext_container_empty_l;

// HardwareLoadIndicator ::= SEQUENCE
struct hardware_load_ind_s {
  bool                                ie_exts_present      = false;
  uint16_t                            dl_hardware_load_ind = 0;
  uint16_t                            ul_hardware_load_ind = 0;
  hardware_load_ind_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTNLAddressUsage ::= ENUMERATED
struct iab_tnl_address_usage_opts {
  enum options { f1_c, f1_u, non_f1, /*...*/ nulltype } value;

  const char* to_string() const;
};
using iab_tnl_address_usage_e = enumerated<iab_tnl_address_usage_opts, true>;

// IAB-Allocated-TNL-Address-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_allocated_tnl_address_item_ext_ies_o = protocol_ext_empty_o;

using iab_allocated_tnl_address_item_ext_ies_container = protocol_ext_container_empty_l;

// IAB-Allocated-TNL-Address-Item ::= SEQUENCE
struct iab_allocated_tnl_address_item_s {
  bool                                             iab_tnl_address_usage_present = false;
  bool                                             ie_exts_present               = false;
  iab_tnl_address_c                                iab_tnl_address;
  iab_tnl_address_usage_e                          iab_tnl_address_usage;
  iab_allocated_tnl_address_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-Congestion-Indication-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_congestion_ind_item_ext_ies_o = protocol_ext_empty_o;

using iab_congestion_ind_item_ext_ies_container = protocol_ext_container_empty_l;

// IAB-Congestion-Indication-Item ::= SEQUENCE
struct iab_congestion_ind_item_s {
  bool                                      ie_exts_present = false;
  fixed_bitstring<10, false, true>          child_node_id;
  bh_rlc_ch_list_l                          bh_rlc_ch_list;
  iab_congestion_ind_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-Congestion-Indication-List ::= SEQUENCE (SIZE (1..1024)) OF IAB-Congestion-Indication-Item
using iab_congestion_ind_list_l = dyn_array<iab_congestion_ind_item_s>;

// IAB-Congestion-Indication-List-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_congestion_ind_list_ext_ies_o = protocol_ext_empty_o;

// IAB-MT-Cell-NA-Resource-Configuration-FDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_mt_cell_na_res_cfg_fdd_info_ext_ies_o = protocol_ext_empty_o;

using iab_mt_cell_na_res_cfg_fdd_info_ext_ies_container = protocol_ext_container_empty_l;

// IAB-MT-Cell-NA-Resource-Configuration-FDD-Info ::= SEQUENCE
struct iab_mt_cell_na_res_cfg_fdd_info_s {
  bool                                              ul_freq_info_present = false;
  bool                                              ul_tx_bw_present     = false;
  bool                                              dl_freq_info_present = false;
  bool                                              dl_tx_bw_present     = false;
  bool                                              ie_exts_present      = false;
  gnb_du_cell_res_cfg_s                             gnb_du_cell_na_res_cfg_fdd_ul;
  gnb_du_cell_res_cfg_s                             gnb_du_cell_na_res_cfg_fdd_dl;
  nr_freq_info_s                                    ul_freq_info;
  tx_bw_s                                           ul_tx_bw;
  nr_carrier_list_l                                 ul_nr_carrier_list;
  nr_freq_info_s                                    dl_freq_info;
  tx_bw_s                                           dl_tx_bw;
  nr_carrier_list_l                                 dl_nr_carrier_list;
  iab_mt_cell_na_res_cfg_fdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-MT-Cell-NA-Resource-Configuration-TDD-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_mt_cell_na_res_cfg_tdd_info_ext_ies_o = protocol_ext_empty_o;

using iab_mt_cell_na_res_cfg_tdd_info_ext_ies_container = protocol_ext_container_empty_l;

// IAB-MT-Cell-NA-Resource-Configuration-TDD-Info ::= SEQUENCE
struct iab_mt_cell_na_res_cfg_tdd_info_s {
  bool                                              nr_freq_info_present = false;
  bool                                              tx_bw_present        = false;
  bool                                              ie_exts_present      = false;
  gnb_du_cell_res_cfg_s                             gnb_du_cell_na_resourc_cfg_tdd;
  nr_freq_info_s                                    nr_freq_info;
  tx_bw_s                                           tx_bw;
  nr_carrier_list_l                                 nr_carrier_list;
  iab_mt_cell_na_res_cfg_tdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-MT-Cell-NA-Resource-Configuration-Mode-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using iab_mt_cell_na_res_cfg_mode_info_ext_ies_o = protocol_ies_empty_o;

// IAB-MT-Cell-NA-Resource-Configuration-Mode-Info ::= CHOICE
struct iab_mt_cell_na_res_cfg_mode_info_c {
  struct types_opts {
    enum options { fdd, tdd, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  iab_mt_cell_na_res_cfg_mode_info_c() = default;
  iab_mt_cell_na_res_cfg_mode_info_c(const iab_mt_cell_na_res_cfg_mode_info_c& other);
  iab_mt_cell_na_res_cfg_mode_info_c& operator=(const iab_mt_cell_na_res_cfg_mode_info_c& other);
  ~iab_mt_cell_na_res_cfg_mode_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  iab_mt_cell_na_res_cfg_fdd_info_s& fdd()
  {
    assert_choice_type(types::fdd, type_, "IAB-MT-Cell-NA-Resource-Configuration-Mode-Info");
    return c.get<iab_mt_cell_na_res_cfg_fdd_info_s>();
  }
  iab_mt_cell_na_res_cfg_tdd_info_s& tdd()
  {
    assert_choice_type(types::tdd, type_, "IAB-MT-Cell-NA-Resource-Configuration-Mode-Info");
    return c.get<iab_mt_cell_na_res_cfg_tdd_info_s>();
  }
  protocol_ie_single_container_s<iab_mt_cell_na_res_cfg_mode_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "IAB-MT-Cell-NA-Resource-Configuration-Mode-Info");
    return c.get<protocol_ie_single_container_s<iab_mt_cell_na_res_cfg_mode_info_ext_ies_o>>();
  }
  const iab_mt_cell_na_res_cfg_fdd_info_s& fdd() const
  {
    assert_choice_type(types::fdd, type_, "IAB-MT-Cell-NA-Resource-Configuration-Mode-Info");
    return c.get<iab_mt_cell_na_res_cfg_fdd_info_s>();
  }
  const iab_mt_cell_na_res_cfg_tdd_info_s& tdd() const
  {
    assert_choice_type(types::tdd, type_, "IAB-MT-Cell-NA-Resource-Configuration-Mode-Info");
    return c.get<iab_mt_cell_na_res_cfg_tdd_info_s>();
  }
  const protocol_ie_single_container_s<iab_mt_cell_na_res_cfg_mode_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "IAB-MT-Cell-NA-Resource-Configuration-Mode-Info");
    return c.get<protocol_ie_single_container_s<iab_mt_cell_na_res_cfg_mode_info_ext_ies_o>>();
  }
  iab_mt_cell_na_res_cfg_fdd_info_s&                                          set_fdd();
  iab_mt_cell_na_res_cfg_tdd_info_s&                                          set_tdd();
  protocol_ie_single_container_s<iab_mt_cell_na_res_cfg_mode_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<iab_mt_cell_na_res_cfg_fdd_info_s,
                  iab_mt_cell_na_res_cfg_tdd_info_s,
                  protocol_ie_single_container_s<iab_mt_cell_na_res_cfg_mode_info_ext_ies_o>>
      c;

  void destroy_();
};

// IABTNLAddress-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_tnl_address_item_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_address_item_ext_ies_container = protocol_ext_container_empty_l;

// IABTNLAddress-Item ::= SEQUENCE
struct iab_tnl_address_item_s {
  bool                                   ie_exts_present = false;
  iab_tnl_address_c                      iab_tnl_address;
  iab_tnl_address_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTNLAddressList ::= SEQUENCE (SIZE (1..1024)) OF IABTNLAddress-Item
using iab_tnl_address_list_l = dyn_array<iab_tnl_address_item_s>;

// IAB-TNL-Addresses-Exception-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_tnl_addresses_exception_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_addresses_exception_ext_ies_container = protocol_ext_container_empty_l;

// IAB-TNL-Addresses-Exception ::= SEQUENCE
struct iab_tnl_addresses_exception_s {
  bool                                          ie_exts_present = false;
  iab_tnl_address_list_l                        iab_tnl_address_list;
  iab_tnl_addresses_exception_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-TNL-Addresses-To-Remove-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_tnl_addresses_to_rem_item_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_addresses_to_rem_item_ext_ies_container = protocol_ext_container_empty_l;

// IAB-TNL-Addresses-To-Remove-Item ::= SEQUENCE
struct iab_tnl_addresses_to_rem_item_s {
  bool                                            ie_exts_present = false;
  iab_tnl_address_c                               iab_tnl_address;
  iab_tnl_addresses_to_rem_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABConditionalRRCMessageDeliveryIndication ::= ENUMERATED
struct iab_conditional_rrc_msg_delivery_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using iab_conditional_rrc_msg_delivery_ind_e = enumerated<iab_conditional_rrc_msg_delivery_ind_opts, true>;

using iab_congestion_ind_list_ext_ies_container = protocol_ext_container_empty_l;

// IABCongestionIndication ::= SEQUENCE
struct iab_congestion_ind_s {
  bool                                      ie_exts_present = false;
  iab_congestion_ind_list_l                 iab_congestion_ind_list;
  iab_congestion_ind_list_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTNLAddressesRequested-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using iab_tnl_addresses_requested_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_addresses_requested_ext_ies_container = protocol_ext_container_empty_l;

// IABTNLAddressesRequested ::= SEQUENCE
struct iab_tnl_addresses_requested_s {
  bool                                          tnl_addresses_or_prefixes_requested_all_traffic_present = false;
  bool                                          tnl_addresses_or_prefixes_requested_f1_c_present        = false;
  bool                                          tnl_addresses_or_prefixes_requested_f1_u_present        = false;
  bool                                          tnl_addresses_or_prefixes_requested_no_nf1_present      = false;
  bool                                          ie_exts_present                                         = false;
  uint16_t                                      tnl_addresses_or_prefixes_requested_all_traffic         = 1;
  uint16_t                                      tnl_addresses_or_prefixes_requested_f1_c                = 1;
  uint16_t                                      tnl_addresses_or_prefixes_requested_f1_u                = 1;
  uint16_t                                      tnl_addresses_or_prefixes_requested_no_nf1              = 1;
  iab_tnl_addresses_requested_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABIPv6RequestType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using iab_ip_v6_request_type_ext_ies_o = protocol_ies_empty_o;

// IABIPv6RequestType ::= CHOICE
struct iab_ip_v6_request_type_c {
  struct types_opts {
    enum options { ip_v6_address, ip_v6_prefix, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  iab_ip_v6_request_type_c() = default;
  iab_ip_v6_request_type_c(const iab_ip_v6_request_type_c& other);
  iab_ip_v6_request_type_c& operator=(const iab_ip_v6_request_type_c& other);
  ~iab_ip_v6_request_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  iab_tnl_addresses_requested_s& ip_v6_address()
  {
    assert_choice_type(types::ip_v6_address, type_, "IABIPv6RequestType");
    return c.get<iab_tnl_addresses_requested_s>();
  }
  iab_tnl_addresses_requested_s& ip_v6_prefix()
  {
    assert_choice_type(types::ip_v6_prefix, type_, "IABIPv6RequestType");
    return c.get<iab_tnl_addresses_requested_s>();
  }
  protocol_ie_single_container_s<iab_ip_v6_request_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "IABIPv6RequestType");
    return c.get<protocol_ie_single_container_s<iab_ip_v6_request_type_ext_ies_o>>();
  }
  const iab_tnl_addresses_requested_s& ip_v6_address() const
  {
    assert_choice_type(types::ip_v6_address, type_, "IABIPv6RequestType");
    return c.get<iab_tnl_addresses_requested_s>();
  }
  const iab_tnl_addresses_requested_s& ip_v6_prefix() const
  {
    assert_choice_type(types::ip_v6_prefix, type_, "IABIPv6RequestType");
    return c.get<iab_tnl_addresses_requested_s>();
  }
  const protocol_ie_single_container_s<iab_ip_v6_request_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "IABIPv6RequestType");
    return c.get<protocol_ie_single_container_s<iab_ip_v6_request_type_ext_ies_o>>();
  }
  iab_tnl_addresses_requested_s&                                    set_ip_v6_address();
  iab_tnl_addresses_requested_s&                                    set_ip_v6_prefix();
  protocol_ie_single_container_s<iab_ip_v6_request_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<iab_tnl_addresses_requested_s, protocol_ie_single_container_s<iab_ip_v6_request_type_ext_ies_o>> c;

  void destroy_();
};

// IABv4AddressesRequested-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ia_bv4_addresses_requested_ext_ies_o = protocol_ext_empty_o;

using ia_bv4_addresses_requested_ext_ies_container = protocol_ext_container_empty_l;

// IABv4AddressesRequested ::= SEQUENCE
struct ia_bv4_addresses_requested_s {
  bool                                         ie_exts_present = false;
  iab_tnl_addresses_requested_s                ia_bv4_addresses_requested;
  ia_bv4_addresses_requested_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IgnorePRACHConfiguration ::= ENUMERATED
struct ignore_prach_cfg_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ignore_prach_cfg_e = enumerated<ignore_prach_cfg_opts, true>;

// IgnoreResourceCoordinationContainer ::= ENUMERATED
struct ignore_res_coordination_container_opts {
  enum options { yes, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ignore_res_coordination_container_e = enumerated<ignore_res_coordination_container_opts, true>;

// InactivityMonitoringRequest ::= ENUMERATED
struct inactivity_monitoring_request_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using inactivity_monitoring_request_e = enumerated<inactivity_monitoring_request_opts, true>;

// InactivityMonitoringResponse ::= ENUMERATED
struct inactivity_monitoring_resp_opts {
  enum options { not_supported, /*...*/ nulltype } value;

  const char* to_string() const;
};
using inactivity_monitoring_resp_e = enumerated<inactivity_monitoring_resp_opts, true>;

// IndicationMCInactiveReception ::= ENUMERATED
struct ind_mc_inactive_reception_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ind_mc_inactive_reception_e = enumerated<ind_mc_inactive_reception_opts, true>;

// IndirectPathAddition-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using indirect_path_addition_ext_ies_o = protocol_ext_empty_o;

using indirect_path_addition_ext_ies_container = protocol_ext_container_empty_l;

// IndirectPathAddition ::= SEQUENCE
struct indirect_path_addition_s {
  bool                                     ie_exts_present = false;
  fixed_bitstring<24, false, true>         target_relay_ue_id;
  uint16_t                                 remote_ue_local_id = 0;
  indirect_path_addition_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LCStoGCSTranslation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using lc_sto_gcs_translation_ext_ies_o = protocol_ext_empty_o;

using lc_sto_gcs_translation_ext_ies_container = protocol_ext_container_empty_l;

// LCStoGCSTranslation ::= SEQUENCE
struct lc_sto_gcs_translation_s {
  bool                                     alpha_fine_present = false;
  bool                                     beta_fine_present  = false;
  bool                                     gamma_fine_present = false;
  bool                                     ie_exts_present    = false;
  uint16_t                                 alpha              = 0;
  uint8_t                                  alpha_fine         = 0;
  uint16_t                                 beta               = 0;
  uint8_t                                  beta_fine          = 0;
  uint16_t                                 gamma              = 0;
  uint8_t                                  gamma_fine         = 0;
  lc_sto_gcs_translation_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LCStoGCSTranslationList ::= SEQUENCE (SIZE (1..3)) OF LCStoGCSTranslation
using lc_sto_gcs_translation_list_l = dyn_array<lc_sto_gcs_translation_s>;

// LTEA2XServicesAuthorized-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltea2_x_services_authorized_ext_ies_o = protocol_ext_empty_o;

using ltea2_x_services_authorized_ext_ies_container = protocol_ext_container_empty_l;

// LTEA2XServicesAuthorized ::= SEQUENCE
struct ltea2_x_services_authorized_s {
  bool                                          aerial_ue_present  = false;
  bool                                          ctrller_ue_present = false;
  bool                                          ie_exts_present    = false;
  aerial_ue_e                                   aerial_ue;
  ctrller_ue_e                                  ctrller_ue;
  ltea2_x_services_authorized_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTEUESidelinkAggregateMaximumBitrate-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using lte_ue_sidelink_aggr_max_bitrate_ext_ies_o = protocol_ext_empty_o;

using lte_ue_sidelink_aggr_max_bitrate_ext_ies_container = protocol_ext_container_empty_l;

// LTEUESidelinkAggregateMaximumBitrate ::= SEQUENCE
struct lte_ue_sidelink_aggr_max_bitrate_s {
  bool                                               ie_exts_present                  = false;
  uint64_t                                           ue_lte_sidelink_aggr_max_bitrate = 0;
  lte_ue_sidelink_aggr_max_bitrate_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// VehicleUE ::= ENUMERATED
struct vehicle_ue_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using vehicle_ue_e = enumerated<vehicle_ue_opts, true>;

// PedestrianUE ::= ENUMERATED
struct pedestrian_ue_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pedestrian_ue_e = enumerated<pedestrian_ue_opts, true>;

// LTEV2XServicesAuthorized-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltev2x_services_authorized_ext_ies_o = protocol_ext_empty_o;

using ltev2x_services_authorized_ext_ies_container = protocol_ext_container_empty_l;

// LTEV2XServicesAuthorized ::= SEQUENCE
struct ltev2x_services_authorized_s {
  bool                                         vehicle_ue_present    = false;
  bool                                         pedestrian_ue_present = false;
  bool                                         ie_exts_present       = false;
  vehicle_ue_e                                 vehicle_ue;
  pedestrian_ue_e                              pedestrian_ue;
  ltev2x_services_authorized_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMCFRAResourceConfig-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltmcfra_res_cfg_item_ext_ies_o = protocol_ext_empty_o;

using ltmcfra_res_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// LTMCFRAResourceConfig-Item ::= SEQUENCE
struct ltmcfra_res_cfg_item_s {
  bool                                   ie_exts_present = false;
  nr_cgi_s                               nr_cgi;
  unbounded_octstring<true>              ltmcfra_res_cfg;
  unbounded_octstring<true>              ltmcfra_res_cfg_sul;
  ltmcfra_res_cfg_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMCFRAResourceConfig-List ::= SEQUENCE (SIZE (1..8)) OF LTMCFRAResourceConfig-Item
using ltmcfra_res_cfg_list_l = dyn_array<ltmcfra_res_cfg_item_s>;

// LTMCellSwitchInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltm_cell_switch_info_ext_ies_o = protocol_ext_empty_o;

using ltm_cell_switch_info_ext_ies_container = protocol_ext_container_empty_l;

// LTMCellSwitchInformation ::= SEQUENCE
struct ltm_cell_switch_info_s {
  bool                                   ie_exts_present = false;
  unbounded_octstring<true>              jointor_dl_tci_state_id;
  unbounded_octstring<true>              ul_tci_state_id;
  ltm_cell_switch_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMCells-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltm_cells_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using ltm_cells_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// LTMCells-ToBeReleased-Item ::= SEQUENCE
struct ltm_cells_to_be_released_item_s {
  bool                                            ie_exts_present = false;
  nr_cgi_s                                        nr_cgi;
  ltm_cells_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMCells-ToBeReleased-List ::= SEQUENCE (SIZE (1..8)) OF LTMCells-ToBeReleased-Item
using ltm_cells_to_be_released_list_l = dyn_array<ltm_cells_to_be_released_item_s>;

// SSB-TF-Configuration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_tf_cfg_ext_ies_o = protocol_ext_empty_o;

using ssb_tf_cfg_ext_ies_container = protocol_ext_container_empty_l;

// SSB-TF-Configuration ::= SEQUENCE
struct ssb_tf_cfg_s {
  struct ssb_subcarrier_spacing_opts {
    enum options { khz15, khz30, khz60, khz120, khz240, /*...*/ khz480, khz960, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using ssb_subcarrier_spacing_e_ = enumerated<ssb_subcarrier_spacing_opts, true, 2>;
  struct ssb_periodicity_opts {
    enum options { ms5, ms10, ms20, ms40, ms80, ms160, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using ssb_periodicity_e_ = enumerated<ssb_periodicity_opts, true>;

  // member variables
  bool                             ssb_position_in_burst_present = false;
  bool                             sfn_initisation_time_present  = false;
  bool                             ie_exts_present               = false;
  uint32_t                         ssb_freq                      = 0;
  ssb_subcarrier_spacing_e_        ssb_subcarrier_spacing;
  int8_t                           ssb_tx_pwr = -60;
  ssb_periodicity_e_               ssb_periodicity;
  uint8_t                          ssb_half_frame_offset = 0;
  uint8_t                          ssb_sfn_offset        = 0;
  ssb_positions_in_burst_c         ssb_position_in_burst;
  fixed_bitstring<64, false, true> sfn_initisation_time;
  ssb_tf_cfg_ext_ies_container     ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBInformationItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_info_item_ext_ies_o = protocol_ext_empty_o;

using ssb_info_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBInformationItem ::= SEQUENCE
struct ssb_info_item_s {
  bool                            ie_exts_present = false;
  ssb_tf_cfg_s                    ssb_cfg;
  uint16_t                        pci_nr = 0;
  ssb_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBInformationList ::= SEQUENCE (SIZE (1..255)) OF SSBInformationItem
using ssb_info_list_l = dyn_array<ssb_info_item_s>;

// SSBInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ssb_info_ext_ies_o = protocol_ext_empty_o;

using ssb_info_ext_ies_container = protocol_ext_container_empty_l;

// SSBInformation ::= SEQUENCE
struct ssb_info_s {
  bool                       ie_exts_present = false;
  ssb_info_list_l            ssb_info_list;
  ssb_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct ltm_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ltm_cfg_ext_ies_container {
  bool                      tci_states_cfgs_list_present = false;
  unbounded_octstring<true> tci_states_cfgs_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMConfiguration ::= SEQUENCE
struct ltm_cfg_s {
  bool                         complete_candidate_cfg_ind_present = false;
  bool                         ie_exts_present                    = false;
  ssb_info_s                   ssb_info;
  unbounded_octstring<true>    ref_cfg_info;
  complete_candidate_cfg_ind_e complete_candidate_cfg_ind;
  unbounded_octstring<true>    ltmcfra_res_cfg;
  unbounded_octstring<true>    ltmcfra_res_cfg_sul;
  ltm_cfg_ext_ies_container    ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMConfigurationIDMapping-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltm_cfg_id_map_item_ext_ies_o = protocol_ext_empty_o;

using ltm_cfg_id_map_item_ext_ies_container = protocol_ext_container_empty_l;

// LTMConfigurationIDMapping-Item ::= SEQUENCE
struct ltm_cfg_id_map_item_s {
  bool                                  ie_exts_present = false;
  nr_cgi_s                              ltm_cell_id;
  uint8_t                               ltm_cfg_id = 1;
  ltm_cfg_id_map_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMConfigurationIDMappingList ::= SEQUENCE (SIZE (1..8)) OF LTMConfigurationIDMapping-Item
using ltm_cfg_id_map_list_l = dyn_array<ltm_cfg_id_map_item_s>;

// LTMIndicator ::= ENUMERATED
struct ltm_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ltm_ind_e = enumerated<ltm_ind_opts, true>;

// RequestforLowerLayerConfiguration ::= ENUMERATED
struct requestfor_lower_layer_cfg_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using requestfor_lower_layer_cfg_e = enumerated<requestfor_lower_layer_cfg_opts, true>;

// ReferenceConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using ref_cfg_ext_ies_o = protocol_ies_empty_o;

// ReferenceConfiguration ::= CHOICE
struct ref_cfg_c {
  struct types_opts {
    enum options { req_ue_st_for_lower_layer_cfg, ref_cfg, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ref_cfg_c() = default;
  ref_cfg_c(const ref_cfg_c& other);
  ref_cfg_c& operator=(const ref_cfg_c& other);
  ~ref_cfg_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  requestfor_lower_layer_cfg_e& req_ue_st_for_lower_layer_cfg()
  {
    assert_choice_type(types::req_ue_st_for_lower_layer_cfg, type_, "ReferenceConfiguration");
    return c.get<requestfor_lower_layer_cfg_e>();
  }
  unbounded_octstring<true>& ref_cfg()
  {
    assert_choice_type(types::ref_cfg, type_, "ReferenceConfiguration");
    return c.get<unbounded_octstring<true>>();
  }
  protocol_ie_single_container_s<ref_cfg_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ReferenceConfiguration");
    return c.get<protocol_ie_single_container_s<ref_cfg_ext_ies_o>>();
  }
  const requestfor_lower_layer_cfg_e& req_ue_st_for_lower_layer_cfg() const
  {
    assert_choice_type(types::req_ue_st_for_lower_layer_cfg, type_, "ReferenceConfiguration");
    return c.get<requestfor_lower_layer_cfg_e>();
  }
  const unbounded_octstring<true>& ref_cfg() const
  {
    assert_choice_type(types::ref_cfg, type_, "ReferenceConfiguration");
    return c.get<unbounded_octstring<true>>();
  }
  const protocol_ie_single_container_s<ref_cfg_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ReferenceConfiguration");
    return c.get<protocol_ie_single_container_s<ref_cfg_ext_ies_o>>();
  }
  requestfor_lower_layer_cfg_e&                      set_req_ue_st_for_lower_layer_cfg();
  unbounded_octstring<true>&                         set_ref_cfg();
  protocol_ie_single_container_s<ref_cfg_ext_ies_o>& set_choice_ext();

private:
  types                                                                                         type_;
  choice_buffer_t<protocol_ie_single_container_s<ref_cfg_ext_ies_o>, unbounded_octstring<true>> c;

  void destroy_();
};

// LTMInformation-Modify-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltm_info_modify_ext_ies_o = protocol_ext_empty_o;

using ltm_info_modify_ext_ies_container = protocol_ext_container_empty_l;

// LTMInformation-Modify ::= SEQUENCE
struct ltm_info_modify_s {
  bool                              ref_cfg_present     = false;
  bool                              csi_res_cfg_present = false;
  bool                              ie_exts_present     = false;
  ltm_ind_e                         ltm_ind;
  ref_cfg_c                         ref_cfg;
  csi_res_cfg_s                     csi_res_cfg;
  ltm_info_modify_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMInformation-Setup-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltm_info_setup_ext_ies_o = protocol_ext_empty_o;

using ltm_info_setup_ext_ies_container = protocol_ext_container_empty_l;

// LTMInformation-Setup ::= SEQUENCE
struct ltm_info_setup_s {
  bool                             ref_cfg_present     = false;
  bool                             csi_res_cfg_present = false;
  bool                             ie_exts_present     = false;
  ltm_ind_e                        ltm_ind;
  ref_cfg_c                        ref_cfg;
  csi_res_cfg_s                    csi_res_cfg;
  ltm_info_setup_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTML2ResetConfiguration-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltml2_reset_cfg_item_ext_ies_o = protocol_ext_empty_o;

using ltml2_reset_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// LTML2ResetConfiguration-Item ::= SEQUENCE
struct ltml2_reset_cfg_item_s {
  bool                                   ie_exts_present = false;
  nr_cgi_s                               cell_id;
  unbounded_octstring<true>              ltm_l2_reset_cfg;
  ltml2_reset_cfg_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTML2ResetConfigurationList ::= SEQUENCE (SIZE (1..8)) OF LTML2ResetConfiguration-Item
using ltml2_reset_cfg_list_l = dyn_array<ltml2_reset_cfg_item_s>;

// LTMResetInformation-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltm_reset_info_item_ext_ies_o = protocol_ext_empty_o;

using ltm_reset_info_item_ext_ies_container = protocol_ext_container_empty_l;

// LTMResetInformation ::= SEQUENCE
struct ltm_reset_info_s {
  bool                                  ie_exts_present = false;
  unbounded_octstring<true>             serving_cell_l2_reset_cfg;
  ltml2_reset_cfg_list_l                ltml2_reset_cfg_list;
  ltm_reset_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMTCIStatesConfigurations-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ltmtci_states_cfgs_item_ext_ies_o = protocol_ext_empty_o;

using ltmtci_states_cfgs_item_ext_ies_container = protocol_ext_container_empty_l;

// LTMTCIStatesConfigurations-Item ::= SEQUENCE
struct ltmtci_states_cfgs_item_s {
  bool                                      ie_exts_present = false;
  nr_cgi_s                                  nr_cgi;
  unbounded_octstring<true>                 tci_states_cfgs_list;
  ltmtci_states_cfgs_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTMTCIStatesConfigurationsList ::= SEQUENCE (SIZE (1..8)) OF LTMTCIStatesConfigurations-Item
using ltmtci_states_cfgs_list_l = dyn_array<ltmtci_states_cfgs_item_s>;

// LastUsedCellIndication ::= ENUMERATED
struct last_used_cell_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using last_used_cell_ind_e = enumerated<last_used_cell_ind_opts, true>;

// LoS-NLoSIndicatorHard ::= ENUMERATED
struct lo_s_n_lo_si_ndicator_hard_opts {
  enum options { nlo_s, lo_s, nulltype } value;

  const char* to_string() const;
};
using lo_s_n_lo_si_ndicator_hard_e = enumerated<lo_s_n_lo_si_ndicator_hard_opts>;

// LoS-NLoSInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using lo_s_n_lo_si_nformation_ext_ies_o = protocol_ies_empty_o;

// LoS-NLoSInformation ::= CHOICE
struct lo_s_n_lo_si_nformation_c {
  struct types_opts {
    enum options { lo_s_n_lo_si_ndicator_soft, lo_s_n_lo_si_ndicator_hard, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  lo_s_n_lo_si_nformation_c() = default;
  lo_s_n_lo_si_nformation_c(const lo_s_n_lo_si_nformation_c& other);
  lo_s_n_lo_si_nformation_c& operator=(const lo_s_n_lo_si_nformation_c& other);
  ~lo_s_n_lo_si_nformation_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& lo_s_n_lo_si_ndicator_soft()
  {
    assert_choice_type(types::lo_s_n_lo_si_ndicator_soft, type_, "LoS-NLoSInformation");
    return c.get<uint8_t>();
  }
  lo_s_n_lo_si_ndicator_hard_e& lo_s_n_lo_si_ndicator_hard()
  {
    assert_choice_type(types::lo_s_n_lo_si_ndicator_hard, type_, "LoS-NLoSInformation");
    return c.get<lo_s_n_lo_si_ndicator_hard_e>();
  }
  protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "LoS-NLoSInformation");
    return c.get<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>();
  }
  const uint8_t& lo_s_n_lo_si_ndicator_soft() const
  {
    assert_choice_type(types::lo_s_n_lo_si_ndicator_soft, type_, "LoS-NLoSInformation");
    return c.get<uint8_t>();
  }
  const lo_s_n_lo_si_ndicator_hard_e& lo_s_n_lo_si_ndicator_hard() const
  {
    assert_choice_type(types::lo_s_n_lo_si_ndicator_hard, type_, "LoS-NLoSInformation");
    return c.get<lo_s_n_lo_si_ndicator_hard_e>();
  }
  const protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "LoS-NLoSInformation");
    return c.get<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>>();
  }
  uint8_t&                                                           set_lo_s_n_lo_si_ndicator_soft();
  lo_s_n_lo_si_ndicator_hard_e&                                      set_lo_s_n_lo_si_ndicator_hard();
  protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>& set_choice_ext();

private:
  types                                                                              type_;
  choice_buffer_t<protocol_ie_single_container_s<lo_s_n_lo_si_nformation_ext_ies_o>> c;

  void destroy_();
};

// LongNonIntegerDRXCycleLength ::= ENUMERATED
struct long_non_integer_drx_cycle_len_opts {
  enum options {
    ms1001over240,
    ms25over6,
    ms25over3,
    ms1001over120,
    ms100over9,
    ms25over2,
    ms40over3,
    ms125over9,
    ms50over3,
    ms1001over60,
    ms125over6,
    ms200over9,
    ms250over9,
    ms100over3,
    ms1001over30,
    ms75over2,
    ms125over3,
    ms1001over24,
    ms200over3,
    ms1001over15,
    ms250over3,
    ms1001over12,
    ms400over3,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using long_non_integer_drx_cycle_len_e = enumerated<long_non_integer_drx_cycle_len_opts, true>;

// LowerLayerPresenceStatusChange ::= ENUMERATED
struct lower_layer_presence_status_change_opts {
  enum options { suspend_lower_layers, resume_lower_layers, /*...*/ nulltype } value;

  const char* to_string() const;
};
using lower_layer_presence_status_change_e = enumerated<lower_layer_presence_status_change_opts, true>;

// M2Configuration ::= ENUMERATED
struct m2_cfg_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using m2_cfg_e = enumerated<m2_cfg_opts, true>;

// M5-Links-to-log ::= ENUMERATED
struct m5_links_to_log_opts {
  enum options { ul, dl, both_ul_and_dl, /*...*/ nulltype } value;

  const char* to_string() const;
};
using m5_links_to_log_e = enumerated<m5_links_to_log_opts, true>;

// M5period ::= ENUMERATED
struct m5period_opts {
  enum options { ms1024, ms2048, ms5120, ms10240, min1, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using m5period_e = enumerated<m5period_opts, true>;

// M5ReportAmount ::= ENUMERATED
struct m5_report_amount_opts {
  enum options { r1, r2, r4, r8, r16, r32, r64, infinity, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using m5_report_amount_e = enumerated<m5_report_amount_opts, true>;

// M5Configuration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct m5_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m5_cfg_ext_ies_container {
  bool               m5_report_amount_present = false;
  m5_report_amount_e m5_report_amount;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M5Configuration ::= SEQUENCE
struct m5_cfg_s {
  bool                     ie_exts_present = false;
  m5period_e               m5period;
  m5_links_to_log_e        m5_links_to_log;
  m5_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M6-Links-to-log ::= ENUMERATED
struct m6_links_to_log_opts {
  enum options { ul, dl, both_ul_and_dl, /*...*/ nulltype } value;

  const char* to_string() const;
};
using m6_links_to_log_e = enumerated<m6_links_to_log_opts, true>;

// M6report-Interval ::= ENUMERATED
struct m6report_interv_opts {
  enum options {
    ms120,
    ms240,
    ms640,
    ms1024,
    ms2048,
    ms5120,
    ms10240,
    ms20480,
    ms40960,
    min1,
    min6,
    min12,
    min30,
    // ...
    ms480,
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using m6report_interv_e = enumerated<m6report_interv_opts, true, 1>;

// M6ReportAmount ::= ENUMERATED
struct m6_report_amount_opts {
  enum options { r1, r2, r4, r8, r16, r32, r64, infinity, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using m6_report_amount_e = enumerated<m6_report_amount_opts, true>;

// M6Configuration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct m6_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m6_cfg_ext_ies_container {
  bool               m6_report_amount_present = false;
  m6_report_amount_e m6_report_amount;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M6Configuration ::= SEQUENCE
struct m6_cfg_s {
  bool                     ie_exts_present = false;
  m6report_interv_e        m6report_interv;
  m6_links_to_log_e        m6_links_to_log;
  m6_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M7-Links-to-log ::= ENUMERATED
struct m7_links_to_log_opts {
  enum options { dl, /*...*/ nulltype } value;

  const char* to_string() const;
};
using m7_links_to_log_e = enumerated<m7_links_to_log_opts, true>;

// M7ReportAmount ::= ENUMERATED
struct m7_report_amount_opts {
  enum options { r1, r2, r4, r8, r16, r32, r64, infinity, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using m7_report_amount_e = enumerated<m7_report_amount_opts, true>;

// M7Configuration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct m7_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m7_cfg_ext_ies_container {
  bool               m7_report_amount_present = false;
  m7_report_amount_e m7_report_amount;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M7Configuration ::= SEQUENCE
struct m7_cfg_s {
  bool                     ie_exts_present = false;
  uint8_t                  m7period        = 1;
  m7_links_to_log_e        m7_links_to_log;
  m7_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Broadcast-Cell-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_broadcast_cell_item_ext_ies_o = protocol_ext_empty_o;

using mbs_broadcast_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-Broadcast-Cell-Item ::= SEQUENCE
struct mbs_broadcast_cell_item_s {
  bool                                      ie_exts_present = false;
  nr_cgi_s                                  nr_cgi;
  unbounded_octstring<true>                 mtch_neighbour_cell;
  mbs_broadcast_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Broadcast-Cell-List ::= SEQUENCE (SIZE (1..512)) OF MBS-Broadcast-Cell-Item
using mbs_broadcast_cell_list_l = dyn_array<mbs_broadcast_cell_item_s>;

// MBS-Broadcast-MRB-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_broadcast_mrb_item_ext_ies_o = protocol_ext_empty_o;

using mbs_broadcast_mrb_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-Broadcast-MRB-Item ::= SEQUENCE
struct mbs_broadcast_mrb_item_s {
  bool                                     ie_exts_present = false;
  uint16_t                                 mrb_id          = 1;
  unbounded_octstring<true>                mrb_pdcp_cfg_broadcast;
  mbs_broadcast_mrb_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Broadcast-MRB-List ::= SEQUENCE (SIZE (1..32)) OF MBS-Broadcast-MRB-Item
using mbs_broadcast_mrb_list_l = dyn_array<mbs_broadcast_mrb_item_s>;

// MBS-CUtoDURRCInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_cu_to_du_rrc_info_ext_ies_o = protocol_ext_empty_o;

using mbs_cu_to_du_rrc_info_ext_ies_container = protocol_ext_container_empty_l;

// MBS-CUtoDURRCInformation ::= SEQUENCE
struct mbs_cu_to_du_rrc_info_s {
  bool                                    ie_exts_present = false;
  mbs_broadcast_cell_list_l               mbs_broadcast_cell_list;
  mbs_broadcast_mrb_list_l                mbs_broadcast_mrb_list;
  mbs_cu_to_du_rrc_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBSMulticastRRCINACTIVEReceptionMode ::= ENUMERATED
struct mbs_multicast_rrc_inactive_reception_mode_opts {
  enum options { activ, deactiv, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mbs_multicast_rrc_inactive_reception_mode_e = enumerated<mbs_multicast_rrc_inactive_reception_mode_opts, true>;

// MBS-Multicast-CU2DU-Cell-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_multicast_cu2_du_cell_item_ext_ies_o = protocol_ext_empty_o;

using mbs_multicast_cu2_du_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-Multicast-CU2DU-Cell-Item ::= SEQUENCE
struct mbs_multicast_cu2_du_cell_item_s {
  struct mbs_multicast_cfg_request_opts {
    enum options { query, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using mbs_multicast_cfg_request_e_ = enumerated<mbs_multicast_cfg_request_opts, true>;

  // member variables
  bool                                             mbs_multicast_rrc_inactive_reception_mode_present = false;
  bool                                             mbs_multicast_cfg_request_present                 = false;
  bool                                             ie_exts_present                                   = false;
  nr_cgi_s                                         nr_cgi;
  mbs_multicast_rrc_inactive_reception_mode_e      mbs_multicast_rrc_inactive_reception_mode;
  mbs_multicast_cfg_request_e_                     mbs_multicast_cfg_request;
  mbs_multicast_cu2_du_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Multicast-CU2DU-Cell-List ::= SEQUENCE (SIZE (1..512)) OF MBS-Multicast-CU2DU-Cell-Item
using mbs_multicast_cu2_du_cell_list_l = dyn_array<mbs_multicast_cu2_du_cell_item_s>;

// MBSMulticastConfiguration-available-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_multicast_cfg_available_ext_ies_o = protocol_ext_empty_o;

using mbs_multicast_cfg_available_ext_ies_container = protocol_ext_container_empty_l;

// MBSMulticastConfiguration-available ::= SEQUENCE
struct mbs_multicast_cfg_available_s {
  bool                                          ie_exts_present = false;
  unbounded_octstring<true>                     mbs_multicast_cfg;
  mbs_multicast_cfg_available_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBSMulticastConfiguration-notavailable-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_multicast_cfg_notavailable_ext_ies_o = protocol_ext_empty_o;

using mbs_multicast_cfg_notavailable_ext_ies_container = protocol_ext_container_empty_l;

// MBSMulticastConfiguration-notavailable ::= SEQUENCE
struct mbs_multicast_cfg_notavailable_s {
  struct mbs_multicast_cfg_notavailable_opts {
    enum options { not_available, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using mbs_multicast_cfg_notavailable_e_ = enumerated<mbs_multicast_cfg_notavailable_opts, true>;

  // member variables
  bool                                             ie_exts_present = false;
  mbs_multicast_cfg_notavailable_e_                mbs_multicast_cfg_notavailable;
  mbs_multicast_cfg_notavailable_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBSMulticastConfigurationResponseInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using mbs_multicast_cfg_resp_info_ext_ies_o = protocol_ies_empty_o;

// MBSMulticastConfigurationResponseInfo ::= CHOICE
struct mbs_multicast_cfg_resp_info_c {
  struct types_opts {
    enum options { mbs_multicast_cfg_available, mbs_multicast_cfg_notavailable, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mbs_multicast_cfg_resp_info_c() = default;
  mbs_multicast_cfg_resp_info_c(const mbs_multicast_cfg_resp_info_c& other);
  mbs_multicast_cfg_resp_info_c& operator=(const mbs_multicast_cfg_resp_info_c& other);
  ~mbs_multicast_cfg_resp_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  mbs_multicast_cfg_available_s& mbs_multicast_cfg_available()
  {
    assert_choice_type(types::mbs_multicast_cfg_available, type_, "MBSMulticastConfigurationResponseInfo");
    return c.get<mbs_multicast_cfg_available_s>();
  }
  mbs_multicast_cfg_notavailable_s& mbs_multicast_cfg_notavailable()
  {
    assert_choice_type(types::mbs_multicast_cfg_notavailable, type_, "MBSMulticastConfigurationResponseInfo");
    return c.get<mbs_multicast_cfg_notavailable_s>();
  }
  protocol_ie_single_container_s<mbs_multicast_cfg_resp_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MBSMulticastConfigurationResponseInfo");
    return c.get<protocol_ie_single_container_s<mbs_multicast_cfg_resp_info_ext_ies_o>>();
  }
  const mbs_multicast_cfg_available_s& mbs_multicast_cfg_available() const
  {
    assert_choice_type(types::mbs_multicast_cfg_available, type_, "MBSMulticastConfigurationResponseInfo");
    return c.get<mbs_multicast_cfg_available_s>();
  }
  const mbs_multicast_cfg_notavailable_s& mbs_multicast_cfg_notavailable() const
  {
    assert_choice_type(types::mbs_multicast_cfg_notavailable, type_, "MBSMulticastConfigurationResponseInfo");
    return c.get<mbs_multicast_cfg_notavailable_s>();
  }
  const protocol_ie_single_container_s<mbs_multicast_cfg_resp_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MBSMulticastConfigurationResponseInfo");
    return c.get<protocol_ie_single_container_s<mbs_multicast_cfg_resp_info_ext_ies_o>>();
  }
  mbs_multicast_cfg_available_s&                                         set_mbs_multicast_cfg_available();
  mbs_multicast_cfg_notavailable_s&                                      set_mbs_multicast_cfg_notavailable();
  protocol_ie_single_container_s<mbs_multicast_cfg_resp_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<mbs_multicast_cfg_available_s,
                  mbs_multicast_cfg_notavailable_s,
                  protocol_ie_single_container_s<mbs_multicast_cfg_resp_info_ext_ies_o>>
      c;

  void destroy_();
};

// MBSMulticastConfigurationNotificationInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using mbs_multicast_cfg_notif_info_ext_ies_o = protocol_ies_empty_o;

// MBSMulticastConfigurationNotificationInfo ::= CHOICE
struct mbs_multicast_cfg_notif_info_c {
  struct types_opts {
    enum options { mbs_multicast_cfg_changed, mbs_multicast_cfg_remd, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mbs_multicast_cfg_notif_info_c() = default;
  mbs_multicast_cfg_notif_info_c(const mbs_multicast_cfg_notif_info_c& other);
  mbs_multicast_cfg_notif_info_c& operator=(const mbs_multicast_cfg_notif_info_c& other);
  ~mbs_multicast_cfg_notif_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  unbounded_octstring<true>& mbs_multicast_cfg_changed()
  {
    assert_choice_type(types::mbs_multicast_cfg_changed, type_, "MBSMulticastConfigurationNotificationInfo");
    return c.get<unbounded_octstring<true>>();
  }
  protocol_ie_single_container_s<mbs_multicast_cfg_notif_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MBSMulticastConfigurationNotificationInfo");
    return c.get<protocol_ie_single_container_s<mbs_multicast_cfg_notif_info_ext_ies_o>>();
  }
  const unbounded_octstring<true>& mbs_multicast_cfg_changed() const
  {
    assert_choice_type(types::mbs_multicast_cfg_changed, type_, "MBSMulticastConfigurationNotificationInfo");
    return c.get<unbounded_octstring<true>>();
  }
  const protocol_ie_single_container_s<mbs_multicast_cfg_notif_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MBSMulticastConfigurationNotificationInfo");
    return c.get<protocol_ie_single_container_s<mbs_multicast_cfg_notif_info_ext_ies_o>>();
  }
  unbounded_octstring<true>&                                              set_mbs_multicast_cfg_changed();
  void                                                                    set_mbs_multicast_cfg_remd();
  protocol_ie_single_container_s<mbs_multicast_cfg_notif_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<mbs_multicast_cfg_notif_info_ext_ies_o>, unbounded_octstring<true>> c;

  void destroy_();
};

// MBSMulticastConfigurationNotification-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_multicast_cfg_notif_ext_ies_o = protocol_ext_empty_o;

using mbs_multicast_cfg_notif_ext_ies_container = protocol_ext_container_empty_l;

// MBSMulticastConfigurationNotification ::= SEQUENCE
struct mbs_multicast_cfg_notif_s {
  bool                                      mbs_multicast_cfg_notif_info_present = false;
  bool                                      ie_exts_present                      = false;
  mbs_multicast_cfg_notif_info_c            mbs_multicast_cfg_notif_info;
  mbs_multicast_cfg_notif_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Multicast-DU2CU-Cell-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_multicast_du2_cu_cell_item_ext_ies_o = protocol_ext_empty_o;

using mbs_multicast_du2_cu_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-Multicast-DU2CU-Cell-Item ::= SEQUENCE
struct mbs_multicast_du2_cu_cell_item_s {
  bool                                             mbs_multicast_cfg_resp_info_present = false;
  bool                                             mbs_multicast_cfg_notif_present     = false;
  bool                                             ie_exts_present                     = false;
  nr_cgi_s                                         nr_cgi;
  mbs_multicast_cfg_resp_info_c                    mbs_multicast_cfg_resp_info;
  mbs_multicast_cfg_notif_s                        mbs_multicast_cfg_notif;
  mbs_multicast_du2_cu_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Multicast-DU2CU-Cell-List ::= SEQUENCE (SIZE (1..512)) OF MBS-Multicast-DU2CU-Cell-Item
using mbs_multicast_du2_cu_cell_list_l = dyn_array<mbs_multicast_du2_cu_cell_item_s>;

// MBS-Multicast-MRB-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_multicast_mrb_item_ext_ies_o = protocol_ext_empty_o;

using mbs_multicast_mrb_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-Multicast-MRB-Item ::= SEQUENCE
struct mbs_multicast_mrb_item_s {
  bool                                     ie_exts_present = false;
  uint16_t                                 mrb_id          = 1;
  unbounded_octstring<true>                mrb_pdcp_cfg_broadcast;
  mbs_multicast_mrb_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-Multicast-MRB-List ::= SEQUENCE (SIZE (1..32)) OF MBS-Multicast-MRB-Item
using mbs_multicast_mrb_list_l = dyn_array<mbs_multicast_mrb_item_s>;

// MBS-ServiceAreaCellList ::= SEQUENCE (SIZE (1..512)) OF NRCGI
using mbs_service_area_cell_list_l = dyn_array<nr_cgi_s>;

// MBS-ServiceAreaTAIList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_service_area_tai_list_item_ext_ies_o = protocol_ext_empty_o;

using mbs_service_area_tai_list_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-ServiceAreaTAIList-Item ::= SEQUENCE
struct mbs_service_area_tai_list_item_s {
  bool                                             ie_exts_present = false;
  fixed_octstring<3, true>                         plmn_id;
  fixed_octstring<3, true>                         five_gs_tac;
  mbs_service_area_tai_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-ServiceAreaTAIList ::= SEQUENCE (SIZE (1..512)) OF MBS-ServiceAreaTAIList-Item
using mbs_service_area_tai_list_l = dyn_array<mbs_service_area_tai_list_item_s>;

// MBS-ServiceAreaInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_service_area_info_ext_ies_o = protocol_ext_empty_o;

using mbs_service_area_info_ext_ies_container = protocol_ext_container_empty_l;

// MBS-ServiceAreaInformation ::= SEQUENCE
struct mbs_service_area_info_s {
  bool                                    ie_exts_present = false;
  mbs_service_area_cell_list_l            mbs_service_area_cell_list;
  mbs_service_area_tai_list_l             mbs_service_area_tai_list;
  mbs_service_area_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-ServiceAreaInformationItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_service_area_info_item_ext_ies_o = protocol_ext_empty_o;

using mbs_service_area_info_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-ServiceAreaInformationItem ::= SEQUENCE
struct mbs_service_area_info_item_s {
  bool                                         ie_exts_present     = false;
  uint32_t                                     mbs_area_session_id = 0;
  mbs_service_area_info_s                      mbs_service_area_info;
  mbs_service_area_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-ServiceAreaInformationList ::= SEQUENCE (SIZE (1..256)) OF MBS-ServiceAreaInformationItem
using mbs_service_area_info_list_l = dyn_array<mbs_service_area_info_item_s>;

// MBSServiceArea-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using mbs_service_area_ext_ies_o = protocol_ies_empty_o;

// MBS-ServiceArea ::= CHOICE
struct mbs_service_area_c {
  struct types_opts {
    enum options { locationindependent, locationdependent, choice_exts, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mbs_service_area_c() = default;
  mbs_service_area_c(const mbs_service_area_c& other);
  mbs_service_area_c& operator=(const mbs_service_area_c& other);
  ~mbs_service_area_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  mbs_service_area_info_s& locationindependent()
  {
    assert_choice_type(types::locationindependent, type_, "MBS-ServiceArea");
    return c.get<mbs_service_area_info_s>();
  }
  mbs_service_area_info_list_l& locationdependent()
  {
    assert_choice_type(types::locationdependent, type_, "MBS-ServiceArea");
    return c.get<mbs_service_area_info_list_l>();
  }
  protocol_ie_single_container_s<mbs_service_area_ext_ies_o>& choice_exts()
  {
    assert_choice_type(types::choice_exts, type_, "MBS-ServiceArea");
    return c.get<protocol_ie_single_container_s<mbs_service_area_ext_ies_o>>();
  }
  const mbs_service_area_info_s& locationindependent() const
  {
    assert_choice_type(types::locationindependent, type_, "MBS-ServiceArea");
    return c.get<mbs_service_area_info_s>();
  }
  const mbs_service_area_info_list_l& locationdependent() const
  {
    assert_choice_type(types::locationdependent, type_, "MBS-ServiceArea");
    return c.get<mbs_service_area_info_list_l>();
  }
  const protocol_ie_single_container_s<mbs_service_area_ext_ies_o>& choice_exts() const
  {
    assert_choice_type(types::choice_exts, type_, "MBS-ServiceArea");
    return c.get<protocol_ie_single_container_s<mbs_service_area_ext_ies_o>>();
  }
  mbs_service_area_info_s&                                    set_locationindependent();
  mbs_service_area_info_list_l&                               set_locationdependent();
  protocol_ie_single_container_s<mbs_service_area_ext_ies_o>& set_choice_exts();

private:
  types type_;
  choice_buffer_t<mbs_service_area_info_list_l,
                  mbs_service_area_info_s,
                  protocol_ie_single_container_s<mbs_service_area_ext_ies_o>>
      c;

  void destroy_();
};

// MBS-Session-ID-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_session_id_ext_ies_o = protocol_ext_empty_o;

using mbs_session_id_ext_ies_container = protocol_ext_container_empty_l;

// MBS-Session-ID ::= SEQUENCE
struct mbs_session_id_s {
  bool                             nid_present     = false;
  bool                             ie_exts_present = false;
  fixed_octstring<6, true>         tmgi;
  fixed_bitstring<44, false, true> nid;
  mbs_session_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBSMulticastF1UContextDescriptor-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mbs_multicast_f1_u_context_descriptor_ext_ies_o = protocol_ext_empty_o;

using mbs_multicast_f1_u_context_descriptor_ext_ies_container = protocol_ext_container_empty_l;

// MBSMulticastF1UContextDescriptor ::= SEQUENCE
struct mbs_multicast_f1_u_context_descriptor_s {
  struct mc_f1_u_ctxtusage_opts {
    enum options { ptm, ptp, ptp_retx, ptp_forwarding, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using mc_f1_u_ctxtusage_e_ = enumerated<mc_f1_u_ctxtusage_opts, true>;

  // member variables
  bool                                                    mbs_area_session_present = false;
  bool                                                    ie_exts_present          = false;
  fixed_octstring<4, true>                                multicast_f1_u_context_ref_f1;
  mc_f1_u_ctxtusage_e_                                    mc_f1_u_ctxtusage;
  uint32_t                                                mbs_area_session = 0;
  mbs_multicast_f1_u_context_descriptor_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MTCH-NeighbourCellInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using mtch_neighbour_cell_info_ext_ies_o = protocol_ies_empty_o;

// MTCH-NeighbourCellInformation ::= CHOICE
struct mtch_neighbour_cell_info_c {
  struct types_opts {
    enum options { mtch_neighbour_cellprovided, mtch_neighbour_cellnotprovided, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mtch_neighbour_cell_info_c() = default;
  mtch_neighbour_cell_info_c(const mtch_neighbour_cell_info_c& other);
  mtch_neighbour_cell_info_c& operator=(const mtch_neighbour_cell_info_c& other);
  ~mtch_neighbour_cell_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  unbounded_octstring<true>& mtch_neighbour_cellprovided()
  {
    assert_choice_type(types::mtch_neighbour_cellprovided, type_, "MTCH-NeighbourCellInformation");
    return c.get<unbounded_octstring<true>>();
  }
  protocol_ie_single_container_s<mtch_neighbour_cell_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MTCH-NeighbourCellInformation");
    return c.get<protocol_ie_single_container_s<mtch_neighbour_cell_info_ext_ies_o>>();
  }
  const unbounded_octstring<true>& mtch_neighbour_cellprovided() const
  {
    assert_choice_type(types::mtch_neighbour_cellprovided, type_, "MTCH-NeighbourCellInformation");
    return c.get<unbounded_octstring<true>>();
  }
  const protocol_ie_single_container_s<mtch_neighbour_cell_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MTCH-NeighbourCellInformation");
    return c.get<protocol_ie_single_container_s<mtch_neighbour_cell_info_ext_ies_o>>();
  }
  unbounded_octstring<true>&                                          set_mtch_neighbour_cellprovided();
  void                                                                set_mtch_neighbour_cellnotprovided();
  protocol_ie_single_container_s<mtch_neighbour_cell_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                          type_;
  choice_buffer_t<protocol_ie_single_container_s<mtch_neighbour_cell_info_ext_ies_o>, unbounded_octstring<true>> c;

  void destroy_();
};

// MTCH-NeighbourCellSession-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mtch_neighbour_cell_session_item_ext_ies_o = protocol_ext_empty_o;

using mtch_neighbour_cell_session_item_ext_ies_container = protocol_ext_container_empty_l;

// MTCH-NeighbourCellSession-Item ::= SEQUENCE
struct mtch_neighbour_cell_session_item_s {
  bool                                               ie_exts_present = false;
  mbs_session_id_s                                   mbs_session_id;
  mtch_neighbour_cell_info_c                         mtch_neighbour_cell_info;
  mtch_neighbour_cell_session_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MTCH-NeighbourCellSessionList ::= SEQUENCE (SIZE (1..1024)) OF MTCH-NeighbourCellSession-Item
using mtch_neighbour_cell_session_list_l = dyn_array<mtch_neighbour_cell_session_item_s>;

// UpdateMBSMulticastNeighbourCellListInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using upd_mbs_multicast_neighbour_cell_list_info_ext_ies_o = protocol_ext_empty_o;

using upd_mbs_multicast_neighbour_cell_list_info_ext_ies_container = protocol_ext_container_empty_l;

// UpdateMBSMulticastNeighbourCellListInformation ::= SEQUENCE
struct upd_mbs_multicast_neighbour_cell_list_info_s {
  bool                                                         ie_exts_present = false;
  unbounded_octstring<true>                                    mbs_neighbour_cell_list;
  mtch_neighbour_cell_session_list_l                           mbs_multicast_session_list;
  upd_mbs_multicast_neighbour_cell_list_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBSMulticastNeighbourCellListItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using mbs_multicast_neighbour_cell_list_item_ext_ies_o = protocol_ies_empty_o;

// MBSMulticastNeighbourCellListItem ::= CHOICE
struct mbs_multicast_neighbour_cell_list_item_c {
  struct types_opts {
    enum options {
      mbs_multicast_neighbour_cell_list_infoprovided,
      nombs_multicast_neighbour_cell_list_infoprovided,
      choice_ext,
      nulltype
    } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mbs_multicast_neighbour_cell_list_item_c() = default;
  mbs_multicast_neighbour_cell_list_item_c(const mbs_multicast_neighbour_cell_list_item_c& other);
  mbs_multicast_neighbour_cell_list_item_c& operator=(const mbs_multicast_neighbour_cell_list_item_c& other);
  ~mbs_multicast_neighbour_cell_list_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  upd_mbs_multicast_neighbour_cell_list_info_s& mbs_multicast_neighbour_cell_list_infoprovided()
  {
    assert_choice_type(
        types::mbs_multicast_neighbour_cell_list_infoprovided, type_, "MBSMulticastNeighbourCellListItem");
    return c.get<upd_mbs_multicast_neighbour_cell_list_info_s>();
  }
  protocol_ie_single_container_s<mbs_multicast_neighbour_cell_list_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MBSMulticastNeighbourCellListItem");
    return c.get<protocol_ie_single_container_s<mbs_multicast_neighbour_cell_list_item_ext_ies_o>>();
  }
  const upd_mbs_multicast_neighbour_cell_list_info_s& mbs_multicast_neighbour_cell_list_infoprovided() const
  {
    assert_choice_type(
        types::mbs_multicast_neighbour_cell_list_infoprovided, type_, "MBSMulticastNeighbourCellListItem");
    return c.get<upd_mbs_multicast_neighbour_cell_list_info_s>();
  }
  const protocol_ie_single_container_s<mbs_multicast_neighbour_cell_list_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MBSMulticastNeighbourCellListItem");
    return c.get<protocol_ie_single_container_s<mbs_multicast_neighbour_cell_list_item_ext_ies_o>>();
  }
  upd_mbs_multicast_neighbour_cell_list_info_s& set_mbs_multicast_neighbour_cell_list_infoprovided();
  void                                          set_nombs_multicast_neighbour_cell_list_infoprovided();
  protocol_ie_single_container_s<mbs_multicast_neighbour_cell_list_item_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<mbs_multicast_neighbour_cell_list_item_ext_ies_o>,
                  upd_mbs_multicast_neighbour_cell_list_info_s>
      c;

  void destroy_();
};

// MBSMulticastSessionReceptionState ::= ENUMERATED
struct mbs_multicast_session_reception_state_opts {
  enum options { start_monitoring_g_rnti, stop_monitoring_g_rnti, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mbs_multicast_session_reception_state_e = enumerated<mbs_multicast_session_reception_state_opts, true>;

// MBSPTPRetransmissionTunnelRequired ::= ENUMERATED
struct mbsptp_retx_tunnel_required_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mbsptp_retx_tunnel_required_e = enumerated<mbsptp_retx_tunnel_required_opts, true>;

// MC-PagingCell-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mc_paging_cell_item_ext_ies_o = protocol_ext_empty_o;

using mc_paging_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// MC-PagingCell-Item ::= SEQUENCE
struct mc_paging_cell_item_s {
  bool                                  ie_exts_present = false;
  nr_cgi_s                              nr_cgi;
  mc_paging_cell_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MDT-Activation ::= ENUMERATED
struct mdt_activation_opts {
  enum options { immediate_mdt_only, immediate_mdt_and_trace, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mdt_activation_e = enumerated<mdt_activation_opts, true>;

// MDTConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mdt_cfg_ext_ies_o = protocol_ext_empty_o;

using mdt_cfg_ext_ies_container = protocol_ext_container_empty_l;

// MDTConfiguration ::= SEQUENCE
struct mdt_cfg_s {
  bool                            m2_cfg_present  = false;
  bool                            m5_cfg_present  = false;
  bool                            m6_cfg_present  = false;
  bool                            m7_cfg_present  = false;
  bool                            ie_exts_present = false;
  mdt_activation_e                mdt_activation;
  fixed_bitstring<8, false, true> meass_to_activ;
  m2_cfg_e                        m2_cfg;
  m5_cfg_s                        m5_cfg;
  m6_cfg_s                        m6_cfg;
  m7_cfg_s                        m7_cfg;
  mdt_cfg_ext_ies_container       ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MDTPLMNList ::= SEQUENCE (SIZE (1..16)) OF OCTET STRING (SIZE (3))
using mdt_plmn_list_l = bounded_array<fixed_octstring<3, true>, 16>;

// MDTPLMNModificationList ::= SEQUENCE (SIZE (0..16)) OF OCTET STRING (SIZE (3))
using mdt_plmn_mod_list_l = bounded_array<fixed_octstring<3, true>, 16>;

// MDTPollutedMeasurementIndicator ::= ENUMERATED
struct mdt_polluted_meas_ind_opts {
  enum options { id_c, no_id_c, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mdt_polluted_meas_ind_e = enumerated<mdt_polluted_meas_ind_opts, true>;

// MRB-ProgressInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using mrb_progress_info_ext_ies_o = protocol_ies_empty_o;

// MRB-ProgressInformation ::= CHOICE
struct mrb_progress_info_c {
  struct types_opts {
    enum options { pdcp_sn12, pdcp_sn18, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mrb_progress_info_c() = default;
  mrb_progress_info_c(const mrb_progress_info_c& other);
  mrb_progress_info_c& operator=(const mrb_progress_info_c& other);
  ~mrb_progress_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint16_t& pdcp_sn12()
  {
    assert_choice_type(types::pdcp_sn12, type_, "MRB-ProgressInformation");
    return c.get<uint16_t>();
  }
  uint32_t& pdcp_sn18()
  {
    assert_choice_type(types::pdcp_sn18, type_, "MRB-ProgressInformation");
    return c.get<uint32_t>();
  }
  protocol_ie_single_container_s<mrb_progress_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MRB-ProgressInformation");
    return c.get<protocol_ie_single_container_s<mrb_progress_info_ext_ies_o>>();
  }
  const uint16_t& pdcp_sn12() const
  {
    assert_choice_type(types::pdcp_sn12, type_, "MRB-ProgressInformation");
    return c.get<uint16_t>();
  }
  const uint32_t& pdcp_sn18() const
  {
    assert_choice_type(types::pdcp_sn18, type_, "MRB-ProgressInformation");
    return c.get<uint32_t>();
  }
  const protocol_ie_single_container_s<mrb_progress_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MRB-ProgressInformation");
    return c.get<protocol_ie_single_container_s<mrb_progress_info_ext_ies_o>>();
  }
  uint16_t&                                                    set_pdcp_sn12();
  uint32_t&                                                    set_pdcp_sn18();
  protocol_ie_single_container_s<mrb_progress_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                        type_;
  choice_buffer_t<protocol_ie_single_container_s<mrb_progress_info_ext_ies_o>> c;

  void destroy_();
};

// MT-SDT-Indicator ::= ENUMERATED
struct mt_sdt_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mt_sdt_ind_e = enumerated<mt_sdt_ind_opts, true>;

// MT-SDT-Information-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mt_sdt_info_ext_ies_o = protocol_ext_empty_o;

using mt_sdt_info_ext_ies_container = protocol_ext_container_empty_l;

// MT-SDT-Information ::= SEQUENCE
struct mt_sdt_info_s {
  bool                          ie_exts_present = false;
  mt_sdt_ind_e                  mt_sdt_ind;
  mt_sdt_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasBasedOnAggregatedResources ::= ENUMERATED
struct meas_based_on_aggr_res_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using meas_based_on_aggr_res_e = enumerated<meas_based_on_aggr_res_opts, true>;

// MeasuredFrequencyHops ::= ENUMERATED
struct measured_freq_hops_opts {
  enum options { single_hop, multi_hop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using measured_freq_hops_e = enumerated<measured_freq_hops_opts, true>;

// UL-RTOA-MeasurementItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct ul_rtoa_meas_item_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options {
        report_granularitykminus1,
        report_granularitykminus2,
        report_granularitykminus3,
        report_granularitykminus4,
        report_granularitykminus5,
        report_granularitykminus6,
        nulltype
      } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    value_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint32_t&       report_granularitykminus1();
    uint32_t&       report_granularitykminus2();
    uint32_t&       report_granularitykminus3();
    uint32_t&       report_granularitykminus4();
    uint32_t&       report_granularitykminus5();
    uint32_t&       report_granularitykminus6();
    const uint32_t& report_granularitykminus1() const;
    const uint32_t& report_granularitykminus2() const;
    const uint32_t& report_granularitykminus3() const;
    const uint32_t& report_granularitykminus4() const;
    const uint32_t& report_granularitykminus5() const;
    const uint32_t& report_granularitykminus6() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// UL-RTOA-MeasurementItem ::= CHOICE
struct ul_rtoa_meas_item_c {
  struct types_opts {
    enum options { k0, k1, k2, k3, k4, k5, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ul_rtoa_meas_item_c() = default;
  ul_rtoa_meas_item_c(const ul_rtoa_meas_item_c& other);
  ul_rtoa_meas_item_c& operator=(const ul_rtoa_meas_item_c& other);
  ~ul_rtoa_meas_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint32_t& k0()
  {
    assert_choice_type(types::k0, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  uint32_t& k1()
  {
    assert_choice_type(types::k1, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  uint32_t& k2()
  {
    assert_choice_type(types::k2, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  uint32_t& k3()
  {
    assert_choice_type(types::k3, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  uint32_t& k4()
  {
    assert_choice_type(types::k4, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  uint16_t& k5()
  {
    assert_choice_type(types::k5, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint16_t>();
  }
  protocol_ie_single_container_s<ul_rtoa_meas_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UL-RTOA-MeasurementItem");
    return c.get<protocol_ie_single_container_s<ul_rtoa_meas_item_ext_ies_o>>();
  }
  const uint32_t& k0() const
  {
    assert_choice_type(types::k0, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  const uint32_t& k1() const
  {
    assert_choice_type(types::k1, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  const uint32_t& k2() const
  {
    assert_choice_type(types::k2, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  const uint32_t& k3() const
  {
    assert_choice_type(types::k3, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  const uint32_t& k4() const
  {
    assert_choice_type(types::k4, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint32_t>();
  }
  const uint16_t& k5() const
  {
    assert_choice_type(types::k5, type_, "UL-RTOA-MeasurementItem");
    return c.get<uint16_t>();
  }
  const protocol_ie_single_container_s<ul_rtoa_meas_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UL-RTOA-MeasurementItem");
    return c.get<protocol_ie_single_container_s<ul_rtoa_meas_item_ext_ies_o>>();
  }
  uint32_t&                                                    set_k0();
  uint32_t&                                                    set_k1();
  uint32_t&                                                    set_k2();
  uint32_t&                                                    set_k3();
  uint32_t&                                                    set_k4();
  uint16_t&                                                    set_k5();
  protocol_ie_single_container_s<ul_rtoa_meas_item_ext_ies_o>& set_choice_ext();

private:
  types                                                                        type_;
  choice_buffer_t<protocol_ie_single_container_s<ul_rtoa_meas_item_ext_ies_o>> c;

  void destroy_();
};

// UL-RTOA-Measurement-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct ul_rtoa_meas_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ul_rtoa_meas_ext_ies_container {
  bool                     extended_add_path_list_present = false;
  bool                     trp_rx_teg_info_present        = false;
  extended_add_path_list_l extended_add_path_list;
  trp_rx_teg_info_s        trp_rx_teg_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-RTOA-Measurement ::= SEQUENCE
struct ul_rtoa_meas_s {
  bool                           ie_exts_present = false;
  ul_rtoa_meas_item_c            ul_rtoa_meas_item;
  add_path_list_l                add_path_list;
  ul_rtoa_meas_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-RSCP-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ul_rs_cp_ext_ies_o = protocol_ext_empty_o;

using ul_rs_cp_ext_ies_container = protocol_ext_container_empty_l;

// UL-RSCP ::= SEQUENCE
struct ul_rs_cp_s {
  bool                       ie_exts_present = false;
  uint16_t                   ul_rs_cp        = 0;
  ul_rs_cp_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasuredResultsValue-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct measured_results_value_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { zo_a_info, multiple_ul_ao_a, ul_srs_rsrp_p, ul_rs_cp, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    value_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    zo_a_info_s&              zo_a_info();
    multiple_ul_ao_a_s&       multiple_ul_ao_a();
    ul_srs_rsrp_p_s&          ul_srs_rsrp_p();
    ul_rs_cp_s&               ul_rs_cp();
    const zo_a_info_s&        zo_a_info() const;
    const multiple_ul_ao_a_s& multiple_ul_ao_a() const;
    const ul_srs_rsrp_p_s&    ul_srs_rsrp_p() const;
    const ul_rs_cp_s&         ul_rs_cp() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// MeasuredResultsValue ::= CHOICE
struct measured_results_value_c {
  struct types_opts {
    enum options { ul_angle_of_arrival, ul_srs_rsrp, ul_rtoa, gnb_rx_tx_time_diff, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  measured_results_value_c() = default;
  measured_results_value_c(const measured_results_value_c& other);
  measured_results_value_c& operator=(const measured_results_value_c& other);
  ~measured_results_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ul_ao_a_s& ul_angle_of_arrival()
  {
    assert_choice_type(types::ul_angle_of_arrival, type_, "MeasuredResultsValue");
    return c.get<ul_ao_a_s>();
  }
  uint8_t& ul_srs_rsrp()
  {
    assert_choice_type(types::ul_srs_rsrp, type_, "MeasuredResultsValue");
    return c.get<uint8_t>();
  }
  ul_rtoa_meas_s& ul_rtoa()
  {
    assert_choice_type(types::ul_rtoa, type_, "MeasuredResultsValue");
    return c.get<ul_rtoa_meas_s>();
  }
  gnb_rx_tx_time_diff_s& gnb_rx_tx_time_diff()
  {
    assert_choice_type(types::gnb_rx_tx_time_diff, type_, "MeasuredResultsValue");
    return c.get<gnb_rx_tx_time_diff_s>();
  }
  protocol_ie_single_container_s<measured_results_value_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MeasuredResultsValue");
    return c.get<protocol_ie_single_container_s<measured_results_value_ext_ies_o>>();
  }
  const ul_ao_a_s& ul_angle_of_arrival() const
  {
    assert_choice_type(types::ul_angle_of_arrival, type_, "MeasuredResultsValue");
    return c.get<ul_ao_a_s>();
  }
  const uint8_t& ul_srs_rsrp() const
  {
    assert_choice_type(types::ul_srs_rsrp, type_, "MeasuredResultsValue");
    return c.get<uint8_t>();
  }
  const ul_rtoa_meas_s& ul_rtoa() const
  {
    assert_choice_type(types::ul_rtoa, type_, "MeasuredResultsValue");
    return c.get<ul_rtoa_meas_s>();
  }
  const gnb_rx_tx_time_diff_s& gnb_rx_tx_time_diff() const
  {
    assert_choice_type(types::gnb_rx_tx_time_diff, type_, "MeasuredResultsValue");
    return c.get<gnb_rx_tx_time_diff_s>();
  }
  const protocol_ie_single_container_s<measured_results_value_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MeasuredResultsValue");
    return c.get<protocol_ie_single_container_s<measured_results_value_ext_ies_o>>();
  }
  ul_ao_a_s&                                                        set_ul_angle_of_arrival();
  uint8_t&                                                          set_ul_srs_rsrp();
  ul_rtoa_meas_s&                                                   set_ul_rtoa();
  gnb_rx_tx_time_diff_s&                                            set_gnb_rx_tx_time_diff();
  protocol_ie_single_container_s<measured_results_value_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<gnb_rx_tx_time_diff_s,
                  protocol_ie_single_container_s<measured_results_value_ext_ies_o>,
                  ul_ao_a_s,
                  ul_rtoa_meas_s>
      c;

  void destroy_();
};

// MeasurementBeamInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using meas_beam_info_ext_ies_o = protocol_ext_empty_o;

using meas_beam_info_ext_ies_container = protocol_ext_container_empty_l;

// MeasurementBeamInfo ::= SEQUENCE
struct meas_beam_info_s {
  bool                             prs_res_id_present     = false;
  bool                             prs_res_set_id_present = false;
  bool                             ssb_idx_present        = false;
  bool                             ie_exts_present        = false;
  uint8_t                          prs_res_id             = 0;
  uint8_t                          prs_res_set_id         = 0;
  uint8_t                          ssb_idx                = 0;
  meas_beam_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementBeamInfoRequest ::= ENUMERATED
struct meas_beam_info_request_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using meas_beam_info_request_e = enumerated<meas_beam_info_request_opts, true>;

// MeasurementPeriodicity ::= ENUMERATED
struct meas_periodicity_opts {
  enum options {
    ms120,
    ms240,
    ms480,
    ms640,
    ms1024,
    ms2048,
    ms5120,
    ms10240,
    min1,
    min6,
    min12,
    min30,
    // ...
    ms20480,
    ms40960,
    extended,
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using meas_periodicity_e = enumerated<meas_periodicity_opts, true, 3>;

// MeasurementPeriodicityExtended ::= ENUMERATED
struct meas_periodicity_extended_opts {
  enum options {
    ms160,
    ms320,
    ms1280,
    ms2560,
    ms61440,
    ms81920,
    ms368640,
    ms737280,
    ms1843200,
    /*...*/ nulltype
  } value;
  typedef uint32_t number_type;

  const char* to_string() const;
  uint32_t    to_number() const;
};
using meas_periodicity_extended_e = enumerated<meas_periodicity_extended_opts, true>;

// MeasurementTimeOccasion ::= ENUMERATED
struct meas_time_occasion_opts {
  enum options { o1, o4, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using meas_time_occasion_e = enumerated<meas_time_occasion_opts, true>;

// TAI-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tai_ext_ies_o = protocol_ext_empty_o;

using tai_ext_ies_container = protocol_ext_container_empty_l;

// TAI ::= SEQUENCE
struct tai_s {
  bool                     ie_exts_present = false;
  fixed_octstring<3, true> plmn_id;
  fixed_octstring<3, true> five_gs_tac;
  tai_ext_ies_container    ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Mobile-IAB-MTUserLocationInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mobile_iab_mt_user_location_info_ext_ies_o = protocol_ext_empty_o;

using mobile_iab_mt_user_location_info_ext_ies_container = protocol_ext_container_empty_l;

// Mobile-IAB-MTUserLocationInformation ::= SEQUENCE
struct mobile_iab_mt_user_location_info_s {
  bool                                               ie_exts_present = false;
  nr_cgi_s                                           nr_cgi;
  tai_s                                              tai;
  mobile_iab_mt_user_location_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityInitiation-CellSwitchInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mob_initiation_cell_switch_info_ext_ies_o = protocol_ext_empty_o;

using mob_initiation_cell_switch_info_ext_ies_container = protocol_ext_container_empty_l;

// MobilityInitiation-CellSwitchInfo ::= SEQUENCE
struct mob_initiation_cell_switch_info_s {
  bool                                              ie_exts_present = false;
  candidate_cellwith_beam_info_s                    candidate_cellwith_beam_info;
  mob_initiation_cell_switch_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityInitiation-EarlyULSyncInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mob_initiation_early_ul_sync_info_ext_ies_o = protocol_ext_empty_o;

using mob_initiation_early_ul_sync_info_ext_ies_container = protocol_ext_container_empty_l;

// MobilityInitiation-EarlyULSyncInfo ::= SEQUENCE
struct mob_initiation_early_ul_sync_info_s {
  bool                                                ie_exts_present = false;
  candidate_cellwith_beam_info_list_l                 candidate_cellwith_beam_info_list;
  mob_initiation_early_ul_sync_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityInitiation-EarlyDLSyncInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mob_initiation_early_dl_sync_info_ext_ies_o = protocol_ext_empty_o;

using mob_initiation_early_dl_sync_info_ext_ies_container = protocol_ext_container_empty_l;

// MobilityInitiation-EarlyDLSyncInfo ::= SEQUENCE
struct mob_initiation_early_dl_sync_info_s {
  bool                                                ie_exts_present = false;
  candidate_cellwith_beam_info_list_l                 candidate_cellwith_beam_info_list;
  mob_initiation_early_dl_sync_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityTrigger-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mob_trigger_ext_ies_o = protocol_ext_empty_o;

using mob_trigger_ext_ies_container = protocol_ext_container_empty_l;

// MobilityTrigger ::= SEQUENCE
struct mob_trigger_s {
  bool                                mob_initiation_cell_switch_info_present   = false;
  bool                                mob_initiation_early_ul_sync_info_present = false;
  bool                                mob_initiation_early_dl_sync_info_present = false;
  fixed_bitstring<8, false, true>     mob_trigger_ind;
  mob_initiation_cell_switch_info_s   mob_initiation_cell_switch_info;
  mob_initiation_early_ul_sync_info_s mob_initiation_early_ul_sync_info;
  mob_initiation_early_dl_sync_info_s mob_initiation_early_dl_sync_info;
  mob_trigger_ext_ies_container       ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServingCellMeasurements-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using serving_cell_meass_ext_ies_o = protocol_ext_empty_o;

using serving_cell_meass_ext_ies_container = protocol_ext_container_empty_l;

// ServingCellMeasurements ::= SEQUENCE
struct serving_cell_meass_s {
  bool                                 ie_exts_present = false;
  nr_cgi_s                             serving_cell_id;
  ssb_idxwith_meass_list_l             ssb_idxwith_meass_list;
  serving_cell_meass_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityInitiation-AssistanceInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using mob_initiation_assist_info_ext_ies_o = protocol_ext_empty_o;

using mob_initiation_assist_info_ext_ies_container = protocol_ext_container_empty_l;

// MobilityInitiation-AssistanceInfo ::= SEQUENCE
struct mob_initiation_assist_info_s {
  bool                                         ie_exts_present = false;
  serving_cell_meass_s                         serving_cell_meass;
  candidate_cellwith_meass_list_l              candidate_cellwith_meass_list;
  mob_initiation_assist_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityInitiation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using mob_initiation_ext_ies_o = protocol_ies_empty_o;

// MobilityInitiation ::= CHOICE
struct mob_initiation_c {
  struct types_opts {
    enum options { mob_trigger, mob_initiation_assist_info, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mob_initiation_c() = default;
  mob_initiation_c(const mob_initiation_c& other);
  mob_initiation_c& operator=(const mob_initiation_c& other);
  ~mob_initiation_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  mob_trigger_s& mob_trigger()
  {
    assert_choice_type(types::mob_trigger, type_, "MobilityInitiation");
    return c.get<mob_trigger_s>();
  }
  mob_initiation_assist_info_s& mob_initiation_assist_info()
  {
    assert_choice_type(types::mob_initiation_assist_info, type_, "MobilityInitiation");
    return c.get<mob_initiation_assist_info_s>();
  }
  protocol_ie_single_container_s<mob_initiation_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MobilityInitiation");
    return c.get<protocol_ie_single_container_s<mob_initiation_ext_ies_o>>();
  }
  const mob_trigger_s& mob_trigger() const
  {
    assert_choice_type(types::mob_trigger, type_, "MobilityInitiation");
    return c.get<mob_trigger_s>();
  }
  const mob_initiation_assist_info_s& mob_initiation_assist_info() const
  {
    assert_choice_type(types::mob_initiation_assist_info, type_, "MobilityInitiation");
    return c.get<mob_initiation_assist_info_s>();
  }
  const protocol_ie_single_container_s<mob_initiation_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MobilityInitiation");
    return c.get<protocol_ie_single_container_s<mob_initiation_ext_ies_o>>();
  }
  mob_trigger_s&                                            set_mob_trigger();
  mob_initiation_assist_info_s&                             set_mob_initiation_assist_info();
  protocol_ie_single_container_s<mob_initiation_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<mob_initiation_assist_info_s, mob_trigger_s, protocol_ie_single_container_s<mob_initiation_ext_ies_o>>
      c;

  void destroy_();
};

// ThresholdIndexInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using thres_idx_info_ext_ies_o = protocol_ies_empty_o;

// ThresholdIndexInformation ::= CHOICE
struct thres_idx_info_c {
  struct types_opts {
    enum options { thres_idxprovided, thres_idxnotprovided, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  thres_idx_info_c() = default;
  thres_idx_info_c(const thres_idx_info_c& other);
  thres_idx_info_c& operator=(const thres_idx_info_c& other);
  ~thres_idx_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& thres_idxprovided()
  {
    assert_choice_type(types::thres_idxprovided, type_, "ThresholdIndexInformation");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<thres_idx_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ThresholdIndexInformation");
    return c.get<protocol_ie_single_container_s<thres_idx_info_ext_ies_o>>();
  }
  const uint8_t& thres_idxprovided() const
  {
    assert_choice_type(types::thres_idxprovided, type_, "ThresholdIndexInformation");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<thres_idx_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ThresholdIndexInformation");
    return c.get<protocol_ie_single_container_s<thres_idx_info_ext_ies_o>>();
  }
  uint8_t&                                                  set_thres_idxprovided();
  void                                                      set_thres_idxnotprovided();
  protocol_ie_single_container_s<thres_idx_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                     type_;
  choice_buffer_t<protocol_ie_single_container_s<thres_idx_info_ext_ies_o>> c;

  void destroy_();
};

// ThresholdIndexSession-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using thres_idx_session_item_ext_ies_o = protocol_ext_empty_o;

using thres_idx_session_item_ext_ies_container = protocol_ext_container_empty_l;

// ThresholdIndexSession-Item ::= SEQUENCE
struct thres_idx_session_item_s {
  bool                                     ie_exts_present = false;
  mbs_session_id_s                         mbs_session_id;
  thres_idx_info_c                         thres_idx_info;
  thres_idx_session_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ThresholdIndexSessionList ::= SEQUENCE (SIZE (1..1024)) OF ThresholdIndexSession-Item
using thres_idx_session_list_l = dyn_array<thres_idx_session_item_s>;

// UpdateThresholdMBS-ListInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using upd_thres_mbs_list_info_ext_ies_o = protocol_ext_empty_o;

using upd_thres_mbs_list_info_ext_ies_container = protocol_ext_container_empty_l;

// UpdateThresholdMBS-ListInformation ::= SEQUENCE
struct upd_thres_mbs_list_info_s {
  bool                                      ie_exts_present = false;
  unbounded_octstring<true>                 thres_mbs_list;
  thres_idx_session_list_l                  thres_idx_session_list;
  upd_thres_mbs_list_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ThresholdMBS-ListItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using thres_mbs_list_item_ext_ies_o = protocol_ies_empty_o;

// ThresholdMBS-ListItem ::= CHOICE
struct thres_mbs_list_item_c {
  struct types_opts {
    enum options { thres_mbs_list_infoprovided, nothres_mbs_list_infoprovided, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  thres_mbs_list_item_c() = default;
  thres_mbs_list_item_c(const thres_mbs_list_item_c& other);
  thres_mbs_list_item_c& operator=(const thres_mbs_list_item_c& other);
  ~thres_mbs_list_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  upd_thres_mbs_list_info_s& thres_mbs_list_infoprovided()
  {
    assert_choice_type(types::thres_mbs_list_infoprovided, type_, "ThresholdMBS-ListItem");
    return c.get<upd_thres_mbs_list_info_s>();
  }
  protocol_ie_single_container_s<thres_mbs_list_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ThresholdMBS-ListItem");
    return c.get<protocol_ie_single_container_s<thres_mbs_list_item_ext_ies_o>>();
  }
  const upd_thres_mbs_list_info_s& thres_mbs_list_infoprovided() const
  {
    assert_choice_type(types::thres_mbs_list_infoprovided, type_, "ThresholdMBS-ListItem");
    return c.get<upd_thres_mbs_list_info_s>();
  }
  const protocol_ie_single_container_s<thres_mbs_list_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ThresholdMBS-ListItem");
    return c.get<protocol_ie_single_container_s<thres_mbs_list_item_ext_ies_o>>();
  }
  upd_thres_mbs_list_info_s&                                     set_thres_mbs_list_infoprovided();
  void                                                           set_nothres_mbs_list_infoprovided();
  protocol_ie_single_container_s<thres_mbs_list_item_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                     type_;
  choice_buffer_t<protocol_ie_single_container_s<thres_mbs_list_item_ext_ies_o>, upd_thres_mbs_list_info_s> c;

  void destroy_();
};

// MulticastCommonCu2DUCellInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_common_cu2_du_cell_info_ext_ies_o = protocol_ext_empty_o;

using multicast_common_cu2_du_cell_info_ext_ies_container = protocol_ext_container_empty_l;

// MulticastCommonCu2DUCellInformation ::= SEQUENCE
struct multicast_common_cu2_du_cell_info_s {
  bool                                                mbs_multicast_neighbour_cell_list_item_present = false;
  bool                                                thres_mbs_list_item_present                    = false;
  bool                                                ie_exts_present                                = false;
  mbs_multicast_neighbour_cell_list_item_c            mbs_multicast_neighbour_cell_list_item;
  thres_mbs_list_item_c                               thres_mbs_list_item;
  multicast_common_cu2_du_cell_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastCommonCU2DUCell-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_common_cu2_du_cell_item_ext_ies_o = protocol_ext_empty_o;

using multicast_common_cu2_du_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastCommonCU2DUCell-Item ::= SEQUENCE
struct multicast_common_cu2_du_cell_item_s {
  bool                                                ie_exts_present = false;
  nr_cgi_s                                            nr_cgi;
  multicast_common_cu2_du_cell_info_s                 multicast_common_cu2_du_cell_info;
  multicast_common_cu2_du_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastCommonCU2DUCellList ::= SEQUENCE (SIZE (1..512)) OF MulticastCommonCU2DUCell-Item
using multicast_common_cu2_du_cell_list_l = dyn_array<multicast_common_cu2_du_cell_item_s>;

// MulticastCU2DUCommonRRCInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_cu2_du_common_rrc_info_ext_ies_o = protocol_ext_empty_o;

using multicast_cu2_du_common_rrc_info_ext_ies_container = protocol_ext_container_empty_l;

// MulticastCU2DUCommonRRCInfo ::= SEQUENCE
struct multicast_cu2_du_common_rrc_info_s {
  bool                                               ie_exts_present = false;
  multicast_common_cu2_du_cell_list_l                multicast_common_cu2_du_cell_list;
  multicast_cu2_du_common_rrc_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastCU2DURRCInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_cu2_du_rrc_info_ext_ies_o = protocol_ext_empty_o;

using multicast_cu2_du_rrc_info_ext_ies_container = protocol_ext_container_empty_l;

// MulticastCU2DURRCInfo ::= SEQUENCE
struct multicast_cu2_du_rrc_info_s {
  bool                                        ie_exts_present = false;
  mbs_multicast_cu2_du_cell_list_l            mbs_multicast_cu2_du_cell_list;
  mbs_multicast_mrb_list_l                    mbs_multicast_mrb_list;
  multicast_cu2_du_rrc_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastDU2CURRCInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_du2_cu_rrc_info_ext_ies_o = protocol_ext_empty_o;

using multicast_du2_cu_rrc_info_ext_ies_container = protocol_ext_container_empty_l;

// MulticastDU2CURRCInfo ::= SEQUENCE
struct multicast_du2_cu_rrc_info_s {
  bool                                        ie_exts_present = false;
  mbs_multicast_du2_cu_cell_list_l            mbs_multicast_du2_cu_cell_list;
  multicast_du2_cu_rrc_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastF1UContext-FailedToBeSetup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_f1_u_context_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using multicast_f1_u_context_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastF1UContext-FailedToBeSetup-Item ::= SEQUENCE
struct multicast_f1_u_context_failed_to_be_setup_item_s {
  bool                                                             cause_present   = false;
  bool                                                             ie_exts_present = false;
  uint16_t                                                         mrb_id          = 1;
  cause_c                                                          cause;
  multicast_f1_u_context_failed_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastF1UContext-Setup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_f1_u_context_setup_item_ext_ies_o = protocol_ext_empty_o;

using multicast_f1_u_context_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastF1UContext-Setup-Item ::= SEQUENCE
struct multicast_f1_u_context_setup_item_s {
  bool                                                ie_exts_present = false;
  uint16_t                                            mrb_id          = 1;
  up_transport_layer_info_c                           mbs_f1u_info_at_cu;
  multicast_f1_u_context_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastF1UContext-ToBeSetup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_f1_u_context_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using multicast_f1_u_context_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastF1UContext-ToBeSetup-Item ::= SEQUENCE
struct multicast_f1_u_context_to_be_setup_item_s {
  bool                                                      mbs_progress_info_present = false;
  bool                                                      ie_exts_present           = false;
  uint16_t                                                  mrb_id                    = 1;
  up_transport_layer_info_c                                 mbs_f1u_info_at_du;
  mrb_progress_info_c                                       mbs_progress_info;
  multicast_f1_u_context_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMBSSessionList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_mbs_session_list_item_ext_ies_o = protocol_ext_empty_o;

using multicast_mbs_session_list_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMBSSessionList-Item ::= SEQUENCE
struct multicast_mbs_session_list_item_s {
  bool                                              ie_exts_present = false;
  mbs_session_id_s                                  mbs_session_id;
  multicast_mbs_session_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMBSSessionList ::= SEQUENCE (SIZE (1..256)) OF MulticastMBSSessionList-Item
using multicast_mbs_session_list_l = dyn_array<multicast_mbs_session_list_item_s>;

// MulticastMRBs-FailedtoBeModified-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_failedto_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_failedto_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-FailedToBeModified-Item ::= SEQUENCE
struct multicast_m_rbs_failed_to_be_modified_item_s {
  bool                                                        cause_present   = false;
  bool                                                        ie_exts_present = false;
  uint16_t                                                    mrb_id          = 1;
  cause_c                                                     cause;
  multicast_m_rbs_failedto_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-FailedToBeSetup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-FailedToBeSetup-Item ::= SEQUENCE
struct multicast_m_rbs_failed_to_be_setup_item_s {
  bool                                                      cause_present   = false;
  bool                                                      ie_exts_present = false;
  uint16_t                                                  mrb_id          = 1;
  cause_c                                                   cause;
  multicast_m_rbs_failed_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-FailedToBeSetupMod-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_failed_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_failed_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-FailedToBeSetupMod-Item ::= SEQUENCE
struct multicast_m_rbs_failed_to_be_setup_mod_item_s {
  bool                                                          cause_present   = false;
  bool                                                          ie_exts_present = false;
  uint16_t                                                      mrb_id          = 1;
  cause_c                                                       cause;
  multicast_m_rbs_failed_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-Modified-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_modified_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-Modified-Item ::= SEQUENCE
struct multicast_m_rbs_modified_item_s {
  bool                                            ie_exts_present = false;
  uint16_t                                        mrb_id          = 1;
  multicast_m_rbs_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-Setup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_setup_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-Setup-Item ::= SEQUENCE
struct multicast_m_rbs_setup_item_s {
  bool                                         ie_exts_present = false;
  uint16_t                                     mrb_id          = 1;
  multicast_m_rbs_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-SetupMod-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-SetupMod-Item ::= SEQUENCE
struct multicast_m_rbs_setup_mod_item_s {
  bool                                             ie_exts_present = false;
  uint16_t                                         mrb_id          = 1;
  multicast_m_rbs_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-ToBeModified-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-ToBeModified-Item ::= SEQUENCE
struct multicast_m_rbs_to_be_modified_item_s {
  bool                                                  mrb_qos_info_present       = false;
  bool                                                  mbs_dl_pdcp_sn_len_present = false;
  bool                                                  ie_exts_present            = false;
  uint16_t                                              mrb_id                     = 1;
  qos_flow_level_qos_params_s                           mrb_qos_info;
  mbs_flows_mapped_to_mrb_list_l                        mbs_flows_mapped_to_mrb_list;
  pdcp_sn_len_e                                         mbs_dl_pdcp_sn_len;
  multicast_m_rbs_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-ToBeReleased-Item ::= SEQUENCE
struct multicast_m_rbs_to_be_released_item_s {
  bool                                                  ie_exts_present = false;
  uint16_t                                              mrb_id          = 1;
  multicast_m_rbs_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-ToBeSetup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-ToBeSetup-Item ::= SEQUENCE
struct multicast_m_rbs_to_be_setup_item_s {
  bool                                               ie_exts_present = false;
  uint16_t                                           mrb_id          = 1;
  qos_flow_level_qos_params_s                        mrb_qos_info;
  mbs_flows_mapped_to_mrb_list_l                     mbs_flows_mapped_to_mrb_list;
  pdcp_sn_len_e                                      mbs_dl_pdcp_sn_len;
  multicast_m_rbs_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MulticastMRBs-ToBeSetupMod-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using multicast_m_rbs_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using multicast_m_rbs_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// MulticastMRBs-ToBeSetupMod-Item ::= SEQUENCE
struct multicast_m_rbs_to_be_setup_mod_item_s {
  bool                                                   ie_exts_present = false;
  uint16_t                                               mrb_id          = 1;
  qos_flow_level_qos_params_s                            mrb_qos_info;
  mbs_flows_mapped_to_mrb_list_l                         mbs_flows_mapped_to_mrb_list;
  pdcp_sn_len_e                                          mbs_dl_pdcp_sn_len;
  multicast_m_rbs_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// N3CIndirectPathAddition-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using n3_c_indirect_path_addition_ext_ies_o = protocol_ext_empty_o;

using n3_c_indirect_path_addition_ext_ies_container = protocol_ext_container_empty_l;

// N3CIndirectPathAddition ::= SEQUENCE
struct n3_c_indirect_path_addition_s {
  bool                                          ie_exts_present    = false;
  uint64_t                                      target_relay_ue_id = 0;
  n3_c_indirect_path_addition_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NCGI-to-be-Updated-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ncgi_to_be_upd_list_item_ext_ies_o = protocol_ext_empty_o;

using ncgi_to_be_upd_list_item_ext_ies_container = protocol_ext_container_empty_l;

// NCGI-to-be-Updated-List-Item ::= SEQUENCE
struct ncgi_to_be_upd_list_item_s {
  bool                                       ie_exts_present = false;
  nr_cgi_s                                   oldncgi;
  nr_cgi_s                                   newncgi;
  ncgi_to_be_upd_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-CGI-List-For-Restart-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_cgi_list_for_restart_item_ext_ies_o = protocol_ext_empty_o;

using nr_cgi_list_for_restart_item_ext_ies_container = protocol_ext_container_empty_l;

// NR-CGI-List-For-Restart-Item ::= SEQUENCE
struct nr_cgi_list_for_restart_item_s {
  bool                                           ie_exts_present = false;
  nr_cgi_s                                       nr_cgi;
  nr_cgi_list_for_restart_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSAngleItem-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct prs_angle_item_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct prs_angle_item_item_ext_ies_container {
  bool    prs_res_id_present = false;
  uint8_t prs_res_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSAngleItem ::= SEQUENCE
struct prs_angle_item_s {
  bool                                  nr_prs_azimuth_fine_present   = false;
  bool                                  nr_prs_elevation_present      = false;
  bool                                  nr_prs_elevation_fine_present = false;
  bool                                  ie_exts_present               = false;
  uint16_t                              nr_prs_azimuth                = 0;
  uint8_t                               nr_prs_azimuth_fine           = 0;
  uint8_t                               nr_prs_elevation              = 0;
  uint8_t                               nr_prs_elevation_fine         = 0;
  prs_angle_item_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSAngleList ::= SEQUENCE (SIZE (1..64)) OF PRSAngleItem
using prs_angle_list_l = dyn_array<prs_angle_item_s>;

// NR-PRSBeamInformationItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_prs_beam_info_item_ext_ies_o = protocol_ext_empty_o;

using nr_prs_beam_info_item_ext_ies_container = protocol_ext_container_empty_l;

// NR-PRSBeamInformationItem ::= SEQUENCE
struct nr_prs_beam_info_item_s {
  bool                                    ie_exts_present = false;
  uint8_t                                 prs_res_set_id  = 0;
  prs_angle_list_l                        prs_angle_list;
  nr_prs_beam_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-PRSBeamInformationList ::= SEQUENCE (SIZE (1..2)) OF NR-PRSBeamInformationItem
using nr_prs_beam_info_list_l = dyn_array<nr_prs_beam_info_item_s>;

// NR-PRSBeamInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_prs_beam_info_ext_ies_o = protocol_ext_empty_o;

using nr_prs_beam_info_ext_ies_container = protocol_ext_container_empty_l;

// NR-PRSBeamInformation ::= SEQUENCE
struct nr_prs_beam_info_s {
  bool                               ie_exts_present = false;
  nr_prs_beam_info_list_l            nr_prs_beam_info_list;
  lc_sto_gcs_translation_list_l      lc_sto_gcs_translation_list;
  nr_prs_beam_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRA2XServicesAuthorized-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_a2_x_services_authorized_ext_ies_o = protocol_ext_empty_o;

using nr_a2_x_services_authorized_ext_ies_container = protocol_ext_container_empty_l;

// NRA2XServicesAuthorized ::= SEQUENCE
struct nr_a2_x_services_authorized_s {
  bool                                          aerial_ue_present  = false;
  bool                                          ctrller_ue_present = false;
  bool                                          ie_exts_present    = false;
  aerial_ue_e                                   aerial_ue;
  ctrller_ue_e                                  ctrller_ue;
  nr_a2_x_services_authorized_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRPaging-Time-Window ::= ENUMERATED
struct nr_paging_time_win_opts {
  enum options {
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    s12,
    s13,
    s14,
    s15,
    s16,
    // ...
    s17,
    s18,
    s19,
    s20,
    s21,
    s22,
    s23,
    s24,
    s25,
    s26,
    s27,
    s28,
    s29,
    s30,
    s31,
    s32,
    nulltype
  } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nr_paging_time_win_e = enumerated<nr_paging_time_win_opts, true, 16>;

// NRPaging-Time-Window-Inactive ::= ENUMERATED
struct nr_paging_time_win_inactive_opts {
  enum options {
    s1,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    s12,
    s13,
    s14,
    s15,
    s16,
    s17,
    s18,
    s19,
    s20,
    s21,
    s22,
    s23,
    s24,
    s25,
    s26,
    s27,
    s28,
    s29,
    s30,
    s31,
    s32,
    // ...
    nulltype
  } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nr_paging_time_win_inactive_e = enumerated<nr_paging_time_win_inactive_opts, true>;

// NRPaging-eDRX-Cycle-Idle ::= ENUMERATED
struct nr_paging_e_drx_cycle_idle_opts {
  enum options {
    hfquarter,
    hfhalf,
    hf1,
    hf2,
    hf4,
    hf8,
    hf16,
    hf32,
    hf64,
    hf128,
    hf256,
    hf512,
    hf1024,
    /*...*/ nulltype
  } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using nr_paging_e_drx_cycle_idle_e = enumerated<nr_paging_e_drx_cycle_idle_opts, true>;

// NRPaging-eDRX-Cycle-Inactive ::= ENUMERATED
struct nr_paging_e_drx_cycle_inactive_opts {
  enum options { hfquarter, hfhalf, hf1, /*...*/ nulltype } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using nr_paging_e_drx_cycle_inactive_e = enumerated<nr_paging_e_drx_cycle_inactive_opts, true>;

// NRPaging-long-eDRX-Cycle-Inactive ::= ENUMERATED
struct nr_paging_long_e_drx_cycle_inactive_opts {
  enum options { hf2, hf4, hf8, hf16, hf32, hf64, hf128, hf256, hf512, hf1024, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using nr_paging_long_e_drx_cycle_inactive_e = enumerated<nr_paging_long_e_drx_cycle_inactive_opts, true>;

// NRPagingeDRXInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_paginge_drx_info_ext_ies_o = protocol_ext_empty_o;

using nr_paginge_drx_info_ext_ies_container = protocol_ext_container_empty_l;

// NRPagingeDRXInformation ::= SEQUENCE
struct nr_paginge_drx_info_s {
  bool                                  nrpaging_time_win_present = false;
  bool                                  ie_exts_present           = false;
  nr_paging_e_drx_cycle_idle_e          nrpaging_e_drx_cycle_idle;
  nr_paging_time_win_e                  nrpaging_time_win;
  nr_paginge_drx_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRPagingeDRXInformationforRRCINACTIVE-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_paginge_drx_infofor_rrc_inactive_ext_ies_o = protocol_ext_empty_o;

using nr_paginge_drx_infofor_rrc_inactive_ext_ies_container = protocol_ext_container_empty_l;

// NRPagingeDRXInformationforRRCINACTIVE ::= SEQUENCE
struct nr_paginge_drx_infofor_rrc_inactive_s {
  bool                                                  ie_exts_present = false;
  nr_paging_e_drx_cycle_inactive_e                      nrpaging_e_drx_cycle_inactive;
  nr_paginge_drx_infofor_rrc_inactive_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRPaginglongeDRXInformationforRRCINACTIVE-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_paginglonge_drx_infofor_rrc_inactive_ext_ies_o = protocol_ext_empty_o;

using nr_paginglonge_drx_infofor_rrc_inactive_ext_ies_container = protocol_ext_container_empty_l;

// NRPaginglongeDRXInformationforRRCINACTIVE ::= SEQUENCE
struct nr_paginglonge_drx_infofor_rrc_inactive_s {
  bool                                                      ie_exts_present = false;
  nr_paging_long_e_drx_cycle_inactive_e                     nr_paging_long_e_drx_cycle_inactive;
  nr_paging_time_win_inactive_e                             nr_paging_time_win_inactive;
  nr_paginglonge_drx_infofor_rrc_inactive_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRRedCapUEIndication ::= ENUMERATED
struct nr_red_cap_ue_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using nr_red_cap_ue_ind_e = enumerated<nr_red_cap_ue_ind_opts, true>;

// NRUESidelinkAggregateMaximumBitrate-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_ue_sidelink_aggr_max_bitrate_ext_ies_o = protocol_ext_empty_o;

using nr_ue_sidelink_aggr_max_bitrate_ext_ies_container = protocol_ext_container_empty_l;

// NRUESidelinkAggregateMaximumBitrate ::= SEQUENCE
struct nr_ue_sidelink_aggr_max_bitrate_s {
  bool                                              ie_exts_present                 = false;
  uint64_t                                          ue_nr_sidelink_aggr_max_bitrate = 0;
  nr_ue_sidelink_aggr_max_bitrate_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRV2XServicesAuthorized-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using nr_v2x_services_authorized_ext_ies_o = protocol_ext_empty_o;

using nr_v2x_services_authorized_ext_ies_container = protocol_ext_container_empty_l;

// NRV2XServicesAuthorized ::= SEQUENCE
struct nr_v2x_services_authorized_s {
  bool                                         vehicle_ue_present    = false;
  bool                                         pedestrian_ue_present = false;
  bool                                         ie_exts_present       = false;
  vehicle_ue_e                                 vehicle_ue;
  pedestrian_ue_e                              pedestrian_ue;
  nr_v2x_services_authorized_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NReRedCapUEIndication ::= ENUMERATED
struct nr_e_red_cap_ue_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using nr_e_red_cap_ue_ind_e = enumerated<nr_e_red_cap_ue_ind_opts, true>;

// NeedforGap ::= ENUMERATED
struct needfor_gap_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using needfor_gap_e = enumerated<needfor_gap_opts, true>;

// Neighbour-Cell-Information-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using neighbour_cell_info_item_ext_ies_o = protocol_ext_empty_o;

using neighbour_cell_info_item_ext_ies_container = protocol_ext_container_empty_l;

// Neighbour-Cell-Information-Item ::= SEQUENCE
struct neighbour_cell_info_item_s {
  bool                                       intended_tdd_dl_ul_cfg_present = false;
  bool                                       ie_exts_present                = false;
  nr_cgi_s                                   nr_cgi;
  intended_tdd_dl_ul_cfg_s                   intended_tdd_dl_ul_cfg;
  neighbour_cell_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Neighbour-Node-Cells-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using neighbour_node_cells_list_item_ext_ies_o = protocol_ext_empty_o;

using neighbour_node_cells_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Neighbour-Node-Cells-List-Item ::= SEQUENCE
struct neighbour_node_cells_list_item_s {
  struct peer_parent_node_ind_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using peer_parent_node_ind_e_ = enumerated<peer_parent_node_ind_opts, true>;

  // member variables
  bool                                             gnb_cu_ue_f1ap_id_present             = false;
  bool                                             gnb_du_ue_f1ap_id_present             = false;
  bool                                             peer_parent_node_ind_present          = false;
  bool                                             iab_du_cell_res_cfg_mode_info_present = false;
  bool                                             iab_stc_info_present                  = false;
  bool                                             ie_exts_present                       = false;
  nr_cgi_s                                         nr_cgi;
  uint64_t                                         gnb_cu_ue_f1ap_id = 0;
  uint64_t                                         gnb_du_ue_f1ap_id = 0;
  peer_parent_node_ind_e_                          peer_parent_node_ind;
  iab_du_cell_res_cfg_mode_info_c                  iab_du_cell_res_cfg_mode_info;
  iab_stc_info_s                                   iab_stc_info;
  unbounded_octstring<true>                        rach_cfg_common;
  unbounded_octstring<true>                        rach_cfg_common_iab;
  unbounded_octstring<true>                        csi_rs_cfg;
  unbounded_octstring<true>                        sr_cfg;
  unbounded_octstring<true>                        pdcch_cfg_sib1;
  unbounded_octstring<true>                        scs_common;
  neighbour_node_cells_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Neighbour-Node-Cells-List ::= SEQUENCE (SIZE (1..1024)) OF Neighbour-Node-Cells-List-Item
using neighbour_node_cells_list_l = dyn_array<neighbour_node_cells_list_item_s>;

// NetworkControlledRepeaterAuthorized ::= ENUMERATED
struct network_ctrl_repeater_authorized_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using network_ctrl_repeater_authorized_e = enumerated<network_ctrl_repeater_authorized_opts, true>;

// NonDynamicPQIDescriptor-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using non_dyn_pq_id_escriptor_ext_ies_o = protocol_ext_empty_o;

using non_dyn_pq_id_escriptor_ext_ies_container = protocol_ext_container_empty_l;

// NonDynamicPQIDescriptor ::= SEQUENCE
struct non_dyn_pq_id_escriptor_s {
  bool                                      qos_prio_level_present        = false;
  bool                                      averaging_win_present         = false;
  bool                                      max_data_burst_volume_present = false;
  bool                                      ie_exts_present               = false;
  uint16_t                                  five_qi                       = 0;
  uint8_t                                   qos_prio_level                = 1;
  uint16_t                                  averaging_win                 = 0;
  uint16_t                                  max_data_burst_volume         = 0;
  non_dyn_pq_id_escriptor_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ShortNonIntegerDRXCycleLength ::= ENUMERATED
struct short_non_integer_drx_cycle_len_opts {
  enum options {
    ms1001over240,
    ms25over6,
    ms25over3,
    ms1001over120,
    ms100over9,
    ms25over2,
    ms40over3,
    ms125over9,
    ms50over3,
    ms1001over60,
    ms125over6,
    ms200over9,
    ms100over3,
    ms1001over30,
    ms125over3,
    ms1001over24,
    ms200over3,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using short_non_integer_drx_cycle_len_e = enumerated<short_non_integer_drx_cycle_len_opts, true>;

// NonIntegerDRXCycle-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using non_integer_drx_cycle_ext_ies_o = protocol_ext_empty_o;

using non_integer_drx_cycle_ext_ies_container = protocol_ext_container_empty_l;

// NonIntegerDRXCycle ::= SEQUENCE
struct non_integer_drx_cycle_s {
  bool                                    short_non_integer_drx_cycle_len_present = false;
  bool                                    short_drx_cycle_timer_present           = false;
  bool                                    ie_exts_present                         = false;
  long_non_integer_drx_cycle_len_e        long_non_integer_drx_cycle_len;
  short_non_integer_drx_cycle_len_e       short_non_integer_drx_cycle_len;
  uint8_t                                 short_drx_cycle_timer = 1;
  non_integer_drx_cycle_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NonUPTrafficType ::= ENUMERATED
struct non_up_traffic_type_opts {
  enum options { ue_associated, non_ue_associated, non_f1, bap_ctrl_pdu, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using non_up_traffic_type_e = enumerated<non_up_traffic_type_opts, true>;

// NotificationInformationExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using notif_info_ext_ies_o = protocol_ext_empty_o;

using notif_info_ext_ies_container = protocol_ext_container_empty_l;

// NotificationInformation ::= SEQUENCE
struct notif_info_s {
  bool                             ie_exts_present = false;
  fixed_bitstring<16, false, true> msg_id;
  fixed_bitstring<16, false, true> serial_num;
  notif_info_ext_ies_container     ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NumberOfTRPRxTEG ::= ENUMERATED
struct nof_trp_rx_teg_opts {
  enum options { two, three, four, six, eight, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nof_trp_rx_teg_e = enumerated<nof_trp_rx_teg_opts, true>;

// NumberOfTRPRxTxTEG ::= ENUMERATED
struct nof_trp_rx_tx_teg_opts {
  enum options { wo, three, four, six, eight, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nof_trp_rx_tx_teg_e = enumerated<nof_trp_rx_tx_teg_opts, true>;

// OnDemandPRS-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using on_demand_prs_info_ext_ies_o = protocol_ext_empty_o;

using on_demand_prs_info_ext_ies_container = protocol_ext_container_empty_l;

// OnDemandPRS-Info ::= SEQUENCE
struct on_demand_prs_info_s {
  bool                                 allowed_res_set_periodicity_values_present = false;
  bool                                 allowed_prs_bw_values_present              = false;
  bool                                 allowed_res_repeat_factor_values_present   = false;
  bool                                 allowed_res_nof_symbols_values_present     = false;
  bool                                 allowed_comb_size_values_present           = false;
  bool                                 ie_exts_present                            = false;
  fixed_bitstring<16, false, true>     on_demand_prs_request_allowed;
  fixed_bitstring<24, false, true>     allowed_res_set_periodicity_values;
  fixed_bitstring<64, false, true>     allowed_prs_bw_values;
  fixed_bitstring<8, false, true>      allowed_res_repeat_factor_values;
  fixed_bitstring<8, false, true>      allowed_res_nof_symbols_values;
  fixed_bitstring<8, false, true>      allowed_comb_size_values;
  on_demand_prs_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5-QoS-Characteristics-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using pc5_qos_characteristics_ext_ies_o = protocol_ies_empty_o;

// PC5-QoS-Characteristics ::= CHOICE
struct pc5_qos_characteristics_c {
  struct types_opts {
    enum options { non_dyn_pqi, dyn_pqi, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  pc5_qos_characteristics_c() = default;
  pc5_qos_characteristics_c(const pc5_qos_characteristics_c& other);
  pc5_qos_characteristics_c& operator=(const pc5_qos_characteristics_c& other);
  ~pc5_qos_characteristics_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  non_dyn_pq_id_escriptor_s& non_dyn_pqi()
  {
    assert_choice_type(types::non_dyn_pqi, type_, "PC5-QoS-Characteristics");
    return c.get<non_dyn_pq_id_escriptor_s>();
  }
  dyn_pq_id_escriptor_s& dyn_pqi()
  {
    assert_choice_type(types::dyn_pqi, type_, "PC5-QoS-Characteristics");
    return c.get<dyn_pq_id_escriptor_s>();
  }
  protocol_ie_single_container_s<pc5_qos_characteristics_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PC5-QoS-Characteristics");
    return c.get<protocol_ie_single_container_s<pc5_qos_characteristics_ext_ies_o>>();
  }
  const non_dyn_pq_id_escriptor_s& non_dyn_pqi() const
  {
    assert_choice_type(types::non_dyn_pqi, type_, "PC5-QoS-Characteristics");
    return c.get<non_dyn_pq_id_escriptor_s>();
  }
  const dyn_pq_id_escriptor_s& dyn_pqi() const
  {
    assert_choice_type(types::dyn_pqi, type_, "PC5-QoS-Characteristics");
    return c.get<dyn_pq_id_escriptor_s>();
  }
  const protocol_ie_single_container_s<pc5_qos_characteristics_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PC5-QoS-Characteristics");
    return c.get<protocol_ie_single_container_s<pc5_qos_characteristics_ext_ies_o>>();
  }
  non_dyn_pq_id_escriptor_s&                                         set_non_dyn_pqi();
  dyn_pq_id_escriptor_s&                                             set_dyn_pqi();
  protocol_ie_single_container_s<pc5_qos_characteristics_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<dyn_pq_id_escriptor_s,
                  non_dyn_pq_id_escriptor_s,
                  protocol_ie_single_container_s<pc5_qos_characteristics_ext_ies_o>>
      c;

  void destroy_();
};

// PC5FlowBitRates-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_flow_bit_rates_ext_ies_o = protocol_ext_empty_o;

using pc5_flow_bit_rates_ext_ies_container = protocol_ext_container_empty_l;

// PC5FlowBitRates ::= SEQUENCE
struct pc5_flow_bit_rates_s {
  bool                                 ie_exts_present          = false;
  uint64_t                             guaranteed_flow_bit_rate = 0;
  uint64_t                             max_flow_bit_rate        = 0;
  pc5_flow_bit_rates_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5QoSParameters-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_qos_params_ext_ies_o = protocol_ext_empty_o;

using pc5_qos_params_ext_ies_container = protocol_ext_container_empty_l;

// PC5QoSParameters ::= SEQUENCE
struct pc5_qos_params_s {
  bool                             pc5_qos_flow_bit_rates_present = false;
  bool                             ie_exts_present                = false;
  pc5_qos_characteristics_c        pc5_qos_characteristics;
  pc5_flow_bit_rates_s             pc5_qos_flow_bit_rates;
  pc5_qos_params_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelFailedToBeModifiedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_rlc_ch_failed_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using pc5_rlc_ch_failed_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5RLCChannelFailedToBeModifiedItem ::= SEQUENCE
struct pc5_rlc_ch_failed_to_be_modified_item_s {
  bool                                                    remote_ue_local_id_present = false;
  bool                                                    cause_present              = false;
  bool                                                    ie_exts_present            = false;
  uint16_t                                                pc5_rlc_ch_id              = 1;
  uint16_t                                                remote_ue_local_id         = 0;
  cause_c                                                 cause;
  pc5_rlc_ch_failed_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelFailedToBeModifiedList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelFailedToBeModifiedItem
using pc5_rlc_ch_failed_to_be_modified_list_l = dyn_array<pc5_rlc_ch_failed_to_be_modified_item_s>;

// PC5RLCChannelFailedToBeSetupItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_rlc_ch_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using pc5_rlc_ch_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5RLCChannelFailedToBeSetupItem ::= SEQUENCE
struct pc5_rlc_ch_failed_to_be_setup_item_s {
  bool                                                 remote_ue_local_id_present = false;
  bool                                                 cause_present              = false;
  bool                                                 ie_exts_present            = false;
  uint16_t                                             pc5_rlc_ch_id              = 1;
  uint16_t                                             remote_ue_local_id         = 0;
  cause_c                                              cause;
  pc5_rlc_ch_failed_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelFailedToBeSetupList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelFailedToBeSetupItem
using pc5_rlc_ch_failed_to_be_setup_list_l = dyn_array<pc5_rlc_ch_failed_to_be_setup_item_s>;

// PC5RLCChannelModifiedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_rlc_ch_modified_item_ext_ies_o = protocol_ext_empty_o;

using pc5_rlc_ch_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5RLCChannelModifiedItem ::= SEQUENCE
struct pc5_rlc_ch_modified_item_s {
  bool                                       remote_ue_local_id_present = false;
  bool                                       ie_exts_present            = false;
  uint16_t                                   pc5_rlc_ch_id              = 1;
  uint16_t                                   remote_ue_local_id         = 0;
  pc5_rlc_ch_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelModifiedList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelModifiedItem
using pc5_rlc_ch_modified_list_l = dyn_array<pc5_rlc_ch_modified_item_s>;

// PC5RLCChannelQoSInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct pc5_rlc_ch_qos_info_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { u2_u_rlc_ch_qos, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::u2_u_rlc_ch_qos; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    pc5_qos_params_s&       u2_u_rlc_ch_qos() { return c; }
    const pc5_qos_params_s& u2_u_rlc_ch_qos() const { return c; }

  private:
    pc5_qos_params_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// PC5RLCChannelQoSInformation ::= CHOICE
struct pc5_rlc_ch_qos_info_c {
  struct pc5_ctrl_plane_traffic_type_opts {
    enum options { srb1, srb2, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using pc5_ctrl_plane_traffic_type_e_ = enumerated<pc5_ctrl_plane_traffic_type_opts, true>;
  struct types_opts {
    enum options { pc5_rlc_ch_qos, pc5_ctrl_plane_traffic_type, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  pc5_rlc_ch_qos_info_c() = default;
  pc5_rlc_ch_qos_info_c(const pc5_rlc_ch_qos_info_c& other);
  pc5_rlc_ch_qos_info_c& operator=(const pc5_rlc_ch_qos_info_c& other);
  ~pc5_rlc_ch_qos_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  qos_flow_level_qos_params_s& pc5_rlc_ch_qos()
  {
    assert_choice_type(types::pc5_rlc_ch_qos, type_, "PC5RLCChannelQoSInformation");
    return c.get<qos_flow_level_qos_params_s>();
  }
  pc5_ctrl_plane_traffic_type_e_& pc5_ctrl_plane_traffic_type()
  {
    assert_choice_type(types::pc5_ctrl_plane_traffic_type, type_, "PC5RLCChannelQoSInformation");
    return c.get<pc5_ctrl_plane_traffic_type_e_>();
  }
  protocol_ie_single_container_s<pc5_rlc_ch_qos_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PC5RLCChannelQoSInformation");
    return c.get<protocol_ie_single_container_s<pc5_rlc_ch_qos_info_ext_ies_o>>();
  }
  const qos_flow_level_qos_params_s& pc5_rlc_ch_qos() const
  {
    assert_choice_type(types::pc5_rlc_ch_qos, type_, "PC5RLCChannelQoSInformation");
    return c.get<qos_flow_level_qos_params_s>();
  }
  const pc5_ctrl_plane_traffic_type_e_& pc5_ctrl_plane_traffic_type() const
  {
    assert_choice_type(types::pc5_ctrl_plane_traffic_type, type_, "PC5RLCChannelQoSInformation");
    return c.get<pc5_ctrl_plane_traffic_type_e_>();
  }
  const protocol_ie_single_container_s<pc5_rlc_ch_qos_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PC5RLCChannelQoSInformation");
    return c.get<protocol_ie_single_container_s<pc5_rlc_ch_qos_info_ext_ies_o>>();
  }
  qos_flow_level_qos_params_s&                                   set_pc5_rlc_ch_qos();
  pc5_ctrl_plane_traffic_type_e_&                                set_pc5_ctrl_plane_traffic_type();
  protocol_ie_single_container_s<pc5_rlc_ch_qos_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                       type_;
  choice_buffer_t<protocol_ie_single_container_s<pc5_rlc_ch_qos_info_ext_ies_o>, qos_flow_level_qos_params_s> c;

  void destroy_();
};

// PC5RLCChannelRequiredToBeModifiedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_rlc_ch_required_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using pc5_rlc_ch_required_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5RLCChannelRequiredToBeModifiedItem ::= SEQUENCE
struct pc5_rlc_ch_required_to_be_modified_item_s {
  bool                                                      remote_ue_local_id_present = false;
  bool                                                      ie_exts_present            = false;
  uint16_t                                                  pc5_rlc_ch_id              = 1;
  uint16_t                                                  remote_ue_local_id         = 0;
  pc5_rlc_ch_required_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelRequiredToBeModifiedList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelRequiredToBeModifiedItem
using pc5_rlc_ch_required_to_be_modified_list_l = dyn_array<pc5_rlc_ch_required_to_be_modified_item_s>;

// PC5RLCChannelRequiredToBeReleasedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_rlc_ch_required_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using pc5_rlc_ch_required_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5RLCChannelRequiredToBeReleasedItem ::= SEQUENCE
struct pc5_rlc_ch_required_to_be_released_item_s {
  bool                                                      remote_ue_local_id_present = false;
  bool                                                      ie_exts_present            = false;
  uint16_t                                                  pc5_rlc_ch_id              = 1;
  uint16_t                                                  remote_ue_local_id         = 0;
  pc5_rlc_ch_required_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelRequiredToBeReleasedList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelRequiredToBeReleasedItem
using pc5_rlc_ch_required_to_be_released_list_l = dyn_array<pc5_rlc_ch_required_to_be_released_item_s>;

// PC5RLCChannelSetupItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_rlc_ch_setup_item_ext_ies_o = protocol_ext_empty_o;

using pc5_rlc_ch_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5RLCChannelSetupItem ::= SEQUENCE
struct pc5_rlc_ch_setup_item_s {
  bool                                    remote_ue_local_id_present = false;
  bool                                    ie_exts_present            = false;
  uint16_t                                pc5_rlc_ch_id              = 1;
  uint16_t                                remote_ue_local_id         = 0;
  pc5_rlc_ch_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelSetupList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelSetupItem
using pc5_rlc_ch_setup_list_l = dyn_array<pc5_rlc_ch_setup_item_s>;

// PC5RLCChannelToBeModifiedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_rlc_ch_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using pc5_rlc_ch_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5RLCChannelToBeModifiedItem ::= SEQUENCE
struct pc5_rlc_ch_to_be_modified_item_s {
  bool                                             remote_ue_local_id_present  = false;
  bool                                             pc5_rlc_ch_qos_info_present = false;
  bool                                             rlc_mode_present            = false;
  bool                                             ie_exts_present             = false;
  uint16_t                                         pc5_rlc_ch_id               = 1;
  uint16_t                                         remote_ue_local_id          = 0;
  pc5_rlc_ch_qos_info_c                            pc5_rlc_ch_qos_info;
  rlc_mode_e                                       rlc_mode;
  pc5_rlc_ch_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelToBeModifiedList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelToBeModifiedItem
using pc5_rlc_ch_to_be_modified_list_l = dyn_array<pc5_rlc_ch_to_be_modified_item_s>;

// PC5RLCChannelToBeReleasedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pc5_rlc_ch_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using pc5_rlc_ch_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5RLCChannelToBeReleasedItem ::= SEQUENCE
struct pc5_rlc_ch_to_be_released_item_s {
  bool                                             remote_ue_local_id_present = false;
  bool                                             ie_exts_present            = false;
  uint16_t                                         pc5_rlc_ch_id              = 1;
  uint16_t                                         remote_ue_local_id         = 0;
  pc5_rlc_ch_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelToBeReleasedList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelToBeReleasedItem
using pc5_rlc_ch_to_be_released_list_l = dyn_array<pc5_rlc_ch_to_be_released_item_s>;

// PC5RLCChannelToBeSetupItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct pc5_rlc_ch_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pc5_rlc_ch_to_be_setup_item_ext_ies_container {
  bool                             peer_ue_id_present = false;
  fixed_bitstring<24, false, true> peer_ue_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelToBeSetupItem ::= SEQUENCE
struct pc5_rlc_ch_to_be_setup_item_s {
  bool                                          remote_ue_local_id_present = false;
  bool                                          ie_exts_present            = false;
  uint16_t                                      pc5_rlc_ch_id              = 1;
  uint16_t                                      remote_ue_local_id         = 0;
  pc5_rlc_ch_qos_info_c                         pc5_rlc_ch_qos_info;
  rlc_mode_e                                    rlc_mode;
  pc5_rlc_ch_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5RLCChannelToBeSetupList ::= SEQUENCE (SIZE (1..512)) OF PC5RLCChannelToBeSetupItem
using pc5_rlc_ch_to_be_setup_list_l = dyn_array<pc5_rlc_ch_to_be_setup_item_s>;

// PDCMeasuredResults-Value-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using pdc_measured_results_value_ext_ies_o = protocol_ies_empty_o;

// PDCMeasuredResults-Value ::= CHOICE
struct pdc_measured_results_value_c {
  struct types_opts {
    enum options { pdc_tadv_nr, pdc_rx_tx_time_diff, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  pdc_measured_results_value_c() = default;
  pdc_measured_results_value_c(const pdc_measured_results_value_c& other);
  pdc_measured_results_value_c& operator=(const pdc_measured_results_value_c& other);
  ~pdc_measured_results_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint32_t& pdc_tadv_nr()
  {
    assert_choice_type(types::pdc_tadv_nr, type_, "PDCMeasuredResults-Value");
    return c.get<uint32_t>();
  }
  uint32_t& pdc_rx_tx_time_diff()
  {
    assert_choice_type(types::pdc_rx_tx_time_diff, type_, "PDCMeasuredResults-Value");
    return c.get<uint32_t>();
  }
  protocol_ie_single_container_s<pdc_measured_results_value_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PDCMeasuredResults-Value");
    return c.get<protocol_ie_single_container_s<pdc_measured_results_value_ext_ies_o>>();
  }
  const uint32_t& pdc_tadv_nr() const
  {
    assert_choice_type(types::pdc_tadv_nr, type_, "PDCMeasuredResults-Value");
    return c.get<uint32_t>();
  }
  const uint32_t& pdc_rx_tx_time_diff() const
  {
    assert_choice_type(types::pdc_rx_tx_time_diff, type_, "PDCMeasuredResults-Value");
    return c.get<uint32_t>();
  }
  const protocol_ie_single_container_s<pdc_measured_results_value_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PDCMeasuredResults-Value");
    return c.get<protocol_ie_single_container_s<pdc_measured_results_value_ext_ies_o>>();
  }
  uint32_t&                                                             set_pdc_tadv_nr();
  uint32_t&                                                             set_pdc_rx_tx_time_diff();
  protocol_ie_single_container_s<pdc_measured_results_value_ext_ies_o>& set_choice_ext();

private:
  types                                                                                 type_;
  choice_buffer_t<protocol_ie_single_container_s<pdc_measured_results_value_ext_ies_o>> c;

  void destroy_();
};

// PDCMeasuredResults-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pdc_measured_results_item_ext_ies_o = protocol_ext_empty_o;

using pdc_measured_results_item_ext_ies_container = protocol_ext_container_empty_l;

// PDCMeasuredResults-Item ::= SEQUENCE
struct pdc_measured_results_item_s {
  bool                                        ie_exts_present = false;
  pdc_measured_results_value_c                pdc_measured_results_value;
  pdc_measured_results_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDCMeasuredResultsList ::= SEQUENCE (SIZE (1..16)) OF PDCMeasuredResults-Item
using pdc_measured_results_list_l = dyn_array<pdc_measured_results_item_s>;

// PDCMeasurementPeriodicity ::= ENUMERATED
struct pdc_meas_periodicity_opts {
  enum options {
    ms80,
    ms120,
    ms160,
    ms240,
    ms320,
    ms480,
    ms640,
    ms1024,
    ms1280,
    ms2048,
    ms2560,
    ms5120,
    /*...*/ nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using pdc_meas_periodicity_e = enumerated<pdc_meas_periodicity_opts, true>;

// PDCMeasurementQuantitiesValue ::= ENUMERATED
struct pdc_meas_quantities_value_opts {
  enum options { nr_pdc_tadv, gnb_rx_tx, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pdc_meas_quantities_value_e = enumerated<pdc_meas_quantities_value_opts, true>;

// PDCMeasurementQuantitiesValue-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pdc_meas_quantities_value_ext_ies_o = protocol_ext_empty_o;

using pdc_meas_quantities_value_ext_ies_container = protocol_ext_container_empty_l;

// PDCMeasurementQuantities-Item ::= SEQUENCE
struct pdc_meas_quantities_item_s {
  bool                                        ie_exts_present = false;
  pdc_meas_quantities_value_e                 pd_cmeas_quantities_value;
  pdc_meas_quantities_value_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDCMeasurementQuantities-ItemIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct pdc_meas_quantities_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { pdc_meas_quantities_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::pdc_meas_quantities_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    pdc_meas_quantities_item_s&       pdc_meas_quantities_item() { return c; }
    const pdc_meas_quantities_item_s& pdc_meas_quantities_item() const { return c; }

  private:
    pdc_meas_quantities_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// PDCMeasurementQuantities ::= SEQUENCE (SIZE (1..16)) OF ProtocolIE-Field{F1AP-PROTOCOL-IES : IEsSetParam}
using pdc_meas_quantities_l = bounded_array<protocol_ie_single_container_s<pdc_meas_quantities_item_ies_o>, 16>;

// PDCMeasurementResult-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pdc_meas_result_ext_ies_o = protocol_ext_empty_o;

using pdc_meas_result_ext_ies_container = protocol_ext_container_empty_l;

// PDCMeasurementResult ::= SEQUENCE
struct pdc_meas_result_s {
  bool                              ie_exts_present = false;
  pdc_measured_results_list_l       pdc_measured_results_list;
  pdc_meas_result_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDCReportType ::= ENUMERATED
struct pdc_report_type_opts {
  enum options { on_demand, periodic, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pdc_report_type_e = enumerated<pdc_report_type_opts, true>;

// PEIPSAssistanceInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pe_ip_s_assist_info_ext_ies_o = protocol_ext_empty_o;

using pe_ip_s_assist_info_ext_ies_container = protocol_ext_container_empty_l;

// PEIPSAssistanceInfo ::= SEQUENCE
struct pe_ip_s_assist_info_s {
  bool                                  ie_exts_present = false;
  uint8_t                               cn_subgroup_id  = 0;
  pe_ip_s_assist_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PEISubgroupingSupportIndication ::= ENUMERATED
struct pei_subgrouping_support_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pei_subgrouping_support_ind_e = enumerated<pei_subgrouping_support_ind_opts, true>;

// PRS-Measurement-Info-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_meas_info_list_item_ext_ies_o = protocol_ext_empty_o;

using prs_meas_info_list_item_ext_ies_container = protocol_ext_container_empty_l;

// PRS-Measurement-Info-List-Item ::= SEQUENCE
struct prs_meas_info_list_item_s {
  struct meas_prs_periodicity_opts {
    enum options { ms20, ms40, ms80, ms160, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using meas_prs_periodicity_e_ = enumerated<meas_prs_periodicity_opts, true>;
  struct meas_prs_len_opts {
    enum options { ms1dot5, ms3, ms3dot5, ms4, ms5dot5, ms6, ms10, ms20, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using meas_prs_len_e_ = enumerated<meas_prs_len_opts>;

  // member variables
  bool                                      ie_exts_present = false;
  uint32_t                                  point_a         = 0;
  meas_prs_periodicity_e_                   meas_prs_periodicity;
  uint16_t                                  meas_prs_offset = 0;
  meas_prs_len_e_                           meas_prs_len;
  prs_meas_info_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRS-Measurement-Info-List ::= SEQUENCE (SIZE (1..4)) OF PRS-Measurement-Info-List-Item
using prs_meas_info_list_l = dyn_array<prs_meas_info_list_item_s>;

// PRSBWAggregationRequestInfo-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prsbw_aggregation_request_info_item_ext_ies_o = protocol_ext_empty_o;

using prsbw_aggregation_request_info_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSBWAggregationRequestInfo-Item ::= SEQUENCE
struct prsbw_aggregation_request_info_item_s {
  bool                                                  ie_exts_present = false;
  dl_prsbw_aggregation_request_info_list_l              dl_prsbw_aggregation_request_info_list;
  prsbw_aggregation_request_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSBWAggregationRequestInfoList ::= SEQUENCE (SIZE (1..2)) OF PRSBWAggregationRequestInfo-Item
using prsbw_aggregation_request_info_list_l = dyn_array<prsbw_aggregation_request_info_item_s>;

// PRSConfigRequestType ::= ENUMERATED
struct prs_cfg_request_type_opts {
  enum options { cfgure, off, /*...*/ nulltype } value;

  const char* to_string() const;
};
using prs_cfg_request_type_e = enumerated<prs_cfg_request_type_opts, true>;

// PRSMutingOption1-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_muting_option1_ext_ies_o = protocol_ext_empty_o;

using prs_muting_option1_ext_ies_container = protocol_ext_container_empty_l;

// PRSMutingOption1 ::= SEQUENCE
struct prs_muting_option1_s {
  struct muting_bit_repeat_factor_opts {
    enum options { rf1, rf2, rf4, rf8, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using muting_bit_repeat_factor_e_ = enumerated<muting_bit_repeat_factor_opts, true>;

  // member variables
  bool                                 ie_exts_present = false;
  dl_prs_muting_pattern_c              muting_pattern;
  muting_bit_repeat_factor_e_          muting_bit_repeat_factor;
  prs_muting_option1_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSMutingOption2-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_muting_option2_ext_ies_o = protocol_ext_empty_o;

using prs_muting_option2_ext_ies_container = protocol_ext_container_empty_l;

// PRSMutingOption2 ::= SEQUENCE
struct prs_muting_option2_s {
  bool                                 ie_exts_present = false;
  dl_prs_muting_pattern_c              muting_pattern;
  prs_muting_option2_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSMuting-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_muting_ext_ies_o = protocol_ext_empty_o;

using prs_muting_ext_ies_container = protocol_ext_container_empty_l;

// PRSMuting ::= SEQUENCE
struct prs_muting_s {
  bool                         prs_muting_option1_present = false;
  bool                         prs_muting_option2_present = false;
  bool                         ie_exts_present            = false;
  prs_muting_option1_s         prs_muting_option1;
  prs_muting_option2_s         prs_muting_option2;
  prs_muting_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResource-QCLSourceSSB-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_res_qcl_source_ssb_ext_ies_o = protocol_ext_empty_o;

using prs_res_qcl_source_ssb_ext_ies_container = protocol_ext_container_empty_l;

// PRSResource-QCLSourceSSB ::= SEQUENCE
struct prs_res_qcl_source_ssb_s {
  bool                                     ssb_idx_present = false;
  bool                                     ie_exts_present = false;
  uint16_t                                 pci_nr          = 0;
  uint8_t                                  ssb_idx         = 0;
  prs_res_qcl_source_ssb_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResource-QCLSourcePRS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_res_qcl_source_prs_ext_ies_o = protocol_ext_empty_o;

using prs_res_qcl_source_prs_ext_ies_container = protocol_ext_container_empty_l;

// PRSResource-QCLSourcePRS ::= SEQUENCE
struct prs_res_qcl_source_prs_s {
  bool                                     qcl_source_prs_res_id_present = false;
  bool                                     ie_exts_present               = false;
  uint8_t                                  qcl_source_prs_res_set_id     = 0;
  uint8_t                                  qcl_source_prs_res_id         = 0;
  prs_res_qcl_source_prs_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResource-QCLInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using prs_res_qcl_info_ext_ies_o = protocol_ies_empty_o;

// PRSResource-QCLInfo ::= CHOICE
struct prs_res_qcl_info_c {
  struct types_opts {
    enum options { qcl_source_ssb, qcl_source_prs, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  prs_res_qcl_info_c() = default;
  prs_res_qcl_info_c(const prs_res_qcl_info_c& other);
  prs_res_qcl_info_c& operator=(const prs_res_qcl_info_c& other);
  ~prs_res_qcl_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  prs_res_qcl_source_ssb_s& qcl_source_ssb()
  {
    assert_choice_type(types::qcl_source_ssb, type_, "PRSResource-QCLInfo");
    return c.get<prs_res_qcl_source_ssb_s>();
  }
  prs_res_qcl_source_prs_s& qcl_source_prs()
  {
    assert_choice_type(types::qcl_source_prs, type_, "PRSResource-QCLInfo");
    return c.get<prs_res_qcl_source_prs_s>();
  }
  protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PRSResource-QCLInfo");
    return c.get<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>();
  }
  const prs_res_qcl_source_ssb_s& qcl_source_ssb() const
  {
    assert_choice_type(types::qcl_source_ssb, type_, "PRSResource-QCLInfo");
    return c.get<prs_res_qcl_source_ssb_s>();
  }
  const prs_res_qcl_source_prs_s& qcl_source_prs() const
  {
    assert_choice_type(types::qcl_source_prs, type_, "PRSResource-QCLInfo");
    return c.get<prs_res_qcl_source_prs_s>();
  }
  const protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PRSResource-QCLInfo");
    return c.get<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>>();
  }
  prs_res_qcl_source_ssb_s&                                   set_qcl_source_ssb();
  prs_res_qcl_source_prs_s&                                   set_qcl_source_prs();
  protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<prs_res_qcl_info_ext_ies_o>,
                  prs_res_qcl_source_prs_s,
                  prs_res_qcl_source_ssb_s>
      c;

  void destroy_();
};

// PRSResource-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct prs_res_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct prs_res_item_ext_ies_container {
  bool    extended_res_symbol_offset_present = false;
  uint8_t extended_res_symbol_offset;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResource-Item ::= SEQUENCE
struct prs_res_item_s {
  bool                           qcl_info_present  = false;
  bool                           ie_exts_present   = false;
  uint8_t                        prs_res_id        = 0;
  uint16_t                       seq_id            = 0;
  uint8_t                        re_offset         = 0;
  uint16_t                       res_slot_offset   = 0;
  uint8_t                        res_symbol_offset = 0;
  prs_res_qcl_info_c             qcl_info;
  prs_res_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResource-List ::= SEQUENCE (SIZE (1..64)) OF PRSResource-Item
using prs_res_list_l = dyn_array<prs_res_item_s>;

// PRSResourceSet-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using prs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSResourceSet-Item ::= SEQUENCE
struct prs_res_set_item_s {
  struct subcarrier_spacing_opts {
    enum options { khz15, khz30, khz60, khz120, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using subcarrier_spacing_e_ = enumerated<subcarrier_spacing_opts, true>;
  struct comb_size_opts {
    enum options { n2, n4, n6, n12, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using comb_size_e_ = enumerated<comb_size_opts, true>;
  struct cp_type_opts {
    enum options { normal, extended, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using cp_type_e_ = enumerated<cp_type_opts, true>;
  struct res_set_periodicity_opts {
    enum options {
      n4,
      n5,
      n8,
      n10,
      n16,
      n20,
      n32,
      n40,
      n64,
      n80,
      n160,
      n320,
      n640,
      n1280,
      n2560,
      n5120,
      n10240,
      n20480,
      n40960,
      n81920,
      // ...
      n128,
      n256,
      n512,
      nulltype
    } value;
    typedef uint32_t number_type;

    const char* to_string() const;
    uint32_t    to_number() const;
  };
  using res_set_periodicity_e_ = enumerated<res_set_periodicity_opts, true, 3>;
  struct res_repeat_factor_opts {
    enum options { rf1, rf2, rf4, rf6, rf8, rf16, rf32, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using res_repeat_factor_e_ = enumerated<res_repeat_factor_opts, true>;
  struct res_time_gap_opts {
    enum options { tg1, tg2, tg4, tg8, tg16, tg32, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using res_time_gap_e_ = enumerated<res_time_gap_opts, true>;
  struct res_numof_symbols_opts {
    enum options { n2, n4, n6, n12, /*...*/ n1, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using res_numof_symbols_e_ = enumerated<res_numof_symbols_opts, true, 1>;

  // member variables
  bool                               prs_muting_present = false;
  bool                               ie_exts_present    = false;
  uint8_t                            prs_res_set_id     = 0;
  subcarrier_spacing_e_              subcarrier_spacing;
  uint8_t                            pr_sbw    = 1;
  uint16_t                           start_prb = 0;
  uint32_t                           point_a   = 0;
  comb_size_e_                       comb_size;
  cp_type_e_                         cp_type;
  res_set_periodicity_e_             res_set_periodicity;
  uint32_t                           res_set_slot_offset = 0;
  res_repeat_factor_e_               res_repeat_factor;
  res_time_gap_e_                    res_time_gap;
  res_numof_symbols_e_               res_numof_symbols;
  prs_muting_s                       prs_muting;
  int8_t                             prs_res_tx_pwr = -60;
  prs_res_list_l                     prs_res_list;
  prs_res_set_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResourceSet-List ::= SEQUENCE (SIZE (1..8)) OF PRSResourceSet-Item
using prs_res_set_list_l = dyn_array<prs_res_set_item_s>;

// PRSConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct prs_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct prs_cfg_ext_ies_container {
  bool                    aggr_prs_res_set_list_present = false;
  aggr_prs_res_set_list_l aggr_prs_res_set_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSConfiguration ::= SEQUENCE
struct prs_cfg_s {
  bool                      ie_exts_present = false;
  prs_res_set_list_l        prs_res_set_list;
  prs_cfg_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedDLPRSResource-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using requested_dl_prs_res_item_ext_ies_o = protocol_ext_empty_o;

using requested_dl_prs_res_item_ext_ies_container = protocol_ext_container_empty_l;

// RequestedDLPRSResource-Item ::= SEQUENCE
struct requested_dl_prs_res_item_s {
  bool                                        qcl_info_present = false;
  bool                                        ie_exts_present  = false;
  prs_res_qcl_info_c                          qcl_info;
  requested_dl_prs_res_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedDLPRSResource-List ::= SEQUENCE (SIZE (1..64)) OF RequestedDLPRSResource-Item
using requested_dl_prs_res_list_l = dyn_array<requested_dl_prs_res_item_s>;

// StartTimeAndDuration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using start_time_and_dur_ext_ies_o = protocol_ext_empty_o;

using start_time_and_dur_ext_ies_container = protocol_ext_container_empty_l;

// StartTimeAndDuration ::= SEQUENCE
struct start_time_and_dur_s {
  bool                                 start_time_present = false;
  bool                                 dur_present        = false;
  bool                                 ie_exts_present    = false;
  fixed_bitstring<64, false, true>     start_time;
  uint32_t                             dur = 0;
  start_time_and_dur_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedDLPRSResourceSet-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using requested_dl_prs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using requested_dl_prs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// RequestedDLPRSResourceSet-Item ::= SEQUENCE
struct requested_dl_prs_res_set_item_s {
  struct comb_size_opts {
    enum options { n2, n4, n6, n12, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using comb_size_e_ = enumerated<comb_size_opts, true>;
  struct res_set_periodicity_opts {
    enum options {
      n4,
      n5,
      n8,
      n10,
      n16,
      n20,
      n32,
      n40,
      n64,
      n80,
      n160,
      n320,
      n640,
      n1280,
      n2560,
      n5120,
      n10240,
      n20480,
      n40960,
      n81920,
      // ...
      n128,
      n256,
      n512,
      nulltype
    } value;
    typedef uint32_t number_type;

    const char* to_string() const;
    uint32_t    to_number() const;
  };
  using res_set_periodicity_e_ = enumerated<res_set_periodicity_opts, true, 3>;
  struct res_repeat_factor_opts {
    enum options { rf1, rf2, rf4, rf6, rf8, rf16, rf32, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using res_repeat_factor_e_ = enumerated<res_repeat_factor_opts, true>;
  struct res_numof_symbols_opts {
    enum options { n2, n4, n6, n12, /*...*/ n1, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using res_numof_symbols_e_ = enumerated<res_numof_symbols_opts, true, 1>;

  // member variables
  bool                                            pr_sbw_present                     = false;
  bool                                            comb_size_present                  = false;
  bool                                            res_set_periodicity_present        = false;
  bool                                            res_repeat_factor_present          = false;
  bool                                            res_numof_symbols_present          = false;
  bool                                            res_set_start_time_and_dur_present = false;
  bool                                            ie_exts_present                    = false;
  uint8_t                                         pr_sbw                             = 1;
  comb_size_e_                                    comb_size;
  res_set_periodicity_e_                          res_set_periodicity;
  res_repeat_factor_e_                            res_repeat_factor;
  res_numof_symbols_e_                            res_numof_symbols;
  requested_dl_prs_res_list_l                     requested_dl_prs_res_list;
  start_time_and_dur_s                            res_set_start_time_and_dur;
  requested_dl_prs_res_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedDLPRSResourceSet-List ::= SEQUENCE (SIZE (1..8)) OF RequestedDLPRSResourceSet-Item
using requested_dl_prs_res_set_list_l = dyn_array<requested_dl_prs_res_set_item_s>;

// RequestedDLPRSTransmissionCharacteristics-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct requested_dl_prs_tx_characteristics_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct requested_dl_prs_tx_characteristics_ext_ies_container {
  bool                                  prsbw_aggregation_request_info_list_present = false;
  prsbw_aggregation_request_info_list_l prsbw_aggregation_request_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedDLPRSTransmissionCharacteristics ::= SEQUENCE
struct requested_dl_prs_tx_characteristics_s {
  bool                                                  numof_freq_layers_present  = false;
  bool                                                  start_time_and_dur_present = false;
  bool                                                  ie_exts_present            = false;
  requested_dl_prs_res_set_list_l                       requested_dl_prs_res_set_list;
  uint8_t                                               numof_freq_layers = 1;
  start_time_and_dur_s                                  start_time_and_dur;
  requested_dl_prs_tx_characteristics_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSTransmissionOffPerResourceSet-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_tx_off_per_res_set_item_ext_ies_o = protocol_ext_empty_o;

using prs_tx_off_per_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSTransmissionOffPerResourceSet-Item ::= SEQUENCE
struct prs_tx_off_per_res_set_item_s {
  bool                                          ie_exts_present = false;
  uint8_t                                       prs_res_set_id  = 0;
  prs_tx_off_per_res_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSTransmissionOffPerResourceSet ::= SEQUENCE (SIZE (1..8)) OF PRSTransmissionOffPerResourceSet-Item
using prs_tx_off_per_res_set_l = dyn_array<prs_tx_off_per_res_set_item_s>;

// PRSTransmissionOffIndicationPerResource-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_tx_off_ind_per_res_item_ext_ies_o = protocol_ext_empty_o;

using prs_tx_off_ind_per_res_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSTransmissionOffIndicationPerResource-Item ::= SEQUENCE
struct prs_tx_off_ind_per_res_item_s {
  bool                                          ie_exts_present = false;
  uint8_t                                       prs_res_id      = 0;
  prs_tx_off_ind_per_res_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSTransmissionOffPerResource-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_tx_off_per_res_item_ext_ies_o = protocol_ext_empty_o;

using prs_tx_off_per_res_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSTransmissionOffPerResource-Item ::= SEQUENCE
struct prs_tx_off_per_res_item_s {
  using prs_tx_off_ind_per_res_list_l_ = dyn_array<prs_tx_off_ind_per_res_item_s>;

  // member variables
  bool                                      ie_exts_present = false;
  uint8_t                                   prs_res_set_id  = 0;
  prs_tx_off_ind_per_res_list_l_            prs_tx_off_ind_per_res_list;
  prs_tx_off_per_res_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSTransmissionOffPerResource ::= SEQUENCE (SIZE (1..8)) OF PRSTransmissionOffPerResource-Item
using prs_tx_off_per_res_l = dyn_array<prs_tx_off_per_res_item_s>;

// PRSTransmissionOffIndication-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using prs_tx_off_ind_ext_ies_o = protocol_ies_empty_o;

// PRSTransmissionOffIndication ::= CHOICE
struct prs_tx_off_ind_c {
  struct types_opts {
    enum options { prs_tx_off_per_trp, prs_tx_off_per_res_set, prs_tx_off_per_res, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  prs_tx_off_ind_c() = default;
  prs_tx_off_ind_c(const prs_tx_off_ind_c& other);
  prs_tx_off_ind_c& operator=(const prs_tx_off_ind_c& other);
  ~prs_tx_off_ind_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  prs_tx_off_per_res_set_l& prs_tx_off_per_res_set()
  {
    assert_choice_type(types::prs_tx_off_per_res_set, type_, "PRSTransmissionOffIndication");
    return c.get<prs_tx_off_per_res_set_l>();
  }
  prs_tx_off_per_res_l& prs_tx_off_per_res()
  {
    assert_choice_type(types::prs_tx_off_per_res, type_, "PRSTransmissionOffIndication");
    return c.get<prs_tx_off_per_res_l>();
  }
  protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PRSTransmissionOffIndication");
    return c.get<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>();
  }
  const prs_tx_off_per_res_set_l& prs_tx_off_per_res_set() const
  {
    assert_choice_type(types::prs_tx_off_per_res_set, type_, "PRSTransmissionOffIndication");
    return c.get<prs_tx_off_per_res_set_l>();
  }
  const prs_tx_off_per_res_l& prs_tx_off_per_res() const
  {
    assert_choice_type(types::prs_tx_off_per_res, type_, "PRSTransmissionOffIndication");
    return c.get<prs_tx_off_per_res_l>();
  }
  const protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PRSTransmissionOffIndication");
    return c.get<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>>();
  }
  void                                                      set_prs_tx_off_per_trp();
  prs_tx_off_per_res_set_l&                                 set_prs_tx_off_per_res_set();
  prs_tx_off_per_res_l&                                     set_prs_tx_off_per_res();
  protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<prs_tx_off_ind_ext_ies_o>,
                  prs_tx_off_per_res_l,
                  prs_tx_off_per_res_set_l>
      c;

  void destroy_();
};

// PRSTransmissionOffInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_tx_off_info_ext_ies_o = protocol_ext_empty_o;

using prs_tx_off_info_ext_ies_container = protocol_ext_container_empty_l;

// PRSTransmissionOffInformation ::= SEQUENCE
struct prs_tx_off_info_s {
  bool                              ie_exts_present = false;
  prs_tx_off_ind_c                  prs_tx_off_ind;
  prs_tx_off_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSTRPItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prstrp_item_ext_ies_o = protocol_ext_empty_o;

using prstrp_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSTRPItem ::= SEQUENCE
struct prstrp_item_s {
  bool                                  requested_dl_prs_tx_characteristics_present = false;
  bool                                  prs_tx_off_info_present                     = false;
  bool                                  ie_exts_present                             = false;
  uint32_t                              trp_id                                      = 0;
  requested_dl_prs_tx_characteristics_s requested_dl_prs_tx_characteristics;
  prs_tx_off_info_s                     prs_tx_off_info;
  prstrp_item_ext_ies_container         ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSTRPList ::= SEQUENCE (SIZE (1..65535)) OF PRSTRPItem
using prstrp_list_l = dyn_array<prstrp_item_s>;

// PRSTransmissionTRPItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using prs_tx_trp_item_ext_ies_o = protocol_ext_empty_o;

using prs_tx_trp_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSTransmissionTRPItem ::= SEQUENCE
struct prs_tx_trp_item_s {
  bool                              ie_exts_present = false;
  uint32_t                          trp_id          = 0;
  prs_cfg_s                         prs_cfg;
  prs_tx_trp_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSTransmissionTRPList ::= SEQUENCE (SIZE (1..65535)) OF PRSTransmissionTRPItem
using prs_tx_trp_list_l = dyn_array<prs_tx_trp_item_s>;

// PWS-Failed-NR-CGI-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pws_failed_nr_cgi_item_ext_ies_o = protocol_ext_empty_o;

using pws_failed_nr_cgi_item_ext_ies_container = protocol_ext_container_empty_l;

// PWS-Failed-NR-CGI-Item ::= SEQUENCE
struct pws_failed_nr_cgi_item_s {
  bool                                     ie_exts_present = false;
  nr_cgi_s                                 nr_cgi;
  uint32_t                                 nof_broadcasts = 0;
  pws_failed_nr_cgi_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PWSSystemInformationExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct pws_sys_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pws_sys_info_ext_ies_container {
  bool               notif_info_present       = false;
  bool               add_sib_msg_list_present = false;
  notif_info_s       notif_info;
  add_sib_msg_list_l add_sib_msg_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PWSSystemInformation ::= SEQUENCE
struct pws_sys_info_s {
  bool                           ie_exts_present = false;
  uint8_t                        sib_type        = 6;
  unbounded_octstring<true>      sib_msg;
  pws_sys_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PagingCause ::= ENUMERATED
struct paging_cause_opts {
  enum options { voice, /*...*/ nulltype } value;

  const char* to_string() const;
};
using paging_cause_e = enumerated<paging_cause_opts, true>;

// RecommendedSSBItem-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using recommended_ssb_item_list_item_ext_ies_o = protocol_ext_empty_o;

using recommended_ssb_item_list_item_ext_ies_container = protocol_ext_container_empty_l;

// RecommendedSSBItem-List-Item ::= SEQUENCE
struct recommended_ssb_item_list_item_s {
  bool                                             ie_exts_present = false;
  uint8_t                                          ssb_idx         = 0;
  recommended_ssb_item_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Recommended-SSBs-List ::= SEQUENCE (SIZE (1..64)) OF RecommendedSSBItem-List-Item
using recommended_ss_bs_list_l = dyn_array<recommended_ssb_item_list_item_s>;

// PagingCell-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct paging_cell_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct paging_cell_item_ext_ies_container {
  bool                          last_used_cell_ind_present          = false;
  bool                          pei_subgrouping_support_ind_present = false;
  bool                          recommended_ss_bs_list_present      = false;
  last_used_cell_ind_e          last_used_cell_ind;
  pei_subgrouping_support_ind_e pei_subgrouping_support_ind;
  recommended_ss_bs_list_l      recommended_ss_bs_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PagingCell-Item ::= SEQUENCE
struct paging_cell_item_s {
  bool                               ie_exts_present = false;
  nr_cgi_s                           nr_cgi;
  paging_cell_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PagingDRX ::= ENUMERATED
struct paging_drx_opts {
  enum options { v32, v64, v128, v256, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using paging_drx_e = enumerated<paging_drx_opts, true>;

// RANUEPagingIdentity-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ran_ue_paging_id_ext_ies_o = protocol_ext_empty_o;

using ran_ue_paging_id_ext_ies_container = protocol_ext_container_empty_l;

// RANUEPagingIdentity ::= SEQUENCE
struct ran_ue_paging_id_s {
  bool                               ie_exts_present = false;
  fixed_bitstring<40, false, true>   irnti;
  ran_ue_paging_id_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PagingIdentity-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using paging_id_ext_ies_o = protocol_ies_empty_o;

// PagingIdentity ::= CHOICE
struct paging_id_c {
  struct types_opts {
    enum options { ran_ue_paging_id, cn_ue_paging_id, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  paging_id_c() = default;
  paging_id_c(const paging_id_c& other);
  paging_id_c& operator=(const paging_id_c& other);
  ~paging_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ran_ue_paging_id_s& ran_ue_paging_id()
  {
    assert_choice_type(types::ran_ue_paging_id, type_, "PagingIdentity");
    return c.get<ran_ue_paging_id_s>();
  }
  cn_ue_paging_id_c& cn_ue_paging_id()
  {
    assert_choice_type(types::cn_ue_paging_id, type_, "PagingIdentity");
    return c.get<cn_ue_paging_id_c>();
  }
  protocol_ie_single_container_s<paging_id_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PagingIdentity");
    return c.get<protocol_ie_single_container_s<paging_id_ext_ies_o>>();
  }
  const ran_ue_paging_id_s& ran_ue_paging_id() const
  {
    assert_choice_type(types::ran_ue_paging_id, type_, "PagingIdentity");
    return c.get<ran_ue_paging_id_s>();
  }
  const cn_ue_paging_id_c& cn_ue_paging_id() const
  {
    assert_choice_type(types::cn_ue_paging_id, type_, "PagingIdentity");
    return c.get<cn_ue_paging_id_c>();
  }
  const protocol_ie_single_container_s<paging_id_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PagingIdentity");
    return c.get<protocol_ie_single_container_s<paging_id_ext_ies_o>>();
  }
  ran_ue_paging_id_s&                                  set_ran_ue_paging_id();
  cn_ue_paging_id_c&                                   set_cn_ue_paging_id();
  protocol_ie_single_container_s<paging_id_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                       type_;
  choice_buffer_t<cn_ue_paging_id_c, protocol_ie_single_container_s<paging_id_ext_ies_o>, ran_ue_paging_id_s> c;

  void destroy_();
};

// PagingOrigin ::= ENUMERATED
struct paging_origin_opts {
  enum options { non_neg3gpp, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using paging_origin_e = enumerated<paging_origin_opts, true>;

// PagingPriority ::= ENUMERATED
struct paging_prio_opts {
  enum options {
    priolevel1,
    priolevel2,
    priolevel3,
    priolevel4,
    priolevel5,
    priolevel6,
    priolevel7,
    priolevel8,
    // ...
    nulltype
  } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using paging_prio_e = enumerated<paging_prio_opts, true>;

// ParentTImeSource ::= ENUMERATED
struct parent_t_ime_source_opts {
  enum options {
    synce,
    ptp,
    gnss,
    atomicclock,
    terrestrialradio,
    serialtimecode,
    ntp,
    handset,
    other,
    /*...*/ nulltype
  } value;

  const char* to_string() const;
};
using parent_t_ime_source_e = enumerated<parent_t_ime_source_opts, true>;

// PathAdditionInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using path_addition_info_ext_ies_o = protocol_ies_empty_o;

// PathAdditionInformation ::= CHOICE
struct path_addition_info_c {
  struct types_opts {
    enum options {
      indirect_path_addition,
      direct_path_addition,
      n3_c_indirect_path_addition,
      choice_ext,
      nulltype
    } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  path_addition_info_c() = default;
  path_addition_info_c(const path_addition_info_c& other);
  path_addition_info_c& operator=(const path_addition_info_c& other);
  ~path_addition_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  indirect_path_addition_s& indirect_path_addition()
  {
    assert_choice_type(types::indirect_path_addition, type_, "PathAdditionInformation");
    return c.get<indirect_path_addition_s>();
  }
  n3_c_indirect_path_addition_s& n3_c_indirect_path_addition()
  {
    assert_choice_type(types::n3_c_indirect_path_addition, type_, "PathAdditionInformation");
    return c.get<n3_c_indirect_path_addition_s>();
  }
  protocol_ie_single_container_s<path_addition_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PathAdditionInformation");
    return c.get<protocol_ie_single_container_s<path_addition_info_ext_ies_o>>();
  }
  const indirect_path_addition_s& indirect_path_addition() const
  {
    assert_choice_type(types::indirect_path_addition, type_, "PathAdditionInformation");
    return c.get<indirect_path_addition_s>();
  }
  const n3_c_indirect_path_addition_s& n3_c_indirect_path_addition() const
  {
    assert_choice_type(types::n3_c_indirect_path_addition, type_, "PathAdditionInformation");
    return c.get<n3_c_indirect_path_addition_s>();
  }
  const protocol_ie_single_container_s<path_addition_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PathAdditionInformation");
    return c.get<protocol_ie_single_container_s<path_addition_info_ext_ies_o>>();
  }
  indirect_path_addition_s&                                     set_indirect_path_addition();
  void                                                          set_direct_path_addition();
  n3_c_indirect_path_addition_s&                                set_n3_c_indirect_path_addition();
  protocol_ie_single_container_s<path_addition_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<indirect_path_addition_s,
                  n3_c_indirect_path_addition_s,
                  protocol_ie_single_container_s<path_addition_info_ext_ies_o>>
      c;

  void destroy_();
};

// PathSwitchConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using path_switch_cfg_ext_ies_o = protocol_ext_empty_o;

using path_switch_cfg_ext_ies_container = protocol_ext_container_empty_l;

// PathSwitchConfiguration ::= SEQUENCE
struct path_switch_cfg_s {
  struct t420_opts {
    enum options { ms50, ms100, ms150, ms200, ms500, ms1000, ms2000, ms10000, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using t420_e_ = enumerated<t420_opts>;

  // member variables
  bool                              ie_exts_present = false;
  fixed_bitstring<24, false, true>  target_relay_ue_id;
  uint16_t                          remote_ue_local_id = 0;
  t420_e_                           t420;
  path_switch_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PathlossReferenceSignal-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using pathloss_ref_sig_ext_ies_o = protocol_ies_empty_o;

// PathlossReferenceSignal ::= CHOICE
struct pathloss_ref_sig_c {
  struct types_opts {
    enum options { ssb, dl_prs, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  pathloss_ref_sig_c() = default;
  pathloss_ref_sig_c(const pathloss_ref_sig_c& other);
  pathloss_ref_sig_c& operator=(const pathloss_ref_sig_c& other);
  ~pathloss_ref_sig_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ssb_s& ssb()
  {
    assert_choice_type(types::ssb, type_, "PathlossReferenceSignal");
    return c.get<ssb_s>();
  }
  dl_prs_s& dl_prs()
  {
    assert_choice_type(types::dl_prs, type_, "PathlossReferenceSignal");
    return c.get<dl_prs_s>();
  }
  protocol_ie_single_container_s<pathloss_ref_sig_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PathlossReferenceSignal");
    return c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ies_o>>();
  }
  const ssb_s& ssb() const
  {
    assert_choice_type(types::ssb, type_, "PathlossReferenceSignal");
    return c.get<ssb_s>();
  }
  const dl_prs_s& dl_prs() const
  {
    assert_choice_type(types::dl_prs, type_, "PathlossReferenceSignal");
    return c.get<dl_prs_s>();
  }
  const protocol_ie_single_container_s<pathloss_ref_sig_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PathlossReferenceSignal");
    return c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ies_o>>();
  }
  ssb_s&                                                      set_ssb();
  dl_prs_s&                                                   set_dl_prs();
  protocol_ie_single_container_s<pathloss_ref_sig_ext_ies_o>& set_choice_ext();

private:
  types                                                                                        type_;
  choice_buffer_t<dl_prs_s, protocol_ie_single_container_s<pathloss_ref_sig_ext_ies_o>, ssb_s> c;

  void destroy_();
};

// PathlossReferenceInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pathloss_ref_info_ext_ies_o = protocol_ext_empty_o;

using pathloss_ref_info_ext_ies_container = protocol_ext_container_empty_l;

// PathlossReferenceInfo ::= SEQUENCE
struct pathloss_ref_info_s {
  bool                                ie_exts_present = false;
  pathloss_ref_sig_c                  pathloss_ref_sig;
  pathloss_ref_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PeriodicitySRS ::= ENUMERATED
struct periodicity_srs_opts {
  enum options {
    ms0p125,
    ms0p25,
    ms0p5,
    ms0p625,
    ms1,
    ms1p25,
    ms2,
    ms2p5,
    ms4,
    ms5,
    ms8,
    ms10,
    ms16,
    ms20,
    ms32,
    ms40,
    ms64,
    ms80,
    ms160,
    ms320,
    ms640,
    ms1280,
    ms2560,
    ms5120,
    ms10240,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using periodicity_srs_e = enumerated<periodicity_srs_opts, true>;

// PeriodicityList-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using periodicity_list_item_ext_ies_o = protocol_ext_empty_o;

using periodicity_list_item_ext_ies_container = protocol_ext_container_empty_l;

// PeriodicityList-Item ::= SEQUENCE
struct periodicity_list_item_s {
  bool                                    ie_exts_present = false;
  periodicity_srs_e                       periodicity_srs;
  periodicity_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PeriodicityList ::= SEQUENCE (SIZE (1..16)) OF PeriodicityList-Item
using periodicity_list_l = dyn_array<periodicity_list_item_s>;

// PosBroadcast ::= ENUMERATED
struct pos_broadcast_opts {
  enum options { start, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pos_broadcast_e = enumerated<pos_broadcast_opts, true>;

// PosContextRevIndication ::= ENUMERATED
struct pos_context_rev_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pos_context_rev_ind_e = enumerated<pos_context_rev_ind_opts, true>;

// PosMeasGapPreConfigList-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pos_meas_gap_pre_cfg_list_ext_ies_o = protocol_ext_empty_o;

using pos_meas_gap_pre_cfg_list_ext_ies_container = protocol_ext_container_empty_l;

// PosMeasGapPreConfigList ::= SEQUENCE
struct pos_meas_gap_pre_cfg_list_s {
  bool                                        ie_exts_present = false;
  unbounded_octstring<true>                   pos_meas_gap_pre_cfg_to_add_mod_list;
  unbounded_octstring<true>                   pos_meas_gap_pre_cfg_to_release_list;
  pos_meas_gap_pre_cfg_list_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosMeasurementAmount ::= ENUMERATED
struct pos_meas_amount_opts {
  enum options { ma0, ma1, ma2, ma4, ma8, ma16, ma32, ma64, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using pos_meas_amount_e = enumerated<pos_meas_amount_opts>;

// PosMeasurementPeriodicityNR-AoA ::= ENUMERATED
struct pos_meas_periodicity_nr_ao_a_opts {
  enum options {
    ms160,
    ms320,
    ms640,
    ms1280,
    ms2560,
    ms5120,
    ms10240,
    ms20480,
    ms40960,
    ms61440,
    ms81920,
    ms368640,
    ms737280,
    ms1843200,
    // ...
    nulltype
  } value;
  typedef uint32_t number_type;

  const char* to_string() const;
  uint32_t    to_number() const;
};
using pos_meas_periodicity_nr_ao_a_e = enumerated<pos_meas_periodicity_nr_ao_a_opts, true>;

// PosMeasurementType ::= ENUMERATED
struct pos_meas_type_opts {
  enum options {
    gnb_rx_tx,
    ul_srs_rsrp,
    ul_aoa,
    ul_rtoa,
    /*...*/ multiple_ul_aoa,
    ul_srs_rsrpp,
    ul_rscp,
    nulltype
  } value;

  const char* to_string() const;
};
using pos_meas_type_e = enumerated<pos_meas_type_opts, true, 3>;

// PosMeasurementQuantities-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct pos_meas_quantities_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pos_meas_quantities_item_ext_ies_container {
  bool   timing_report_granularity_factor_extended_present = false;
  int8_t timing_report_granularity_factor_extended;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosMeasurementQuantities-Item ::= SEQUENCE
struct pos_meas_quantities_item_s {
  bool                                       timing_report_granularity_factor_present = false;
  bool                                       ie_exts_present                          = false;
  pos_meas_type_e                            pos_meas_type;
  uint8_t                                    timing_report_granularity_factor = 0;
  pos_meas_quantities_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosMeasurementQuantities ::= SEQUENCE (SIZE (1..16384)) OF PosMeasurementQuantities-Item
using pos_meas_quantities_l = dyn_array<pos_meas_quantities_item_s>;

// SRSInfo ::= SEQUENCE
struct srs_info_s {
  uint8_t srs_res = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSInfo ::= SEQUENCE
struct pos_srs_info_s {
  uint8_t pos_srs_res_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResourceTypeChoice-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using srs_res_type_choice_ext_ies_o = protocol_ies_empty_o;

// SRSResourceTypeChoice ::= CHOICE
struct srs_res_type_choice_c {
  struct types_opts {
    enum options { srs_res_info, pos_srs_res_info, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  srs_res_type_choice_c() = default;
  srs_res_type_choice_c(const srs_res_type_choice_c& other);
  srs_res_type_choice_c& operator=(const srs_res_type_choice_c& other);
  ~srs_res_type_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  srs_info_s& srs_res_info()
  {
    assert_choice_type(types::srs_res_info, type_, "SRSResourceTypeChoice");
    return c.get<srs_info_s>();
  }
  pos_srs_info_s& pos_srs_res_info()
  {
    assert_choice_type(types::pos_srs_res_info, type_, "SRSResourceTypeChoice");
    return c.get<pos_srs_info_s>();
  }
  protocol_ie_single_container_s<srs_res_type_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SRSResourceTypeChoice");
    return c.get<protocol_ie_single_container_s<srs_res_type_choice_ext_ies_o>>();
  }
  const srs_info_s& srs_res_info() const
  {
    assert_choice_type(types::srs_res_info, type_, "SRSResourceTypeChoice");
    return c.get<srs_info_s>();
  }
  const pos_srs_info_s& pos_srs_res_info() const
  {
    assert_choice_type(types::pos_srs_res_info, type_, "SRSResourceTypeChoice");
    return c.get<pos_srs_info_s>();
  }
  const protocol_ie_single_container_s<srs_res_type_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SRSResourceTypeChoice");
    return c.get<protocol_ie_single_container_s<srs_res_type_choice_ext_ies_o>>();
  }
  srs_info_s&                                                    set_srs_res_info();
  pos_srs_info_s&                                                set_pos_srs_res_info();
  protocol_ie_single_container_s<srs_res_type_choice_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                      type_;
  choice_buffer_t<pos_srs_info_s, protocol_ie_single_container_s<srs_res_type_choice_ext_ies_o>, srs_info_s> c;

  void destroy_();
};

// SRSPortIndex ::= ENUMERATED
struct srs_port_idx_opts {
  enum options { id1000, id1001, id1002, id1003, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using srs_port_idx_e = enumerated<srs_port_idx_opts, true>;

// SRSResourcetype-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct srs_restype_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct srs_restype_ext_ies_container {
  bool           srs_port_idx_present = false;
  srs_port_idx_e srs_port_idx;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResourcetype ::= SEQUENCE
struct srs_restype_s {
  bool                          ie_exts_present = false;
  srs_res_type_choice_c         srs_res_type_choice;
  srs_restype_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosMeasurementResultItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct pos_meas_result_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pos_meas_result_item_ext_ies_container {
  bool                       arp_id_present                   = false;
  bool                       srs_restype_present              = false;
  bool                       lo_s_n_lo_si_nformation_present  = false;
  bool                       mobile_trp_location_info_present = false;
  bool                       aggr_pos_srs_res_id_list_present = false;
  bool                       measured_freq_hops_present       = false;
  bool                       meas_based_on_aggr_res_present   = false;
  uint8_t                    arp_id;
  srs_restype_s              srs_restype;
  lo_s_n_lo_si_nformation_c  lo_s_n_lo_si_nformation;
  mobile_trp_location_info_s mobile_trp_location_info;
  aggr_pos_srs_res_id_list_l aggr_pos_srs_res_id_list;
  measured_freq_hops_e       measured_freq_hops;
  meas_based_on_aggr_res_e   meas_based_on_aggr_res;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosMeasurementResultItem ::= SEQUENCE
struct pos_meas_result_item_s {
  bool                                   meas_quality_present   = false;
  bool                                   meas_beam_info_present = false;
  bool                                   ie_exts_present        = false;
  measured_results_value_c               measured_results_value;
  time_stamp_s                           time_stamp;
  trp_meas_quality_s                     meas_quality;
  meas_beam_info_s                       meas_beam_info;
  pos_meas_result_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosMeasurementResult ::= SEQUENCE (SIZE (1..16384)) OF PosMeasurementResultItem
using pos_meas_result_l = dyn_array<pos_meas_result_item_s>;

// PosMeasurementResultList-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct pos_meas_result_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pos_meas_result_list_item_ext_ies_container {
  bool     nr_cgi_present = false;
  nr_cgi_s nr_cgi;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosMeasurementResultList-Item ::= SEQUENCE
struct pos_meas_result_list_item_s {
  bool                                        ie_exts_present = false;
  pos_meas_result_l                           pos_meas_result;
  uint32_t                                    trp_id = 0;
  pos_meas_result_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosMeasurementResultList ::= SEQUENCE (SIZE (1..64)) OF PosMeasurementResultList-Item
using pos_meas_result_list_l = dyn_array<pos_meas_result_list_item_s>;

// PosReportCharacteristics ::= ENUMERATED
struct pos_report_characteristics_opts {
  enum options { ondemand, periodic, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pos_report_characteristics_e = enumerated<pos_report_characteristics_opts, true>;

// PosSItype-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pos_sitype_item_ext_ies_o = protocol_ext_empty_o;

using pos_sitype_item_ext_ies_container = protocol_ext_container_empty_l;

// PosSItype-Item ::= SEQUENCE
struct pos_sitype_item_s {
  bool                              ie_exts_present = false;
  uint8_t                           pos_itype       = 1;
  pos_sitype_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSItypeList ::= SEQUENCE (SIZE (1..32)) OF PosSItype-Item
using pos_sitype_list_l = dyn_array<pos_sitype_item_s>;

// PosValidityAreaCellList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using pos_validity_area_cell_list_item_ext_ies_o = protocol_ext_empty_o;

using pos_validity_area_cell_list_item_ext_ies_container = protocol_ext_container_empty_l;

// PosValidityAreaCellList-Item ::= SEQUENCE
struct pos_validity_area_cell_list_item_s {
  bool                                               nr_pci_present  = false;
  bool                                               ie_exts_present = false;
  nr_cgi_s                                           nr_cgi;
  uint16_t                                           nr_pci = 0;
  pos_validity_area_cell_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosValidityAreaCellList ::= SEQUENCE (SIZE (1..32)) OF PosValidityAreaCellList-Item
using pos_validity_area_cell_list_l = dyn_array<pos_validity_area_cell_list_item_s>;

// PositioningBroadcastCells ::= SEQUENCE (SIZE (1..16384)) OF NRCGI
using positioning_broadcast_cells_l = dyn_array<nr_cgi_s>;

// Potential-SpCell-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using potential_sp_cell_item_ext_ies_o = protocol_ext_empty_o;

using potential_sp_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// Potential-SpCell-Item ::= SEQUENCE
struct potential_sp_cell_item_s {
  bool                                     ie_exts_present = false;
  nr_cgi_s                                 potential_sp_cell_id;
  potential_sp_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PrivacyIndicator ::= ENUMERATED
struct privacy_ind_opts {
  enum options { immediate_mdt, logged_mdt, /*...*/ nulltype } value;

  const char* to_string() const;
};
using privacy_ind_e = enumerated<privacy_ind_opts, true>;

// Protected-EUTRA-Resources-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using protected_eutra_res_item_ext_ies_o = protocol_ext_empty_o;

using protected_eutra_res_item_ext_ies_container = protocol_ext_container_empty_l;

// Protected-EUTRA-Resources-Item ::= SEQUENCE
struct protected_eutra_res_item_s {
  bool                                       ie_exts_present       = false;
  uint16_t                                   spec_sharing_group_id = 1;
  eutra_cells_list_l                         eutra_cells_list;
  protected_eutra_res_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoEMetrics-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using qo_e_metrics_ext_ies_o = protocol_ext_empty_o;

using qo_e_metrics_ext_ies_container = protocol_ext_container_empty_l;

// QoEMetrics ::= SEQUENCE
struct qo_e_metrics_s {
  bool                           ie_exts_present = false;
  unbounded_octstring<true>      app_layer_buffer_level_list;
  unbounded_octstring<true>      playout_delay_for_media_startup;
  qo_e_metrics_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoEInformationList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct qo_e_info_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qo_e_info_list_item_ext_ies_container {
  bool       drb_list_present = false;
  drb_list_l drb_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoEInformationList-Item ::= SEQUENCE
struct qo_e_info_list_item_s {
  bool                                  qo_e_metrics_present = false;
  bool                                  ie_exts_present      = false;
  qo_e_metrics_s                        qo_e_metrics;
  qo_e_info_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoEInformationList ::= SEQUENCE (SIZE (1..16)) OF QoEInformationList-Item
using qo_e_info_list_l = dyn_array<qo_e_info_list_item_s>;

// QoEInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using qo_e_info_ext_ies_o = protocol_ext_empty_o;

using qo_e_info_ext_ies_container = protocol_ext_container_empty_l;

// QoEInformation ::= SEQUENCE
struct qo_e_info_s {
  bool                        ie_exts_present = false;
  qo_e_info_list_l            qo_e_info_list;
  qo_e_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANSharingAssistanceInformation ::= ENUMERATED
struct ran_sharing_assist_info_opts {
  enum options { mbs_session_in_non_shared_cell_res, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ran_sharing_assist_info_e = enumerated<ran_sharing_assist_info_opts, true>;

// RANTSSRequestType ::= ENUMERATED
struct rantss_request_type_opts {
  enum options { start, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using rantss_request_type_e = enumerated<rantss_request_type_opts, true>;

// RANTimingSynchronisationStatusInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ran_timing_synchronisation_status_info_ext_ies_o = protocol_ext_empty_o;

using ran_timing_synchronisation_status_info_ext_ies_container = protocol_ext_container_empty_l;

// RANTimingSynchronisationStatusInfo ::= SEQUENCE
struct ran_timing_synchronisation_status_info_s {
  struct synchronisationstate_opts {
    enum options { locked, holdover, free_run, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using synchronisationstate_e_ = enumerated<synchronisationstate_opts, true>;
  struct traceableto_utc_opts {
    enum options { true_value, false_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using traceableto_utc_e_ = enumerated<traceableto_utc_opts, true>;
  struct traceableto_gnss_opts {
    enum options { true_value, false_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using traceableto_gnss_e_ = enumerated<traceableto_gnss_opts, true>;

  // member variables
  bool                                                     synchronisationstate_present = false;
  bool                                                     traceableto_utc_present      = false;
  bool                                                     traceableto_gnss_present     = false;
  bool                                                     clock_freq_stability_present = false;
  bool                                                     clock_accuracy_present       = false;
  bool                                                     parent_time_source_present   = false;
  bool                                                     ie_exts_present              = false;
  synchronisationstate_e_                                  synchronisationstate;
  traceableto_utc_e_                                       traceableto_utc;
  traceableto_gnss_e_                                      traceableto_gnss;
  fixed_bitstring<16, false, true>                         clock_freq_stability;
  clock_accuracy_c                                         clock_accuracy;
  parent_t_ime_source_e                                    parent_time_source;
  ran_timing_synchronisation_status_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RAReportIndicationList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ra_report_ind_list_item_ext_ies_o = protocol_ext_empty_o;

using ra_report_ind_list_item_ext_ies_container = protocol_ext_container_empty_l;

// RAReportIndicationList-Item ::= SEQUENCE
struct ra_report_ind_list_item_s {
  bool                                      ie_exts_present   = false;
  uint64_t                                  gnb_cu_ue_f1ap_id = 0;
  ra_report_ind_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RAReportIndicationList ::= SEQUENCE (SIZE (1..64)) OF RAReportIndicationList-Item
using ra_report_ind_list_l = dyn_array<ra_report_ind_list_item_s>;

// RAReportItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ra_report_item_ext_ies_o = protocol_ext_empty_o;

using ra_report_item_ext_ies_container = protocol_ext_container_empty_l;

// RAReportItem ::= SEQUENCE
struct ra_report_item_s {
  bool                             ue_assitant_id_present = false;
  bool                             ie_exts_present        = false;
  unbounded_octstring<true>        ra_report_container;
  uint64_t                         ue_assitant_id = 0;
  ra_report_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RAReportList ::= SEQUENCE (SIZE (1..64)) OF RAReportItem
using ra_report_list_l = dyn_array<ra_report_item_s>;

// RAT-FrequencyPriorityInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using rat_freq_prio_info_ext_ies_o = protocol_ies_empty_o;

// RAT-FrequencyPriorityInformation ::= CHOICE
struct rat_freq_prio_info_c {
  struct types_opts {
    enum options { endc, ngran, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  rat_freq_prio_info_c() = default;
  rat_freq_prio_info_c(const rat_freq_prio_info_c& other);
  rat_freq_prio_info_c& operator=(const rat_freq_prio_info_c& other);
  ~rat_freq_prio_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint16_t& endc()
  {
    assert_choice_type(types::endc, type_, "RAT-FrequencyPriorityInformation");
    return c.get<uint16_t>();
  }
  uint16_t& ngran()
  {
    assert_choice_type(types::ngran, type_, "RAT-FrequencyPriorityInformation");
    return c.get<uint16_t>();
  }
  protocol_ie_single_container_s<rat_freq_prio_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "RAT-FrequencyPriorityInformation");
    return c.get<protocol_ie_single_container_s<rat_freq_prio_info_ext_ies_o>>();
  }
  const uint16_t& endc() const
  {
    assert_choice_type(types::endc, type_, "RAT-FrequencyPriorityInformation");
    return c.get<uint16_t>();
  }
  const uint16_t& ngran() const
  {
    assert_choice_type(types::ngran, type_, "RAT-FrequencyPriorityInformation");
    return c.get<uint16_t>();
  }
  const protocol_ie_single_container_s<rat_freq_prio_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "RAT-FrequencyPriorityInformation");
    return c.get<protocol_ie_single_container_s<rat_freq_prio_info_ext_ies_o>>();
  }
  uint16_t&                                                     set_endc();
  uint16_t&                                                     set_ngran();
  protocol_ie_single_container_s<rat_freq_prio_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                         type_;
  choice_buffer_t<protocol_ie_single_container_s<rat_freq_prio_info_ext_ies_o>> c;

  void destroy_();
};

// RLCFailureIndication-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rlc_fail_ind_ext_ies_o = protocol_ext_empty_o;

using rlc_fail_ind_ext_ies_container = protocol_ext_container_empty_l;

// RLCFailureIndication ::= SEQUENCE
struct rlc_fail_ind_s {
  bool                           ie_exts_present = false;
  uint8_t                        assocated_lcid  = 1;
  rlc_fail_ind_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RLFReportInformationItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rlf_report_info_item_ext_ies_o = protocol_ext_empty_o;

using rlf_report_info_item_ext_ies_container = protocol_ext_container_empty_l;

// RLFReportInformationItem ::= SEQUENCE
struct rlf_report_info_item_s {
  bool                                   ue_assitant_id_present = false;
  bool                                   ie_exts_present        = false;
  unbounded_octstring<true>              nr_ue_rlf_report_container;
  uint64_t                               ue_assitant_id = 0;
  rlf_report_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RLFReportInformationList ::= SEQUENCE (SIZE (1..64)) OF RLFReportInformationItem
using rlf_report_info_list_l = dyn_array<rlf_report_info_item_s>;

// RRC-Terminating-IAB-Donor-Related-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rrc_terminating_iab_donor_related_info_ext_ies_o = protocol_ext_empty_o;

using rrc_terminating_iab_donor_related_info_ext_ies_container = protocol_ext_container_empty_l;

// RRC-Terminating-IAB-Donor-Related-Info ::= SEQUENCE
struct rrc_terminating_iab_donor_related_info_s {
  bool                                                     ie_exts_present = false;
  global_gnb_id_s                                          rrc_terminating_iab_donor_gnb_id;
  fixed_bitstring<10, false, true>                         mobile_iab_mt_bap_address;
  rrc_terminating_iab_donor_related_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRC-Version-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct rrc_version_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct rrc_version_ext_ies_container {
  bool                     latest_rrc_version_enhanced_present = false;
  fixed_octstring<3, true> latest_rrc_version_enhanced;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRC-Version ::= SEQUENCE
struct rrc_version_s {
  bool                            ie_exts_present = false;
  fixed_bitstring<3, false, true> latest_rrc_version;
  rrc_version_ext_ies_container   ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCDeliveryStatus-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rrc_delivery_status_ext_ies_o = protocol_ext_empty_o;

using rrc_delivery_status_ext_ies_container = protocol_ext_container_empty_l;

// RRCDeliveryStatus ::= SEQUENCE
struct rrc_delivery_status_s {
  bool                                  ie_exts_present = false;
  uint16_t                              delivery_status = 0;
  uint16_t                              trigger_msg     = 0;
  rrc_delivery_status_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCDeliveryStatusRequest ::= ENUMERATED
struct rrc_delivery_status_request_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using rrc_delivery_status_request_e = enumerated<rrc_delivery_status_request_opts, true>;

// RRCReconfigurationCompleteIndicator ::= ENUMERATED
struct rrc_recfg_complete_ind_opts {
  enum options { true_value, /*...*/ fail, nulltype } value;

  const char* to_string() const;
};
using rrc_recfg_complete_ind_e = enumerated<rrc_recfg_complete_ind_opts, true, 1>;

// RSPPFlowBitRates-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rspp_flow_bit_rates_ext_ies_o = protocol_ext_empty_o;

using rspp_flow_bit_rates_ext_ies_container = protocol_ext_container_empty_l;

// RSPPFlowBitRates ::= SEQUENCE
struct rspp_flow_bit_rates_s {
  bool                                  ie_exts_present          = false;
  uint64_t                              guaranteed_flow_bit_rate = 0;
  uint64_t                              max_flow_bit_rate        = 0;
  rspp_flow_bit_rates_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Range ::= ENUMERATED
struct range_opts {
  enum options { m50, m80, m180, m200, m350, m400, m500, m700, m1000, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using range_e = enumerated<range_opts, true>;

// RSPPQoSFlowItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rspp_qos_flow_item_ext_ies_o = protocol_ext_empty_o;

using rspp_qos_flow_item_ext_ies_container = protocol_ext_container_empty_l;

// RSPPQoSFlowItem ::= SEQUENCE
struct rspp_qos_flow_item_s {
  bool                                 rspp_flow_bit_rates_present = false;
  bool                                 range_present               = false;
  bool                                 ie_exts_present             = false;
  uint16_t                             pqi                         = 0;
  rspp_flow_bit_rates_s                rspp_flow_bit_rates;
  range_e                              range;
  rspp_qos_flow_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RSPPQoSFlowList ::= SEQUENCE (SIZE (1..2048)) OF RSPPQoSFlowItem
using rspp_qos_flow_list_l = dyn_array<rspp_qos_flow_item_s>;

// RSPP-transport-QoS-parameters-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using rspp_transport_qos_params_ext_ies_o = protocol_ext_empty_o;

using rspp_transport_qos_params_ext_ies_container = protocol_ext_container_empty_l;

// RSPP-transport-QoS-parameters ::= SEQUENCE
struct rspp_transport_qos_params_s {
  bool                                        rspp_link_aggr_bit_rates_present = false;
  bool                                        ie_exts_present                  = false;
  rspp_qos_flow_list_l                        rspp_qos_flow_list;
  uint64_t                                    rspp_link_aggr_bit_rates = 0;
  rspp_transport_qos_params_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Re-routingEnableIndicator ::= ENUMERATED
struct re_routing_enable_ind_opts {
  enum options { true_value, false_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using re_routing_enable_ind_e = enumerated<re_routing_enable_ind_opts, true>;

// SSBs-forPaging-List ::= SEQUENCE (SIZE (1..64)) OF INTEGER (0..63)
using ss_bs_for_paging_list_l = dyn_array<uint8_t>;

// Recommended-SSBs-for-Paging-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using recommended_ss_bs_for_paging_list_item_ext_ies_o = protocol_ext_empty_o;

using recommended_ss_bs_for_paging_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Recommended-SSBs-for-Paging-List-Item ::= SEQUENCE
struct recommended_ss_bs_for_paging_list_item_s {
  bool                                                     ie_exts_present = false;
  nr_cgi_s                                                 nr_cgi;
  ss_bs_for_paging_list_l                                  ss_bs_for_paging_list;
  recommended_ss_bs_for_paging_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Recommended-SSBs-for-Paging-List ::= SEQUENCE (SIZE (1..512)) OF Recommended-SSBs-for-Paging-List-Item
using recommended_ss_bs_for_paging_list_l = dyn_array<recommended_ss_bs_for_paging_list_item_s>;

// RedCapIndication ::= ENUMERATED
struct red_cap_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using red_cap_ind_e = enumerated<red_cap_ind_opts, true>;

// RegistrationRequest ::= ENUMERATED
struct regist_request_opts {
  enum options { start, stop, add, /*...*/ nulltype } value;

  const char* to_string() const;
};
using regist_request_e = enumerated<regist_request_opts, true>;

// ReportingPeriodicity ::= ENUMERATED
struct report_periodicity_opts {
  enum options { ms500, ms1000, ms2000, ms5000, ms10000, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using report_periodicity_e = enumerated<report_periodicity_opts, true>;

// ReportingRequestType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using report_request_type_ext_ies_o = protocol_ext_empty_o;

using report_request_type_ext_ies_container = protocol_ext_container_empty_l;

// ReportingRequestType ::= SEQUENCE
struct report_request_type_s {
  bool                                  report_periodicity_value_present = false;
  bool                                  ie_exts_present                  = false;
  event_type_e                          event_type;
  uint16_t                              report_periodicity_value = 0;
  report_request_type_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestType ::= ENUMERATED
struct request_type_opts {
  enum options { offer, execution, /*...*/ nulltype } value;

  const char* to_string() const;
};
using request_type_e = enumerated<request_type_opts, true>;

// SRSResourceSetItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct srs_res_set_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct srs_res_set_item_ext_ies_container {
  bool                           srs_spatial_relation_per_srs_res_present = false;
  spatial_relation_per_srs_res_s srs_spatial_relation_per_srs_res;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResourceSetItem ::= SEQUENCE
struct srs_res_set_item_s {
  bool                               num_srs_resperset_present     = false;
  bool                               spatial_relation_info_present = false;
  bool                               pathloss_ref_info_present     = false;
  bool                               ie_exts_present               = false;
  uint8_t                            num_srs_resperset             = 1;
  periodicity_list_l                 periodicity_list;
  spatial_relation_info_s            spatial_relation_info;
  pathloss_ref_info_s                pathloss_ref_info;
  srs_res_set_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResourceSetList ::= SEQUENCE (SIZE (1..16)) OF SRSResourceSetItem
using srs_res_set_list_l = dyn_array<srs_res_set_item_s>;

// ResourceMapping-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_map_ext_ies_o = protocol_ext_empty_o;

using res_map_ext_ies_container = protocol_ext_container_empty_l;

// ResourceMapping ::= SEQUENCE
struct res_map_s {
  struct nrof_sumbols_opts {
    enum options { n1, n2, n4, n8, n12, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using nrof_sumbols_e_ = enumerated<nrof_sumbols_opts>;

  // member variables
  bool                      ie_exts_present = false;
  uint8_t                   start_position  = 0;
  nrof_sumbols_e_           nrof_sumbols;
  res_map_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ValidityAreaSpecificSRSInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using validity_area_specific_srs_info_ext_ies_o = protocol_ext_empty_o;

using validity_area_specific_srs_info_ext_ies_container = protocol_ext_container_empty_l;

// ValidityAreaSpecificSRSInformation ::= SEQUENCE
struct validity_area_specific_srs_info_s {
  bool                                              tx_comb_pos_present       = false;
  bool                                              res_map_present           = false;
  bool                                              freq_domain_shift_present = false;
  bool                                              c_srs_present             = false;
  bool                                              res_type_pos_present      = false;
  bool                                              seq_id_pos_present        = false;
  bool                                              ie_exts_present           = false;
  tx_comb_pos_c                                     tx_comb_pos;
  res_map_s                                         res_map;
  uint16_t                                          freq_domain_shift = 0;
  uint8_t                                           c_srs             = 0;
  res_type_pos_c                                    res_type_pos;
  uint32_t                                          seq_id_pos = 0;
  validity_area_specific_srs_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ValidityAreaSpecificSRSInformationExtended-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using validity_area_specific_srs_info_extended_ext_ies_o = protocol_ext_empty_o;

using validity_area_specific_srs_info_extended_ext_ies_container = protocol_ext_container_empty_l;

// ValidityAreaSpecificSRSInformationExtended ::= SEQUENCE
struct validity_area_specific_srs_info_extended_s {
  bool                                                       ie_exts_present = false;
  pos_srs_res_list_l                                         pos_srs_res_list;
  pos_srs_res_set_list_l                                     pos_srs_res_set_list;
  validity_area_specific_srs_info_extended_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedSRSTransmissionCharacteristics-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct requested_srs_tx_characteristics_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct requested_srs_tx_characteristics_ext_ies_container {
  bool                                       srs_freq_present                                 = false;
  bool                                       bw_aggregation_request_ind_present               = false;
  bool                                       pos_validity_area_cell_list_present              = false;
  bool                                       validity_area_specific_srs_info_present          = false;
  bool                                       validity_area_specific_srs_info_extended_present = false;
  uint32_t                                   srs_freq;
  bw_aggregation_request_ind_e               bw_aggregation_request_ind;
  pos_validity_area_cell_list_l              pos_validity_area_cell_list;
  validity_area_specific_srs_info_s          validity_area_specific_srs_info;
  validity_area_specific_srs_info_extended_s validity_area_specific_srs_info_extended;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedSRSTransmissionCharacteristics ::= SEQUENCE
struct requested_srs_tx_characteristics_s {
  struct res_type_opts {
    enum options { periodic, semi_persistent, aperiodic, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using res_type_e_ = enumerated<res_type_opts, true>;

  // member variables
  bool                                               nof_txs_present  = false;
  bool                                               ssb_info_present = false;
  bool                                               ie_exts_present  = false;
  uint16_t                                           nof_txs          = 0;
  res_type_e_                                        res_type;
  bw_srs_c                                           bw_srs;
  srs_res_set_list_l                                 srs_res_set_list;
  ssb_info_s                                         ssb_info;
  requested_srs_tx_characteristics_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedSRSPreconfigurationCharacteristics-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using requested_srs_precfg_characteristics_item_ext_ies_o = protocol_ext_empty_o;

using requested_srs_precfg_characteristics_item_ext_ies_container = protocol_ext_container_empty_l;

// RequestedSRSPreconfigurationCharacteristics-Item ::= SEQUENCE
struct requested_srs_precfg_characteristics_item_s {
  bool                                                        ie_exts_present = false;
  requested_srs_tx_characteristics_s                          requested_srs_tx_characteristics;
  requested_srs_precfg_characteristics_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedSRSPreconfigurationCharacteristics-List ::= SEQUENCE (SIZE (1..16)) OF
// RequestedSRSPreconfigurationCharacteristics-Item
using requested_srs_precfg_characteristics_list_l = dyn_array<requested_srs_precfg_characteristics_item_s>;

// ResourceCoordinationEUTRACellInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct res_coordination_eutra_cell_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct res_coordination_eutra_cell_info_ext_ies_container {
  bool               ignore_prach_cfg_present = false;
  ignore_prach_cfg_e ignore_prach_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceCoordinationEUTRACellInfo ::= SEQUENCE
struct res_coordination_eutra_cell_info_s {
  bool                                               ie_exts_present = false;
  eutra_coex_mode_info_c                             eutra_mode_info;
  eutra_prach_cfg_s                                  eutra_prach_cfg;
  res_coordination_eutra_cell_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceCoordinationTransferInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using res_coordination_transfer_info_ext_ies_o = protocol_ext_empty_o;

using res_coordination_transfer_info_ext_ies_container = protocol_ext_container_empty_l;

// ResourceCoordinationTransferInformation ::= SEQUENCE
struct res_coordination_transfer_info_s {
  bool                                             res_coordination_eutra_cell_info_present = false;
  bool                                             ie_exts_present                          = false;
  fixed_bitstring<28, false, true>                 m_enb_cell_id;
  res_coordination_eutra_cell_info_s               res_coordination_eutra_cell_info;
  res_coordination_transfer_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResponseTime-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using resp_time_ext_ies_o = protocol_ext_empty_o;

using resp_time_ext_ies_container = protocol_ext_container_empty_l;

// ResponseTime ::= SEQUENCE
struct resp_time_s {
  struct time_unit_opts {
    enum options { second, ten_seconds, ten_milliseconds, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using time_unit_e_ = enumerated<time_unit_opts, true>;

  // member variables
  bool                        ie_exts_present = false;
  uint16_t                    time            = 1;
  time_unit_e_                time_unit;
  resp_time_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// S-CPAC-Configuration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using s_cp_ac_cfg_ext_ies_o = protocol_ext_empty_o;

using s_cp_ac_cfg_ext_ies_container = protocol_ext_container_empty_l;

// S-CPAC-Configuration ::= SEQUENCE
struct s_cp_ac_cfg_s {
  bool                          complete_candidate_cfg_ind_present = false;
  bool                          ie_exts_present                    = false;
  unbounded_octstring<true>     ref_cfg_info;
  complete_candidate_cfg_ind_e  complete_candidate_cfg_ind;
  s_cp_ac_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SCGActivationRequest ::= ENUMERATED
struct scg_activation_request_opts {
  enum options { activ_scg, deactiv_scg, /*...*/ nulltype } value;

  const char* to_string() const;
};
using scg_activation_request_e = enumerated<scg_activation_request_opts, true>;

// SCGActivationStatus ::= ENUMERATED
struct scg_activation_status_opts {
  enum options { scg_activ, scg_deactiv, /*...*/ nulltype } value;

  const char* to_string() const;
};
using scg_activation_status_e = enumerated<scg_activation_status_opts, true>;

// SCGIndicator ::= ENUMERATED
struct scg_ind_opts {
  enum options { released, /*...*/ nulltype } value;

  const char* to_string() const;
};
using scg_ind_e = enumerated<scg_ind_opts, true>;

// SCell-FailedtoSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using scell_failedto_setup_item_ext_ies_o = protocol_ext_empty_o;

using scell_failedto_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// SCell-FailedtoSetup-Item ::= SEQUENCE
struct scell_failedto_setup_item_s {
  bool                                        cause_present   = false;
  bool                                        ie_exts_present = false;
  nr_cgi_s                                    scell_id;
  cause_c                                     cause;
  scell_failedto_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SCell-FailedtoSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using scell_failedto_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using scell_failedto_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// SCell-FailedtoSetupMod-Item ::= SEQUENCE
struct scell_failedto_setup_mod_item_s {
  bool                                            cause_present   = false;
  bool                                            ie_exts_present = false;
  nr_cgi_s                                        scell_id;
  cause_c                                         cause;
  scell_failedto_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SCell-ToBeRemoved-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using scell_to_be_remd_item_ext_ies_o = protocol_ext_empty_o;

using scell_to_be_remd_item_ext_ies_container = protocol_ext_container_empty_l;

// SCell-ToBeRemoved-Item ::= SEQUENCE
struct scell_to_be_remd_item_s {
  bool                                    ie_exts_present = false;
  nr_cgi_s                                scell_id;
  scell_to_be_remd_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SCell-ToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct scell_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct scell_to_be_setup_item_ext_ies_container {
  bool     serving_cell_mo_present = false;
  uint16_t serving_cell_mo;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SCell-ToBeSetup-Item ::= SEQUENCE
struct scell_to_be_setup_item_s {
  bool                                     scell_ul_cfg_present = false;
  bool                                     ie_exts_present      = false;
  nr_cgi_s                                 scell_id;
  uint8_t                                  scell_idx = 1;
  cell_ul_cfg_e                            scell_ul_cfg;
  scell_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SCell-ToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct scell_to_be_setup_mod_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct scell_to_be_setup_mod_item_ext_ies_container {
  bool     serving_cell_mo_present = false;
  uint16_t serving_cell_mo;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SCell-ToBeSetupMod-Item ::= SEQUENCE
struct scell_to_be_setup_mod_item_s {
  bool                                         scell_ul_cfg_present = false;
  bool                                         ie_exts_present      = false;
  nr_cgi_s                                     scell_id;
  uint8_t                                      scell_idx = 1;
  cell_ul_cfg_e                                scell_ul_cfg;
  scell_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SDT-Termination-Request ::= ENUMERATED
struct sdt_termination_request_opts {
  enum options { radio_link_problem, normal, /*...*/ sdt_volume_thres_crossed, nulltype } value;

  const char* to_string() const;
};
using sdt_termination_request_e = enumerated<sdt_termination_request_opts, true, 1>;

// SDTBearerType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using sdt_bearer_type_ext_ies_o = protocol_ies_empty_o;

// SDTBearerType ::= CHOICE
struct sdt_bearer_type_c {
  struct types_opts {
    enum options { srb, drb, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  sdt_bearer_type_c() = default;
  sdt_bearer_type_c(const sdt_bearer_type_c& other);
  sdt_bearer_type_c& operator=(const sdt_bearer_type_c& other);
  ~sdt_bearer_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& srb()
  {
    assert_choice_type(types::srb, type_, "SDTBearerType");
    return c.get<uint8_t>();
  }
  uint8_t& drb()
  {
    assert_choice_type(types::drb, type_, "SDTBearerType");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<sdt_bearer_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SDTBearerType");
    return c.get<protocol_ie_single_container_s<sdt_bearer_type_ext_ies_o>>();
  }
  const uint8_t& srb() const
  {
    assert_choice_type(types::srb, type_, "SDTBearerType");
    return c.get<uint8_t>();
  }
  const uint8_t& drb() const
  {
    assert_choice_type(types::drb, type_, "SDTBearerType");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<sdt_bearer_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SDTBearerType");
    return c.get<protocol_ie_single_container_s<sdt_bearer_type_ext_ies_o>>();
  }
  uint8_t&                                                   set_srb();
  uint8_t&                                                   set_drb();
  protocol_ie_single_container_s<sdt_bearer_type_ext_ies_o>& set_choice_ext();

private:
  types                                                                      type_;
  choice_buffer_t<protocol_ie_single_container_s<sdt_bearer_type_ext_ies_o>> c;

  void destroy_();
};

// SDTBearerConfig-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sdt_bearer_cfg_list_item_ext_ies_o = protocol_ext_empty_o;

using sdt_bearer_cfg_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SDTBearerConfig-List-Item ::= SEQUENCE
struct sdt_bearer_cfg_list_item_s {
  bool                                       ie_exts_present = false;
  sdt_bearer_type_c                          sdt_bearer_type;
  unbounded_octstring<true>                  sdt_rlc_bearer_cfg;
  sdt_bearer_cfg_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SDTBearerConfig-List ::= SEQUENCE (SIZE (1..72)) OF SDTBearerConfig-List-Item
using sdt_bearer_cfg_list_l = dyn_array<sdt_bearer_cfg_list_item_s>;

// SDTBearerConfigurationInfo-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sdt_bearer_cfg_info_ext_ies_o = protocol_ext_empty_o;

using sdt_bearer_cfg_info_ext_ies_container = protocol_ext_container_empty_l;

// SDTBearerConfigurationInfo ::= SEQUENCE
struct sdt_bearer_cfg_info_s {
  bool                                  ie_exts_present = false;
  sdt_bearer_cfg_list_l                 sdt_bearer_cfg_list;
  sdt_bearer_cfg_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SDTBearerConfigurationQueryIndication ::= ENUMERATED
struct sdt_bearer_cfg_query_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sdt_bearer_cfg_query_ind_e = enumerated<sdt_bearer_cfg_query_ind_opts, true>;

// SDTInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sdt_info_ext_ies_o = protocol_ext_empty_o;

using sdt_info_ext_ies_container = protocol_ext_container_empty_l;

// SDTInformation ::= SEQUENCE
struct sdt_info_s {
  struct sdt_ind_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using sdt_ind_e_ = enumerated<sdt_ind_opts, true>;
  struct sdt_assistant_info_opts {
    enum options { singlepacket, multiplepackets, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using sdt_assistant_info_e_ = enumerated<sdt_assistant_info_opts, true>;

  // member variables
  bool                       sdt_assistant_info_present = false;
  bool                       ie_exts_present            = false;
  sdt_ind_e_                 sdt_ind;
  sdt_assistant_info_e_      sdt_assistant_info;
  sdt_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SItype-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sitype_item_ext_ies_o = protocol_ext_empty_o;

using sitype_item_ext_ies_container = protocol_ext_container_empty_l;

// SItype-Item ::= SEQUENCE
struct sitype_item_s {
  bool                          ie_exts_present = false;
  uint8_t                       sitype          = 1;
  sitype_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SItype-List ::= SEQUENCE (SIZE (1..32)) OF SItype-Item
using sitype_list_l = dyn_array<sitype_item_s>;

// SLDRBInformation ::= SEQUENCE
struct sl_drb_info_s {
  pc5_qos_params_s              sl_drb_qos;
  flows_mapped_to_sl_drb_list_l flows_mapped_to_sl_drb_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-FailedToBeModified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_failed_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_failed_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-FailedToBeModified-Item ::= SEQUENCE
struct sl_drbs_failed_to_be_modified_item_s {
  bool                                                 cause_present   = false;
  bool                                                 ie_exts_present = false;
  uint16_t                                             sl_drb_id       = 1;
  cause_c                                              cause;
  sl_drbs_failed_to_be_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-FailedToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-FailedToBeSetup-Item ::= SEQUENCE
struct sl_drbs_failed_to_be_setup_item_s {
  bool                                              cause_present   = false;
  bool                                              ie_exts_present = false;
  uint16_t                                          sl_drb_id       = 1;
  cause_c                                           cause;
  sl_drbs_failed_to_be_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-FailedToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_failed_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_failed_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-FailedToBeSetupMod-Item ::= SEQUENCE
struct sl_drbs_failed_to_be_setup_mod_item_s {
  bool                                                  cause_present   = false;
  bool                                                  ie_exts_present = false;
  uint16_t                                              sl_drb_id       = 1;
  cause_c                                               cause;
  sl_drbs_failed_to_be_setup_mod_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-Modified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_modified_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-Modified-Item ::= SEQUENCE
struct sl_drbs_modified_item_s {
  bool                                    ie_exts_present = false;
  uint16_t                                sl_drb_id       = 1;
  sl_drbs_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-ModifiedConf-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_modified_conf_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_modified_conf_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-ModifiedConf-Item ::= SEQUENCE
struct sl_drbs_modified_conf_item_s {
  bool                                         ie_exts_present = false;
  uint16_t                                     sl_drb_id       = 1;
  sl_drbs_modified_conf_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-Required-ToBeModified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_required_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_required_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-Required-ToBeModified-Item ::= SEQUENCE
struct sl_drbs_required_to_be_modified_item_s {
  bool                                                   ie_exts_present = false;
  uint16_t                                               sl_drb_id       = 1;
  sl_drbs_required_to_be_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-Required-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_required_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_required_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-Required-ToBeReleased-Item ::= SEQUENCE
struct sl_drbs_required_to_be_released_item_s {
  bool                                                   ie_exts_present = false;
  uint16_t                                               sl_drb_id       = 1;
  sl_drbs_required_to_be_released_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-Setup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_setup_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-Setup-Item ::= SEQUENCE
struct sl_drbs_setup_item_s {
  bool                                 ie_exts_present = false;
  uint16_t                             sl_drb_id       = 1;
  sl_drbs_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-SetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-SetupMod-Item ::= SEQUENCE
struct sl_drbs_setup_mod_item_s {
  bool                                     ie_exts_present = false;
  uint16_t                                 sl_drb_id       = 1;
  sl_drbs_setup_mod_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-ToBeModified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct sl_drbs_to_be_modified_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sl_drbs_to_be_modified_item_ext_ies_container {
  bool       dupl_ind_present = false;
  dupl_ind_e dupl_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-ToBeModified-Item ::= SEQUENCE
struct sl_drbs_to_be_modified_item_s {
  bool                                          sl_drb_info_present = false;
  bool                                          rlc_mode_present    = false;
  bool                                          ie_exts_present     = false;
  uint16_t                                      sl_drb_id           = 1;
  sl_drb_info_s                                 sl_drb_info;
  rlc_mode_e                                    rlc_mode;
  sl_drbs_to_be_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_drbs_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using sl_drbs_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRBs-ToBeReleased-Item ::= SEQUENCE
struct sl_drbs_to_be_released_item_s {
  bool                                          ie_exts_present = false;
  uint16_t                                      sl_drb_id       = 1;
  sl_drbs_to_be_released_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-ToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct sl_drbs_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sl_drbs_to_be_setup_item_ext_ies_container {
  bool       dupl_ind_present = false;
  dupl_ind_e dupl_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-ToBeSetup-Item ::= SEQUENCE
struct sl_drbs_to_be_setup_item_s {
  bool                                       ie_exts_present = false;
  uint16_t                                   sl_drb_id       = 1;
  sl_drb_info_s                              sl_drb_info;
  rlc_mode_e                                 rlc_mode;
  sl_drbs_to_be_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-ToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct sl_drbs_to_be_setup_mod_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sl_drbs_to_be_setup_mod_item_ext_ies_container {
  bool       dupl_ind_present = false;
  dupl_ind_e dupl_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRBs-ToBeSetupMod-Item ::= SEQUENCE
struct sl_drbs_to_be_setup_mod_item_s {
  bool                                           rlc_mode_present = false;
  bool                                           ie_exts_present  = false;
  uint16_t                                       sl_drb_id        = 1;
  sl_drb_info_s                                  sl_drb_info;
  rlc_mode_e                                     rlc_mode;
  sl_drbs_to_be_setup_mod_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRXConfigurationIndicator ::= ENUMERATED
struct sldrx_cfg_ind_opts {
  enum options { release, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sldrx_cfg_ind_e = enumerated<sldrx_cfg_ind_opts, true>;

// SLDRXCycleLength ::= ENUMERATED
struct sldrx_cycle_len_opts {
  enum options {
    ms10,
    ms20,
    ms32,
    ms40,
    ms60,
    ms64,
    ms70,
    ms80,
    ms128,
    ms160,
    ms256,
    ms320,
    ms512,
    ms640,
    ms1024,
    ms1280,
    ms2048,
    ms2560,
    ms5120,
    ms10240,
    // ...
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using sldrx_cycle_len_e = enumerated<sldrx_cycle_len_opts, true>;

// SLDRXInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using sldrx_info_ext_ies_o = protocol_ies_empty_o;

// SLDRXInformation ::= CHOICE
struct sldrx_info_c {
  struct types_opts {
    enum options { sldrx_cycle, nos_ldrx, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  sldrx_info_c() = default;
  sldrx_info_c(const sldrx_info_c& other);
  sldrx_info_c& operator=(const sldrx_info_c& other);
  ~sldrx_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  sldrx_cycle_len_e& sldrx_cycle()
  {
    assert_choice_type(types::sldrx_cycle, type_, "SLDRXInformation");
    return c.get<sldrx_cycle_len_e>();
  }
  sldrx_cfg_ind_e& nos_ldrx()
  {
    assert_choice_type(types::nos_ldrx, type_, "SLDRXInformation");
    return c.get<sldrx_cfg_ind_e>();
  }
  protocol_ie_single_container_s<sldrx_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SLDRXInformation");
    return c.get<protocol_ie_single_container_s<sldrx_info_ext_ies_o>>();
  }
  const sldrx_cycle_len_e& sldrx_cycle() const
  {
    assert_choice_type(types::sldrx_cycle, type_, "SLDRXInformation");
    return c.get<sldrx_cycle_len_e>();
  }
  const sldrx_cfg_ind_e& nos_ldrx() const
  {
    assert_choice_type(types::nos_ldrx, type_, "SLDRXInformation");
    return c.get<sldrx_cfg_ind_e>();
  }
  const protocol_ie_single_container_s<sldrx_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SLDRXInformation");
    return c.get<protocol_ie_single_container_s<sldrx_info_ext_ies_o>>();
  }
  sldrx_cycle_len_e&                                    set_sldrx_cycle();
  sldrx_cfg_ind_e&                                      set_nos_ldrx();
  protocol_ie_single_container_s<sldrx_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                 type_;
  choice_buffer_t<protocol_ie_single_container_s<sldrx_info_ext_ies_o>> c;

  void destroy_();
};

// SLDRXCycleItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sldrx_cycle_item_ext_ies_o = protocol_ext_empty_o;

using sldrx_cycle_item_ext_ies_container = protocol_ext_container_empty_l;

// SLDRXCycleItem ::= SEQUENCE
struct sldrx_cycle_item_s {
  bool                               ie_exts_present = false;
  fixed_bitstring<24, false, true>   rx_ue_id;
  sldrx_info_c                       sldrx_info;
  sldrx_cycle_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SLDRXCycleList ::= SEQUENCE (SIZE (1..32)) OF SLDRXCycleItem
using sldrx_cycle_list_l = dyn_array<sldrx_cycle_item_s>;

// SLPositioning-Ranging-Authorized ::= ENUMERATED
struct sl_positioning_ranging_authorized_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sl_positioning_ranging_authorized_e = enumerated<sl_positioning_ranging_authorized_opts, true>;

// SLPositioning-Ranging-Service-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sl_positioning_ranging_service_info_ext_ies_o = protocol_ext_empty_o;

using sl_positioning_ranging_service_info_ext_ies_container = protocol_ext_container_empty_l;

// SLPositioning-Ranging-Service-Info ::= SEQUENCE
struct sl_positioning_ranging_service_info_s {
  bool                                                  rspp_transport_qos_params_present = false;
  bool                                                  ie_exts_present                   = false;
  sl_positioning_ranging_authorized_e                   sl_positioning_ranging_authorized;
  rspp_transport_qos_params_s                           rspp_transport_qos_params;
  sl_positioning_ranging_service_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-FailedToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srbs_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using srbs_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// SRBs-FailedToBeSetup-Item ::= SEQUENCE
struct srbs_failed_to_be_setup_item_s {
  bool                                           cause_present   = false;
  bool                                           ie_exts_present = false;
  uint8_t                                        srb_id          = 0;
  cause_c                                        cause;
  srbs_failed_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-FailedToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srbs_failed_to_be_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using srbs_failed_to_be_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// SRBs-FailedToBeSetupMod-Item ::= SEQUENCE
struct srbs_failed_to_be_setup_mod_item_s {
  bool                                               cause_present   = false;
  bool                                               ie_exts_present = false;
  uint8_t                                            srb_id          = 0;
  cause_c                                            cause;
  srbs_failed_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-Modified-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srbs_modified_item_ext_ies_o = protocol_ext_empty_o;

using srbs_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// SRBs-Modified-Item ::= SEQUENCE
struct srbs_modified_item_s {
  bool                                 ie_exts_present = false;
  uint8_t                              srb_id          = 0;
  uint8_t                              lcid            = 1;
  srbs_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-Required-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srbs_required_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using srbs_required_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// SRBs-Required-ToBeReleased-Item ::= SEQUENCE
struct srbs_required_to_be_released_item_s {
  bool                                                ie_exts_present = false;
  uint8_t                                             srb_id          = 0;
  srbs_required_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-Setup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srbs_setup_item_ext_ies_o = protocol_ext_empty_o;

using srbs_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// SRBs-Setup-Item ::= SEQUENCE
struct srbs_setup_item_s {
  bool                              ie_exts_present = false;
  uint8_t                           srb_id          = 0;
  uint8_t                           lcid            = 1;
  srbs_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-SetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srbs_setup_mod_item_ext_ies_o = protocol_ext_empty_o;

using srbs_setup_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// SRBs-SetupMod-Item ::= SEQUENCE
struct srbs_setup_mod_item_s {
  bool                                  ie_exts_present = false;
  uint8_t                               srb_id          = 0;
  uint8_t                               lcid            = 1;
  srbs_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-ToBeReleased-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srbs_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using srbs_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// SRBs-ToBeReleased-Item ::= SEQUENCE
struct srbs_to_be_released_item_s {
  bool                                       ie_exts_present = false;
  uint8_t                                    srb_id          = 0;
  srbs_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-ToBeSetup-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct srbs_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct srbs_to_be_setup_item_ext_ies_container {
  bool                      add_dupl_ind_present       = false;
  bool                      sdt_rlc_bearer_cfg_present = false;
  bool                      srb_map_info_present       = false;
  add_dupl_ind_e            add_dupl_ind;
  unbounded_octstring<true> sdt_rlc_bearer_cfg;
  uint8_t                   srb_map_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-ToBeSetup-Item ::= SEQUENCE
struct srbs_to_be_setup_item_s {
  bool                                    dupl_ind_present = false;
  bool                                    ie_exts_present  = false;
  uint8_t                                 srb_id           = 0;
  dupl_ind_e                              dupl_ind;
  srbs_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-ToBeSetupMod-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct srbs_to_be_setup_mod_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct srbs_to_be_setup_mod_item_ext_ies_container {
  bool               add_dupl_ind_present     = false;
  bool               srb_map_info_present     = false;
  bool               cg_sd_tind_setup_present = false;
  add_dupl_ind_e     add_dupl_ind;
  uint8_t            srb_map_info;
  cg_sd_tind_setup_e cg_sd_tind_setup;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRBs-ToBeSetupMod-Item ::= SEQUENCE
struct srbs_to_be_setup_mod_item_s {
  bool                                        dupl_ind_present = false;
  bool                                        ie_exts_present  = false;
  uint8_t                                     srb_id           = 0;
  dupl_ind_e                                  dupl_ind;
  srbs_to_be_setup_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UplinkChannelBW-PerSCS-List ::= SEQUENCE (SIZE (1..5)) OF SCS-SpecificCarrier
using ul_ch_bw_per_scs_list_l = dyn_array<scs_specific_carrier_s>;

// SRSCarrier-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srs_carrier_list_item_ext_ies_o = protocol_ext_empty_o;

using srs_carrier_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SRSCarrier-List-Item ::= SEQUENCE
struct srs_carrier_list_item_s {
  bool                                    pci_present     = false;
  bool                                    ie_exts_present = false;
  uint32_t                                point_a         = 0;
  ul_ch_bw_per_scs_list_l                 ul_ch_bw_per_scs_list;
  active_ul_bwp_s                         active_ul_bwp;
  uint16_t                                pci = 0;
  srs_carrier_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSCarrier-List ::= SEQUENCE (SIZE (1..32)) OF SRSCarrier-List-Item
using srs_carrier_list_l = dyn_array<srs_carrier_list_item_s>;

// SRSConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct srs_configuration_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct srs_configuration_ext_ies_container {
  bool                        aggr_pos_srs_res_set_list_present = false;
  aggr_pos_srs_res_set_list_l aggr_pos_srs_res_set_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSConfiguration ::= SEQUENCE
struct srs_configuration_s {
  bool                                ie_exts_present = false;
  srs_carrier_list_l                  srs_carrier_list;
  srs_configuration_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSPosRRCInactiveQueryIndication ::= ENUMERATED
struct srs_pos_rrc_inactive_query_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using srs_pos_rrc_inactive_query_ind_e = enumerated<srs_pos_rrc_inactive_query_ind_opts, true>;

// SRSPreconfiguration-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srs_precfg_item_ext_ies_o = protocol_ext_empty_o;

using srs_precfg_item_ext_ies_container = protocol_ext_container_empty_l;

// SRSPreconfiguration-Item ::= SEQUENCE
struct srs_precfg_item_s {
  bool                              ie_exts_present = false;
  unbounded_octstring<true>         srs_pos_rrc_inactive_validity_area_cfg;
  pos_validity_area_cell_list_l     pos_validity_area_cell_list;
  srs_precfg_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSPreconfiguration-List ::= SEQUENCE (SIZE (1..16)) OF SRSPreconfiguration-Item
using srs_precfg_list_l = dyn_array<srs_precfg_item_s>;

// SRSReservationType ::= ENUMERATED
struct srs_reserv_type_opts {
  enum options { reserve, release, /*...*/ nulltype } value;

  const char* to_string() const;
};
using srs_reserv_type_e = enumerated<srs_reserv_type_opts, true>;

// SRSResourceTrigger-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using srs_res_trigger_ext_ies_o = protocol_ext_empty_o;

using srs_res_trigger_ext_ies_container = protocol_ext_container_empty_l;

// SRSResourceTrigger ::= SEQUENCE
struct srs_res_trigger_s {
  bool                              ie_exts_present = false;
  aperiodic_srs_res_trigger_list_l  aperiodic_srs_res_trigger_list;
  srs_res_trigger_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SULAccessIndication ::= ENUMERATED
struct sul_access_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sul_access_ind_e = enumerated<sul_access_ind_opts, true>;

// Search-window-information-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using search_win_info_ext_ies_o = protocol_ext_empty_o;

using search_win_info_ext_ies_container = protocol_ext_container_empty_l;

// Search-window-information ::= SEQUENCE
struct search_win_info_s {
  bool                              ie_exts_present            = false;
  int16_t                           expected_propagation_delay = -3841;
  uint16_t                          delay_uncertainty          = 1;
  search_win_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Served-Cells-To-Add-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using served_cells_to_add_item_ext_ies_o = protocol_ext_empty_o;

using served_cells_to_add_item_ext_ies_container = protocol_ext_container_empty_l;

// Served-Cells-To-Add-Item ::= SEQUENCE
struct served_cells_to_add_item_s {
  bool                                       gnb_du_sys_info_present = false;
  bool                                       ie_exts_present         = false;
  served_cell_info_s                         served_cell_info;
  gnb_du_sys_info_s                          gnb_du_sys_info;
  served_cells_to_add_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Served-Cells-To-Delete-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using served_cells_to_delete_item_ext_ies_o = protocol_ext_empty_o;

using served_cells_to_delete_item_ext_ies_container = protocol_ext_container_empty_l;

// Served-Cells-To-Delete-Item ::= SEQUENCE
struct served_cells_to_delete_item_s {
  bool                                          ie_exts_present = false;
  nr_cgi_s                                      old_nr_cgi;
  served_cells_to_delete_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Served-Cells-To-Modify-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using served_cells_to_modify_item_ext_ies_o = protocol_ext_empty_o;

using served_cells_to_modify_item_ext_ies_container = protocol_ext_container_empty_l;

// Served-Cells-To-Modify-Item ::= SEQUENCE
struct served_cells_to_modify_item_s {
  bool                                          gnb_du_sys_info_present = false;
  bool                                          ie_exts_present         = false;
  nr_cgi_s                                      old_nr_cgi;
  served_cell_info_s                            served_cell_info;
  gnb_du_sys_info_s                             gnb_du_sys_info;
  served_cells_to_modify_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Serving-Cells-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using serving_cells_list_item_ext_ies_o = protocol_ext_empty_o;

using serving_cells_list_item_ext_ies_container = protocol_ext_container_empty_l;

// Serving-Cells-List-Item ::= SEQUENCE
struct serving_cells_list_item_s {
  bool                                      iab_mt_cell_na_res_cfg_mode_info_present = false;
  bool                                      ie_exts_present                          = false;
  nr_cgi_s                                  nr_cgi;
  iab_mt_cell_na_res_cfg_mode_info_c        iab_mt_cell_na_res_cfg_mode_info;
  serving_cells_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Serving-Cells-List ::= SEQUENCE (SIZE (1..32)) OF Serving-Cells-List-Item
using serving_cells_list_l = dyn_array<serving_cells_list_item_s>;

// ServingCellMO-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using serving_cell_mo_list_item_ext_ies_o = protocol_ext_empty_o;

using serving_cell_mo_list_item_ext_ies_container = protocol_ext_container_empty_l;

// ServingCellMO-List-Item ::= SEQUENCE
struct serving_cell_mo_list_item_s {
  bool                                        ie_exts_present = false;
  uint16_t                                    serving_cell_mo = 1;
  uint32_t                                    ssb_freq        = 0;
  serving_cell_mo_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServingCellMO-encoded-in-CGC-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct serving_cell_mo_encoded_in_cgc_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct serving_cell_mo_encoded_in_cgc_item_ext_ies_container {
  bool    bwp_id_present = false;
  uint8_t bwp_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServingCellMO-encoded-in-CGC-Item ::= SEQUENCE
struct serving_cell_mo_encoded_in_cgc_item_s {
  bool                                                  ie_exts_present = false;
  uint16_t                                              serving_cell_mo = 1;
  serving_cell_mo_encoded_in_cgc_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServingCellMO-encoded-in-CGC-List ::= SEQUENCE (SIZE (1..8)) OF ServingCellMO-encoded-in-CGC-Item
using serving_cell_mo_encoded_in_cgc_list_l = dyn_array<serving_cell_mo_encoded_in_cgc_item_s>;

// SidelinkRelayConfiguration-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using sidelink_relay_cfg_ext_ies_o = protocol_ext_empty_o;

using sidelink_relay_cfg_ext_ies_container = protocol_ext_container_empty_l;

// SidelinkRelayConfiguration ::= SEQUENCE
struct sidelink_relay_cfg_s {
  bool                                 ie_exts_present               = false;
  uint64_t                             gnb_du_ue_f1ap_id_of_relay_ue = 0;
  uint16_t                             remote_ue_local_id            = 0;
  unbounded_octstring<true>            sidelink_cfg_container;
  sidelink_relay_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialDirectionInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using spatial_direction_info_ext_ies_o = protocol_ext_empty_o;

using spatial_direction_info_ext_ies_container = protocol_ext_container_empty_l;

// SpatialDirectionInformation ::= SEQUENCE
struct spatial_direction_info_s {
  bool                                     ie_exts_present = false;
  nr_prs_beam_info_s                       nr_prs_beam_info;
  spatial_direction_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SuccessfulHOReportInformation-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using successful_ho_report_info_item_ext_ies_o = protocol_ext_empty_o;

using successful_ho_report_info_item_ext_ies_container = protocol_ext_container_empty_l;

// SuccessfulHOReportInformation-Item ::= SEQUENCE
struct successful_ho_report_info_item_s {
  bool                                             ie_exts_present = false;
  unbounded_octstring<true>                        successful_ho_report_container;
  successful_ho_report_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SuccessfulHOReportInformationList ::= SEQUENCE (SIZE (1..64)) OF SuccessfulHOReportInformation-Item
using successful_ho_report_info_list_l = dyn_array<successful_ho_report_info_item_s>;

// SuccessfulPSCellChangeReportInformation-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using successful_pscell_change_report_info_item_ext_ies_o = protocol_ext_empty_o;

using successful_pscell_change_report_info_item_ext_ies_container = protocol_ext_container_empty_l;

// SuccessfulPSCellChangeReportInformation-Item ::= SEQUENCE
struct successful_pscell_change_report_info_item_s {
  bool                                                        ie_exts_present = false;
  unbounded_octstring<true>                                   successful_pscell_change_report_container;
  successful_pscell_change_report_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SuccessfulPSCellChangeReportInformationList ::= SEQUENCE (SIZE (1..64)) OF
// SuccessfulPSCellChangeReportInformation-Item
using successful_pscell_change_report_info_list_l = dyn_array<successful_pscell_change_report_info_item_s>;

// SupportedUETypeList-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using supported_ue_type_list_item_ext_ies_o = protocol_ext_empty_o;

using supported_ue_type_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SupportedUETypeList-Item ::= SEQUENCE
struct supported_ue_type_list_item_s {
  struct supported_ue_type_opts {
    enum options { non_redcap_eredcap_ue, redcap_eredcap_ue, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using supported_ue_type_e_ = enumerated<supported_ue_type_opts, true>;

  // member variables
  bool                                          ie_exts_present = false;
  supported_ue_type_e_                          supported_ue_type;
  supported_ue_type_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SupportedUETypeList ::= SEQUENCE (SIZE (1..8)) OF SupportedUETypeList-Item
using supported_ue_type_list_l = dyn_array<supported_ue_type_list_item_s>;

// TAInformation-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct tai_nformation_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct tai_nformation_item_ext_ies_container {
  bool                      tag_id_pointer_present = false;
  unbounded_octstring<true> tag_id_pointer;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAInformation-Item ::= SEQUENCE
struct tai_nformation_item_s {
  bool                                  ie_exts_present = false;
  nr_cgi_s                              nr_cgi;
  uint16_t                              ta_value = 0;
  tai_nformation_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAInformation-List ::= SEQUENCE (SIZE (1..8)) OF TAInformation-Item
using tai_nformation_list_l = dyn_array<tai_nformation_item_s>;

// TNLCapacityIndicator-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using tnl_capacity_ind_ext_ies_o = protocol_ext_empty_o;

using tnl_capacity_ind_ext_ies_container = protocol_ext_container_empty_l;

// TNLCapacityIndicator ::= SEQUENCE
struct tnl_capacity_ind_s {
  bool                               ie_exts_present           = false;
  uint32_t                           dl_tnl_offered_capacity   = 1;
  uint16_t                           dl_tnl_available_capacity = 0;
  uint32_t                           ul_tnl_offered_capacity   = 1;
  uint16_t                           ul_tnl_available_capacity = 0;
  tnl_capacity_ind_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementRequestItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct trp_meas_request_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_meas_request_item_ext_ies_container {
  bool                nr_cgi_present            = false;
  bool                ao_a_search_win_present   = false;
  bool                nof_trp_rx_teg_present    = false;
  bool                nof_trp_rx_tx_teg_present = false;
  nr_cgi_s            nr_cgi;
  ao_a_assist_info_s  ao_a_search_win;
  nof_trp_rx_teg_e    nof_trp_rx_teg;
  nof_trp_rx_tx_teg_e nof_trp_rx_tx_teg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementRequestItem ::= SEQUENCE
struct trp_meas_request_item_s {
  bool                                    search_win_info_present = false;
  bool                                    ie_exts_present         = false;
  uint32_t                                trp_id                  = 0;
  search_win_info_s                       search_win_info;
  trp_meas_request_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementRequestList ::= SEQUENCE (SIZE (1..64)) OF TRP-MeasurementRequestItem
using trp_meas_request_list_l = dyn_array<trp_meas_request_item_s>;

// TRP-MeasurementUpdateItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct trp_meas_upd_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_meas_upd_item_ext_ies_container {
  bool                nof_trp_rx_teg_present    = false;
  bool                nof_trp_rx_tx_teg_present = false;
  nof_trp_rx_teg_e    nof_trp_rx_teg;
  nof_trp_rx_tx_teg_e nof_trp_rx_tx_teg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementUpdateItem ::= SEQUENCE
struct trp_meas_upd_item_s {
  bool                                ao_a_win_info_present = false;
  bool                                ie_exts_present       = false;
  uint32_t                            trp_id                = 0;
  ao_a_assist_info_s                  ao_a_win_info;
  trp_meas_upd_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementUpdateList ::= SEQUENCE (SIZE (1..64)) OF TRP-MeasurementUpdateItem
using trp_meas_upd_list_l = dyn_array<trp_meas_upd_item_s>;

// TRP-PRS-Info-List-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_prs_info_list_item_ext_ies_o = protocol_ext_empty_o;

using trp_prs_info_list_item_ext_ies_container = protocol_ext_container_empty_l;

// TRP-PRS-Info-List-Item ::= SEQUENCE
struct trp_prs_info_list_item_s {
  bool                                     cgi_nr_present  = false;
  bool                                     ie_exts_present = false;
  uint32_t                                 trp_id          = 0;
  uint16_t                                 nr_pci          = 0;
  nr_cgi_s                                 cgi_nr;
  prs_cfg_s                                prs_cfg;
  trp_prs_info_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-PRS-Info-List ::= SEQUENCE (SIZE (1..256)) OF TRP-PRS-Info-List-Item
using trp_prs_info_list_l = dyn_array<trp_prs_info_list_item_s>;

// TRPBeamAntennaInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_beam_ant_info_ext_ies_o = protocol_ext_empty_o;

using trp_beam_ant_info_ext_ies_container = protocol_ext_container_empty_l;

// TRPBeamAntennaInformation ::= SEQUENCE
struct trp_beam_ant_info_s {
  bool                                ie_exts_present = false;
  choice_trp_beam_ant_info_item_c     choice_trp_beam_ant_info_item;
  trp_beam_ant_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPType ::= ENUMERATED
struct trp_type_opts {
  enum options { prs_only_tp, srs_only_rp, tp, rp, trp, /*...*/ mobile_trp, nulltype } value;

  const char* to_string() const;
};
using trp_type_e = enumerated<trp_type_opts, true, 1>;

// TRPTEGItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trpteg_item_ext_ies_o = protocol_ext_empty_o;

using trpteg_item_ext_ies_container = protocol_ext_container_empty_l;

// TRPTEG-Item ::= SEQUENCE
struct trpteg_item_s {
  using dl_prs_res_id_list_l_ = dyn_array<dl_prs_res_id_item_s>;

  // member variables
  bool                          ie_exts_present = false;
  trp_tx_teg_info_s             trp_tx_teg_info;
  uint8_t                       dl_prs_res_set_id = 0;
  dl_prs_res_id_list_l_         dl_prs_res_id_list;
  trpteg_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPTxTEGAssociation ::= SEQUENCE (SIZE (1..8)) OF TRPTEG-Item
using trp_tx_teg_assoc_l = dyn_array<trpteg_item_s>;

// TRPInformationTypeResponseItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
struct trp_info_type_resp_item_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options {
        trp_type,
        on_demand_prs,
        trp_tx_teg_assoc,
        trp_beam_ant_info,
        mobile_trp_location_info,
        nulltype
      } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    value_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    trp_type_e&                       trp_type();
    on_demand_prs_info_s&             on_demand_prs();
    trp_tx_teg_assoc_l&               trp_tx_teg_assoc();
    trp_beam_ant_info_s&              trp_beam_ant_info();
    mobile_trp_location_info_s&       mobile_trp_location_info();
    const trp_type_e&                 trp_type() const;
    const on_demand_prs_info_s&       on_demand_prs() const;
    const trp_tx_teg_assoc_l&         trp_tx_teg_assoc() const;
    const trp_beam_ant_info_s&        trp_beam_ant_info() const;
    const mobile_trp_location_info_s& mobile_trp_location_info() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// TRPInformationTypeResponseItem ::= CHOICE
struct trp_info_type_resp_item_c {
  struct types_opts {
    enum options {
      pci_nr,
      ng_ran_cgi,
      nr_arfcn,
      prs_cfg,
      ss_binfo,
      sfn_initisation_time,
      spatial_direction_info,
      geographical_coordinates,
      choice_ext,
      nulltype
    } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  trp_info_type_resp_item_c() = default;
  trp_info_type_resp_item_c(const trp_info_type_resp_item_c& other);
  trp_info_type_resp_item_c& operator=(const trp_info_type_resp_item_c& other);
  ~trp_info_type_resp_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint16_t& pci_nr()
  {
    assert_choice_type(types::pci_nr, type_, "TRPInformationTypeResponseItem");
    return c.get<uint16_t>();
  }
  nr_cgi_s& ng_ran_cgi()
  {
    assert_choice_type(types::ng_ran_cgi, type_, "TRPInformationTypeResponseItem");
    return c.get<nr_cgi_s>();
  }
  uint32_t& nr_arfcn()
  {
    assert_choice_type(types::nr_arfcn, type_, "TRPInformationTypeResponseItem");
    return c.get<uint32_t>();
  }
  prs_cfg_s& prs_cfg()
  {
    assert_choice_type(types::prs_cfg, type_, "TRPInformationTypeResponseItem");
    return c.get<prs_cfg_s>();
  }
  ssb_info_s& ss_binfo()
  {
    assert_choice_type(types::ss_binfo, type_, "TRPInformationTypeResponseItem");
    return c.get<ssb_info_s>();
  }
  fixed_bitstring<64, false, true>& sfn_initisation_time()
  {
    assert_choice_type(types::sfn_initisation_time, type_, "TRPInformationTypeResponseItem");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  spatial_direction_info_s& spatial_direction_info()
  {
    assert_choice_type(types::spatial_direction_info, type_, "TRPInformationTypeResponseItem");
    return c.get<spatial_direction_info_s>();
  }
  geographical_coordinates_s& geographical_coordinates()
  {
    assert_choice_type(types::geographical_coordinates, type_, "TRPInformationTypeResponseItem");
    return c.get<geographical_coordinates_s>();
  }
  protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TRPInformationTypeResponseItem");
    return c.get<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>();
  }
  const uint16_t& pci_nr() const
  {
    assert_choice_type(types::pci_nr, type_, "TRPInformationTypeResponseItem");
    return c.get<uint16_t>();
  }
  const nr_cgi_s& ng_ran_cgi() const
  {
    assert_choice_type(types::ng_ran_cgi, type_, "TRPInformationTypeResponseItem");
    return c.get<nr_cgi_s>();
  }
  const uint32_t& nr_arfcn() const
  {
    assert_choice_type(types::nr_arfcn, type_, "TRPInformationTypeResponseItem");
    return c.get<uint32_t>();
  }
  const prs_cfg_s& prs_cfg() const
  {
    assert_choice_type(types::prs_cfg, type_, "TRPInformationTypeResponseItem");
    return c.get<prs_cfg_s>();
  }
  const ssb_info_s& ss_binfo() const
  {
    assert_choice_type(types::ss_binfo, type_, "TRPInformationTypeResponseItem");
    return c.get<ssb_info_s>();
  }
  const fixed_bitstring<64, false, true>& sfn_initisation_time() const
  {
    assert_choice_type(types::sfn_initisation_time, type_, "TRPInformationTypeResponseItem");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  const spatial_direction_info_s& spatial_direction_info() const
  {
    assert_choice_type(types::spatial_direction_info, type_, "TRPInformationTypeResponseItem");
    return c.get<spatial_direction_info_s>();
  }
  const geographical_coordinates_s& geographical_coordinates() const
  {
    assert_choice_type(types::geographical_coordinates, type_, "TRPInformationTypeResponseItem");
    return c.get<geographical_coordinates_s>();
  }
  const protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TRPInformationTypeResponseItem");
    return c.get<protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>>();
  }
  uint16_t&                                                          set_pci_nr();
  nr_cgi_s&                                                          set_ng_ran_cgi();
  uint32_t&                                                          set_nr_arfcn();
  prs_cfg_s&                                                         set_prs_cfg();
  ssb_info_s&                                                        set_ss_binfo();
  fixed_bitstring<64, false, true>&                                  set_sfn_initisation_time();
  spatial_direction_info_s&                                          set_spatial_direction_info();
  geographical_coordinates_s&                                        set_geographical_coordinates();
  protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<64, false, true>,
                  geographical_coordinates_s,
                  nr_cgi_s,
                  protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>,
                  prs_cfg_s,
                  spatial_direction_info_s,
                  ssb_info_s>
      c;

  void destroy_();
};

// TRPInformationTypeResponseList ::= SEQUENCE (SIZE (1..64)) OF TRPInformationTypeResponseItem
using trp_info_type_resp_list_l = dyn_array<trp_info_type_resp_item_c>;

// TRPInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct trp_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_info_ext_ies_container {
  bool                      mobile_iab_mt_ue_id_present = false;
  unbounded_octstring<true> mobile_iab_mt_ue_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPInformation ::= SEQUENCE
struct trp_info_s {
  bool                       ie_exts_present = false;
  uint32_t                   trp_id          = 0;
  trp_info_type_resp_list_l  trp_info_type_resp_list;
  trp_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPInformationItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_info_item_ext_ies_o = protocol_ext_empty_o;

using trp_info_item_ext_ies_container = protocol_ext_container_empty_l;

// TRPInformationItem ::= SEQUENCE
struct trp_info_item_s {
  bool                            ie_exts_present = false;
  trp_info_s                      trp_info;
  trp_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPInformationTypeItem ::= ENUMERATED
struct trp_info_type_item_opts {
  enum options {
    nr_pci,
    ng_ran_cgi,
    arfcn,
    prs_cfg,
    ssb_cfg,
    sfn_init_time,
    spatial_direct_info,
    geo_coord,
    // ...
    trp_type,
    ondemand_prs,
    trp_tx_teg,
    beam_ant_info,
    mobile_trp_location_info,
    nulltype
  } value;

  const char* to_string() const;
};
using trp_info_type_item_e = enumerated<trp_info_type_item_opts, true, 5>;

// TRPListItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using trp_list_item_ext_ies_o = protocol_ext_empty_o;

using trp_list_item_ext_ies_container = protocol_ext_container_empty_l;

// TRPListItem ::= SEQUENCE
struct trp_list_item_s {
  bool                            ie_exts_present = false;
  uint32_t                        trp_id          = 0;
  trp_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPList ::= SEQUENCE (SIZE (1..65535)) OF TRPListItem
using trp_list_l = dyn_array<trp_list_item_s>;

// TimeReferenceInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using time_ref_info_ext_ies_o = protocol_ext_empty_o;

using time_ref_info_ext_ies_container = protocol_ext_container_empty_l;

// TimeReferenceInformation ::= SEQUENCE
struct time_ref_info_s {
  bool                            uncertainty_present    = false;
  bool                            time_info_type_present = false;
  bool                            ie_exts_present        = false;
  unbounded_octstring<true>       ref_time;
  uint16_t                        ref_sfn     = 0;
  uint32_t                        uncertainty = 0;
  time_ref_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeToWait ::= ENUMERATED
struct time_to_wait_opts {
  enum options { v1s, v2s, v5s, v10s, v20s, v60s, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using time_to_wait_e = enumerated<time_to_wait_opts, true>;

// TimeWindowDurationMeasurement-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using time_win_dur_meas_ext_ies_o = protocol_ies_empty_o;

// TimeWindowDurationMeasurement ::= CHOICE
struct time_win_dur_meas_c {
  struct dur_slots_opts {
    enum options { n1, n2, n4, n6, n8, n12, n16, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using dur_slots_e_ = enumerated<dur_slots_opts, true>;
  struct types_opts {
    enum options { dur_slots, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  time_win_dur_meas_c() = default;
  time_win_dur_meas_c(const time_win_dur_meas_c& other);
  time_win_dur_meas_c& operator=(const time_win_dur_meas_c& other);
  ~time_win_dur_meas_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  dur_slots_e_& dur_slots()
  {
    assert_choice_type(types::dur_slots, type_, "TimeWindowDurationMeasurement");
    return c.get<dur_slots_e_>();
  }
  protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TimeWindowDurationMeasurement");
    return c.get<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>();
  }
  const dur_slots_e_& dur_slots() const
  {
    assert_choice_type(types::dur_slots, type_, "TimeWindowDurationMeasurement");
    return c.get<dur_slots_e_>();
  }
  const protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TimeWindowDurationMeasurement");
    return c.get<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>>();
  }
  dur_slots_e_&                                                set_dur_slots();
  protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>& set_choice_ext();

private:
  types                                                                        type_;
  choice_buffer_t<protocol_ie_single_container_s<time_win_dur_meas_ext_ies_o>> c;

  void destroy_();
};

// TimeWindowDurationSRS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using time_win_dur_srs_ext_ies_o = protocol_ies_empty_o;

// TimeWindowDurationSRS ::= CHOICE
struct time_win_dur_srs_c {
  struct dur_symbols_opts {
    enum options { n1, n2, n4, n8, n12, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using dur_symbols_e_ = enumerated<dur_symbols_opts, true>;
  struct dur_slots_opts {
    enum options { n1, n2, n4, n6, n8, n12, n16, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using dur_slots_e_ = enumerated<dur_slots_opts, true>;
  struct types_opts {
    enum options { dur_symbols, dur_slots, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  time_win_dur_srs_c() = default;
  time_win_dur_srs_c(const time_win_dur_srs_c& other);
  time_win_dur_srs_c& operator=(const time_win_dur_srs_c& other);
  ~time_win_dur_srs_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  dur_symbols_e_& dur_symbols()
  {
    assert_choice_type(types::dur_symbols, type_, "TimeWindowDurationSRS");
    return c.get<dur_symbols_e_>();
  }
  dur_slots_e_& dur_slots()
  {
    assert_choice_type(types::dur_slots, type_, "TimeWindowDurationSRS");
    return c.get<dur_slots_e_>();
  }
  protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TimeWindowDurationSRS");
    return c.get<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>();
  }
  const dur_symbols_e_& dur_symbols() const
  {
    assert_choice_type(types::dur_symbols, type_, "TimeWindowDurationSRS");
    return c.get<dur_symbols_e_>();
  }
  const dur_slots_e_& dur_slots() const
  {
    assert_choice_type(types::dur_slots, type_, "TimeWindowDurationSRS");
    return c.get<dur_slots_e_>();
  }
  const protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TimeWindowDurationSRS");
    return c.get<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>>();
  }
  dur_symbols_e_&                                             set_dur_symbols();
  dur_slots_e_&                                               set_dur_slots();
  protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>& set_choice_ext();

private:
  types                                                                       type_;
  choice_buffer_t<protocol_ie_single_container_s<time_win_dur_srs_ext_ies_o>> c;

  void destroy_();
};

// TimeWindowPeriodicityMeasurement ::= ENUMERATED
struct time_win_periodicity_meas_opts {
  enum options {
    ms160,
    ms320,
    ms640,
    ms1280,
    ms2560,
    ms5120,
    ms10240,
    ms20480,
    ms40960,
    ms61440,
    ms81920,
    ms368640,
    ms737280,
    ms1843200,
    // ...
    nulltype
  } value;
  typedef uint32_t number_type;

  const char* to_string() const;
  uint32_t    to_number() const;
};
using time_win_periodicity_meas_e = enumerated<time_win_periodicity_meas_opts, true>;

// TimeWindowStart-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using time_win_start_ext_ies_o = protocol_ext_empty_o;

using time_win_start_ext_ies_container = protocol_ext_container_empty_l;

// TimeWindowStart ::= SEQUENCE
struct time_win_start_s {
  bool                             ie_ext_present = false;
  uint16_t                         sys_frame_num  = 0;
  uint8_t                          slot_num       = 0;
  uint8_t                          symbol_idx     = 0;
  time_win_start_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeWindowInformation-Measurement-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using time_win_info_meas_item_ext_ies_o = protocol_ext_empty_o;

using time_win_info_meas_item_ext_ies_container = protocol_ext_container_empty_l;

// TimeWindowInformation-Measurement-Item ::= SEQUENCE
struct time_win_info_meas_item_s {
  struct time_win_type_opts {
    enum options { single, periodic, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using time_win_type_e_ = enumerated<time_win_type_opts, true>;

  // member variables
  bool                                      time_win_periodicity_meas_present = false;
  bool                                      ie_ext_present                    = false;
  time_win_dur_meas_c                       time_win_dur_meas;
  time_win_type_e_                          time_win_type;
  time_win_periodicity_meas_e               time_win_periodicity_meas;
  time_win_start_s                          time_win_start;
  time_win_info_meas_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeWindowInformation-Measurement-List ::= SEQUENCE (SIZE (1..16)) OF TimeWindowInformation-Measurement-Item
using time_win_info_meas_list_l = dyn_array<time_win_info_meas_item_s>;

// TimeWindowInformation-SRS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using time_win_info_srs_ext_ies_o = protocol_ext_empty_o;

// TimeWindowStartSRS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using time_win_start_srs_ext_ies_o = protocol_ext_empty_o;

using time_win_start_srs_ext_ies_container = protocol_ext_container_empty_l;

// TimeWindowStartSRS ::= SEQUENCE
struct time_win_start_srs_s {
  bool                                 ie_ext_present = false;
  uint16_t                             sys_frame_num  = 0;
  uint8_t                              slot_num       = 0;
  uint8_t                              symbol_idx     = 0;
  time_win_start_srs_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeWindowPeriodicitySRS ::= ENUMERATED
struct time_win_periodicity_srs_opts {
  enum options {
    ms0dot125,
    ms0dot25,
    ms0dot5,
    ms0dot625,
    ms1,
    ms1dot25,
    ms2,
    ms2dot5,
    ms4,
    ms5,
    ms8,
    ms10,
    ms16,
    ms20,
    ms32,
    ms40,
    ms64,
    ms80,
    ms160,
    ms320,
    ms640,
    ms1280,
    ms2560,
    ms5120,
    ms10240,
    // ...
    nulltype
  } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using time_win_periodicity_srs_e = enumerated<time_win_periodicity_srs_opts, true>;

using time_win_info_srs_ext_ies_container = protocol_ext_container_empty_l;

// TimeWindowInformation-SRS-Item ::= SEQUENCE
struct time_win_info_srs_item_s {
  struct time_win_type_opts {
    enum options { single, periodic, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using time_win_type_e_ = enumerated<time_win_type_opts, true>;

  // member variables
  bool                                time_win_periodicity_srs_present = false;
  bool                                ie_ext_present                   = false;
  time_win_start_srs_s                time_win_start_srs;
  time_win_dur_srs_c                  time_win_dur_srs;
  time_win_type_e_                    time_win_type;
  time_win_periodicity_srs_e          time_win_periodicity_srs;
  time_win_info_srs_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeWindowInformation-SRS-List ::= SEQUENCE (SIZE (1..16)) OF TimeWindowInformation-SRS-Item
using time_win_info_srs_list_l = dyn_array<time_win_info_srs_item_s>;

// TraceDepth ::= ENUMERATED
struct trace_depth_opts {
  enum options {
    minimum,
    medium,
    max,
    minimum_without_vendor_specific_ext,
    medium_without_vendor_specific_ext,
    max_without_vendor_specific_ext,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using trace_depth_e = enumerated<trace_depth_opts, true>;

// TraceActivation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct trace_activation_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trace_activation_ext_ies_container {
  bool                                                  mdt_cfg_present                     = false;
  bool                                                  trace_collection_entity_uri_present = false;
  mdt_cfg_s                                             mdt_cfg;
  visible_string<0, MAX_ASN_STRING_LENGTH, false, true> trace_collection_entity_uri;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TraceActivation ::= SEQUENCE
struct trace_activation_s {
  bool                                  ie_exts_present = false;
  fixed_octstring<8, true>              trace_id;
  fixed_bitstring<8, false, true>       interfaces_to_trace;
  trace_depth_e                         trace_depth;
  bounded_bitstring<1, 160, true, true> trace_collection_entity_ip_address;
  trace_activation_ext_ies_container    ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TransmissionActionIndicator ::= ENUMERATED
struct tx_action_ind_opts {
  enum options { stop, /*...*/ restart, nulltype } value;

  const char* to_string() const;
};
using tx_action_ind_e = enumerated<tx_action_ind_opts, true, 1>;

// Transport-UP-Layer-Address-Info-To-Add-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using transport_up_layer_address_info_to_add_item_ext_ies_o = protocol_ext_empty_o;

using transport_up_layer_address_info_to_add_item_ext_ies_container = protocol_ext_container_empty_l;

// Transport-UP-Layer-Address-Info-To-Add-Item ::= SEQUENCE
struct transport_up_layer_address_info_to_add_item_s {
  bool                                                          ie_exts_present = false;
  bounded_bitstring<1, 160, true, true>                         ip_sec_transport_layer_address;
  gtp_tlas_l                                                    gtp_transport_layer_address_to_add;
  transport_up_layer_address_info_to_add_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Transport-UP-Layer-Address-Info-To-Add-List ::= SEQUENCE (SIZE (1..16)) OF
// Transport-UP-Layer-Address-Info-To-Add-Item
using transport_up_layer_address_info_to_add_list_l = dyn_array<transport_up_layer_address_info_to_add_item_s>;

// Transport-UP-Layer-Address-Info-To-Remove-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using transport_up_layer_address_info_to_rem_item_ext_ies_o = protocol_ext_empty_o;

using transport_up_layer_address_info_to_rem_item_ext_ies_container = protocol_ext_container_empty_l;

// Transport-UP-Layer-Address-Info-To-Remove-Item ::= SEQUENCE
struct transport_up_layer_address_info_to_rem_item_s {
  bool                                                          ie_exts_present = false;
  bounded_bitstring<1, 160, true, true>                         ip_sec_transport_layer_address;
  gtp_tlas_l                                                    gtp_transport_layer_address_to_rem;
  transport_up_layer_address_info_to_rem_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Transport-UP-Layer-Address-Info-To-Remove-List ::= SEQUENCE (SIZE (1..16)) OF
// Transport-UP-Layer-Address-Info-To-Remove-Item
using transport_up_layer_address_info_to_rem_list_l = dyn_array<transport_up_layer_address_info_to_rem_item_s>;

// Transport-Layer-Address-Info-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using transport_layer_address_info_ext_ies_o = protocol_ext_empty_o;

using transport_layer_address_info_ext_ies_container = protocol_ext_container_empty_l;

// Transport-Layer-Address-Info ::= SEQUENCE
struct transport_layer_address_info_s {
  bool                                           ie_exts_present = false;
  transport_up_layer_address_info_to_add_list_l  transport_up_layer_address_info_to_add_list;
  transport_up_layer_address_info_to_rem_list_l  transport_up_layer_address_info_to_rem_list;
  transport_layer_address_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UACAction ::= ENUMERATED
struct uac_action_opts {
  enum options {
    reject_non_emergency_mo_dt,
    reject_rrc_cr_sig,
    permit_emergency_sessions_and_mobile_terminated_services_only,
    permit_high_prio_sessions_and_mobile_terminated_services_only,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using uac_action_e = enumerated<uac_action_opts, true>;

// UACOperatorDefined-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uac_operator_defined_ext_ies_o = protocol_ext_empty_o;

using uac_operator_defined_ext_ies_container = protocol_ext_container_empty_l;

// UACOperatorDefined ::= SEQUENCE
struct uac_operator_defined_s {
  bool                                   ie_exts_present = false;
  uint8_t                                access_category = 32;
  fixed_bitstring<7, false, true>        access_id;
  uac_operator_defined_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UACCategoryType-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using uac_category_type_ext_ies_o = protocol_ies_empty_o;

// UACCategoryType ::= CHOICE
struct uac_category_type_c {
  struct types_opts {
    enum options { uac_standardized, uac_operator_defined, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  uac_category_type_c() = default;
  uac_category_type_c(const uac_category_type_c& other);
  uac_category_type_c& operator=(const uac_category_type_c& other);
  ~uac_category_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uac_action_e& uac_standardized()
  {
    assert_choice_type(types::uac_standardized, type_, "UACCategoryType");
    return c.get<uac_action_e>();
  }
  uac_operator_defined_s& uac_operator_defined()
  {
    assert_choice_type(types::uac_operator_defined, type_, "UACCategoryType");
    return c.get<uac_operator_defined_s>();
  }
  protocol_ie_single_container_s<uac_category_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UACCategoryType");
    return c.get<protocol_ie_single_container_s<uac_category_type_ext_ies_o>>();
  }
  const uac_action_e& uac_standardized() const
  {
    assert_choice_type(types::uac_standardized, type_, "UACCategoryType");
    return c.get<uac_action_e>();
  }
  const uac_operator_defined_s& uac_operator_defined() const
  {
    assert_choice_type(types::uac_operator_defined, type_, "UACCategoryType");
    return c.get<uac_operator_defined_s>();
  }
  const protocol_ie_single_container_s<uac_category_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UACCategoryType");
    return c.get<protocol_ie_single_container_s<uac_category_type_ext_ies_o>>();
  }
  uac_action_e&                                                set_uac_standardized();
  uac_operator_defined_s&                                      set_uac_operator_defined();
  protocol_ie_single_container_s<uac_category_type_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                type_;
  choice_buffer_t<protocol_ie_single_container_s<uac_category_type_ext_ies_o>, uac_operator_defined_s> c;

  void destroy_();
};

// UACType-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uac_type_item_ext_ies_o = protocol_ext_empty_o;

using uac_type_item_ext_ies_container = protocol_ext_container_empty_l;

// UACType-Item ::= SEQUENCE
struct uac_type_item_s {
  bool                            ie_exts_present   = false;
  uint8_t                         uac_reduction_ind = 0;
  uac_category_type_c             uac_category_type;
  uac_type_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UACType-List ::= SEQUENCE (SIZE (1..64)) OF UACType-Item
using uac_type_list_l = dyn_array<uac_type_item_s>;

// UACPLMN-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct uac_plmn_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct uac_plmn_item_ext_ies_container {
  bool                             n_id_present = false;
  fixed_bitstring<44, false, true> n_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UACPLMN-Item ::= SEQUENCE
struct uac_plmn_item_s {
  bool                            ie_exts_present = false;
  fixed_octstring<3, true>        plmn_id;
  uac_type_list_l                 uac_type_list;
  uac_plmn_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UACPLMN-List ::= SEQUENCE (SIZE (1..12)) OF UACPLMN-Item
using uac_plmn_list_l = dyn_array<uac_plmn_item_s>;

// UAC-Assistance-InfoExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uac_assist_info_ext_ies_o = protocol_ext_empty_o;

using uac_assist_info_ext_ies_container = protocol_ext_container_empty_l;

// UAC-Assistance-Info ::= SEQUENCE
struct uac_assist_info_s {
  bool                              ie_exts_present = false;
  uac_plmn_list_l                   uac_plmn_list;
  uac_assist_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-ConfirmedToBeModified-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ue_multicast_m_rbs_confirmed_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using ue_multicast_m_rbs_confirmed_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// UE-MulticastMRBs-ConfirmedToBeModified-Item ::= SEQUENCE
struct ue_multicast_m_rbs_confirmed_to_be_modified_item_s {
  bool                                                               mrb_type_recfg_present = false;
  bool                                                               ie_exts_present        = false;
  uint16_t                                                           mrb_id                 = 1;
  mbsptp_retx_tunnel_required_e                                      mrb_type_recfg;
  ue_multicast_m_rbs_confirmed_to_be_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-RequiredToBeModified-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct ue_multicast_m_rbs_required_to_be_modified_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_multicast_m_rbs_required_to_be_modified_item_ext_ies_container {
  bool                     multicast_f1_u_context_ref_cu_present = false;
  fixed_octstring<4, true> multicast_f1_u_context_ref_cu;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-RequiredToBeModified-Item ::= SEQUENCE
struct ue_multicast_m_rbs_required_to_be_modified_item_s {
  struct mrb_type_recfg_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using mrb_type_recfg_e_ = enumerated<mrb_type_recfg_opts, true>;
  struct mrb_recfg_rlc_type_opts {
    enum options {
      rlc_um_ptp,
      rlc_am_ptp,
      rlc_um_dl_ptm,
      two_rlc_um_dl_ptp_and_dl_ptm,
      three_rlc_um_dl_ptp_ul_ptp_dl_ptm,
      two_rlc_am_ptp_um_dl_ptm,
      // ...
      nulltype
    } value;

    const char* to_string() const;
  };
  using mrb_recfg_rlc_type_e_ = enumerated<mrb_recfg_rlc_type_opts, true>;

  // member variables
  bool                                                              mrb_type_recfg_present     = false;
  bool                                                              mrb_recfg_rlc_type_present = false;
  bool                                                              ie_exts_present            = false;
  uint16_t                                                          mrb_id                     = 1;
  mrb_type_recfg_e_                                                 mrb_type_recfg;
  mrb_recfg_rlc_type_e_                                             mrb_recfg_rlc_type;
  ue_multicast_m_rbs_required_to_be_modified_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-RequiredToBeReleased-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ue_multicast_m_rbs_required_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using ue_multicast_m_rbs_required_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// UE-MulticastMRBs-RequiredToBeReleased-Item ::= SEQUENCE
struct ue_multicast_m_rbs_required_to_be_released_item_s {
  bool                                                              ie_exts_present = false;
  uint16_t                                                          mrb_id          = 1;
  ue_multicast_m_rbs_required_to_be_released_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-Setup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ue_multicast_m_rbs_setup_item_ext_ies_o = protocol_ext_empty_o;

using ue_multicast_m_rbs_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// UE-MulticastMRBs-Setup-Item ::= SEQUENCE
struct ue_multicast_m_rbs_setup_item_s {
  bool                                            ie_exts_present = false;
  uint16_t                                        mrb_id          = 1;
  fixed_octstring<4, true>                        multicast_f1_u_context_ref_cu;
  ue_multicast_m_rbs_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-Setupnew-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ue_multicast_m_rbs_setupnew_item_ext_ies_o = protocol_ext_empty_o;

using ue_multicast_m_rbs_setupnew_item_ext_ies_container = protocol_ext_container_empty_l;

// UE-MulticastMRBs-Setupnew-Item ::= SEQUENCE
struct ue_multicast_m_rbs_setupnew_item_s {
  bool                                               ie_exts_present = false;
  uint16_t                                           mrb_id          = 1;
  fixed_octstring<4, true>                           multicast_f1_u_context_ref_cu;
  ue_multicast_m_rbs_setupnew_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-ToBeReleased-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ue_multicast_m_rbs_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using ue_multicast_m_rbs_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// UE-MulticastMRBs-ToBeReleased-Item ::= SEQUENCE
struct ue_multicast_m_rbs_to_be_released_item_s {
  bool                                                     ie_exts_present = false;
  uint16_t                                                 mrb_id          = 1;
  ue_multicast_m_rbs_to_be_released_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-ToBeSetup-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct ue_multicast_m_rbs_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_multicast_m_rbs_to_be_setup_item_ext_ies_container {
  bool     source_mrb_id_present = false;
  uint16_t source_mrb_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-ToBeSetup-Item ::= SEQUENCE
struct ue_multicast_m_rbs_to_be_setup_item_s {
  bool                                                  mbs_ptp_retx_tunnel_required_present     = false;
  bool                                                  mbs_ptp_forwarding_required_info_present = false;
  bool                                                  ie_exts_present                          = false;
  uint16_t                                              mrb_id                                   = 1;
  mbsptp_retx_tunnel_required_e                         mbs_ptp_retx_tunnel_required;
  mrb_progress_info_c                                   mbs_ptp_forwarding_required_info;
  ue_multicast_m_rbs_to_be_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-MulticastMRBs-ToBeSetup-atModify-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ue_multicast_m_rbs_to_be_setup_at_modify_item_ext_ies_o = protocol_ext_empty_o;

using ue_multicast_m_rbs_to_be_setup_at_modify_item_ext_ies_container = protocol_ext_container_empty_l;

// UE-MulticastMRBs-ToBeSetup-atModify-Item ::= SEQUENCE
struct ue_multicast_m_rbs_to_be_setup_at_modify_item_s {
  bool                                                            mbs_ptp_retx_tunnel_required_present     = false;
  bool                                                            mbs_ptp_forwarding_required_info_present = false;
  bool                                                            ie_exts_present                          = false;
  uint16_t                                                        mrb_id                                   = 1;
  mbsptp_retx_tunnel_required_e                                   mbs_ptp_retx_tunnel_required;
  mrb_progress_info_c                                             mbs_ptp_forwarding_required_info;
  ue_multicast_m_rbs_to_be_setup_at_modify_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UE-associatedLogicalF1-ConnectionItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ue_associated_lc_f1_conn_item_ext_ies_o = protocol_ext_empty_o;

using ue_associated_lc_f1_conn_item_ext_ies_container = protocol_ext_container_empty_l;

// UE-associatedLogicalF1-ConnectionItem ::= SEQUENCE
struct ue_associated_lc_f1_conn_item_s {
  bool                                            gnb_cu_ue_f1ap_id_present = false;
  bool                                            gnb_du_ue_f1ap_id_present = false;
  bool                                            ie_exts_present           = false;
  uint64_t                                        gnb_cu_ue_f1ap_id         = 0;
  uint64_t                                        gnb_du_ue_f1ap_id         = 0;
  ue_associated_lc_f1_conn_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextNotRetrievable ::= ENUMERATED
struct ue_context_not_retrievable_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ue_context_not_retrievable_e = enumerated<ue_context_not_retrievable_opts, true>;

// UEIdentityIndexValueChoice-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using ue_id_idx_value_choice_ext_ies_o = protocol_ies_empty_o;

// UEIdentityIndexValue ::= CHOICE
struct ue_id_idx_value_c {
  struct types_opts {
    enum options { idx_len10, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ue_id_idx_value_c() = default;
  ue_id_idx_value_c(const ue_id_idx_value_c& other);
  ue_id_idx_value_c& operator=(const ue_id_idx_value_c& other);
  ~ue_id_idx_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<10, false, true>& idx_len10()
  {
    assert_choice_type(types::idx_len10, type_, "UEIdentityIndexValue");
    return c.get<fixed_bitstring<10, false, true>>();
  }
  protocol_ie_single_container_s<ue_id_idx_value_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UEIdentityIndexValue");
    return c.get<protocol_ie_single_container_s<ue_id_idx_value_choice_ext_ies_o>>();
  }
  const fixed_bitstring<10, false, true>& idx_len10() const
  {
    assert_choice_type(types::idx_len10, type_, "UEIdentityIndexValue");
    return c.get<fixed_bitstring<10, false, true>>();
  }
  const protocol_ie_single_container_s<ue_id_idx_value_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UEIdentityIndexValue");
    return c.get<protocol_ie_single_container_s<ue_id_idx_value_choice_ext_ies_o>>();
  }
  fixed_bitstring<10, false, true>&                                 set_idx_len10();
  protocol_ie_single_container_s<ue_id_idx_value_choice_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<10, false, true>, protocol_ie_single_container_s<ue_id_idx_value_choice_ext_ies_o>> c;

  void destroy_();
};

// UEIdentity-List-For-Paging-Item-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ue_id_list_for_paging_item_ext_ies_o = protocol_ext_empty_o;

using ue_id_list_for_paging_item_ext_ies_container = protocol_ext_container_empty_l;

// UEIdentity-List-For-Paging-Item ::= SEQUENCE
struct ue_id_list_for_paging_item_s {
  bool                                         paging_drx_present = false;
  bool                                         ie_exts_present    = false;
  ue_id_idx_value_c                            ue_id_idx_value;
  paging_drx_e                                 paging_drx;
  ue_id_list_for_paging_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEPagingCapability-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct ue_paging_cap_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_paging_cap_ext_ies_container {
  bool          red_cap_ind_present = false;
  red_cap_ind_e red_cap_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEPagingCapability ::= SEQUENCE
struct ue_paging_cap_s {
  struct inactive_state_po_determination_opts {
    enum options { supported, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using inactive_state_po_determination_e_ = enumerated<inactive_state_po_determination_opts, true>;

  // member variables
  bool                               inactive_state_po_determination_present = false;
  bool                               ie_ext_present                          = false;
  inactive_state_po_determination_e_ inactive_state_po_determination;
  ue_paging_cap_ext_ies_container    ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEReportingInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
struct ue_report_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_report_info_ext_ies_container {
  bool     report_interv_i_ms_present = false;
  uint16_t report_interv_i_ms;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEReportingInformation ::= SEQUENCE
struct ue_report_info_s {
  struct report_amount_opts {
    enum options { ma0, ma1, ma2, ma4, ma8, ma16, ma32, ma64, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using report_amount_e_ = enumerated<report_amount_opts>;
  struct report_interv_opts {
    enum options { none, one, two, four, eight, ten, sixteen, twenty, thirty_two, sixty_four, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using report_interv_e_ = enumerated<report_interv_opts, true>;

  // member variables
  bool                             ie_exts_present = false;
  report_amount_e_                 report_amount;
  report_interv_e_                 report_interv;
  ue_report_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-BH-Non-UP-Traffic-Mapping-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ul_bh_non_up_traffic_map_item_ext_ies_o = protocol_ext_empty_o;

using ul_bh_non_up_traffic_map_item_ext_ies_container = protocol_ext_container_empty_l;

// UL-BH-Non-UP-Traffic-Mapping-Item ::= SEQUENCE
struct ul_bh_non_up_traffic_map_item_s {
  bool                                            ie_exts_present = false;
  non_up_traffic_type_e                           non_up_traffic_type;
  bh_info_s                                       bh_info;
  ul_bh_non_up_traffic_map_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-BH-Non-UP-Traffic-Mapping-List ::= SEQUENCE (SIZE (1..32)) OF UL-BH-Non-UP-Traffic-Mapping-Item
using ul_bh_non_up_traffic_map_list_l = dyn_array<ul_bh_non_up_traffic_map_item_s>;

// UL-BH-Non-UP-Traffic-Mapping-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ul_bh_non_up_traffic_map_ext_ies_o = protocol_ext_empty_o;

using ul_bh_non_up_traffic_map_ext_ies_container = protocol_ext_container_empty_l;

// UL-BH-Non-UP-Traffic-Mapping ::= SEQUENCE
struct ul_bh_non_up_traffic_map_s {
  bool                                       ie_exts_present = false;
  ul_bh_non_up_traffic_map_list_l            ul_bh_non_up_traffic_map_list;
  ul_bh_non_up_traffic_map_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-UP-TNL-Address-to-Update-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ul_up_tnl_address_to_upd_list_item_ext_ies_o = protocol_ext_empty_o;

using ul_up_tnl_address_to_upd_list_item_ext_ies_container = protocol_ext_container_empty_l;

// UL-UP-TNL-Address-to-Update-List-Item ::= SEQUENCE
struct ul_up_tnl_address_to_upd_list_item_s {
  bool                                                 ie_exts_present = false;
  bounded_bitstring<1, 160, true, true>                old_ip_adress;
  bounded_bitstring<1, 160, true, true>                new_ip_adress;
  ul_up_tnl_address_to_upd_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-UP-TNL-Information-to-Update-List-ItemExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using ul_up_tnl_info_to_upd_list_item_ext_ies_o = protocol_ext_empty_o;

using ul_up_tnl_info_to_upd_list_item_ext_ies_container = protocol_ext_container_empty_l;

// UL-UP-TNL-Information-to-Update-List-Item ::= SEQUENCE
struct ul_up_tnl_info_to_upd_list_item_s {
  bool                                              new_ul_up_tnl_info_present = false;
  bool                                              ie_exts_present            = false;
  up_transport_layer_info_c                         ul_up_tnl_info;
  up_transport_layer_info_c                         new_ul_up_tnl_info;
  bh_info_s                                         bh_info;
  ul_up_tnl_info_to_upd_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelFailedToBeModifiedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_failed_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_failed_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelFailedToBeModifiedItem ::= SEQUENCE
struct uu_rlc_ch_failed_to_be_modified_item_s {
  bool                                                   cause_present   = false;
  bool                                                   ie_exts_present = false;
  uint8_t                                                uu_rlc_ch_id    = 1;
  cause_c                                                cause;
  uu_rlc_ch_failed_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelFailedToBeModifiedList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelFailedToBeModifiedItem
using uu_rlc_ch_failed_to_be_modified_list_l = dyn_array<uu_rlc_ch_failed_to_be_modified_item_s>;

// UuRLCChannelFailedToBeSetupItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_failed_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_failed_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelFailedToBeSetupItem ::= SEQUENCE
struct uu_rlc_ch_failed_to_be_setup_item_s {
  bool                                                cause_present   = false;
  bool                                                ie_exts_present = false;
  uint8_t                                             uu_rlc_ch_id    = 1;
  cause_c                                             cause;
  uu_rlc_ch_failed_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelFailedToBeSetupList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelFailedToBeSetupItem
using uu_rlc_ch_failed_to_be_setup_list_l = dyn_array<uu_rlc_ch_failed_to_be_setup_item_s>;

// UuRLCChannelModifiedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_modified_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelModifiedItem ::= SEQUENCE
struct uu_rlc_ch_modified_item_s {
  bool                                      ie_exts_present = false;
  uint8_t                                   uu_rlc_ch_id    = 1;
  uu_rlc_ch_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelModifiedList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelModifiedItem
using uu_rlc_ch_modified_list_l = dyn_array<uu_rlc_ch_modified_item_s>;

// UuRLCChannelQoSInformation-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
using uu_rlc_ch_qos_info_ext_ies_o = protocol_ies_empty_o;

// UuRLCChannelQoSInformation ::= CHOICE
struct uu_rlc_ch_qos_info_c {
  struct uu_ctrl_plane_traffic_type_opts {
    enum options { srb0, srb1, srb2, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using uu_ctrl_plane_traffic_type_e_ = enumerated<uu_ctrl_plane_traffic_type_opts, true>;
  struct types_opts {
    enum options { uu_rlc_ch_qos, uu_ctrl_plane_traffic_type, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  uu_rlc_ch_qos_info_c() = default;
  uu_rlc_ch_qos_info_c(const uu_rlc_ch_qos_info_c& other);
  uu_rlc_ch_qos_info_c& operator=(const uu_rlc_ch_qos_info_c& other);
  ~uu_rlc_ch_qos_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  qos_flow_level_qos_params_s& uu_rlc_ch_qos()
  {
    assert_choice_type(types::uu_rlc_ch_qos, type_, "UuRLCChannelQoSInformation");
    return c.get<qos_flow_level_qos_params_s>();
  }
  uu_ctrl_plane_traffic_type_e_& uu_ctrl_plane_traffic_type()
  {
    assert_choice_type(types::uu_ctrl_plane_traffic_type, type_, "UuRLCChannelQoSInformation");
    return c.get<uu_ctrl_plane_traffic_type_e_>();
  }
  protocol_ie_single_container_s<uu_rlc_ch_qos_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UuRLCChannelQoSInformation");
    return c.get<protocol_ie_single_container_s<uu_rlc_ch_qos_info_ext_ies_o>>();
  }
  const qos_flow_level_qos_params_s& uu_rlc_ch_qos() const
  {
    assert_choice_type(types::uu_rlc_ch_qos, type_, "UuRLCChannelQoSInformation");
    return c.get<qos_flow_level_qos_params_s>();
  }
  const uu_ctrl_plane_traffic_type_e_& uu_ctrl_plane_traffic_type() const
  {
    assert_choice_type(types::uu_ctrl_plane_traffic_type, type_, "UuRLCChannelQoSInformation");
    return c.get<uu_ctrl_plane_traffic_type_e_>();
  }
  const protocol_ie_single_container_s<uu_rlc_ch_qos_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UuRLCChannelQoSInformation");
    return c.get<protocol_ie_single_container_s<uu_rlc_ch_qos_info_ext_ies_o>>();
  }
  qos_flow_level_qos_params_s&                                  set_uu_rlc_ch_qos();
  uu_ctrl_plane_traffic_type_e_&                                set_uu_ctrl_plane_traffic_type();
  protocol_ie_single_container_s<uu_rlc_ch_qos_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                      type_;
  choice_buffer_t<protocol_ie_single_container_s<uu_rlc_ch_qos_info_ext_ies_o>, qos_flow_level_qos_params_s> c;

  void destroy_();
};

// UuRLCChannelRequiredToBeModifiedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_required_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_required_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelRequiredToBeModifiedItem ::= SEQUENCE
struct uu_rlc_ch_required_to_be_modified_item_s {
  bool                                                     ie_exts_present = false;
  uint8_t                                                  uu_rlc_ch_id    = 1;
  uu_rlc_ch_required_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelRequiredToBeModifiedList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelRequiredToBeModifiedItem
using uu_rlc_ch_required_to_be_modified_list_l = dyn_array<uu_rlc_ch_required_to_be_modified_item_s>;

// UuRLCChannelRequiredToBeReleasedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_required_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_required_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelRequiredToBeReleasedItem ::= SEQUENCE
struct uu_rlc_ch_required_to_be_released_item_s {
  bool                                                     ie_exts_present = false;
  uint8_t                                                  uu_rlc_ch_id    = 1;
  uu_rlc_ch_required_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelRequiredToBeReleasedList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelRequiredToBeReleasedItem
using uu_rlc_ch_required_to_be_released_list_l = dyn_array<uu_rlc_ch_required_to_be_released_item_s>;

// UuRLCChannelSetupItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_setup_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelSetupItem ::= SEQUENCE
struct uu_rlc_ch_setup_item_s {
  bool                                   ie_exts_present = false;
  uint8_t                                uu_rlc_ch_id    = 1;
  uu_rlc_ch_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelSetupList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelSetupItem
using uu_rlc_ch_setup_list_l = dyn_array<uu_rlc_ch_setup_item_s>;

// UuRLCChannelToBeModifiedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelToBeModifiedItem ::= SEQUENCE
struct uu_rlc_ch_to_be_modified_item_s {
  bool                                            uu_rlc_ch_qos_info_present = false;
  bool                                            rlc_mode_present           = false;
  bool                                            ie_exts_present            = false;
  uint8_t                                         uu_rlc_ch_id               = 1;
  uu_rlc_ch_qos_info_c                            uu_rlc_ch_qos_info;
  rlc_mode_e                                      rlc_mode;
  uu_rlc_ch_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelToBeModifiedList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelToBeModifiedItem
using uu_rlc_ch_to_be_modified_list_l = dyn_array<uu_rlc_ch_to_be_modified_item_s>;

// UuRLCChannelToBeReleasedItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelToBeReleasedItem ::= SEQUENCE
struct uu_rlc_ch_to_be_released_item_s {
  bool                                            ie_exts_present = false;
  uint8_t                                         uu_rlc_ch_id    = 1;
  uu_rlc_ch_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelToBeReleasedList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelToBeReleasedItem
using uu_rlc_ch_to_be_released_list_l = dyn_array<uu_rlc_ch_to_be_released_item_s>;

// UuRLCChannelToBeSetupItem-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
using uu_rlc_ch_to_be_setup_item_ext_ies_o = protocol_ext_empty_o;

using uu_rlc_ch_to_be_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// UuRLCChannelToBeSetupItem ::= SEQUENCE
struct uu_rlc_ch_to_be_setup_item_s {
  bool                                         ie_exts_present = false;
  uint8_t                                      uu_rlc_ch_id    = 1;
  uu_rlc_ch_qos_info_c                         uu_rlc_ch_qos_info;
  rlc_mode_e                                   rlc_mode;
  uu_rlc_ch_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UuRLCChannelToBeSetupList ::= SEQUENCE (SIZE (1..32)) OF UuRLCChannelToBeSetupItem
using uu_rlc_ch_to_be_setup_list_l = dyn_array<uu_rlc_ch_to_be_setup_item_s>;

} // namespace f1ap
} // namespace asn1

extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::arp_location_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::combined_pos_srs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::aggr_pos_srs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::du_f_slot_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::hs_nas_lot_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::freq_domain_h_sn_a_slot_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::freq_domain_h_sn_a_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::na_res_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::child_iab_nodes_na_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::parent_iab_nodes_na_res_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_carrier_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::freq_band_nr_item_s::supported_sul_band_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::nr_freq_info_s::freq_band_list_nr_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::activ_cells_to_be_upd_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::slot_offset_for_remaining_hops_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_srs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_res_set_list2_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_srs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::egress_bh_rlc_ch_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::add_pdcp_dupl_tnl_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::multiple_ul_ao_a_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::add_path_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::add_sib_msg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::affected_ssb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::affected_cells_and_beams_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::dl_prs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::aggr_prs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::aggr_pos_srs_res_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::aggressor_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::alt_qos_para_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::spatial_relationfor_res_id_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::spatial_relation_per_srs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::available_plmn_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::available_sn_pn_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ba_player_bh_rlc_ch_map_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::location_dependent_mbsf1_u_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ip_tolayer2_traffic_map_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bh_rlc_ch_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::extended_available_plmn_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::broadcast_sn_pn_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::broadcast_pni_npn_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::bplmn_id_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::broadcast_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_flows_mapped_to_mrb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pscell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::cu_to_du_tai_nformation_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_idx_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::candidate_cellwith_beam_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ssb_idxwith_meass_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::candidate_cellwith_meass_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ssb_area_capacity_value_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ssb_area_rr_status_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::snssai_rr_status_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::slice_rr_status_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::snssai_available_capacity_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::slice_available_capacity_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_u_ch_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cell_meas_result_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_to_report_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::snssai_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::slice_to_report_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cell_to_report_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::cells_with_ss_bs_activ_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::gnb_cu_sys_info_s::sibtypetobeupdlist_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::iab_stc_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_prach_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::neighbour_nr_cells_for_son_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cells_for_son_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::iab_mt_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::child_node_cells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::child_nodes_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::trp_elevation_angle_list_item_s::trp_beam_pwr_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::trp_beam_ant_angles_list_item_s::trp_elevation_angle_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_beam_ant_angles_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::target_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::cfg_bwp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_coverage_mod_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::coverage_mod_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::crit_diagnostics_ie_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::dl_prsbw_aggregation_request_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::dl_lbt_fail_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::dl_prs_res_set_arp_s::listof_dl_prs_res_arp_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::dl_prs_res_coordinates_s::listof_dl_prs_res_set_arp_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::dl_up_tnl_info_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::flows_mapped_to_drb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::drb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ul_up_tnl_info_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::rlc_dupl_state_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::du_to_cu_tai_nformation_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::deactivation_ind_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::e_c_id_measured_results_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::e_c_id_measured_results_associated_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::e_c_id_meas_quantities_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::eutra_cells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::preamb_idx_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ltm_gnb_du_ids_preamb_idx_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::early_sync_candidate_cell_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltm_gnb_du_ids_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::extended_add_path_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::slice_support_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nsag_support_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::extended_served_plmns_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::flows_mapped_to_sl_drb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::served_plmns_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::slot_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_u_ch_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::gnb_du_ue_slice_max_bit_rate_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::gtp_tlas_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::iab_congestion_ind_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::iab_tnl_address_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::lc_sto_gcs_translation_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltmcfra_res_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ltm_cells_to_be_released_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ssb_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltm_cfg_id_map_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ltml2_reset_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ltmtci_states_cfgs_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_broadcast_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_broadcast_mrb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_multicast_cu2_du_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_multicast_du2_cu_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_multicast_mrb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_service_area_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_service_area_tai_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mbs_service_area_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::mtch_neighbour_cell_session_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::thres_idx_session_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::multicast_common_cu2_du_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::multicast_mbs_session_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_angle_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::nr_prs_beam_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::neighbour_node_cells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_failed_to_be_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_failed_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_required_to_be_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_required_to_be_released_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_to_be_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_to_be_released_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pc5_rlc_ch_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pdc_measured_results_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_meas_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::prsbw_aggregation_request_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::requested_dl_prs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::requested_dl_prs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::prs_tx_off_per_res_set_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::prs_tx_off_per_res_item_s::prs_tx_off_ind_per_res_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_tx_off_per_res_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prstrp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::prs_tx_trp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::recommended_ss_bs_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::periodicity_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_meas_quantities_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_meas_result_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_meas_result_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::pos_sitype_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::pos_validity_area_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::qo_e_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ra_report_ind_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ra_report_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::rlf_report_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::rspp_qos_flow_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::recommended_ss_bs_for_paging_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::requested_srs_precfg_characteristics_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sdt_bearer_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sitype_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::sldrx_cycle_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::ul_ch_bw_per_scs_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_carrier_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::srs_precfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::serving_cells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::serving_cell_mo_encoded_in_cgc_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::successful_ho_report_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::successful_pscell_change_report_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::supported_ue_type_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::tai_nformation_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_meas_request_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_meas_upd_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_prs_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::trpteg_item_s::dl_prs_res_id_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_tx_teg_assoc_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::trp_info_type_resp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::trp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::time_win_info_meas_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::time_win_info_srs_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::transport_up_layer_address_info_to_add_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::transport_up_layer_address_info_to_rem_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::uac_type_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::uac_plmn_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::ul_bh_non_up_traffic_map_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_failed_to_be_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_failed_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_required_to_be_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_required_to_be_released_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::f1ap::uu_rlc_ch_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_to_be_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_to_be_released_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::f1ap::uu_rlc_ch_to_be_setup_list_l::value_type>::ops;
