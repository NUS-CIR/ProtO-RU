// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/*******************************************************************************
 *
 *                     3GPP TS ASN1 XNAP v18.7.0 (2025-12)
 *
 ******************************************************************************/

#pragma once

#include "ocudu/asn1/xnap/common.h"
#include "ocudu/asn1/xnap/xnap_ies.h"

namespace asn1 {
namespace xnap {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// AccessAndMobilityIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct access_and_mob_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct access_and_mob_ind_ies_container {
  bool                                   ra_report_present                            = false;
  bool                                   successful_ho_report_info_present            = false;
  bool                                   successful_pscell_change_report_info_present = false;
  bool                                   dl_lbt_fail_info_list_present                = false;
  ra_report_l                            ra_report;
  successful_ho_report_info_l            successful_ho_report_info;
  successful_pscell_change_report_info_l successful_pscell_change_report_info;
  dl_lbt_fail_info_list_l                dl_lbt_fail_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AccessAndMobilityIndication ::= SEQUENCE
using access_and_mob_ind_s = elementary_procedure_option<access_and_mob_ind_ies_container>;

// SSBsActivated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ss_bs_activ_item_ext_ies_o = protocol_ext_empty_o;

using ss_bs_activ_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBsActivated-Item ::= SEQUENCE
struct ss_bs_activ_item_s {
  bool                               ie_exts_present = false;
  uint8_t                            ssb_idx         = 0;
  ss_bs_activ_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ActivatedNRCellsAndSSBs-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using activ_nr_cells_and_ss_bs_item_ext_ies_o = protocol_ext_empty_o;

using activ_nr_cells_and_ss_bs_item_ext_ies_container = protocol_ext_container_empty_l;

// ActivatedNRCellsAndSSBs-Item ::= SEQUENCE
struct activ_nr_cells_and_ss_bs_item_s {
  using ss_bs_activ_list_l_ = dyn_array<ss_bs_activ_item_s>;

  // member variables
  bool                                            ie_exts_present = false;
  nr_cgi_s                                        nr_cgi;
  ss_bs_activ_list_l_                             ss_bs_activ_list;
  activ_nr_cells_and_ss_bs_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ActivatedNRCellsAndSSBsList ::= SEQUENCE (SIZE (1..16384)) OF ActivatedNRCellsAndSSBs-Item
using activ_nr_cells_and_ss_bs_list_l = dyn_array<activ_nr_cells_and_ss_bs_item_s>;

// ActivatedServedCells-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct activ_served_cells_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { activ_nr_cells_and_ss_bs_list, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::activ_nr_cells_and_ss_bs_list; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    activ_nr_cells_and_ss_bs_list_l&       activ_nr_cells_and_ss_bs_list() { return c; }
    const activ_nr_cells_and_ss_bs_list_l& activ_nr_cells_and_ss_bs_list() const { return c; }

  private:
    activ_nr_cells_and_ss_bs_list_l c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// ActivatedServedCells ::= CHOICE
struct activ_served_cells_c {
  using nr_cells_l_     = dyn_array<nr_cgi_s>;
  using e_utra_cells_l_ = dyn_array<e_utra_cgi_s>;
  struct types_opts {
    enum options { nr_cells, e_utra_cells, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  activ_served_cells_c() = default;
  activ_served_cells_c(const activ_served_cells_c& other);
  activ_served_cells_c& operator=(const activ_served_cells_c& other);
  ~activ_served_cells_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  nr_cells_l_& nr_cells()
  {
    assert_choice_type(types::nr_cells, type_, "ActivatedServedCells");
    return c.get<nr_cells_l_>();
  }
  e_utra_cells_l_& e_utra_cells()
  {
    assert_choice_type(types::e_utra_cells, type_, "ActivatedServedCells");
    return c.get<e_utra_cells_l_>();
  }
  protocol_ie_single_container_s<activ_served_cells_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ActivatedServedCells");
    return c.get<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>();
  }
  const nr_cells_l_& nr_cells() const
  {
    assert_choice_type(types::nr_cells, type_, "ActivatedServedCells");
    return c.get<nr_cells_l_>();
  }
  const e_utra_cells_l_& e_utra_cells() const
  {
    assert_choice_type(types::e_utra_cells, type_, "ActivatedServedCells");
    return c.get<e_utra_cells_l_>();
  }
  const protocol_ie_single_container_s<activ_served_cells_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ActivatedServedCells");
    return c.get<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>();
  }
  nr_cells_l_&                                                  set_nr_cells();
  e_utra_cells_l_&                                              set_e_utra_cells();
  protocol_ie_single_container_s<activ_served_cells_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                       type_;
  choice_buffer_t<e_utra_cells_l_, nr_cells_l_, protocol_ie_single_container_s<activ_served_cells_ext_ies_o>> c;

  void destroy_();
};

// QoSFlowsActivityNotifyItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qos_flows_activity_notify_item_ext_ies_o = protocol_ext_empty_o;

using qos_flows_activity_notify_item_ext_ies_container = protocol_ext_container_empty_l;

// QoSFlowsActivityNotifyItem ::= SEQUENCE
struct qos_flows_activity_notify_item_s {
  bool                                             ie_exts_present = false;
  uint8_t                                          qos_flow_id     = 0;
  user_plane_traffic_activity_report_e             pdu_session_level_up_activityreport;
  qos_flows_activity_notify_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsActivityNotifyList ::= SEQUENCE (SIZE (1..64)) OF QoSFlowsActivityNotifyItem
using qos_flows_activity_notify_list_l = dyn_array<qos_flows_activity_notify_item_s>;

// PDUSessionResourcesActivityNotify-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_activity_notify_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_activity_notify_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourcesActivityNotify-Item ::= SEQUENCE
struct pdu_session_res_activity_notify_item_s {
  bool                                                   pdu_session_level_up_activityreport_present = false;
  bool                                                   ie_exts_present                             = false;
  uint16_t                                               pdu_session_id                              = 0;
  user_plane_traffic_activity_report_e                   pdu_session_level_up_activityreport;
  qos_flows_activity_notify_list_l                       qos_flows_activity_notify_list;
  pdu_session_res_activity_notify_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourcesActivityNotifyList ::= SEQUENCE (SIZE (1..256)) OF PDUSessionResourcesActivityNotify-Item
using pdu_session_res_activity_notify_list_l = dyn_array<pdu_session_res_activity_notify_item_s>;

// ActivityNotification-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct activity_notif_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct activity_notif_ies_container {
  bool                                   user_plane_traffic_activity_report_present   = false;
  bool                                   pdu_session_res_activity_notify_list_present = false;
  bool                                   ran_paging_fail_present                      = false;
  uint64_t                               m_ng_ran_node_ue_xnap_id;
  uint64_t                               s_ng_ran_node_ue_xnap_id;
  user_plane_traffic_activity_report_e   user_plane_traffic_activity_report;
  pdu_session_res_activity_notify_list_l pdu_session_res_activity_notify_list;
  ran_paging_fail_e                      ran_paging_fail;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ActivityNotification ::= SEQUENCE
using activity_notif_s = elementary_procedure_option<activity_notif_ies_container>;

// AvailableFastMCGRecoveryViaSRB3 ::= ENUMERATED
struct available_fast_mcg_recovery_via_srb3_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using available_fast_mcg_recovery_via_srb3_e = enumerated<available_fast_mcg_recovery_via_srb3_opts, true>;

// BearersSubjectToCounterCheck-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using bearers_subject_to_counter_check_item_ext_ies_o = protocol_ext_empty_o;

using bearers_subject_to_counter_check_item_ext_ies_container = protocol_ext_container_empty_l;

// BearersSubjectToCounterCheck-Item ::= SEQUENCE
struct bearers_subject_to_counter_check_item_s {
  bool                                                    ie_exts_present = false;
  uint8_t                                                 drb_id          = 1;
  uint64_t                                                ul_count        = 0;
  uint64_t                                                dl_count        = 0;
  bearers_subject_to_counter_check_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BearersSubjectToCounterCheck-List ::= SEQUENCE (SIZE (1..32)) OF BearersSubjectToCounterCheck-Item
using bearers_subject_to_counter_check_list_l = dyn_array<bearers_subject_to_counter_check_item_s>;

// BoundaryNodeCellsList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using boundary_node_cells_list_item_ext_ies_o = protocol_ext_empty_o;

using boundary_node_cells_list_item_ext_ies_container = protocol_ext_container_empty_l;

// BoundaryNodeCellsList-Item ::= SEQUENCE
struct boundary_node_cells_list_item_s {
  bool                                            ie_exts_present = false;
  iab_cell_info_s                                 boundary_node_cell_info;
  boundary_node_cells_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BoundaryNodeCellsList ::= SEQUENCE (SIZE (1..512)) OF BoundaryNodeCellsList-Item
using boundary_node_cells_list_l = dyn_array<boundary_node_cells_list_item_s>;

// CPCCancel-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct cp_c_cancel_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cp_c_cancel_ies_container {
  bool                    cause_present = false;
  uint64_t                m_ng_ran_node_ue_xnap_id;
  uint64_t                s_ng_ran_node_ue_xnap_id;
  cause_c                 cause;
  global_ng_ran_node_id_c target_s_ng_ran_node_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPCCancel ::= SEQUENCE
using cp_c_cancel_s = elementary_procedure_option<cp_c_cancel_ies_container>;

// CellActivationFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct cell_activation_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cell_activation_fail_ies_container {
  bool               crit_diagnostics_present       = false;
  bool               interface_instance_ind_present = false;
  uint16_t           activation_id_for_cell_activation;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;
  uint16_t           interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellActivationFailure ::= SEQUENCE
using cell_activation_fail_s = elementary_procedure_option<cell_activation_fail_ies_container>;

// SSBsToBeActivated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ss_bs_to_be_activ_item_ext_ies_o = protocol_ext_empty_o;

using ss_bs_to_be_activ_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBsToBeActivated-Item ::= SEQUENCE
struct ss_bs_to_be_activ_item_s {
  bool                                     ie_exts_present = false;
  uint8_t                                  ssb_idx         = 0;
  ss_bs_to_be_activ_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ToBeActivatedNRCellsAndSSBs-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using to_be_activ_nr_cells_and_ss_bs_item_ext_ies_o = protocol_ext_empty_o;

using to_be_activ_nr_cells_and_ss_bs_item_ext_ies_container = protocol_ext_container_empty_l;

// ToBeActivatedNRCellsAndSSBs-Item ::= SEQUENCE
struct to_be_activ_nr_cells_and_ss_bs_item_s {
  using ss_bstobe_activ_list_l_ = dyn_array<ss_bs_to_be_activ_item_s>;

  // member variables
  bool                                                  ie_exts_present = false;
  nr_cgi_s                                              nr_cgi;
  ss_bstobe_activ_list_l_                               ss_bstobe_activ_list;
  to_be_activ_nr_cells_and_ss_bs_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ToBeActivatedNRCellsAndSSBsList ::= SEQUENCE (SIZE (1..16384)) OF ToBeActivatedNRCellsAndSSBs-Item
using to_be_activ_nr_cells_and_ss_bs_list_l = dyn_array<to_be_activ_nr_cells_and_ss_bs_item_s>;

// ServedCellsToActivate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct served_cells_to_activ_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { nr_cells_and_ss_bs_list, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::nr_cells_and_ss_bs_list; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    to_be_activ_nr_cells_and_ss_bs_list_l&       nr_cells_and_ss_bs_list() { return c; }
    const to_be_activ_nr_cells_and_ss_bs_list_l& nr_cells_and_ss_bs_list() const { return c; }

  private:
    to_be_activ_nr_cells_and_ss_bs_list_l c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// ServedCellsToActivate ::= CHOICE
struct served_cells_to_activ_c {
  using nr_cells_l_     = dyn_array<nr_cgi_s>;
  using e_utra_cells_l_ = dyn_array<e_utra_cgi_s>;
  struct types_opts {
    enum options { nr_cells, e_utra_cells, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  served_cells_to_activ_c() = default;
  served_cells_to_activ_c(const served_cells_to_activ_c& other);
  served_cells_to_activ_c& operator=(const served_cells_to_activ_c& other);
  ~served_cells_to_activ_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  nr_cells_l_& nr_cells()
  {
    assert_choice_type(types::nr_cells, type_, "ServedCellsToActivate");
    return c.get<nr_cells_l_>();
  }
  e_utra_cells_l_& e_utra_cells()
  {
    assert_choice_type(types::e_utra_cells, type_, "ServedCellsToActivate");
    return c.get<e_utra_cells_l_>();
  }
  protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ServedCellsToActivate");
    return c.get<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>();
  }
  const nr_cells_l_& nr_cells() const
  {
    assert_choice_type(types::nr_cells, type_, "ServedCellsToActivate");
    return c.get<nr_cells_l_>();
  }
  const e_utra_cells_l_& e_utra_cells() const
  {
    assert_choice_type(types::e_utra_cells, type_, "ServedCellsToActivate");
    return c.get<e_utra_cells_l_>();
  }
  const protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ServedCellsToActivate");
    return c.get<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>();
  }
  nr_cells_l_&                                                     set_nr_cells();
  e_utra_cells_l_&                                                 set_e_utra_cells();
  protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                          type_;
  choice_buffer_t<e_utra_cells_l_, nr_cells_l_, protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>> c;

