// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/*******************************************************************************
 *
 *                      3GPP TS ASN1 E2AP v08.00 (2025-10)
 *
 ******************************************************************************/

#pragma once

#include "ocudu/asn1/e2ap/common.h"

namespace asn1 {
namespace e2ap {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// CauseRICrequest ::= ENUMERATED
struct cause_ric_request_opts {
  enum options {
    ran_function_id_invalid,
    action_not_supported,
    excessive_actions,
    duplicate_action,
    duplicate_event_trigger,
    function_res_limit,
    request_id_unknown,
    inconsistent_action_subsequent_action_seq,
    ctrl_msg_invalid,
    ric_call_process_id_invalid,
    ctrl_timer_expired,
    ctrl_failed_to_execute,
    sys_not_ready,
    unspecified,
    // ...
    ric_sub_end_time_expired,
    ric_sub_end_time_invalid,
    duplicate_ric_request_id,
    event_trigger_not_supported,
    requested_info_unavailable,
    invalid_info_request,
    nulltype
  } value;

  const char* to_string() const;
};
using cause_ric_request_e = enumerated<cause_ric_request_opts, true, 6>;

// CauseRICservice ::= ENUMERATED
struct cause_ric_service_opts {
  enum options { ran_function_not_supported, excessive_functions, ric_res_limit, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cause_ric_service_e = enumerated<cause_ric_service_opts, true>;

// CauseE2node ::= ENUMERATED
struct cause_e2node_opts {
  enum options { e2node_component_unknown, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using cause_e2node_e = enumerated<cause_e2node_opts, true>;

// CauseTransport ::= ENUMERATED
struct cause_transport_opts {
  enum options { unspecified, transport_res_unavailable, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cause_transport_e = enumerated<cause_transport_opts, true>;

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
  enum options { ctrl_processing_overload, hardware_fail, om_intervention, unspecified, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cause_misc_e = enumerated<cause_misc_opts, true>;

// CauseServiceLayer ::= SEQUENCE
struct cause_service_layer_s {
  unbounded_octstring<true> service_layer_cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cause ::= CHOICE
struct cause_c {
  struct types_opts {
    enum options {
      ric_request,
      ric_service,
      e2_node,
      transport,
      protocol,
      misc,
      /*...*/ service_layer,
      nulltype
    } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts, true, 1>;

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
  cause_ric_request_e& ric_request()
  {
    assert_choice_type(types::ric_request, type_, "Cause");
    return c.get<cause_ric_request_e>();
  }
  cause_ric_service_e& ric_service()
  {
    assert_choice_type(types::ric_service, type_, "Cause");
    return c.get<cause_ric_service_e>();
  }
  cause_e2node_e& e2_node()
  {
    assert_choice_type(types::e2_node, type_, "Cause");
    return c.get<cause_e2node_e>();
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
  cause_service_layer_s& service_layer()
  {
    assert_choice_type(types::service_layer, type_, "Cause");
    return c.get<cause_service_layer_s>();
  }
  const cause_ric_request_e& ric_request() const
  {
    assert_choice_type(types::ric_request, type_, "Cause");
    return c.get<cause_ric_request_e>();
  }
  const cause_ric_service_e& ric_service() const
  {
    assert_choice_type(types::ric_service, type_, "Cause");
    return c.get<cause_ric_service_e>();
  }
  const cause_e2node_e& e2_node() const
  {
    assert_choice_type(types::e2_node, type_, "Cause");
    return c.get<cause_e2node_e>();
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
  const cause_service_layer_s& service_layer() const
  {
    assert_choice_type(types::service_layer, type_, "Cause");
    return c.get<cause_service_layer_s>();
  }
  cause_ric_request_e&   set_ric_request();
  cause_ric_service_e&   set_ric_service();
  cause_e2node_e&        set_e2_node();
  cause_transport_e&     set_transport();
  cause_protocol_e&      set_protocol();
  cause_misc_e&          set_misc();
  cause_service_layer_s& set_service_layer();

private:
  types                                  type_;
  choice_buffer_t<cause_service_layer_s> c;

  void destroy_();
};

// RICrequestID ::= SEQUENCE
struct ric_request_id_s {
  uint32_t ric_requestor_id = 0;
  uint32_t ric_instance_id  = 0;
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

// CriticalityDiagnostics-IE-Item ::= SEQUENCE
struct crit_diagnostics_ie_item_s {
  crit_e          ie_crit;
  uint32_t        ie_id = 0;
  type_of_error_e type_of_error;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CriticalityDiagnostics-IE-List ::= SEQUENCE (SIZE (1..256)) OF CriticalityDiagnostics-IE-Item
using crit_diagnostics_ie_list_l = dyn_array<crit_diagnostics_ie_item_s>;

// CriticalityDiagnostics ::= SEQUENCE
struct crit_diagnostics_s {
  bool                       proc_code_present        = false;
  bool                       trigger_msg_present      = false;
  bool                       proc_crit_present        = false;
  bool                       ric_requestor_id_present = false;
  uint16_t                   proc_code                = 0;
  trigger_msg_e              trigger_msg;
  crit_e                     proc_crit;
  ric_request_id_s           ric_requestor_id;
  crit_diagnostics_ie_list_l ies_crit_diagnostics;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentConfiguration ::= SEQUENCE
struct e2node_component_cfg_s {
  unbounded_octstring<true> e2node_component_request_part;
  unbounded_octstring<true> e2node_component_resp_part;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentConfigurationAck ::= SEQUENCE
struct e2node_component_cfg_ack_s {
  struct upd_outcome_opts {
    enum options { success, fail, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using upd_outcome_e_ = enumerated<upd_outcome_opts, true>;

  // member variables
  bool           fail_cause_present = false;
  upd_outcome_e_ upd_outcome;
  cause_c        fail_cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentInterfaceNG ::= SEQUENCE
struct e2node_component_interface_ng_s {
  printable_string<1, 150, true, true> amf_name;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-ID-Choice ::= CHOICE
struct gnb_id_choice_c {
  struct types_opts {
    enum options { gnb_id, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  types         type() const { return types::gnb_id; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  bounded_bitstring<22, 32, false, true>&       gnb_id() { return c; }
  const bounded_bitstring<22, 32, false, true>& gnb_id() const { return c; }

private:
  bounded_bitstring<22, 32, false, true> c;
};

// GlobalgNB-ID ::= SEQUENCE
struct global_gnb_id_s {
  fixed_octstring<3, true> plmn_id;
  gnb_id_choice_c          gnb_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ENB-ID-Choice ::= CHOICE
struct enb_id_choice_c {
  struct types_opts {
    enum options { enb_id_macro, enb_id_shortmacro, enb_id_longmacro, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  enb_id_choice_c() = default;
  enb_id_choice_c(const enb_id_choice_c& other);
  enb_id_choice_c& operator=(const enb_id_choice_c& other);
  ~enb_id_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<20, false, true>& enb_id_macro()
  {
    assert_choice_type(types::enb_id_macro, type_, "ENB-ID-Choice");
    return c.get<fixed_bitstring<20, false, true>>();
  }
  fixed_bitstring<18, false, true>& enb_id_shortmacro()
  {
    assert_choice_type(types::enb_id_shortmacro, type_, "ENB-ID-Choice");
    return c.get<fixed_bitstring<18, false, true>>();
  }
  fixed_bitstring<21, false, true>& enb_id_longmacro()
  {
    assert_choice_type(types::enb_id_longmacro, type_, "ENB-ID-Choice");
    return c.get<fixed_bitstring<21, false, true>>();
  }
  const fixed_bitstring<20, false, true>& enb_id_macro() const
  {
    assert_choice_type(types::enb_id_macro, type_, "ENB-ID-Choice");
    return c.get<fixed_bitstring<20, false, true>>();
  }
  const fixed_bitstring<18, false, true>& enb_id_shortmacro() const
  {
    assert_choice_type(types::enb_id_shortmacro, type_, "ENB-ID-Choice");
    return c.get<fixed_bitstring<18, false, true>>();
  }
  const fixed_bitstring<21, false, true>& enb_id_longmacro() const
  {
    assert_choice_type(types::enb_id_longmacro, type_, "ENB-ID-Choice");
    return c.get<fixed_bitstring<21, false, true>>();
  }
  fixed_bitstring<20, false, true>& set_enb_id_macro();
  fixed_bitstring<18, false, true>& set_enb_id_shortmacro();
  fixed_bitstring<21, false, true>& set_enb_id_longmacro();

private:
  types                                             type_;
  choice_buffer_t<fixed_bitstring<21, false, true>> c;

  void destroy_();
};

// GlobalngeNB-ID ::= SEQUENCE
struct globalng_enb_id_s {
  fixed_octstring<3, true> plmn_id;
  enb_id_choice_c          enb_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GlobalNG-RANNode-ID ::= CHOICE
struct global_ng_ran_node_id_c {
  struct types_opts {
    enum options { gnb, ng_enb, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  global_ng_ran_node_id_c() = default;
  global_ng_ran_node_id_c(const global_ng_ran_node_id_c& other);
  global_ng_ran_node_id_c& operator=(const global_ng_ran_node_id_c& other);
  ~global_ng_ran_node_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  global_gnb_id_s& gnb()
  {
    assert_choice_type(types::gnb, type_, "GlobalNG-RANNode-ID");
    return c.get<global_gnb_id_s>();
  }
  globalng_enb_id_s& ng_enb()
  {
    assert_choice_type(types::ng_enb, type_, "GlobalNG-RANNode-ID");
    return c.get<globalng_enb_id_s>();
  }
  const global_gnb_id_s& gnb() const
  {
    assert_choice_type(types::gnb, type_, "GlobalNG-RANNode-ID");
    return c.get<global_gnb_id_s>();
  }
  const globalng_enb_id_s& ng_enb() const
  {
    assert_choice_type(types::ng_enb, type_, "GlobalNG-RANNode-ID");
    return c.get<globalng_enb_id_s>();
  }
  global_gnb_id_s&   set_gnb();
  globalng_enb_id_s& set_ng_enb();

private:
  types                                               type_;
  choice_buffer_t<global_gnb_id_s, globalng_enb_id_s> c;

  void destroy_();
};

// E2nodeComponentInterfaceXn ::= SEQUENCE
struct e2node_component_interface_xn_s {
  global_ng_ran_node_id_c global_ng_ran_node_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentInterfaceE1 ::= SEQUENCE
struct e2node_component_interface_e1_s {
  uint64_t gnb_cu_up_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentInterfaceF1 ::= SEQUENCE
struct e2node_component_interface_f1_s {
  uint64_t gnb_du_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentInterfaceW1 ::= SEQUENCE
struct e2node_component_interface_w1_s {
  uint64_t ng_enb_du_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentInterfaceS1 ::= SEQUENCE
struct e2node_component_interface_s1_s {
  printable_string<1, 150, true, true> mme_name;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ENB-ID ::= CHOICE
struct enb_id_c {
  struct types_opts {
    enum options { macro_enb_id, home_enb_id, /*...*/ short_macro_enb_id, long_macro_enb_id, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true, 2>;

  // choice methods
  enb_id_c() = default;
  enb_id_c(const enb_id_c& other);
  enb_id_c& operator=(const enb_id_c& other);
  ~enb_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<20, false, true>& macro_enb_id()
  {
    assert_choice_type(types::macro_enb_id, type_, "ENB-ID");
    return c.get<fixed_bitstring<20, false, true>>();
  }
  fixed_bitstring<28, false, true>& home_enb_id()
  {
    assert_choice_type(types::home_enb_id, type_, "ENB-ID");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  fixed_bitstring<18, false, true>& short_macro_enb_id()
  {
    assert_choice_type(types::short_macro_enb_id, type_, "ENB-ID");
    return c.get<fixed_bitstring<18, false, true>>();
  }
  fixed_bitstring<21, false, true>& long_macro_enb_id()
  {
    assert_choice_type(types::long_macro_enb_id, type_, "ENB-ID");
    return c.get<fixed_bitstring<21, false, true>>();
  }
  const fixed_bitstring<20, false, true>& macro_enb_id() const
  {
    assert_choice_type(types::macro_enb_id, type_, "ENB-ID");
    return c.get<fixed_bitstring<20, false, true>>();
  }
  const fixed_bitstring<28, false, true>& home_enb_id() const
  {
    assert_choice_type(types::home_enb_id, type_, "ENB-ID");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  const fixed_bitstring<18, false, true>& short_macro_enb_id() const
  {
    assert_choice_type(types::short_macro_enb_id, type_, "ENB-ID");
    return c.get<fixed_bitstring<18, false, true>>();
  }
  const fixed_bitstring<21, false, true>& long_macro_enb_id() const
  {
    assert_choice_type(types::long_macro_enb_id, type_, "ENB-ID");
    return c.get<fixed_bitstring<21, false, true>>();
  }
  fixed_bitstring<20, false, true>& set_macro_enb_id();
  fixed_bitstring<28, false, true>& set_home_enb_id();
  fixed_bitstring<18, false, true>& set_short_macro_enb_id();
  fixed_bitstring<21, false, true>& set_long_macro_enb_id();

private:
  types                                             type_;
  choice_buffer_t<fixed_bitstring<28, false, true>> c;

  void destroy_();
};

// GlobalENB-ID ::= SEQUENCE
struct global_enb_id_s {
  fixed_octstring<3, true> plmn_id;
  enb_id_c                 enb_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ENGNB-ID ::= CHOICE
struct en_gnb_id_c {
  struct types_opts {
    enum options { gnb_id, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  types         type() const { return types::gnb_id; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  bounded_bitstring<22, 32, false, true>&       gnb_id() { return c; }
  const bounded_bitstring<22, 32, false, true>& gnb_id() const { return c; }

private:
  bounded_bitstring<22, 32, false, true> c;
};

// GlobalenGNB-ID ::= SEQUENCE
struct globalen_gnb_id_s {
  fixed_octstring<3, true> plmn_id;
  en_gnb_id_c              gnb_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentInterfaceX2 ::= SEQUENCE
struct e2node_component_interface_x2_s {
  bool              global_enb_id_present    = false;
  bool              global_en_gnb_id_present = false;
  global_enb_id_s   global_enb_id;
  globalen_gnb_id_s global_en_gnb_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentID ::= CHOICE
struct e2node_component_id_c {
  struct types_opts {
    enum options {
      e2node_component_interface_type_ng,
      e2node_component_interface_type_xn,
      e2node_component_interface_type_e1,
      e2node_component_interface_type_f1,
      e2node_component_interface_type_w1,
      e2node_component_interface_type_s1,
      e2node_component_interface_type_x2,
      // ...
      nulltype
    } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  e2node_component_id_c() = default;
  e2node_component_id_c(const e2node_component_id_c& other);
  e2node_component_id_c& operator=(const e2node_component_id_c& other);
  ~e2node_component_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  e2node_component_interface_ng_s& e2node_component_interface_type_ng()
  {
    assert_choice_type(types::e2node_component_interface_type_ng, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_ng_s>();
  }
  e2node_component_interface_xn_s& e2node_component_interface_type_xn()
  {
    assert_choice_type(types::e2node_component_interface_type_xn, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_xn_s>();
  }
  e2node_component_interface_e1_s& e2node_component_interface_type_e1()
  {
    assert_choice_type(types::e2node_component_interface_type_e1, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_e1_s>();
  }
  e2node_component_interface_f1_s& e2node_component_interface_type_f1()
  {
    assert_choice_type(types::e2node_component_interface_type_f1, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_f1_s>();
  }
  e2node_component_interface_w1_s& e2node_component_interface_type_w1()
  {
    assert_choice_type(types::e2node_component_interface_type_w1, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_w1_s>();
  }
  e2node_component_interface_s1_s& e2node_component_interface_type_s1()
  {
    assert_choice_type(types::e2node_component_interface_type_s1, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_s1_s>();
  }
  e2node_component_interface_x2_s& e2node_component_interface_type_x2()
  {
    assert_choice_type(types::e2node_component_interface_type_x2, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_x2_s>();
  }
  const e2node_component_interface_ng_s& e2node_component_interface_type_ng() const
  {
    assert_choice_type(types::e2node_component_interface_type_ng, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_ng_s>();
  }
  const e2node_component_interface_xn_s& e2node_component_interface_type_xn() const
  {
    assert_choice_type(types::e2node_component_interface_type_xn, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_xn_s>();
  }
  const e2node_component_interface_e1_s& e2node_component_interface_type_e1() const
  {
    assert_choice_type(types::e2node_component_interface_type_e1, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_e1_s>();
  }
  const e2node_component_interface_f1_s& e2node_component_interface_type_f1() const
  {
    assert_choice_type(types::e2node_component_interface_type_f1, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_f1_s>();
  }
  const e2node_component_interface_w1_s& e2node_component_interface_type_w1() const
  {
    assert_choice_type(types::e2node_component_interface_type_w1, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_w1_s>();
  }
  const e2node_component_interface_s1_s& e2node_component_interface_type_s1() const
  {
    assert_choice_type(types::e2node_component_interface_type_s1, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_s1_s>();
  }
  const e2node_component_interface_x2_s& e2node_component_interface_type_x2() const
  {
    assert_choice_type(types::e2node_component_interface_type_x2, type_, "E2nodeComponentID");
    return c.get<e2node_component_interface_x2_s>();
  }
  e2node_component_interface_ng_s& set_e2node_component_interface_type_ng();
  e2node_component_interface_xn_s& set_e2node_component_interface_type_xn();
  e2node_component_interface_e1_s& set_e2node_component_interface_type_e1();
  e2node_component_interface_f1_s& set_e2node_component_interface_type_f1();
  e2node_component_interface_w1_s& set_e2node_component_interface_type_w1();
  e2node_component_interface_s1_s& set_e2node_component_interface_type_s1();
  e2node_component_interface_x2_s& set_e2node_component_interface_type_x2();

private:
  types type_;
  choice_buffer_t<e2node_component_interface_e1_s,
                  e2node_component_interface_f1_s,
                  e2node_component_interface_ng_s,
                  e2node_component_interface_s1_s,
                  e2node_component_interface_w1_s,
                  e2node_component_interface_x2_s,
                  e2node_component_interface_xn_s>
      c;

  void destroy_();
};

// E2nodeComponentInterfaceType ::= ENUMERATED
struct e2node_component_interface_type_opts {
  enum options { ng, xn, e1, f1, w1, s1, x2, /*...*/ nulltype } value;

  const char* to_string() const;
};
using e2node_component_interface_type_e = enumerated<e2node_component_interface_type_opts, true>;

// GlobalE2node-gNB-ID ::= SEQUENCE
struct global_e2node_gnb_id_s {
  bool              global_en_gnb_id_present = false;
  bool              gnb_cu_up_id_present     = false;
  bool              gnb_du_id_present        = false;
  global_gnb_id_s   global_gnb_id;
  globalen_gnb_id_s global_en_gnb_id;
  uint64_t          gnb_cu_up_id = 0;
  uint64_t          gnb_du_id    = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GlobalE2node-en-gNB-ID ::= SEQUENCE
struct global_e2node_en_gnb_id_s {
  bool              en_gnb_cu_up_id_present = false;
  bool              en_gnb_du_id_present    = false;
  globalen_gnb_id_s global_en_gnb_id;
  uint64_t          en_gnb_cu_up_id = 0;
  uint64_t          en_gnb_du_id    = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GlobalE2node-ng-eNB-ID ::= SEQUENCE
struct global_e2node_ng_enb_id_s {
  bool              global_enb_id_present = false;
  bool              ng_enb_du_id_present  = false;
  globalng_enb_id_s global_ng_enb_id;
  global_enb_id_s   global_enb_id;
  uint64_t          ng_enb_du_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GlobalE2node-eNB-ID ::= SEQUENCE
struct global_e2node_enb_id_s {
  global_enb_id_s global_enb_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GlobalE2node-ID ::= CHOICE
struct global_e2node_id_c {
  struct types_opts {
    enum options { gnb, en_gnb, ng_enb, enb, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  global_e2node_id_c() = default;
  global_e2node_id_c(const global_e2node_id_c& other);
  global_e2node_id_c& operator=(const global_e2node_id_c& other);
  ~global_e2node_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  global_e2node_gnb_id_s& gnb()
  {
    assert_choice_type(types::gnb, type_, "GlobalE2node-ID");
    return c.get<global_e2node_gnb_id_s>();
  }
  global_e2node_en_gnb_id_s& en_gnb()
  {
    assert_choice_type(types::en_gnb, type_, "GlobalE2node-ID");
    return c.get<global_e2node_en_gnb_id_s>();
  }
  global_e2node_ng_enb_id_s& ng_enb()
  {
    assert_choice_type(types::ng_enb, type_, "GlobalE2node-ID");
    return c.get<global_e2node_ng_enb_id_s>();
  }
  global_e2node_enb_id_s& enb()
  {
    assert_choice_type(types::enb, type_, "GlobalE2node-ID");
    return c.get<global_e2node_enb_id_s>();
  }
  const global_e2node_gnb_id_s& gnb() const
  {
    assert_choice_type(types::gnb, type_, "GlobalE2node-ID");
    return c.get<global_e2node_gnb_id_s>();
  }
  const global_e2node_en_gnb_id_s& en_gnb() const
  {
    assert_choice_type(types::en_gnb, type_, "GlobalE2node-ID");
    return c.get<global_e2node_en_gnb_id_s>();
  }
  const global_e2node_ng_enb_id_s& ng_enb() const
  {
    assert_choice_type(types::ng_enb, type_, "GlobalE2node-ID");
    return c.get<global_e2node_ng_enb_id_s>();
  }
  const global_e2node_enb_id_s& enb() const
  {
    assert_choice_type(types::enb, type_, "GlobalE2node-ID");
    return c.get<global_e2node_enb_id_s>();
  }
  global_e2node_gnb_id_s&    set_gnb();
  global_e2node_en_gnb_id_s& set_en_gnb();
  global_e2node_ng_enb_id_s& set_ng_enb();
  global_e2node_enb_id_s&    set_enb();

private:
  types type_;
  choice_buffer_t<global_e2node_en_gnb_id_s, global_e2node_enb_id_s, global_e2node_gnb_id_s, global_e2node_ng_enb_id_s>
      c;

  void destroy_();
};

// GlobalRIC-ID ::= SEQUENCE
struct global_ric_id_s {
  fixed_octstring<3, true>         plmn_id;
  fixed_bitstring<20, false, true> ric_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ListedRecordsOnly ::= ENUMERATED
struct listed_records_only_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using listed_records_only_e = enumerated<listed_records_only_opts, true>;

// RICactionType ::= ENUMERATED
struct ric_action_type_opts {
  enum options { report, insert, policy, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ric_action_type_e = enumerated<ric_action_type_opts, true>;

// RICassistanceUpdate ::= ENUMERATED
struct ric_assist_upd_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ric_assist_upd_e = enumerated<ric_assist_upd_opts, true>;

// RICcontrolAckRequest ::= ENUMERATED
struct ric_ctrl_ack_request_opts {
  enum options { no_ack, ack, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ric_ctrl_ack_request_e = enumerated<ric_ctrl_ack_request_opts, true>;

// RICindicationType ::= ENUMERATED
struct ric_ind_type_opts {
  enum options { report, insert, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ric_ind_type_e = enumerated<ric_ind_type_opts, true>;

// RICloadConfirm ::= ENUMERATED
struct ric_load_confirm_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ric_load_confirm_e = enumerated<ric_load_confirm_opts, true>;

// RICloadInformation ::= SEQUENCE
struct ric_load_info_s {
  struct load_status_opts {
    enum options { overload, notoverload, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using load_status_e_ = enumerated<load_status_opts, true>;

  // member variables
  bool           load_status_present   = false;
  bool           load_estimate_present = false;
  load_status_e_ load_status;
  uint8_t        load_estimate = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICloadRequest ::= ENUMERATED
struct ric_load_request_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ric_load_request_e = enumerated<ric_load_request_opts, true>;

// RICserviceLoadConfirm ::= SEQUENCE
struct ric_service_load_confirm_s {
  bool               ric_service_report_load_confirm_present = false;
  bool               ric_service_insert_load_confirm_present = false;
  bool               ric_service_ctrl_load_confirm_present   = false;
  bool               ric_service_policy_load_confirm_present = false;
  bool               ric_service_query_load_confirm_present  = false;
  ric_load_confirm_e ric_service_report_load_confirm;
  ric_load_confirm_e ric_service_insert_load_confirm;
  ric_load_confirm_e ric_service_ctrl_load_confirm;
  ric_load_confirm_e ric_service_policy_load_confirm;
  ric_load_confirm_e ric_service_query_load_confirm;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceLoadInformation ::= SEQUENCE
struct ric_service_load_info_s {
  bool            ric_service_report_load_info_present = false;
  bool            ric_service_insert_load_info_present = false;
  bool            ric_service_ctrl_load_info_present   = false;
  bool            ric_service_policy_load_info_present = false;
  bool            ric_service_query_load_info_present  = false;
  ric_load_info_s ric_service_report_load_info;
  ric_load_info_s ric_service_insert_load_info;
  ric_load_info_s ric_service_ctrl_load_info;
  ric_load_info_s ric_service_policy_load_info;
  ric_load_info_s ric_service_query_load_info;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceLoadRequest ::= SEQUENCE
struct ric_service_load_request_s {
  bool               ric_service_report_load_request_present = false;
  bool               ric_service_insert_load_request_present = false;
  bool               ric_service_ctrl_load_request_present   = false;
  bool               ric_service_policy_load_request_present = false;
  bool               ric_service_query_load_request_present  = false;
  ric_load_request_e ric_service_report_load_request;
  ric_load_request_e ric_service_insert_load_request;
  ric_load_request_e ric_service_ctrl_load_request;
  ric_load_request_e ric_service_policy_load_request;
  ric_load_request_e ric_service_query_load_request;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionAuditFlag ::= SEQUENCE
struct ric_sub_audit_flag_s {
  bool                  listed_records_only_present = false;
  listed_records_only_e listed_records_only;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubsequentActionType ::= ENUMERATED
struct ric_subsequent_action_type_opts {
  enum options { continue_value, wait, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ric_subsequent_action_type_e = enumerated<ric_subsequent_action_type_opts, true>;

// RICtimeToWait ::= ENUMERATED
struct ric_time_to_wait_opts {
  enum options {
    w1ms,
    w2ms,
    w5ms,
    w10ms,
    w20ms,
    w30ms,
    w40ms,
    w50ms,
    w100ms,
    w200ms,
    w500ms,
    w1s,
    w2s,
    w5s,
    w10s,
    w20s,
    w60s,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using ric_time_to_wait_e = enumerated<ric_time_to_wait_opts, true>;

// RICsubsequentAction ::= SEQUENCE
struct ric_subsequent_action_s {
  ric_subsequent_action_type_e ric_subsequent_action_type;
  ric_time_to_wait_e           ric_time_to_wait;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RegistrationRequest ::= ENUMERATED
struct regist_request_opts {
  enum options { start, stop, add, /*...*/ nulltype } value;

  const char* to_string() const;
};
using regist_request_e = enumerated<regist_request_opts, true>;

// RegistrationRequest-E2 ::= ENUMERATED
struct regist_request_e2_opts {
  enum options { start, stop, add, /*...*/ nulltype } value;

  const char* to_string() const;
};
using regist_request_e2_e = enumerated<regist_request_e2_opts, true>;

// ReportingPeriodicity ::= ENUMERATED
struct report_periodicity_opts {
  enum options { ms500, ms1000, ms2000, ms5000, ms10000, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using report_periodicity_e = enumerated<report_periodicity_opts, true>;

// ReportingPeriodicity-E2 ::= ENUMERATED
struct report_periodicity_e2_opts {
  enum options { ms500, ms1000, ms2000, ms5000, ms10000, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using report_periodicity_e2_e = enumerated<report_periodicity_e2_opts, true>;

// TNLinformation ::= SEQUENCE
struct tnl_info_s {
  bool                                  tnl_port_present = false;
  bounded_bitstring<1, 160, true, true> tnl_address;
  fixed_bitstring<16, false, true>      tnl_port;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TNLusage ::= ENUMERATED
struct tnl_usage_opts {
  enum options { ric_service, support_function, both, /*...*/ nulltype } value;

  const char* to_string() const;
};
using tnl_usage_e = enumerated<tnl_usage_opts, true>;

// TimeToWait ::= ENUMERATED
struct time_to_wait_opts {
  enum options { v1s, v2s, v5s, v10s, v20s, v60s, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using time_to_wait_e = enumerated<time_to_wait_opts, true>;

} // namespace e2ap
} // namespace asn1

extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::crit_diagnostics_ie_list_l::value_type>::ops;
