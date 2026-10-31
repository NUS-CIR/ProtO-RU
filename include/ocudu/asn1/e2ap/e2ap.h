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

#include "ocudu/asn1/asn1_ap_utils.h"

namespace asn1 {
namespace e2ap {

struct ric_sub_request_ies_container;
using ric_sub_request_s = elementary_procedure_option<ric_sub_request_ies_container>;
struct ric_sub_audit_request_ies_container;
using ric_sub_audit_request_s = elementary_procedure_option<ric_sub_audit_request_ies_container>;
struct ric_sub_delete_request_ies_container;
using ric_sub_delete_request_s = elementary_procedure_option<ric_sub_delete_request_ies_container>;
struct ric_sub_mod_request_ies_container;
using ric_sub_mod_request_s = elementary_procedure_option<ric_sub_mod_request_ies_container>;
struct ric_sub_mod_required_ies_container;
using ric_sub_mod_required_s = elementary_procedure_option<ric_sub_mod_required_ies_container>;
struct ric_sub_state_ctrl_request_ies_container;
using ric_sub_state_ctrl_request_s = elementary_procedure_option<ric_sub_state_ctrl_request_ies_container>;
struct ric_query_request_ies_container;
using ric_query_request_s = elementary_procedure_option<ric_query_request_ies_container>;
struct ric_service_upd_ies_container;
using ric_service_upd_s = elementary_procedure_option<ric_service_upd_ies_container>;
struct ric_assist_request_ies_container;
using ric_assist_request_s = elementary_procedure_option<ric_assist_request_ies_container>;
struct ric_ctrl_request_ies_container;
using ric_ctrl_request_s = elementary_procedure_option<ric_ctrl_request_ies_container>;
struct ric_service_load_status_request_ies_container;
using ric_service_load_status_request_s = elementary_procedure_option<ric_service_load_status_request_ies_container>;
struct e2setup_request_ies_container;
using e2setup_request_s = elementary_procedure_option<e2setup_request_ies_container>;
struct e2node_cfg_upd_ies_container;
using e2node_cfg_upd_s = elementary_procedure_option<e2node_cfg_upd_ies_container>;
struct e2conn_upd_ies_container;
using e2conn_upd_s = elementary_procedure_option<e2conn_upd_ies_container>;
struct reset_request_ies_container;
using reset_request_s = elementary_procedure_option<reset_request_ies_container>;
struct e2_removal_request_ies_container;
using e2_removal_request_s = elementary_procedure_option<e2_removal_request_ies_container>;
struct e2_res_status_request_ies_container;
using e2_res_status_request_s = elementary_procedure_option<e2_res_status_request_ies_container>;
struct ric_assist_ind_ies_container;
using ric_assist_ind_s = elementary_procedure_option<ric_assist_ind_ies_container>;
struct ric_assist_halt_ies_container;
using ric_assist_halt_s = elementary_procedure_option<ric_assist_halt_ies_container>;
struct ric_ind_ies_container;
using ric_ind_s = elementary_procedure_option<ric_ind_ies_container>;
struct ric_service_query_ies_container;
using ric_service_query_s = elementary_procedure_option<ric_service_query_ies_container>;
struct ric_service_load_upd_ies_container;
using ric_service_load_upd_s = elementary_procedure_option<ric_service_load_upd_ies_container>;
struct error_ind_ies_container;
using error_ind_s = elementary_procedure_option<error_ind_ies_container>;
struct ric_sub_delete_required_ies_container;
using ric_sub_delete_required_s = elementary_procedure_option<ric_sub_delete_required_ies_container>;
struct e2_res_status_upd_ies_container;
using e2_res_status_upd_s = elementary_procedure_option<e2_res_status_upd_ies_container>;
struct ric_sub_resp_ies_container;
using ric_sub_resp_s = elementary_procedure_option<ric_sub_resp_ies_container>;
struct ric_sub_audit_resp_ies_container;
using ric_sub_audit_resp_s = elementary_procedure_option<ric_sub_audit_resp_ies_container>;
struct ric_sub_delete_resp_ies_container;
using ric_sub_delete_resp_s = elementary_procedure_option<ric_sub_delete_resp_ies_container>;
struct ric_sub_mod_resp_ies_container;
using ric_sub_mod_resp_s = elementary_procedure_option<ric_sub_mod_resp_ies_container>;
struct ric_sub_mod_confirm_ies_container;
using ric_sub_mod_confirm_s = elementary_procedure_option<ric_sub_mod_confirm_ies_container>;
struct ric_sub_state_ctrl_resp_ies_container;
using ric_sub_state_ctrl_resp_s = elementary_procedure_option<ric_sub_state_ctrl_resp_ies_container>;
struct ric_query_resp_ies_container;
using ric_query_resp_s = elementary_procedure_option<ric_query_resp_ies_container>;
struct ric_service_upd_ack_ies_container;
using ric_service_upd_ack_s = elementary_procedure_option<ric_service_upd_ack_ies_container>;
struct ric_assist_resp_ies_container;
using ric_assist_resp_s = elementary_procedure_option<ric_assist_resp_ies_container>;
struct ric_ctrl_ack_ies_container;
using ric_ctrl_ack_s = elementary_procedure_option<ric_ctrl_ack_ies_container>;
struct ric_service_load_status_resp_ies_container;
using ric_service_load_status_resp_s = elementary_procedure_option<ric_service_load_status_resp_ies_container>;
struct e2setup_resp_ies_container;
using e2setup_resp_s = elementary_procedure_option<e2setup_resp_ies_container>;
struct e2node_cfg_upd_ack_ies_container;
using e2node_cfg_upd_ack_s = elementary_procedure_option<e2node_cfg_upd_ack_ies_container>;
struct e2conn_upd_ack_ies_container;
using e2conn_upd_ack_s = elementary_procedure_option<e2conn_upd_ack_ies_container>;
struct reset_resp_ies_container;
using reset_resp_s = elementary_procedure_option<reset_resp_ies_container>;
struct e2_removal_resp_ies_container;
using e2_removal_resp_s = elementary_procedure_option<e2_removal_resp_ies_container>;
struct e2_res_status_resp_ies_container;
using e2_res_status_resp_s = elementary_procedure_option<e2_res_status_resp_ies_container>;
struct ric_sub_fail_ies_container;
using ric_sub_fail_s = elementary_procedure_option<ric_sub_fail_ies_container>;
struct ric_sub_audit_fail_ies_container;
using ric_sub_audit_fail_s = elementary_procedure_option<ric_sub_audit_fail_ies_container>;
struct ric_sub_delete_fail_ies_container;
using ric_sub_delete_fail_s = elementary_procedure_option<ric_sub_delete_fail_ies_container>;
struct ric_sub_mod_fail_ies_container;
using ric_sub_mod_fail_s = elementary_procedure_option<ric_sub_mod_fail_ies_container>;
struct ric_sub_mod_refuse_ies_container;
using ric_sub_mod_refuse_s = elementary_procedure_option<ric_sub_mod_refuse_ies_container>;
struct ric_sub_state_ctrl_fail_ies_container;
using ric_sub_state_ctrl_fail_s = elementary_procedure_option<ric_sub_state_ctrl_fail_ies_container>;
struct ric_query_fail_ies_container;
using ric_query_fail_s = elementary_procedure_option<ric_query_fail_ies_container>;
struct ric_service_upd_fail_ies_container;
using ric_service_upd_fail_s = elementary_procedure_option<ric_service_upd_fail_ies_container>;
struct ric_assist_fail_ies_container;
using ric_assist_fail_s = elementary_procedure_option<ric_assist_fail_ies_container>;
struct ric_ctrl_fail_ies_container;
using ric_ctrl_fail_s = elementary_procedure_option<ric_ctrl_fail_ies_container>;
struct ric_service_load_status_fail_ies_container;
using ric_service_load_status_fail_s = elementary_procedure_option<ric_service_load_status_fail_ies_container>;
struct e2setup_fail_ies_container;
using e2setup_fail_s = elementary_procedure_option<e2setup_fail_ies_container>;
struct e2node_cfg_upd_fail_ies_container;
using e2node_cfg_upd_fail_s = elementary_procedure_option<e2node_cfg_upd_fail_ies_container>;
struct e2conn_upd_fail_ies_container;
using e2conn_upd_fail_s = elementary_procedure_option<e2conn_upd_fail_ies_container>;
struct e2_removal_fail_ies_container;
using e2_removal_fail_s = elementary_procedure_option<e2_removal_fail_ies_container>;
struct e2_res_status_fail_ies_container;
using e2_res_status_fail_s = elementary_procedure_option<e2_res_status_fail_ies_container>;

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// ProtocolIE-FieldPair{E2AP-PROTOCOL-IES-PAIR : IEsSetParam} ::= SEQUENCE{{E2AP-PROTOCOL-IES-PAIR}}
template <class ies_set_paramT_>
struct protocol_ie_field_pair_s {
  uint32_t                                 id = 0;
  crit_e                                   first_crit;
  typename ies_set_paramT_::first_value_c  first_value;
  crit_e                                   second_crit;
  typename ies_set_paramT_::second_value_c second_value;

  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  bool          load_info_obj(const uint32_t& id_);
};

// ProtocolIE-ContainerPair{E2AP-PROTOCOL-IES-PAIR : IEsSetParam} ::= SEQUENCE (SIZE (0..65535)) OF
// ProtocolIE-FieldPair{E2AP-PROTOCOL-IES-PAIR : IEsSetParam}
template <class ies_set_paramT_>
using protocol_ie_container_pair_l = dyn_seq_of<protocol_ie_field_pair_s<ies_set_paramT_>, 0, 65535, true>;

// E2AP-ELEMENTARY-PROCEDURES ::= OBJECT SET OF E2AP-ELEMENTARY-PROCEDURE
struct e2ap_elem_procs_o {
  // InitiatingMessage ::= OPEN TYPE
  struct init_msg_c {
    struct types_opts {
      enum options {
        ric_sub_request,
        ric_sub_audit_request,
        ric_sub_delete_request,
        ric_sub_mod_request,
        ric_sub_mod_required,
        ric_sub_state_ctrl_request,
        ric_query_request,
        ric_service_upd,
        ric_assist_request,
        ric_ctrl_request,
        ric_service_load_status_request,
        e2setup_request,
        e2node_cfg_upd,
        e2conn_upd,
        reset_request,
        e2_removal_request,
        e2_res_status_request,
        ric_assist_ind,
        ric_assist_halt,
        ric_ind,
        ric_service_query,
        ric_service_load_upd,
        error_ind,
        ric_sub_delete_required,
        e2_res_status_upd,
        nulltype
      } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    init_msg_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_request_s&                       ric_sub_request();
    ric_sub_audit_request_s&                 ric_sub_audit_request();
    ric_sub_delete_request_s&                ric_sub_delete_request();
    ric_sub_mod_request_s&                   ric_sub_mod_request();
    ric_sub_mod_required_s&                  ric_sub_mod_required();
    ric_sub_state_ctrl_request_s&            ric_sub_state_ctrl_request();
    ric_query_request_s&                     ric_query_request();
    ric_service_upd_s&                       ric_service_upd();
    ric_assist_request_s&                    ric_assist_request();
    ric_ctrl_request_s&                      ric_ctrl_request();
    ric_service_load_status_request_s&       ric_service_load_status_request();
    e2setup_request_s&                       e2setup_request();
    e2node_cfg_upd_s&                        e2node_cfg_upd();
    e2conn_upd_s&                            e2conn_upd();
    reset_request_s&                         reset_request();
    e2_removal_request_s&                    e2_removal_request();
    e2_res_status_request_s&                 e2_res_status_request();
    ric_assist_ind_s&                        ric_assist_ind();
    ric_assist_halt_s&                       ric_assist_halt();
    ric_ind_s&                               ric_ind();
    ric_service_query_s&                     ric_service_query();
    ric_service_load_upd_s&                  ric_service_load_upd();
    error_ind_s&                             error_ind();
    ric_sub_delete_required_s&               ric_sub_delete_required();
    e2_res_status_upd_s&                     e2_res_status_upd();
    const ric_sub_request_s&                 ric_sub_request() const;
    const ric_sub_audit_request_s&           ric_sub_audit_request() const;
    const ric_sub_delete_request_s&          ric_sub_delete_request() const;
    const ric_sub_mod_request_s&             ric_sub_mod_request() const;
    const ric_sub_mod_required_s&            ric_sub_mod_required() const;
    const ric_sub_state_ctrl_request_s&      ric_sub_state_ctrl_request() const;
    const ric_query_request_s&               ric_query_request() const;
    const ric_service_upd_s&                 ric_service_upd() const;
    const ric_assist_request_s&              ric_assist_request() const;
    const ric_ctrl_request_s&                ric_ctrl_request() const;
    const ric_service_load_status_request_s& ric_service_load_status_request() const;
    const e2setup_request_s&                 e2setup_request() const;
    const e2node_cfg_upd_s&                  e2node_cfg_upd() const;
    const e2conn_upd_s&                      e2conn_upd() const;
    const reset_request_s&                   reset_request() const;
    const e2_removal_request_s&              e2_removal_request() const;
    const e2_res_status_request_s&           e2_res_status_request() const;
    const ric_assist_ind_s&                  ric_assist_ind() const;
    const ric_assist_halt_s&                 ric_assist_halt() const;
    const ric_ind_s&                         ric_ind() const;
    const ric_service_query_s&               ric_service_query() const;
    const ric_service_load_upd_s&            ric_service_load_upd() const;
    const error_ind_s&                       error_ind() const;
    const ric_sub_delete_required_s&         ric_sub_delete_required() const;
    const e2_res_status_upd_s&               e2_res_status_upd() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };
  // SuccessfulOutcome ::= OPEN TYPE
  struct successful_outcome_c {
    struct types_opts {
      enum options {
        ric_sub_resp,
        ric_sub_audit_resp,
        ric_sub_delete_resp,
        ric_sub_mod_resp,
        ric_sub_mod_confirm,
        ric_sub_state_ctrl_resp,
        ric_query_resp,
        ric_service_upd_ack,
        ric_assist_resp,
        ric_ctrl_ack,
        ric_service_load_status_resp,
        e2setup_resp,
        e2node_cfg_upd_ack,
        e2conn_upd_ack,
        reset_resp,
        e2_removal_resp,
        e2_res_status_resp,
        nulltype
      } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    successful_outcome_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_resp_s&                       ric_sub_resp();
    ric_sub_audit_resp_s&                 ric_sub_audit_resp();
    ric_sub_delete_resp_s&                ric_sub_delete_resp();
    ric_sub_mod_resp_s&                   ric_sub_mod_resp();
    ric_sub_mod_confirm_s&                ric_sub_mod_confirm();
    ric_sub_state_ctrl_resp_s&            ric_sub_state_ctrl_resp();
    ric_query_resp_s&                     ric_query_resp();
    ric_service_upd_ack_s&                ric_service_upd_ack();
    ric_assist_resp_s&                    ric_assist_resp();
    ric_ctrl_ack_s&                       ric_ctrl_ack();
    ric_service_load_status_resp_s&       ric_service_load_status_resp();
    e2setup_resp_s&                       e2setup_resp();
    e2node_cfg_upd_ack_s&                 e2node_cfg_upd_ack();
    e2conn_upd_ack_s&                     e2conn_upd_ack();
    reset_resp_s&                         reset_resp();
    e2_removal_resp_s&                    e2_removal_resp();
    e2_res_status_resp_s&                 e2_res_status_resp();
    const ric_sub_resp_s&                 ric_sub_resp() const;
    const ric_sub_audit_resp_s&           ric_sub_audit_resp() const;
    const ric_sub_delete_resp_s&          ric_sub_delete_resp() const;
    const ric_sub_mod_resp_s&             ric_sub_mod_resp() const;
    const ric_sub_mod_confirm_s&          ric_sub_mod_confirm() const;
    const ric_sub_state_ctrl_resp_s&      ric_sub_state_ctrl_resp() const;
    const ric_query_resp_s&               ric_query_resp() const;
    const ric_service_upd_ack_s&          ric_service_upd_ack() const;
    const ric_assist_resp_s&              ric_assist_resp() const;
    const ric_ctrl_ack_s&                 ric_ctrl_ack() const;
    const ric_service_load_status_resp_s& ric_service_load_status_resp() const;
    const e2setup_resp_s&                 e2setup_resp() const;
    const e2node_cfg_upd_ack_s&           e2node_cfg_upd_ack() const;
    const e2conn_upd_ack_s&               e2conn_upd_ack() const;
    const reset_resp_s&                   reset_resp() const;
    const e2_removal_resp_s&              e2_removal_resp() const;
    const e2_res_status_resp_s&           e2_res_status_resp() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };
  // UnsuccessfulOutcome ::= OPEN TYPE
  struct unsuccessful_outcome_c {
    struct types_opts {
      enum options {
        ric_sub_fail,
        ric_sub_audit_fail,
        ric_sub_delete_fail,
        ric_sub_mod_fail,
        ric_sub_mod_refuse,
        ric_sub_state_ctrl_fail,
        ric_query_fail,
        ric_service_upd_fail,
        ric_assist_fail,
        ric_ctrl_fail,
        ric_service_load_status_fail,
        e2setup_fail,
        e2node_cfg_upd_fail,
        e2conn_upd_fail,
        e2_removal_fail,
        e2_res_status_fail,
        nulltype
      } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    unsuccessful_outcome_c() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ric_sub_fail_s&                       ric_sub_fail();
    ric_sub_audit_fail_s&                 ric_sub_audit_fail();
    ric_sub_delete_fail_s&                ric_sub_delete_fail();
    ric_sub_mod_fail_s&                   ric_sub_mod_fail();
    ric_sub_mod_refuse_s&                 ric_sub_mod_refuse();
    ric_sub_state_ctrl_fail_s&            ric_sub_state_ctrl_fail();
    ric_query_fail_s&                     ric_query_fail();
    ric_service_upd_fail_s&               ric_service_upd_fail();
    ric_assist_fail_s&                    ric_assist_fail();
    ric_ctrl_fail_s&                      ric_ctrl_fail();
    ric_service_load_status_fail_s&       ric_service_load_status_fail();
    e2setup_fail_s&                       e2setup_fail();
    e2node_cfg_upd_fail_s&                e2node_cfg_upd_fail();
    e2conn_upd_fail_s&                    e2conn_upd_fail();
    e2_removal_fail_s&                    e2_removal_fail();
    e2_res_status_fail_s&                 e2_res_status_fail();
    const ric_sub_fail_s&                 ric_sub_fail() const;
    const ric_sub_audit_fail_s&           ric_sub_audit_fail() const;
    const ric_sub_delete_fail_s&          ric_sub_delete_fail() const;
    const ric_sub_mod_fail_s&             ric_sub_mod_fail() const;
    const ric_sub_mod_refuse_s&           ric_sub_mod_refuse() const;
    const ric_sub_state_ctrl_fail_s&      ric_sub_state_ctrl_fail() const;
    const ric_query_fail_s&               ric_query_fail() const;
    const ric_service_upd_fail_s&         ric_service_upd_fail() const;
    const ric_assist_fail_s&              ric_assist_fail() const;
    const ric_ctrl_fail_s&                ric_ctrl_fail() const;
    const ric_service_load_status_fail_s& ric_service_load_status_fail() const;
    const e2setup_fail_s&                 e2setup_fail() const;
    const e2node_cfg_upd_fail_s&          e2node_cfg_upd_fail() const;
    const e2conn_upd_fail_s&              e2conn_upd_fail() const;
    const e2_removal_fail_s&              e2_removal_fail() const;
    const e2_res_status_fail_s&           e2_res_status_fail() const;

