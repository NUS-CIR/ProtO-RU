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

#include "ocudu/asn1/e2ap/e2ap_ies.h"

namespace asn1 {
namespace e2ap {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// E2RemovalFailureIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2_removal_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2_removal_fail_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2RemovalFailure ::= SEQUENCE
using e2_removal_fail_s = elementary_procedure_option<e2_removal_fail_ies_container>;

// E2RemovalRequestIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2_removal_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2_removal_request_ies_container {
  uint16_t transaction_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2RemovalRequest ::= SEQUENCE
using e2_removal_request_s = elementary_procedure_option<e2_removal_request_ies_container>;

// E2RemovalResponseIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2_removal_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2_removal_resp_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2RemovalResponse ::= SEQUENCE
using e2_removal_resp_s = elementary_procedure_option<e2_removal_resp_ies_container>;

// E2ResourceStatusFailureIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2_res_status_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2_res_status_fail_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  uint16_t           ric_load_meas_id;
  uint16_t           e2node_load_meas_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2ResourceStatusFailure ::= SEQUENCE
using e2_res_status_fail_s = elementary_procedure_option<e2_res_status_fail_ies_container>;

// E2ResourceStatusRequestIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2_res_status_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2_res_status_request_ies_container {
  bool                            e2node_load_meas_id_present    = false;
  bool                            report_characteristics_present = false;
  bool                            report_periodicity_e2_present  = false;
  uint16_t                        transaction_id;
  uint16_t                        ric_load_meas_id;
  uint16_t                        e2node_load_meas_id;
  regist_request_e2_e             regist_request_e2;
  fixed_bitstring<8, false, true> report_characteristics;
  report_periodicity_e2_e         report_periodicity_e2;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2ResourceStatusRequest ::= SEQUENCE
using e2_res_status_request_s = elementary_procedure_option<e2_res_status_request_ies_container>;

// E2ResourceStatusResponseIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2_res_status_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2_res_status_resp_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  uint16_t           ric_load_meas_id;
  uint16_t           e2node_load_meas_id;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2ResourceStatusResponse ::= SEQUENCE
using e2_res_status_resp_s = elementary_procedure_option<e2_res_status_resp_ies_container>;

// E2ResourceStatusUpdateIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2_res_status_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2_res_status_upd_ies_container {
  bool        tnl_capacity_ind_present  = false;
  bool        hardware_load_ind_present = false;
  uint16_t    transaction_id;
  uint16_t    ric_load_meas_id;
  uint16_t    e2node_load_meas_id;
  asn1_null_t tnl_capacity_ind;
  asn1_null_t hardware_load_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2ResourceStatusUpdate ::= SEQUENCE
using e2_res_status_upd_s = elementary_procedure_option<e2_res_status_upd_ies_container>;

// E2connectionSetupFailed-Item ::= SEQUENCE
struct e2conn_setup_failed_item_s {
  tnl_info_s tnl_info;
  cause_c    cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2connectionSetupFailed-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2conn_setup_failed_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2conn_setup_failed_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2conn_setup_failed_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2conn_setup_failed_item_s&       e2conn_setup_failed_item() { return c; }
    const e2conn_setup_failed_item_s& e2conn_setup_failed_item() const { return c; }

