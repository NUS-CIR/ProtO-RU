// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/*******************************************************************************
 *
 *                     3GPP TS ASN1 NRPPA v18.7.0 (2025-12)
 *
 ******************************************************************************/

#pragma once

#include "ocudu/asn1/asn1_ap_utils.h"

namespace asn1 {
namespace nrppa {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// TriggeringMessage ::= ENUMERATED
struct trigger_msg_opts {
  enum options { init_msg, successful_outcome, unsuccessful_outcome, nulltype } value;

  const char* to_string() const;
};
using trigger_msg_e = enumerated<trigger_msg_opts>;

// LocationUncertainty-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RelativeGeodeticLocation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RelativeCartesianLocation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ARPLocationType-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// ARPLocationInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// SCS-SpecificCarrier-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Combined-PosSRSResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using combined_pos_srs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using combined_pos_srs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// Combined-PosSRSResourceSet-Item ::= SEQUENCE
struct combined_pos_srs_res_set_item_s {
  bool                                            pci_nr_present    = false;
  bool                                            ie_exts_present   = false;
  uint32_t                                        point_a           = 0;
  uint16_t                                        pci_nr            = 0;
  uint8_t                                         possrs_res_set_id = 0;
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

// PosSRSResourceSet-Aggregation-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pos_srs_res_set_aggregation_item_ext_ies_o = protocol_ext_empty_o;

using pos_srs_res_set_aggregation_item_ext_ies_container = protocol_ext_container_empty_l;

// PosSRSResourceSet-Aggregation-Item ::= SEQUENCE
struct pos_srs_res_set_aggregation_item_s {
  bool                                               ie_exts_present = false;
  combined_pos_srs_res_set_list_l                    combined_pos_srs_res_set_list;
  pos_srs_res_set_aggregation_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSResourceSet-Aggregation-List ::= SEQUENCE (SIZE (1..32)) OF PosSRSResourceSet-Aggregation-Item
using pos_srs_res_set_aggregation_list_l = dyn_array<pos_srs_res_set_aggregation_item_s>;

// AbortTransmission-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct abort_tx_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { pos_srs_res_set_aggregation_list, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::pos_srs_res_set_aggregation_list; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    pos_srs_res_set_aggregation_list_l&       pos_srs_res_set_aggregation_list() { return c; }
    const pos_srs_res_set_aggregation_list_l& pos_srs_res_set_aggregation_list() const { return c; }

  private:
    pos_srs_res_set_aggregation_list_l c;
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
    enum options { deactiv_srs_res_set_id, release_all, choice_ext, nulltype } value;

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
  uint8_t& deactiv_srs_res_set_id()
  {
    assert_choice_type(types::deactiv_srs_res_set_id, type_, "AbortTransmission");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<abort_tx_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "AbortTransmission");
    return c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>();
  }
  const uint8_t& deactiv_srs_res_set_id() const
  {
    assert_choice_type(types::deactiv_srs_res_set_id, type_, "AbortTransmission");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<abort_tx_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "AbortTransmission");
    return c.get<protocol_ie_single_container_s<abort_tx_ext_ies_o>>();
  }
  uint8_t&                                            set_deactiv_srs_res_set_id();
  void                                                set_release_all();
  protocol_ie_single_container_s<abort_tx_ext_ies_o>& set_choice_ext();

private:
  types                                                               type_;
  choice_buffer_t<protocol_ie_single_container_s<abort_tx_ext_ies_o>> c;

  void destroy_();
};

// TransmissionCombn8-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TransmissionComb-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// ResourceTypePeriodic-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeSemi-persistent-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeAperiodic-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceType-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// StartRBIndex-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// SRSResource-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResource-List ::= SEQUENCE (SIZE (1..64)) OF SRSResource
using srs_res_list_l = dyn_array<srs_res_s>;

// TransmissionCombPos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// SRSPeriodicity ::= ENUMERATED
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

// ResourceTypePeriodicPos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  srs_periodicity_e                       srs_periodicity;
  uint32_t                                offset = 0;
  res_type_periodic_pos_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeSemi-persistentPos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  srs_periodicity_e                              srs_periodicity;
  uint32_t                                       offset = 0;
  res_type_semi_persistent_pos_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypeAperiodicPos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using res_type_aperiodic_pos_ext_ies_o = protocol_ext_empty_o;

using res_type_aperiodic_pos_ext_ies_container = protocol_ext_container_empty_l;

// ResourceTypeAperiodicPos ::= SEQUENCE
struct res_type_aperiodic_pos_s {
  bool                                     ie_exts_present = false;
  uint8_t                                  slot_offset     = 0;
  res_type_aperiodic_pos_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceTypePos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// SSB-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using ssb_ext_ies_o = protocol_ext_empty_o;

using ssb_ext_ies_container = protocol_ext_container_empty_l;

// SSB ::= SEQUENCE
struct ssb_s {
  bool                  ssb_idx_present = false;
  bool                  ie_exts_present = false;
  uint16_t              pci_nr          = 0;
  uint8_t               ssb_idx         = 0;
  ssb_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSInformationPos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialInformationPos-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// SlotOffsetRemainingHopsAperiodic-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// SlotOffsetRemainingHopsSemiPersistent-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// SlotOffsetRemainingHopsSemiPeriodic-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// SlotOffsetRemainingHops-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// SlotOffsetForRemainingHopsItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TxHoppingConfiguration-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// PosSRSResource-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSResource-List ::= SEQUENCE (SIZE (1..64)) OF PosSRSResource-Item
using pos_srs_res_list_l = dyn_array<pos_srs_res_item_s>;

// SRSResourceID-List ::= SEQUENCE (SIZE (1..16)) OF INTEGER (0..63)
using srs_res_id_list_l = bounded_array<uint8_t, 16>;

// ResourceSetTypePeriodic-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceSetTypeSemi-persistent-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceSetTypeAperiodic-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using res_set_type_aperiodic_ext_ies_o = protocol_ext_empty_o;

using res_set_type_aperiodic_ext_ies_container = protocol_ext_container_empty_l;

// ResourceSetTypeAperiodic ::= SEQUENCE
struct res_set_type_aperiodic_s {
  bool                                     ie_exts_present = false;
  uint8_t                                  srs_res_trigger = 1;
  uint8_t                                  slotoffset      = 0;
  res_set_type_aperiodic_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceSetType-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// SRSResourceSet-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using srs_res_set_ext_ies_o = protocol_ext_empty_o;

using srs_res_set_ext_ies_container = protocol_ext_container_empty_l;

// SRSResourceSet ::= SEQUENCE
struct srs_res_set_s {
  bool                          ie_exts_present = false;
  uint8_t                       srs_res_set_id1 = 0;
  srs_res_id_list_l             srs_res_id_list;
  res_set_type_c                res_set_type;
  srs_res_set_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResourceSet-List ::= SEQUENCE (SIZE (1..16)) OF SRSResourceSet
using srs_res_set_list_l = dyn_array<srs_res_set_s>;

// PosSRSResourceIDPerSet-List ::= SEQUENCE (SIZE (1..16)) OF INTEGER (0..63)
using pos_srs_res_id_per_set_list_l = bounded_array<uint8_t, 16>;

// PosResourceSetTypePeriodic-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pos_res_set_type_periodic_ext_ies_o = protocol_ext_empty_o;

using pos_res_set_type_periodic_ext_ies_container = protocol_ext_container_empty_l;

// PosResourceSetTypePeriodic ::= SEQUENCE
struct pos_res_set_type_periodic_s {
  struct posperiodic_set_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using posperiodic_set_e_ = enumerated<posperiodic_set_opts, true>;

  // member variables
  bool                                        ie_exts_present = false;
  posperiodic_set_e_                          posperiodic_set;
  pos_res_set_type_periodic_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosResourceSetTypeSemi-persistent-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pos_res_set_type_semi_persistent_ext_ies_o = protocol_ext_empty_o;

using pos_res_set_type_semi_persistent_ext_ies_container = protocol_ext_container_empty_l;

// PosResourceSetTypeSemi-persistent ::= SEQUENCE
struct pos_res_set_type_semi_persistent_s {
  struct possemi_persistent_set_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using possemi_persistent_set_e_ = enumerated<possemi_persistent_set_opts, true>;