  private:
    types             type_;
    choice_buffer_ptr c;
  };

  // members lookup methods
  static uint16_t               idx_to_proc_code(uint32_t idx);
  static bool                   is_proc_code_valid(const uint16_t& proc_code);
  static init_msg_c             get_init_msg(const uint16_t& proc_code);
  static successful_outcome_c   get_successful_outcome(const uint16_t& proc_code);
  static unsuccessful_outcome_c get_unsuccessful_outcome(const uint16_t& proc_code);
  static crit_e                 get_crit(const uint16_t& proc_code);
};

// InitiatingMessage ::= SEQUENCE{{E2AP-ELEMENTARY-PROCEDURE}}
struct init_msg_s {
  uint16_t                      proc_code = 0;
  crit_e                        crit;
  e2ap_elem_procs_o::init_msg_c value;

  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  bool          load_info_obj(const uint16_t& proc_code_);
};

// SuccessfulOutcome ::= SEQUENCE{{E2AP-ELEMENTARY-PROCEDURE}}
struct successful_outcome_s {
  uint16_t                                proc_code = 0;
  crit_e                                  crit;
  e2ap_elem_procs_o::successful_outcome_c value;

  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  bool          load_info_obj(const uint16_t& proc_code_);
};

// UnsuccessfulOutcome ::= SEQUENCE{{E2AP-ELEMENTARY-PROCEDURE}}
struct unsuccessful_outcome_s {
  uint16_t                                  proc_code = 0;
  crit_e                                    crit;
  e2ap_elem_procs_o::unsuccessful_outcome_c value;

  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  bool          load_info_obj(const uint16_t& proc_code_);
};

// E2AP-PDU ::= CHOICE
struct e2ap_pdu_c {
  struct types_opts {
    enum options { init_msg, successful_outcome, unsuccessful_outcome, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  e2ap_pdu_c() = default;
  e2ap_pdu_c(const e2ap_pdu_c& other);
  e2ap_pdu_c& operator=(const e2ap_pdu_c& other);
  ~e2ap_pdu_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  init_msg_s& init_msg()
  {
    assert_choice_type(types::init_msg, type_, "E2AP-PDU");
    return c.get<init_msg_s>();
  }
  successful_outcome_s& successful_outcome()
  {
    assert_choice_type(types::successful_outcome, type_, "E2AP-PDU");
    return c.get<successful_outcome_s>();
  }
  unsuccessful_outcome_s& unsuccessful_outcome()
  {
    assert_choice_type(types::unsuccessful_outcome, type_, "E2AP-PDU");
    return c.get<unsuccessful_outcome_s>();
  }
  const init_msg_s& init_msg() const
  {
    assert_choice_type(types::init_msg, type_, "E2AP-PDU");
    return c.get<init_msg_s>();
  }
  const successful_outcome_s& successful_outcome() const
  {
    assert_choice_type(types::successful_outcome, type_, "E2AP-PDU");
    return c.get<successful_outcome_s>();
  }
  const unsuccessful_outcome_s& unsuccessful_outcome() const
  {
    assert_choice_type(types::unsuccessful_outcome, type_, "E2AP-PDU");
    return c.get<unsuccessful_outcome_s>();
  }
  init_msg_s&             set_init_msg();
  successful_outcome_s&   set_successful_outcome();
  unsuccessful_outcome_s& set_unsuccessful_outcome();

private:
  types                                                                     type_;
  choice_buffer_t<init_msg_s, successful_outcome_s, unsuccessful_outcome_s> c;

  void destroy_();
};

} // namespace e2ap
} // namespace asn1