  private:
    e2conn_setup_failed_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2connectionSetupFailed-List ::= SEQUENCE (SIZE (1..32)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using e2conn_setup_failed_list_l = bounded_array<protocol_ie_single_container_s<e2conn_setup_failed_item_ies_o>, 32>;

// E2connectionUpdate-Item ::= SEQUENCE
struct e2conn_upd_item_s {
  tnl_info_s  tnl_info;
  tnl_usage_e tnl_usage;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2connectionUpdate-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2conn_upd_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2conn_upd_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2conn_upd_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2conn_upd_item_s&       e2conn_upd_item() { return c; }
    const e2conn_upd_item_s& e2conn_upd_item() const { return c; }

  private:
    e2conn_upd_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2connectionUpdate-List ::= SEQUENCE (SIZE (1..32)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using e2conn_upd_list_l = bounded_array<protocol_ie_single_container_s<e2conn_upd_item_ies_o>, 32>;

// E2connectionUpdateRemove-Item ::= SEQUENCE
struct e2conn_upd_rem_item_s {
  tnl_info_s tnl_info;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2connectionUpdateRemove-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2conn_upd_rem_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2conn_upd_rem_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2conn_upd_rem_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2conn_upd_rem_item_s&       e2conn_upd_rem_item() { return c; }
    const e2conn_upd_rem_item_s& e2conn_upd_rem_item() const { return c; }

  private:
    e2conn_upd_rem_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2connectionUpdateRemove-List ::= SEQUENCE (SIZE (1..32)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using e2conn_upd_rem_list_l = bounded_array<protocol_ie_single_container_s<e2conn_upd_rem_item_ies_o>, 32>;

// E2connectionUpdate-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2conn_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2conn_upd_ies_container {
  bool                  e2conn_upd_add_present    = false;
  bool                  e2conn_upd_rem_present    = false;
  bool                  e2conn_upd_modify_present = false;
  uint16_t              transaction_id;
  e2conn_upd_list_l     e2conn_upd_add;
  e2conn_upd_rem_list_l e2conn_upd_rem;
  e2conn_upd_list_l     e2conn_upd_modify;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2connectionUpdate ::= SEQUENCE
using e2conn_upd_s = elementary_procedure_option<e2conn_upd_ies_container>;

// E2connectionUpdateAck-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2conn_upd_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2conn_upd_ack_ies_container {
  bool                       e2conn_setup_present        = false;
  bool                       e2conn_setup_failed_present = false;
  uint16_t                   transaction_id;
  e2conn_upd_list_l          e2conn_setup;
  e2conn_setup_failed_list_l e2conn_setup_failed;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2connectionUpdateAcknowledge ::= SEQUENCE
using e2conn_upd_ack_s = elementary_procedure_option<e2conn_upd_ack_ies_container>;

// E2connectionUpdateFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2conn_upd_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2conn_upd_fail_ies_container {
  bool               cause_present            = false;
  bool               time_to_wait_present     = false;
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  cause_c            cause;
  time_to_wait_e     time_to_wait;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2connectionUpdateFailure ::= SEQUENCE
using e2conn_upd_fail_s = elementary_procedure_option<e2conn_upd_fail_ies_container>;

// E2nodeComponentConfigAddition-Item ::= SEQUENCE
struct e2node_component_cfg_addition_item_s {
  e2node_component_interface_type_e e2node_component_interface_type;
  e2node_component_id_c             e2node_component_id;
  e2node_component_cfg_s            e2node_component_cfg;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentConfigAddition-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_component_cfg_addition_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2node_component_cfg_addition_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2node_component_cfg_addition_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2node_component_cfg_addition_item_s&       e2node_component_cfg_addition_item() { return c; }
    const e2node_component_cfg_addition_item_s& e2node_component_cfg_addition_item() const { return c; }

  private:
    e2node_component_cfg_addition_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2nodeComponentConfigAddition-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using e2node_component_cfg_addition_list_l =
    dyn_array<protocol_ie_single_container_s<e2node_component_cfg_addition_item_ies_o>>;

// E2nodeComponentConfigAdditionAck-Item ::= SEQUENCE
struct e2node_component_cfg_addition_ack_item_s {
  e2node_component_interface_type_e e2node_component_interface_type;
  e2node_component_id_c             e2node_component_id;
  e2node_component_cfg_ack_s        e2node_component_cfg_ack;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentConfigAdditionAck-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_component_cfg_addition_ack_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2node_component_cfg_addition_ack_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2node_component_cfg_addition_ack_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2node_component_cfg_addition_ack_item_s&       e2node_component_cfg_addition_ack_item() { return c; }
    const e2node_component_cfg_addition_ack_item_s& e2node_component_cfg_addition_ack_item() const { return c; }

  private:
    e2node_component_cfg_addition_ack_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2nodeComponentConfigAdditionAck-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using e2node_component_cfg_addition_ack_list_l =
    dyn_array<protocol_ie_single_container_s<e2node_component_cfg_addition_ack_item_ies_o>>;

// E2nodeComponentConfigRemoval-Item ::= SEQUENCE
struct e2node_component_cfg_removal_item_s {
  e2node_component_interface_type_e e2node_component_interface_type;
  e2node_component_id_c             e2node_component_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentConfigRemoval-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_component_cfg_removal_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2node_component_cfg_removal_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2node_component_cfg_removal_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2node_component_cfg_removal_item_s&       e2node_component_cfg_removal_item() { return c; }
    const e2node_component_cfg_removal_item_s& e2node_component_cfg_removal_item() const { return c; }

  private:
    e2node_component_cfg_removal_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2nodeComponentConfigRemoval-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using e2node_component_cfg_removal_list_l =
    dyn_array<protocol_ie_single_container_s<e2node_component_cfg_removal_item_ies_o>>;

// E2nodeComponentConfigRemovalAck-Item ::= SEQUENCE
struct e2node_component_cfg_removal_ack_item_s {
  e2node_component_interface_type_e e2node_component_interface_type;
  e2node_component_id_c             e2node_component_id;
  e2node_component_cfg_ack_s        e2node_component_cfg_ack;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentConfigRemovalAck-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_component_cfg_removal_ack_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2node_component_cfg_removal_ack_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2node_component_cfg_removal_ack_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2node_component_cfg_removal_ack_item_s&       e2node_component_cfg_removal_ack_item() { return c; }
    const e2node_component_cfg_removal_ack_item_s& e2node_component_cfg_removal_ack_item() const { return c; }

  private:
    e2node_component_cfg_removal_ack_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2nodeComponentConfigRemovalAck-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using e2node_component_cfg_removal_ack_list_l =
    dyn_array<protocol_ie_single_container_s<e2node_component_cfg_removal_ack_item_ies_o>>;

// E2nodeComponentConfigUpdate-Item ::= SEQUENCE
struct e2node_component_cfg_upd_item_s {
  e2node_component_interface_type_e e2node_component_interface_type;
  e2node_component_id_c             e2node_component_id;
  e2node_component_cfg_s            e2node_component_cfg;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentConfigUpdate-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_component_cfg_upd_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2node_component_cfg_upd_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2node_component_cfg_upd_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2node_component_cfg_upd_item_s&       e2node_component_cfg_upd_item() { return c; }
    const e2node_component_cfg_upd_item_s& e2node_component_cfg_upd_item() const { return c; }

  private:
    e2node_component_cfg_upd_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2nodeComponentConfigUpdate-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using e2node_component_cfg_upd_list_l = dyn_array<protocol_ie_single_container_s<e2node_component_cfg_upd_item_ies_o>>;

// E2nodeComponentConfigUpdateAck-Item ::= SEQUENCE
struct e2node_component_cfg_upd_ack_item_s {
  e2node_component_interface_type_e e2node_component_interface_type;
  e2node_component_id_c             e2node_component_id;
  e2node_component_cfg_ack_s        e2node_component_cfg_ack;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeComponentConfigUpdateAck-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_component_cfg_upd_ack_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2node_component_cfg_upd_ack_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2node_component_cfg_upd_ack_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2node_component_cfg_upd_ack_item_s&       e2node_component_cfg_upd_ack_item() { return c; }
    const e2node_component_cfg_upd_ack_item_s& e2node_component_cfg_upd_ack_item() const { return c; }

  private:
    e2node_component_cfg_upd_ack_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2nodeComponentConfigUpdateAck-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using e2node_component_cfg_upd_ack_list_l =
    dyn_array<protocol_ie_single_container_s<e2node_component_cfg_upd_ack_item_ies_o>>;

// E2nodeTNLassociationRemoval-Item ::= SEQUENCE
struct e2node_tnl_assoc_removal_item_s {
  tnl_info_s tnl_info;
  tnl_info_s tnl_info_ric;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeTNLassociationRemoval-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_tnl_assoc_removal_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { e2node_tnl_assoc_removal_item, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::e2node_tnl_assoc_removal_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2node_tnl_assoc_removal_item_s&       e2node_tnl_assoc_removal_item() { return c; }
    const e2node_tnl_assoc_removal_item_s& e2node_tnl_assoc_removal_item() const { return c; }

  private:
    e2node_tnl_assoc_removal_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// E2nodeTNLassociationRemoval-List ::= SEQUENCE (SIZE (1..32)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using e2node_tnl_assoc_removal_list_l =
    bounded_array<protocol_ie_single_container_s<e2node_tnl_assoc_removal_item_ies_o>, 32>;

// E2nodeConfigurationUpdate-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_cfg_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2node_cfg_upd_ies_container {
  bool                                 global_e2node_id_present              = false;
  bool                                 e2node_component_cfg_addition_present = false;
  bool                                 e2node_component_cfg_upd_present      = false;
  bool                                 e2node_component_cfg_removal_present  = false;
  bool                                 e2node_tnl_assoc_removal_present      = false;
  uint16_t                             transaction_id;
  global_e2node_id_c                   global_e2node_id;
  e2node_component_cfg_addition_list_l e2node_component_cfg_addition;
  e2node_component_cfg_upd_list_l      e2node_component_cfg_upd;
  e2node_component_cfg_removal_list_l  e2node_component_cfg_removal;
  e2node_tnl_assoc_removal_list_l      e2node_tnl_assoc_removal;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeConfigurationUpdate ::= SEQUENCE
using e2node_cfg_upd_s = elementary_procedure_option<e2node_cfg_upd_ies_container>;

// E2nodeConfigurationUpdateAcknowledge-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_cfg_upd_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2node_cfg_upd_ack_ies_container {
  bool                                     e2node_component_cfg_addition_ack_present = false;
  bool                                     e2node_component_cfg_upd_ack_present      = false;
  bool                                     e2node_component_cfg_removal_ack_present  = false;
  uint16_t                                 transaction_id;
  e2node_component_cfg_addition_ack_list_l e2node_component_cfg_addition_ack;
  e2node_component_cfg_upd_ack_list_l      e2node_component_cfg_upd_ack;
  e2node_component_cfg_removal_ack_list_l  e2node_component_cfg_removal_ack;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeConfigurationUpdateAcknowledge ::= SEQUENCE
using e2node_cfg_upd_ack_s = elementary_procedure_option<e2node_cfg_upd_ack_ies_container>;

// E2nodeConfigurationUpdateFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2node_cfg_upd_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2node_cfg_upd_fail_ies_container {
  bool               time_to_wait_present     = false;
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  cause_c            cause;
  time_to_wait_e     time_to_wait;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2nodeConfigurationUpdateFailure ::= SEQUENCE
using e2node_cfg_upd_fail_s = elementary_procedure_option<e2node_cfg_upd_fail_ies_container>;

// E2setupFailureIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2setup_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2setup_fail_ies_container {
  bool               time_to_wait_present     = false;
  bool               crit_diagnostics_present = false;
  bool               tnl_info_present         = false;
  uint16_t           transaction_id;
  cause_c            cause;
  time_to_wait_e     time_to_wait;
  crit_diagnostics_s crit_diagnostics;
  tnl_info_s         tnl_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2setupFailure ::= SEQUENCE
using e2setup_fail_s = elementary_procedure_option<e2setup_fail_ies_container>;

// RANfunction-Item ::= SEQUENCE
struct ran_function_item_s {
  uint16_t                              ran_function_id = 0;
  unbounded_octstring<true>             ran_function_definition;
  uint16_t                              ran_function_revision = 0;
  printable_string<1, 1000, true, true> ran_function_o_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunction-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_item_s&       ran_function_item() { return c; }
    const ran_function_item_s& ran_function_item() const { return c; }

  private:
    ran_function_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctions-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_functions_list_l = dyn_array<protocol_ie_single_container_s<ran_function_item_ies_o>>;

// E2setupRequestIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2setup_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2setup_request_ies_container {
  uint16_t                             transaction_id;
  global_e2node_id_c                   global_e2node_id;
  ran_functions_list_l                 ran_functions_added;
  e2node_component_cfg_addition_list_l e2node_component_cfg_addition;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2setupRequest ::= SEQUENCE
using e2setup_request_s = elementary_procedure_option<e2setup_request_ies_container>;

// RANfunctionID-Item ::= SEQUENCE
struct ran_function_id_item_s {
  uint16_t ran_function_id       = 0;
  uint16_t ran_function_revision = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunctionID-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_id_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_id_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_id_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_id_item_s&       ran_function_id_item() { return c; }
    const ran_function_id_item_s& ran_function_id_item() const { return c; }

  private:
    ran_function_id_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctionsID-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_functions_id_list_l = dyn_array<protocol_ie_single_container_s<ran_function_id_item_ies_o>>;

// RANfunctionIDcause-Item ::= SEQUENCE
struct ran_function_id_cause_item_s {
  uint16_t ran_function_id = 0;
  cause_c  cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunctionIDcause-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_id_cause_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_i_ecause_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_i_ecause_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_id_cause_item_s&       ran_function_i_ecause_item() { return c; }
    const ran_function_id_cause_item_s& ran_function_i_ecause_item() const { return c; }

  private:
    ran_function_id_cause_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctionsIDcause-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_functions_id_cause_list_l = dyn_array<protocol_ie_single_container_s<ran_function_id_cause_item_ies_o>>;

// RANfunction-AcceptModified-Item ::= SEQUENCE
struct ran_function_accept_modified_item_s {
  uint16_t                  ran_function_id = 0;
  unbounded_octstring<true> rev_ran_function_definition;
  uint16_t                  ran_function_revision = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunction-AcceptModified-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_accept_modified_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_accept_modified_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_accept_modified_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_accept_modified_item_s&       ran_function_accept_modified_item() { return c; }
    const ran_function_accept_modified_item_s& ran_function_accept_modified_item() const { return c; }

  private:
    ran_function_accept_modified_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctions-AcceptModified-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_functions_accept_modified_list_l =
    dyn_array<protocol_ie_single_container_s<ran_function_accept_modified_item_ies_o>>;

// E2setupResponseIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct e2setup_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e2setup_resp_ies_container {
  bool                                     ran_functions_accepted_present             = false;
  bool                                     ran_functions_rejected_present             = false;
  bool                                     ran_functions_accept_modified_list_present = false;
  uint16_t                                 transaction_id;
  global_ric_id_s                          global_ric_id;
  ran_functions_id_list_l                  ran_functions_accepted;
  ran_functions_id_cause_list_l            ran_functions_rejected;
  e2node_component_cfg_addition_ack_list_l e2node_component_cfg_addition_ack;
  ran_functions_accept_modified_list_l     ran_functions_accept_modified_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2setupResponse ::= SEQUENCE
using e2setup_resp_s = elementary_procedure_option<e2setup_resp_ies_container>;

// ErrorIndication-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct error_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct error_ind_ies_container {
  bool               transaction_id_present   = false;
  bool               ric_request_id_present   = false;
  bool               ran_function_id_present  = false;
  bool               cause_present            = false;
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  ric_request_id_s   ric_request_id;
  uint16_t           ran_function_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ErrorIndication ::= SEQUENCE
using error_ind_s = elementary_procedure_option<error_ind_ies_container>;

// RICactionLoad-Item ::= SEQUENCE
struct ric_action_load_item_s {
  uint16_t        ric_action_id = 0;
  ric_load_info_s ric_action_load_info;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICactionLoad-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_load_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_load_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_load_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_load_item_s&       ric_action_load_item() { return c; }
    const ric_action_load_item_s& ric_action_load_item() const { return c; }

  private:
    ric_action_load_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICactionLoad-List ::= SEQUENCE (SIZE (1..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_action_load_list_l = bounded_array<protocol_ie_single_container_s<ric_action_load_item_ies_o>, 16>;

// RICsubscriptionLoad-ItemIE ::= SEQUENCE
struct ric_sub_load_item_ie_s {
  bool                   ric_sub_load_info_present = false;
  ric_request_id_s       ric_request_id;
  ric_load_info_s        ric_sub_load_info;
  ric_action_load_list_l ric_action_load_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionLoad-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_load_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_sub_load_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_sub_load_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_load_item_ie_s&       ric_sub_load_item() { return c; }
    const ric_sub_load_item_ie_s& ric_sub_load_item() const { return c; }

  private:
    ric_sub_load_item_ie_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICsubscriptionLoad-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_load_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_load_item_ies_o>>;

// RANfunctionLoad-Item ::= SEQUENCE
struct ran_function_load_item_s {
  bool                    ran_function_load_info_present = false;
  bool                    ric_service_load_info_present  = false;
  uint16_t                ran_function_id                = 0;
  ric_load_info_s         ran_function_load_info;
  ric_service_load_info_s ric_service_load_info;
  ric_sub_load_list_l     ric_sub_load_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunctionLoad-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_load_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_load_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_load_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_load_item_s&       ran_function_load_item() { return c; }
    const ran_function_load_item_s& ran_function_load_item() const { return c; }

  private:
    ran_function_load_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctionLoad-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_function_load_list_l = dyn_array<protocol_ie_single_container_s<ran_function_load_item_ies_o>>;

// RICactionLoadConfirm-Item ::= SEQUENCE
struct ric_action_load_confirm_item_s {
  uint16_t           ric_action_id = 0;
  ric_load_confirm_e ric_action_load_confirm;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICactionLoadConfirm-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_load_confirm_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_load_confirm_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_load_confirm_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_load_confirm_item_s&       ric_action_load_confirm_item() { return c; }
    const ric_action_load_confirm_item_s& ric_action_load_confirm_item() const { return c; }

  private:
    ric_action_load_confirm_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICactionLoadConfirm-List ::= SEQUENCE (SIZE (1..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_action_load_confirm_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_load_confirm_item_ies_o>, 16>;

// RICsubscriptionLoadConfirm-ItemIE ::= SEQUENCE
struct ric_sub_load_confirm_item_ie_s {
  bool                           ric_sub_load_confirm_present = false;
  ric_request_id_s               ric_request_id;
  ric_load_confirm_e             ric_sub_load_confirm;
  ric_action_load_confirm_list_l ric_action_load_confirm_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionLoadConfirm-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_load_confirm_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_sub_load_confirm_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_sub_load_confirm_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_load_confirm_item_ie_s&       ric_sub_load_confirm_item() { return c; }
    const ric_sub_load_confirm_item_ie_s& ric_sub_load_confirm_item() const { return c; }

  private:
    ric_sub_load_confirm_item_ie_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICsubscriptionLoadConfirm-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_load_confirm_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_load_confirm_item_ies_o>>;

// RANfunctionLoadConfirm-Item ::= SEQUENCE
struct ran_function_load_confirm_item_s {
  bool                        ran_function_load_confirm_present = false;
  bool                        ric_service_load_confirm_present  = false;
  uint16_t                    ran_function_id                   = 0;
  ric_load_confirm_e          ran_function_load_confirm;
  ric_service_load_confirm_s  ric_service_load_confirm;
  ric_sub_load_confirm_list_l ric_sub_load_confirm_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunctionLoadConfirm-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_load_confirm_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_load_confirm_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_load_confirm_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_load_confirm_item_s&       ran_function_load_confirm_item() { return c; }
    const ran_function_load_confirm_item_s& ran_function_load_confirm_item() const { return c; }

  private:
    ran_function_load_confirm_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctionLoadConfirm-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_function_load_confirm_list_l =
    dyn_array<protocol_ie_single_container_s<ran_function_load_confirm_item_ies_o>>;

// RICactionLoadRequest-Item ::= SEQUENCE
struct ric_action_load_request_item_s {
  uint16_t           ric_action_id = 0;
  ric_load_request_e ric_action_load_request;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICactionLoadRequest-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_load_request_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_load_request_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_load_request_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_load_request_item_s&       ric_action_load_request_item() { return c; }
    const ric_action_load_request_item_s& ric_action_load_request_item() const { return c; }

  private:
    ric_action_load_request_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICactionLoadRequest-List ::= SEQUENCE (SIZE (1..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_action_load_request_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_load_request_item_ies_o>, 16>;

// RICsubscriptionLoadRequest-ItemIE ::= SEQUENCE
struct ric_sub_load_request_item_ie_s {
  bool                           ric_sub_load_request_present = false;
  ric_request_id_s               ric_request_id;
  ric_load_request_e             ric_sub_load_request;
  ric_action_load_request_list_l ric_action_load_request_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionLoadRequest-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_load_request_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_sub_load_request_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_sub_load_request_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_load_request_item_ie_s&       ric_sub_load_request_item() { return c; }
    const ric_sub_load_request_item_ie_s& ric_sub_load_request_item() const { return c; }

  private:
    ric_sub_load_request_item_ie_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICsubscriptionLoadRequest-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_load_request_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_load_request_item_ies_o>>;

// RANfunctionLoadRequest-Item ::= SEQUENCE
struct ran_function_load_request_item_s {
  bool                        ran_function_load_request_present = false;
  bool                        ric_service_load_request_present  = false;
  uint16_t                    ran_function_id                   = 0;
  ric_load_request_e          ran_function_load_request;
  ric_service_load_request_s  ric_service_load_request;
  ric_sub_load_request_list_l ric_sub_load_request_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunctionLoadRequest-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_load_request_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_load_request_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_load_request_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_load_request_item_s&       ran_function_load_request_item() { return c; }
    const ran_function_load_request_item_s& ran_function_load_request_item() const { return c; }

  private:
    ran_function_load_request_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctionLoadRequest-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_function_load_request_list_l =
    dyn_array<protocol_ie_single_container_s<ran_function_load_request_item_ies_o>>;

// RICactionList-Item ::= SEQUENCE
struct ric_action_list_item_s {
  uint16_t ric_action_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_list_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_list_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_list_item_s&       ric_action_list_item() { return c; }
    const ric_action_list_item_s& ric_action_list_item() const { return c; }

  private:
    ric_action_list_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-List ::= SEQUENCE (SIZE (1..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_action_list_l = bounded_array<protocol_ie_single_container_s<ric_action_item_ies_o>, 16>;

// RICsubscriptionList-Item ::= SEQUENCE
struct ric_sub_list_item_s {
  ric_request_id_s  ric_request_id;
  ric_action_list_l ric_action_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionList-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_list_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_sub_list_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_sub_list_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_list_item_s&       ric_sub_list_item() { return c; }
    const ric_sub_list_item_s& ric_sub_list_item() const { return c; }

  private:
    ric_sub_list_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICsubscriptionSuspended-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_suspended_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_list_item_ies_o>>;

// RICsubscriptionResumed-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_resumed_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_list_item_ies_o>>;

// RANfunctionStateConfirm-Item ::= SEQUENCE
struct ran_function_state_confirm_item_s {
  uint16_t                 ran_function_id = 0;
  ric_sub_suspended_list_l ric_sub_suspended_list;
  ric_sub_resumed_list_l   ric_sub_resumed_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunctionStateConfirm-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_state_confirm_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_state_confirm_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_state_confirm_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_state_confirm_item_s&       ran_function_state_confirm_item() { return c; }
    const ran_function_state_confirm_item_s& ran_function_state_confirm_item() const { return c; }

  private:
    ran_function_state_confirm_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctionStateConfirm-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_function_state_confirm_list_l =
    dyn_array<protocol_ie_single_container_s<ran_function_state_confirm_item_ies_o>>;

// RICsubscriptionToBeSuspended-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_to_be_suspended_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_list_item_ies_o>>;

// RICsubscriptionToBeResumed-List ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_to_be_resumed_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_list_item_ies_o>>;

// RANfunctionStateControl-Item ::= SEQUENCE
struct ran_function_state_ctrl_item_s {
  uint16_t                       ran_function_id = 0;
  ric_sub_to_be_suspended_list_l ric_sub_to_be_suspended_list;
  ric_sub_to_be_resumed_list_l   ric_sub_to_be_resumed_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANfunctionStateControl-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ran_function_state_ctrl_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ran_function_state_ctrl_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ran_function_state_ctrl_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ran_function_state_ctrl_item_s&       ran_function_state_ctrl_item() { return c; }
    const ran_function_state_ctrl_item_s& ran_function_state_ctrl_item() const { return c; }

  private:
    ran_function_state_ctrl_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RANfunctionStateControl-List ::= SEQUENCE (SIZE (1..256)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ran_function_state_ctrl_list_l = dyn_array<protocol_ie_single_container_s<ran_function_state_ctrl_item_ies_o>>;

// RICaction-AddedForModification-Item ::= SEQUENCE
struct ric_action_added_for_mod_item_s {
  uint16_t ric_action_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-AddedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_added_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_added_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_added_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_added_for_mod_item_s&       ric_action_added_for_mod_item() { return c; }
    const ric_action_added_for_mod_item_s& ric_action_added_for_mod_item() const { return c; }

  private:
    ric_action_added_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-Admitted-Item ::= SEQUENCE
struct ric_action_admitted_item_s {
  uint16_t ric_action_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-Admitted-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_admitted_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_admitted_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_admitted_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_admitted_item_s&       ric_action_admitted_item() { return c; }
    const ric_action_admitted_item_s& ric_action_admitted_item() const { return c; }

  private:
    ric_action_admitted_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-Admitted-List ::= SEQUENCE (SIZE (1..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_action_admitted_list_l = bounded_array<protocol_ie_single_container_s<ric_action_admitted_item_ies_o>, 16>;

// RICaction-ConfirmedForModification-Item ::= SEQUENCE
struct ric_action_confirmed_for_mod_item_s {
  uint16_t ric_action_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-ConfirmedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_confirmed_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_confirmed_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_confirmed_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_confirmed_for_mod_item_s&       ric_action_confirmed_for_mod_item() { return c; }
    const ric_action_confirmed_for_mod_item_s& ric_action_confirmed_for_mod_item() const { return c; }

  private:
    ric_action_confirmed_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-ConfirmedForRemoval-Item ::= SEQUENCE
struct ric_action_confirmed_for_removal_item_s {
  uint16_t ric_action_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-ConfirmedForRemoval-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_confirmed_for_removal_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_confirmed_for_removal_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_confirmed_for_removal_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_confirmed_for_removal_item_s&       ric_action_confirmed_for_removal_item() { return c; }
    const ric_action_confirmed_for_removal_item_s& ric_action_confirmed_for_removal_item() const { return c; }

  private:
    ric_action_confirmed_for_removal_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-FailedToBeAddedForModification-Item ::= SEQUENCE
struct ric_action_failed_to_be_added_for_mod_item_s {
  uint16_t ric_action_id = 0;
  cause_c  cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-FailedToBeAddedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_failed_to_be_added_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_failed_to_be_added_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_failed_to_be_added_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_failed_to_be_added_for_mod_item_s&       ric_action_failed_to_be_added_for_mod_item() { return c; }
    const ric_action_failed_to_be_added_for_mod_item_s& ric_action_failed_to_be_added_for_mod_item() const { return c; }

  private:
    ric_action_failed_to_be_added_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-FailedToBeModifiedForModification-Item ::= SEQUENCE
struct ric_action_failed_to_be_modified_for_mod_item_s {
  uint16_t ric_action_id = 0;
  cause_c  cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-FailedToBeModifiedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_failed_to_be_modified_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_failed_to_be_modified_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_failed_to_be_modified_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_failed_to_be_modified_for_mod_item_s&       ric_action_failed_to_be_modified_for_mod_item() { return c; }
    const ric_action_failed_to_be_modified_for_mod_item_s& ric_action_failed_to_be_modified_for_mod_item() const
    {
      return c;
    }

  private:
    ric_action_failed_to_be_modified_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-FailedToBeRemovedForModification-Item ::= SEQUENCE
struct ric_action_failed_to_be_remd_for_mod_item_s {
  uint16_t ric_action_id = 0;
  cause_c  cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-FailedToBeRemovedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_failed_to_be_remd_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_failed_to_be_remd_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_failed_to_be_remd_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_failed_to_be_remd_for_mod_item_s&       ric_action_failed_to_be_remd_for_mod_item() { return c; }
    const ric_action_failed_to_be_remd_for_mod_item_s& ric_action_failed_to_be_remd_for_mod_item() const { return c; }

  private:
    ric_action_failed_to_be_remd_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-ModifiedForModification-Item ::= SEQUENCE
struct ric_action_modified_for_mod_item_s {
  uint16_t ric_action_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-ModifiedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_modified_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_modified_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_modified_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_modified_for_mod_item_s&       ric_action_modified_for_mod_item() { return c; }
    const ric_action_modified_for_mod_item_s& ric_action_modified_for_mod_item() const { return c; }

  private:
    ric_action_modified_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-NotAdmitted-Item ::= SEQUENCE
struct ric_action_not_admitted_item_s {
  uint16_t ric_action_id = 0;
  cause_c  cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-NotAdmitted-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_not_admitted_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_not_admitted_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_not_admitted_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_not_admitted_item_s&       ric_action_not_admitted_item() { return c; }
    const ric_action_not_admitted_item_s& ric_action_not_admitted_item() const { return c; }

  private:
    ric_action_not_admitted_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-NotAdmitted-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_action_not_admitted_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_not_admitted_item_ies_o>, 16>;

// RICaction-RefusedToBeModified-Item ::= SEQUENCE
struct ric_action_refused_to_be_modified_item_s {
  uint16_t ric_action_id = 0;
  cause_c  cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-RefusedToBeModified-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_refused_to_be_modified_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_refused_to_be_modified_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_refused_to_be_modified_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_refused_to_be_modified_item_s&       ric_action_refused_to_be_modified_item() { return c; }
    const ric_action_refused_to_be_modified_item_s& ric_action_refused_to_be_modified_item() const { return c; }

  private:
    ric_action_refused_to_be_modified_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-RefusedToBeRemoved-Item ::= SEQUENCE
struct ric_action_refused_to_be_remd_item_s {
  uint16_t ric_action_id = 0;
  cause_c  cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-RefusedToBeRemoved-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_refused_to_be_remd_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_refused_to_be_remd_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_refused_to_be_remd_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_refused_to_be_remd_item_s&       ric_action_refused_to_be_remd_item() { return c; }
    const ric_action_refused_to_be_remd_item_s& ric_action_refused_to_be_remd_item() const { return c; }

  private:
    ric_action_refused_to_be_remd_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-RemovedForModification-Item ::= SEQUENCE
struct ric_action_remd_for_mod_item_s {
  uint16_t ric_action_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-RemovedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_remd_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_remd_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_remd_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_remd_for_mod_item_s&       ric_action_remd_for_mod_item() { return c; }
    const ric_action_remd_for_mod_item_s& ric_action_remd_for_mod_item() const { return c; }

  private:
    ric_action_remd_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-RequiredToBeModified-Item ::= SEQUENCE
struct ric_action_required_to_be_modified_item_s {
  uint16_t           ric_action_id = 0;
  ric_time_to_wait_e ric_time_to_wait;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-RequiredToBeModified-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_required_to_be_modified_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_required_to_be_modified_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_required_to_be_modified_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_required_to_be_modified_item_s&       ric_action_required_to_be_modified_item() { return c; }
    const ric_action_required_to_be_modified_item_s& ric_action_required_to_be_modified_item() const { return c; }

  private:
    ric_action_required_to_be_modified_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-RequiredToBeRemoved-Item ::= SEQUENCE
struct ric_action_required_to_be_remd_item_s {
  uint16_t ric_action_id = 0;
  cause_c  cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-RequiredToBeRemoved-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_required_to_be_remd_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_required_to_be_remd_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_required_to_be_remd_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_required_to_be_remd_item_s&       ric_action_required_to_be_remd_item() { return c; }
    const ric_action_required_to_be_remd_item_s& ric_action_required_to_be_remd_item() const { return c; }

  private:
    ric_action_required_to_be_remd_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-ToBeAddedForModification-Item ::= SEQUENCE
struct ric_action_to_be_added_for_mod_item_s {
  bool                      ric_subsequent_action_present = false;
  uint16_t                  ric_action_id                 = 0;
  ric_action_type_e         ric_action_type;
  unbounded_octstring<true> ric_action_definition;
  uint16_t                  ric_action_execution_order = 0;
  ric_subsequent_action_s   ric_subsequent_action;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-ToBeAddedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_to_be_added_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_to_be_added_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_to_be_added_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_to_be_added_for_mod_item_s&       ric_action_to_be_added_for_mod_item() { return c; }
    const ric_action_to_be_added_for_mod_item_s& ric_action_to_be_added_for_mod_item() const { return c; }

  private:
    ric_action_to_be_added_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-ToBeModifiedForModification-Item ::= SEQUENCE
struct ric_action_to_be_modified_for_mod_item_s {
  bool                      ric_action_execution_order_present = false;
  bool                      ric_subsequent_action_present      = false;
  uint16_t                  ric_action_id                      = 0;
  unbounded_octstring<true> ric_action_definition;
  uint16_t                  ric_action_execution_order = 0;
  ric_subsequent_action_s   ric_subsequent_action;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-ToBeModifiedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_to_be_modified_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_to_be_modified_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_to_be_modified_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_to_be_modified_for_mod_item_s&       ric_action_to_be_modified_for_mod_item() { return c; }
    const ric_action_to_be_modified_for_mod_item_s& ric_action_to_be_modified_for_mod_item() const { return c; }

  private:
    ric_action_to_be_modified_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-ToBeRemovedForModification-Item ::= SEQUENCE
struct ric_action_to_be_remd_for_mod_item_s {
  uint16_t ric_action_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-ToBeRemovedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_to_be_remd_for_mod_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_to_be_remd_for_mod_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_to_be_remd_for_mod_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_to_be_remd_for_mod_item_s&       ric_action_to_be_remd_for_mod_item() { return c; }
    const ric_action_to_be_remd_for_mod_item_s& ric_action_to_be_remd_for_mod_item() const { return c; }

  private:
    ric_action_to_be_remd_for_mod_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICaction-ToBeSetup-Item ::= SEQUENCE
struct ric_action_to_be_setup_item_s {
  bool                      ric_subsequent_action_present = false;
  uint16_t                  ric_action_id                 = 0;
  ric_action_type_e         ric_action_type;
  unbounded_octstring<true> ric_action_definition;
  ric_subsequent_action_s   ric_subsequent_action;
  // ...
  bool     ric_action_execution_order_present = false;
  uint16_t ric_action_execution_order         = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICaction-ToBeSetup-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_action_to_be_setup_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_action_to_be_setup_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_action_to_be_setup_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_action_to_be_setup_item_s&       ric_action_to_be_setup_item() { return c; }
    const ric_action_to_be_setup_item_s& ric_action_to_be_setup_item() const { return c; }

  private:
    ric_action_to_be_setup_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICactions-AddedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_actions_added_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_added_for_mod_item_ies_o>, 16>;

// RICactions-ConfirmedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_confirmed_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_confirmed_for_mod_item_ies_o>, 16>;

// RICactions-ConfirmedForRemoval-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_actions_confirmed_for_removal_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_confirmed_for_removal_item_ies_o>, 16>;

// RICactions-FailedToBeAddedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_failed_to_be_added_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_failed_to_be_added_for_mod_item_ies_o>, 16>;

// RICactions-FailedToBeModifiedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_failed_to_be_modified_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_failed_to_be_modified_for_mod_item_ies_o>, 16>;

// RICactions-FailedToBeRemovedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_failed_to_be_remd_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_failed_to_be_remd_for_mod_item_ies_o>, 16>;

// RICactions-ModifiedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_modified_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_modified_for_mod_item_ies_o>, 16>;

// RICactions-RefusedToBeModified-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_actions_refused_to_be_modified_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_refused_to_be_modified_item_ies_o>, 16>;

// RICactions-RefusedToBeRemoved-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_actions_refused_to_be_remd_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_refused_to_be_remd_item_ies_o>, 16>;

// RICactions-RemovedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_remd_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_remd_for_mod_item_ies_o>, 16>;

// RICactions-RequiredToBeModified-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_actions_required_to_be_modified_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_required_to_be_modified_item_ies_o>, 16>;

// RICactions-RequiredToBeRemoved-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_actions_required_to_be_remd_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_required_to_be_remd_item_ies_o>, 16>;

// RICactions-ToBeAddedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_to_be_added_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_to_be_added_for_mod_item_ies_o>, 16>;

// RICactions-ToBeModifiedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_to_be_modified_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_to_be_modified_for_mod_item_ies_o>, 16>;

// RICactions-ToBeRemovedForModification-List ::= SEQUENCE (SIZE (0..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_actions_to_be_remd_for_mod_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_to_be_remd_for_mod_item_ies_o>, 16>;

// RICactions-ToBeSetup-List ::= SEQUENCE (SIZE (1..16)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_actions_to_be_setup_list_l =
    bounded_array<protocol_ie_single_container_s<ric_action_to_be_setup_item_ies_o>, 16>;

// RICassistanceFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_assist_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_assist_fail_ies_container {
  bool               crit_diagnostics_present = false;
  ric_request_id_s   ric_request_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICassistanceFailure ::= SEQUENCE
using ric_assist_fail_s = elementary_procedure_option<ric_assist_fail_ies_container>;

// RICassistanceHalt-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_assist_halt_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_assist_halt_ies_container {
  ric_request_id_s ric_request_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICassistanceHalt ::= SEQUENCE
using ric_assist_halt_s = elementary_procedure_option<ric_assist_halt_ies_container>;

// RICassistanceIndication-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_assist_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_assist_ind_ies_container {
  ric_request_id_s          ric_request_id;
  uint32_t                  ric_assist_sn;
  unbounded_octstring<true> ric_assist_hdr;
  unbounded_octstring<true> ric_assist_outcome;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICassistanceIndication ::= SEQUENCE
using ric_assist_ind_s = elementary_procedure_option<ric_assist_ind_ies_container>;

// RICassistanceRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_assist_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_assist_request_ies_container {
  bool                      ric_assist_upd_present     = false;
  bool                      ric_assist_upd_num_present = false;
  ric_request_id_s          ric_request_id;
  unbounded_octstring<true> ric_assist_hdr;
  unbounded_octstring<true> ric_assist_msg;
  ric_assist_upd_e          ric_assist_upd;
  uint32_t                  ric_assist_upd_num;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICassistanceRequest ::= SEQUENCE
using ric_assist_request_s = elementary_procedure_option<ric_assist_request_ies_container>;

// RICassistanceResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_assist_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_assist_resp_ies_container {
  ric_request_id_s          ric_request_id;
  unbounded_octstring<true> ric_assist_hdr;
  unbounded_octstring<true> ric_assist_outcome;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICassistanceResponse ::= SEQUENCE
using ric_assist_resp_s = elementary_procedure_option<ric_assist_resp_ies_container>;

// RICcontrolAcknowledge-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_ctrl_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_ctrl_ack_ies_container {
  bool                      ric_call_process_id_present = false;
  bool                      ric_ctrl_outcome_present    = false;
  ric_request_id_s          ric_request_id;
  uint16_t                  ran_function_id;
  unbounded_octstring<true> ric_call_process_id;
  unbounded_octstring<true> ric_ctrl_outcome;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICcontrolAcknowledge ::= SEQUENCE
using ric_ctrl_ack_s = elementary_procedure_option<ric_ctrl_ack_ies_container>;

// RICcontrolFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_ctrl_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_ctrl_fail_ies_container {
  bool                      ric_call_process_id_present = false;
  bool                      ric_ctrl_outcome_present    = false;
  bool                      crit_diagnostics_present    = false;
  ric_request_id_s          ric_request_id;
  uint16_t                  ran_function_id;
  unbounded_octstring<true> ric_call_process_id;
  cause_c                   cause;
  unbounded_octstring<true> ric_ctrl_outcome;
  crit_diagnostics_s        crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICcontrolFailure ::= SEQUENCE
using ric_ctrl_fail_s = elementary_procedure_option<ric_ctrl_fail_ies_container>;

// RICcontrolRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_ctrl_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_ctrl_request_ies_container {
  bool                      ric_call_process_id_present  = false;
  bool                      ric_ctrl_ack_request_present = false;
  ric_request_id_s          ric_request_id;
  uint16_t                  ran_function_id;
  unbounded_octstring<true> ric_call_process_id;
  unbounded_octstring<true> ric_ctrl_hdr;
  unbounded_octstring<true> ric_ctrl_msg;
  ric_ctrl_ack_request_e    ric_ctrl_ack_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICcontrolRequest ::= SEQUENCE
using ric_ctrl_request_s = elementary_procedure_option<ric_ctrl_request_ies_container>;

// RICindication-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_ind_ies_container {
  bool                      ric_ind_sn_present          = false;
  bool                      ric_call_process_id_present = false;
  ric_request_id_s          ric_request_id;
  uint16_t                  ran_function_id;
  uint16_t                  ric_action_id;
  uint32_t                  ric_ind_sn;
  ric_ind_type_e            ric_ind_type;
  unbounded_octstring<true> ric_ind_hdr;
  unbounded_octstring<true> ric_ind_msg;
  unbounded_octstring<true> ric_call_process_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICindication ::= SEQUENCE
using ric_ind_s = elementary_procedure_option<ric_ind_ies_container>;

// RICqueryFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_query_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_query_fail_ies_container {
  bool               crit_diagnostics_present = false;
  ric_request_id_s   ric_request_id;
  uint16_t           ran_function_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICqueryFailure ::= SEQUENCE
using ric_query_fail_s = elementary_procedure_option<ric_query_fail_ies_container>;

// RICqueryRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_query_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_query_request_ies_container {
  ric_request_id_s          ric_request_id;
  uint16_t                  ran_function_id;
  unbounded_octstring<true> ric_query_hdr;
  unbounded_octstring<true> ric_query_definition;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICqueryRequest ::= SEQUENCE
using ric_query_request_s = elementary_procedure_option<ric_query_request_ies_container>;

// RICqueryResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_query_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_query_resp_ies_container {
  ric_request_id_s          ric_request_id;
  uint16_t                  ran_function_id;
  unbounded_octstring<true> ric_query_outcome;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICqueryResponse ::= SEQUENCE
using ric_query_resp_s = elementary_procedure_option<ric_query_resp_ies_container>;

// RICserviceLoadStatusFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_service_load_status_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_service_load_status_fail_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           ric_load_meas_id;
  uint16_t           e2node_load_meas_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceLoadStatusFailure ::= SEQUENCE
using ric_service_load_status_fail_s = elementary_procedure_option<ric_service_load_status_fail_ies_container>;

// RICserviceLoadStatusRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_service_load_status_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_service_load_status_request_ies_container {
  bool                             e2node_load_meas_id_present            = false;
  bool                             ran_function_load_request_list_present = false;
  bool                             report_periodicity_present             = false;
  uint16_t                         ric_load_meas_id;
  uint16_t                         e2node_load_meas_id;
  regist_request_e                 regist_request;
  ran_function_load_request_list_l ran_function_load_request_list;
  report_periodicity_e             report_periodicity;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceLoadStatusRequest ::= SEQUENCE
using ric_service_load_status_request_s = elementary_procedure_option<ric_service_load_status_request_ies_container>;

// RICserviceLoadStatusResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_service_load_status_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_service_load_status_resp_ies_container {
  bool                             ran_function_load_confirm_list_present = false;
  uint16_t                         ric_load_meas_id;
  uint16_t                         e2node_load_meas_id;
  ran_function_load_confirm_list_l ran_function_load_confirm_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceLoadStatusResponse ::= SEQUENCE
using ric_service_load_status_resp_s = elementary_procedure_option<ric_service_load_status_resp_ies_container>;

// RICserviceLoadUpdate-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_service_load_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_service_load_upd_ies_container {
  uint16_t                 ric_load_meas_id;
  uint16_t                 e2node_load_meas_id;
  ran_function_load_list_l ran_function_load_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceLoadUpdate ::= SEQUENCE
using ric_service_load_upd_s = elementary_procedure_option<ric_service_load_upd_ies_container>;

// RICserviceQuery-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_service_query_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_service_query_ies_container {
  bool                    ran_functions_accepted_present = false;
  uint16_t                transaction_id;
  ran_functions_id_list_l ran_functions_accepted;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceQuery ::= SEQUENCE
using ric_service_query_s = elementary_procedure_option<ric_service_query_ies_container>;

// RICserviceUpdate-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_service_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_service_upd_ies_container {
  bool                    ran_functions_added_present    = false;
  bool                    ran_functions_modified_present = false;
  bool                    ran_functions_deleted_present  = false;
  uint16_t                transaction_id;
  ran_functions_list_l    ran_functions_added;
  ran_functions_list_l    ran_functions_modified;
  ran_functions_id_list_l ran_functions_deleted;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceUpdate ::= SEQUENCE
using ric_service_upd_s = elementary_procedure_option<ric_service_upd_ies_container>;

// RICserviceUpdateAcknowledge-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_service_upd_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_service_upd_ack_ies_container {
  bool                                 ran_functions_accepted_present             = false;
  bool                                 ran_functions_rejected_present             = false;
  bool                                 ran_functions_accept_modified_list_present = false;
  uint16_t                             transaction_id;
  ran_functions_id_list_l              ran_functions_accepted;
  ran_functions_id_cause_list_l        ran_functions_rejected;
  ran_functions_accept_modified_list_l ran_functions_accept_modified_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceUpdateAcknowledge ::= SEQUENCE
using ric_service_upd_ack_s = elementary_procedure_option<ric_service_upd_ack_ies_container>;

// RICserviceUpdateFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_service_upd_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_service_upd_fail_ies_container {
  bool               time_to_wait_present     = false;
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  cause_c            cause;
  time_to_wait_e     time_to_wait;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICserviceUpdateFailure ::= SEQUENCE
using ric_service_upd_fail_s = elementary_procedure_option<ric_service_upd_fail_ies_container>;

// RICsubscription-withCause-Item ::= SEQUENCE
struct ric_sub_with_cause_item_s {
  ric_request_id_s ric_request_id;
  uint16_t         ran_function_id = 0;
  cause_c          cause;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscription-withCause-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_with_cause_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_sub_with_cause_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_sub_with_cause_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_with_cause_item_s&       ric_sub_with_cause_item() { return c; }
    const ric_sub_with_cause_item_s& ric_sub_with_cause_item() const { return c; }

  private:
    ric_sub_with_cause_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICsubscription-List-withCause ::= SEQUENCE (SIZE (1..1024)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_list_with_cause_l = dyn_array<protocol_ie_single_container_s<ric_sub_with_cause_item_ies_o>>;

// RICsubscriptionAudit-Item ::= SEQUENCE
struct ric_sub_audit_item_s {
  ric_request_id_s ric_request_id;
  uint16_t         ran_function_id = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionAudit-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_audit_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_sub_audit_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_sub_audit_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_audit_item_s&       ric_sub_audit_item() { return c; }
    const ric_sub_audit_item_s& ric_sub_audit_item() const { return c; }

  private:
    ric_sub_audit_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICsubscriptionAuditAction-Item ::= SEQUENCE
struct ric_sub_audit_action_item_s {
  ric_request_id_s           ric_request_id;
  uint16_t                   ran_function_id = 0;
  ric_action_admitted_list_l ric_action_admitted_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionAuditAction-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_audit_action_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ric_sub_audit_action_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ric_sub_audit_action_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_audit_action_item_s&       ric_sub_audit_action_item() { return c; }
    const ric_sub_audit_action_item_s& ric_sub_audit_action_item() const { return c; }

  private:
    ric_sub_audit_action_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// RICsubscriptionAuditActionList ::= SEQUENCE (SIZE (1..2147483648)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES :
// IEsSetParam}
using ric_sub_audit_action_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_audit_action_item_ies_o>>;

// RICsubscriptionAuditFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_audit_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_audit_fail_ies_container {
  bool               crit_diagnostics_present = false;
  ric_request_id_s   ric_request_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionAuditFailure ::= SEQUENCE
using ric_sub_audit_fail_s = elementary_procedure_option<ric_sub_audit_fail_ies_container>;

// RICsubscriptionAuditList ::= SEQUENCE (SIZE (1..2147483648)) OF ProtocolIE-Field{E2AP-PROTOCOL-IES : IEsSetParam}
using ric_sub_audit_list_l = dyn_array<protocol_ie_single_container_s<ric_sub_audit_item_ies_o>>;

// RICsubscriptionAuditRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_audit_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_audit_request_ies_container {
  bool                 ric_sub_audit_flag_present = false;
  bool                 ric_sub_audit_list_present = false;
  ric_request_id_s     ric_request_id;
  ric_sub_audit_flag_s ric_sub_audit_flag;
  ric_sub_audit_list_l ric_sub_audit_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionAuditRequest ::= SEQUENCE
using ric_sub_audit_request_s = elementary_procedure_option<ric_sub_audit_request_ies_container>;

// RICsubscriptionAuditResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_audit_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_audit_resp_ies_container {
  bool                        ric_sub_audit_unkown_list_present  = false;
  bool                        ric_sub_audit_missing_list_present = false;
  ric_request_id_s            ric_request_id;
  ric_sub_audit_action_list_l ric_sub_audit_confirmed_list;
  ric_sub_audit_list_l        ric_sub_audit_unkown_list;
  ric_sub_audit_action_list_l ric_sub_audit_missing_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionAuditResponse ::= SEQUENCE
using ric_sub_audit_resp_s = elementary_procedure_option<ric_sub_audit_resp_ies_container>;

// RICsubscriptionDeleteFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_delete_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_delete_fail_ies_container {
  bool               crit_diagnostics_present = false;
  ric_request_id_s   ric_request_id;
  uint16_t           ran_function_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionDeleteFailure ::= SEQUENCE
using ric_sub_delete_fail_s = elementary_procedure_option<ric_sub_delete_fail_ies_container>;

// RICsubscriptionDeleteRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_delete_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_delete_request_ies_container {
  ric_request_id_s ric_request_id;
  uint16_t         ran_function_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionDeleteRequest ::= SEQUENCE
using ric_sub_delete_request_s = elementary_procedure_option<ric_sub_delete_request_ies_container>;

// RICsubscriptionDeleteRequired-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_delete_required_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_delete_required_ies_container {
  ric_sub_list_with_cause_l ric_sub_to_be_remd;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionDeleteRequired ::= SEQUENCE
using ric_sub_delete_required_s = elementary_procedure_option<ric_sub_delete_required_ies_container>;

// RICsubscriptionDeleteResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_delete_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_delete_resp_ies_container {
  ric_request_id_s ric_request_id;
  uint16_t         ran_function_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionDeleteResponse ::= SEQUENCE
using ric_sub_delete_resp_s = elementary_procedure_option<ric_sub_delete_resp_ies_container>;

// RICsubscriptionDetails ::= SEQUENCE
struct ric_sub_details_s {
  unbounded_octstring<true>      ric_event_trigger_definition;
  ric_actions_to_be_setup_list_l ric_action_to_be_setup_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_fail_ies_container {
  bool               crit_diagnostics_present = false;
  ric_request_id_s   ric_request_id;
  uint16_t           ran_function_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionFailure ::= SEQUENCE
using ric_sub_fail_s = elementary_procedure_option<ric_sub_fail_ies_container>;

// RICsubscriptionModificationConfirm-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_mod_confirm_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_mod_confirm_ies_container {
  bool                                      ric_actions_confirmed_for_mod_list_present      = false;
  bool                                      ric_actions_refused_to_be_modified_list_present = false;
  bool                                      ric_actions_confirmed_for_removal_list_present  = false;
  bool                                      ric_actions_refused_to_be_remd_list_present     = false;
  ric_request_id_s                          ric_request_id;
  uint16_t                                  ran_function_id;
  ric_actions_confirmed_for_mod_list_l      ric_actions_confirmed_for_mod_list;
  ric_actions_refused_to_be_modified_list_l ric_actions_refused_to_be_modified_list;
  ric_actions_confirmed_for_removal_list_l  ric_actions_confirmed_for_removal_list;
  ric_actions_refused_to_be_remd_list_l     ric_actions_refused_to_be_remd_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionModificationConfirm ::= SEQUENCE
using ric_sub_mod_confirm_s = elementary_procedure_option<ric_sub_mod_confirm_ies_container>;

// RICsubscriptionModificationFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_mod_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_mod_fail_ies_container {
  bool               crit_diagnostics_present = false;
  ric_request_id_s   ric_request_id;
  uint16_t           ran_function_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionModificationFailure ::= SEQUENCE
using ric_sub_mod_fail_s = elementary_procedure_option<ric_sub_mod_fail_ies_container>;

// RICsubscriptionModificationRefuse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_mod_refuse_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_mod_refuse_ies_container {
  bool               crit_diagnostics_present = false;
  ric_request_id_s   ric_request_id;
  uint16_t           ran_function_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionModificationRefuse ::= SEQUENCE
using ric_sub_mod_refuse_s = elementary_procedure_option<ric_sub_mod_refuse_ies_container>;

// RICsubscriptionModificationRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_mod_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_mod_request_ies_container {
  bool                                      ric_event_trigger_definition_to_be_modified_present = false;
  bool                                      ric_actions_to_be_remd_for_mod_list_present         = false;
  bool                                      ric_actions_to_be_modified_for_mod_list_present     = false;
  bool                                      ric_actions_to_be_added_for_mod_list_present        = false;
  ric_request_id_s                          ric_request_id;
  uint16_t                                  ran_function_id;
  unbounded_octstring<true>                 ric_event_trigger_definition_to_be_modified;
  ric_actions_to_be_remd_for_mod_list_l     ric_actions_to_be_remd_for_mod_list;
  ric_actions_to_be_modified_for_mod_list_l ric_actions_to_be_modified_for_mod_list;
  ric_actions_to_be_added_for_mod_list_l    ric_actions_to_be_added_for_mod_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionModificationRequest ::= SEQUENCE
using ric_sub_mod_request_s = elementary_procedure_option<ric_sub_mod_request_ies_container>;

// RICsubscriptionModificationRequired-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_mod_required_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_mod_required_ies_container {
  bool                                       ric_actions_required_to_be_modified_list_present = false;
  bool                                       ric_actions_required_to_be_remd_list_present     = false;
  ric_request_id_s                           ric_request_id;
  uint16_t                                   ran_function_id;
  ric_actions_required_to_be_modified_list_l ric_actions_required_to_be_modified_list;
  ric_actions_required_to_be_remd_list_l     ric_actions_required_to_be_remd_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionModificationRequired ::= SEQUENCE
using ric_sub_mod_required_s = elementary_procedure_option<ric_sub_mod_required_ies_container>;

// RICsubscriptionModificationResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_mod_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_mod_resp_ies_container {
  bool                                             ric_actions_remd_for_mod_list_present                  = false;
  bool                                             ric_actions_failed_to_be_remd_for_mod_list_present     = false;
  bool                                             ric_actions_modified_for_mod_list_present              = false;
  bool                                             ric_actions_failed_to_be_modified_for_mod_list_present = false;
  bool                                             ric_actions_added_for_mod_list_present                 = false;
  bool                                             ric_actions_failed_to_be_added_for_mod_list_present    = false;
  ric_request_id_s                                 ric_request_id;
  uint16_t                                         ran_function_id;
  ric_actions_remd_for_mod_list_l                  ric_actions_remd_for_mod_list;
  ric_actions_failed_to_be_remd_for_mod_list_l     ric_actions_failed_to_be_remd_for_mod_list;
  ric_actions_modified_for_mod_list_l              ric_actions_modified_for_mod_list;
  ric_actions_failed_to_be_modified_for_mod_list_l ric_actions_failed_to_be_modified_for_mod_list;
  ric_actions_added_for_mod_list_l                 ric_actions_added_for_mod_list;
  ric_actions_failed_to_be_added_for_mod_list_l    ric_actions_failed_to_be_added_for_mod_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionModificationResponse ::= SEQUENCE
using ric_sub_mod_resp_s = elementary_procedure_option<ric_sub_mod_resp_ies_container>;

// RICsubscriptionRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_request_ies_container {
  bool                     ric_sub_start_time_present = false;
  bool                     ric_sub_end_time_present   = false;
  ric_request_id_s         ric_request_id;
  uint16_t                 ran_function_id;
  ric_sub_details_s        ric_sub_details;
  fixed_octstring<8, true> ric_sub_start_time;
  fixed_octstring<8, true> ric_sub_end_time;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionRequest ::= SEQUENCE
using ric_sub_request_s = elementary_procedure_option<ric_sub_request_ies_container>;

// RICsubscriptionResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_resp_ies_container {
  bool                           ric_actions_not_admitted_present = false;
  ric_request_id_s               ric_request_id;
  uint16_t                       ran_function_id;
  ric_action_admitted_list_l     ric_actions_admitted;
  ric_action_not_admitted_list_l ric_actions_not_admitted;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionResponse ::= SEQUENCE
using ric_sub_resp_s = elementary_procedure_option<ric_sub_resp_ies_container>;

// RICsubscriptionStateControlFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_state_ctrl_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_state_ctrl_fail_ies_container {
  bool               crit_diagnostics_present = false;
  ric_request_id_s   ric_request_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionStateControlFailure ::= SEQUENCE
using ric_sub_state_ctrl_fail_s = elementary_procedure_option<ric_sub_state_ctrl_fail_ies_container>;

// RICsubscriptionStateControlRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_state_ctrl_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_state_ctrl_request_ies_container {
  ric_request_id_s               ric_request_id;
  ran_function_state_ctrl_list_l ran_function_state_ctrl_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionStateControlRequest ::= SEQUENCE
using ric_sub_state_ctrl_request_s = elementary_procedure_option<ric_sub_state_ctrl_request_ies_container>;

// RICsubscriptionStateControlResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct ric_sub_state_ctrl_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ric_sub_state_ctrl_resp_ies_container {
  ric_request_id_s                  ric_request_id;
  ran_function_state_confirm_list_l ran_function_state_confirm_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RICsubscriptionStateControlResponse ::= SEQUENCE
using ric_sub_state_ctrl_resp_s = elementary_procedure_option<ric_sub_state_ctrl_resp_ies_container>;

// ResetRequestIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct reset_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct reset_request_ies_container {
  uint16_t transaction_id;
  cause_c  cause;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetRequest ::= SEQUENCE
using reset_request_s = elementary_procedure_option<reset_request_ies_container>;

// ResetResponseIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
struct reset_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct reset_resp_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           transaction_id;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetResponse ::= SEQUENCE
using reset_resp_s = elementary_procedure_option<reset_resp_ies_container>;

} // namespace e2ap
} // namespace asn1

extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_addition_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_addition_ack_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_removal_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_removal_ack_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_upd_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::e2node_component_cfg_upd_ack_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ran_functions_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ran_functions_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_functions_id_cause_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_functions_accept_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_load_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_function_load_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ric_sub_load_confirm_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_function_load_confirm_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ric_sub_load_request_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_function_load_request_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ric_sub_suspended_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_function_state_confirm_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ran_function_state_ctrl_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ric_sub_list_with_cause_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::e2ap::ric_sub_audit_action_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::e2ap::ric_sub_audit_list_l::value_type>::ops;