  // member variables
  bool                                               ie_exts_present = false;
  possemi_persistent_set_e_                          possemi_persistent_set;
  pos_res_set_type_semi_persistent_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosResourceSetTypeAperiodic-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pos_res_set_type_aperiodic_ext_ies_o = protocol_ext_empty_o;

using pos_res_set_type_aperiodic_ext_ies_container = protocol_ext_container_empty_l;

// PosResourceSetTypeAperiodic ::= SEQUENCE
struct pos_res_set_type_aperiodic_s {
  bool                                         ie_exts_present = false;
  uint8_t                                      srs_res_trigger = 1;
  pos_res_set_type_aperiodic_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosResourceSetType-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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
  pos_res_set_type_periodic_s& periodic()
  {
    assert_choice_type(types::periodic, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_periodic_s>();
  }
  pos_res_set_type_semi_persistent_s& semi_persistent()
  {
    assert_choice_type(types::semi_persistent, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_semi_persistent_s>();
  }
  pos_res_set_type_aperiodic_s& aperiodic()
  {
    assert_choice_type(types::aperiodic, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_aperiodic_s>();
  }
  protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PosResourceSetType");
    return c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>();
  }
  const pos_res_set_type_periodic_s& periodic() const
  {
    assert_choice_type(types::periodic, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_periodic_s>();
  }
  const pos_res_set_type_semi_persistent_s& semi_persistent() const
  {
    assert_choice_type(types::semi_persistent, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_semi_persistent_s>();
  }
  const pos_res_set_type_aperiodic_s& aperiodic() const
  {
    assert_choice_type(types::aperiodic, type_, "PosResourceSetType");
    return c.get<pos_res_set_type_aperiodic_s>();
  }
  const protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PosResourceSetType");
    return c.get<protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>();
  }
  pos_res_set_type_periodic_s&                                set_periodic();
  pos_res_set_type_semi_persistent_s&                         set_semi_persistent();
  pos_res_set_type_aperiodic_s&                               set_aperiodic();
  protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<pos_res_set_type_aperiodic_s,
                  pos_res_set_type_periodic_s,
                  pos_res_set_type_semi_persistent_s,
                  protocol_ie_single_container_s<pos_res_set_type_ext_ies_o>>
      c;

  void destroy_();
};

// PosSRSResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pos_srs_res_set_item_ext_ies_o = protocol_ext_empty_o;

using pos_srs_res_set_item_ext_ies_container = protocol_ext_container_empty_l;

// PosSRSResourceSet-Item ::= SEQUENCE
struct pos_srs_res_set_item_s {
  bool                                   ie_exts_present   = false;
  uint8_t                                possrs_res_set_id = 0;
  pos_srs_res_id_per_set_list_l          poss_rs_res_id_per_set_list;
  pos_res_set_type_c                     posres_set_type;
  pos_srs_res_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSResourceSet-List ::= SEQUENCE (SIZE (1..16)) OF PosSRSResourceSet-Item
using pos_srs_res_set_list_l = dyn_array<pos_srs_res_set_item_s>;

// SRSConfig-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using srs_cfg_ext_ies_o = protocol_ext_empty_o;

using srs_cfg_ext_ies_container = protocol_ext_container_empty_l;

// SRSConfig ::= SEQUENCE
struct srs_cfg_s {
  bool                      ie_exts_present = false;
  srs_res_list_l            srs_res_list;
  pos_srs_res_list_l        pos_srs_res_list;
  srs_res_set_list_l        srs_res_set_list;
  pos_srs_res_set_list_l    pos_srs_res_set_list;
  srs_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ActiveULBWP-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RelativePathDelay-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct relative_path_delay_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options {
        report_granularitykminus1_add_path,
        report_granularitykminus2_add_path,
        report_granularitykminus3_add_path,
        report_granularitykminus4_add_path,
        report_granularitykminus5_add_path,
        report_granularitykminus6_add_path,
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
    uint16_t&       report_granularitykminus1_add_path();
    uint16_t&       report_granularitykminus2_add_path();
    uint32_t&       report_granularitykminus3_add_path();
    uint32_t&       report_granularitykminus4_add_path();
    uint32_t&       report_granularitykminus5_add_path();
    uint32_t&       report_granularitykminus6_add_path();
    const uint16_t& report_granularitykminus1_add_path() const;
    const uint16_t& report_granularitykminus2_add_path() const;
    const uint32_t& report_granularitykminus3_add_path() const;
    const uint32_t& report_granularitykminus4_add_path() const;
    const uint32_t& report_granularitykminus5_add_path() const;
    const uint32_t& report_granularitykminus6_add_path() const;

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

// TrpMeasurementTimingQuality-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trp_meas_timing_quality_ext_ies_o = protocol_ext_empty_o;

using trp_meas_timing_quality_ext_ies_container = protocol_ext_container_empty_l;

// TrpMeasurementTimingQuality ::= SEQUENCE
struct trp_meas_timing_quality_s {
  struct resolution_opts {
    enum options { m0dot1, m1, m10, m30, /*...*/ nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using resolution_e_ = enumerated<resolution_opts, true>;

  // member variables
  bool                                      ie_exts_present = false;
  uint8_t                                   meas_quality    = 0;
  resolution_e_                             resolution;
  trp_meas_timing_quality_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrpMeasurementAngleQuality-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trp_meas_angle_quality_ext_ies_o = protocol_ext_empty_o;

using trp_meas_angle_quality_ext_ies_container = protocol_ext_container_empty_l;

// TrpMeasurementAngleQuality ::= SEQUENCE
struct trp_meas_angle_quality_s {
  struct resolution_opts {
    enum options { deg0dot1, /*...*/ nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using resolution_e_ = enumerated<resolution_opts, true>;

  // member variables
  bool                                     zenith_quality_present = false;
  bool                                     ie_exts_present        = false;
  uint16_t                                 azimuth_quality        = 0;
  uint16_t                                 zenith_quality         = 0;
  resolution_e_                            resolution;
  trp_meas_angle_quality_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPPhaseQuality-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trp_phase_quality_ext_ies_o = protocol_ext_empty_o;

using trp_phase_quality_ext_ies_container = protocol_ext_container_empty_l;

// TRPPhaseQuality ::= SEQUENCE
struct trp_phase_quality_s {
  struct resolution_opts {
    enum options { deg0dot1, deg1, /*...*/ nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using resolution_e_ = enumerated<resolution_opts, true>;

  // member variables
  bool                                ie_exts_present   = false;
  uint8_t                             phase_quality_idx = 0;
  resolution_e_                       resolution;
  trp_phase_quality_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrpMeasurementQuality-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct trp_meas_quality_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { trp_phase_quality, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::trp_phase_quality; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    trp_phase_quality_s&       trp_phase_quality() { return c; }
    const trp_phase_quality_s& trp_phase_quality() const { return c; }

  private:
    trp_phase_quality_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// TrpMeasurementQuality ::= CHOICE
struct trp_meas_quality_c {
  struct types_opts {
    enum options { timing_meas_quality, angle_meas_quality, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  trp_meas_quality_c() = default;
  trp_meas_quality_c(const trp_meas_quality_c& other);
  trp_meas_quality_c& operator=(const trp_meas_quality_c& other);
  ~trp_meas_quality_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  trp_meas_timing_quality_s& timing_meas_quality()
  {
    assert_choice_type(types::timing_meas_quality, type_, "TrpMeasurementQuality");
    return c.get<trp_meas_timing_quality_s>();
  }
  trp_meas_angle_quality_s& angle_meas_quality()
  {
    assert_choice_type(types::angle_meas_quality, type_, "TrpMeasurementQuality");
    return c.get<trp_meas_angle_quality_s>();
  }
  protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TrpMeasurementQuality");
    return c.get<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>();
  }
  const trp_meas_timing_quality_s& timing_meas_quality() const
  {
    assert_choice_type(types::timing_meas_quality, type_, "TrpMeasurementQuality");
    return c.get<trp_meas_timing_quality_s>();
  }
  const trp_meas_angle_quality_s& angle_meas_quality() const
  {
    assert_choice_type(types::angle_meas_quality, type_, "TrpMeasurementQuality");
    return c.get<trp_meas_angle_quality_s>();
  }
  const protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TrpMeasurementQuality");
    return c.get<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>>();
  }
  trp_meas_timing_quality_s&                                  set_timing_meas_quality();
  trp_meas_angle_quality_s&                                   set_angle_meas_quality();
  protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<trp_meas_quality_ext_ies_o>,
                  trp_meas_angle_quality_s,
                  trp_meas_timing_quality_s>
      c;

  void destroy_();
};

// LCS-to-GCS-Translation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// UL-AoA-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// ZoA-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using zo_a_ext_ies_o = protocol_ext_empty_o;

using zo_a_ext_ies_container = protocol_ext_container_empty_l;

// ZoA ::= SEQUENCE
struct zo_a_s {
  bool                     lcs_to_gcs_translation_present = false;
  bool                     ie_exts_present                = false;
  uint16_t                 zenith_ao_a                    = 0;
  lcs_to_gcs_translation_s lcs_to_gcs_translation;
  zo_a_ext_ies_container   ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MultipleULAoA-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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
  zo_a_s& ul_zo_a()
  {
    assert_choice_type(types::ul_zo_a, type_, "MultipleULAoA-Item");
    return c.get<zo_a_s>();
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
  const zo_a_s& ul_zo_a() const
  {
    assert_choice_type(types::ul_zo_a, type_, "MultipleULAoA-Item");
    return c.get<zo_a_s>();
  }
  const protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MultipleULAoA-Item");
    return c.get<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>>();
  }
  ul_ao_a_s&                                                       set_ul_ao_a();
  zo_a_s&                                                          set_ul_zo_a();
  protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>& set_choice_ext();

private:
  types                                                                                               type_;
  choice_buffer_t<protocol_ie_single_container_s<multiple_ul_ao_a_item_ext_ies_o>, ul_ao_a_s, zo_a_s> c;

  void destroy_();
};

// MultipleULAoA-List ::= SEQUENCE (SIZE (1..8)) OF MultipleULAoA-Item
using multiple_ul_ao_a_list_l = dyn_array<multiple_ul_ao_a_item_c>;

// MultipleULAoA-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// UL-SRS-RSRPP-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// AdditionalPathListItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct add_path_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct add_path_list_item_ext_ies_container {
  bool               multiple_ul_ao_a_present = false;
  bool               path_pwr_present         = false;
  multiple_ul_ao_a_s multiple_ul_ao_a;
  ul_srs_rsrp_p_s    path_pwr;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalPathListItem ::= SEQUENCE
struct add_path_list_item_s {
  bool                                 path_quality_present = false;
  bool                                 ie_exts_present      = false;
  relative_path_delay_c                relative_time_of_path;
  trp_meas_quality_c                   path_quality;
  add_path_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalPathList ::= SEQUENCE (SIZE (1..2)) OF AdditionalPathListItem
using add_path_list_l = dyn_array<add_path_list_item_s>;

// DL-PRS-ResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// AggregatedPRSResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// AggregatedPosSRSResourceID-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// AggregatedPosSRSResourceID-Item ::= SEQUENCE
struct aggr_pos_srs_res_id_item_s {
  bool                                       ie_exts_present = false;
  uint8_t                                    srs_pos_res_id  = 0;
  aggr_pos_srs_res_id_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AggregatedPosSRSResourceID-List ::= SEQUENCE (SIZE (2..3)) OF AggregatedPosSRSResourceID-Item
using aggr_pos_srs_res_id_list_l = dyn_array<aggr_pos_srs_res_id_item_s>;

// Expected-Azimuth-AoA-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// Expected-Zenith-AoA-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// Expected-UL-AoA-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// Expected-ZoA-only-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// AngleMeasurementType-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// AoA-AssistanceInfo-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// BroadcastPeriodicity ::= ENUMERATED
struct broadcast_periodicity_opts {
  enum options { ms80, ms160, ms320, ms640, ms1280, ms2560, ms5120, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using broadcast_periodicity_e = enumerated<broadcast_periodicity_opts, true>;

// PosSIB-Type ::= ENUMERATED
struct pos_sib_type_opts {
  enum options {
    pos_sib_type1_neg1,
    pos_sib_type1_neg2,
    pos_sib_type1_neg3,
    pos_sib_type1_neg4,
    pos_sib_type1_neg5,
    pos_sib_type1_neg6,
    pos_sib_type1_neg7,
    pos_sib_type1_neg8,
    pos_sib_type2_neg1,
    pos_sib_type2_neg2,
    pos_sib_type2_neg3,
    pos_sib_type2_neg4,
    pos_sib_type2_neg5,
    pos_sib_type2_neg6,
    pos_sib_type2_neg7,
    pos_sib_type2_neg8,
    pos_sib_type2_neg9,
    pos_sib_type2_neg10,
    pos_sib_type2_neg11,
    pos_sib_type2_neg12,
    pos_sib_type2_neg13,
    pos_sib_type2_neg14,
    pos_sib_type2_neg15,
    pos_sib_type2_neg16,
    pos_sib_type2_neg17,
    pos_sib_type2_neg18,
    pos_sib_type2_neg19,
    pos_sib_type2_neg20,
    pos_sib_type2_neg21,
    pos_sib_type2_neg22,
    pos_sib_type2_neg23,
    pos_sib_type2_neg24,
    pos_sib_type2_neg25,
    pos_sib_type3_neg1,
    pos_sib_type4_neg1,
    pos_sib_type5_neg1,
    pos_sib_type6_neg1,
    pos_sib_type6_neg2,
    pos_sib_type6_neg3,
    // ...
    pos_sib_type1_neg9,
    pos_sib_type1_neg10,
    pos_sib_type6_neg4,
    pos_sib_type6_neg5,
    pos_sib_type6_neg6,
    pos_sib_type1_neg11,
    pos_sib_type1_neg12,
    pos_sib_type2_neg17a,
    pos_sib_type2_neg18a,
    pos_sib_type2_neg20a,
    pos_sib_type2_neg26,
    pos_sib_type2_neg27,
    pos_sib_type6_neg7,
    pos_sib_type7_neg1,
    pos_sib_type7_neg2,
    pos_sib_type7_neg3,
    pos_sib_type7_neg4,
    nulltype
  } value;

  const char* to_string() const;
};
using pos_sib_type_e = enumerated<pos_sib_type_opts, true, 17>;

// PosSIB-Segments-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pos_sib_segments_ext_ies_o = protocol_ext_empty_o;

using pos_sib_segments_ext_ies_container = protocol_ext_container_empty_l;

struct pos_sib_segments_item_s_ {
  bool                               ie_exts_present = false;
  unbounded_octstring<true>          assist_data_sib_elem;
  pos_sib_segments_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSIB-Segments ::= SEQUENCE (SIZE (1..64)) OF PosSIB-Segments-item
using pos_sib_segments_l = dyn_array<pos_sib_segments_item_s_>;

// AssistanceInformationMetaData-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using assist_info_meta_data_ext_ies_o = protocol_ext_empty_o;

using assist_info_meta_data_ext_ies_container = protocol_ext_container_empty_l;

// AssistanceInformationMetaData ::= SEQUENCE
struct assist_info_meta_data_s {
  struct encrypted_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using encrypted_e_ = enumerated<encrypted_opts, true>;
  struct gns_si_d_opts {
    enum options { gps, sbas, qzss, galileo, glonass, bds, navic, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using gns_si_d_e_ = enumerated<gns_si_d_opts, true>;
  struct sba_si_d_opts {
    enum options { waas, egnos, msas, gagan, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using sba_si_d_e_ = enumerated<sba_si_d_opts, true>;

  // member variables
  bool                                    encrypted_present = false;
  bool                                    gns_si_d_present  = false;
  bool                                    sba_si_d_present  = false;
  bool                                    ie_exts_present   = false;
  encrypted_e_                            encrypted;
  gns_si_d_e_                             gns_si_d;
  sba_si_d_e_                             sba_si_d;
  assist_info_meta_data_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSIBs-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pos_sibs_ext_ies_o = protocol_ext_empty_o;

using pos_sibs_ext_ies_container = protocol_ext_container_empty_l;

struct pos_sibs_item_s_ {
  bool                       assist_info_meta_data_present = false;
  bool                       broadcast_prio_present        = false;
  bool                       ie_exts_present               = false;
  pos_sib_type_e             pos_sib_type;
  pos_sib_segments_l         pos_sib_segments;
  assist_info_meta_data_s    assist_info_meta_data;
  uint8_t                    broadcast_prio = 1;
  pos_sibs_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSIBs ::= SEQUENCE (SIZE (1..32)) OF PosSIBs-item
using pos_sibs_l = dyn_array<pos_sibs_item_s_>;

// SystemInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using sys_info_ext_ies_o = protocol_ext_empty_o;

using sys_info_ext_ies_container = protocol_ext_container_empty_l;

struct sys_info_item_s_ {
  bool                       ie_exts_present = false;
  broadcast_periodicity_e    broadcast_periodicity;
  pos_sibs_l                 pos_sibs;
  sys_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SystemInformation ::= SEQUENCE (SIZE (1..32)) OF SystemInformation-item
using sys_info_l = dyn_array<sys_info_item_s_>;

// Assistance-Information-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using assist_info_ext_ies_o = protocol_ext_empty_o;

using assist_info_ext_ies_container = protocol_ext_container_empty_l;

// Assistance-Information ::= SEQUENCE
struct assist_info_s {
  bool                          ie_exts_present = false;
  sys_info_l                    sys_info;
  assist_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Outcome ::= ENUMERATED
struct outcome_opts {
  enum options { failed, /*...*/ nulltype } value;

  const char* to_string() const;
};
using outcome_e = enumerated<outcome_opts, true>;

// AssistanceInformationFailureList-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using assist_info_fail_list_ext_ies_o = protocol_ext_empty_o;

using assist_info_fail_list_ext_ies_container = protocol_ext_container_empty_l;

struct assist_info_fail_list_item_s_ {
  bool                                    ie_exts_present = false;
  pos_sib_type_e                          pos_sib_type;
  outcome_e                               outcome;
  assist_info_fail_list_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AssistanceInformationFailureList ::= SEQUENCE (SIZE (1..32)) OF AssistanceInformationFailureList-item
using assist_info_fail_list_l = dyn_array<assist_info_fail_list_item_s_>;

// Bandwidth-Aggregation-Request-Indication ::= ENUMERATED
struct bw_aggregation_request_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using bw_aggregation_request_ind_e = enumerated<bw_aggregation_request_ind_opts, true>;

// BandwidthSRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
using bw_srs_ext_ies_o = protocol_ies_empty_o;

// BandwidthSRS ::= CHOICE
struct bw_srs_c {
  struct fr1_opts {
    enum options {
      mhz5,
      mhz10,
      mhz20,
      mhz40,
      mhz50,
      mhz80,
      mhz100,
      // ...
      mhz160,
      mhz200,
      mhz15,
      mhz25,
      mhz30,
      mhz60,
      mhz35,
      mhz45,
      mhz70,
      mhz90,
      nulltype
    } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using fr1_e_ = enumerated<fr1_opts, true, 10>;
  struct fr2_opts {
    enum options { mhz50, mhz100, mhz200, mhz400, /*...*/ mhz600, mhz800, mhz1600, mhz2000, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using fr2_e_ = enumerated<fr2_opts, true, 4>;
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
  fr1_e_& fr1()
  {
    assert_choice_type(types::fr1, type_, "BandwidthSRS");
    return c.get<fr1_e_>();
  }
  fr2_e_& fr2()
  {
    assert_choice_type(types::fr2, type_, "BandwidthSRS");
    return c.get<fr2_e_>();
  }
  protocol_ie_single_container_s<bw_srs_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "BandwidthSRS");
    return c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>();
  }
  const fr1_e_& fr1() const
  {
    assert_choice_type(types::fr1, type_, "BandwidthSRS");
    return c.get<fr1_e_>();
  }
  const fr2_e_& fr2() const
  {
    assert_choice_type(types::fr2, type_, "BandwidthSRS");
    return c.get<fr2_e_>();
  }
  const protocol_ie_single_container_s<bw_srs_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "BandwidthSRS");
    return c.get<protocol_ie_single_container_s<bw_srs_ext_ies_o>>();
  }
  fr1_e_&                                           set_fr1();
  fr2_e_&                                           set_fr2();
  protocol_ie_single_container_s<bw_srs_ext_ies_o>& set_choice_ext();

private:
  types                                                             type_;
  choice_buffer_t<protocol_ie_single_container_s<bw_srs_ext_ies_o>> c;

  void destroy_();
};

// Broadcast ::= ENUMERATED
struct broadcast_opts {
  enum options { start, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using broadcast_e = enumerated<broadcast_opts, true>;

// CGI-EUTRA-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using cgi_eutra_ext_ies_o = protocol_ext_empty_o;

using cgi_eutra_ext_ies_container = protocol_ext_container_empty_l;

// CGI-EUTRA ::= SEQUENCE
struct cgi_eutra_s {
  bool                             ie_exts_present = false;
  fixed_octstring<3, true>         plmn_id;
  fixed_bitstring<28, false, true> eutr_acell_id;
  cgi_eutra_ext_ies_container      ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CGI-NR-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using cgi_nr_ext_ies_o = protocol_ext_empty_o;

using cgi_nr_ext_ies_container = protocol_ext_container_empty_l;

// CGI-NR ::= SEQUENCE
struct cgi_nr_s {
  bool                             ie_exts_present = false;
  fixed_octstring<3, true>         plmn_id;
  fixed_bitstring<36, false, true> nr_cell_id;
  cgi_nr_ext_ies_container         ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPLength-EUTRA ::= ENUMERATED
struct cp_len_eutra_opts {
  enum options { normal, extended, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cp_len_eutra_e = enumerated<cp_len_eutra_opts, true>;

// CarrierFreq-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using carrier_freq_ext_ies_o = protocol_ext_empty_o;

using carrier_freq_ext_ies_container = protocol_ext_container_empty_l;

// CarrierFreq ::= SEQUENCE
struct carrier_freq_s {
  bool                           ie_exts_present   = false;
  uint32_t                       point_a           = 0;
  uint16_t                       offset_to_carrier = 0;
  carrier_freq_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CauseRadioNetwork ::= ENUMERATED
struct cause_radio_network_opts {
  enum options {
    unspecified,
    requested_item_not_supported,
    requested_item_temporarily_not_available,
    // ...
    serving_ng_ran_node_changed,
    requested_item_not_supported_on_time,
    nulltype
  } value;

  const char* to_string() const;
};
using cause_radio_network_e = enumerated<cause_radio_network_opts, true, 2>;

// CauseProtocol ::= ENUMERATED
struct cause_protocol_opts {
  enum options {
    transfer_syntax_error,
    abstract_syntax_error_reject,
    abstract_syntax_error_ignore_and_notify,
    msg_not_compatible_with_receiver_state,
    semantic_error,
    unspecified,
    abstract_syntax_error_falsely_constructed_msg,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using cause_protocol_e = enumerated<cause_protocol_opts, true>;

// CauseMisc ::= ENUMERATED
struct cause_misc_opts {
  enum options { unspecified, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cause_misc_e = enumerated<cause_misc_opts, true>;

// Cause-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
using cause_ext_ie_o = protocol_ies_empty_o;

// Cause ::= CHOICE
struct cause_c {
  struct types_opts {
    enum options { radio_network, protocol, misc, choice_ext, nulltype } value;
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
  protocol_ie_single_container_s<cause_ext_ie_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "Cause");
    return c.get<protocol_ie_single_container_s<cause_ext_ie_o>>();
  }
  const cause_radio_network_e& radio_network() const
  {
    assert_choice_type(types::radio_network, type_, "Cause");
    return c.get<cause_radio_network_e>();
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
  const protocol_ie_single_container_s<cause_ext_ie_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "Cause");
    return c.get<protocol_ie_single_container_s<cause_ext_ie_o>>();
  }
  cause_radio_network_e&                          set_radio_network();
  cause_protocol_e&                               set_protocol();
  cause_misc_e&                                   set_misc();
  protocol_ie_single_container_s<cause_ext_ie_o>& set_choice_ext();

private:
  types                                                           type_;
  choice_buffer_t<protocol_ie_single_container_s<cause_ext_ie_o>> c;

  void destroy_();
};

// TRP-Beam-Power-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TRP-ElevationAngleList-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TRP-BeamAntennaAnglesList-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TRP-BeamAntennaExplicitInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// Choice-TRP-Beam-Info-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// CommonTAParameters-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using common_ta_params_ext_ies_o = protocol_ext_empty_o;

using common_ta_params_ext_ies_container = protocol_ext_container_empty_l;

// CommonTAParameters ::= SEQUENCE
struct common_ta_params_s {
  bool                               ie_exts_present = false;
  unbounded_octstring<true>          epoch_time;
  unbounded_octstring<true>          ta_info;
  common_ta_params_ext_ies_container ie_exts;
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

// CriticalityDiagnostics-IE-List-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using crit_diagnostics_ie_list_ext_ies_o = protocol_ext_empty_o;

using crit_diagnostics_ie_list_ext_ies_container = protocol_ext_container_empty_l;

struct crit_diagnostics_ie_list_item_s_ {
  bool                                       ie_exts_present = false;
  crit_e                                     ie_crit;
  uint32_t                                   ie_id = 0;
  type_of_error_e                            type_of_error;
  crit_diagnostics_ie_list_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CriticalityDiagnostics-IE-List ::= SEQUENCE (SIZE (1..256)) OF CriticalityDiagnostics-IE-List-item
using crit_diagnostics_ie_list_l = dyn_array<crit_diagnostics_ie_list_item_s_>;

// CriticalityDiagnostics-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using crit_diagnostics_ext_ies_o = protocol_ext_empty_o;

using crit_diagnostics_ext_ies_container = protocol_ext_container_empty_l;

// CriticalityDiagnostics ::= SEQUENCE
struct crit_diagnostics_s {
  bool                               proc_code_present           = false;
  bool                               trigger_msg_present         = false;
  bool                               proc_crit_present           = false;
  bool                               nrppatransaction_id_present = false;
  bool                               ie_exts_present             = false;
  uint16_t                           proc_code                   = 0;
  trigger_msg_e                      trigger_msg;
  crit_e                             proc_crit;
  uint16_t                           nrppatransaction_id = 0;
  crit_diagnostics_ie_list_l         ies_crit_diagnostics;
  crit_diagnostics_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DL-Bandwidth-EUTRA ::= ENUMERATED
struct dl_bw_eutra_opts {
  enum options { bw6, bw15, bw25, bw50, bw75, bw100, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using dl_bw_eutra_e = enumerated<dl_bw_eutra_opts, true>;

// DL-PRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DL-PRSBWAggregationRequestInfo-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// DL-PRSMutingPattern-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// DL-PRSResourceARPLocation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// DL-PRSResourceSetARPLocation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// DL-reference-signal-UERxTx-TD ::= ENUMERATED
struct dl_ref_sig_ue_rx_tx_td_opts {
  enum options { csirs, prs, /*...*/ nulltype } value;

  const char* to_string() const;
};
using dl_ref_sig_ue_rx_tx_td_e = enumerated<dl_ref_sig_ue_rx_tx_td_opts, true>;

// DLPRSResource-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using dl_prs_res_item_ext_ies_o = protocol_ext_empty_o;

// DLPRSResourceARP-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using dl_prs_res_arp_ext_ies_o = protocol_ext_empty_o;

using dl_prs_res_arp_ext_ies_container = protocol_ext_container_empty_l;

// DLPRSResourceARP ::= SEQUENCE
struct dl_prs_res_arp_s {
  bool                             ie_exts_present = false;
  uint8_t                          dl_prs_res_id   = 0;
  dl_prs_res_arp_location_c        dl_prs_res_arp_location;
  dl_prs_res_arp_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLPRSResourceSetARP-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLPRSResourceCoordinates-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using dl_prs_res_coordinates_ext_ies_o = protocol_ext_empty_o;

using dl_prs_res_coordinates_ext_ies_container = protocol_ext_container_empty_l;

// DLPRSResourceCoordinates ::= SEQUENCE
struct dl_prs_res_coordinates_s {
  using listof_dl_prs_res_set_arp_l_ = dyn_array<dl_prs_res_set_arp_s>;

  // member variables
  bool                                     ie_exts_present = false;
  listof_dl_prs_res_set_arp_l_             listof_dl_prs_res_set_arp;
  dl_prs_res_coordinates_ext_ies_container ie_exts;
  // ...

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

// NG-RANCell-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
using ng_ran_cell_ext_ie_o = protocol_ies_empty_o;

// NG-RANCell ::= CHOICE
struct ng_ran_cell_c {
  struct types_opts {
    enum options { eutra_cell_id, nr_cell_id, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ng_ran_cell_c() = default;
  ng_ran_cell_c(const ng_ran_cell_c& other);
  ng_ran_cell_c& operator=(const ng_ran_cell_c& other);
  ~ng_ran_cell_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<28, false, true>& eutra_cell_id()
  {
    assert_choice_type(types::eutra_cell_id, type_, "NG-RANCell");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  fixed_bitstring<36, false, true>& nr_cell_id()
  {
    assert_choice_type(types::nr_cell_id, type_, "NG-RANCell");
    return c.get<fixed_bitstring<36, false, true>>();
  }
  protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NG-RANCell");
    return c.get<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>();
  }
  const fixed_bitstring<28, false, true>& eutra_cell_id() const
  {
    assert_choice_type(types::eutra_cell_id, type_, "NG-RANCell");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  const fixed_bitstring<36, false, true>& nr_cell_id() const
  {
    assert_choice_type(types::nr_cell_id, type_, "NG-RANCell");
    return c.get<fixed_bitstring<36, false, true>>();
  }
  const protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NG-RANCell");
    return c.get<protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>>();
  }
  fixed_bitstring<28, false, true>&                     set_eutra_cell_id();
  fixed_bitstring<36, false, true>&                     set_nr_cell_id();
  protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>& set_choice_ext();

private:
  types                                                                                                   type_;
  choice_buffer_t<fixed_bitstring<36, false, true>, protocol_ie_single_container_s<ng_ran_cell_ext_ie_o>> c;

  void destroy_();
};

// NG-RAN-CGI-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using ng_ran_cgi_ext_ies_o = protocol_ext_empty_o;

using ng_ran_cgi_ext_ies_container = protocol_ext_container_empty_l;

// NG-RAN-CGI ::= SEQUENCE
struct ng_ran_cgi_s {
  bool                         ie_exts_present = false;
  fixed_octstring<3, true>     plmn_id;
  ng_ran_cell_c                ng_ran_cell;
  ng_ran_cgi_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NG-RANAccessPointPosition-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using ng_ran_access_point_position_ext_ies_o = protocol_ext_empty_o;

using ng_ran_access_point_position_ext_ies_container = protocol_ext_container_empty_l;

// NG-RANAccessPointPosition ::= SEQUENCE
struct ng_ran_access_point_position_s {
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
  bool                                           ie_exts_present = false;
  latitude_sign_e_                               latitude_sign;
  uint32_t                                       latitude  = 0;
  int32_t                                        longitude = -8388608;
  direction_of_altitude_e_                       direction_of_altitude;
  uint16_t                                       altitude                  = 0;
  uint8_t                                        uncertainty_semi_major    = 0;
  uint8_t                                        uncertainty_semi_minor    = 0;
  uint8_t                                        orientation_of_major_axis = 0;
  uint8_t                                        uncertainty_altitude      = 0;
  uint8_t                                        confidence                = 0;
  ng_ran_access_point_position_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultRSRP-EUTRA-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_rsrp_eutra_item_ext_ies_o = protocol_ext_empty_o;

using result_rsrp_eutra_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultRSRP-EUTRA-Item ::= SEQUENCE
struct result_rsrp_eutra_item_s {
  bool                                     cgi_eutra_present = false;
  bool                                     ie_exts_present   = false;
  uint16_t                                 pci_eutra         = 0;
  uint32_t                                 earfcn            = 0;
  cgi_eutra_s                              cgi_eutra;
  uint16_t                                 value_rsrp_eutra = 0;
  result_rsrp_eutra_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultRSRP-EUTRA ::= SEQUENCE (SIZE (1..9)) OF ResultRSRP-EUTRA-Item
using result_rsrp_eutra_l = dyn_array<result_rsrp_eutra_item_s>;

// ResultRSRQ-EUTRA-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_rsrq_eutra_item_ext_ies_o = protocol_ext_empty_o;

using result_rsrq_eutra_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultRSRQ-EUTRA-Item ::= SEQUENCE
struct result_rsrq_eutra_item_s {
  bool                                     cgi_utra_present = false;
  bool                                     ie_exts_present  = false;
  uint16_t                                 pci_eutra        = 0;
  uint32_t                                 earfcn           = 0;
  cgi_eutra_s                              cgi_utra;
  uint8_t                                  value_rsrq_eutra = 0;
  result_rsrq_eutra_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultRSRQ-EUTRA ::= SEQUENCE (SIZE (1..9)) OF ResultRSRQ-EUTRA-Item
using result_rsrq_eutra_l = dyn_array<result_rsrq_eutra_item_s>;

// ResultSS-RSRP-PerSSB-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_ss_rsrp_per_ssb_item_ext_ies_o = protocol_ext_empty_o;

using result_ss_rsrp_per_ssb_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultSS-RSRP-PerSSB-Item ::= SEQUENCE
struct result_ss_rsrp_per_ssb_item_s {
  bool                                          ie_exts_present = false;
  uint8_t                                       ssb_idx         = 0;
  uint8_t                                       value_ss_rsrp   = 0;
  result_ss_rsrp_per_ssb_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultSS-RSRP-PerSSB ::= SEQUENCE (SIZE (1..64)) OF ResultSS-RSRP-PerSSB-Item
using result_ss_rsrp_per_ssb_l = dyn_array<result_ss_rsrp_per_ssb_item_s>;

// ResultSS-RSRP-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_ss_rsrp_item_ext_ies_o = protocol_ext_empty_o;

using result_ss_rsrp_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultSS-RSRP-Item ::= SEQUENCE
struct result_ss_rsrp_item_s {
  bool                                  cgi_nr_present             = false;
  bool                                  value_ss_rsrp_cell_present = false;
  bool                                  ie_exts_present            = false;
  uint16_t                              nr_pci                     = 0;
  uint32_t                              nr_arfcn                   = 0;
  cgi_nr_s                              cgi_nr;
  uint8_t                               value_ss_rsrp_cell = 0;
  result_ss_rsrp_per_ssb_l              ss_rsrp_per_ssb;
  result_ss_rsrp_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultSS-RSRP ::= SEQUENCE (SIZE (1..9)) OF ResultSS-RSRP-Item
using result_ss_rsrp_l = dyn_array<result_ss_rsrp_item_s>;

// ResultSS-RSRQ-PerSSB-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_ss_rsrq_per_ssb_item_ext_ies_o = protocol_ext_empty_o;

using result_ss_rsrq_per_ssb_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultSS-RSRQ-PerSSB-Item ::= SEQUENCE
struct result_ss_rsrq_per_ssb_item_s {
  bool                                          ie_exts_present = false;
  uint8_t                                       ssb_idx         = 0;
  uint8_t                                       value_ss_rsrq   = 0;
  result_ss_rsrq_per_ssb_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultSS-RSRQ-PerSSB ::= SEQUENCE (SIZE (1..64)) OF ResultSS-RSRQ-PerSSB-Item
using result_ss_rsrq_per_ssb_l = dyn_array<result_ss_rsrq_per_ssb_item_s>;

// ResultSS-RSRQ-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_ss_rsrq_item_ext_ies_o = protocol_ext_empty_o;

using result_ss_rsrq_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultSS-RSRQ-Item ::= SEQUENCE
struct result_ss_rsrq_item_s {
  bool                                  cgi_nr_present             = false;
  bool                                  value_ss_rsrq_cell_present = false;
  bool                                  ie_exts_present            = false;
  uint16_t                              nr_pci                     = 0;
  uint32_t                              nr_arfcn                   = 0;
  cgi_nr_s                              cgi_nr;
  uint8_t                               value_ss_rsrq_cell = 0;
  result_ss_rsrq_per_ssb_l              ss_rsrq_per_ssb;
  result_ss_rsrq_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultSS-RSRQ ::= SEQUENCE (SIZE (1..9)) OF ResultSS-RSRQ-Item
using result_ss_rsrq_l = dyn_array<result_ss_rsrq_item_s>;

// ResultCSI-RSRP-PerCSI-RS-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_csi_rsrp_per_csi_rs_item_ext_ies_o = protocol_ext_empty_o;

using result_csi_rsrp_per_csi_rs_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultCSI-RSRP-PerCSI-RS-Item ::= SEQUENCE
struct result_csi_rsrp_per_csi_rs_item_s {
  bool                                              ie_exts_present = false;
  uint8_t                                           csi_rs_idx      = 0;
  uint8_t                                           value_csi_rsrp  = 0;
  result_csi_rsrp_per_csi_rs_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultCSI-RSRP-PerCSI-RS ::= SEQUENCE (SIZE (1..64)) OF ResultCSI-RSRP-PerCSI-RS-Item
using result_csi_rsrp_per_csi_rs_l = dyn_array<result_csi_rsrp_per_csi_rs_item_s>;

// ResultCSI-RSRP-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_csi_rsrp_item_ext_ies_o = protocol_ext_empty_o;

using result_csi_rsrp_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultCSI-RSRP-Item ::= SEQUENCE
struct result_csi_rsrp_item_s {
  bool                                   cgi_nr_present              = false;
  bool                                   value_csi_rsrp_cell_present = false;
  bool                                   ie_exts_present             = false;
  uint16_t                               nr_pci                      = 0;
  uint32_t                               nr_arfcn                    = 0;
  cgi_nr_s                               cgi_nr;
  uint8_t                                value_csi_rsrp_cell = 0;
  result_csi_rsrp_per_csi_rs_l           csi_rsrp_per_csi_rs;
  result_csi_rsrp_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultCSI-RSRP ::= SEQUENCE (SIZE (1..9)) OF ResultCSI-RSRP-Item
using result_csi_rsrp_l = dyn_array<result_csi_rsrp_item_s>;

// ResultCSI-RSRQ-PerCSI-RS-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_csi_rsrq_per_csi_rs_item_ext_ies_o = protocol_ext_empty_o;

using result_csi_rsrq_per_csi_rs_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultCSI-RSRQ-PerCSI-RS-Item ::= SEQUENCE
struct result_csi_rsrq_per_csi_rs_item_s {
  bool                                              ie_exts_present = false;
  uint8_t                                           csi_rs_idx      = 0;
  uint8_t                                           value_csi_rsrq  = 0;
  result_csi_rsrq_per_csi_rs_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultCSI-RSRQ-PerCSI-RS ::= SEQUENCE (SIZE (1..64)) OF ResultCSI-RSRQ-PerCSI-RS-Item
using result_csi_rsrq_per_csi_rs_l = dyn_array<result_csi_rsrq_per_csi_rs_item_s>;

// ResultCSI-RSRQ-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_csi_rsrq_item_ext_ies_o = protocol_ext_empty_o;

using result_csi_rsrq_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultCSI-RSRQ-Item ::= SEQUENCE
struct result_csi_rsrq_item_s {
  bool                                   cgi_nr_present              = false;
  bool                                   value_csi_rsrq_cell_present = false;
  bool                                   ie_exts_present             = false;
  uint16_t                               nr_pci                      = 0;
  uint32_t                               nr_arfcn                    = 0;
  cgi_nr_s                               cgi_nr;
  uint8_t                                value_csi_rsrq_cell = 0;
  result_csi_rsrq_per_csi_rs_l           csi_rsrq_per_csi_rs;
  result_csi_rsrq_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultCSI-RSRQ ::= SEQUENCE (SIZE (1..9)) OF ResultCSI-RSRQ-Item
using result_csi_rsrq_l = dyn_array<result_csi_rsrq_item_s>;

// MeasuredResultsValue-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct measured_results_value_ext_ie_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options {
        result_ss_rsrp,
        result_ss_rsrq,
        result_csi_rsrp,
        result_csi_rsrq,
        angle_of_arrival_nr,
        nr_tadv,
        ue_rx_tx_time_diff,
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
    result_ss_rsrp_l&        result_ss_rsrp();
    result_ss_rsrq_l&        result_ss_rsrq();
    result_csi_rsrp_l&       result_csi_rsrp();
    result_csi_rsrq_l&       result_csi_rsrq();
    ul_ao_a_s&               angle_of_arrival_nr();
    uint16_t&                nr_tadv();
    uint16_t&                ue_rx_tx_time_diff();
    const result_ss_rsrp_l&  result_ss_rsrp() const;
    const result_ss_rsrq_l&  result_ss_rsrq() const;
    const result_csi_rsrp_l& result_csi_rsrp() const;
    const result_csi_rsrq_l& result_csi_rsrq() const;
    const ul_ao_a_s&         angle_of_arrival_nr() const;
    const uint16_t&          nr_tadv() const;
    const uint16_t&          ue_rx_tx_time_diff() const;

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
    enum options {
      value_angle_of_arrival_eutra,
      value_timing_advance_type1_eutra,
      value_timing_advance_type2_eutra,
      result_rsrp_eutra,
      result_rsrq_eutra,
      choice_ext,
      nulltype
    } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
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
  uint16_t& value_angle_of_arrival_eutra()
  {
    assert_choice_type(types::value_angle_of_arrival_eutra, type_, "MeasuredResultsValue");
    return c.get<uint16_t>();
  }
  uint16_t& value_timing_advance_type1_eutra()
  {
    assert_choice_type(types::value_timing_advance_type1_eutra, type_, "MeasuredResultsValue");
    return c.get<uint16_t>();
  }
  uint16_t& value_timing_advance_type2_eutra()
  {
    assert_choice_type(types::value_timing_advance_type2_eutra, type_, "MeasuredResultsValue");
    return c.get<uint16_t>();
  }
  result_rsrp_eutra_l& result_rsrp_eutra()
  {
    assert_choice_type(types::result_rsrp_eutra, type_, "MeasuredResultsValue");
    return c.get<result_rsrp_eutra_l>();
  }
  result_rsrq_eutra_l& result_rsrq_eutra()
  {
    assert_choice_type(types::result_rsrq_eutra, type_, "MeasuredResultsValue");
    return c.get<result_rsrq_eutra_l>();
  }
  protocol_ie_single_container_s<measured_results_value_ext_ie_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MeasuredResultsValue");
    return c.get<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>();
  }
  const uint16_t& value_angle_of_arrival_eutra() const
  {
    assert_choice_type(types::value_angle_of_arrival_eutra, type_, "MeasuredResultsValue");
    return c.get<uint16_t>();
  }
  const uint16_t& value_timing_advance_type1_eutra() const
  {
    assert_choice_type(types::value_timing_advance_type1_eutra, type_, "MeasuredResultsValue");
    return c.get<uint16_t>();
  }
  const uint16_t& value_timing_advance_type2_eutra() const
  {
    assert_choice_type(types::value_timing_advance_type2_eutra, type_, "MeasuredResultsValue");
    return c.get<uint16_t>();
  }
  const result_rsrp_eutra_l& result_rsrp_eutra() const
  {
    assert_choice_type(types::result_rsrp_eutra, type_, "MeasuredResultsValue");
    return c.get<result_rsrp_eutra_l>();
  }
  const result_rsrq_eutra_l& result_rsrq_eutra() const
  {
    assert_choice_type(types::result_rsrq_eutra, type_, "MeasuredResultsValue");
    return c.get<result_rsrq_eutra_l>();
  }
  const protocol_ie_single_container_s<measured_results_value_ext_ie_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MeasuredResultsValue");
    return c.get<protocol_ie_single_container_s<measured_results_value_ext_ie_o>>();
  }
  uint16_t&                                                        set_value_angle_of_arrival_eutra();
  uint16_t&                                                        set_value_timing_advance_type1_eutra();
  uint16_t&                                                        set_value_timing_advance_type2_eutra();
  result_rsrp_eutra_l&                                             set_result_rsrp_eutra();
  result_rsrq_eutra_l&                                             set_result_rsrq_eutra();
  protocol_ie_single_container_s<measured_results_value_ext_ie_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<measured_results_value_ext_ie_o>,
                  result_rsrp_eutra_l,
                  result_rsrq_eutra_l>
      c;

  void destroy_();
};

// MeasuredResults ::= SEQUENCE (SIZE (1..64)) OF MeasuredResultsValue
using measured_results_l = dyn_array<measured_results_value_c>;

// NGRANHighAccuracyAccessPointPosition-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPPositionDirectAccuracy-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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
  ng_ran_access_point_position_s& trp_position()
  {
    assert_choice_type(types::trp_position, type_, "TRPPositionDirectAccuracy");
    return c.get<ng_ran_access_point_position_s>();
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
  const ng_ran_access_point_position_s& trp_position() const
  {
    assert_choice_type(types::trp_position, type_, "TRPPositionDirectAccuracy");
    return c.get<ng_ran_access_point_position_s>();
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
  ng_ran_access_point_position_s&                                         set_trp_position();
  ngran_high_accuracy_access_point_position_s&                            set_trph_aposition();
  protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<ng_ran_access_point_position_s,
                  ngran_high_accuracy_access_point_position_s,
                  protocol_ie_single_container_s<trp_position_direct_accuracy_ext_ies_o>>
      c;

  void destroy_();
};

// TRPPositionDirect-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trp_position_direct_ext_ies_o = protocol_ext_empty_o;

using trp_position_direct_ext_ies_container = protocol_ext_container_empty_l;

// TRPPositionDirect ::= SEQUENCE
struct trp_position_direct_s {
  bool                                  ie_exts_present = false;
  trp_position_direct_accuracy_c        accuracy;
  trp_position_direct_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LocalOrigin-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// ReferencePoint-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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
    enum options { relative_coordinate_id, ref_point_coordinate, ref_point_coordinate_ha, choice_ext, nulltype } value;

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
  uint16_t& relative_coordinate_id()
  {
    assert_choice_type(types::relative_coordinate_id, type_, "ReferencePoint");
    return c.get<uint16_t>();
  }
  ng_ran_access_point_position_s& ref_point_coordinate()
  {
    assert_choice_type(types::ref_point_coordinate, type_, "ReferencePoint");
    return c.get<ng_ran_access_point_position_s>();
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
  const uint16_t& relative_coordinate_id() const
  {
    assert_choice_type(types::relative_coordinate_id, type_, "ReferencePoint");
    return c.get<uint16_t>();
  }
  const ng_ran_access_point_position_s& ref_point_coordinate() const
  {
    assert_choice_type(types::ref_point_coordinate, type_, "ReferencePoint");
    return c.get<ng_ran_access_point_position_s>();
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
  uint16_t&                                            set_relative_coordinate_id();
  ng_ran_access_point_position_s&                      set_ref_point_coordinate();
  ngran_high_accuracy_access_point_position_s&         set_ref_point_coordinate_ha();
  protocol_ie_single_container_s<ref_point_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<ng_ran_access_point_position_s,
                  ngran_high_accuracy_access_point_position_s,
                  protocol_ie_single_container_s<ref_point_ext_ies_o>>
      c;

  void destroy_();
};

// TRPReferencePointType-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// TRPPositionReferenced-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trp_position_refd_ext_ies_o = protocol_ext_empty_o;

using trp_position_refd_ext_ies_container = protocol_ext_container_empty_l;

// TRPPositionReferenced ::= SEQUENCE
struct trp_position_refd_s {
  bool                                ie_exts_present = false;
  ref_point_c                         ref_point;
  trp_ref_point_type_c                ref_point_type;
  trp_position_refd_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPPositionDefinitionType-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// GeographicalCoordinates-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeStampSlotIndex-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// TimeStamp-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Mobile-TRP-LocationInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// MeasuredResultsAssociatedInfoItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct measured_results_associated_info_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct measured_results_associated_info_item_ext_ies_container {
  bool                     dl_ref_sig_ue_rx_tx_td_present = false;
  dl_ref_sig_ue_rx_tx_td_e dl_ref_sig_ue_rx_tx_td;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasuredResultsAssociatedInfoItem ::= SEQUENCE
struct measured_results_associated_info_item_s {
  bool                                                    time_stamp_present   = false;
  bool                                                    meas_quality_present = false;
  bool                                                    ie_exts_present      = false;
  time_stamp_s                                            time_stamp;
  trp_meas_quality_c                                      meas_quality;
  measured_results_associated_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasuredResultsAssociatedInfoList ::= SEQUENCE (SIZE (1..64)) OF MeasuredResultsAssociatedInfoItem
using measured_results_associated_info_list_l = dyn_array<measured_results_associated_info_item_s>;

// E-CID-MeasurementResult-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct e_c_id_meas_result_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_c_id_meas_result_ext_ies_container {
  bool                                    geographical_coordinates_present              = false;
  bool                                    mobile_access_point_location_present          = false;
  bool                                    measured_results_associated_info_list_present = false;
  geographical_coordinates_s              geographical_coordinates;
  mobile_trp_location_info_s              mobile_access_point_location;
  measured_results_associated_info_list_l measured_results_associated_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CID-MeasurementResult ::= SEQUENCE
struct e_c_id_meas_result_s {
  bool                                 ng_ran_access_point_position_present = false;
  bool                                 ie_exts_present                      = false;
  ng_ran_cgi_s                         serving_cell_id;
  fixed_octstring<3, true>             serving_cell_tac;
  ng_ran_access_point_position_s       ng_ran_access_point_position;
  measured_results_l                   measured_results;
  e_c_id_meas_result_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExtendedAdditionalPathList-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using extended_add_path_list_item_ext_ies_o = protocol_ext_empty_o;

using extended_add_path_list_item_ext_ies_container = protocol_ext_container_empty_l;

// ExtendedAdditionalPathList-Item ::= SEQUENCE
struct extended_add_path_list_item_s {
  bool                                          path_quality_present     = false;
  bool                                          multiple_ul_ao_a_present = false;
  bool                                          path_pwr_present         = false;
  bool                                          ie_exts_present          = false;
  relative_path_delay_c                         relative_time_of_path;
  trp_meas_quality_c                            path_quality;
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

// GNBRxTxTimeDiffMeas-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// TRP-RxTx-TEGInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
    tc0,
    tc2,
    tc4,
    tc6,
    tc8,
    tc12,
    tc16,
    tc20,
    tc24,
    tc32,
    tc40,
    tc48,
    tc56,
    tc64,
    tc72,
    tc80,
    /*...*/ nulltype
  } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using timing_error_margin_e = enumerated<timing_error_margin_opts, true>;

// TRP-Tx-TEGInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// RxTxTEG-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TRP-Rx-TEGInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// RxTEG-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TRPTEGInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// GNB-RxTxTimeDiff-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LCS-to-GCS-TranslationItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using lcs_to_gcs_translation_item_ext_ies_o = protocol_ext_empty_o;

using lcs_to_gcs_translation_item_ext_ies_container = protocol_ext_container_empty_l;

// LCS-to-GCS-TranslationItem ::= SEQUENCE
struct lcs_to_gcs_translation_item_s {
  bool                                          alpha_fine_present = false;
  bool                                          beta_fine_present  = false;
  bool                                          gamma_fine_present = false;
  bool                                          ie_exts_present    = false;
  uint16_t                                      alpha              = 0;
  uint8_t                                       alpha_fine         = 0;
  uint16_t                                      beta               = 0;
  uint8_t                                       beta_fine          = 0;
  uint16_t                                      gamma              = 0;
  uint8_t                                       gamma_fine         = 0;
  lcs_to_gcs_translation_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LoS-NLoSIndicatorHard ::= ENUMERATED
struct lo_s_n_lo_si_ndicator_hard_opts {
  enum options { nlos, los, nulltype } value;

  const char* to_string() const;
};
using lo_s_n_lo_si_ndicator_hard_e = enumerated<lo_s_n_lo_si_ndicator_hard_opts>;

// LoS-NLoSInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// MeasurementAmount ::= ENUMERATED
struct meas_amount_opts {
  enum options { ma0, ma1, ma2, ma4, ma8, ma16, ma32, ma64, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using meas_amount_e = enumerated<meas_amount_opts>;

// MeasurementBeamInfo-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

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
    min60,
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

// MeasurementPeriodicityNR-AoA ::= ENUMERATED
struct meas_periodicity_nr_ao_a_opts {
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
using meas_periodicity_nr_ao_a_e = enumerated<meas_periodicity_nr_ao_a_opts, true>;

// MeasurementQuantitiesValue ::= ENUMERATED
struct meas_quantities_value_opts {
  enum options {
    cell_id,
    angle_of_arrival,
    timing_advance_type1,
    timing_advance_type2,
    rsrp,
    rsrq,
    // ...
    ss_rsrp,
    ss_rsrq,
    csi_rsrp,
    csi_rsrq,
    angle_of_arrival_nr,
    timing_advance_nr,
    ue_rx_tx_time_diff,
    nulltype
  } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using meas_quantities_value_e = enumerated<meas_quantities_value_opts, true, 7>;

// MeasurementQuantitiesValue-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using meas_quantities_value_ext_ies_o = protocol_ext_empty_o;

using meas_quantities_value_ext_ies_container = protocol_ext_container_empty_l;

// MeasurementQuantities-Item ::= SEQUENCE
struct meas_quantities_item_s {
  bool                                    ie_exts_present = false;
  meas_quantities_value_e                 meas_quantities_value;
  meas_quantities_value_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementQuantities-ItemIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_quantities_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { meas_quantities_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::meas_quantities_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    meas_quantities_item_s&       meas_quantities_item() { return c; }
    const meas_quantities_item_s& meas_quantities_item() const { return c; }

  private:
    meas_quantities_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// MeasurementQuantities ::= SEQUENCE (SIZE (1..64)) OF ProtocolIE-Field{NRPPA-PROTOCOL-IES : IEsSetParam}
using meas_quantities_l = dyn_array<protocol_ie_single_container_s<meas_quantities_item_ies_o>>;

// MeasurementTimeOccasion ::= ENUMERATED
struct meas_time_occasion_opts {
  enum options { o1, o4, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using meas_time_occasion_e = enumerated<meas_time_occasion_opts, true>;

// PRSAngleItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct prs_angle_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct prs_angle_item_ext_ies_container {
  bool    prs_res_id_present = false;
  uint8_t prs_res_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSAngleItem ::= SEQUENCE
struct prs_angle_item_s {
  bool                             nr_prs_azimuth_fine_present   = false;
  bool                             nr_prs_elevation_present      = false;
  bool                             nr_prs_elevation_fine_present = false;
  bool                             ie_exts_present               = false;
  uint16_t                         nr_prs_azimuth                = 0;
  uint8_t                          nr_prs_azimuth_fine           = 0;
  uint8_t                          nr_prs_elevation              = 0;
  uint8_t                          nr_prs_elevation_fine         = 0;
  prs_angle_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-PRS-Beam-InformationItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using nr_prs_beam_info_item_ext_ies_o = protocol_ext_empty_o;

using nr_prs_beam_info_item_ext_ies_container = protocol_ext_container_empty_l;

// NR-PRS-Beam-InformationItem ::= SEQUENCE
struct nr_prs_beam_info_item_s {
  using prs_angle_l_ = dyn_array<prs_angle_item_s>;

  // member variables
  bool                                    ie_exts_present = false;
  uint8_t                                 pr_sres_set_id  = 0;
  prs_angle_l_                            prs_angle;
  nr_prs_beam_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-PRS-Beam-Information-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using nr_prs_beam_info_ies_o = protocol_ext_empty_o;

using nr_prs_beam_info_ies_container = protocol_ext_container_empty_l;

// NR-PRS-Beam-Information ::= SEQUENCE
struct nr_prs_beam_info_s {
  using nr_prs_beam_info_list_l_       = dyn_array<nr_prs_beam_info_item_s>;
  using lcs_to_gcs_translation_list_l_ = dyn_array<lcs_to_gcs_translation_item_s>;

  // member variables
  bool                           ie_exts_present = false;
  nr_prs_beam_info_list_l_       nr_prs_beam_info_list;
  lcs_to_gcs_translation_list_l_ lcs_to_gcs_translation_list;
  nr_prs_beam_info_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NumberOfAntennaPorts-EUTRA ::= ENUMERATED
struct nof_ant_ports_eutra_opts {
  enum options { n1_or_n2, n4, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nof_ant_ports_eutra_e = enumerated<nof_ant_ports_eutra_opts, true>;

// NumberOfDlFrames-EUTRA ::= ENUMERATED
struct nof_dl_frames_eutra_opts {
  enum options { sf1, sf2, sf4, sf6, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nof_dl_frames_eutra_e = enumerated<nof_dl_frames_eutra_opts, true>;

// NumberOfFrequencyHoppingBands ::= ENUMERATED
struct nof_freq_hop_bands_opts {
  enum options { twobands, fourbands, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nof_freq_hop_bands_e = enumerated<nof_freq_hop_bands_opts, true>;

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
  enum options { two, three, four, six, eight, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nof_trp_rx_tx_teg_e = enumerated<nof_trp_rx_tx_teg_opts, true>;

// OTDOA-Information-Item ::= ENUMERATED
struct otdoa_info_item_opts {
  enum options {
    pci,
    cgi,
    tac,
    earfcn,
    prs_bw,
    prs_cfg_idx,
    cp_len,
    no_dl_frames,
    no_ant_ports,
    sfn_init_time,
    ng_ran_access_point_position,
    prsmutingcfg,
    prsid,
    tpid,
    tp_type,
    crs_cp_len,
    dl_bw,
    multipleprs_cfgsper_cell,
    prs_occasion_group,
    prs_freq_hop_cfg,
    // ...
    tdd_cfg,
    nulltype
  } value;

  const char* to_string() const;
};
using otdoa_info_item_e = enumerated<otdoa_info_item_opts, true, 1>;

// PRS-Bandwidth-EUTRA ::= ENUMERATED
struct prs_bw_eutra_opts {
  enum options { bw6, bw15, bw25, bw50, bw75, bw100, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using prs_bw_eutra_e = enumerated<prs_bw_eutra_opts, true>;

// PRSMutingConfiguration-EUTRA-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
using prs_muting_cfg_eutra_ext_ie_o = protocol_ies_empty_o;

// PRSMutingConfiguration-EUTRA ::= CHOICE
struct prs_muting_cfg_eutra_c {
  struct types_opts {
    enum options {
      two,
      four,
      eight,
      sixteen,
      thirty_two,
      sixty_four,
      one_hundred_and_twenty_eight,
      two_hundred_and_fifty_six,
      five_hundred_and_twelve,
      one_thousand_and_twenty_four,
      choice_ext,
      nulltype
    } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  prs_muting_cfg_eutra_c() = default;
  prs_muting_cfg_eutra_c(const prs_muting_cfg_eutra_c& other);
  prs_muting_cfg_eutra_c& operator=(const prs_muting_cfg_eutra_c& other);
  ~prs_muting_cfg_eutra_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<2, false, true>& two()
  {
    assert_choice_type(types::two, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<2, false, true>>();
  }
  fixed_bitstring<4, false, true>& four()
  {
    assert_choice_type(types::four, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  fixed_bitstring<8, false, true>& eight()
  {
    assert_choice_type(types::eight, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  fixed_bitstring<16, false, true>& sixteen()
  {
    assert_choice_type(types::sixteen, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<16, false, true>>();
  }
  fixed_bitstring<32, false, true>& thirty_two()
  {
    assert_choice_type(types::thirty_two, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<32, false, true>>();
  }
  fixed_bitstring<64, false, true>& sixty_four()
  {
    assert_choice_type(types::sixty_four, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  fixed_bitstring<128, false, true>& one_hundred_and_twenty_eight()
  {
    assert_choice_type(types::one_hundred_and_twenty_eight, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<128, false, true>>();
  }
  fixed_bitstring<256, false, true>& two_hundred_and_fifty_six()
  {
    assert_choice_type(types::two_hundred_and_fifty_six, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<256, false, true>>();
  }
  fixed_bitstring<512, false, true>& five_hundred_and_twelve()
  {
    assert_choice_type(types::five_hundred_and_twelve, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<512, false, true>>();
  }
  fixed_bitstring<1024, false, true>& one_thousand_and_twenty_four()
  {
    assert_choice_type(types::one_thousand_and_twenty_four, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<1024, false, true>>();
  }
  protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>();
  }
  const fixed_bitstring<2, false, true>& two() const
  {
    assert_choice_type(types::two, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<2, false, true>>();
  }
  const fixed_bitstring<4, false, true>& four() const
  {
    assert_choice_type(types::four, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  const fixed_bitstring<8, false, true>& eight() const
  {
    assert_choice_type(types::eight, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  const fixed_bitstring<16, false, true>& sixteen() const
  {
    assert_choice_type(types::sixteen, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<16, false, true>>();
  }
  const fixed_bitstring<32, false, true>& thirty_two() const
  {
    assert_choice_type(types::thirty_two, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<32, false, true>>();
  }
  const fixed_bitstring<64, false, true>& sixty_four() const
  {
    assert_choice_type(types::sixty_four, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  const fixed_bitstring<128, false, true>& one_hundred_and_twenty_eight() const
  {
    assert_choice_type(types::one_hundred_and_twenty_eight, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<128, false, true>>();
  }
  const fixed_bitstring<256, false, true>& two_hundred_and_fifty_six() const
  {
    assert_choice_type(types::two_hundred_and_fifty_six, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<256, false, true>>();
  }
  const fixed_bitstring<512, false, true>& five_hundred_and_twelve() const
  {
    assert_choice_type(types::five_hundred_and_twelve, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<512, false, true>>();
  }
  const fixed_bitstring<1024, false, true>& one_thousand_and_twenty_four() const
  {
    assert_choice_type(types::one_thousand_and_twenty_four, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<fixed_bitstring<1024, false, true>>();
  }
  const protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PRSMutingConfiguration-EUTRA");
    return c.get<protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>>();
  }
  fixed_bitstring<2, false, true>&                               set_two();
  fixed_bitstring<4, false, true>&                               set_four();
  fixed_bitstring<8, false, true>&                               set_eight();
  fixed_bitstring<16, false, true>&                              set_sixteen();
  fixed_bitstring<32, false, true>&                              set_thirty_two();
  fixed_bitstring<64, false, true>&                              set_sixty_four();
  fixed_bitstring<128, false, true>&                             set_one_hundred_and_twenty_eight();
  fixed_bitstring<256, false, true>&                             set_two_hundred_and_fifty_six();
  fixed_bitstring<512, false, true>&                             set_five_hundred_and_twelve();
  fixed_bitstring<1024, false, true>&                            set_one_thousand_and_twenty_four();
  protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<1024, false, true>, protocol_ie_single_container_s<prs_muting_cfg_eutra_ext_ie_o>> c;

  void destroy_();
};

// TP-Type-EUTRA ::= ENUMERATED
struct tp_type_eutra_opts {
  enum options { prs_only_tp, /*...*/ nulltype } value;

  const char* to_string() const;
};
using tp_type_eutra_e = enumerated<tp_type_eutra_opts, true>;

// PRSOccasionGroup-EUTRA ::= ENUMERATED
struct prs_occasion_group_eutra_opts {
  enum options { og2, og4, og8, og16, og32, og64, og128, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using prs_occasion_group_eutra_e = enumerated<prs_occasion_group_eutra_opts, true>;

// PRSFrequencyHoppingConfiguration-EUTRA-Item-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using prs_freq_hop_cfg_eutra_item_ies_o = protocol_ext_empty_o;

using prs_freq_hop_cfg_eutra_item_ies_container = protocol_ext_container_empty_l;

// PRSFrequencyHoppingConfiguration-EUTRA ::= SEQUENCE
struct prs_freq_hop_cfg_eutra_s {
  using band_positions_l_ = bounded_array<uint8_t, 7>;

  // member variables
  bool                                      ie_exts_present = false;
  nof_freq_hop_bands_e                      no_of_freq_hop_bands;
  band_positions_l_                         band_positions;
  prs_freq_hop_cfg_eutra_item_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TDD-Config-EUTRA-Item-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using tdd_cfg_eutra_item_item_ext_ies_o = protocol_ext_empty_o;

using tdd_cfg_eutra_item_item_ext_ies_container = protocol_ext_container_empty_l;

// TDD-Config-EUTRA-Item ::= SEQUENCE
struct tdd_cfg_eutra_item_s {
  struct sf_assign_opts {
    enum options { sa0, sa1, sa2, sa3, sa4, sa5, sa6, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sf_assign_e_ = enumerated<sf_assign_opts, true>;

  // member variables
  bool                                      ie_exts_present = false;
  sf_assign_e_                              sf_assign;
  tdd_cfg_eutra_item_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// OTDOACell-Information-Item-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct otdoa_cell_info_item_ext_ie_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { tdd_cfg_eutra_item, cgi_nr, sfn_initisation_time_nr, nulltype } value;

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
    tdd_cfg_eutra_item_s&                   tdd_cfg_eutra_item();
    cgi_nr_s&                               cgi_nr();
    fixed_bitstring<64, false, true>&       sfn_initisation_time_nr();
    const tdd_cfg_eutra_item_s&             tdd_cfg_eutra_item() const;
    const cgi_nr_s&                         cgi_nr() const;
    const fixed_bitstring<64, false, true>& sfn_initisation_time_nr() const;

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

// OTDOACell-Information-Item ::= CHOICE
struct otdoa_cell_info_item_c {
  struct types_opts {
    enum options {
      pci_eutra,
      cgi_eutra,
      tac,
      earfcn,
      prs_bw_eutra,
      prs_cfg_idx_eutra,
      cp_len_eutra,
      nof_dl_frames_eutra,
      nof_ant_ports_eutra,
      sfn_initisation_time_eutra,
      ng_ran_access_point_position,
      prs_muting_cfg_eutra,
      prsid_eutra,
      tpid_eutra,
      tp_type_eutra,
      nof_dl_frames_extended_eutra,
      crs_cp_len_eutra,
      dl_bw_eutra,
      prs_occasion_group_eutra,
      prs_freq_hop_cfg_eutra,
      choice_ext,
      nulltype
    } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  otdoa_cell_info_item_c() = default;
  otdoa_cell_info_item_c(const otdoa_cell_info_item_c& other);
  otdoa_cell_info_item_c& operator=(const otdoa_cell_info_item_c& other);
  ~otdoa_cell_info_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint16_t& pci_eutra()
  {
    assert_choice_type(types::pci_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  cgi_eutra_s& cgi_eutra()
  {
    assert_choice_type(types::cgi_eutra, type_, "OTDOACell-Information-Item");
    return c.get<cgi_eutra_s>();
  }
  fixed_octstring<3, true>& tac()
  {
    assert_choice_type(types::tac, type_, "OTDOACell-Information-Item");
    return c.get<fixed_octstring<3, true>>();
  }
  uint32_t& earfcn()
  {
    assert_choice_type(types::earfcn, type_, "OTDOACell-Information-Item");
    return c.get<uint32_t>();
  }
  prs_bw_eutra_e& prs_bw_eutra()
  {
    assert_choice_type(types::prs_bw_eutra, type_, "OTDOACell-Information-Item");
    return c.get<prs_bw_eutra_e>();
  }
  uint16_t& prs_cfg_idx_eutra()
  {
    assert_choice_type(types::prs_cfg_idx_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  cp_len_eutra_e& cp_len_eutra()
  {
    assert_choice_type(types::cp_len_eutra, type_, "OTDOACell-Information-Item");
    return c.get<cp_len_eutra_e>();
  }
  nof_dl_frames_eutra_e& nof_dl_frames_eutra()
  {
    assert_choice_type(types::nof_dl_frames_eutra, type_, "OTDOACell-Information-Item");
    return c.get<nof_dl_frames_eutra_e>();
  }
  nof_ant_ports_eutra_e& nof_ant_ports_eutra()
  {
    assert_choice_type(types::nof_ant_ports_eutra, type_, "OTDOACell-Information-Item");
    return c.get<nof_ant_ports_eutra_e>();
  }
  fixed_bitstring<64, false, true>& sfn_initisation_time_eutra()
  {
    assert_choice_type(types::sfn_initisation_time_eutra, type_, "OTDOACell-Information-Item");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  ng_ran_access_point_position_s& ng_ran_access_point_position()
  {
    assert_choice_type(types::ng_ran_access_point_position, type_, "OTDOACell-Information-Item");
    return c.get<ng_ran_access_point_position_s>();
  }
  prs_muting_cfg_eutra_c& prs_muting_cfg_eutra()
  {
    assert_choice_type(types::prs_muting_cfg_eutra, type_, "OTDOACell-Information-Item");
    return c.get<prs_muting_cfg_eutra_c>();
  }
  uint16_t& prsid_eutra()
  {
    assert_choice_type(types::prsid_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  uint16_t& tpid_eutra()
  {
    assert_choice_type(types::tpid_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  tp_type_eutra_e& tp_type_eutra()
  {
    assert_choice_type(types::tp_type_eutra, type_, "OTDOACell-Information-Item");
    return c.get<tp_type_eutra_e>();
  }
  uint16_t& nof_dl_frames_extended_eutra()
  {
    assert_choice_type(types::nof_dl_frames_extended_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  cp_len_eutra_e& crs_cp_len_eutra()
  {
    assert_choice_type(types::crs_cp_len_eutra, type_, "OTDOACell-Information-Item");
    return c.get<cp_len_eutra_e>();
  }
  dl_bw_eutra_e& dl_bw_eutra()
  {
    assert_choice_type(types::dl_bw_eutra, type_, "OTDOACell-Information-Item");
    return c.get<dl_bw_eutra_e>();
  }
  prs_occasion_group_eutra_e& prs_occasion_group_eutra()
  {
    assert_choice_type(types::prs_occasion_group_eutra, type_, "OTDOACell-Information-Item");
    return c.get<prs_occasion_group_eutra_e>();
  }
  prs_freq_hop_cfg_eutra_s& prs_freq_hop_cfg_eutra()
  {
    assert_choice_type(types::prs_freq_hop_cfg_eutra, type_, "OTDOACell-Information-Item");
    return c.get<prs_freq_hop_cfg_eutra_s>();
  }
  protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "OTDOACell-Information-Item");
    return c.get<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>();
  }
  const uint16_t& pci_eutra() const
  {
    assert_choice_type(types::pci_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  const cgi_eutra_s& cgi_eutra() const
  {
    assert_choice_type(types::cgi_eutra, type_, "OTDOACell-Information-Item");
    return c.get<cgi_eutra_s>();
  }
  const fixed_octstring<3, true>& tac() const
  {
    assert_choice_type(types::tac, type_, "OTDOACell-Information-Item");
    return c.get<fixed_octstring<3, true>>();
  }
  const uint32_t& earfcn() const
  {
    assert_choice_type(types::earfcn, type_, "OTDOACell-Information-Item");
    return c.get<uint32_t>();
  }
  const prs_bw_eutra_e& prs_bw_eutra() const
  {
    assert_choice_type(types::prs_bw_eutra, type_, "OTDOACell-Information-Item");
    return c.get<prs_bw_eutra_e>();
  }
  const uint16_t& prs_cfg_idx_eutra() const
  {
    assert_choice_type(types::prs_cfg_idx_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  const cp_len_eutra_e& cp_len_eutra() const
  {
    assert_choice_type(types::cp_len_eutra, type_, "OTDOACell-Information-Item");
    return c.get<cp_len_eutra_e>();
  }
  const nof_dl_frames_eutra_e& nof_dl_frames_eutra() const
  {
    assert_choice_type(types::nof_dl_frames_eutra, type_, "OTDOACell-Information-Item");
    return c.get<nof_dl_frames_eutra_e>();
  }
  const nof_ant_ports_eutra_e& nof_ant_ports_eutra() const
  {
    assert_choice_type(types::nof_ant_ports_eutra, type_, "OTDOACell-Information-Item");
    return c.get<nof_ant_ports_eutra_e>();
  }
  const fixed_bitstring<64, false, true>& sfn_initisation_time_eutra() const
  {
    assert_choice_type(types::sfn_initisation_time_eutra, type_, "OTDOACell-Information-Item");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  const ng_ran_access_point_position_s& ng_ran_access_point_position() const
  {
    assert_choice_type(types::ng_ran_access_point_position, type_, "OTDOACell-Information-Item");
    return c.get<ng_ran_access_point_position_s>();
  }
  const prs_muting_cfg_eutra_c& prs_muting_cfg_eutra() const
  {
    assert_choice_type(types::prs_muting_cfg_eutra, type_, "OTDOACell-Information-Item");
    return c.get<prs_muting_cfg_eutra_c>();
  }
  const uint16_t& prsid_eutra() const
  {
    assert_choice_type(types::prsid_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  const uint16_t& tpid_eutra() const
  {
    assert_choice_type(types::tpid_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  const tp_type_eutra_e& tp_type_eutra() const
  {
    assert_choice_type(types::tp_type_eutra, type_, "OTDOACell-Information-Item");
    return c.get<tp_type_eutra_e>();
  }
  const uint16_t& nof_dl_frames_extended_eutra() const
  {
    assert_choice_type(types::nof_dl_frames_extended_eutra, type_, "OTDOACell-Information-Item");
    return c.get<uint16_t>();
  }
  const cp_len_eutra_e& crs_cp_len_eutra() const
  {
    assert_choice_type(types::crs_cp_len_eutra, type_, "OTDOACell-Information-Item");
    return c.get<cp_len_eutra_e>();
  }
  const dl_bw_eutra_e& dl_bw_eutra() const
  {
    assert_choice_type(types::dl_bw_eutra, type_, "OTDOACell-Information-Item");
    return c.get<dl_bw_eutra_e>();
  }
  const prs_occasion_group_eutra_e& prs_occasion_group_eutra() const
  {
    assert_choice_type(types::prs_occasion_group_eutra, type_, "OTDOACell-Information-Item");
    return c.get<prs_occasion_group_eutra_e>();
  }
  const prs_freq_hop_cfg_eutra_s& prs_freq_hop_cfg_eutra() const
  {
    assert_choice_type(types::prs_freq_hop_cfg_eutra, type_, "OTDOACell-Information-Item");
    return c.get<prs_freq_hop_cfg_eutra_s>();
  }
  const protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "OTDOACell-Information-Item");
    return c.get<protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>>();
  }
  uint16_t&                                                      set_pci_eutra();
  cgi_eutra_s&                                                   set_cgi_eutra();
  fixed_octstring<3, true>&                                      set_tac();
  uint32_t&                                                      set_earfcn();
  prs_bw_eutra_e&                                                set_prs_bw_eutra();
  uint16_t&                                                      set_prs_cfg_idx_eutra();
  cp_len_eutra_e&                                                set_cp_len_eutra();
  nof_dl_frames_eutra_e&                                         set_nof_dl_frames_eutra();
  nof_ant_ports_eutra_e&                                         set_nof_ant_ports_eutra();
  fixed_bitstring<64, false, true>&                              set_sfn_initisation_time_eutra();
  ng_ran_access_point_position_s&                                set_ng_ran_access_point_position();
  prs_muting_cfg_eutra_c&                                        set_prs_muting_cfg_eutra();
  uint16_t&                                                      set_prsid_eutra();
  uint16_t&                                                      set_tpid_eutra();
  tp_type_eutra_e&                                               set_tp_type_eutra();
  uint16_t&                                                      set_nof_dl_frames_extended_eutra();
  cp_len_eutra_e&                                                set_crs_cp_len_eutra();
  dl_bw_eutra_e&                                                 set_dl_bw_eutra();
  prs_occasion_group_eutra_e&                                    set_prs_occasion_group_eutra();
  prs_freq_hop_cfg_eutra_s&                                      set_prs_freq_hop_cfg_eutra();
  protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<cgi_eutra_s,
                  fixed_bitstring<64, false, true>,
                  fixed_octstring<3, true>,
                  ng_ran_access_point_position_s,
                  protocol_ie_single_container_s<otdoa_cell_info_item_ext_ie_o>,
                  prs_freq_hop_cfg_eutra_s,
                  prs_muting_cfg_eutra_c>
      c;

  void destroy_();
};

// OTDOACell-Information ::= SEQUENCE (SIZE (1..63)) OF OTDOACell-Information-Item
using otdoa_cell_info_l = dyn_array<otdoa_cell_info_item_c>;

// OTDOACells-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using otdoa_cells_ext_ies_o = protocol_ext_empty_o;

using otdoa_cells_ext_ies_container = protocol_ext_container_empty_l;

struct otdoa_cells_item_s_ {
  bool                          ie_exts_present = false;
  otdoa_cell_info_l             otdoa_cell_info;
  otdoa_cells_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// OTDOACells ::= SEQUENCE (SIZE (1..3840)) OF OTDOACells-item
using otdoa_cells_l = dyn_array<otdoa_cells_item_s_>;

// OnDemandPRS-Info-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// ResultGERAN-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_geran_item_ext_ies_o = protocol_ext_empty_o;

using result_geran_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultGERAN-Item ::= SEQUENCE
struct result_geran_item_s {
  bool                                ie_exts_present = false;
  uint16_t                            bcch            = 0;
  uint8_t                             pci_geran       = 0;
  uint8_t                             rssi            = 0;
  result_geran_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultGERAN ::= SEQUENCE (SIZE (1..8)) OF ResultGERAN-Item
using result_geran_l = dyn_array<result_geran_item_s>;

// ResultUTRAN-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_utran_item_ext_ies_o = protocol_ext_empty_o;

using result_utran_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultUTRAN-Item ::= SEQUENCE
struct result_utran_item_s {
  struct pci_utran_c_ {
    struct types_opts {
      enum options { phys_cell_i_du_tra_fdd, phys_cell_i_du_tra_tdd, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    pci_utran_c_() = default;
    pci_utran_c_(const pci_utran_c_& other);
    pci_utran_c_& operator=(const pci_utran_c_& other);
    ~pci_utran_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint16_t& phys_cell_i_du_tra_fdd()
    {
      assert_choice_type(types::phys_cell_i_du_tra_fdd, type_, "physCellIDUTRAN");
      return c.get<uint16_t>();
    }
    uint16_t& phys_cell_i_du_tra_tdd()
    {
      assert_choice_type(types::phys_cell_i_du_tra_tdd, type_, "physCellIDUTRAN");
      return c.get<uint16_t>();
    }
    const uint16_t& phys_cell_i_du_tra_fdd() const
    {
      assert_choice_type(types::phys_cell_i_du_tra_fdd, type_, "physCellIDUTRAN");
      return c.get<uint16_t>();
    }
    const uint16_t& phys_cell_i_du_tra_tdd() const
    {
      assert_choice_type(types::phys_cell_i_du_tra_tdd, type_, "physCellIDUTRAN");
      return c.get<uint16_t>();
    }
    uint16_t& set_phys_cell_i_du_tra_fdd();
    uint16_t& set_phys_cell_i_du_tra_tdd();

  private:
    types               type_;
    pod_choice_buffer_t c;

    void destroy_();
  };

  // member variables
  bool                                utra_rs_cp_present = false;
  bool                                utra_ec_n0_present = false;
  bool                                ie_exts_present    = false;
  uint16_t                            uarfcn             = 0;
  pci_utran_c_                        pci_utran;
  int16_t                             utra_rs_cp = -5;
  uint8_t                             utra_ec_n0 = 0;
  result_utran_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultUTRAN ::= SEQUENCE (SIZE (1..8)) OF ResultUTRAN-Item
using result_utran_l = dyn_array<result_utran_item_s>;

// ResultNR-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_nr_item_ext_ies_o = protocol_ext_empty_o;

using result_nr_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultNR-Item ::= SEQUENCE
struct result_nr_item_s {
  bool                             value_ss_rsrp_cell_present = false;
  bool                             value_ss_rsrq_cell_present = false;
  bool                             cgi_nr_present             = false;
  bool                             ie_exts_present            = false;
  uint16_t                         nr_pci                     = 0;
  uint32_t                         nr_arfcn                   = 0;
  uint8_t                          value_ss_rsrp_cell         = 0;
  uint8_t                          value_ss_rsrq_cell         = 0;
  result_ss_rsrp_per_ssb_l         ss_rsrp_per_ssb;
  result_ss_rsrq_per_ssb_l         ss_rsrq_per_ssb;
  cgi_nr_s                         cgi_nr;
  result_nr_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultNR ::= SEQUENCE (SIZE (1..8)) OF ResultNR-Item
using result_nr_l = dyn_array<result_nr_item_s>;

// ResultEUTRA-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using result_eutra_item_ext_ies_o = protocol_ext_empty_o;

using result_eutra_item_ext_ies_container = protocol_ext_container_empty_l;

// ResultEUTRA-Item ::= SEQUENCE
struct result_eutra_item_s {
  bool                                value_rsrp_eutra_present = false;
  bool                                value_rsrq_eutra_present = false;
  bool                                cgi_eutra_present        = false;
  bool                                ie_exts_present          = false;
  uint16_t                            pci_eutra                = 0;
  uint32_t                            earfcn                   = 0;
  uint16_t                            value_rsrp_eutra         = 0;
  uint8_t                             value_rsrq_eutra         = 0;
  cgi_eutra_s                         cgi_eutra;
  result_eutra_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResultEUTRA ::= SEQUENCE (SIZE (1..8)) OF ResultEUTRA-Item
using result_eutra_l = dyn_array<result_eutra_item_s>;

// OtherRATMeasuredResultsValue-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct other_rat_measured_results_value_ext_ie_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { result_nr, result_eutra, nulltype } value;

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
    result_nr_l&          result_nr();
    result_eutra_l&       result_eutra();
    const result_nr_l&    result_nr() const;
    const result_eutra_l& result_eutra() const;

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

// OtherRATMeasuredResultsValue ::= CHOICE
struct other_rat_measured_results_value_c {
  struct types_opts {
    enum options { result_geran, result_utran, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  other_rat_measured_results_value_c() = default;
  other_rat_measured_results_value_c(const other_rat_measured_results_value_c& other);
  other_rat_measured_results_value_c& operator=(const other_rat_measured_results_value_c& other);
  ~other_rat_measured_results_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  result_geran_l& result_geran()
  {
    assert_choice_type(types::result_geran, type_, "OtherRATMeasuredResultsValue");
    return c.get<result_geran_l>();
  }
  result_utran_l& result_utran()
  {
    assert_choice_type(types::result_utran, type_, "OtherRATMeasuredResultsValue");
    return c.get<result_utran_l>();
  }
  protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "OtherRATMeasuredResultsValue");
    return c.get<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>();
  }
  const result_geran_l& result_geran() const
  {
    assert_choice_type(types::result_geran, type_, "OtherRATMeasuredResultsValue");
    return c.get<result_geran_l>();
  }
  const result_utran_l& result_utran() const
  {
    assert_choice_type(types::result_utran, type_, "OtherRATMeasuredResultsValue");
    return c.get<result_utran_l>();
  }
  const protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "OtherRATMeasuredResultsValue");
    return c.get<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>>();
  }
  result_geran_l&                                                            set_result_geran();
  result_utran_l&                                                            set_result_utran();
  protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<other_rat_measured_results_value_ext_ie_o>,
                  result_geran_l,
                  result_utran_l>
      c;

  void destroy_();
};

// OtherRATMeasurementQuantitiesValue ::= ENUMERATED
struct other_rat_meas_quantities_value_opts {
  enum options { geran, utran, /*...*/ nr, eutra, nulltype } value;

  const char* to_string() const;
};
using other_rat_meas_quantities_value_e = enumerated<other_rat_meas_quantities_value_opts, true, 2>;

// OtherRATMeasurementQuantitiesValue-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using other_rat_meas_quantities_value_ext_ies_o = protocol_ext_empty_o;

using other_rat_meas_quantities_value_ext_ies_container = protocol_ext_container_empty_l;

// OtherRATMeasurementQuantities-Item ::= SEQUENCE
struct other_rat_meas_quantities_item_s {
  bool                                              ie_exts_present = false;
  other_rat_meas_quantities_value_e                 other_rat_meas_quantities_value;
  other_rat_meas_quantities_value_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// OtherRATMeasurementQuantities-ItemIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct other_rat_meas_quantities_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { other_rat_meas_quantities_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::other_rat_meas_quantities_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    other_rat_meas_quantities_item_s&       other_rat_meas_quantities_item() { return c; }
    const other_rat_meas_quantities_item_s& other_rat_meas_quantities_item() const { return c; }

  private:
    other_rat_meas_quantities_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// OtherRATMeasurementQuantities ::= SEQUENCE (SIZE (0..64)) OF ProtocolIE-Field{NRPPA-PROTOCOL-IES : IEsSetParam}
using other_rat_meas_quantities_l = dyn_array<protocol_ie_single_container_s<other_rat_meas_quantities_item_ies_o>>;

// OtherRATMeasurementResult ::= SEQUENCE (SIZE (1..64)) OF OtherRATMeasuredResultsValue
using other_rat_meas_result_l = dyn_array<other_rat_measured_results_value_c>;

// PRS-Measurements-Info-List-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using prs_meass_info_list_item_ext_ies_o = protocol_ext_empty_o;

using prs_meass_info_list_item_ext_ies_container = protocol_ext_container_empty_l;

// PRS-Measurements-Info-List-Item ::= SEQUENCE
struct prs_meass_info_list_item_s {
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
  bool                                       ie_exts_present = false;
  uint32_t                                   point_a         = 0;
  meas_prs_periodicity_e_                    meas_prs_periodicity;
  uint16_t                                   meas_prs_offset = 0;
  meas_prs_len_e_                            meas_prs_len;
  prs_meass_info_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRS-Measurements-Info-List ::= SEQUENCE (SIZE (1..4)) OF PRS-Measurements-Info-List-Item
using prs_meass_info_list_l = dyn_array<prs_meass_info_list_item_s>;

// PRSBWAggregationRequestInfo-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// PRSMutingOption1-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using prs_muting_option1_ext_ies_o = protocol_ext_empty_o;

using prs_muting_option1_ext_ies_container = protocol_ext_container_empty_l;

// PRSMutingOption1 ::= SEQUENCE
struct prs_muting_option1_s {
  struct muting_bit_repeat_factor_opts {
    enum options { n1, n2, n4, n8, /*...*/ nulltype } value;
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSMutingOption2-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using prs_muting_option2_ext_ies_o = protocol_ext_empty_o;

using prs_muting_option2_ext_ies_container = protocol_ext_container_empty_l;

// PRSMutingOption2 ::= SEQUENCE
struct prs_muting_option2_s {
  bool                                 ie_exts_present = false;
  dl_prs_muting_pattern_c              muting_pattern;
  prs_muting_option2_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSMuting-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResource-QCLSourceSSB-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// PRSResource-QCLSourcePRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using prs_res_qcl_source_prs_ext_ies_o = protocol_ext_empty_o;

using prs_res_qcl_source_prs_ext_ies_container = protocol_ext_container_empty_l;

// PRSResource-QCLSourcePRS ::= SEQUENCE
struct prs_res_qcl_source_prs_s {
  bool                                     qcl_source_prs_res_id_present = false;
  bool                                     ie_exts_present               = false;
  uint8_t                                  qcl_source_prs_res_set_id     = 0;
  uint8_t                                  qcl_source_prs_res_id         = 0;
  prs_res_qcl_source_prs_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResource-QCLInfo-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// PRSResource-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResource-List ::= SEQUENCE (SIZE (1..64)) OF PRSResource-Item
using prs_res_list_l = dyn_array<prs_res_item_s>;

// PRSResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSResourceSet-List ::= SEQUENCE (SIZE (1..8)) OF PRSResourceSet-Item
using prs_res_set_list_l = dyn_array<prs_res_set_item_s>;

// PRSConfiguration-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedDLPRSResource-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// StartTimeAndDuration-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// RequestedDLPRSResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// RequestedDLPRSTransmissionCharacteristics-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// PRSTransmissionOffPerResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// PRSTransmissionOffIndicationPerResource-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// PRSTransmissionOffPerResource-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// PRSTransmissionOffIndication-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// PRSTransmissionOffInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// PRSTRPItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using prstrp_item_ext_ies_o = protocol_ext_empty_o;

using prstrp_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSTRPItem ::= SEQUENCE
struct prstrp_item_s {
  bool                                  requested_dl_prs_tx_characteristics_present = false;
  bool                                  prs_tx_off_info_present                     = false;
  bool                                  ie_exts_present                             = false;
  uint32_t                              trp_id                                      = 1;
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

// PRSTransmissionTRPItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using prs_tx_trp_item_ext_ies_o = protocol_ext_empty_o;

using prs_tx_trp_item_ext_ies_container = protocol_ext_container_empty_l;

// PRSTransmissionTRPItem ::= SEQUENCE
struct prs_tx_trp_item_s {
  bool                              ie_exts_present = false;
  uint32_t                          trp_id          = 1;
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

// PathlossReferenceSignal-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
using pathloss_ref_sig_ext_ie_o = protocol_ies_empty_o;

// PathlossReferenceSignal ::= CHOICE
struct pathloss_ref_sig_c {
  struct types_opts {
    enum options { ssb_ref, dl_prs_ref, choice_ext, nulltype } value;

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
  ssb_s& ssb_ref()
  {
    assert_choice_type(types::ssb_ref, type_, "PathlossReferenceSignal");
    return c.get<ssb_s>();
  }
  dl_prs_s& dl_prs_ref()
  {
    assert_choice_type(types::dl_prs_ref, type_, "PathlossReferenceSignal");
    return c.get<dl_prs_s>();
  }
  protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PathlossReferenceSignal");
    return c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>();
  }
  const ssb_s& ssb_ref() const
  {
    assert_choice_type(types::ssb_ref, type_, "PathlossReferenceSignal");
    return c.get<ssb_s>();
  }
  const dl_prs_s& dl_prs_ref() const
  {
    assert_choice_type(types::dl_prs_ref, type_, "PathlossReferenceSignal");
    return c.get<dl_prs_s>();
  }
  const protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PathlossReferenceSignal");
    return c.get<protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>>();
  }
  ssb_s&                                                     set_ssb_ref();
  dl_prs_s&                                                  set_dl_prs_ref();
  protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>& set_choice_ext();

private:
  types                                                                                       type_;
  choice_buffer_t<dl_prs_s, protocol_ie_single_container_s<pathloss_ref_sig_ext_ie_o>, ssb_s> c;

  void destroy_();
};

// PathlossReferenceInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pathloss_ref_info_ext_ies_o = protocol_ext_empty_o;

using pathloss_ref_info_ext_ies_container = protocol_ext_container_empty_l;

// PathlossReferenceInformation ::= SEQUENCE
struct pathloss_ref_info_s {
  bool                                ie_exts_present = false;
  pathloss_ref_sig_c                  pathloss_ref_sig;
  pathloss_ref_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PeriodicityItem ::= ENUMERATED
struct periodicity_item_opts {
  enum options {
    ms0dot125,
    ms0dot25,
    ms0dot5,
    ms0dot625,
    ms1,
    ms1dot25,
    ms2,
    ms2dot5,
    ms4dot,
    ms5,
    ms8,
    ms10,
    ms16,
    ms20,
    ms32,
    ms40,
    ms64,
    ms80m,
    ms160,
    ms320,
    ms640m,
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
using periodicity_item_e = enumerated<periodicity_item_opts, true>;

// PeriodicityList ::= SEQUENCE (SIZE (1..16)) OF PeriodicityItem
using periodicity_list_l = bounded_array<periodicity_item_e, 16>;

// PosSRSInfo ::= SEQUENCE
struct pos_srs_info_s {
  uint8_t pos_srs_res_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosSRSResourceID-List ::= SEQUENCE (SIZE (1..64)) OF INTEGER (0..63)
using pos_srs_res_id_list_l = dyn_array<uint8_t>;

// PosValidityAreaCell-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using pos_validity_area_cell_item_ext_ies_o = protocol_ext_empty_o;

using pos_validity_area_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// PosValidityAreaCell-Item ::= SEQUENCE
struct pos_validity_area_cell_item_s {
  bool                                          nr_pci_present  = false;
  bool                                          ie_exts_present = false;
  cgi_nr_s                                      nr_cgi;
  uint16_t                                      nr_pci = 0;
  pos_validity_area_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PosValidityAreaCellList ::= SEQUENCE (SIZE (1..32)) OF PosValidityAreaCell-Item
using pos_validity_area_cell_list_l = dyn_array<pos_validity_area_cell_item_s>;

// PositioningBroadcastCells ::= SEQUENCE (SIZE (1..16384)) OF NG-RAN-CGI
using positioning_broadcast_cells_l = dyn_array<ng_ran_cgi_s>;

// ReferenceSignal-ExtensionIE ::= OBJECT SET OF NRPPA-PROTOCOL-IES
using ref_sig_ext_ie_o = protocol_ies_empty_o;

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
  protocol_ie_single_container_s<ref_sig_ext_ie_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ReferenceSignal");
    return c.get<protocol_ie_single_container_s<ref_sig_ext_ie_o>>();
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
  const protocol_ie_single_container_s<ref_sig_ext_ie_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ReferenceSignal");
    return c.get<protocol_ie_single_container_s<ref_sig_ext_ie_o>>();
  }
  uint8_t&                                          set_nzp_csi_rs();
  ssb_s&                                            set_ssb();
  uint8_t&                                          set_srs();
  uint8_t&                                          set_positioning_srs();
  dl_prs_s&                                         set_dl_prs();
  protocol_ie_single_container_s<ref_sig_ext_ie_o>& set_choice_ext();

private:
  types                                                                              type_;
  choice_buffer_t<dl_prs_s, protocol_ie_single_container_s<ref_sig_ext_ie_o>, ssb_s> c;

  void destroy_();
};

// Remote-UE-Indication ::= ENUMERATED
struct remote_ue_ind_opts {
  enum options { l2_u2n_remote_ue, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using remote_ue_ind_e = enumerated<remote_ue_ind_opts, true>;

// Remote-UE-Indication-Request ::= ENUMERATED
struct remote_ue_ind_request_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using remote_ue_ind_request_e = enumerated<remote_ue_ind_request_opts, true>;

// Remote-UE-Status ::= ENUMERATED
struct remote_ue_status_opts {
  enum options { l2_u2n_remote_ue, no, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using remote_ue_status_e = enumerated<remote_ue_status_opts, true>;

// ReportCharacteristics ::= ENUMERATED
struct report_characteristics_opts {
  enum options { on_demand, periodic, /*...*/ nulltype } value;

  const char* to_string() const;
};
using report_characteristics_e = enumerated<report_characteristics_opts, true>;

// RequestType ::= ENUMERATED
struct request_type_opts {
  enum options { activ, deactiv, /*...*/ nulltype } value;

  const char* to_string() const;
};
using request_type_e = enumerated<request_type_opts, true>;

// SpatialRelationforResourceIDItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using spatial_relationfor_res_id_item_ext_ies_o = protocol_ext_empty_o;

using spatial_relationfor_res_id_item_ext_ies_container = protocol_ext_container_empty_l;

// SpatialRelationforResourceIDItem ::= SEQUENCE
struct spatial_relationfor_res_id_item_s {
  bool                                              ie_exts_present = false;
  ref_sig_c                                         ref_sig;
  spatial_relationfor_res_id_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialRelationforResourceID ::= SEQUENCE (SIZE (1..64)) OF SpatialRelationforResourceIDItem
using spatial_relationfor_res_id_l = dyn_array<spatial_relationfor_res_id_item_s>;

// SpatialRelationInfo-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using spatial_relation_info_ext_ies_o = protocol_ext_empty_o;

using spatial_relation_info_ext_ies_container = protocol_ext_container_empty_l;

// SpatialRelationInfo ::= SEQUENCE
struct spatial_relation_info_s {
  bool                                    ie_exts_present = false;
  spatial_relationfor_res_id_l            spatial_relationfor_res_id;
  spatial_relation_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialRelationPerSRSResourceItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// SpatialRelationPerSRSResource-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// SRSResourceSet-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// SRSResourceSet-Item ::= SEQUENCE
struct srs_res_set_item_s {
  bool                               nof_srs_res_per_set_present   = false;
  bool                               spatial_relation_info_present = false;
  bool                               pathloss_ref_info_present     = false;
  bool                               ie_exts_present               = false;
  uint8_t                            nof_srs_res_per_set           = 1;
  periodicity_list_l                 periodicity_list;
  spatial_relation_info_s            spatial_relation_info;
  pathloss_ref_info_s                pathloss_ref_info;
  srs_res_set_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBBurstPosition-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
using ssb_burst_position_ext_ies_o = protocol_ies_empty_o;

// SSBBurstPosition ::= CHOICE
struct ssb_burst_position_c {
  struct types_opts {
    enum options { short_bitmap, medium_bitmap, long_bitmap, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ssb_burst_position_c() = default;
  ssb_burst_position_c(const ssb_burst_position_c& other);
  ssb_burst_position_c& operator=(const ssb_burst_position_c& other);
  ~ssb_burst_position_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<4, false, true>& short_bitmap()
  {
    assert_choice_type(types::short_bitmap, type_, "SSBBurstPosition");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  fixed_bitstring<8, false, true>& medium_bitmap()
  {
    assert_choice_type(types::medium_bitmap, type_, "SSBBurstPosition");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  fixed_bitstring<64, false, true>& long_bitmap()
  {
    assert_choice_type(types::long_bitmap, type_, "SSBBurstPosition");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SSBBurstPosition");
    return c.get<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>();
  }
  const fixed_bitstring<4, false, true>& short_bitmap() const
  {
    assert_choice_type(types::short_bitmap, type_, "SSBBurstPosition");
    return c.get<fixed_bitstring<4, false, true>>();
  }
  const fixed_bitstring<8, false, true>& medium_bitmap() const
  {
    assert_choice_type(types::medium_bitmap, type_, "SSBBurstPosition");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  const fixed_bitstring<64, false, true>& long_bitmap() const
  {
    assert_choice_type(types::long_bitmap, type_, "SSBBurstPosition");
    return c.get<fixed_bitstring<64, false, true>>();
  }
  const protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SSBBurstPosition");
    return c.get<protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>>();
  }
  fixed_bitstring<4, false, true>&                              set_short_bitmap();
  fixed_bitstring<8, false, true>&                              set_medium_bitmap();
  fixed_bitstring<64, false, true>&                             set_long_bitmap();
  protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                           type_;
  choice_buffer_t<fixed_bitstring<64, false, true>, protocol_ie_single_container_s<ssb_burst_position_ext_ies_o>> c;

  void destroy_();
};

// TF-Configuration-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using tf_cfg_ext_ies_o = protocol_ext_empty_o;

using tf_cfg_ext_ies_container = protocol_ext_container_empty_l;

// TF-Configuration ::= SEQUENCE
struct tf_cfg_s {
  struct ssb_subcarrier_spacing_opts {
    enum options { khz15, khz30, khz120, khz240, /*...*/ khz60, khz480, khz960, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using ssb_subcarrier_spacing_e_ = enumerated<ssb_subcarrier_spacing_opts, true, 3>;
  struct ssb_periodicity_opts {
    enum options { ms5, ms10, ms20, ms40, ms80, ms160, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using ssb_periodicity_e_ = enumerated<ssb_periodicity_opts, true>;

  // member variables
  bool                             ssb_burst_position_present   = false;
  bool                             sfn_initisation_time_present = false;
  bool                             ie_exts_present              = false;
  uint32_t                         ssb_freq                     = 0;
  ssb_subcarrier_spacing_e_        ssb_subcarrier_spacing;
  int8_t                           ssb_tx_pwr = -60;
  ssb_periodicity_e_               ssb_periodicity;
  uint8_t                          ssb_half_frame_offset = 0;
  uint8_t                          ssb_sfn_offset        = 0;
  ssb_burst_position_c             ssb_burst_position;
  fixed_bitstring<64, false, true> sfn_initisation_time;
  tf_cfg_ext_ies_container         ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBInfoItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using ssb_info_item_ext_ies_o = protocol_ext_empty_o;

using ssb_info_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBInfoItem ::= SEQUENCE
struct ssb_info_item_s {
  bool                            ie_exts_present = false;
  tf_cfg_s                        ssb_cfg;
  uint16_t                        pci_nr = 0;
  ssb_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBInfo-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using ssb_info_ext_ies_o = protocol_ext_empty_o;

using ssb_info_ext_ies_container = protocol_ext_container_empty_l;

// SSBInfo ::= SEQUENCE
struct ssb_info_s {
  using list_of_ssb_info_l_ = dyn_array<ssb_info_item_s>;

  // member variables
  bool                       ie_exts_present = false;
  list_of_ssb_info_l_        list_of_ssb_info;
  ssb_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceMapping-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// ValidityAreaSpecificSRSInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// ValidityAreaSpecificSRSInformationExtended-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// RequestedSRSTransmissionCharacteristics-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  using res_type_e_            = enumerated<res_type_opts, true>;
  using list_of_srs_res_set_l_ = dyn_array<srs_res_set_item_s>;

  // member variables
  bool                                               nof_txs_present  = false;
  bool                                               ssb_info_present = false;
  bool                                               ie_exts_present  = false;
  uint16_t                                           nof_txs          = 0;
  res_type_e_                                        res_type;
  bw_srs_c                                           bw;
  list_of_srs_res_set_l_                             list_of_srs_res_set;
  ssb_info_s                                         ssb_info;
  requested_srs_tx_characteristics_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RequestedSRSPreconfigurationCharacteristics-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// ResponseTime-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// UplinkChannelBW-PerSCS-List ::= SEQUENCE (SIZE (1..5)) OF SCS-SpecificCarrier
using ul_ch_bw_per_scs_list_l = dyn_array<scs_specific_carrier_s>;

// SRSCarrier-List-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using srs_carrier_list_item_ext_ies_o = protocol_ext_empty_o;

using srs_carrier_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SRSCarrier-List-Item ::= SEQUENCE
struct srs_carrier_list_item_s {
  bool                                    pci_nr_present  = false;
  bool                                    ie_exts_present = false;
  uint32_t                                point_a         = 0;
  ul_ch_bw_per_scs_list_l                 ul_ch_bw_per_scs_list;
  active_ul_bwp_s                         active_ul_bwp;
  uint16_t                                pci_nr = 0;
  srs_carrier_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSCarrier-List ::= SEQUENCE (SIZE (1..32)) OF SRSCarrier-List-Item
using srs_carrier_list_l = dyn_array<srs_carrier_list_item_s>;

// SRSConfiguration-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct srs_configuration_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct srs_configuration_ext_ies_container {
  bool                               pos_srs_res_set_aggregation_list_present = false;
  pos_srs_res_set_aggregation_list_l pos_srs_res_set_aggregation_list;

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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSInfo ::= SEQUENCE
struct srs_info_s {
  uint8_t srs_res = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSPortIndex ::= ENUMERATED
struct srs_port_idx_opts {
  enum options { id1000, id1001, id1002, id1003, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using srs_port_idx_e = enumerated<srs_port_idx_opts, true>;

// SRSPreconfiguration-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using srs_precfg_item_ext_ies_o = protocol_ext_empty_o;

using srs_precfg_item_ext_ies_container = protocol_ext_container_empty_l;

// SRSPreconfiguration-Item ::= SEQUENCE
struct srs_precfg_item_s {
  bool                              ie_exts_present = false;
  srs_configuration_s               srs_cfg;
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

// SRSResourceID-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using srs_res_id_item_ext_ies_o = protocol_ext_empty_o;

using srs_res_id_item_ext_ies_container = protocol_ext_container_empty_l;

// SRSResourceID-Item ::= SEQUENCE
struct srs_res_id_item_s {
  bool                              ie_exts_present = false;
  uint8_t                           srs_res_id      = 0;
  srs_res_id_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResourceTrigger-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using srs_res_trigger_ext_ies_o = protocol_ext_empty_o;

using srs_res_trigger_ext_ies_container = protocol_ext_container_empty_l;

// SRSResourceTrigger ::= SEQUENCE
struct srs_res_trigger_s {
  bool                              ie_exts_present = false;
  aperiodic_srs_res_trigger_list_l  aperiodic_srs_res_trigger_list;
  srs_res_trigger_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSResourceTypeChoice ::= CHOICE
struct srs_res_type_choice_c {
  struct types_opts {
    enum options { srs_res_info, pos_srs_res_info, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

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
  srs_info_s&     set_srs_res_info();
  pos_srs_info_s& set_pos_srs_res_info();

private:
  types                                       type_;
  choice_buffer_t<pos_srs_info_s, srs_info_s> c;

  void destroy_();
};

// SRSResourcetype-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// SRSTransmissionStatus ::= ENUMERATED
struct srs_tx_status_opts {
  enum options { stopped, /*...*/ area_specific_srs_activ, nulltype } value;

  const char* to_string() const;
};
using srs_tx_status_e = enumerated<srs_tx_status_opts, true, 1>;

// Search-window-information-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using search_win_info_ext_ies_o = protocol_ext_empty_o;

using search_win_info_ext_ies_container = protocol_ext_container_empty_l;

// Search-window-information ::= SEQUENCE
struct search_win_info_s {
  bool                              ie_exts_present            = false;
  int16_t                           expected_propagation_delay = -3841;
  uint16_t                          delay_uncertainty          = 1;
  search_win_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpatialDirectionInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using spatial_direction_info_ext_ies_o = protocol_ext_empty_o;

using spatial_direction_info_ext_ies_container = protocol_ext_container_empty_l;

// SpatialDirectionInformation ::= SEQUENCE
struct spatial_direction_info_s {
  bool                                     ie_exts_present = false;
  nr_prs_beam_info_s                       nr_prs_beam_info;
  spatial_direction_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementRequestItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct trp_meas_request_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_meas_request_item_ext_ies_container {
  bool                cell_id_present           = false;
  bool                ao_a_search_win_present   = false;
  bool                nof_trp_rx_teg_present    = false;
  bool                nof_trp_rx_tx_teg_present = false;
  cgi_nr_s            cell_id;
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
  uint32_t                                trp_id                  = 1;
  search_win_info_s                       search_win_info;
  trp_meas_request_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementRequestList ::= SEQUENCE (SIZE (1..64)) OF TRP-MeasurementRequestItem
using trp_meas_request_list_l = dyn_array<trp_meas_request_item_s>;

// ULRTOAMeas-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct ul_rtoa_meas_ext_ies_o {
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

// ULRTOAMeas ::= CHOICE
struct ul_rtoa_meas_c {
  struct types_opts {
    enum options { k0, k1, k2, k3, k4, k5, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ul_rtoa_meas_c() = default;
  ul_rtoa_meas_c(const ul_rtoa_meas_c& other);
  ul_rtoa_meas_c& operator=(const ul_rtoa_meas_c& other);
  ~ul_rtoa_meas_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint32_t& k0()
  {
    assert_choice_type(types::k0, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  uint32_t& k1()
  {
    assert_choice_type(types::k1, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  uint32_t& k2()
  {
    assert_choice_type(types::k2, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  uint32_t& k3()
  {
    assert_choice_type(types::k3, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  uint32_t& k4()
  {
    assert_choice_type(types::k4, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  uint16_t& k5()
  {
    assert_choice_type(types::k5, type_, "ULRTOAMeas");
    return c.get<uint16_t>();
  }
  protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ULRTOAMeas");
    return c.get<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>();
  }
  const uint32_t& k0() const
  {
    assert_choice_type(types::k0, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  const uint32_t& k1() const
  {
    assert_choice_type(types::k1, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  const uint32_t& k2() const
  {
    assert_choice_type(types::k2, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  const uint32_t& k3() const
  {
    assert_choice_type(types::k3, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  const uint32_t& k4() const
  {
    assert_choice_type(types::k4, type_, "ULRTOAMeas");
    return c.get<uint32_t>();
  }
  const uint16_t& k5() const
  {
    assert_choice_type(types::k5, type_, "ULRTOAMeas");
    return c.get<uint16_t>();
  }
  const protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ULRTOAMeas");
    return c.get<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>>();
  }
  uint32_t&                                               set_k0();
  uint32_t&                                               set_k1();
  uint32_t&                                               set_k2();
  uint32_t&                                               set_k3();
  uint32_t&                                               set_k4();
  uint16_t&                                               set_k5();
  protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>& set_choice_ext();

private:
  types                                                                   type_;
  choice_buffer_t<protocol_ie_single_container_s<ul_rtoa_meas_ext_ies_o>> c;

  void destroy_();
};

// UL-RTOAMeasurement-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct ul_rtoa_measurement_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ul_rtoa_measurement_ext_ies_container {
  bool                     extended_add_path_list_present = false;
  bool                     trp_rx_teg_info_present        = false;
  extended_add_path_list_l extended_add_path_list;
  trp_rx_teg_info_s        trp_rx_teg_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-RTOAMeasurement ::= SEQUENCE
struct ul_rtoa_measurement_s {
  bool                                  ie_exts_present = false;
  ul_rtoa_meas_c                        ul_rto_ameas;
  add_path_list_l                       add_path_list;
  ul_rtoa_measurement_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UL-RSCPMeas-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using ul_rs_cp_meas_ext_ies_o = protocol_ext_empty_o;

using ul_rs_cp_meas_ext_ies_container = protocol_ext_container_empty_l;

// UL-RSCPMeas ::= SEQUENCE
struct ul_rs_cp_meas_s {
  bool                            ie_exts_present = false;
  uint16_t                        ul_rs_cp        = 0;
  ul_rs_cp_meas_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrpMeasuredResultsValue-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct trp_measured_results_value_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { zo_a, multiple_ul_ao_a, ul_srs_rsrp_p, ul_rs_cp_meas, nulltype } value;

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
    zo_a_s&                   zo_a();
    multiple_ul_ao_a_s&       multiple_ul_ao_a();
    ul_srs_rsrp_p_s&          ul_srs_rsrp_p();
    ul_rs_cp_meas_s&          ul_rs_cp_meas();
    const zo_a_s&             zo_a() const;
    const multiple_ul_ao_a_s& multiple_ul_ao_a() const;
    const ul_srs_rsrp_p_s&    ul_srs_rsrp_p() const;
    const ul_rs_cp_meas_s&    ul_rs_cp_meas() const;

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

// TrpMeasuredResultsValue ::= CHOICE
struct trp_measured_results_value_c {
  struct types_opts {
    enum options { ul_angle_of_arrival, ul_srs_rsrp, ul_rtoa, gnb_rx_tx_time_diff, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  trp_measured_results_value_c() = default;
  trp_measured_results_value_c(const trp_measured_results_value_c& other);
  trp_measured_results_value_c& operator=(const trp_measured_results_value_c& other);
  ~trp_measured_results_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ul_ao_a_s& ul_angle_of_arrival()
  {
    assert_choice_type(types::ul_angle_of_arrival, type_, "TrpMeasuredResultsValue");
    return c.get<ul_ao_a_s>();
  }
  uint8_t& ul_srs_rsrp()
  {
    assert_choice_type(types::ul_srs_rsrp, type_, "TrpMeasuredResultsValue");
    return c.get<uint8_t>();
  }
  ul_rtoa_measurement_s& ul_rtoa()
  {
    assert_choice_type(types::ul_rtoa, type_, "TrpMeasuredResultsValue");
    return c.get<ul_rtoa_measurement_s>();
  }
  gnb_rx_tx_time_diff_s& gnb_rx_tx_time_diff()
  {
    assert_choice_type(types::gnb_rx_tx_time_diff, type_, "TrpMeasuredResultsValue");
    return c.get<gnb_rx_tx_time_diff_s>();
  }
  protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TrpMeasuredResultsValue");
    return c.get<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>();
  }
  const ul_ao_a_s& ul_angle_of_arrival() const
  {
    assert_choice_type(types::ul_angle_of_arrival, type_, "TrpMeasuredResultsValue");
    return c.get<ul_ao_a_s>();
  }
  const uint8_t& ul_srs_rsrp() const
  {
    assert_choice_type(types::ul_srs_rsrp, type_, "TrpMeasuredResultsValue");
    return c.get<uint8_t>();
  }
  const ul_rtoa_measurement_s& ul_rtoa() const
  {
    assert_choice_type(types::ul_rtoa, type_, "TrpMeasuredResultsValue");
    return c.get<ul_rtoa_measurement_s>();
  }
  const gnb_rx_tx_time_diff_s& gnb_rx_tx_time_diff() const
  {
    assert_choice_type(types::gnb_rx_tx_time_diff, type_, "TrpMeasuredResultsValue");
    return c.get<gnb_rx_tx_time_diff_s>();
  }
  const protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TrpMeasuredResultsValue");
    return c.get<protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>>();
  }
  ul_ao_a_s&                                                            set_ul_angle_of_arrival();
  uint8_t&                                                              set_ul_srs_rsrp();
  ul_rtoa_measurement_s&                                                set_ul_rtoa();
  gnb_rx_tx_time_diff_s&                                                set_gnb_rx_tx_time_diff();
  protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<gnb_rx_tx_time_diff_s,
                  protocol_ie_single_container_s<trp_measured_results_value_ext_ies_o>,
                  ul_ao_a_s,
                  ul_rtoa_measurement_s>
      c;

  void destroy_();
};

// TrpMeasurementResultItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct trp_meas_result_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_meas_result_item_ext_ies_container {
  bool                       srs_restype_present              = false;
  bool                       arp_id_present                   = false;
  bool                       lo_s_n_lo_si_nformation_present  = false;
  bool                       mobile_trp_location_info_present = false;
  bool                       measured_freq_hops_present       = false;
  bool                       aggr_pos_srs_res_id_list_present = false;
  bool                       meas_based_on_aggr_res_present   = false;
  srs_restype_s              srs_restype;
  uint8_t                    arp_id;
  lo_s_n_lo_si_nformation_c  lo_s_n_lo_si_nformation;
  mobile_trp_location_info_s mobile_trp_location_info;
  measured_freq_hops_e       measured_freq_hops;
  aggr_pos_srs_res_id_list_l aggr_pos_srs_res_id_list;
  meas_based_on_aggr_res_e   meas_based_on_aggr_res;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrpMeasurementResultItem ::= SEQUENCE
struct trp_meas_result_item_s {
  bool                                   meas_quality_present   = false;
  bool                                   meas_beam_info_present = false;
  bool                                   ie_exts_present        = false;
  trp_measured_results_value_c           measured_results_value;
  time_stamp_s                           time_stamp;
  trp_meas_quality_c                     meas_quality;
  meas_beam_info_s                       meas_beam_info;
  trp_meas_result_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrpMeasurementResult ::= SEQUENCE (SIZE (1..16384)) OF TrpMeasurementResultItem
using trp_meas_result_l = dyn_array<trp_meas_result_item_s>;

// TRP-MeasurementResponseItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct trp_meas_resp_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_meas_resp_item_ext_ies_container {
  bool     cell_id_present = false;
  cgi_nr_s cell_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementResponseItem ::= SEQUENCE
struct trp_meas_resp_item_s {
  bool                                 ie_exts_present = false;
  uint32_t                             trp_id          = 1;
  trp_meas_result_l                    meas_result;
  trp_meas_resp_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-MeasurementResponseList ::= SEQUENCE (SIZE (1..64)) OF TRP-MeasurementResponseItem
using trp_meas_resp_list_l = dyn_array<trp_meas_resp_item_s>;

// TRP-MeasurementUpdateItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  uint32_t                            trp_id                = 1;
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

// TRP-PRS-Information-List-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trp_prs_info_list_item_ext_ies_o = protocol_ext_empty_o;

using trp_prs_info_list_item_ext_ies_container = protocol_ext_container_empty_l;

// TRP-PRS-Information-List-Item ::= SEQUENCE
struct trp_prs_info_list_item_s {
  bool                                     cgi_nr_present  = false;
  bool                                     ie_exts_present = false;
  uint32_t                                 trp_id          = 1;
  uint16_t                                 nr_pci          = 0;
  cgi_nr_s                                 cgi_nr;
  prs_cfg_s                                prs_cfg;
  trp_prs_info_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRP-PRS-Information-List ::= SEQUENCE (SIZE (1..256)) OF TRP-PRS-Information-List-Item
using trp_prs_info_list_l = dyn_array<trp_prs_info_list_item_s>;

// TRPBeamAntennaInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TRPTEGItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trpteg_item_ext_ies_o = protocol_ext_empty_o;

using trpteg_item_ext_ies_container = protocol_ext_container_empty_l;

// TRPTEGItem ::= SEQUENCE
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

// TRPTxTEGAssociation ::= SEQUENCE (SIZE (1..8)) OF TRPTEGItem
using trp_tx_teg_assoc_l = dyn_array<trpteg_item_s>;

// TRPInformationTypeResponseItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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
        common_ta_params,
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
    common_ta_params_s&               common_ta_params();
    const trp_type_e&                 trp_type() const;
    const on_demand_prs_info_s&       on_demand_prs() const;
    const trp_tx_teg_assoc_l&         trp_tx_teg_assoc() const;
    const trp_beam_ant_info_s&        trp_beam_ant_info() const;
    const mobile_trp_location_info_s& mobile_trp_location_info() const;
    const common_ta_params_s&         common_ta_params() const;

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
      cgi_nr,
      arfcn,
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
  cgi_nr_s& cgi_nr()
  {
    assert_choice_type(types::cgi_nr, type_, "TRPInformationTypeResponseItem");
    return c.get<cgi_nr_s>();
  }
  uint32_t& arfcn()
  {
    assert_choice_type(types::arfcn, type_, "TRPInformationTypeResponseItem");
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
  const cgi_nr_s& cgi_nr() const
  {
    assert_choice_type(types::cgi_nr, type_, "TRPInformationTypeResponseItem");
    return c.get<cgi_nr_s>();
  }
  const uint32_t& arfcn() const
  {
    assert_choice_type(types::arfcn, type_, "TRPInformationTypeResponseItem");
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
  cgi_nr_s&                                                          set_cgi_nr();
  uint32_t&                                                          set_arfcn();
  prs_cfg_s&                                                         set_prs_cfg();
  ssb_info_s&                                                        set_ss_binfo();
  fixed_bitstring<64, false, true>&                                  set_sfn_initisation_time();
  spatial_direction_info_s&                                          set_spatial_direction_info();
  geographical_coordinates_s&                                        set_geographical_coordinates();
  protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<cgi_nr_s,
                  fixed_bitstring<64, false, true>,
                  geographical_coordinates_s,
                  protocol_ie_single_container_s<trp_info_type_resp_item_ext_ies_o>,
                  prs_cfg_s,
                  spatial_direction_info_s,
                  ssb_info_s>
      c;

  void destroy_();
};

// TRPInformationTypeResponseList ::= SEQUENCE (SIZE (1..64)) OF TRPInformationTypeResponseItem
using trp_info_type_resp_list_l = dyn_array<trp_info_type_resp_item_c>;

// TRPInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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
  uint32_t                   trp_id          = 1;
  trp_info_type_resp_list_l  trp_info_type_resp_list;
  trp_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPInformationTRPResp-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trp_info_trp_resp_ext_ies_o = protocol_ext_empty_o;

using trp_info_trp_resp_ext_ies_container = protocol_ext_container_empty_l;

struct trp_info_list_trp_resp_item_s_ {
  bool                                ie_exts_present = false;
  trp_info_s                          trp_info;
  trp_info_trp_resp_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPInformationListTRPResp ::= SEQUENCE (SIZE (1..65535)) OF TRPInformationListTRPResp-item
using trp_info_list_trp_resp_l = dyn_array<trp_info_list_trp_resp_item_s_>;

// TRPInformationTypeItem ::= ENUMERATED
struct trp_info_type_item_opts {
  enum options {
    nr_pci,
    ng_ran_cgi,
    arfcn,
    prs_cfg,
    ssb_info,
    sfn_init_time,
    spatial_direct_info,
    geo_coord,
    // ...
    trp_type,
    ondemand_pr_si_nfo,
    trp_tx_teg,
    beam_ant_info,
    mobile_trp_location_info,
    common_ta,
    nulltype
  } value;

  const char* to_string() const;
};
using trp_info_type_item_e = enumerated<trp_info_type_item_opts, true, 6>;

// TRPInformationTypeItemTRPReq ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct trp_info_type_item_trp_req_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { trp_info_type_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::trp_info_type_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    trp_info_type_item_e&       trp_info_type_item() { return c; }
    const trp_info_type_item_e& trp_info_type_item() const { return c; }

  private:
    trp_info_type_item_e c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// TRPInformationTypeListTRPReq ::= SEQUENCE (SIZE (1..64)) OF ProtocolIE-Field{NRPPA-PROTOCOL-IES : IEsSetParam}
using trp_info_type_list_trp_req_l = dyn_array<protocol_ie_single_container_s<trp_info_type_item_trp_req_o>>;

// TRPItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using trp_item_ext_ies_o = protocol_ext_empty_o;

using trp_item_ext_ies_container = protocol_ext_container_empty_l;

// TRPItem ::= SEQUENCE
struct trp_item_s {
  bool                       ie_exts_present = false;
  uint32_t                   trp_id          = 1;
  trp_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPList ::= SEQUENCE (SIZE (1..65535)) OF TRPItem
using trp_list_l = dyn_array<trp_item_s>;

// TRPMeasurementType ::= ENUMERATED
struct trp_meas_type_opts {
  enum options {
    gnb_rx_tx_time_diff,
    ul_srs_rsrp,
    ul_ao_a,
    ul_rtoa,
    // ...
    multiple_ul_ao_a,
    ul_srs_rsrp_p,
    ul_rs_cp,
    nulltype
  } value;

  const char* to_string() const;
};
using trp_meas_type_e = enumerated<trp_meas_type_opts, true, 3>;

// TRPMeasurementQuantitiesList-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct trp_meas_quantities_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_meas_quantities_list_item_ext_ies_container {
  bool   timing_report_granularity_factor_extended_present = false;
  int8_t timing_report_granularity_factor_extended;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPMeasurementQuantitiesList-Item ::= SEQUENCE
struct trp_meas_quantities_list_item_s {
  bool                                            timing_report_granularity_factor_present = false;
  bool                                            ie_exts_present                          = false;
  trp_meas_type_e                                 trp_meas_quantities_item;
  uint8_t                                         timing_report_granularity_factor = 0;
  trp_meas_quantities_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPMeasurementQuantities ::= SEQUENCE (SIZE (1..16384)) OF TRPMeasurementQuantitiesList-Item
using trp_meas_quantities_l = dyn_array<trp_meas_quantities_list_item_s>;

// TimeWindowDurationMeasurement-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// TimeWindowDurationSRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
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

// TimeWindowStart-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TimeWindowInformation-Measurement-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TimeWindowStartSRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
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

// TimeWindowInformation-SRS-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using time_win_info_srs_item_ext_ies_o = protocol_ext_empty_o;

using time_win_info_srs_item_ext_ies_container = protocol_ext_container_empty_l;

// TimeWindowInformation-SRS-Item ::= SEQUENCE
struct time_win_info_srs_item_s {
  struct time_win_type_opts {
    enum options { single, periodic, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using time_win_type_e_ = enumerated<time_win_type_opts, true>;

  // member variables
  bool                                     time_win_periodicity_srs_present = false;
  bool                                     ie_ext_present                   = false;
  time_win_start_srs_s                     time_win_start_srs;
  time_win_dur_srs_c                       time_win_dur_srs;
  time_win_type_e_                         time_win_type;
  time_win_periodicity_srs_e               time_win_periodicity_srs;
  time_win_info_srs_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeWindowInformation-SRS-List ::= SEQUENCE (SIZE (1..16)) OF TimeWindowInformation-SRS-Item
using time_win_info_srs_list_l = dyn_array<time_win_info_srs_item_s>;

// UE-TEG-Info-Request ::= ENUMERATED
struct ue_teg_info_request_opts {
  enum options { on_demand, periodic, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ue_teg_info_request_e = enumerated<ue_teg_info_request_opts, true>;

// UE-TEG-ReportingPeriodicity ::= ENUMERATED
struct ue_teg_report_periodicity_opts {
  enum options { ms160, ms320, ms1280, ms2560, ms61440, ms81920, ms368640, ms737280, /*...*/ nulltype } value;
  typedef uint32_t number_type;

  const char* to_string() const;
  uint32_t    to_number() const;
};
using ue_teg_report_periodicity_e = enumerated<ue_teg_report_periodicity_opts, true>;

// UEReportingInformation-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct ue_report_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_report_info_ext_ies_container {
  bool     ue_report_interv_milliseconds_present = false;
  uint16_t ue_report_interv_milliseconds;

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

// UETxTEGAssociationItem-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct ue_tx_teg_assoc_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_tx_teg_assoc_item_ext_ies_container {
  bool                  ue_tx_timing_error_margin_present = false;
  timing_error_margin_e ue_tx_timing_error_margin;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UETxTEGAssociationItem ::= SEQUENCE
struct ue_tx_teg_assoc_item_s {
  bool                                   carrier_freq_present = false;
  bool                                   ie_exts_present      = false;
  uint8_t                                ue_tx_teg_id         = 0;
  pos_srs_res_id_list_l                  pos_srs_res_id_list;
  time_stamp_s                           time_stamp;
  carrier_freq_s                         carrier_freq;
  ue_tx_teg_assoc_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UETxTEGAssociationList ::= SEQUENCE (SIZE (1..256)) OF UETxTEGAssociationItem
using ue_tx_teg_assoc_list_l = dyn_array<ue_tx_teg_assoc_item_s>;

// WLANBand ::= ENUMERATED
struct wlan_band_opts {
  enum options { band2dot4, band5, /*...*/ nulltype } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using wlan_band_e = enumerated<wlan_band_opts, true>;

// WLANChannelList ::= SEQUENCE (SIZE (1..16)) OF INTEGER (0..255)
using wlan_ch_list_l = bounded_array<uint16_t, 16>;

// WLANCountryCode ::= ENUMERATED
struct wlan_country_code_opts {
  enum options { united_states, europe, japan, global, /*...*/ nulltype } value;

  const char* to_string() const;
};
using wlan_country_code_e = enumerated<wlan_country_code_opts, true>;

// WLANMeasurementQuantitiesValue ::= ENUMERATED
struct wlan_meas_quantities_value_opts {
  enum options { wlan, /*...*/ nulltype } value;

  const char* to_string() const;
};
using wlan_meas_quantities_value_e = enumerated<wlan_meas_quantities_value_opts, true>;

// WLANMeasurementQuantitiesValue-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using wlan_meas_quantities_value_ext_ies_o = protocol_ext_empty_o;

using wlan_meas_quantities_value_ext_ies_container = protocol_ext_container_empty_l;

// WLANMeasurementQuantities-Item ::= SEQUENCE
struct wlan_meas_quantities_item_s {
  bool                                         ie_exts_present = false;
  wlan_meas_quantities_value_e                 wlan_meas_quantities_value;
  wlan_meas_quantities_value_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// WLANMeasurementQuantities-ItemIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct wlan_meas_quantities_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { wlan_meas_quantities_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::wlan_meas_quantities_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    wlan_meas_quantities_item_s&       wlan_meas_quantities_item() { return c; }
    const wlan_meas_quantities_item_s& wlan_meas_quantities_item() const { return c; }

  private:
    wlan_meas_quantities_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// WLANMeasurementQuantities ::= SEQUENCE (SIZE (0..64)) OF ProtocolIE-Field{NRPPA-PROTOCOL-IES : IEsSetParam}
using wlan_meas_quantities_l = dyn_array<protocol_ie_single_container_s<wlan_meas_quantities_item_ies_o>>;

// WLANMeasurementResult-Item-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using wlan_meas_result_item_ext_ies_o = protocol_ext_empty_o;

using wlan_meas_result_item_ext_ies_container = protocol_ext_container_empty_l;

// WLANMeasurementResult-Item ::= SEQUENCE
struct wlan_meas_result_item_s {
  bool                                    bs_si_d_present         = false;
  bool                                    hes_si_d_present        = false;
  bool                                    operating_class_present = false;
  bool                                    country_code_present    = false;
  bool                                    wlan_band_present       = false;
  bool                                    ie_exts_present         = false;
  uint16_t                                wlan_rssi               = 0;
  bounded_octstring<1, 32, true>          ssi_d;
  fixed_octstring<6, true>                bs_si_d;
  fixed_octstring<6, true>                hes_si_d;
  uint16_t                                operating_class = 0;
  wlan_country_code_e                     country_code;
  wlan_ch_list_l                          wlan_ch_list;
  wlan_band_e                             wlan_band;
  wlan_meas_result_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// WLANMeasurementResult ::= SEQUENCE (SIZE (1..64)) OF WLANMeasurementResult-Item
using wlan_meas_result_l = dyn_array<wlan_meas_result_item_s>;

} // namespace nrppa
} // namespace asn1

extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::arp_location_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::combined_pos_srs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::pos_srs_res_set_aggregation_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::srs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::slot_offset_for_remaining_hops_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_srs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::srs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_srs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::multiple_ul_ao_a_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::add_path_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::dl_prs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::aggr_prs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::aggr_pos_srs_res_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_sib_segments_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::pos_sibs_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::sys_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::assist_info_fail_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trp_elevation_angle_list_item_s::trp_beam_pwr_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trp_beam_ant_angles_list_item_s::trp_elevation_angle_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_beam_ant_angles_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::crit_diagnostics_ie_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::dl_prsbw_aggregation_request_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::dl_prs_res_set_arp_s::listof_dl_prs_res_arp_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::dl_prs_res_coordinates_s::listof_dl_prs_res_set_arp_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_rsrp_eutra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_rsrq_eutra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::result_ss_rsrp_per_ssb_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_ss_rsrp_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::result_ss_rsrq_per_ssb_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_ss_rsrq_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::result_csi_rsrp_per_csi_rs_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_csi_rsrp_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::result_csi_rsrq_per_csi_rs_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_csi_rsrq_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::measured_results_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::measured_results_associated_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::extended_add_path_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::meas_quantities_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::nr_prs_beam_info_item_s::prs_angle_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::nr_prs_beam_info_s::nr_prs_beam_info_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::nr_prs_beam_info_s::lcs_to_gcs_translation_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::otdoa_cell_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::otdoa_cells_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_geran_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_utran_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_nr_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::result_eutra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::other_rat_meas_quantities_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::other_rat_meas_result_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_meass_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::prsbw_aggregation_request_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::requested_dl_prs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::requested_dl_prs_res_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::prs_tx_off_per_res_set_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::prs_tx_off_per_res_item_s::prs_tx_off_ind_per_res_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_tx_off_per_res_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prstrp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::prs_tx_trp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::pos_validity_area_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::positioning_broadcast_cells_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::spatial_relationfor_res_id_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::spatial_relation_per_srs_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::requested_srs_tx_characteristics_s::list_of_srs_res_set_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::ssb_info_s::list_of_ssb_info_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::requested_srs_precfg_characteristics_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::ul_ch_bw_per_scs_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::srs_carrier_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::srs_precfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trp_meas_request_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_result_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_resp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_upd_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_prs_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trpteg_item_s::dl_prs_res_id_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_tx_teg_assoc_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trp_info_type_resp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trp_info_list_trp_resp_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::trp_info_type_list_trp_req_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::trp_meas_quantities_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::time_win_info_meas_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::nrppa::time_win_info_srs_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::ue_tx_teg_assoc_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::wlan_meas_quantities_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::nrppa::wlan_meas_result_l::value_type>::ops;