  void destroy_();
};

// CellActivationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct cell_activation_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cell_activation_request_ies_container {
  bool                    interface_instance_ind_present = false;
  served_cells_to_activ_c served_cells_to_activ;
  uint16_t                activation_id_for_cell_activation;
  uint16_t                interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellActivationRequest ::= SEQUENCE
using cell_activation_request_s = elementary_procedure_option<cell_activation_request_ies_container>;

// CellActivationResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct cell_activation_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cell_activation_resp_ies_container {
  bool                 crit_diagnostics_present       = false;
  bool                 interface_instance_ind_present = false;
  activ_served_cells_c activ_served_cells;
  uint16_t             activation_id_for_cell_activation;
  crit_diagnostics_s   crit_diagnostics;
  uint16_t             interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellActivationResponse ::= SEQUENCE
using cell_activation_resp_s = elementary_procedure_option<cell_activation_resp_ies_container>;

// CellTrafficTraceIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct cell_traffic_trace_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cell_traffic_trace_ies_container {
  bool                                                  privacy_ind_present                 = false;
  bool                                                  trace_collection_entity_uri_present = false;
  uint64_t                                              m_ng_ran_node_ue_xnap_id;
  uint64_t                                              s_ng_ran_node_ue_xnap_id;
  fixed_octstring<8, true>                              ng_ran_trace_id;
  bounded_bitstring<1, 160, true, true>                 trace_collection_entity_ip_address;
  privacy_ind_e                                         privacy_ind;
  visible_string<0, MAX_ASN_STRING_LENGTH, false, true> trace_collection_entity_uri;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellTrafficTrace ::= SEQUENCE
using cell_traffic_trace_s = elementary_procedure_option<cell_traffic_trace_ies_container>;

// ConditionalHandoverCancel-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct conditional_ho_cancel_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct conditional_ho_cancel_ies_container {
  bool                     target_cells_to_cancel_present           = false;
  bool                     conditional_recfg_to_cancel_list_present = false;
  uint64_t                 source_ng_ran_node_ue_xnap_id;
  uint64_t                 target_ng_ran_node_ue_xnap_id;
  cause_c                  cause;
  target_cell_list_l       target_cells_to_cancel;
  conditional_recfg_list_l conditional_recfg_to_cancel_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ConditionalHandoverCancel ::= SEQUENCE
using conditional_ho_cancel_s = elementary_procedure_option<conditional_ho_cancel_ies_container>;

// Configuration-rejected-by-M-NG-RANNode-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cfg_rejected_by_m_ng_ran_node_ext_ies_o = protocol_ext_empty_o;

using cfg_rejected_by_m_ng_ran_node_ext_ies_container = protocol_ext_container_empty_l;

// Configuration-rejected-by-M-NG-RANNode ::= SEQUENCE
struct cfg_rejected_by_m_ng_ran_node_s {
  bool                                            ie_exts_present = false;
  cause_c                                         cause;
  unbounded_octstring<true>                       m_ng_ran_node_to_s_ng_ran_node_container;
  cfg_rejected_by_m_ng_ran_node_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Configuration-successfully-applied-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct cfg_successfully_applied_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cfg_successfully_applied_ext_ies_container {
  bool     sk_counter_present = false;
  uint32_t sk_counter;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Configuration-successfully-applied ::= SEQUENCE
struct cfg_successfully_applied_s {
  bool                                       ie_exts_present = false;
  unbounded_octstring<true>                  m_ng_ran_node_to_s_ng_ran_node_container;
  cfg_successfully_applied_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ConfigurationUpdate-gNB ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct cfg_upd_gnb_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// ConfigurationUpdate-ng-eNB ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct cfg_upd_ng_enb_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cfg_upd_gnb_container {
  bool                               served_cells_to_upd_nr_present           = false;
  bool                               cell_assist_info_nr_present              = false;
  bool                               cell_assist_info_eutra_present           = false;
  bool                               served_cell_specific_info_req_nr_present = false;
  served_cells_to_upd_nr_s           served_cells_to_upd_nr;
  cell_assist_info_nr_c              cell_assist_info_nr;
  cell_assist_info_eutra_c           cell_assist_info_eutra;
  served_cell_specific_info_req_nr_l served_cell_specific_info_req_nr;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

struct cfg_upd_ng_enb_container {
  bool                         served_cells_to_upd_e_utra_present = false;
  bool                         cell_assist_info_nr_present        = false;
  bool                         cell_assist_info_eutra_present     = false;
  served_cells_to_upd_e_utra_s served_cells_to_upd_e_utra;
  cell_assist_info_nr_c        cell_assist_info_nr;
  cell_assist_info_eutra_c     cell_assist_info_eutra;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellsToUpdateInitiatingNodeChoice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using served_cells_to_upd_init_node_choice_ext_ies_o = protocol_ies_empty_o;

// ConfigurationUpdateInitiatingNodeChoice ::= CHOICE
struct cfg_upd_init_node_choice_c {
  struct types_opts {
    enum options { gnb, ng_enb, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cfg_upd_init_node_choice_c() = default;
  cfg_upd_init_node_choice_c(const cfg_upd_init_node_choice_c& other);
  cfg_upd_init_node_choice_c& operator=(const cfg_upd_init_node_choice_c& other);
  ~cfg_upd_init_node_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  cfg_upd_gnb_container& gnb()
  {
    assert_choice_type(types::gnb, type_, "ConfigurationUpdateInitiatingNodeChoice");
    return c.get<cfg_upd_gnb_container>();
  }
  cfg_upd_ng_enb_container& ng_enb()
  {
    assert_choice_type(types::ng_enb, type_, "ConfigurationUpdateInitiatingNodeChoice");
    return c.get<cfg_upd_ng_enb_container>();
  }
  protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ConfigurationUpdateInitiatingNodeChoice");
    return c.get<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>();
  }
  const cfg_upd_gnb_container& gnb() const
  {
    assert_choice_type(types::gnb, type_, "ConfigurationUpdateInitiatingNodeChoice");
    return c.get<cfg_upd_gnb_container>();
  }
  const cfg_upd_ng_enb_container& ng_enb() const
  {
    assert_choice_type(types::ng_enb, type_, "ConfigurationUpdateInitiatingNodeChoice");
    return c.get<cfg_upd_ng_enb_container>();
  }
  const protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ConfigurationUpdateInitiatingNodeChoice");
    return c.get<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>();
  }
  cfg_upd_gnb_container&                                                          set_gnb();
  cfg_upd_ng_enb_container&                                                       set_ng_enb();
  protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<cfg_upd_gnb_container,
                  cfg_upd_ng_enb_container,
                  protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>
      c;

  void destroy_();
};

// DLDiscarding-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using dl_discarding_ext_ies_o = protocol_ext_empty_o;

using dl_discarding_ext_ies_container = protocol_ext_container_empty_l;

// DLDiscarding ::= SEQUENCE
struct dl_discarding_s {
  bool                                 ie_ext_present = false;
  drbs_subject_to_dl_discarding_list_l drbs_subject_to_dl_discarding;
  dl_discarding_ext_ies_container      ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataCollectionFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct data_collection_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct data_collection_fail_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           ngran_node1_meas_id;
  uint16_t           ngran_node2_meas_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataCollectionFailure ::= SEQUENCE
using data_collection_fail_s = elementary_procedure_option<data_collection_fail_ies_container>;

// DataCollectionRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct data_collection_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct data_collection_request_ies_container {
  bool                                      ngran_node2_meas_id_present                        = false;
  bool                                      report_characteristics_for_data_collection_present = false;
  bool                                      cell_to_report_for_data_collection_list_present    = false;
  bool                                      report_periodicity_for_data_collection_present     = false;
  bool                                      requested_prediction_time_present                  = false;
  bool                                      ue_trajectory_collection_cfg_present               = false;
  bool                                      ue_performance_collection_cfg_present              = false;
  uint16_t                                  ngran_node1_meas_id;
  uint16_t                                  ngran_node2_meas_id;
  regist_request_for_data_collection_e      regist_request_for_data_collection;
  fixed_bitstring<32, false, true>          report_characteristics_for_data_collection;
  cell_to_report_for_data_collection_list_l cell_to_report_for_data_collection_list;
  report_periodicity_for_data_collection_e  report_periodicity_for_data_collection;
  uint8_t                                   requested_prediction_time;
  ue_trajectory_collection_cfg_s            ue_trajectory_collection_cfg;
  ue_performance_collection_cfg_s           ue_performance_collection_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataCollectionRequest ::= SEQUENCE
using data_collection_request_s = elementary_procedure_option<data_collection_request_ies_container>;

// DataCollectionResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct data_collection_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct data_collection_resp_ies_container {
  bool                               node_meas_initiation_result_list_present = false;
  bool                               cell_meas_initiation_result_list_present = false;
  bool                               crit_diagnostics_present                 = false;
  uint16_t                           ngran_node1_meas_id;
  uint16_t                           ngran_node2_meas_id;
  node_meas_initiation_result_list_l node_meas_initiation_result_list;
  cell_meas_initiation_result_list_l cell_meas_initiation_result_list;
  crit_diagnostics_s                 crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataCollectionResponse ::= SEQUENCE
using data_collection_resp_s = elementary_procedure_option<data_collection_resp_ies_container>;

// DataCollectionUpdate-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct data_collection_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct data_collection_upd_ies_container {
  bool                                        cell_meas_result_for_data_collection_list_present = false;
  bool                                        ue_associated_info_result_list_present            = false;
  bool                                        node_associated_info_result_present               = false;
  uint16_t                                    ngran_node1_meas_id;
  uint16_t                                    ngran_node2_meas_id;
  cell_meas_result_for_data_collection_list_l cell_meas_result_for_data_collection_list;
  ue_associated_info_result_list_l            ue_associated_info_result_list;
  node_associated_info_result_s               node_associated_info_result;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataCollectionUpdate ::= SEQUENCE
using data_collection_upd_s = elementary_procedure_option<data_collection_upd_ies_container>;

// DeactivateTraceIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct deactiv_trace_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct deactiv_trace_ies_container {
  uint64_t                 m_ng_ran_node_ue_xnap_id;
  uint64_t                 s_ng_ran_node_ue_xnap_id;
  fixed_octstring<8, true> ng_ran_trace_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DeactivateTrace ::= SEQUENCE
using deactiv_trace_s = elementary_procedure_option<deactiv_trace_ies_container>;

// ResourceCoordRequest-ng-eNB-initiated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using res_coord_request_ng_enb_initiated_ext_ies_o = protocol_ext_empty_o;

using res_coord_request_ng_enb_initiated_ext_ies_container = protocol_ext_container_empty_l;

// ResourceCoordRequest-ng-eNB-initiated ::= SEQUENCE
struct res_coord_request_ng_enb_initiated_s {
  using listof_e_utra_cells_l_ = dyn_array<e_utra_cgi_s>;

  // member variables
  bool                                                 ie_exts_present = false;
  data_traffic_res_ind_s                               data_traffic_res_ind;
  uint16_t                                             spec_sharing_group_id = 1;
  listof_e_utra_cells_l_                               listof_e_utra_cells;
  res_coord_request_ng_enb_initiated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceCoordRequest-gNB-initiated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using res_coord_request_gnb_initiated_ext_ies_o = protocol_ext_empty_o;

using res_coord_request_gnb_initiated_ext_ies_container = protocol_ext_container_empty_l;

// ResourceCoordRequest-gNB-initiated ::= SEQUENCE
struct res_coord_request_gnb_initiated_s {
  using listof_e_utra_cells_l_ = dyn_array<e_utra_cgi_s>;
  using listof_nr_cells_l_     = dyn_array<nr_cgi_s>;

  // member variables
  bool                                              ie_exts_present = false;
  data_traffic_res_ind_s                            data_traffic_res_ind;
  listof_e_utra_cells_l_                            listof_e_utra_cells;
  uint16_t                                          spec_sharing_group_id = 1;
  listof_nr_cells_l_                                listof_nr_cells;
  res_coord_request_gnb_initiated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// InitiatingNodeType-ResourceCoordRequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using init_node_type_res_coord_request_ext_ies_o = protocol_ies_empty_o;

// InitiatingNodeType-ResourceCoordRequest ::= CHOICE
struct init_node_type_res_coord_request_c {
  struct types_opts {
    enum options { ng_enb, gnb, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  init_node_type_res_coord_request_c() = default;
  init_node_type_res_coord_request_c(const init_node_type_res_coord_request_c& other);
  init_node_type_res_coord_request_c& operator=(const init_node_type_res_coord_request_c& other);
  ~init_node_type_res_coord_request_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  res_coord_request_ng_enb_initiated_s& ng_enb()
  {
    assert_choice_type(types::ng_enb, type_, "InitiatingNodeType-ResourceCoordRequest");
    return c.get<res_coord_request_ng_enb_initiated_s>();
  }
  res_coord_request_gnb_initiated_s& gnb()
  {
    assert_choice_type(types::gnb, type_, "InitiatingNodeType-ResourceCoordRequest");
    return c.get<res_coord_request_gnb_initiated_s>();
  }
  protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "InitiatingNodeType-ResourceCoordRequest");
    return c.get<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>();
  }
  const res_coord_request_ng_enb_initiated_s& ng_enb() const
  {
    assert_choice_type(types::ng_enb, type_, "InitiatingNodeType-ResourceCoordRequest");
    return c.get<res_coord_request_ng_enb_initiated_s>();
  }
  const res_coord_request_gnb_initiated_s& gnb() const
  {
    assert_choice_type(types::gnb, type_, "InitiatingNodeType-ResourceCoordRequest");
    return c.get<res_coord_request_gnb_initiated_s>();
  }
  const protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "InitiatingNodeType-ResourceCoordRequest");
    return c.get<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>();
  }
  res_coord_request_ng_enb_initiated_s&                                       set_ng_enb();
  res_coord_request_gnb_initiated_s&                                          set_gnb();
  protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>,
                  res_coord_request_gnb_initiated_s,
                  res_coord_request_ng_enb_initiated_s>
      c;

  void destroy_();
};

// E-UTRA-NR-CellResourceCoordinationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct e_utra_nr_cell_res_coordination_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_utra_nr_cell_res_coordination_request_ies_container {
  bool                               interface_instance_ind_present = false;
  init_node_type_res_coord_request_c init_node_type_res_coord_request;
  uint16_t                           interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-UTRA-NR-CellResourceCoordinationRequest ::= SEQUENCE
using e_utra_nr_cell_res_coordination_request_s =
    elementary_procedure_option<e_utra_nr_cell_res_coordination_request_ies_container>;

// ResourceCoordResponse-ng-eNB-initiated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using res_coord_resp_ng_enb_initiated_ext_ies_o = protocol_ext_empty_o;

using res_coord_resp_ng_enb_initiated_ext_ies_container = protocol_ext_container_empty_l;

// ResourceCoordResponse-ng-eNB-initiated ::= SEQUENCE
struct res_coord_resp_ng_enb_initiated_s {
  using listof_e_utra_cells_l_ = dyn_array<e_utra_cgi_s>;

  // member variables
  bool                                              ie_exts_present = false;
  data_traffic_res_ind_s                            data_traffic_res_ind;
  uint16_t                                          spec_sharing_group_id = 1;
  listof_e_utra_cells_l_                            listof_e_utra_cells;
  res_coord_resp_ng_enb_initiated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceCoordResponse-gNB-initiated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using res_coord_resp_gnb_initiated_ext_ies_o = protocol_ext_empty_o;

using res_coord_resp_gnb_initiated_ext_ies_container = protocol_ext_container_empty_l;

// ResourceCoordResponse-gNB-initiated ::= SEQUENCE
struct res_coord_resp_gnb_initiated_s {
  using listof_nr_cells_l_ = dyn_array<nr_cgi_s>;

  // member variables
  bool                                           ie_exts_present = false;
  data_traffic_res_ind_s                         data_traffic_res_ind;
  uint16_t                                       spec_sharing_group_id = 1;
  listof_nr_cells_l_                             listof_nr_cells;
  res_coord_resp_gnb_initiated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RespondingNodeType-ResourceCoordResponse-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using responding_node_type_res_coord_resp_ext_ies_o = protocol_ies_empty_o;

// RespondingNodeType-ResourceCoordResponse ::= CHOICE
struct responding_node_type_res_coord_resp_c {
  struct types_opts {
    enum options { ng_enb, gnb, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  responding_node_type_res_coord_resp_c() = default;
  responding_node_type_res_coord_resp_c(const responding_node_type_res_coord_resp_c& other);
  responding_node_type_res_coord_resp_c& operator=(const responding_node_type_res_coord_resp_c& other);
  ~responding_node_type_res_coord_resp_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  res_coord_resp_ng_enb_initiated_s& ng_enb()
  {
    assert_choice_type(types::ng_enb, type_, "RespondingNodeType-ResourceCoordResponse");
    return c.get<res_coord_resp_ng_enb_initiated_s>();
  }
  res_coord_resp_gnb_initiated_s& gnb()
  {
    assert_choice_type(types::gnb, type_, "RespondingNodeType-ResourceCoordResponse");
    return c.get<res_coord_resp_gnb_initiated_s>();
  }
  protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "RespondingNodeType-ResourceCoordResponse");
    return c.get<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>();
  }
  const res_coord_resp_ng_enb_initiated_s& ng_enb() const
  {
    assert_choice_type(types::ng_enb, type_, "RespondingNodeType-ResourceCoordResponse");
    return c.get<res_coord_resp_ng_enb_initiated_s>();
  }
  const res_coord_resp_gnb_initiated_s& gnb() const
  {
    assert_choice_type(types::gnb, type_, "RespondingNodeType-ResourceCoordResponse");
    return c.get<res_coord_resp_gnb_initiated_s>();
  }
  const protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "RespondingNodeType-ResourceCoordResponse");
    return c.get<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>();
  }
  res_coord_resp_ng_enb_initiated_s&                                             set_ng_enb();
  res_coord_resp_gnb_initiated_s&                                                set_gnb();
  protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>,
                  res_coord_resp_gnb_initiated_s,
                  res_coord_resp_ng_enb_initiated_s>
      c;

  void destroy_();
};

// E-UTRA-NR-CellResourceCoordinationResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct e_utra_nr_cell_res_coordination_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_utra_nr_cell_res_coordination_resp_ies_container {
  bool                                  interface_instance_ind_present = false;
  responding_node_type_res_coord_resp_c responding_node_type_res_coord_resp;
  uint16_t                              interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-UTRA-NR-CellResourceCoordinationResponse ::= SEQUENCE
using e_utra_nr_cell_res_coordination_resp_s =
    elementary_procedure_option<e_utra_nr_cell_res_coordination_resp_ies_container>;

// FirstDLCount-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using first_dl_count_ext_ies_o = protocol_ext_empty_o;

using first_dl_count_ext_ies_container = protocol_ext_container_empty_l;

// FirstDLCount ::= SEQUENCE
struct first_dl_count_s {
  bool                                         ie_ext_present = false;
  drbs_subject_to_early_status_transfer_list_l drbs_subject_to_early_status_transfer;
  first_dl_count_ext_ies_container             ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ProcedureStageChoice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using proc_stage_choice_ext_ies_o = protocol_ies_empty_o;

// ProcedureStageChoice ::= CHOICE
struct proc_stage_choice_c {
  struct types_opts {
    enum options { first_dl_count, dl_discarding, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  proc_stage_choice_c() = default;
  proc_stage_choice_c(const proc_stage_choice_c& other);
  proc_stage_choice_c& operator=(const proc_stage_choice_c& other);
  ~proc_stage_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  first_dl_count_s& first_dl_count()
  {
    assert_choice_type(types::first_dl_count, type_, "ProcedureStageChoice");
    return c.get<first_dl_count_s>();
  }
  dl_discarding_s& dl_discarding()
  {
    assert_choice_type(types::dl_discarding, type_, "ProcedureStageChoice");
    return c.get<dl_discarding_s>();
  }
  protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ProcedureStageChoice");
    return c.get<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>();
  }
  const first_dl_count_s& first_dl_count() const
  {
    assert_choice_type(types::first_dl_count, type_, "ProcedureStageChoice");
    return c.get<first_dl_count_s>();
  }
  const dl_discarding_s& dl_discarding() const
  {
    assert_choice_type(types::dl_discarding, type_, "ProcedureStageChoice");
    return c.get<dl_discarding_s>();
  }
  const protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ProcedureStageChoice");
    return c.get<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>();
  }
  first_dl_count_s&                                            set_first_dl_count();
  dl_discarding_s&                                             set_dl_discarding();
  protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                           type_;
  choice_buffer_t<dl_discarding_s, first_dl_count_s, protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>> c;

  void destroy_();
};

// EarlyStatusTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct early_status_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct early_status_transfer_ies_container {
  uint64_t            source_ng_ran_node_ue_xnap_id;
  uint64_t            target_ng_ran_node_ue_xnap_id;
  proc_stage_choice_c proc_stage;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EarlyStatusTransfer ::= SEQUENCE
using early_status_transfer_s = elementary_procedure_option<early_status_transfer_ies_container>;

// ErrorIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct error_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct error_ind_ies_container {
  bool               old_ng_ran_node_ue_xnap_id_present = false;
  bool               new_ng_ran_node_ue_xnap_id_present = false;
  bool               cause_present                      = false;
  bool               crit_diagnostics_present           = false;
  bool               interface_instance_ind_present     = false;
  uint64_t           old_ng_ran_node_ue_xnap_id;
  uint64_t           new_ng_ran_node_ue_xnap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;
  uint16_t           interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ErrorIndication ::= SEQUENCE
using error_ind_s = elementary_procedure_option<error_ind_ies_container>;

// F1CTrafficTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct f1_c_traffic_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct f1_c_traffic_transfer_ies_container {
  uint64_t                  m_ng_ran_node_ue_xnap_id;
  uint64_t                  s_ng_ran_node_ue_xnap_id;
  unbounded_octstring<true> f1_c_traffic_container;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// F1CTrafficTransfer ::= SEQUENCE
using f1_c_traffic_transfer_s = elementary_procedure_option<f1_c_traffic_transfer_ies_container>;

// FailureIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct fail_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct fail_ind_ies_container {
  init_condition_fail_ind_c init_condition_fail_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FailureIndication ::= SEQUENCE
using fail_ind_s = elementary_procedure_option<fail_ind_ies_container>;

// FastMCGRecoveryRRCTransfer-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using fast_mcg_recovery_rrc_transfer_ext_ies_o = protocol_ext_empty_o;

using fast_mcg_recovery_rrc_transfer_ext_ies_container = protocol_ext_container_empty_l;

// FastMCGRecoveryRRCTransfer ::= SEQUENCE
struct fast_mcg_recovery_rrc_transfer_s {
  bool                                             ie_exts_present = false;
  unbounded_octstring<true>                        rrc_container;
  fast_mcg_recovery_rrc_transfer_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverCancel-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ho_cancel_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ho_cancel_ies_container {
  bool               target_ng_ran_node_ue_xnap_id_present = false;
  bool               target_cells_to_cancel_present        = false;
  uint64_t           source_ng_ran_node_ue_xnap_id;
  uint64_t           target_ng_ran_node_ue_xnap_id;
  cause_c            cause;
  target_cell_list_l target_cells_to_cancel;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverCancel ::= SEQUENCE
using ho_cancel_s = elementary_procedure_option<ho_cancel_ies_container>;

// HandoverPreparationFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ho_prep_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ho_prep_fail_ies_container {
  bool               crit_diagnostics_present                = false;
  bool               requested_target_cell_global_id_present = false;
  uint64_t           source_ng_ran_node_ue_xnap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;
  target_cgi_c       requested_target_cell_global_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverPreparationFailure ::= SEQUENCE
using ho_prep_fail_s = elementary_procedure_option<ho_prep_fail_ies_container>;

// HandoverReport-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ho_report_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ho_report_ies_container {
  bool                             re_establishment_cell_cgi_present = false;
  bool                             target_cellin_eutran_present      = false;
  bool                             source_cell_c_rnti_present        = false;
  bool                             mob_info_present                  = false;
  bool                             ue_rlf_report_container_present   = false;
  bool                             cho_cfg_present                   = false;
  bool                             target_cell_c_rnti_present        = false;
  bool                             time_since_fail_present           = false;
  ho_report_type_e                 ho_report_type;
  cause_c                          ho_cause;
  global_ng_ran_cell_id_s          source_cell_cgi;
  global_ng_ran_cell_id_s          target_cell_cgi;
  global_cell_id_s                 re_establishment_cell_cgi;
  unbounded_octstring<true>        target_cellin_eutran;
  fixed_bitstring<16, false, true> source_cell_c_rnti;
  fixed_bitstring<32, false, true> mob_info;
  ue_rlf_report_container_c        ue_rlf_report_container;
  cho_cfg_s                        cho_cfg;
  fixed_bitstring<16, false, true> target_cell_c_rnti;
  uint32_t                         time_since_fail;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverReport ::= SEQUENCE
using ho_report_s = elementary_procedure_option<ho_report_ies_container>;

// UEContextInfoHORequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ue_context_info_ho_request_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_info_ho_request_ext_ies_container {
  bool                                five_gc_mob_restrict_list_container_present    = false;
  bool                                nr_ue_sidelink_aggr_max_bit_rate_present       = false;
  bool                                lte_ue_sidelink_aggr_max_bit_rate_present      = false;
  bool                                mdt_plmn_list_present                          = false;
  bool                                ue_radio_cap_id_present                        = false;
  bool                                mbs_session_info_list_present                  = false;
  bool                                five_g_pro_se_ue_pc5_aggr_max_bit_rate_present = false;
  bool                                ue_slice_max_bit_rate_list_present             = false;
  bool                                nr_a2_x_ue_pc5_aggr_max_bit_rate_present       = false;
  bool                                ltea2_x_ue_pc5_aggr_max_bit_rate_present       = false;
  unbounded_octstring<true>           five_gc_mob_restrict_list_container;
  nr_ue_sidelink_aggr_max_bit_rate_s  nr_ue_sidelink_aggr_max_bit_rate;
  lte_ue_sidelink_aggr_max_bit_rate_s lte_ue_sidelink_aggr_max_bit_rate;
  mdt_plmn_list_l                     mdt_plmn_list;
  unbounded_octstring<true>           ue_radio_cap_id;
  mbs_session_info_list_l             mbs_session_info_list;
  nr_ue_sidelink_aggr_max_bit_rate_s  five_g_pro_se_ue_pc5_aggr_max_bit_rate;
  ue_slice_max_bit_rate_list_l        ue_slice_max_bit_rate_list;
  nr_ue_sidelink_aggr_max_bit_rate_s  nr_a2_x_ue_pc5_aggr_max_bit_rate;
  lte_ue_sidelink_aggr_max_bit_rate_s ltea2_x_ue_pc5_aggr_max_bit_rate;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextInfoHORequest ::= SEQUENCE
struct ue_context_info_ho_request_s {
  bool                                         idx_to_rat_freq_sel_prio_present = false;
  bool                                         location_report_info_present     = false;
  bool                                         mrl_present                      = false;
  bool                                         ie_exts_present                  = false;
  uint64_t                                     ng_c_ue_ref                      = 0;
  cp_transport_layer_info_c                    cp_tnl_info_source;
  ue_security_cap_s                            ue_security_cap;
  as_security_info_s                           security_info;
  uint16_t                                     idx_to_rat_freq_sel_prio = 1;
  ue_aggr_max_bit_rate_s                       ue_ambr;
  pdu_session_res_to_be_setup_list_l           pdu_session_res_to_be_setup_list;
  unbounded_octstring<true>                    rrc_context;
  location_report_info_s                       location_report_info;
  mob_restrict_list_s                          mrl;
  ue_context_info_ho_request_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextRefAtSN-HORequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_context_ref_at_sn_ho_request_ext_ies_o = protocol_ext_empty_o;

using ue_context_ref_at_sn_ho_request_ext_ies_container = protocol_ext_container_empty_l;

// UEContextRefAtSN-HORequest ::= SEQUENCE
struct ue_context_ref_at_sn_ho_request_s {
  bool                                              ie_exts_present = false;
  global_ng_ran_node_id_c                           global_ng_ran_node_id;
  uint64_t                                          sn_ng_ran_node_ue_xnap_id = 0;
  ue_context_ref_at_sn_ho_request_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ho_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ho_request_ies_container {
  bool                                   trace_activation_present                     = false;
  bool                                   masked_imeisv_present                        = false;
  bool                                   ue_context_ref_at_sn_ho_request_present      = false;
  bool                                   ch_oinfo_req_present                         = false;
  bool                                   nr_v2x_services_authorized_present           = false;
  bool                                   ltev2x_services_authorized_present           = false;
  bool                                   pc5_qos_params_present                       = false;
  bool                                   mob_info_present                             = false;
  bool                                   ue_history_info_from_the_ue_present          = false;
  bool                                   iab_node_ind_present                         = false;
  bool                                   no_pdu_session_ind_present                   = false;
  bool                                   time_sync_assist_info_present                = false;
  bool                                   q_mcc_onfig_info_present                     = false;
  bool                                   five_g_pro_se_authorized_present             = false;
  bool                                   five_g_pro_se_pc5_qos_params_present         = false;
  bool                                   iab_authorization_status_present             = false;
  bool                                   dl_lbt_fail_info_request_present             = false;
  bool                                   aerial_ue_sub_info_present                   = false;
  bool                                   nr_a2_x_services_authorized_present          = false;
  bool                                   ltea2_x_services_authorized_present          = false;
  bool                                   a2_xpc5_qos_params_present                   = false;
  bool                                   cell_based_ue_trajectory_prediction_present  = false;
  bool                                   data_collection_id_present                   = false;
  bool                                   candidate_relay_ue_info_list_present         = false;
  bool                                   source_sn_to_target_sn_qmc_info_present      = false;
  bool                                   mobile_iab_authorization_status_present      = false;
  bool                                   sl_positioning_ranging_services_info_present = false;
  uint64_t                               source_ng_ran_node_ue_xnap_id;
  cause_c                                cause;
  target_cgi_c                           target_cell_global_id;
  guami_s                                guami;
  ue_context_info_ho_request_s           ue_context_info_ho_request;
  trace_activation_s                     trace_activation;
  fixed_bitstring<64, false, true>       masked_imeisv;
  ue_history_info_l                      ue_history_info;
  ue_context_ref_at_sn_ho_request_s      ue_context_ref_at_sn_ho_request;
  ch_oinfo_req_s                         ch_oinfo_req;
  nr_v2x_services_authorized_s           nr_v2x_services_authorized;
  ltev2x_services_authorized_s           ltev2x_services_authorized;
  pc5_qos_params_s                       pc5_qos_params;
  fixed_bitstring<32, false, true>       mob_info;
  ue_history_info_from_the_ue_c          ue_history_info_from_the_ue;
  iab_node_ind_e                         iab_node_ind;
  no_pdu_session_ind_e                   no_pdu_session_ind;
  time_sync_assist_info_s                time_sync_assist_info;
  q_mcc_onfig_info_s                     q_mcc_onfig_info;
  five_g_pro_se_authorized_s             five_g_pro_se_authorized;
  five_g_pro_se_pc5_qos_params_s         five_g_pro_se_pc5_qos_params;
  iab_authorization_status_e             iab_authorization_status;
  dl_lbt_fail_info_request_e             dl_lbt_fail_info_request;
  aerial_ue_sub_info_e                   aerial_ue_sub_info;
  nr_a2_x_services_authorized_s          nr_a2_x_services_authorized;
  ltea2_x_services_authorized_s          ltea2_x_services_authorized;
  a2_xpc5_qos_params_s                   a2_xpc5_qos_params;
  cell_based_ue_trajectory_prediction_l  cell_based_ue_trajectory_prediction;
  data_collection_id_s                   data_collection_id;
  candidate_relay_ue_info_list_l         candidate_relay_ue_info_list;
  q_mcc_onfig_info_s                     source_sn_to_target_sn_qmc_info;
  mobile_iab_authorization_status_e      mobile_iab_authorization_status;
  sl_positioning_ranging_services_info_s sl_positioning_ranging_services_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverRequest ::= SEQUENCE
using ho_request_s = elementary_procedure_option<ho_request_ies_container>;

// HandoverRequestAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ho_request_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ho_request_ack_ies_container {
  bool                                pdu_session_res_not_admitted_list_present = false;
  bool                                ue_context_kept_ind_present               = false;
  bool                                crit_diagnostics_present                  = false;
  bool                                drbs_transferred_to_mn_present            = false;
  bool                                daps_resp_info_list_present               = false;
  bool                                ch_oinfo_ack_present                      = false;
  bool                                mbs_session_info_resp_list_present        = false;
  bool                                rrc_cfg_ind_present                       = false;
  bool                                pdu_setbased_handling_ind_present         = false;
  uint64_t                            source_ng_ran_node_ue_xnap_id;
  uint64_t                            target_ng_ran_node_ue_xnap_id;
  pdu_session_res_admitted_list_l     pdu_session_res_admitted_list;
  pdu_session_res_not_admitted_list_l pdu_session_res_not_admitted_list;
  unbounded_octstring<true>           target2_source_ng_ran_node_transp_container;
  ue_context_kept_ind_e               ue_context_kept_ind;
  crit_diagnostics_s                  crit_diagnostics;
  drb_list_l                          drbs_transferred_to_mn;
  daps_resp_info_list_l               daps_resp_info_list;
  ch_oinfo_ack_s                      ch_oinfo_ack;
  mbs_session_info_resp_list_l        mbs_session_info_resp_list;
  rrc_cfg_ind_e                       rrc_cfg_ind;
  pdu_setbased_handling_ind_e         pdu_setbased_handling_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverRequestAcknowledge ::= SEQUENCE
using ho_request_ack_s = elementary_procedure_option<ho_request_ack_ies_container>;

// HandoverSuccess-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ho_success_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ho_success_ies_container {
  bool         accessed_pscell_id_present = false;
  uint64_t     source_ng_ran_node_ue_xnap_id;
  uint64_t     target_ng_ran_node_ue_xnap_id;
  target_cgi_c requested_target_cell_global_id;
  nr_cgi_s     accessed_pscell_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverSuccess ::= SEQUENCE
using ho_success_s = elementary_procedure_option<ho_success_ies_container>;

// ParentNodeCellsList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using parent_node_cells_list_item_ext_ies_o = protocol_ext_empty_o;

using parent_node_cells_list_item_ext_ies_container = protocol_ext_container_empty_l;

// ParentNodeCellsList-Item ::= SEQUENCE
struct parent_node_cells_list_item_s {
  bool                                          ie_exts_present = false;
  iab_cell_info_s                               parent_node_cell_info;
  parent_node_cells_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ParentNodeCellsList ::= SEQUENCE (SIZE (1..32)) OF ParentNodeCellsList-Item
using parent_node_cells_list_l = dyn_array<parent_node_cells_list_item_s>;

// IABResourceCoordinationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct iab_res_coordination_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_res_coordination_request_ies_container {
  bool                       boundary_node_cells_list_present = false;
  bool                       parent_node_cells_list_present   = false;
  uint64_t                   f1_terminating_iab_donor_ue_xnap_id;
  uint64_t                   non_f1_terminating_iab_donor_ue_xnap_id;
  boundary_node_cells_list_l boundary_node_cells_list;
  parent_node_cells_list_l   parent_node_cells_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABResourceCoordinationRequest ::= SEQUENCE
using iab_res_coordination_request_s = elementary_procedure_option<iab_res_coordination_request_ies_container>;

// IABResourceCoordinationResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct iab_res_coordination_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_res_coordination_resp_ies_container {
  bool                       boundary_node_cells_list_present = false;
  bool                       parent_node_cells_list_present   = false;
  uint64_t                   f1_terminating_iab_donor_ue_xnap_id;
  uint64_t                   non_f1_terminating_iab_donor_ue_xnap_id;
  boundary_node_cells_list_l boundary_node_cells_list;
  parent_node_cells_list_l   parent_node_cells_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABResourceCoordinationResponse ::= SEQUENCE
using iab_res_coordination_resp_s = elementary_procedure_option<iab_res_coordination_resp_ies_container>;

// IABTNLAddressToBeReleased-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_tnl_address_to_be_released_item_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_address_to_be_released_item_ext_ies_container = protocol_ext_container_empty_l;

// IABTNLAddressToBeReleased-Item ::= SEQUENCE
struct iab_tnl_address_to_be_released_item_s {
  bool                                                  ie_exts_present = false;
  iab_tnl_address_c                                     iab_tnl_address;
  iab_tnl_address_to_be_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTNLAddressToBeReleasedList ::= SEQUENCE (SIZE (1..1024)) OF IABTNLAddressToBeReleased-Item
using iab_tnl_address_to_be_released_list_l = dyn_array<iab_tnl_address_to_be_released_item_s>;

// IABTransportMigrationManagementReject-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct iab_transport_migration_management_reject_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_transport_migration_management_reject_ies_container {
  bool               crit_diagnostics_present = false;
  uint64_t           f1_terminating_iab_donor_ue_xnap_id;
  uint64_t           non_f1_terminating_iab_donor_ue_xnap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTransportMigrationManagementReject ::= SEQUENCE
using iab_transport_migration_management_reject_s =
    elementary_procedure_option<iab_transport_migration_management_reject_ies_container>;

// TrafficToBeAdded-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_to_be_added_item_ext_ies_o = protocol_ext_empty_o;

using traffic_to_be_added_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficToBeAdded-Item ::= SEQUENCE
struct traffic_to_be_added_item_s {
  bool                                       f1_terminating_topology_bh_info_present = false;
  bool                                       ie_exts_present                         = false;
  uint16_t                                   traffic_idx                             = 1;
  traffic_profile_c                          traffic_profile;
  f1_terminating_topology_bh_info_s          f1_terminating_topology_bh_info;
  traffic_to_be_added_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficToBeAddedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficToBeAdded-Item
using traffic_to_be_added_list_l = dyn_array<traffic_to_be_added_item_s>;

// TrafficToBeModified-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using traffic_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficToBeModified-Item ::= SEQUENCE
struct traffic_to_be_modified_item_s {
  bool                                          traffic_profile_present                 = false;
  bool                                          f1_terminating_topology_bh_info_present = false;
  bool                                          ie_ext_present                          = false;
  uint16_t                                      traffic_idx                             = 1;
  traffic_profile_c                             traffic_profile;
  f1_terminating_topology_bh_info_s             f1_terminating_topology_bh_info;
  traffic_to_be_modified_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficToBeModifiedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficToBeModified-Item
using traffic_to_be_modified_list_l = dyn_array<traffic_to_be_modified_item_s>;

// IABTransportMigrationManagementRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct iab_transport_migration_management_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_transport_migration_management_request_ies_container {
  bool                             traffic_to_be_added_list_present    = false;
  bool                             traffic_to_be_modified_list_present = false;
  bool                             traffic_to_be_release_info_present  = false;
  bool                             iab_tnl_address_request_present     = false;
  bool                             iab_tnl_address_exception_present   = false;
  bool                             miab_mt_bap_address_present         = false;
  uint64_t                         f1_terminating_iab_donor_ue_xnap_id;
  uint64_t                         non_f1_terminating_iab_donor_ue_xnap_id;
  traffic_to_be_added_list_l       traffic_to_be_added_list;
  traffic_to_be_modified_list_l    traffic_to_be_modified_list;
  traffic_to_be_release_info_s     traffic_to_be_release_info;
  iab_tnl_address_request_s        iab_tnl_address_request;
  iab_tnl_address_exception_l      iab_tnl_address_exception;
  fixed_bitstring<10, false, true> miab_mt_bap_address;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTransportMigrationManagementRequest ::= SEQUENCE
using iab_transport_migration_management_request_s =
    elementary_procedure_option<iab_transport_migration_management_request_ies_container>;

// TrafficAdded-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_added_item_ext_ies_o = protocol_ext_empty_o;

using traffic_added_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficAdded-Item ::= SEQUENCE
struct traffic_added_item_s {
  bool                                  ie_exts_present = false;
  uint16_t                              traffic_idx     = 1;
  non_f1_terminating_topology_bh_info_s non_f1_terminating_topology_bh_info;
  traffic_added_item_ext_ies_container  ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficAddedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficAdded-Item
using traffic_added_list_l = dyn_array<traffic_added_item_s>;

// TrafficModified-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_modified_item_ext_ies_o = protocol_ext_empty_o;

using traffic_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficModified-Item ::= SEQUENCE
struct traffic_modified_item_s {
  bool                                    ie_exts_present = false;
  uint16_t                                traffic_idx     = 1;
  non_f1_terminating_topology_bh_info_s   non_f1_terminating_topology_bh_info;
  traffic_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficModifiedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficModified-Item
using traffic_modified_list_l = dyn_array<traffic_modified_item_s>;

// TrafficNotAdded-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_not_added_item_ext_ies_o = protocol_ext_empty_o;

using traffic_not_added_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficNotAdded-Item ::= SEQUENCE
struct traffic_not_added_item_s {
  bool                                     casue_present   = false;
  bool                                     ie_exts_present = false;
  uint16_t                                 traffic_idx     = 1;
  cause_c                                  casue;
  traffic_not_added_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficNotAddedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficNotAdded-Item
using traffic_not_added_list_l = dyn_array<traffic_not_added_item_s>;

// TrafficNotModified-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_not_modified_item_ext_ies_o = protocol_ext_empty_o;

using traffic_not_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficNotModified-Item ::= SEQUENCE
struct traffic_not_modified_item_s {
  bool                                        cause_present   = false;
  bool                                        ie_exts_present = false;
  uint16_t                                    traffic_idx     = 1;
  cause_c                                     cause;
  traffic_not_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficNotModifiedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficNotModified-Item
using traffic_not_modified_list_l = dyn_array<traffic_not_modified_item_s>;

// TrafficReleased-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_released_item_ext_ies_o = protocol_ext_empty_o;

using traffic_released_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficReleased-Item ::= SEQUENCE
struct traffic_released_item_s {
  bool                                    ie_exts_present = false;
  uint16_t                                traffic_idx     = 1;
  bh_info_list_l                          bh_info_list;
  traffic_released_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficReleasedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficReleased-Item
using traffic_released_list_l = dyn_array<traffic_released_item_s>;

// IABTransportMigrationManagementResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct iab_transport_migration_management_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_transport_migration_management_resp_ies_container {
  bool                        traffic_added_list_present        = false;
  bool                        traffic_modified_list_present     = false;
  bool                        traffic_not_added_list_present    = false;
  bool                        traffic_not_modified_list_present = false;
  bool                        iab_tnl_address_resp_present      = false;
  bool                        traffic_released_list_present     = false;
  uint64_t                    f1_terminating_iab_donor_ue_xnap_id;
  uint64_t                    non_f1_terminating_iab_donor_ue_xnap_id;
  traffic_added_list_l        traffic_added_list;
  traffic_modified_list_l     traffic_modified_list;
  traffic_not_added_list_l    traffic_not_added_list;
  traffic_not_modified_list_l traffic_not_modified_list;
  iab_tnl_address_resp_s      iab_tnl_address_resp;
  traffic_released_list_l     traffic_released_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTransportMigrationManagementResponse ::= SEQUENCE
using iab_transport_migration_management_resp_s =
    elementary_procedure_option<iab_transport_migration_management_resp_ies_container>;

// TrafficRequiredToBeModified-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_required_to_be_modified_item_ext_ies_o = protocol_ext_empty_o;

using traffic_required_to_be_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficRequiredToBeModified-Item ::= SEQUENCE
struct traffic_required_to_be_modified_item_s {
  bool                                                   ie_exts_present = false;
  uint16_t                                               traffic_idx     = 1;
  non_f1_terminating_topology_bh_info_s                  non_f1_terminating_topology_bh_info;
  traffic_required_to_be_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficRequiredToBeModifiedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficRequiredToBeModified-Item
using traffic_required_to_be_modified_list_l = dyn_array<traffic_required_to_be_modified_item_s>;

// IABTransportMigrationModificationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct iab_transport_migration_mod_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_transport_migration_mod_request_ies_container {
  bool                                   traffic_required_to_be_modified_list_present = false;
  bool                                   traffic_to_be_release_info_present           = false;
  bool                                   iab_tnl_address_to_be_added_present          = false;
  bool                                   iab_tnl_address_to_be_released_list_present  = false;
  bool                                   iab_authorization_status_present             = false;
  bool                                   mobile_iab_authorization_status_present      = false;
  uint64_t                               f1_terminating_iab_donor_ue_xnap_id;
  uint64_t                               non_f1_terminating_iab_donor_ue_xnap_id;
  traffic_required_to_be_modified_list_l traffic_required_to_be_modified_list;
  traffic_to_be_release_info_s           traffic_to_be_release_info;
  iab_tnl_address_resp_s                 iab_tnl_address_to_be_added;
  iab_tnl_address_to_be_released_list_l  iab_tnl_address_to_be_released_list;
  iab_authorization_status_e             iab_authorization_status;
  mobile_iab_authorization_status_e      mobile_iab_authorization_status;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTransportMigrationModificationRequest ::= SEQUENCE
using iab_transport_migration_mod_request_s =
    elementary_procedure_option<iab_transport_migration_mod_request_ies_container>;

// TrafficRequiredModified-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_required_modified_item_ext_ies_o = protocol_ext_empty_o;

using traffic_required_modified_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficRequiredModified-Item ::= SEQUENCE
struct traffic_required_modified_item_s {
  bool                                             ie_exts_present = false;
  uint16_t                                         traffic_idx     = 1;
  traffic_required_modified_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficRequiredModifiedList ::= SEQUENCE (SIZE (1..1024)) OF TrafficRequiredModified-Item
using traffic_required_modified_list_l = dyn_array<traffic_required_modified_item_s>;

// IABTransportMigrationModificationResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct iab_transport_migration_mod_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct iab_transport_migration_mod_resp_ies_container {
  bool                             traffic_required_modified_list_present = false;
  bool                             traffic_released_list_present          = false;
  uint64_t                         f1_terminating_iab_donor_ue_xnap_id;
  uint64_t                         non_f1_terminating_iab_donor_ue_xnap_id;
  traffic_required_modified_list_l traffic_required_modified_list;
  traffic_released_list_l          traffic_released_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTransportMigrationModificationResponse ::= SEQUENCE
using iab_transport_migration_mod_resp_s = elementary_procedure_option<iab_transport_migration_mod_resp_ies_container>;

// MobilityChangeAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct mob_change_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct mob_change_ack_ies_container {
  bool                    crit_diagnostics_present = false;
  global_ng_ran_cell_id_s ng_ran_node1_cell_id;
  global_ng_ran_cell_id_s ng_ran_node2_cell_id;
  crit_diagnostics_s      crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityChangeAcknowledge ::= SEQUENCE
using mob_change_ack_s = elementary_procedure_option<mob_change_ack_ies_container>;

// MobilityChangeFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct mob_change_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct mob_change_fail_ies_container {
  bool                                 mob_params_mod_range_present               = false;
  bool                                 crit_diagnostics_present                   = false;
  bool                                 ng_ran_node2_ssb_offsets_mod_range_present = false;
  global_ng_ran_cell_id_s              ng_ran_node1_cell_id;
  global_ng_ran_cell_id_s              ng_ran_node2_cell_id;
  cause_c                              cause;
  mob_params_mod_range_s               mob_params_mod_range;
  crit_diagnostics_s                   crit_diagnostics;
  ng_ran_node2_ssb_offsets_mod_range_l ng_ran_node2_ssb_offsets_mod_range;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityChangeFailure ::= SEQUENCE
using mob_change_fail_s = elementary_procedure_option<mob_change_fail_ies_container>;

// MobilityChangeRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct mob_change_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct mob_change_request_ies_container {
  bool                    ng_ran_node1_mob_params_present = false;
  bool                    ssb_offsets_list_present        = false;
  global_ng_ran_cell_id_s ng_ran_node1_cell_id;
  global_ng_ran_cell_id_s ng_ran_node2_cell_id;
  mob_params_info_s       ng_ran_node1_mob_params;
  mob_params_info_s       ng_ran_node2_proposed_mob_params;
  cause_c                 cause;
  ssb_offsets_list_l      ssb_offsets_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityChangeRequest ::= SEQUENCE
using mob_change_request_s = elementary_procedure_option<mob_change_request_ies_container>;

// NGRANNodeConfigurationUpdate-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ngran_node_cfg_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ngran_node_cfg_upd_ies_container {
  bool                         tai_support_list_present             = false;
  bool                         tnl_a_to_add_list_present            = false;
  bool                         tnl_a_to_rem_list_present            = false;
  bool                         tnl_a_to_upd_list_present            = false;
  bool                         global_ng_ran_node_id_present        = false;
  bool                         amf_region_info_to_add_present       = false;
  bool                         amf_region_info_to_delete_present    = false;
  bool                         interface_instance_ind_present       = false;
  bool                         tnl_cfg_info_present                 = false;
  bool                         coverage_mod_list_present            = false;
  bool                         local_ng_ran_node_id_present         = false;
  bool                         neighbour_ng_ran_node_list_present   = false;
  bool                         local_ng_ran_node_id_removal_present = false;
  tai_support_list_l           tai_support_list;
  cfg_upd_init_node_choice_c   cfg_upd_init_node_choice;
  tnl_a_to_add_list_l          tnl_a_to_add_list;
  tnl_a_to_rem_list_l          tnl_a_to_rem_list;
  tnl_a_to_upd_list_l          tnl_a_to_upd_list;
  global_ng_ran_node_id_c      global_ng_ran_node_id;
  amf_region_info_l            amf_region_info_to_add;
  amf_region_info_l            amf_region_info_to_delete;
  uint16_t                     interface_instance_ind;
  tnl_cfg_info_s               tnl_cfg_info;
  coverage_mod_list_l          coverage_mod_list;
  local_ng_ran_node_id_c       local_ng_ran_node_id;
  neighbour_ng_ran_node_list_l neighbour_ng_ran_node_list;
  local_ng_ran_node_id_c       local_ng_ran_node_id_removal;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NGRANNodeConfigurationUpdate ::= SEQUENCE
using ngran_node_cfg_upd_s = elementary_procedure_option<ngran_node_cfg_upd_ies_container>;

// RespondingNodeTypeConfigUpdateAck-ng-eNB-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct responding_node_type_cfg_upd_ack_ng_enb_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct responding_node_type_cfg_upd_ack_ng_enb_ext_ies_container {
  bool                                  list_of_served_cells_e_utra_present         = false;
  bool                                  partial_list_ind_eutra_present              = false;
  bool                                  cell_and_capacity_assist_info_eutra_present = false;
  served_cells_e_utra_l                 list_of_served_cells_e_utra;
  partial_list_ind_e                    partial_list_ind_eutra;
  cell_and_capacity_assist_info_eutra_s cell_and_capacity_assist_info_eutra;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RespondingNodeTypeConfigUpdateAck-ng-eNB ::= SEQUENCE
struct responding_node_type_cfg_upd_ack_ng_enb_s {
  bool                                                      ie_ext_present = false;
  responding_node_type_cfg_upd_ack_ng_enb_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RespondingNodeTypeConfigUpdateAck-gNB-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct responding_node_type_cfg_upd_ack_gnb_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct responding_node_type_cfg_upd_ack_gnb_ext_ies_container {
  bool                               partial_list_ind_nr_present              = false;
  bool                               cell_and_capacity_assist_info_nr_present = false;
  partial_list_ind_e                 partial_list_ind_nr;
  cell_and_capacity_assist_info_nr_s cell_and_capacity_assist_info_nr;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RespondingNodeTypeConfigUpdateAck-gNB ::= SEQUENCE
struct responding_node_type_cfg_upd_ack_gnb_s {
  bool                                                   ie_ext_present = false;
  served_cells_nr_l                                      served_nr_cells;
  responding_node_type_cfg_upd_ack_gnb_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RespondingNodeTypeConfigUpdateAck-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using responding_node_type_cfg_upd_ack_ext_ies_o = protocol_ies_empty_o;

// RespondingNodeTypeConfigUpdateAck ::= CHOICE
struct responding_node_type_cfg_upd_ack_c {
  struct types_opts {
    enum options { ng_enb, gnb, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  responding_node_type_cfg_upd_ack_c() = default;
  responding_node_type_cfg_upd_ack_c(const responding_node_type_cfg_upd_ack_c& other);
  responding_node_type_cfg_upd_ack_c& operator=(const responding_node_type_cfg_upd_ack_c& other);
  ~responding_node_type_cfg_upd_ack_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  responding_node_type_cfg_upd_ack_ng_enb_s& ng_enb()
  {
    assert_choice_type(types::ng_enb, type_, "RespondingNodeTypeConfigUpdateAck");
    return c.get<responding_node_type_cfg_upd_ack_ng_enb_s>();
  }
  responding_node_type_cfg_upd_ack_gnb_s& gnb()
  {
    assert_choice_type(types::gnb, type_, "RespondingNodeTypeConfigUpdateAck");
    return c.get<responding_node_type_cfg_upd_ack_gnb_s>();
  }
  protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "RespondingNodeTypeConfigUpdateAck");
    return c.get<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>();
  }
  const responding_node_type_cfg_upd_ack_ng_enb_s& ng_enb() const
  {
    assert_choice_type(types::ng_enb, type_, "RespondingNodeTypeConfigUpdateAck");
    return c.get<responding_node_type_cfg_upd_ack_ng_enb_s>();
  }
  const responding_node_type_cfg_upd_ack_gnb_s& gnb() const
  {
    assert_choice_type(types::gnb, type_, "RespondingNodeTypeConfigUpdateAck");
    return c.get<responding_node_type_cfg_upd_ack_gnb_s>();
  }
  const protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "RespondingNodeTypeConfigUpdateAck");
    return c.get<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>();
  }
  responding_node_type_cfg_upd_ack_ng_enb_s&                                  set_ng_enb();
  responding_node_type_cfg_upd_ack_gnb_s&                                     set_gnb();
  protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>,
                  responding_node_type_cfg_upd_ack_gnb_s,
                  responding_node_type_cfg_upd_ack_ng_enb_s>
      c;

  void destroy_();
};

// NGRANNodeConfigurationUpdateAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ngran_node_cfg_upd_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ngran_node_cfg_upd_ack_ies_container {
  bool                               tnl_a_setup_list_present             = false;
  bool                               tnl_a_failed_to_setup_list_present   = false;
  bool                               crit_diagnostics_present             = false;
  bool                               interface_instance_ind_present       = false;
  bool                               tnl_cfg_info_present                 = false;
  bool                               local_ng_ran_node_id_present         = false;
  bool                               neighbour_ng_ran_node_list_present   = false;
  bool                               local_ng_ran_node_id_removal_present = false;
  responding_node_type_cfg_upd_ack_c responding_node_type_cfg_upd_ack;
  tnl_a_setup_list_l                 tnl_a_setup_list;
  tnl_a_failed_to_setup_list_l       tnl_a_failed_to_setup_list;
  crit_diagnostics_s                 crit_diagnostics;
  uint16_t                           interface_instance_ind;
  tnl_cfg_info_s                     tnl_cfg_info;
  local_ng_ran_node_id_c             local_ng_ran_node_id;
  neighbour_ng_ran_node_list_l       neighbour_ng_ran_node_list;
  local_ng_ran_node_id_c             local_ng_ran_node_id_removal;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NGRANNodeConfigurationUpdateAcknowledge ::= SEQUENCE
using ngran_node_cfg_upd_ack_s = elementary_procedure_option<ngran_node_cfg_upd_ack_ies_container>;

// NGRANNodeConfigurationUpdateFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ngran_node_cfg_upd_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ngran_node_cfg_upd_fail_ies_container {
  bool               time_to_wait_present           = false;
  bool               crit_diagnostics_present       = false;
  bool               interface_instance_ind_present = false;
  cause_c            cause;
  time_to_wait_e     time_to_wait;
  crit_diagnostics_s crit_diagnostics;
  uint16_t           interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NGRANNodeConfigurationUpdateFailure ::= SEQUENCE
using ngran_node_cfg_upd_fail_s = elementary_procedure_option<ngran_node_cfg_upd_fail_ies_container>;

// PDUSessionResourcesNotify-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_notify_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_notify_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourcesNotify-Item ::= SEQUENCE
struct pdu_session_res_notify_item_s {
  bool                                          ie_exts_present = false;
  uint16_t                                      pdu_session_id  = 0;
  qos_flow_notif_ctrl_ind_info_l                qos_flows_notif_contr_ind_info;
  pdu_session_res_notify_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourcesNotifyList ::= SEQUENCE (SIZE (1..256)) OF PDUSessionResourcesNotify-Item
using pdu_session_res_notify_list_l = dyn_array<pdu_session_res_notify_item_s>;

// NotificationControlIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct notif_ctrl_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct notif_ctrl_ind_ies_container {
  bool                          pdu_session_res_notify_list_present = false;
  uint64_t                      m_ng_ran_node_ue_xnap_id;
  uint64_t                      s_ng_ran_node_ue_xnap_id;
  pdu_session_res_notify_list_l pdu_session_res_notify_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NotificationControlIndication ::= SEQUENCE
using notif_ctrl_ind_s = elementary_procedure_option<notif_ctrl_ind_ies_container>;

// PDUSession-SNChangeConfirm-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_sn_change_confirm_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_sn_change_confirm_item_ext_ies_container {
  bool add_listof_pdu_session_res_change_confirm_info_sn_terminated_present = false;
  add_listof_pdu_session_res_change_confirm_info_sn_terminated_l
      add_listof_pdu_session_res_change_confirm_info_sn_terminated;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSession-SNChangeConfirm-Item ::= SEQUENCE
struct pdu_session_sn_change_confirm_item_s {
  bool                                                 sn_terminated_present = false;
  bool                                                 mn_terminated_present = false;
  bool                                                 ie_ext_present        = false;
  uint16_t                                             pdu_session_id        = 0;
  pdu_session_res_change_confirm_info_sn_terminated_s  sn_terminated;
  pdu_session_res_change_confirm_info_m_nterminated_s  mn_terminated;
  pdu_session_sn_change_confirm_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSession-SNChangeConfirm-List ::= SEQUENCE (SIZE (1..256)) OF PDUSession-SNChangeConfirm-Item
using pdu_session_sn_change_confirm_list_l = dyn_array<pdu_session_sn_change_confirm_item_s>;

// PDUSession-SNChangeRequired-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_sn_change_required_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_sn_change_required_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSession-SNChangeRequired-Item ::= SEQUENCE
struct pdu_session_sn_change_required_item_s {
  bool                                                  sn_terminated_present = false;
  bool                                                  mn_terminated_present = false;
  bool                                                  ie_ext_present        = false;
  uint16_t                                              pdu_session_id        = 0;
  pdu_session_res_change_required_info_sn_terminated_s  sn_terminated;
  pdu_session_res_change_required_info_m_nterminated_s  mn_terminated;
  pdu_session_sn_change_required_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSession-SNChangeRequired-List ::= SEQUENCE (SIZE (1..256)) OF PDUSession-SNChangeRequired-Item
using pdu_session_sn_change_required_list_l = dyn_array<pdu_session_sn_change_required_item_s>;

// PDUSessionAdmittedToBeAddedSNModResponse-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_admitted_to_be_added_sn_mod_resp_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_admitted_to_be_added_sn_mod_resp_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionAdmittedToBeAddedSNModResponse-Item ::= SEQUENCE
struct pdu_session_admitted_to_be_added_sn_mod_resp_item_s {
  bool                                                                sn_terminated_present = false;
  bool                                                                mn_terminated_present = false;
  bool                                                                ie_ext_present        = false;
  uint16_t                                                            pdu_session_id        = 0;
  pdu_session_res_setup_resp_info_sn_terminated_s                     sn_terminated;
  pdu_session_res_setup_resp_info_m_nterminated_s                     mn_terminated;
  pdu_session_admitted_to_be_added_sn_mod_resp_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionAdmittedToBeAddedSNModResponse ::= SEQUENCE (SIZE (1..256)) OF
// PDUSessionAdmittedToBeAddedSNModResponse-Item
using pdu_session_admitted_to_be_added_sn_mod_resp_l = dyn_array<pdu_session_admitted_to_be_added_sn_mod_resp_item_s>;

// PDUSessionAdmittedToBeModifiedSNModResponse-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_admitted_to_be_modified_sn_mod_resp_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_admitted_to_be_modified_sn_mod_resp_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionAdmittedToBeModifiedSNModResponse-Item ::= SEQUENCE
struct pdu_session_admitted_to_be_modified_sn_mod_resp_item_s {
  bool                                                                   sn_terminated_present = false;
  bool                                                                   mn_terminated_present = false;
  bool                                                                   ie_ext_present        = false;
  uint16_t                                                               pdu_session_id        = 0;
  pdu_session_res_mod_resp_info_sn_terminated_s                          sn_terminated;
  pdu_session_res_mod_resp_info_m_nterminated_s                          mn_terminated;
  pdu_session_admitted_to_be_modified_sn_mod_resp_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionAdmittedToBeModifiedSNModResponse ::= SEQUENCE (SIZE (1..256)) OF
// PDUSessionAdmittedToBeModifiedSNModResponse-Item
using pdu_session_admitted_to_be_modified_sn_mod_resp_l =
    dyn_array<pdu_session_admitted_to_be_modified_sn_mod_resp_item_s>;

// PDUSessionAdmittedToBeReleasedSNModResponse-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_admitted_to_be_released_sn_mod_resp_ext_ies_o = protocol_ext_empty_o;

using pdu_session_admitted_to_be_released_sn_mod_resp_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionAdmittedToBeReleasedSNModResponse ::= SEQUENCE
struct pdu_session_admitted_to_be_released_sn_mod_resp_s {
  bool                                                              ie_ext_present = false;
  pdu_session_list_with_data_forwarding_request_l                   sn_terminated;
  pdu_session_list_with_cause_l                                     mn_terminated;
  pdu_session_admitted_to_be_released_sn_mod_resp_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionAdmitted-SNModResponse-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_admitted_sn_mod_resp_ext_ies_o = protocol_ext_empty_o;

using pdu_session_admitted_sn_mod_resp_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionAdmitted-SNModResponse ::= SEQUENCE
struct pdu_session_admitted_sn_mod_resp_s {
  bool                                               pdu_session_res_admitted_to_be_released_present = false;
  bool                                               ie_ext_present                                  = false;
  pdu_session_admitted_to_be_added_sn_mod_resp_l     pdu_session_res_admitted_to_be_added;
  pdu_session_admitted_to_be_modified_sn_mod_resp_l  pdu_session_res_admitted_to_be_modified;
  pdu_session_admitted_to_be_released_sn_mod_resp_s  pdu_session_res_admitted_to_be_released;
  pdu_session_admitted_sn_mod_resp_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionAdmittedAddedAddReqAck-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_admitted_added_add_req_ack_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_admitted_added_add_req_ack_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionAdmittedAddedAddReqAck-Item ::= SEQUENCE
struct pdu_session_admitted_added_add_req_ack_item_s {
  bool                                                          sn_terminated_present = false;
  bool                                                          mn_terminated_present = false;
  bool                                                          ie_ext_present        = false;
  uint16_t                                                      pdu_session_id        = 0;
  pdu_session_res_setup_resp_info_sn_terminated_s               sn_terminated;
  pdu_session_res_setup_resp_info_m_nterminated_s               mn_terminated;
  pdu_session_admitted_added_add_req_ack_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionAdmittedAddedAddReqAck ::= SEQUENCE (SIZE (1..256)) OF PDUSessionAdmittedAddedAddReqAck-Item
using pdu_session_admitted_added_add_req_ack_l = dyn_array<pdu_session_admitted_added_add_req_ack_item_s>;

// PDUSessionAdmittedModSNModConfirm-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_admitted_mod_sn_mod_confirm_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_admitted_mod_sn_mod_confirm_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionAdmittedModSNModConfirm-Item ::= SEQUENCE
struct pdu_session_admitted_mod_sn_mod_confirm_item_s {
  bool                                                           sn_terminated_present = false;
  bool                                                           mn_terminated_present = false;
  bool                                                           ie_ext_present        = false;
  uint16_t                                                       pdu_session_id        = 0;
  pdu_session_res_mod_confirm_info_sn_terminated_s               sn_terminated;
  pdu_session_res_mod_confirm_info_m_nterminated_s               mn_terminated;
  pdu_session_admitted_mod_sn_mod_confirm_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionAdmittedModSNModConfirm ::= SEQUENCE (SIZE (1..256)) OF PDUSessionAdmittedModSNModConfirm-Item
using pdu_session_admitted_mod_sn_mod_confirm_l = dyn_array<pdu_session_admitted_mod_sn_mod_confirm_item_s>;

// PDUSessionAdmittedToBeReleasedSNModConfirm-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_admitted_to_be_released_sn_mod_confirm_ext_ies_o = protocol_ext_empty_o;

// PDUSessionDataForwarding-SNModResponse-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_data_forwarding_sn_mod_resp_ext_ies_o = protocol_ext_empty_o;

using pdu_session_data_forwarding_sn_mod_resp_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionDataForwarding-SNModResponse ::= SEQUENCE
struct pdu_session_data_forwarding_sn_mod_resp_s {
  bool                                                      ie_exts_present = false;
  pdu_session_list_with_data_forwarding_request_l           sn_terminated;
  pdu_session_data_forwarding_sn_mod_resp_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionNotAdmitted-SNModResponse-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_not_admitted_sn_mod_resp_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_not_admitted_sn_mod_resp_ext_ies_container {
  bool                                pdu_session_res_not_admitted_list_present = false;
  pdu_session_res_not_admitted_list_l pdu_session_res_not_admitted_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionNotAdmitted-SNModResponse ::= SEQUENCE
struct pdu_session_not_admitted_sn_mod_resp_s {
  bool                                                   ie_ext_present = false;
  pdu_session_list_l                                     pdu_session_list;
  pdu_session_not_admitted_sn_mod_resp_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionNotAdmittedAddReqAck-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_not_admitted_add_req_ack_ext_ies_o = protocol_ext_empty_o;

using pdu_session_not_admitted_add_req_ack_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionNotAdmittedAddReqAck ::= SEQUENCE
struct pdu_session_not_admitted_add_req_ack_s {
  bool                                                   ie_ext_present = false;
  pdu_session_res_not_admitted_list_l                    pdu_session_res_not_admitted_sn_terminated;
  pdu_session_res_not_admitted_list_l                    pdu_session_res_not_admitted_m_nterminated;
  pdu_session_not_admitted_add_req_ack_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionReleasedList-RelConf-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_released_list_rel_conf_ext_ies_o = protocol_ext_empty_o;

using pdu_session_released_list_rel_conf_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionReleasedList-RelConf ::= SEQUENCE
struct pdu_session_released_list_rel_conf_s {
  bool                                                 ie_exts_present = false;
  pdu_session_list_with_data_forwarding_from_target_l  pdu_sessions_released_list_sn_terminated;
  pdu_session_released_list_rel_conf_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

using pdu_session_admitted_to_be_released_sn_mod_confirm_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionReleasedSNModConfirm ::= SEQUENCE
struct pdu_session_released_sn_mod_confirm_s {
  bool                                                                 ie_ext_present = false;
  pdu_session_list_with_data_forwarding_from_target_l                  sn_terminated;
  pdu_session_list_l                                                   mn_terminated;
  pdu_session_admitted_to_be_released_sn_mod_confirm_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionToBeAddedAddReq-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_to_be_added_add_req_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_to_be_added_add_req_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionToBeAddedAddReq-Item ::= SEQUENCE
struct pdu_session_to_be_added_add_req_item_s {
  bool                                                   sn_pdu_session_ambr_present = false;
  bool                                                   sn_terminated_present       = false;
  bool                                                   mn_terminated_present       = false;
  bool                                                   ie_ext_present              = false;
  uint16_t                                               pdu_session_id              = 0;
  s_nssai_s                                              s_nssai;
  pdu_session_aggr_max_bit_rate_s                        sn_pdu_session_ambr;
  pdu_session_res_setup_info_sn_terminated_s             sn_terminated;
  pdu_session_res_setup_info_m_nterminated_s             mn_terminated;
  pdu_session_to_be_added_add_req_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionToBeAddedAddReq ::= SEQUENCE (SIZE (1..256)) OF PDUSessionToBeAddedAddReq-Item
using pdu_session_to_be_added_add_req_l = dyn_array<pdu_session_to_be_added_add_req_item_s>;

// PDUSessionToBeModifiedSNModRequired-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_to_be_modified_sn_mod_required_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_to_be_modified_sn_mod_required_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionToBeModifiedSNModRequired-Item ::= SEQUENCE
struct pdu_session_to_be_modified_sn_mod_required_item_s {
  bool                                                              sn_terminated_present = false;
  bool                                                              mn_terminated_present = false;
  bool                                                              ie_ext_present        = false;
  uint16_t                                                          pdu_session_id        = 0;
  pdu_session_res_mod_rqd_info_sn_terminated_s                      sn_terminated;
  pdu_session_res_mod_rqd_info_m_nterminated_s                      mn_terminated;
  pdu_session_to_be_modified_sn_mod_required_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionToBeModifiedSNModRequired ::= SEQUENCE (SIZE (1..256)) OF PDUSessionToBeModifiedSNModRequired-Item
using pdu_session_to_be_modified_sn_mod_required_l = dyn_array<pdu_session_to_be_modified_sn_mod_required_item_s>;

// PDUSessionToBeReleasedList-RelReqAck-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_to_be_released_list_rel_req_ack_ext_ies_o = protocol_ext_empty_o;

using pdu_session_to_be_released_list_rel_req_ack_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionToBeReleasedList-RelReqAck ::= SEQUENCE
struct pdu_session_to_be_released_list_rel_req_ack_s {
  bool                                                          ie_exts_present = false;
  pdu_session_list_with_data_forwarding_request_l               pdu_sessions_to_be_released_list_sn_terminated;
  pdu_session_to_be_released_list_rel_req_ack_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionToBeReleasedList-RelRqd-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_to_be_released_list_rel_rqd_ext_ies_o = protocol_ext_empty_o;

using pdu_session_to_be_released_list_rel_rqd_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionToBeReleasedList-RelRqd ::= SEQUENCE
struct pdu_session_to_be_released_list_rel_rqd_s {
  bool                                                      ie_exts_present = false;
  pdu_session_list_with_data_forwarding_request_l           pdu_sessions_to_be_released_list_sn_terminated;
  pdu_session_to_be_released_list_rel_rqd_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionToBeReleasedSNModRequired-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_to_be_released_sn_mod_required_ext_ies_o = protocol_ext_empty_o;

using pdu_session_to_be_released_sn_mod_required_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionToBeReleasedSNModRequired ::= SEQUENCE
struct pdu_session_to_be_released_sn_mod_required_s {
  bool                                                         ie_ext_present = false;
  pdu_session_list_with_data_forwarding_request_l              sn_terminated;
  pdu_session_list_with_cause_l                                mn_terminated;
  pdu_session_to_be_released_sn_mod_required_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionsToBeAdded-SNModRequest-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_container {
  bool                             pdu_session_expected_ue_activity_behaviour_present = false;
  expected_ue_activity_behaviour_s pdu_session_expected_ue_activity_behaviour;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionsToBeAdded-SNModRequest-Item ::= SEQUENCE
struct pdu_sessions_to_be_added_sn_mod_request_item_s {
  bool                                                           sn_pdu_session_ambr_present = false;
  bool                                                           sn_terminated_present       = false;
  bool                                                           mn_terminated_present       = false;
  bool                                                           ie_ext_present              = false;
  uint16_t                                                       pdu_session_id              = 0;
  s_nssai_s                                                      s_nssai;
  pdu_session_aggr_max_bit_rate_s                                sn_pdu_session_ambr;
  pdu_session_res_setup_info_sn_terminated_s                     sn_terminated;
  pdu_session_res_setup_info_m_nterminated_s                     mn_terminated;
  pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionsToBeAdded-SNModRequest-List ::= SEQUENCE (SIZE (1..256)) OF PDUSessionsToBeAdded-SNModRequest-Item
using pdu_sessions_to_be_added_sn_mod_request_list_l = dyn_array<pdu_sessions_to_be_added_sn_mod_request_item_s>;

// PDUSessionsToBeModified-SNModRequest-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_container {
  bool                             s_nssai_present                                    = false;
  bool                             pdu_session_expected_ue_activity_behaviour_present = false;
  bool                             user_plane_fail_ind_present                        = false;
  s_nssai_s                        s_nssai;
  expected_ue_activity_behaviour_s pdu_session_expected_ue_activity_behaviour;
  user_plane_fail_ind_s            user_plane_fail_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionsToBeModified-SNModRequest-Item ::= SEQUENCE
struct pdu_sessions_to_be_modified_sn_mod_request_item_s {
  bool                                                              sn_pdu_session_ambr_present = false;
  bool                                                              sn_terminated_present       = false;
  bool                                                              mn_terminated_present       = false;
  bool                                                              ie_ext_present              = false;
  uint16_t                                                          pdu_session_id              = 0;
  pdu_session_aggr_max_bit_rate_s                                   sn_pdu_session_ambr;
  pdu_session_res_mod_info_sn_terminated_s                          sn_terminated;
  pdu_session_res_mod_info_m_nterminated_s                          mn_terminated;
  pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionsToBeModified-SNModRequest-List ::= SEQUENCE (SIZE (1..256)) OF PDUSessionsToBeModified-SNModRequest-Item
using pdu_sessions_to_be_modified_sn_mod_request_list_l = dyn_array<pdu_sessions_to_be_modified_sn_mod_request_item_s>;

// PDUSessionsToBeReleased-SNModRequest-List-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_sessions_to_be_released_sn_mod_request_list_ext_ies_o = protocol_ext_empty_o;

using pdu_sessions_to_be_released_sn_mod_request_list_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionsToBeReleased-SNModRequest-List ::= SEQUENCE
struct pdu_sessions_to_be_released_sn_mod_request_list_s {
  bool                                                              ie_ext_present = false;
  pdu_session_list_with_cause_l                                     pdu_session_list;
  pdu_sessions_to_be_released_sn_mod_request_list_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PartialUEContextTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct partial_ue_context_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct partial_ue_context_transfer_ies_container {
  bool                          pos_partial_ue_context_info_present = false;
  uint64_t                      new_ng_ran_node_ue_xnap_id;
  uint64_t                      old_ng_ran_node_ue_xnap_id;
  sdt_partial_ue_context_info_s sdt_partial_ue_context_info;
  pos_partial_ue_context_info_s pos_partial_ue_context_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PartialUEContextTransfer ::= SEQUENCE
using partial_ue_context_transfer_s = elementary_procedure_option<partial_ue_context_transfer_ies_container>;

// PartialUEContextTransferAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct partial_ue_context_transfer_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct partial_ue_context_transfer_ack_ies_container {
  bool                           sdt_data_forwarding_drb_list_present = false;
  bool                           crit_diagnostics_present             = false;
  bool                           srs_cfg_present                      = false;
  uint64_t                       new_ng_ran_node_ue_xnap_id;
  uint64_t                       old_ng_ran_node_ue_xnap_id;
  sdt_data_forwarding_drb_list_l sdt_data_forwarding_drb_list;
  crit_diagnostics_s             crit_diagnostics;
  unbounded_octstring<true>      srs_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PartialUEContextTransferAcknowledge ::= SEQUENCE
using partial_ue_context_transfer_ack_s = elementary_procedure_option<partial_ue_context_transfer_ack_ies_container>;

// PartialUEContextTransferFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct partial_ue_context_transfer_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct partial_ue_context_transfer_fail_ies_container {
  bool               crit_diagnostics_present = false;
  uint64_t           new_ng_ran_node_ue_xnap_id;
  uint64_t           old_ng_ran_node_ue_xnap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PartialUEContextTransferFailure ::= SEQUENCE
using partial_ue_context_transfer_fail_s = elementary_procedure_option<partial_ue_context_transfer_fail_ies_container>;

struct xnap_private_ies_empty_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::nulltype; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
  };
};
// PrivateMessage-IEs ::= OBJECT SET OF XNAP-PRIVATE-IES
using private_msg_ies_o = xnap_private_ies_empty_o;

struct private_ie_container_empty_l {
  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};
using private_msg_ies_container = private_ie_container_empty_l;

// PrivateMessage ::= SEQUENCE
struct private_msg_s {
  private_msg_ies_container private_ies;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoE-Measurement-Results-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qo_e_meas_results_ext_ies_o = protocol_ext_empty_o;

using qo_e_meas_results_ext_ies_container = protocol_ext_container_empty_l;

// QoE-Measurement-Results ::= SEQUENCE
struct qo_e_meas_results_s {
  struct app_layer_session_status_opts {
    enum options { started, stopped, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using app_layer_session_status_e_ = enumerated<app_layer_session_status_opts, true>;

  // member variables
  bool                                app_layer_session_status_present = false;
  bool                                ie_exts_present                  = false;
  fixed_octstring<6, true>            qoe_ref;
  unbounded_octstring<true>           rrc_container_for_rv_qo_e_report;
  unbounded_octstring<true>           rrc_container_for_qo_e_report;
  app_layer_session_status_e_         app_layer_session_status;
  qo_e_meas_results_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANMulticastGroupPaging-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ran_multicast_group_paging_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ran_multicast_group_paging_ies_container {
  mbs_session_id_s                  mbs_session_id;
  ue_id_idx_list_mbs_group_paging_l ue_id_idx_list_mbs_group_paging;
  ran_paging_area_s                 multicast_ran_paging_area;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANMulticastGroupPaging ::= SEQUENCE
using ran_multicast_group_paging_s = elementary_procedure_option<ran_multicast_group_paging_ies_container>;

// RANPaging-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ran_paging_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ran_paging_ies_container {
  bool                                       paging_prio_present                              = false;
  bool                                       assist_data_for_ran_paging_present               = false;
  bool                                       ue_radio_cap_for_paging_present                  = false;
  bool                                       extended_ue_id_idx_value_present                 = false;
  bool                                       eutra_paginge_drx_info_present                   = false;
  bool                                       ue_specific_drx_present                          = false;
  bool                                       nr_paginge_drx_info_present                      = false;
  bool                                       nr_paginge_drx_infofor_rrc_inactive_present      = false;
  bool                                       paging_cause_present                             = false;
  bool                                       pe_ip_sassist_info_present                       = false;
  bool                                       hashed_ue_id_idx_value_present                   = false;
  bool                                       mt_sdt_info_present                              = false;
  bool                                       nr_paging_longe_drx_infofor_rrc_inactive_present = false;
  ue_id_idx_value_c                          ue_id_idx_value;
  ue_ran_paging_id_c                         ue_ran_paging_id;
  paging_drx_e                               paging_drx;
  ran_paging_area_s                          ran_paging_area;
  paging_prio_e                              paging_prio;
  assist_data_for_ran_paging_s               assist_data_for_ran_paging;
  ue_radio_cap_for_paging_s                  ue_radio_cap_for_paging;
  fixed_bitstring<16, false, true>           extended_ue_id_idx_value;
  eutra_paginge_drx_info_s                   eutra_paginge_drx_info;
  ue_specific_drx_e                          ue_specific_drx;
  nr_paginge_drx_info_s                      nr_paginge_drx_info;
  nr_paginge_drx_infofor_rrc_inactive_s      nr_paginge_drx_infofor_rrc_inactive;
  paging_cause_e                             paging_cause;
  pe_ip_sassist_info_s                       pe_ip_sassist_info;
  fixed_bitstring<13, true, true>            hashed_ue_id_idx_value;
  mt_sdt_info_s                              mt_sdt_info;
  nr_paging_longe_drx_infofor_rrc_inactive_s nr_paging_longe_drx_infofor_rrc_inactive;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANPaging ::= SEQUENCE
using ran_paging_s = elementary_procedure_option<ran_paging_ies_container>;

// SplitSRB-RRCTransfer-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using split_srb_rrc_transfer_ext_ies_o = protocol_ext_empty_o;

using split_srb_rrc_transfer_ext_ies_container = protocol_ext_container_empty_l;

// SplitSRB-RRCTransfer ::= SEQUENCE
struct split_srb_rrc_transfer_s {
  struct srb_type_opts {
    enum options { srb1, srb2, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using srb_type_e_ = enumerated<srb_type_opts, true>;

  // member variables
  bool                                     delivery_status_present = false;
  bool                                     ie_exts_present         = false;
  unbounded_octstring<true>                rrc_container;
  srb_type_e_                              srb_type;
  uint16_t                                 delivery_status = 0;
  split_srb_rrc_transfer_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEReportRRCTransfer-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_report_rrc_transfer_ext_ies_o = protocol_ext_empty_o;

using ue_report_rrc_transfer_ext_ies_container = protocol_ext_container_empty_l;

// UEReportRRCTransfer ::= SEQUENCE
struct ue_report_rrc_transfer_s {
  bool                                     ie_exts_present = false;
  unbounded_octstring<true>                rrc_container;
  ue_report_rrc_transfer_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SDT-SRB-between-NewNode-OldNode-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sdt_srb_between_new_node_old_node_ext_ies_o = protocol_ext_empty_o;

using sdt_srb_between_new_node_old_node_ext_ies_container = protocol_ext_container_empty_l;

// SDT-SRB-between-NewNode-OldNode ::= SEQUENCE
struct sdt_srb_between_new_node_old_node_s {
  bool                                                ie_exts_present = false;
  unbounded_octstring<true>                           rrc_container;
  uint8_t                                             srb_id = 0;
  sdt_srb_between_new_node_old_node_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct rrc_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct rrc_transfer_ies_container {
  bool                                split_srb_rrc_transfer_present                  = false;
  bool                                ue_report_rrc_transfer_present                  = false;
  bool                                fast_mcg_recovery_rrc_transfer_sn_to_mn_present = false;
  bool                                fast_mcg_recovery_rrc_transfer_mn_to_sn_present = false;
  bool                                sdt_srb_between_new_node_old_node_present       = false;
  bool                                qo_e_meas_results_present                       = false;
  uint64_t                            m_ng_ran_node_ue_xnap_id;
  uint64_t                            s_ng_ran_node_ue_xnap_id;
  split_srb_rrc_transfer_s            split_srb_rrc_transfer;
  ue_report_rrc_transfer_s            ue_report_rrc_transfer;
  fast_mcg_recovery_rrc_transfer_s    fast_mcg_recovery_rrc_transfer_sn_to_mn;
  fast_mcg_recovery_rrc_transfer_s    fast_mcg_recovery_rrc_transfer_mn_to_sn;
  sdt_srb_between_new_node_old_node_s sdt_srb_between_new_node_old_node;
  qo_e_meas_results_s                 qo_e_meas_results;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCTransfer ::= SEQUENCE
using rrc_transfer_s = elementary_procedure_option<rrc_transfer_ies_container>;

// RachIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct rach_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct rach_ind_ies_container {
  ra_report_ind_list_l ra_report_ind_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RachIndication ::= SEQUENCE
using rach_ind_s = elementary_procedure_option<rach_ind_ies_container>;

// ReleaseFastMCGRecoveryViaSRB3 ::= ENUMERATED
struct release_fast_mcg_recovery_via_srb3_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using release_fast_mcg_recovery_via_srb3_e = enumerated<release_fast_mcg_recovery_via_srb3_opts, true>;

// RequestedFastMCGRecoveryViaSRB3 ::= ENUMERATED
struct requested_fast_mcg_recovery_via_srb3_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using requested_fast_mcg_recovery_via_srb3_e = enumerated<requested_fast_mcg_recovery_via_srb3_opts, true>;

// RequestedFastMCGRecoveryViaSRB3Release ::= ENUMERATED
struct requested_fast_mcg_recovery_via_srb3_release_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using requested_fast_mcg_recovery_via_srb3_release_e =
    enumerated<requested_fast_mcg_recovery_via_srb3_release_opts, true>;

// ResetRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct reset_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct reset_request_ies_container {
  bool                      interface_instance_ind_present = false;
  reset_request_type_info_c reset_request_type_info;
  cause_c                   cause;
  uint16_t                  interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetRequest ::= SEQUENCE
using reset_request_s = elementary_procedure_option<reset_request_ies_container>;

// ResetResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct reset_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct reset_resp_ies_container {
  bool                   crit_diagnostics_present       = false;
  bool                   interface_instance_ind_present = false;
  reset_resp_type_info_c reset_resp_type_info;
  crit_diagnostics_s     crit_diagnostics;
  uint16_t               interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetResponse ::= SEQUENCE
using reset_resp_s = elementary_procedure_option<reset_resp_ies_container>;

// ResourceStatusFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct res_status_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct res_status_fail_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           ngran_node1_meas_id;
  uint16_t           ngran_node2_meas_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceStatusFailure ::= SEQUENCE
using res_status_fail_s = elementary_procedure_option<res_status_fail_ies_container>;

// ResourceStatusRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct res_status_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct res_status_request_ies_container {
  bool                             ngran_node2_meas_id_present    = false;
  bool                             report_characteristics_present = false;
  bool                             cell_to_report_present         = false;
  bool                             report_periodicity_present     = false;
  uint16_t                         ngran_node1_meas_id;
  uint16_t                         ngran_node2_meas_id;
  regist_request_e                 regist_request;
  fixed_bitstring<32, false, true> report_characteristics;
  cell_to_report_l                 cell_to_report;
  report_periodicity_e             report_periodicity;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceStatusRequest ::= SEQUENCE
using res_status_request_s = elementary_procedure_option<res_status_request_ies_container>;

// ResourceStatusResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct res_status_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct res_status_resp_ies_container {
  bool               crit_diagnostics_present = false;
  uint16_t           ngran_node1_meas_id;
  uint16_t           ngran_node2_meas_id;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceStatusResponse ::= SEQUENCE
using res_status_resp_s = elementary_procedure_option<res_status_resp_ies_container>;

// ResourceStatusUpdate-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct res_status_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct res_status_upd_ies_container {
  uint16_t           ngran_node1_meas_id;
  uint16_t           ngran_node2_meas_id;
  cell_meas_result_l cell_meas_result;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResourceStatusUpdate ::= SEQUENCE
using res_status_upd_s = elementary_procedure_option<res_status_upd_ies_container>;

// ResponseType-ReconfComplete-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using resp_type_reconf_complete_ext_ies_o = protocol_ies_empty_o;

// ResponseType-ReconfComplete ::= CHOICE
struct resp_type_reconf_complete_c {
  struct types_opts {
    enum options { cfg_successfully_applied, cfg_rejected_by_m_ng_ran_node, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  resp_type_reconf_complete_c() = default;
  resp_type_reconf_complete_c(const resp_type_reconf_complete_c& other);
  resp_type_reconf_complete_c& operator=(const resp_type_reconf_complete_c& other);
  ~resp_type_reconf_complete_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  cfg_successfully_applied_s& cfg_successfully_applied()
  {
    assert_choice_type(types::cfg_successfully_applied, type_, "ResponseType-ReconfComplete");
    return c.get<cfg_successfully_applied_s>();
  }
  cfg_rejected_by_m_ng_ran_node_s& cfg_rejected_by_m_ng_ran_node()
  {
    assert_choice_type(types::cfg_rejected_by_m_ng_ran_node, type_, "ResponseType-ReconfComplete");
    return c.get<cfg_rejected_by_m_ng_ran_node_s>();
  }
  protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ResponseType-ReconfComplete");
    return c.get<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>();
  }
  const cfg_successfully_applied_s& cfg_successfully_applied() const
  {
    assert_choice_type(types::cfg_successfully_applied, type_, "ResponseType-ReconfComplete");
    return c.get<cfg_successfully_applied_s>();
  }
  const cfg_rejected_by_m_ng_ran_node_s& cfg_rejected_by_m_ng_ran_node() const
  {
    assert_choice_type(types::cfg_rejected_by_m_ng_ran_node, type_, "ResponseType-ReconfComplete");
    return c.get<cfg_rejected_by_m_ng_ran_node_s>();
  }
  const protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ResponseType-ReconfComplete");
    return c.get<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>();
  }
  cfg_successfully_applied_s&                                          set_cfg_successfully_applied();
  cfg_rejected_by_m_ng_ran_node_s&                                     set_cfg_rejected_by_m_ng_ran_node();
  protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<cfg_rejected_by_m_ng_ran_node_s,
                  cfg_successfully_applied_s,
                  protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>
      c;

  void destroy_();
};

// ResponseInfo-ReconfCompl-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using resp_info_reconf_compl_ext_ies_o = protocol_ext_empty_o;

using resp_info_reconf_compl_ext_ies_container = protocol_ext_container_empty_l;

// ResponseInfo-ReconfCompl ::= SEQUENCE
struct resp_info_reconf_compl_s {
  bool                                     ie_exts_present = false;
  resp_type_reconf_complete_c              resp_type_reconf_complete;
  resp_info_reconf_compl_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RetrieveUEContextConfirm-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct retrieve_ue_context_confirm_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct retrieve_ue_context_confirm_ies_container {
  bool                      ue_context_kept_ind_present     = false;
  bool                      sdt_termination_request_present = false;
  uint64_t                  old_ng_ran_node_ue_xnap_id;
  uint64_t                  new_ng_ran_node_ue_xnap_id;
  ue_context_kept_ind_e     ue_context_kept_ind;
  sdt_termination_request_e sdt_termination_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RetrieveUEContextConfirm ::= SEQUENCE
using retrieve_ue_context_confirm_s = elementary_procedure_option<retrieve_ue_context_confirm_ies_container>;

// RetrieveUEContextFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct retrieve_ue_context_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct retrieve_ue_context_fail_ies_container {
  bool                              oldto_new_ng_ran_node_resume_container_present = false;
  bool                              crit_diagnostics_present                       = false;
  bool                              semipersistent_positioning_info_present        = false;
  uint64_t                          new_ng_ran_node_ue_xnap_id;
  unbounded_octstring<true>         oldto_new_ng_ran_node_resume_container;
  cause_c                           cause;
  crit_diagnostics_s                crit_diagnostics;
  semipersistent_positioning_info_s semipersistent_positioning_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RetrieveUEContextFailure ::= SEQUENCE
using retrieve_ue_context_fail_s = elementary_procedure_option<retrieve_ue_context_fail_ies_container>;

// RetrieveUEContextRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct retrieve_ue_context_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct retrieve_ue_context_request_ies_container {
  bool                                        rrc_resume_cause_present                          = false;
  bool                                        sdt_support_request_present                       = false;
  bool                                        srs_positioning_cfg_or_activation_request_present = false;
  uint64_t                                    new_ng_ran_node_ue_xnap_id;
  ue_context_id_c                             ue_context_id;
  fixed_bitstring<16, false, true>            mac_i;
  ng_ran_cell_id_c                            new_ng_ran_cell_id;
  rrc_resume_cause_e                          rrc_resume_cause;
  sdt_support_request_s                       sdt_support_request;
  srs_positioning_cfg_or_activation_request_e srs_positioning_cfg_or_activation_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RetrieveUEContextRequest ::= SEQUENCE
using retrieve_ue_context_request_s = elementary_procedure_option<retrieve_ue_context_request_ies_container>;

// RetrieveUEContextResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct retrieve_ue_context_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct retrieve_ue_context_resp_ies_container {
  bool                                   trace_activation_present                     = false;
  bool                                   masked_imeisv_present                        = false;
  bool                                   location_report_info_present                 = false;
  bool                                   crit_diagnostics_present                     = false;
  bool                                   nr_v2x_services_authorized_present           = false;
  bool                                   ltev2x_services_authorized_present           = false;
  bool                                   pc5_qos_params_present                       = false;
  bool                                   ue_history_info_present                      = false;
  bool                                   ue_history_info_from_the_ue_present          = false;
  bool                                   mdt_plmn_list_present                        = false;
  bool                                   iab_node_ind_present                         = false;
  bool                                   ue_context_ref_at_sn_ho_request_present      = false;
  bool                                   time_sync_assist_info_present                = false;
  bool                                   q_mcc_onfig_info_present                     = false;
  bool                                   five_g_pro_se_authorized_present             = false;
  bool                                   five_g_pro_se_pc5_qos_params_present         = false;
  bool                                   aerial_ue_sub_info_present                   = false;
  bool                                   nr_a2_x_services_authorized_present          = false;
  bool                                   ltea2_x_services_authorized_present          = false;
  bool                                   a2_xpc5_qos_params_present                   = false;
  bool                                   mobile_iab_authorization_status_present      = false;
  bool                                   sl_positioning_ranging_services_info_present = false;
  uint64_t                               new_ng_ran_node_ue_xnap_id;
  uint64_t                               old_ng_ran_node_ue_xnap_id;
  guami_s                                guami;
  ue_context_info_retr_ue_ctxt_resp_s    ue_context_info_retr_ue_ctxt_resp;
  trace_activation_s                     trace_activation;
  fixed_bitstring<64, false, true>       masked_imeisv;
  location_report_info_s                 location_report_info;
  crit_diagnostics_s                     crit_diagnostics;
  nr_v2x_services_authorized_s           nr_v2x_services_authorized;
  ltev2x_services_authorized_s           ltev2x_services_authorized;
  pc5_qos_params_s                       pc5_qos_params;
  ue_history_info_l                      ue_history_info;
  ue_history_info_from_the_ue_c          ue_history_info_from_the_ue;
  mdt_plmn_list_l                        mdt_plmn_list;
  iab_node_ind_e                         iab_node_ind;
  ue_context_ref_at_sn_ho_request_s      ue_context_ref_at_sn_ho_request;
  time_sync_assist_info_s                time_sync_assist_info;
  q_mcc_onfig_info_s                     q_mcc_onfig_info;
  five_g_pro_se_authorized_s             five_g_pro_se_authorized;
  five_g_pro_se_pc5_qos_params_s         five_g_pro_se_pc5_qos_params;
  aerial_ue_sub_info_e                   aerial_ue_sub_info;
  nr_a2_x_services_authorized_s          nr_a2_x_services_authorized;
  ltea2_x_services_authorized_s          ltea2_x_services_authorized;
  a2_xpc5_qos_params_s                   a2_xpc5_qos_params;
  mobile_iab_authorization_status_e      mobile_iab_authorization_status;
  sl_positioning_ranging_services_info_s sl_positioning_ranging_services_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RetrieveUEContextResponse ::= SEQUENCE
using retrieve_ue_context_resp_s = elementary_procedure_option<retrieve_ue_context_resp_ies_container>;

// SNStatusTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_status_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_status_transfer_ies_container {
  bool                                   cho_cfg_present  = false;
  bool                                   mob_info_present = false;
  uint64_t                               source_ng_ran_node_ue_xnap_id;
  uint64_t                               target_ng_ran_node_ue_xnap_id;
  drbs_subject_to_status_transfer_list_l drbs_subject_to_status_transfer_list;
  cho_cfg_s                              cho_cfg;
  fixed_bitstring<32, false, true>       mob_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNStatusTransfer ::= SEQUENCE
using sn_status_transfer_s = elementary_procedure_option<sn_status_transfer_ies_container>;

// SNodeAdditionRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_addition_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_addition_request_ies_container {
  bool                                   sel_plmn_present                             = false;
  bool                                   mob_restrict_list_present                    = false;
  bool                                   idx_to_rat_frequ_sel_prio_present            = false;
  bool                                   s_ng_ran_node_ue_xnap_id_present             = false;
  bool                                   expected_ue_behaviour_present                = false;
  bool                                   requested_split_srb_present                  = false;
  bool                                   pcell_id_present                             = false;
  bool                                   desired_act_notif_level_present              = false;
  bool                                   available_drb_ids_present                    = false;
  bool                                   s_ng_ran_node_max_ip_data_rate_ul_present    = false;
  bool                                   s_ng_ran_node_max_ip_data_rate_dl_present    = false;
  bool                                   location_info_snr_eporting_present           = false;
  bool                                   mr_dc_res_coordination_info_present          = false;
  bool                                   masked_imeisv_present                        = false;
  bool                                   ne_dc_tdm_pattern_present                    = false;
  bool                                   s_ng_ran_node_addition_trigger_ind_present   = false;
  bool                                   trace_activation_present                     = false;
  bool                                   requested_fast_mcg_recovery_via_srb3_present = false;
  bool                                   ue_radio_cap_id_present                      = false;
  bool                                   source_ng_ran_node_id_present                = false;
  bool                                   management_based_mdt_plmn_list_present       = false;
  bool                                   ue_history_info_present                      = false;
  bool                                   ue_history_info_from_the_ue_present          = false;
  bool                                   pscell_change_history_present                = false;
  bool                                   iab_node_ind_present                         = false;
  bool                                   no_pdu_session_ind_present                   = false;
  bool                                   ch_oinfo_add_req_present                     = false;
  bool                                   scg_activation_request_present               = false;
  bool                                   cp_a_info_request_present                    = false;
  bool                                   s_ng_ran_node_ue_slice_mbr_present           = false;
  bool                                   f1_terminating_iab_donor_ind_present         = false;
  bool                                   sel_n_id_present                             = false;
  bool                                   q_mcc_oordination_request_present            = false;
  bool                                   source_sn_to_target_sn_qmc_info_present      = false;
  bool                                   iab_authorization_status_present             = false;
  bool                                   source_m_ng_ran_node_id_present              = false;
  uint64_t                               m_ng_ran_node_ue_xnap_id;
  ue_security_cap_s                      ue_security_cap;
  fixed_bitstring<256, false, true>      s_ng_ra_nnode_security_key;
  ue_aggr_max_bit_rate_s                 s_ng_ran_node_ue_ambr;
  fixed_octstring<3, true>               sel_plmn;
  mob_restrict_list_s                    mob_restrict_list;
  uint16_t                               idx_to_rat_frequ_sel_prio;
  pdu_session_to_be_added_add_req_l      pdu_session_to_be_added_add_req;
  unbounded_octstring<true>              mn_to_sn_container;
  uint64_t                               s_ng_ran_node_ue_xnap_id;
  expected_ue_behaviour_s                expected_ue_behaviour;
  split_srbs_types_e                     requested_split_srb;
  global_ng_ran_cell_id_s                pcell_id;
  desired_act_notif_level_e              desired_act_notif_level;
  drb_list_l                             available_drb_ids;
  uint64_t                               s_ng_ran_node_max_ip_data_rate_ul;
  uint64_t                               s_ng_ran_node_max_ip_data_rate_dl;
  location_info_snr_eporting_e           location_info_snr_eporting;
  mr_dc_res_coordination_info_s          mr_dc_res_coordination_info;
  fixed_bitstring<64, false, true>       masked_imeisv;
  ne_dc_tdm_pattern_s                    ne_dc_tdm_pattern;
  s_ng_ran_node_addition_trigger_ind_e   s_ng_ran_node_addition_trigger_ind;
  trace_activation_s                     trace_activation;
  requested_fast_mcg_recovery_via_srb3_e requested_fast_mcg_recovery_via_srb3;
  unbounded_octstring<true>              ue_radio_cap_id;
  global_ng_ran_node_id_c                source_ng_ran_node_id;
  mdt_plmn_list_l                        management_based_mdt_plmn_list;
  ue_history_info_l                      ue_history_info;
  ue_history_info_from_the_ue_c          ue_history_info_from_the_ue;
  pscell_change_history_e                pscell_change_history;
  iab_node_ind_e                         iab_node_ind;
  no_pdu_session_ind_e                   no_pdu_session_ind;
  ch_oinfo_add_req_s                     ch_oinfo_add_req;
  scg_activation_request_e               scg_activation_request;
  cp_a_info_request_s                    cp_a_info_request;
  ue_slice_max_bit_rate_list_l           s_ng_ran_node_ue_slice_mbr;
  f1_terminating_iab_donor_ind_e         f1_terminating_iab_donor_ind;
  fixed_bitstring<44, false, true>       sel_n_id;
  q_mcc_oordination_request_s            q_mcc_oordination_request;
  q_mcc_onfig_info_s                     source_sn_to_target_sn_qmc_info;
  iab_authorization_status_e             iab_authorization_status;
  global_ng_ran_node_id_c                source_m_ng_ran_node_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeAdditionRequest ::= SEQUENCE
using sn_ode_addition_request_s = elementary_procedure_option<sn_ode_addition_request_ies_container>;

// SNodeAdditionRequestAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_addition_request_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_addition_request_ack_ies_container {
  bool                                     pdu_session_not_admitted_add_req_ack_present               = false;
  bool                                     admitted_split_srb_present                                 = false;
  bool                                     rrc_cfg_ind_present                                        = false;
  bool                                     crit_diagnostics_present                                   = false;
  bool                                     location_info_sn_present                                   = false;
  bool                                     mr_dc_res_coordination_info_present                        = false;
  bool                                     available_fast_mcg_recovery_via_srb3_present               = false;
  bool                                     direct_forwarding_path_availability_present                = false;
  bool                                     scg_activation_status_present                              = false;
  bool                                     cp_a_info_ack_present                                      = false;
  bool                                     sn_mob_info_present                                        = false;
  bool                                     q_mcc_oordination_resp_present                             = false;
  bool                                     ch_oinfo_add_req_ack_present                               = false;
  bool                                     direct_forwarding_path_availability_with_source_mn_present = false;
  uint64_t                                 m_ng_ran_node_ue_xnap_id;
  uint64_t                                 s_ng_ran_node_ue_xnap_id;
  pdu_session_admitted_added_add_req_ack_l pdu_session_admitted_added_add_req_ack;
  pdu_session_not_admitted_add_req_ack_s   pdu_session_not_admitted_add_req_ack;
  unbounded_octstring<true>                sn_to_mn_container;
  split_srbs_types_e                       admitted_split_srb;
  rrc_cfg_ind_e                            rrc_cfg_ind;
  crit_diagnostics_s                       crit_diagnostics;
  target_cgi_c                             location_info_sn;
  mr_dc_res_coordination_info_s            mr_dc_res_coordination_info;
  available_fast_mcg_recovery_via_srb3_e   available_fast_mcg_recovery_via_srb3;
  direct_forwarding_path_availability_e    direct_forwarding_path_availability;
  scg_activation_status_e                  scg_activation_status;
  cp_a_info_ack_s                          cp_a_info_ack;
  fixed_bitstring<32, false, true>         sn_mob_info;
  q_mcc_oordination_resp_s                 q_mcc_oordination_resp;
  ch_oinfo_add_req_ack_s                   ch_oinfo_add_req_ack;
  direct_forwarding_path_availability_with_source_mn_e direct_forwarding_path_availability_with_source_mn;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeAdditionRequestAcknowledge ::= SEQUENCE
using sn_ode_addition_request_ack_s = elementary_procedure_option<sn_ode_addition_request_ack_ies_container>;

// SNodeAdditionRequestReject-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_addition_request_reject_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_addition_request_reject_ies_container {
  bool                    crit_diagnostics_present = false;
  bool                    pcell_id_present         = false;
  uint64_t                m_ng_ran_node_ue_xnap_id;
  uint64_t                s_ng_ran_node_ue_xnap_id;
  cause_c                 cause;
  crit_diagnostics_s      crit_diagnostics;
  global_ng_ran_cell_id_s pcell_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeAdditionRequestReject ::= SEQUENCE
using sn_ode_addition_request_reject_s = elementary_procedure_option<sn_ode_addition_request_reject_ies_container>;

// SNodeChangeConfirm-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_change_confirm_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_change_confirm_ies_container {
  bool                                 pdu_session_sn_change_confirm_list_present = false;
  bool                                 crit_diagnostics_present                   = false;
  bool                                 c_pci_nformation_confirm_present           = false;
  bool                                 mn_to_sn_container_present                 = false;
  uint64_t                             m_ng_ran_node_ue_xnap_id;
  uint64_t                             s_ng_ran_node_ue_xnap_id;
  pdu_session_sn_change_confirm_list_l pdu_session_sn_change_confirm_list;
  crit_diagnostics_s                   crit_diagnostics;
  c_pci_nformation_confirm_s           c_pci_nformation_confirm;
  unbounded_octstring<true>            mn_to_sn_container;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeChangeConfirm ::= SEQUENCE
using sn_ode_change_confirm_s = elementary_procedure_option<sn_ode_change_confirm_ies_container>;

// SNodeChangeRefuse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_change_refuse_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_change_refuse_ies_container {
  bool               crit_diagnostics_present = false;
  uint64_t           m_ng_ran_node_ue_xnap_id;
  uint64_t           s_ng_ran_node_ue_xnap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeChangeRefuse ::= SEQUENCE
using sn_ode_change_refuse_s = elementary_procedure_option<sn_ode_change_refuse_ies_container>;

// SNodeChangeRequired-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_change_required_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_change_required_ies_container {
  bool                                  pdu_session_sn_change_required_list_present = false;
  bool                                  scg_ue_history_info_present                 = false;
  bool                                  sn_mob_info_present                         = false;
  bool                                  source_pscell_id_present                    = false;
  bool                                  c_pci_nformation_required_present           = false;
  bool                                  source_sn_to_target_sn_qmc_info_present     = false;
  uint64_t                              m_ng_ran_node_ue_xnap_id;
  uint64_t                              s_ng_ran_node_ue_xnap_id;
  global_ng_ran_node_id_c               target_s_ng_ran_node_id;
  cause_c                               cause;
  pdu_session_sn_change_required_list_l pdu_session_sn_change_required_list;
  unbounded_octstring<true>             sn_to_mn_container;
  scg_ue_history_info_s                 scg_ue_history_info;
  fixed_bitstring<32, false, true>      sn_mob_info;
  global_ng_ran_cell_id_s               source_pscell_id;
  c_pci_nformation_required_s           c_pci_nformation_required;
  q_mcc_onfig_info_s                    source_sn_to_target_sn_qmc_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeChangeRequired ::= SEQUENCE
using sn_ode_change_required_s = elementary_procedure_option<sn_ode_change_required_ies_container>;

// SNodeCounterCheckRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_counter_check_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_counter_check_request_ies_container {
  uint64_t                                m_ng_ran_node_ue_xnap_id;
  uint64_t                                s_ng_ran_node_ue_xnap_id;
  bearers_subject_to_counter_check_list_l bearers_subject_to_counter_check;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeCounterCheckRequest ::= SEQUENCE
using sn_ode_counter_check_request_s = elementary_procedure_option<sn_ode_counter_check_request_ies_container>;

// SNodeModificationConfirm-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_mod_confirm_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_mod_confirm_ies_container {
  bool                                      pdu_session_admitted_mod_sn_mod_confirm_present = false;
  bool                                      pdu_session_released_sn_mod_confirm_present     = false;
  bool                                      mn_to_sn_container_present                      = false;
  bool                                      add_drb_ids_present                             = false;
  bool                                      crit_diagnostics_present                        = false;
  bool                                      mr_dc_res_coordination_info_present             = false;
  bool                                      q_mcc_oordination_resp_present                  = false;
  uint64_t                                  m_ng_ran_node_ue_xnap_id;
  uint64_t                                  s_ng_ran_node_ue_xnap_id;
  pdu_session_admitted_mod_sn_mod_confirm_l pdu_session_admitted_mod_sn_mod_confirm;
  pdu_session_released_sn_mod_confirm_s     pdu_session_released_sn_mod_confirm;
  unbounded_octstring<true>                 mn_to_sn_container;
  drb_list_l                                add_drb_ids;
  crit_diagnostics_s                        crit_diagnostics;
  mr_dc_res_coordination_info_s             mr_dc_res_coordination_info;
  q_mcc_oordination_resp_s                  q_mcc_oordination_resp;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeModificationConfirm ::= SEQUENCE
using sn_ode_mod_confirm_s = elementary_procedure_option<sn_ode_mod_confirm_ies_container>;

// SNodeModificationRefuse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_mod_refuse_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_mod_refuse_ies_container {
  bool                      mn_to_sn_container_present = false;
  bool                      crit_diagnostics_present   = false;
  uint64_t                  m_ng_ran_node_ue_xnap_id;
  uint64_t                  s_ng_ran_node_ue_xnap_id;
  cause_c                   cause;
  unbounded_octstring<true> mn_to_sn_container;
  crit_diagnostics_s        crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeModificationRefuse ::= SEQUENCE
using sn_ode_mod_refuse_s = elementary_procedure_option<sn_ode_mod_refuse_ies_container>;

// UEContextInfo-SNModRequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_context_info_sn_mod_request_ext_ies_o = protocol_ext_empty_o;

using ue_context_info_sn_mod_request_ext_ies_container = protocol_ext_container_empty_l;

// UEContextInfo-SNModRequest ::= SEQUENCE
struct ue_context_info_sn_mod_request_s {
  bool                                              ue_security_cap_present                    = false;
  bool                                              s_ng_ra_nnode_security_key_present         = false;
  bool                                              s_ng_ra_nnode_ue_ambr_present              = false;
  bool                                              idx_to_rat_freq_sel_prio_present           = false;
  bool                                              lower_layer_presence_status_change_present = false;
  bool                                              pdu_session_res_to_be_released_present     = false;
  bool                                              ie_ext_present                             = false;
  ue_security_cap_s                                 ue_security_cap;
  fixed_bitstring<256, false, true>                 s_ng_ra_nnode_security_key;
  ue_aggr_max_bit_rate_s                            s_ng_ra_nnode_ue_ambr;
  uint16_t                                          idx_to_rat_freq_sel_prio = 1;
  lower_layer_presence_status_change_e              lower_layer_presence_status_change;
  pdu_sessions_to_be_added_sn_mod_request_list_l    pdu_session_res_to_be_added;
  pdu_sessions_to_be_modified_sn_mod_request_list_l pdu_session_res_to_be_modified;
  pdu_sessions_to_be_released_sn_mod_request_list_s pdu_session_res_to_be_released;
  ue_context_info_sn_mod_request_ext_ies_container  ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Src-SN-to-Tgt-SNQMCInfoInquiry ::= ENUMERATED
struct src_sn_to_tgt_sn_qmc_info_inquiry_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using src_sn_to_tgt_sn_qmc_info_inquiry_e = enumerated<src_sn_to_tgt_sn_qmc_info_inquiry_opts, true>;

// SNodeModificationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_mod_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_mod_request_ies_container {
  bool                                           pdcp_change_ind_present                              = false;
  bool                                           sel_plmn_present                                     = false;
  bool                                           mob_restrict_list_present                            = false;
  bool                                           scg_cfg_query_present                                = false;
  bool                                           ue_context_info_sn_mod_request_present               = false;
  bool                                           mn_to_sn_container_present                           = false;
  bool                                           requested_split_srb_present                          = false;
  bool                                           requested_split_srb_release_present                  = false;
  bool                                           desired_act_notif_level_present                      = false;
  bool                                           add_drb_ids_present                                  = false;
  bool                                           s_ng_ran_node_max_ip_data_rate_ul_present            = false;
  bool                                           s_ng_ran_node_max_ip_data_rate_dl_present            = false;
  bool                                           location_info_snr_eporting_present                   = false;
  bool                                           mr_dc_res_coordination_info_present                  = false;
  bool                                           pcell_id_present                                     = false;
  bool                                           ne_dc_tdm_pattern_present                            = false;
  bool                                           requested_fast_mcg_recovery_via_srb3_present         = false;
  bool                                           requested_fast_mcg_recovery_via_srb3_release_present = false;
  bool                                           sn_triggered_present                                 = false;
  bool                                           target_node_id_present                               = false;
  bool                                           pscell_history_info_retrieve_present                 = false;
  bool                                           ue_history_info_from_the_ue_present                  = false;
  bool                                           ch_oinfo_mod_req_present                             = false;
  bool                                           scg_activation_request_present                       = false;
  bool                                           cp_a_info_mod_req_present                            = false;
  bool                                           c_pci_nformation_upd_present                         = false;
  bool                                           s_ng_ran_node_ue_slice_mbr_present                   = false;
  bool                                           management_based_mdt_plmn_mod_list_present           = false;
  bool                                           sel_n_id_present                                     = false;
  bool                                           q_mcc_oordination_request_present                    = false;
  bool                                           src_sn_to_tgt_sn_qmc_info_inquiry_present            = false;
  bool                                           iab_authorization_status_present                     = false;
  uint64_t                                       m_ng_ran_node_ue_xnap_id;
  uint64_t                                       s_ng_ran_node_ue_xnap_id;
  cause_c                                        cause;
  pdcp_change_ind_c                              pdcp_change_ind;
  fixed_octstring<3, true>                       sel_plmn;
  mob_restrict_list_s                            mob_restrict_list;
  scg_cfg_query_e                                scg_cfg_query;
  ue_context_info_sn_mod_request_s               ue_context_info_sn_mod_request;
  unbounded_octstring<true>                      mn_to_sn_container;
  split_srbs_types_e                             requested_split_srb;
  split_srbs_types_e                             requested_split_srb_release;
  desired_act_notif_level_e                      desired_act_notif_level;
  drb_list_l                                     add_drb_ids;
  uint64_t                                       s_ng_ran_node_max_ip_data_rate_ul;
  uint64_t                                       s_ng_ran_node_max_ip_data_rate_dl;
  location_info_snr_eporting_e                   location_info_snr_eporting;
  mr_dc_res_coordination_info_s                  mr_dc_res_coordination_info;
  global_ng_ran_cell_id_s                        pcell_id;
  ne_dc_tdm_pattern_s                            ne_dc_tdm_pattern;
  requested_fast_mcg_recovery_via_srb3_e         requested_fast_mcg_recovery_via_srb3;
  requested_fast_mcg_recovery_via_srb3_release_e requested_fast_mcg_recovery_via_srb3_release;
  sn_triggered_e                                 sn_triggered;
  global_ng_ran_node_id_c                        target_node_id;
  pscell_history_info_retrieve_e                 pscell_history_info_retrieve;
  ue_history_info_from_the_ue_c                  ue_history_info_from_the_ue;
  ch_oinfo_mod_req_s                             ch_oinfo_mod_req;
  scg_activation_request_e                       scg_activation_request;
  cp_a_info_mod_req_s                            cp_a_info_mod_req;
  c_pci_nformation_upd_s                         c_pci_nformation_upd;
  ue_slice_max_bit_rate_list_l                   s_ng_ran_node_ue_slice_mbr;
  mdt_plmn_mod_list_l                            management_based_mdt_plmn_mod_list;
  fixed_bitstring<44, false, true>               sel_n_id;
  q_mcc_oordination_request_s                    q_mcc_oordination_request;
  src_sn_to_tgt_sn_qmc_info_inquiry_e            src_sn_to_tgt_sn_qmc_info_inquiry;
  iab_authorization_status_e                     iab_authorization_status;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeModificationRequest ::= SEQUENCE
using sn_ode_mod_request_s = elementary_procedure_option<sn_ode_mod_request_ies_container>;

// SNodeModificationRequestAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_mod_request_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_mod_request_ack_ies_container {
  bool                                      pdu_session_admitted_sn_mod_resp_present        = false;
  bool                                      pdu_session_not_admitted_sn_mod_resp_present    = false;
  bool                                      sn_to_mn_container_present                      = false;
  bool                                      admitted_split_srb_present                      = false;
  bool                                      admitted_split_srb_release_present              = false;
  bool                                      crit_diagnostics_present                        = false;
  bool                                      location_info_sn_present                        = false;
  bool                                      mr_dc_res_coordination_info_present             = false;
  bool                                      pdu_session_data_forwarding_sn_mod_resp_present = false;
  bool                                      rrc_cfg_ind_present                             = false;
  bool                                      available_fast_mcg_recovery_via_srb3_present    = false;
  bool                                      release_fast_mcg_recovery_via_srb3_present      = false;
  bool                                      direct_forwarding_path_availability_present     = false;
  bool                                      scg_ue_history_info_present                     = false;
  bool                                      scg_activation_status_present                   = false;
  bool                                      cp_a_info_mod_req_ack_present                   = false;
  bool                                      q_mcc_oordination_resp_present                  = false;
  bool                                      source_sn_to_target_sn_qmc_info_present         = false;
  uint64_t                                  m_ng_ran_node_ue_xnap_id;
  uint64_t                                  s_ng_ran_node_ue_xnap_id;
  pdu_session_admitted_sn_mod_resp_s        pdu_session_admitted_sn_mod_resp;
  pdu_session_not_admitted_sn_mod_resp_s    pdu_session_not_admitted_sn_mod_resp;
  unbounded_octstring<true>                 sn_to_mn_container;
  split_srbs_types_e                        admitted_split_srb;
  split_srbs_types_e                        admitted_split_srb_release;
  crit_diagnostics_s                        crit_diagnostics;
  target_cgi_c                              location_info_sn;
  mr_dc_res_coordination_info_s             mr_dc_res_coordination_info;
  pdu_session_data_forwarding_sn_mod_resp_s pdu_session_data_forwarding_sn_mod_resp;
  rrc_cfg_ind_e                             rrc_cfg_ind;
  available_fast_mcg_recovery_via_srb3_e    available_fast_mcg_recovery_via_srb3;
  release_fast_mcg_recovery_via_srb3_e      release_fast_mcg_recovery_via_srb3;
  direct_forwarding_path_availability_e     direct_forwarding_path_availability;
  scg_ue_history_info_s                     scg_ue_history_info;
  scg_activation_status_e                   scg_activation_status;
  cp_a_info_mod_req_ack_s                   cp_a_info_mod_req_ack;
  q_mcc_oordination_resp_s                  q_mcc_oordination_resp;
  q_mcc_onfig_info_s                        source_sn_to_target_sn_qmc_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeModificationRequestAcknowledge ::= SEQUENCE
using sn_ode_mod_request_ack_s = elementary_procedure_option<sn_ode_mod_request_ack_ies_container>;

// SNodeModificationRequestReject-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_mod_request_reject_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_mod_request_reject_ies_container {
  bool               crit_diagnostics_present = false;
  uint64_t           m_ng_ran_node_ue_xnap_id;
  uint64_t           s_ng_ran_node_ue_xnap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeModificationRequestReject ::= SEQUENCE
using sn_ode_mod_request_reject_s = elementary_procedure_option<sn_ode_mod_request_reject_ies_container>;

// SNodeModificationRequired-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_mod_required_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_mod_required_ies_container {
  bool                                         pdcp_change_ind_present                            = false;
  bool                                         pdu_session_to_be_modified_sn_mod_required_present = false;
  bool                                         pdu_session_to_be_released_sn_mod_required_present = false;
  bool                                         sn_to_mn_container_present                         = false;
  bool                                         spare_drb_ids_present                              = false;
  bool                                         required_nof_drb_ids_present                       = false;
  bool                                         location_info_sn_present                           = false;
  bool                                         mr_dc_res_coordination_info_present                = false;
  bool                                         rrc_cfg_ind_present                                = false;
  bool                                         available_fast_mcg_recovery_via_srb3_present       = false;
  bool                                         release_fast_mcg_recovery_via_srb3_present         = false;
  bool                                         scg_ind_present                                    = false;
  bool                                         scg_ue_history_info_present                        = false;
  bool                                         scg_activation_request_present                     = false;
  bool                                         cp_ac_info_mod_required_present                    = false;
  bool                                         sc_grecfg_notif_present                            = false;
  bool                                         spr_availability_present                           = false;
  bool                                         q_mcc_oordination_request_present                  = false;
  bool                                         s_cp_ac_request_present                            = false;
  bool                                         pdu_sessions_list_to_be_released_up_error_present  = false;
  uint64_t                                     m_ng_ran_node_ue_xnap_id;
  uint64_t                                     s_ng_ran_node_ue_xnap_id;
  cause_c                                      cause;
  pdcp_change_ind_c                            pdcp_change_ind;
  pdu_session_to_be_modified_sn_mod_required_l pdu_session_to_be_modified_sn_mod_required;
  pdu_session_to_be_released_sn_mod_required_s pdu_session_to_be_released_sn_mod_required;
  unbounded_octstring<true>                    sn_to_mn_container;
  drb_list_l                                   spare_drb_ids;
  uint8_t                                      required_nof_drb_ids;
  target_cgi_c                                 location_info_sn;
  mr_dc_res_coordination_info_s                mr_dc_res_coordination_info;
  rrc_cfg_ind_e                                rrc_cfg_ind;
  available_fast_mcg_recovery_via_srb3_e       available_fast_mcg_recovery_via_srb3;
  release_fast_mcg_recovery_via_srb3_e         release_fast_mcg_recovery_via_srb3;
  scg_ind_e                                    scg_ind;
  scg_ue_history_info_s                        scg_ue_history_info;
  scg_activation_request_e                     scg_activation_request;
  cp_ac_info_mod_required_s                    cp_ac_info_mod_required;
  sc_grecfg_notif_e                            sc_grecfg_notif;
  spr_availability_e                           spr_availability;
  q_mcc_oordination_request_s                  q_mcc_oordination_request;
  s_cp_ac_request_e                            s_cp_ac_request;
  pdu_sessions_list_to_be_released_up_error_l  pdu_sessions_list_to_be_released_up_error;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeModificationRequired ::= SEQUENCE
using sn_ode_mod_required_s = elementary_procedure_option<sn_ode_mod_required_ies_container>;

// SNodeReconfigurationComplete-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_recfg_complete_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_recfg_complete_ies_container {
  uint64_t                 m_ng_ran_node_ue_xnap_id;
  uint64_t                 s_ng_ran_node_ue_xnap_id;
  resp_info_reconf_compl_s resp_info_reconf_compl;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeReconfigurationComplete ::= SEQUENCE
using sn_ode_recfg_complete_s = elementary_procedure_option<sn_ode_recfg_complete_ies_container>;

// SNodeReleaseConfirm-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_release_confirm_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_release_confirm_ies_container {
  bool                                 pdu_session_released_list_rel_conf_present = false;
  bool                                 crit_diagnostics_present                   = false;
  uint64_t                             m_ng_ran_node_ue_xnap_id;
  uint64_t                             s_ng_ran_node_ue_xnap_id;
  pdu_session_released_list_rel_conf_s pdu_session_released_list_rel_conf;
  crit_diagnostics_s                   crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeReleaseConfirm ::= SEQUENCE
using sn_ode_release_confirm_s = elementary_procedure_option<sn_ode_release_confirm_ies_container>;

// SNodeReleaseReject-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_release_reject_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_release_reject_ies_container {
  bool               s_ng_ran_node_ue_xnap_id_present = false;
  bool               crit_diagnostics_present         = false;
  uint64_t           m_ng_ran_node_ue_xnap_id;
  uint64_t           s_ng_ran_node_ue_xnap_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeReleaseReject ::= SEQUENCE
using sn_ode_release_reject_s = elementary_procedure_option<sn_ode_release_reject_ies_container>;

// SNodeReleaseRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_release_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_release_request_ies_container {
  bool                          s_ng_ran_node_ue_xnap_id_present           = false;
  bool                          pdu_session_to_be_released_rel_req_present = false;
  bool                          ue_context_kept_ind_present                = false;
  bool                          mn_to_sn_container_present                 = false;
  bool                          drbs_transferred_to_mn_present             = false;
  uint64_t                      m_ng_ran_node_ue_xnap_id;
  uint64_t                      s_ng_ran_node_ue_xnap_id;
  cause_c                       cause;
  pdu_session_list_with_cause_l pdu_session_to_be_released_rel_req;
  ue_context_kept_ind_e         ue_context_kept_ind;
  unbounded_octstring<true>     mn_to_sn_container;
  drb_list_l                    drbs_transferred_to_mn;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeReleaseRequest ::= SEQUENCE
using sn_ode_release_request_s = elementary_procedure_option<sn_ode_release_request_ies_container>;

// SNodeReleaseRequestAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_release_request_ack_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_release_request_ack_ies_container {
  bool                                          s_ng_ran_node_ue_xnap_id_present               = false;
  bool                                          pdu_session_to_be_released_rel_req_ack_present = false;
  bool                                          crit_diagnostics_present                       = false;
  bool                                          scg_ue_history_info_present                    = false;
  bool                                          sn_mob_info_present                            = false;
  uint64_t                                      m_ng_ran_node_ue_xnap_id;
  uint64_t                                      s_ng_ran_node_ue_xnap_id;
  pdu_session_to_be_released_list_rel_req_ack_s pdu_session_to_be_released_rel_req_ack;
  crit_diagnostics_s                            crit_diagnostics;
  scg_ue_history_info_s                         scg_ue_history_info;
  fixed_bitstring<32, false, true>              sn_mob_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeReleaseRequestAcknowledge ::= SEQUENCE
using sn_ode_release_request_ack_s = elementary_procedure_option<sn_ode_release_request_ack_ies_container>;

// SNodeReleaseRequired-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct sn_ode_release_required_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct sn_ode_release_required_ies_container {
  bool                                        pdu_session_to_be_released_list_rel_rqd_present   = false;
  bool                                        sn_to_mn_container_present                        = false;
  bool                                        scg_ue_history_info_present                       = false;
  bool                                        pdu_sessions_list_to_be_released_up_error_present = false;
  uint64_t                                    m_ng_ran_node_ue_xnap_id;
  uint64_t                                    s_ng_ran_node_ue_xnap_id;
  pdu_session_to_be_released_list_rel_rqd_s   pdu_session_to_be_released_list_rel_rqd;
  cause_c                                     cause;
  unbounded_octstring<true>                   sn_to_mn_container;
  scg_ue_history_info_s                       scg_ue_history_info;
  pdu_sessions_list_to_be_released_up_error_l pdu_sessions_list_to_be_released_up_error;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNodeReleaseRequired ::= SEQUENCE
using sn_ode_release_required_s = elementary_procedure_option<sn_ode_release_required_ies_container>;

// ScgFailureInformationReport-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct scg_fail_info_report_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct scg_fail_info_report_ies_container {
  bool                             source_pscell_cgi_present = false;
  bool                             failed_pscell_cgi_present = false;
  bool                             sn_mob_info_present       = false;
  bool                             cp_ac_cfg_present         = false;
  uint64_t                         m_ng_ran_node_ue_xnap_id;
  uint64_t                         s_ng_ran_node_ue_xnap_id;
  global_ng_ran_cell_id_s          source_pscell_cgi;
  global_ng_ran_cell_id_s          failed_pscell_cgi;
  unbounded_octstring<true>        scg_fail_report_container;
  fixed_bitstring<32, false, true> sn_mob_info;
  cp_ac_cfg_s                      cp_ac_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ScgFailureInformationReport ::= SEQUENCE
using scg_fail_info_report_s = elementary_procedure_option<scg_fail_info_report_ies_container>;

// ScgFailureTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct scg_fail_transfer_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct scg_fail_transfer_ies_container {
  uint64_t m_ng_ran_node_ue_xnap_id;
  uint64_t s_ng_ran_node_ue_xnap_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ScgFailureTransfer ::= SEQUENCE
using scg_fail_transfer_s = elementary_procedure_option<scg_fail_transfer_ies_container>;

// SecondaryRATDataUsageReport-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct secondary_rat_data_usage_report_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct secondary_rat_data_usage_report_ies_container {
  uint64_t                                   m_ng_ran_node_ue_xnap_id;
  uint64_t                                   s_ng_ran_node_ue_xnap_id;
  pdu_session_res_secondary_rat_usage_list_l pdu_session_res_secondary_rat_usage_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SecondaryRATDataUsageReport ::= SEQUENCE
using secondary_rat_data_usage_report_s = elementary_procedure_option<secondary_rat_data_usage_report_ies_container>;

// TraceStartIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct trace_start_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trace_start_ies_container {
  bool               trace_activation_present = false;
  uint64_t           m_ng_ran_node_ue_xnap_id;
  uint64_t           s_ng_ran_node_ue_xnap_id;
  trace_activation_s trace_activation;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TraceStart ::= SEQUENCE
using trace_start_s = elementary_procedure_option<trace_start_ies_container>;

// UEContextRelease-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ue_context_release_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_release_ies_container {
  uint64_t source_ng_ran_node_ue_xnap_id;
  uint64_t target_ng_ran_node_ue_xnap_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextRelease ::= SEQUENCE
using ue_context_release_s = elementary_procedure_option<ue_context_release_ies_container>;

// XnRemovalFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct xn_removal_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct xn_removal_fail_ies_container {
  bool               crit_diagnostics_present       = false;
  bool               interface_instance_ind_present = false;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;
  uint16_t           interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnRemovalFailure ::= SEQUENCE
using xn_removal_fail_s = elementary_procedure_option<xn_removal_fail_ies_container>;

// XnRemovalRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct xn_removal_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct xn_removal_request_ies_container {
  bool                    xn_removal_thres_present       = false;
  bool                    interface_instance_ind_present = false;
  global_ng_ran_node_id_c global_ng_ran_node_id;
  uint8_t                 xn_removal_thres;
  uint16_t                interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnRemovalRequest ::= SEQUENCE
using xn_removal_request_s = elementary_procedure_option<xn_removal_request_ies_container>;

// XnRemovalResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct xn_removal_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct xn_removal_resp_ies_container {
  bool                    crit_diagnostics_present       = false;
  bool                    interface_instance_ind_present = false;
  global_ng_ran_node_id_c global_ng_ran_node_id;
  crit_diagnostics_s      crit_diagnostics;
  uint16_t                interface_instance_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnRemovalResponse ::= SEQUENCE
using xn_removal_resp_s = elementary_procedure_option<xn_removal_resp_ies_container>;

// XnSetupFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct xn_setup_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct xn_setup_fail_ies_container {
  bool                 time_to_wait_present           = false;
  bool                 crit_diagnostics_present       = false;
  bool                 interface_instance_ind_present = false;
  bool                 msg_oversize_notif_present     = false;
  cause_c              cause;
  time_to_wait_e       time_to_wait;
  crit_diagnostics_s   crit_diagnostics;
  uint16_t             interface_instance_ind;
  msg_oversize_notif_s msg_oversize_notif;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnSetupFailure ::= SEQUENCE
using xn_setup_fail_s = elementary_procedure_option<xn_setup_fail_ies_container>;

// XnSetupRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct xn_setup_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct xn_setup_request_ies_container {
  bool                                  list_of_served_cells_nr_present             = false;
  bool                                  list_of_served_cells_e_utra_present         = false;
  bool                                  interface_instance_ind_present              = false;
  bool                                  tnl_cfg_info_present                        = false;
  bool                                  partial_list_ind_nr_present                 = false;
  bool                                  cell_and_capacity_assist_info_nr_present    = false;
  bool                                  partial_list_ind_eutra_present              = false;
  bool                                  cell_and_capacity_assist_info_eutra_present = false;
  bool                                  local_ng_ran_node_id_present                = false;
  bool                                  neighbour_ng_ran_node_list_present          = false;
  global_ng_ran_node_id_c               global_ng_ran_node_id;
  tai_support_list_l                    tai_support_list;
  amf_region_info_l                     amf_region_info;
  served_cells_nr_l                     list_of_served_cells_nr;
  served_cells_e_utra_l                 list_of_served_cells_e_utra;
  uint16_t                              interface_instance_ind;
  tnl_cfg_info_s                        tnl_cfg_info;
  partial_list_ind_e                    partial_list_ind_nr;
  cell_and_capacity_assist_info_nr_s    cell_and_capacity_assist_info_nr;
  partial_list_ind_e                    partial_list_ind_eutra;
  cell_and_capacity_assist_info_eutra_s cell_and_capacity_assist_info_eutra;
  local_ng_ran_node_id_c                local_ng_ran_node_id;
  neighbour_ng_ran_node_list_l          neighbour_ng_ran_node_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnSetupRequest ::= SEQUENCE
using xn_setup_request_s = elementary_procedure_option<xn_setup_request_ies_container>;

// XnSetupResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct xn_setup_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct xn_setup_resp_ies_container {
  bool                                  list_of_served_cells_nr_present             = false;
  bool                                  list_of_served_cells_e_utra_present         = false;
  bool                                  crit_diagnostics_present                    = false;
  bool                                  amf_region_info_present                     = false;
  bool                                  interface_instance_ind_present              = false;
  bool                                  tnl_cfg_info_present                        = false;
  bool                                  partial_list_ind_nr_present                 = false;
  bool                                  cell_and_capacity_assist_info_nr_present    = false;
  bool                                  partial_list_ind_eutra_present              = false;
  bool                                  cell_and_capacity_assist_info_eutra_present = false;
  bool                                  local_ng_ran_node_id_present                = false;
  bool                                  neighbour_ng_ran_node_list_present          = false;
  global_ng_ran_node_id_c               global_ng_ran_node_id;
  tai_support_list_l                    tai_support_list;
  served_cells_nr_l                     list_of_served_cells_nr;
  served_cells_e_utra_l                 list_of_served_cells_e_utra;
  crit_diagnostics_s                    crit_diagnostics;
  amf_region_info_l                     amf_region_info;
  uint16_t                              interface_instance_ind;
  tnl_cfg_info_s                        tnl_cfg_info;
  partial_list_ind_e                    partial_list_ind_nr;
  cell_and_capacity_assist_info_nr_s    cell_and_capacity_assist_info_nr;
  partial_list_ind_e                    partial_list_ind_eutra;
  cell_and_capacity_assist_info_eutra_s cell_and_capacity_assist_info_eutra;
  local_ng_ran_node_id_c                local_ng_ran_node_id;
  neighbour_ng_ran_node_list_l          neighbour_ng_ran_node_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnSetupResponse ::= SEQUENCE
using xn_setup_resp_s = elementary_procedure_option<xn_setup_resp_ies_container>;

// XnUAddressIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct xn_u_address_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct xn_u_address_ind_ies_container {
  bool                                    cho_mrdc_ind_present                   = false;
  bool                                    cho_mrdc_early_data_forwarding_present = false;
  bool                                    cp_c_data_forwarding_ind_present       = false;
  bool                                    mbs_data_forwarding_ind_present        = false;
  bool                                    mbs_session_info_resp_list_present     = false;
  bool                                    pdu_setbased_handling_ind_present      = false;
  uint64_t                                new_ng_ran_node_ue_xnap_id;
  uint64_t                                old_ng_ran_node_ue_xnap_id;
  xn_u_address_infoper_pdu_session_list_l xn_u_address_infoper_pdu_session_list;
  cho_mrdc_ind_e                          cho_mrdc_ind;
  cho_mrdc_early_data_forwarding_e        cho_mrdc_early_data_forwarding;
  cp_c_data_forwarding_ind_e              cp_c_data_forwarding_ind;
  mbs_data_forwarding_ind_e               mbs_data_forwarding_ind;
  mbs_session_info_resp_list_l            mbs_session_info_resp_list;
  pdu_setbased_handling_ind_e             pdu_setbased_handling_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnUAddressIndication ::= SEQUENCE
using xn_u_address_ind_s = elementary_procedure_option<xn_u_address_ind_ies_container>;

} // namespace xnap
} // namespace asn1

extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::activ_nr_cells_and_ss_bs_item_s::ss_bs_activ_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::activ_nr_cells_and_ss_bs_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_activity_notify_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_activity_notify_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::bearers_subject_to_counter_check_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::boundary_node_cells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::to_be_activ_nr_cells_and_ss_bs_item_s::ss_bstobe_activ_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::to_be_activ_nr_cells_and_ss_bs_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::parent_node_cells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::iab_tnl_address_to_be_released_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_to_be_added_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_to_be_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_added_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_not_added_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_not_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::traffic_released_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_required_to_be_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_required_modified_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_notify_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_sn_change_confirm_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_sn_change_required_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_admitted_to_be_added_sn_mod_resp_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_admitted_to_be_modified_sn_mod_resp_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_admitted_added_add_req_ack_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_admitted_mod_sn_mod_confirm_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_to_be_added_add_req_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_to_be_modified_sn_mod_required_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_sessions_to_be_added_sn_mod_request_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_sessions_to_be_modified_sn_mod_request_list_l::value_type>::ops;
