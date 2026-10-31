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

#include "ocudu/asn1/asn1_ap_utils.h"

namespace asn1 {
namespace xnap {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// TriggeringMessage ::= ENUMERATED
struct trigger_msg_opts {
  enum options { init_msg, successful_outcome, unsuccessful_outcome, nulltype } value;

  const char* to_string() const;
};
using trigger_msg_e = enumerated<trigger_msg_opts>;

// A2XPC5FlowBitRates-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using a2_xpc5_flow_bit_rates_ext_ies_o = protocol_ext_empty_o;

using a2_xpc5_flow_bit_rates_ext_ies_container = protocol_ext_container_empty_l;

// A2XPC5FlowBitRates ::= SEQUENCE
struct a2_xpc5_flow_bit_rates_s {
  bool                                     ie_exts_present              = false;
  uint64_t                                 a2_xguaranteed_flow_bit_rate = 0;
  uint64_t                                 a2_xmax_flow_bit_rate        = 0;
  a2_xpc5_flow_bit_rates_ext_ies_container ie_exts;
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

// A2XPC5QoSFlowItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using a2_xpc5_qos_flow_item_ext_ies_o = protocol_ext_empty_o;

using a2_xpc5_qos_flow_item_ext_ies_container = protocol_ext_container_empty_l;

// A2XPC5QoSFlowItem ::= SEQUENCE
struct a2_xpc5_qos_flow_item_s {
  bool                                    a2_xpc5_flow_bit_rates_present = false;
  bool                                    a2_xrange_present              = false;
  bool                                    ie_exts_present                = false;
  uint16_t                                a2_xp_qi                       = 0;
  a2_xpc5_flow_bit_rates_s                a2_xpc5_flow_bit_rates;
  range_e                                 a2_xrange;
  a2_xpc5_qos_flow_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// A2XPC5QoSFlowList ::= SEQUENCE (SIZE (1..2064)) OF A2XPC5QoSFlowItem
using a2_xpc5_qos_flow_list_l = dyn_array<a2_xpc5_qos_flow_item_s>;

// A2XPC5QoSParameters-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using a2_xpc5_qos_params_ext_ies_o = protocol_ext_empty_o;

using a2_xpc5_qos_params_ext_ies_container = protocol_ext_container_empty_l;

// A2XPC5QoSParameters ::= SEQUENCE
struct a2_xpc5_qos_params_s {
  bool                                 aa2_xpc5_link_aggr_bit_rates_present = false;
  bool                                 ie_exts_present                      = false;
  a2_xpc5_qos_flow_list_l              a2_xpc5_qos_flow_list;
  uint64_t                             aa2_xpc5_link_aggr_bit_rates = 0;
  a2_xpc5_qos_params_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GlobalAMF-Region-Information-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using global_amf_region_info_ext_ies_o = protocol_ext_empty_o;

using global_amf_region_info_ext_ies_container = protocol_ext_container_empty_l;

// GlobalAMF-Region-Information ::= SEQUENCE
struct global_amf_region_info_s {
  bool                                     ie_exts_present = false;
  fixed_octstring<3, true>                 plmn_id;
  fixed_bitstring<8, false, true>          amf_region_id;
  global_amf_region_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AMF-Region-Information ::= SEQUENCE (SIZE (1..16)) OF GlobalAMF-Region-Information
using amf_region_info_l = dyn_array<global_amf_region_info_s>;

// AS-SecurityInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using as_security_info_ext_ies_o = protocol_ext_empty_o;

using as_security_info_ext_ies_container = protocol_ext_container_empty_l;

// AS-SecurityInformation ::= SEQUENCE
struct as_security_info_s {
  bool                               ie_exts_present = false;
  fixed_bitstring<256, false, true>  key_ng_ran_star;
  uint8_t                            ncc = 0;
  as_security_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NonDynamic5QIDescriptor-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
  bool                                     prio_level_qos_present        = false;
  bool                                     averaging_win_present         = false;
  bool                                     max_data_burst_volume_present = false;
  bool                                     ie_ext_present                = false;
  uint16_t                                 five_qi                       = 0;
  uint16_t                                 prio_level_qos                = 1;
  uint16_t                                 averaging_win                 = 0;
  uint16_t                                 max_data_burst_volume         = 0;
  non_dyn_5qi_descriptor_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PacketErrorRate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// Dynamic5QIDescriptor-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
    enum options { delay_crit, non_delay_crit, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using delay_crit_e_ = enumerated<delay_crit_opts, true>;

  // member variables
  bool                                 five_qi_present               = false;
  bool                                 delay_crit_present            = false;
  bool                                 averaging_win_present         = false;
  bool                                 max_data_burst_volume_present = false;
  bool                                 ie_ext_present                = false;
  uint16_t                             prio_level_qos                = 1;
  uint16_t                             packet_delay_budget           = 0;
  packet_error_rate_s                  packet_error_rate;
  uint16_t                             five_qi = 0;
  delay_crit_e_                        delay_crit;
  uint16_t                             averaging_win         = 0;
  uint16_t                             max_data_burst_volume = 0;
  dyn_5qi_descriptor_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSCharacteristics-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using qos_characteristics_ext_ies_o = protocol_ies_empty_o;

// QoSCharacteristics ::= CHOICE
struct qos_characteristics_c {
  struct types_opts {
    enum options { non_dyn, dyn, choice_ext, nulltype } value;

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
  non_dyn_5qi_descriptor_s& non_dyn()
  {
    assert_choice_type(types::non_dyn, type_, "QoSCharacteristics");
    return c.get<non_dyn_5qi_descriptor_s>();
  }
  dyn_5qi_descriptor_s& dyn()
  {
    assert_choice_type(types::dyn, type_, "QoSCharacteristics");
    return c.get<dyn_5qi_descriptor_s>();
  }
  protocol_ie_single_container_s<qos_characteristics_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "QoSCharacteristics");
    return c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>();
  }
  const non_dyn_5qi_descriptor_s& non_dyn() const
  {
    assert_choice_type(types::non_dyn, type_, "QoSCharacteristics");
    return c.get<non_dyn_5qi_descriptor_s>();
  }
  const dyn_5qi_descriptor_s& dyn() const
  {
    assert_choice_type(types::dyn, type_, "QoSCharacteristics");
    return c.get<dyn_5qi_descriptor_s>();
  }
  const protocol_ie_single_container_s<qos_characteristics_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "QoSCharacteristics");
    return c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>();
  }
  non_dyn_5qi_descriptor_s&                                      set_non_dyn();
  dyn_5qi_descriptor_s&                                          set_dyn();
  protocol_ie_single_container_s<qos_characteristics_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<dyn_5qi_descriptor_s,
                  non_dyn_5qi_descriptor_s,
                  protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>
      c;

  void destroy_();
};

// AllocationandRetentionPriority-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using allocand_retention_prio_ext_ies_o = protocol_ext_empty_o;

using allocand_retention_prio_ext_ies_container = protocol_ext_container_empty_l;

// AllocationandRetentionPriority ::= SEQUENCE
struct allocand_retention_prio_s {
  struct pre_emption_cap_opts {
    enum options { shall_not_trigger_preemption, may_trigger_preemption, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using pre_emption_cap_e_ = enumerated<pre_emption_cap_opts, true>;
  struct pre_emption_vulnerability_opts {
    enum options { not_preemptable, preemptable, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using pre_emption_vulnerability_e_ = enumerated<pre_emption_vulnerability_opts, true>;

  // member variables
  bool                                      ie_exts_present = false;
  uint8_t                                   prio_level      = 0;
  pre_emption_cap_e_                        pre_emption_cap;
  pre_emption_vulnerability_e_              pre_emption_vulnerability;
  allocand_retention_prio_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AlternativeQoSParaSetItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// GBRQoSFlowInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// GBRQoSFlowInfo ::= SEQUENCE
struct gbr_qos_flow_info_s {
  struct notif_ctrl_opts {
    enum options { notif_requested, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using notif_ctrl_e_ = enumerated<notif_ctrl_opts, true>;

  // member variables
  bool                                notif_ctrl_present              = false;
  bool                                max_packet_loss_rate_dl_present = false;
  bool                                max_packet_loss_rate_ul_present = false;
  bool                                ie_exts_present                 = false;
  uint64_t                            max_flow_bit_rate_dl            = 0;
  uint64_t                            max_flow_bit_rate_ul            = 0;
  uint64_t                            guaranteed_flow_bit_rate_dl     = 0;
  uint64_t                            guaranteed_flow_bit_rate_ul     = 0;
  notif_ctrl_e_                       notif_ctrl;
  uint16_t                            max_packet_loss_rate_dl = 0;
  uint16_t                            max_packet_loss_rate_ul = 0;
  gbr_qos_flow_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ReflectiveQoSAttribute ::= ENUMERATED
struct reflective_qos_attribute_opts {
  enum options { subject_to_reflective_qos, /*...*/ nulltype } value;

  const char* to_string() const;
};
using reflective_qos_attribute_e = enumerated<reflective_qos_attribute_opts, true>;

// QosMonitoringRequest ::= ENUMERATED
struct qos_monitoring_request_opts {
  enum options { ul, dl, both, nulltype } value;

  const char* to_string() const;
};
using qos_monitoring_request_e = enumerated<qos_monitoring_request_opts>;

// QoSMonitoringDisabled ::= ENUMERATED
struct qos_monitoring_disabled_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using qos_monitoring_disabled_e = enumerated<qos_monitoring_disabled_opts, true>;

// PDUSetQoSInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
  uint32_t                            pdu_set_delay_budget                     = 0;
  packet_error_rate_s                 pdu_set_error_rate;
  pdu_set_integrated_handling_info_e_ pdu_set_integrated_handling_info;
  pdu_set_qos_info_ext_ies_container  ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSetQoSParameters-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// QoSFlowLevelQoSParameters-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flow_level_qos_params_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flow_level_qos_params_ext_ies_container {
  bool                      qos_monitoring_request_present     = false;
  bool                      qos_monitoring_report_freq_present = false;
  bool                      qos_monitoring_disabled_present    = false;
  bool                      pdu_set_qos_params_present         = false;
  qos_monitoring_request_e  qos_monitoring_request;
  uint16_t                  qos_monitoring_report_freq;
  qos_monitoring_disabled_e qos_monitoring_disabled;
  pdu_set_qos_params_s      pdu_set_qos_params;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowLevelQoSParameters ::= SEQUENCE
struct qos_flow_level_qos_params_s {
  struct add_qos_flow_info_opts {
    enum options { more_likely, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using add_qos_flow_info_e_ = enumerated<add_qos_flow_info_opts, true>;

  // member variables
  bool                                        gbr_qos_flow_info_present = false;
  bool                                        reflective_qos_present    = false;
  bool                                        add_qos_flow_info_present = false;
  bool                                        ie_exts_present           = false;
  qos_characteristics_c                       qos_characteristics;
  allocand_retention_prio_s                   alloc_and_retention_prio;
  gbr_qos_flow_info_s                         gbr_qos_flow_info;
  reflective_qos_attribute_e                  reflective_qos;
  add_qos_flow_info_e_                        add_qos_flow_info;
  qos_flow_level_qos_params_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-QoSFlowsToAdd-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mbs_qos_flows_to_add_item_ext_ies_o = protocol_ext_empty_o;

using mbs_qos_flows_to_add_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-QoSFlowsToAdd-Item ::= SEQUENCE
struct mbs_qos_flows_to_add_item_s {
  bool                                        ie_exts_present = false;
  uint8_t                                     mbs_qos_flow_id = 0;
  qos_flow_level_qos_params_s                 mbs_qos_flow_level_qos_params;
  mbs_qos_flows_to_add_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-QoSFlowsToAdd-List ::= SEQUENCE (SIZE (1..64)) OF MBS-QoSFlowsToAdd-Item
using mbs_qos_flows_to_add_list_l = dyn_array<mbs_qos_flows_to_add_item_s>;

// NR-CGI-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_cgi_ext_ies_o = protocol_ext_empty_o;

using nr_cgi_ext_ies_container = protocol_ext_container_empty_l;

// NR-CGI ::= SEQUENCE
struct nr_cgi_s {
  bool                             ie_ext_present = false;
  fixed_octstring<3, true>         plmn_id;
  fixed_bitstring<36, false, true> nr_ci;
  nr_cgi_ext_ies_container         ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-ServiceAreaCell-List ::= SEQUENCE (SIZE (1..8192)) OF NR-CGI
using mbs_service_area_cell_list_l = dyn_array<nr_cgi_s>;

// MBS-ServiceAreaTAI-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mbs_service_area_tai_item_ext_ies_o = protocol_ext_empty_o;

using mbs_service_area_tai_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-ServiceAreaTAI-Item ::= SEQUENCE
struct mbs_service_area_tai_item_s {
  bool                                        ie_exts_present = false;
  fixed_octstring<3, true>                    plmn_id;
  fixed_octstring<3, true>                    tac;
  mbs_service_area_tai_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-ServiceAreaTAI-List ::= SEQUENCE (SIZE (1..1024)) OF MBS-ServiceAreaTAI-Item
using mbs_service_area_tai_list_l = dyn_array<mbs_service_area_tai_item_s>;

// MBS-ServiceAreaInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// MBS-ServiceAreaInformation-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mbs_service_area_info_item_ext_ies_o = protocol_ext_empty_o;

using mbs_service_area_info_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-ServiceAreaInformation-Item ::= SEQUENCE
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

// MBS-ServiceAreaInformationList ::= SEQUENCE (SIZE (1..256)) OF MBS-ServiceAreaInformation-Item
using mbs_service_area_info_list_l = dyn_array<mbs_service_area_info_item_s>;

// MBS-ServiceArea-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using mbs_service_area_ext_ies_o = protocol_ies_empty_o;

// MBS-ServiceArea ::= CHOICE
struct mbs_service_area_c {
  struct types_opts {
    enum options { locationindependent, locationdependent, choice_ext, nulltype } value;

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
  protocol_ie_single_container_s<mbs_service_area_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MBS-ServiceArea");
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
  const protocol_ie_single_container_s<mbs_service_area_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MBS-ServiceArea");
    return c.get<protocol_ie_single_container_s<mbs_service_area_ext_ies_o>>();
  }
  mbs_service_area_info_s&                                    set_locationindependent();
  mbs_service_area_info_list_l&                               set_locationdependent();
  protocol_ie_single_container_s<mbs_service_area_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<mbs_service_area_info_list_l,
                  mbs_service_area_info_s,
                  protocol_ie_single_container_s<mbs_service_area_ext_ies_o>>
      c;

  void destroy_();
};

// MBS-QoSFlow-List ::= SEQUENCE (SIZE (1..64)) OF INTEGER (0..63,...)
using mbs_qos_flow_list_l = dyn_array<uint8_t>;

// MRB-ProgressInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
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

// MBS-MappingandDataForwardingRequestInfofromSource-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mbs_mapand_data_forwarding_request_infofrom_source_item_ext_ies_o = protocol_ext_empty_o;

using mbs_mapand_data_forwarding_request_infofrom_source_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-MappingandDataForwardingRequestInfofromSource-Item ::= SEQUENCE
struct mbs_mapand_data_forwarding_request_infofrom_source_item_s {
  bool                                                                      mrb_progress_info_present = false;
  bool                                                                      ie_exts_present           = false;
  uint16_t                                                                  mrb_id                    = 1;
  mbs_qos_flow_list_l                                                       mbs_qos_flow_list;
  mrb_progress_info_c                                                       mrb_progress_info;
  mbs_mapand_data_forwarding_request_infofrom_source_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-MappingandDataForwardingRequestInfofromSource ::= SEQUENCE (SIZE (1..32)) OF
// MBS-MappingandDataForwardingRequestInfofromSource-Item
using mbs_mapand_data_forwarding_request_infofrom_source_l =
    dyn_array<mbs_mapand_data_forwarding_request_infofrom_source_item_s>;

// Active-MBS-SessionInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using active_mbs_session_info_ext_ies_o = protocol_ext_empty_o;

using active_mbs_session_info_ext_ies_container = protocol_ext_container_empty_l;

// Active-MBS-SessionInformation ::= SEQUENCE
struct active_mbs_session_info_s {
  bool                                                 mbs_service_area_present = false;
  bool                                                 ie_exts_present          = false;
  mbs_qos_flows_to_add_list_l                          mbs_qos_flows_to_add_list;
  mbs_service_area_c                                   mbs_service_area;
  mbs_mapand_data_forwarding_request_infofrom_source_l mbs_mapand_data_forwarding_request_infofrom_source;
  active_mbs_session_info_ext_ies_container            ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionLocationInformation ::= ENUMERATED
struct addition_location_info_opts {
  enum options { include_pscell, /*...*/ nulltype } value;

  const char* to_string() const;
};
using addition_location_info_e = enumerated<addition_location_info_opts, true>;

// CSI-RS-MTC-Neighbour-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using csi_rs_mtc_neighbour_item_ext_ies_o = protocol_ext_empty_o;

using csi_rs_mtc_neighbour_item_ext_ies_container = protocol_ext_container_empty_l;

// CSI-RS-MTC-Neighbour-Item ::= SEQUENCE
struct csi_rs_mtc_neighbour_item_s {
  bool                                        ie_exts_present = false;
  uint8_t                                     csi_rs_idx      = 0;
  csi_rs_mtc_neighbour_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CSI-RS-MTC-Neighbour-List ::= SEQUENCE (SIZE (1..16)) OF CSI-RS-MTC-Neighbour-Item
using csi_rs_mtc_neighbour_list_l = dyn_array<csi_rs_mtc_neighbour_item_s>;

// CSI-RS-Neighbour-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using csi_rs_neighbour_item_ext_ies_o = protocol_ext_empty_o;

using csi_rs_neighbour_item_ext_ies_container = protocol_ext_container_empty_l;

// CSI-RS-Neighbour-Item ::= SEQUENCE
struct csi_rs_neighbour_item_s {
  bool                                    ie_exts_present = false;
  nr_cgi_s                                nr_cgi;
  csi_rs_mtc_neighbour_list_l             csi_rs_mtc_neighbour_list;
  csi_rs_neighbour_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CSI-RS-Neighbour-List ::= SEQUENCE (SIZE (1..16)) OF CSI-RS-Neighbour-Item
using csi_rs_neighbour_list_l = dyn_array<csi_rs_neighbour_item_s>;

// CSI-RS-MTC-Configuration-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using csi_rs_mtc_cfg_item_ext_ies_o = protocol_ext_empty_o;

using csi_rs_mtc_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// CSI-RS-MTC-Configuration-Item ::= SEQUENCE
struct csi_rs_mtc_cfg_item_s {
  struct csi_rs_status_opts {
    enum options { activ, deactiv, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using csi_rs_status_e_ = enumerated<csi_rs_status_opts, true>;

  // member variables
  bool                                  ie_exts_present = false;
  uint8_t                               csi_rs_idx      = 0;
  csi_rs_status_e_                      csi_rs_status;
  csi_rs_neighbour_list_l               csi_rs_neighbour_list;
  csi_rs_mtc_cfg_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CSI-RS-MTC-Configuration-List ::= SEQUENCE (SIZE (1..96)) OF CSI-RS-MTC-Configuration-Item
using csi_rs_mtc_cfg_list_l = dyn_array<csi_rs_mtc_cfg_item_s>;

// Additional-Measurement-Timing-Configuration-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using add_meas_timing_cfg_item_ext_ies_o = protocol_ext_empty_o;

using add_meas_timing_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// Additional-Measurement-Timing-Configuration-Item ::= SEQUENCE
struct add_meas_timing_cfg_item_s {
  bool                                       ie_exts_present         = false;
  uint8_t                                    add_meas_timing_cfg_idx = 0;
  csi_rs_mtc_cfg_list_l                      csi_rs_mtc_cfg_list;
  add_meas_timing_cfg_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Additional-Measurement-Timing-Configuration-List ::= SEQUENCE (SIZE (1..16)) OF
// Additional-Measurement-Timing-Configuration-Item
using add_meas_timing_cfg_list_l = dyn_array<add_meas_timing_cfg_item_s>;

// Additional-PDCP-Duplication-TNL-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using add_pdcp_dupl_tnl_ext_ies_o = protocol_ext_empty_o;

// QoS-Mapping-Information-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qos_map_info_ext_ies_o = protocol_ext_empty_o;

using qos_map_info_ext_ies_container = protocol_ext_container_empty_l;

// QoS-Mapping-Information ::= SEQUENCE
struct qos_map_info_s {
  bool                             dscp_present       = false;
  bool                             flow_label_present = false;
  bool                             ie_exts_present    = false;
  fixed_bitstring<6, false, true>  dscp;
  fixed_bitstring<20, false, true> flow_label;
  qos_map_info_ext_ies_container   ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GTPtunnelTransportLayerInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct gtp_tunnel_transport_layer_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct gtp_tunnel_transport_layer_info_ext_ies_container {
  bool           qos_map_info_present = false;
  qos_map_info_s qos_map_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GTPtunnelTransportLayerInformation ::= SEQUENCE
struct gtp_tunnel_transport_layer_info_s {
  bool                                              ie_exts_present = false;
  bounded_bitstring<1, 160, true, true>             tnl_address;
  fixed_octstring<4, true>                          gtp_teid;
  gtp_tunnel_transport_layer_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UPTransportLayerInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
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
  gtp_tunnel_transport_layer_info_s& gtp_tunnel()
  {
    assert_choice_type(types::gtp_tunnel, type_, "UPTransportLayerInformation");
    return c.get<gtp_tunnel_transport_layer_info_s>();
  }
  protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UPTransportLayerInformation");
    return c.get<protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>>();
  }
  const gtp_tunnel_transport_layer_info_s& gtp_tunnel() const
  {
    assert_choice_type(types::gtp_tunnel, type_, "UPTransportLayerInformation");
    return c.get<gtp_tunnel_transport_layer_info_s>();
  }
  const protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UPTransportLayerInformation");
    return c.get<protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>>();
  }
  gtp_tunnel_transport_layer_info_s&                                 set_gtp_tunnel();
  protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<gtp_tunnel_transport_layer_info_s, protocol_ie_single_container_s<up_transport_layer_info_ext_ies_o>>
      c;

  void destroy_();
};

using add_pdcp_dupl_tnl_ext_ies_container = protocol_ext_container_empty_l;

// Additional-PDCP-Duplication-TNL-Item ::= SEQUENCE
struct add_pdcp_dupl_tnl_item_s {
  bool                                ie_exts_present = false;
  up_transport_layer_info_c           add_pdcp_dupl_up_tnl_info;
  add_pdcp_dupl_tnl_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Additional-PDCP-Duplication-TNL-List ::= SEQUENCE (SIZE (1..2)) OF Additional-PDCP-Duplication-TNL-Item
using add_pdcp_dupl_tnl_list_l = dyn_array<add_pdcp_dupl_tnl_item_s>;

// Additional-UL-NG-U-TNLatUPF-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct add_ul_ng_u_tnl_at_up_f_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct add_ul_ng_u_tnl_at_up_f_item_ext_ies_container {
  bool                      pdu_session_common_network_instance_present = false;
  unbounded_octstring<true> pdu_session_common_network_instance;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Additional-UL-NG-U-TNLatUPF-Item ::= SEQUENCE
struct add_ul_ng_u_tnl_at_up_f_item_s {
  bool                                           ie_exts_present = false;
  up_transport_layer_info_c                      add_ul_ng_u_tnl_at_up_f;
  add_ul_ng_u_tnl_at_up_f_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Additional-UL-NG-U-TNLatUPF-List ::= SEQUENCE (SIZE (1..3)) OF Additional-UL-NG-U-TNLatUPF-Item
using add_ul_ng_u_tnl_at_up_f_list_l = dyn_array<add_ul_ng_u_tnl_at_up_f_item_s>;

// QoSFLowsAcceptedToBeForwarded-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qos_f_lows_accepted_to_be_forwarded_item_ext_ies_o = protocol_ext_empty_o;

using qos_f_lows_accepted_to_be_forwarded_item_ext_ies_container = protocol_ext_container_empty_l;

// QoSFLowsAcceptedToBeForwarded-Item ::= SEQUENCE
struct qos_f_lows_accepted_to_be_forwarded_item_s {
  bool                                                       ie_ext_present = false;
  uint8_t                                                    qos_flow_id    = 0;
  qos_f_lows_accepted_to_be_forwarded_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFLowsAcceptedToBeForwarded-List ::= SEQUENCE (SIZE (1..64)) OF QoSFLowsAcceptedToBeForwarded-Item
using qos_f_lows_accepted_to_be_forwarded_list_l = dyn_array<qos_f_lows_accepted_to_be_forwarded_item_s>;

// DataForwardingResponseDRBItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using data_forwarding_resp_drb_item_ext_ies_o = protocol_ext_empty_o;

using data_forwarding_resp_drb_item_ext_ies_container = protocol_ext_container_empty_l;

// DataForwardingResponseDRBItem ::= SEQUENCE
struct data_forwarding_resp_drb_item_s {
  bool                                            dl_forwarding_up_tnl_present = false;
  bool                                            ul_forwarding_up_tnl_present = false;
  bool                                            ie_ext_present               = false;
  uint8_t                                         drb_id                       = 1;
  up_transport_layer_info_c                       dl_forwarding_up_tnl;
  up_transport_layer_info_c                       ul_forwarding_up_tnl;
  data_forwarding_resp_drb_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataForwardingResponseDRBItemList ::= SEQUENCE (SIZE (1..32)) OF DataForwardingResponseDRBItem
using data_forwarding_resp_drb_item_list_l = dyn_array<data_forwarding_resp_drb_item_s>;

// DirectForwardingPathAvailability ::= ENUMERATED
struct direct_forwarding_path_availability_opts {
  enum options { direct_path_available, /*...*/ nulltype } value;

  const char* to_string() const;
};
using direct_forwarding_path_availability_e = enumerated<direct_forwarding_path_availability_opts, true>;

// DataForwardingInfoFromTargetNGRANnode-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct data_forwarding_info_from_target_ngran_node_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct data_forwarding_info_from_target_ngran_node_ext_ies_container {
  bool                                  direct_forwarding_path_availability_present = false;
  direct_forwarding_path_availability_e direct_forwarding_path_availability;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataForwardingInfoFromTargetNGRANnode ::= SEQUENCE
struct data_forwarding_info_from_target_ngran_node_s {
  bool                                       pdu_session_level_dl_data_forwarding_info_present = false;
  bool                                       pdu_session_level_ul_data_forwarding_info_present = false;
  bool                                       ie_ext_present                                    = false;
  qos_f_lows_accepted_to_be_forwarded_list_l qos_flows_accepted_for_data_forwarding_list;
  up_transport_layer_info_c                  pdu_session_level_dl_data_forwarding_info;
  up_transport_layer_info_c                  pdu_session_level_ul_data_forwarding_info;
  data_forwarding_resp_drb_item_list_l       data_forwarding_resp_drb_item_list;
  data_forwarding_info_from_target_ngran_node_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRB-List ::= SEQUENCE (SIZE (1..32)) OF INTEGER (1..32,...)
using drb_list_l = bounded_array<uint8_t, 32>;

// PDUSessionResourceChangeConfirmInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_change_confirm_info_sn_terminated_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_change_confirm_info_sn_terminated_ext_ies_container {
  bool       drb_ids_takenintouse_present = false;
  drb_list_l drb_ids_takenintouse;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceChangeConfirmInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_change_confirm_info_sn_terminated_s {
  bool                                                                dataforwardinginfo_target_present = false;
  bool                                                                ie_exts_present                   = false;
  data_forwarding_info_from_target_ngran_node_s                       dataforwardinginfo_target;
  pdu_session_res_change_confirm_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalListofPDUSessionResourceChangeConfirmInfo-SNterminated-Item-ExtIEs ::= OBJECT SET OF
// XNAP-PROTOCOL-EXTENSION
using add_listof_pdu_session_res_change_confirm_info_sn_terminated_item_ext_ies_o = protocol_ext_empty_o;

using add_listof_pdu_session_res_change_confirm_info_sn_terminated_item_ext_ies_container =
    protocol_ext_container_empty_l;

// AdditionalListofPDUSessionResourceChangeConfirmInfo-SNterminated-Item ::= SEQUENCE
struct add_listof_pdu_session_res_change_confirm_info_sn_terminated_item_s {
  bool                                                ie_exts_present = false;
  pdu_session_res_change_confirm_info_sn_terminated_s pdu_session_res_change_confirm_info_sn_terminated;
  add_listof_pdu_session_res_change_confirm_info_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AdditionalListofPDUSessionResourceChangeConfirmInfo-SNterminated ::= SEQUENCE (SIZE (1..7)) OF
// AdditionalListofPDUSessionResourceChangeConfirmInfo-SNterminated-Item
using add_listof_pdu_session_res_change_confirm_info_sn_terminated_l =
    dyn_array<add_listof_pdu_session_res_change_confirm_info_sn_terminated_item_s>;

// AerialControllerUE ::= ENUMERATED
struct aerial_ctrller_ue_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using aerial_ctrller_ue_e = enumerated<aerial_ctrller_ue_opts, true>;

// AerialUE ::= ENUMERATED
struct aerial_ue_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using aerial_ue_e = enumerated<aerial_ue_opts, true>;

// AerialUESubscriptionInformation ::= ENUMERATED
struct aerial_ue_sub_info_opts {
  enum options { allowed, not_allowed, /*...*/ nulltype } value;

  const char* to_string() const;
};
using aerial_ue_sub_info_e = enumerated<aerial_ue_sub_info_opts, true>;

// AllTrafficIndication ::= ENUMERATED
struct all_traffic_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using all_traffic_ind_e = enumerated<all_traffic_ind_opts, true>;

// AllowedCAG-ID-List-perPLMN ::= SEQUENCE (SIZE (1..256)) OF BIT STRING (SIZE (32))
using allowed_cag_id_list_per_plmn_l = dyn_array<fixed_bitstring<32, false, true>>;

// PNI-NPN-Restricted-Information ::= ENUMERATED
struct pni_npn_restricted_info_opts {
  enum options { restriced, not_restricted, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pni_npn_restricted_info_e = enumerated<pni_npn_restricted_info_opts, true>;

// AllowedPNI-NPN-ID-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using allowed_pni_npn_id_item_ext_ies_o = protocol_ext_empty_o;

using allowed_pni_npn_id_item_ext_ies_container = protocol_ext_container_empty_l;

// AllowedPNI-NPN-ID-Item ::= SEQUENCE
struct allowed_pni_npn_id_item_s {
  bool                                      ie_exts_present = false;
  fixed_octstring<3, true>                  plmn_id;
  pni_npn_restricted_info_e                 pni_npn_restricted_info;
  allowed_cag_id_list_per_plmn_l            allowed_cag_id_list_per_plmn;
  allowed_pni_npn_id_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AllowedPNI-NPN-ID-List ::= SEQUENCE (SIZE (1..16)) OF AllowedPNI-NPN-ID-Item
using allowed_pni_npn_id_list_l = dyn_array<allowed_pni_npn_id_item_s>;

// TAIsinAoI-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tai_sin_ao_i_item_ext_ies_o = protocol_ext_empty_o;

using tai_sin_ao_i_item_ext_ies_container = protocol_ext_container_empty_l;

// TAIsinAoI-Item ::= SEQUENCE
struct tai_sin_ao_i_item_s {
  bool                                ie_exts_present = false;
  fixed_octstring<3, true>            plmn_id;
  fixed_octstring<3, true>            tac;
  tai_sin_ao_i_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ListOfTAIsinAoI ::= SEQUENCE (SIZE (1..16)) OF TAIsinAoI-Item
using list_of_tai_sin_ao_i_l = dyn_array<tai_sin_ao_i_item_s>;

// NG-RAN-Cell-Identity-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ng_ran_cell_id_ext_ies_o = protocol_ies_empty_o;

// NG-RAN-Cell-Identity ::= CHOICE
struct ng_ran_cell_id_c {
  struct types_opts {
    enum options { nr, e_utra, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ng_ran_cell_id_c() = default;
  ng_ran_cell_id_c(const ng_ran_cell_id_c& other);
  ng_ran_cell_id_c& operator=(const ng_ran_cell_id_c& other);
  ~ng_ran_cell_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<36, false, true>& nr()
  {
    assert_choice_type(types::nr, type_, "NG-RAN-Cell-Identity");
    return c.get<fixed_bitstring<36, false, true>>();
  }
  fixed_bitstring<28, false, true>& e_utra()
  {
    assert_choice_type(types::e_utra, type_, "NG-RAN-Cell-Identity");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  protocol_ie_single_container_s<ng_ran_cell_id_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NG-RAN-Cell-Identity");
    return c.get<protocol_ie_single_container_s<ng_ran_cell_id_ext_ies_o>>();
  }
  const fixed_bitstring<36, false, true>& nr() const
  {
    assert_choice_type(types::nr, type_, "NG-RAN-Cell-Identity");
    return c.get<fixed_bitstring<36, false, true>>();
  }
  const fixed_bitstring<28, false, true>& e_utra() const
  {
    assert_choice_type(types::e_utra, type_, "NG-RAN-Cell-Identity");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  const protocol_ie_single_container_s<ng_ran_cell_id_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NG-RAN-Cell-Identity");
    return c.get<protocol_ie_single_container_s<ng_ran_cell_id_ext_ies_o>>();
  }
  fixed_bitstring<36, false, true>&                         set_nr();
  fixed_bitstring<28, false, true>&                         set_e_utra();
  protocol_ie_single_container_s<ng_ran_cell_id_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                       type_;
  choice_buffer_t<fixed_bitstring<36, false, true>, protocol_ie_single_container_s<ng_ran_cell_id_ext_ies_o>> c;

  void destroy_();
};

// CellsinAoI-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cellsin_ao_i_item_ext_ies_o = protocol_ext_empty_o;

using cellsin_ao_i_item_ext_ies_container = protocol_ext_container_empty_l;

// CellsinAoI-Item ::= SEQUENCE
struct cellsin_ao_i_item_s {
  bool                                ie_exts_present = false;
  fixed_octstring<3, true>            plmn_id;
  ng_ran_cell_id_c                    ng_ran_cell_id;
  cellsin_ao_i_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ListOfCells ::= SEQUENCE (SIZE (1..256)) OF CellsinAoI-Item
using list_of_cells_l = dyn_array<cellsin_ao_i_item_s>;

// GNB-ID-Choice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using gnb_id_choice_ext_ies_o = protocol_ies_empty_o;

// GNB-ID-Choice ::= CHOICE
struct gnb_id_choice_c {
  struct types_opts {
    enum options { gnb_id, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  gnb_id_choice_c() = default;
  gnb_id_choice_c(const gnb_id_choice_c& other);
  gnb_id_choice_c& operator=(const gnb_id_choice_c& other);
  ~gnb_id_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  bounded_bitstring<22, 32, false, true>& gnb_id()
  {
    assert_choice_type(types::gnb_id, type_, "GNB-ID-Choice");
    return c.get<bounded_bitstring<22, 32, false, true>>();
  }
  protocol_ie_single_container_s<gnb_id_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "GNB-ID-Choice");
    return c.get<protocol_ie_single_container_s<gnb_id_choice_ext_ies_o>>();
  }
  const bounded_bitstring<22, 32, false, true>& gnb_id() const
  {
    assert_choice_type(types::gnb_id, type_, "GNB-ID-Choice");
    return c.get<bounded_bitstring<22, 32, false, true>>();
  }
  const protocol_ie_single_container_s<gnb_id_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "GNB-ID-Choice");
    return c.get<protocol_ie_single_container_s<gnb_id_choice_ext_ies_o>>();
  }
  bounded_bitstring<22, 32, false, true>&                  set_gnb_id();
  protocol_ie_single_container_s<gnb_id_choice_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<bounded_bitstring<22, 32, false, true>, protocol_ie_single_container_s<gnb_id_choice_ext_ies_o>> c;

  void destroy_();
};

// GlobalgNB-ID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using global_gnb_id_ext_ies_o = protocol_ext_empty_o;

using global_gnb_id_ext_ies_container = protocol_ext_container_empty_l;

// GlobalgNB-ID ::= SEQUENCE
struct global_gnb_id_s {
  bool                            ie_exts_present = false;
  fixed_octstring<3, true>        plmn_id;
  gnb_id_choice_c                 gnb_id;
  global_gnb_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ENB-ID-Choice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using enb_id_choice_ext_ies_o = protocol_ies_empty_o;

// ENB-ID-Choice ::= CHOICE
struct enb_id_choice_c {
  struct types_opts {
    enum options { enb_id_macro, enb_id_shortmacro, enb_id_longmacro, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

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
  protocol_ie_single_container_s<enb_id_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ENB-ID-Choice");
    return c.get<protocol_ie_single_container_s<enb_id_choice_ext_ies_o>>();
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
  const protocol_ie_single_container_s<enb_id_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ENB-ID-Choice");
    return c.get<protocol_ie_single_container_s<enb_id_choice_ext_ies_o>>();
  }
  fixed_bitstring<20, false, true>&                        set_enb_id_macro();
  fixed_bitstring<18, false, true>&                        set_enb_id_shortmacro();
  fixed_bitstring<21, false, true>&                        set_enb_id_longmacro();
  protocol_ie_single_container_s<enb_id_choice_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                      type_;
  choice_buffer_t<fixed_bitstring<21, false, true>, protocol_ie_single_container_s<enb_id_choice_ext_ies_o>> c;

  void destroy_();
};

// GlobaleNB-ID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using global_enb_id_ext_ies_o = protocol_ext_empty_o;

using global_enb_id_ext_ies_container = protocol_ext_container_empty_l;

// GlobalngeNB-ID ::= SEQUENCE
struct globalng_enb_id_s {
  bool                            ie_exts_present = false;
  fixed_octstring<3, true>        plmn_id;
  enb_id_choice_c                 enb_id;
  global_enb_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GlobalNG-RANNode-ID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using global_ng_ran_node_id_ext_ies_o = protocol_ies_empty_o;

// GlobalNG-RANNode-ID ::= CHOICE
struct global_ng_ran_node_id_c {
  struct types_opts {
    enum options { gnb, ng_enb, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

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
  protocol_ie_single_container_s<global_ng_ran_node_id_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "GlobalNG-RANNode-ID");
    return c.get<protocol_ie_single_container_s<global_ng_ran_node_id_ext_ies_o>>();
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
  const protocol_ie_single_container_s<global_ng_ran_node_id_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "GlobalNG-RANNode-ID");
    return c.get<protocol_ie_single_container_s<global_ng_ran_node_id_ext_ies_o>>();
  }
  global_gnb_id_s&                                                 set_gnb();
  globalng_enb_id_s&                                               set_ng_enb();
  protocol_ie_single_container_s<global_ng_ran_node_id_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<global_gnb_id_s, globalng_enb_id_s, protocol_ie_single_container_s<global_ng_ran_node_id_ext_ies_o>>
      c;

  void destroy_();
};

// GlobalNG-RANNodesinAoI-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using global_ng_ran_nodesin_ao_i_item_ext_ies_o = protocol_ext_empty_o;

using global_ng_ran_nodesin_ao_i_item_ext_ies_container = protocol_ext_container_empty_l;

// GlobalNG-RANNodesinAoI-Item ::= SEQUENCE
struct global_ng_ran_nodesin_ao_i_item_s {
  bool                                              ie_exts_present = false;
  global_ng_ran_node_id_c                           global_ng_ran_node_id;
  global_ng_ran_nodesin_ao_i_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ListOfRANNodesinAoI ::= SEQUENCE (SIZE (1..64)) OF GlobalNG-RANNodesinAoI-Item
using list_of_ran_nodesin_ao_i_l = dyn_array<global_ng_ran_nodesin_ao_i_item_s>;

// AreaOfInterest-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using area_of_interest_item_ext_ies_o = protocol_ext_empty_o;

using area_of_interest_item_ext_ies_container = protocol_ext_container_empty_l;

// AreaOfInterest-Item ::= SEQUENCE
struct area_of_interest_item_s {
  bool                                    ie_exts_present = false;
  list_of_tai_sin_ao_i_l                  list_of_tai_sin_ao_i;
  list_of_cells_l                         list_of_cellsin_ao_i;
  list_of_ran_nodesin_ao_i_l              list_of_ran_nodesin_ao_i;
  uint16_t                                request_ref_id = 1;
  area_of_interest_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AreaOfInterestInformation ::= SEQUENCE (SIZE (1..64)) OF AreaOfInterest-Item
using area_of_interest_info_l = dyn_array<area_of_interest_item_s>;

// E-UTRA-CGI-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using e_utra_cgi_ext_ies_o = protocol_ext_empty_o;

using e_utra_cgi_ext_ies_container = protocol_ext_container_empty_l;

// E-UTRA-CGI ::= SEQUENCE
struct e_utra_cgi_s {
  bool                             ie_ext_present = false;
  fixed_octstring<3, true>         plmn_id;
  fixed_bitstring<28, false, true> e_utra_ci;
  e_utra_cgi_ext_ies_container     ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellIdListforMDT-EUTRA ::= SEQUENCE (SIZE (1..32)) OF E-UTRA-CGI
using cell_id_listfor_mdt_eutra_l = dyn_array<e_utra_cgi_s>;

// CellBasedMDT-EUTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_based_mdt_eutra_ext_ies_o = protocol_ext_empty_o;

using cell_based_mdt_eutra_ext_ies_container = protocol_ext_container_empty_l;

// CellBasedMDT-EUTRA ::= SEQUENCE
struct cell_based_mdt_eutra_s {
  bool                                   ie_exts_present = false;
  cell_id_listfor_mdt_eutra_l            cell_id_listfor_mdt_eutra;
  cell_based_mdt_eutra_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAListforMDT ::= SEQUENCE (SIZE (1..8)) OF OCTET STRING (SIZE (3))
using ta_listfor_mdt_l = bounded_array<fixed_octstring<3, true>, 8>;

// TABasedMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ta_based_mdt_ext_ies_o = protocol_ext_empty_o;

using ta_based_mdt_ext_ies_container = protocol_ext_container_empty_l;

// TABasedMDT ::= SEQUENCE
struct ta_based_mdt_s {
  bool                           ie_exts_present = false;
  ta_listfor_mdt_l               ta_listfor_mdt;
  ta_based_mdt_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAIforMDT-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tai_for_mdt_item_ext_ies_o = protocol_ext_empty_o;

using tai_for_mdt_item_ext_ies_container = protocol_ext_container_empty_l;

// TAIforMDT-Item ::= SEQUENCE
struct tai_for_mdt_item_s {
  bool                               ie_exts_present = false;
  fixed_octstring<3, true>           plmn_id;
  fixed_octstring<3, true>           tac;
  tai_for_mdt_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAIListforMDT ::= SEQUENCE (SIZE (1..8)) OF TAIforMDT-Item
using tai_listfor_mdt_l = dyn_array<tai_for_mdt_item_s>;

// TAIBasedMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tai_based_mdt_ext_ies_o = protocol_ext_empty_o;

using tai_based_mdt_ext_ies_container = protocol_ext_container_empty_l;

// TAIBasedMDT ::= SEQUENCE
struct tai_based_mdt_s {
  bool                            ie_exts_present = false;
  tai_listfor_mdt_l               tai_listfor_mdt;
  tai_based_mdt_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AreaScopeOfMDT-EUTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using area_scope_of_mdt_eutra_ext_ies_o = protocol_ies_empty_o;

// AreaScopeOfMDT-EUTRA ::= CHOICE
struct area_scope_of_mdt_eutra_c {
  struct types_opts {
    enum options { cell_based, ta_based, tai_based, /*...*/ choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true, 1>;

  // choice methods
  area_scope_of_mdt_eutra_c() = default;
  area_scope_of_mdt_eutra_c(const area_scope_of_mdt_eutra_c& other);
  area_scope_of_mdt_eutra_c& operator=(const area_scope_of_mdt_eutra_c& other);
  ~area_scope_of_mdt_eutra_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  cell_based_mdt_eutra_s& cell_based()
  {
    assert_choice_type(types::cell_based, type_, "AreaScopeOfMDT-EUTRA");
    return c.get<cell_based_mdt_eutra_s>();
  }
  ta_based_mdt_s& ta_based()
  {
    assert_choice_type(types::ta_based, type_, "AreaScopeOfMDT-EUTRA");
    return c.get<ta_based_mdt_s>();
  }
  tai_based_mdt_s& tai_based()
  {
    assert_choice_type(types::tai_based, type_, "AreaScopeOfMDT-EUTRA");
    return c.get<tai_based_mdt_s>();
  }
  protocol_ie_single_container_s<area_scope_of_mdt_eutra_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "AreaScopeOfMDT-EUTRA");
    return c.get<protocol_ie_single_container_s<area_scope_of_mdt_eutra_ext_ies_o>>();
  }
  const cell_based_mdt_eutra_s& cell_based() const
  {
    assert_choice_type(types::cell_based, type_, "AreaScopeOfMDT-EUTRA");
    return c.get<cell_based_mdt_eutra_s>();
  }
  const ta_based_mdt_s& ta_based() const
  {
    assert_choice_type(types::ta_based, type_, "AreaScopeOfMDT-EUTRA");
    return c.get<ta_based_mdt_s>();
  }
  const tai_based_mdt_s& tai_based() const
  {
    assert_choice_type(types::tai_based, type_, "AreaScopeOfMDT-EUTRA");
    return c.get<tai_based_mdt_s>();
  }
  const protocol_ie_single_container_s<area_scope_of_mdt_eutra_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "AreaScopeOfMDT-EUTRA");
    return c.get<protocol_ie_single_container_s<area_scope_of_mdt_eutra_ext_ies_o>>();
  }
  cell_based_mdt_eutra_s&                                            set_cell_based();
  ta_based_mdt_s&                                                    set_ta_based();
  tai_based_mdt_s&                                                   set_tai_based();
  protocol_ie_single_container_s<area_scope_of_mdt_eutra_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<cell_based_mdt_eutra_s,
                  protocol_ie_single_container_s<area_scope_of_mdt_eutra_ext_ies_o>,
                  ta_based_mdt_s,
                  tai_based_mdt_s>
      c;

  void destroy_();
};

// CellIdListforMDT-NR ::= SEQUENCE (SIZE (1..32)) OF NR-CGI
using cell_id_listfor_mdt_nr_l = dyn_array<nr_cgi_s>;

// CellBasedMDT-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_based_mdt_nr_ext_ies_o = protocol_ext_empty_o;

using cell_based_mdt_nr_ext_ies_container = protocol_ext_container_empty_l;

// CellBasedMDT-NR ::= SEQUENCE
struct cell_based_mdt_nr_s {
  bool                                ie_exts_present = false;
  cell_id_listfor_mdt_nr_l            cell_id_listfor_mdt_nr;
  cell_based_mdt_nr_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CAGListforMDTItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cag_listfor_mdt_item_ext_ies_o = protocol_ext_empty_o;

using cag_listfor_mdt_item_ext_ies_container = protocol_ext_container_empty_l;

// CAGListforMDTItem ::= SEQUENCE
struct cag_listfor_mdt_item_s {
  bool                                   ie_exts_present = false;
  fixed_octstring<3, true>               plmn_id;
  fixed_bitstring<32, false, true>       cag_id;
  cag_listfor_mdt_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CAGListforMDT ::= SEQUENCE (SIZE (1..256)) OF CAGListforMDTItem
using cag_listfor_mdt_l = dyn_array<cag_listfor_mdt_item_s>;

// PNI-NPNBasedMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pni_npn_based_mdt_ext_ies_o = protocol_ext_empty_o;

using pni_npn_based_mdt_ext_ies_container = protocol_ext_container_empty_l;

// PNI-NPNBasedMDT ::= SEQUENCE
struct pni_npn_based_mdt_s {
  bool                                ie_exts_present = false;
  cag_listfor_mdt_l                   cag_listfor_mdt;
  pni_npn_based_mdt_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNPN-CellIdforMDT-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_pn_cell_idfor_mdt_item_ext_ies_o = protocol_ext_empty_o;

using sn_pn_cell_idfor_mdt_item_ext_ies_container = protocol_ext_container_empty_l;

// SNPN-CellIdforMDT-Item ::= SEQUENCE
struct sn_pn_cell_idfor_mdt_item_s {
  bool                                        ie_exts_present = false;
  nr_cgi_s                                    nr_cgi;
  fixed_bitstring<44, false, true>            nid;
  sn_pn_cell_idfor_mdt_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNPN-CellIdListforMDT ::= SEQUENCE (SIZE (1..32)) OF SNPN-CellIdforMDT-Item
using sn_pn_cell_id_listfor_mdt_l = dyn_array<sn_pn_cell_idfor_mdt_item_s>;

// SNPN-CellBasedMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_pn_cell_based_mdt_ext_ies_o = protocol_ext_empty_o;

using sn_pn_cell_based_mdt_ext_ies_container = protocol_ext_container_empty_l;

// SNPN-CellBasedMDT ::= SEQUENCE
struct sn_pn_cell_based_mdt_s {
  bool                                   ie_exts_present = false;
  sn_pn_cell_id_listfor_mdt_l            sn_pn_cell_id_listfor_mdt;
  sn_pn_cell_based_mdt_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNPN-TAIforMDT-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_pn_tai_for_mdt_item_ext_ies_o = protocol_ext_empty_o;

using sn_pn_tai_for_mdt_item_ext_ies_container = protocol_ext_container_empty_l;

// SNPN-TAIforMDT-Item ::= SEQUENCE
struct sn_pn_tai_for_mdt_item_s {
  bool                                     ie_exts_present = false;
  fixed_octstring<3, true>                 plmn_id;
  fixed_octstring<3, true>                 tac;
  fixed_bitstring<44, false, true>         nid;
  sn_pn_tai_for_mdt_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNPN-TAIListforMDT ::= SEQUENCE (SIZE (1..8)) OF SNPN-TAIforMDT-Item
using sn_pn_tai_listfor_mdt_l = dyn_array<sn_pn_tai_for_mdt_item_s>;

// SNPN-TAIBasedMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_pn_tai_based_mdt_ext_ies_o = protocol_ext_empty_o;

using sn_pn_tai_based_mdt_ext_ies_container = protocol_ext_container_empty_l;

// SNPN-TAIBasedMDT ::= SEQUENCE
struct sn_pn_tai_based_mdt_s {
  bool                                  ie_exts_present = false;
  sn_pn_tai_listfor_mdt_l               sn_pn_tai_listfor_mdt;
  sn_pn_tai_based_mdt_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNPNforMDT-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_pn_for_mdt_item_ext_ies_o = protocol_ext_empty_o;

using sn_pn_for_mdt_item_ext_ies_container = protocol_ext_container_empty_l;

// SNPNforMDT-Item ::= SEQUENCE
struct sn_pn_for_mdt_item_s {
  bool                                 ie_exts_present = false;
  fixed_octstring<3, true>             plmn_id;
  fixed_bitstring<44, false, true>     nid;
  sn_pn_for_mdt_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNPNListforMDT ::= SEQUENCE (SIZE (1..16)) OF SNPNforMDT-Item
using sn_pn_listfor_mdt_l = dyn_array<sn_pn_for_mdt_item_s>;

// SNPN-BasedMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_pn_based_mdt_ext_ies_o = protocol_ext_empty_o;

using sn_pn_based_mdt_ext_ies_container = protocol_ext_container_empty_l;

// SNPN-BasedMDT ::= SEQUENCE
struct sn_pn_based_mdt_s {
  bool                              ie_exts_present = false;
  sn_pn_listfor_mdt_l               sn_pn_listfor_mdt;
  sn_pn_based_mdt_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AreaScopeOfMDT-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct area_scope_of_mdt_nr_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { pni_npn_based_mdt, sn_pn_cell_based_mdt, sn_pn_tai_based_mdt, sn_pn_based_mdt, nulltype } value;

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
    pni_npn_based_mdt_s&          pni_npn_based_mdt();
    sn_pn_cell_based_mdt_s&       sn_pn_cell_based_mdt();
    sn_pn_tai_based_mdt_s&        sn_pn_tai_based_mdt();
    sn_pn_based_mdt_s&            sn_pn_based_mdt();
    const pni_npn_based_mdt_s&    pni_npn_based_mdt() const;
    const sn_pn_cell_based_mdt_s& sn_pn_cell_based_mdt() const;
    const sn_pn_tai_based_mdt_s&  sn_pn_tai_based_mdt() const;
    const sn_pn_based_mdt_s&      sn_pn_based_mdt() const;

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

// AreaScopeOfMDT-NR ::= CHOICE
struct area_scope_of_mdt_nr_c {
  struct types_opts {
    enum options { cell_based, ta_based, tai_based, /*...*/ choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true, 1>;

  // choice methods
  area_scope_of_mdt_nr_c() = default;
  area_scope_of_mdt_nr_c(const area_scope_of_mdt_nr_c& other);
  area_scope_of_mdt_nr_c& operator=(const area_scope_of_mdt_nr_c& other);
  ~area_scope_of_mdt_nr_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  cell_based_mdt_nr_s& cell_based()
  {
    assert_choice_type(types::cell_based, type_, "AreaScopeOfMDT-NR");
    return c.get<cell_based_mdt_nr_s>();
  }
  ta_based_mdt_s& ta_based()
  {
    assert_choice_type(types::ta_based, type_, "AreaScopeOfMDT-NR");
    return c.get<ta_based_mdt_s>();
  }
  tai_based_mdt_s& tai_based()
  {
    assert_choice_type(types::tai_based, type_, "AreaScopeOfMDT-NR");
    return c.get<tai_based_mdt_s>();
  }
  protocol_ie_single_container_s<area_scope_of_mdt_nr_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "AreaScopeOfMDT-NR");
    return c.get<protocol_ie_single_container_s<area_scope_of_mdt_nr_ext_ies_o>>();
  }
  const cell_based_mdt_nr_s& cell_based() const
  {
    assert_choice_type(types::cell_based, type_, "AreaScopeOfMDT-NR");
    return c.get<cell_based_mdt_nr_s>();
  }
  const ta_based_mdt_s& ta_based() const
  {
    assert_choice_type(types::ta_based, type_, "AreaScopeOfMDT-NR");
    return c.get<ta_based_mdt_s>();
  }
  const tai_based_mdt_s& tai_based() const
  {
    assert_choice_type(types::tai_based, type_, "AreaScopeOfMDT-NR");
    return c.get<tai_based_mdt_s>();
  }
  const protocol_ie_single_container_s<area_scope_of_mdt_nr_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "AreaScopeOfMDT-NR");
    return c.get<protocol_ie_single_container_s<area_scope_of_mdt_nr_ext_ies_o>>();
  }
  cell_based_mdt_nr_s&                                            set_cell_based();
  ta_based_mdt_s&                                                 set_ta_based();
  tai_based_mdt_s&                                                set_tai_based();
  protocol_ie_single_container_s<area_scope_of_mdt_nr_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<cell_based_mdt_nr_s,
                  protocol_ie_single_container_s<area_scope_of_mdt_nr_ext_ies_o>,
                  ta_based_mdt_s,
                  tai_based_mdt_s>
      c;

  void destroy_();
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

// NRTransmissionBandwidth-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_tx_bw_ext_ies_o = protocol_ext_empty_o;

using nr_tx_bw_ext_ies_container = protocol_ext_container_empty_l;

// NRTransmissionBandwidth ::= SEQUENCE
struct nr_tx_bw_s {
  bool                       ie_exts_present = false;
  nr_scs_e                   nr_scs;
  nr_nrb_e                   nr_nrb;
  nr_tx_bw_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRCarrierItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// SUL-Information-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
  uint32_t                   sul_freq_info   = 0;
  nr_tx_bw_s                 sul_tx_bw;
  sul_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SupportedSULBandItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using supported_sul_band_item_ext_ies_o = protocol_ext_empty_o;

using supported_sul_band_item_ext_ies_container = protocol_ext_container_empty_l;

// SupportedSULBandItem ::= SEQUENCE
struct supported_sul_band_item_s {
  bool                                      ie_exts_present = false;
  uint16_t                                  sul_band_item   = 1;
  supported_sul_band_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SupportedSULBandList ::= SEQUENCE (SIZE (1..32)) OF SupportedSULBandItem
using supported_sul_band_list_l = dyn_array<supported_sul_band_item_s>;

// NRFrequencyBandItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_freq_band_item_ext_ies_o = protocol_ext_empty_o;

using nr_freq_band_item_ext_ies_container = protocol_ext_container_empty_l;

// NRFrequencyBandItem ::= SEQUENCE
struct nr_freq_band_item_s {
  bool                                ie_ext_present = false;
  uint16_t                            nr_freq_band   = 1;
  supported_sul_band_list_l           supported_sul_band_list;
  nr_freq_band_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRFrequencyBand-List ::= SEQUENCE (SIZE (1..32)) OF NRFrequencyBandItem
using nr_freq_band_list_l = dyn_array<nr_freq_band_item_s>;

// NRFrequencyInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// NRFrequencyInfo ::= SEQUENCE
struct nr_freq_info_s {
  bool                           sul_info_present = false;
  bool                           ie_ext_present   = false;
  uint32_t                       nr_arfcn         = 0;
  sul_info_s                     sul_info;
  nr_freq_band_list_l            freq_band_list;
  nr_freq_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PCIListForMDT ::= SEQUENCE (SIZE (1..32)) OF INTEGER (0..1007,...)
using pci_list_for_mdt_l = bounded_array<uint16_t, 32>;

// AreaScopeOfNeighCellsItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using area_scope_of_neigh_cells_item_ext_ies_o = protocol_ext_empty_o;

using area_scope_of_neigh_cells_item_ext_ies_container = protocol_ext_container_empty_l;

// AreaScopeOfNeighCellsItem ::= SEQUENCE
struct area_scope_of_neigh_cells_item_s {
  bool                                             ie_exts_present = false;
  nr_freq_info_s                                   nr_freq_info;
  pci_list_for_mdt_l                               pci_list_for_mdt;
  area_scope_of_neigh_cells_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AreaScopeOfNeighCellsList ::= SEQUENCE (SIZE (1..8)) OF AreaScopeOfNeighCellsItem
using area_scope_of_neigh_cells_list_l = dyn_array<area_scope_of_neigh_cells_item_s>;

// GlobalNG-RANCell-ID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using global_ng_ran_cell_id_ext_ies_o = protocol_ext_empty_o;

using global_ng_ran_cell_id_ext_ies_container = protocol_ext_container_empty_l;

// GlobalNG-RANCell-ID ::= SEQUENCE
struct global_ng_ran_cell_id_s {
  bool                                    ie_exts_present = false;
  fixed_octstring<3, true>                plmn_id;
  ng_ran_cell_id_c                        ng_ran_cell_id;
  global_ng_ran_cell_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellIdListforQMC ::= SEQUENCE (SIZE (1..32)) OF GlobalNG-RANCell-ID
using cell_id_listfor_qmc_l = dyn_array<global_ng_ran_cell_id_s>;

// CellBasedQMC-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_based_qmc_ext_ies_o = protocol_ext_empty_o;

using cell_based_qmc_ext_ies_container = protocol_ext_container_empty_l;

// CellBasedQMC ::= SEQUENCE
struct cell_based_qmc_s {
  bool                             ie_exts_present = false;
  cell_id_listfor_qmc_l            cell_id_listfor_qmc;
  cell_based_qmc_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAListforQMC ::= SEQUENCE (SIZE (1..8)) OF OCTET STRING (SIZE (3))
using ta_listfor_qmc_l = bounded_array<fixed_octstring<3, true>, 8>;

// TABasedQMC-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ta_based_qmc_ext_ies_o = protocol_ext_empty_o;

using ta_based_qmc_ext_ies_container = protocol_ext_container_empty_l;

// TABasedQMC ::= SEQUENCE
struct ta_based_qmc_s {
  bool                           ie_exts_present = false;
  ta_listfor_qmc_l               ta_listfor_qmc;
  ta_based_qmc_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAI-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tai_item_ext_ies_o = protocol_ext_empty_o;

using tai_item_ext_ies_container = protocol_ext_container_empty_l;

// TAI-Item ::= SEQUENCE
struct tai_item_s {
  bool                       ie_exts_present = false;
  fixed_octstring<3, true>   tac;
  fixed_octstring<3, true>   plmn_id;
  tai_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAIListforQMC ::= SEQUENCE (SIZE (1..8)) OF TAI-Item
using tai_listfor_qmc_l = dyn_array<tai_item_s>;

// TAIBasedQMC-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tai_based_qmc_ext_ies_o = protocol_ext_empty_o;

using tai_based_qmc_ext_ies_container = protocol_ext_container_empty_l;

// TAIBasedQMC ::= SEQUENCE
struct tai_based_qmc_s {
  bool                            ie_exts_present = false;
  tai_listfor_qmc_l               tai_listfor_qmc;
  tai_based_qmc_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PLMNListforQMC ::= SEQUENCE (SIZE (1..16)) OF OCTET STRING (SIZE (3))
using plmn_listfor_qmc_l = bounded_array<fixed_octstring<3, true>, 16>;

// PLMNAreaBasedQMC-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using plmn_area_based_qmc_ext_ies_o = protocol_ext_empty_o;

using plmn_area_based_qmc_ext_ies_container = protocol_ext_container_empty_l;

// PLMNAreaBasedQMC ::= SEQUENCE
struct plmn_area_based_qmc_s {
  bool                                  ie_exts_present = false;
  plmn_listfor_qmc_l                    plmn_listfor_qmc;
  plmn_area_based_qmc_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AreaScopeOfQMC-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using area_scope_of_qmc_ext_ies_o = protocol_ies_empty_o;

// AreaScopeOfQMC ::= CHOICE
struct area_scope_of_qmc_c {
  struct types_opts {
    enum options { cell_based, ta_based, tai_based, plmn_area_based, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  area_scope_of_qmc_c() = default;
  area_scope_of_qmc_c(const area_scope_of_qmc_c& other);
  area_scope_of_qmc_c& operator=(const area_scope_of_qmc_c& other);
  ~area_scope_of_qmc_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  cell_based_qmc_s& cell_based()
  {
    assert_choice_type(types::cell_based, type_, "AreaScopeOfQMC");
    return c.get<cell_based_qmc_s>();
  }
  ta_based_qmc_s& ta_based()
  {
    assert_choice_type(types::ta_based, type_, "AreaScopeOfQMC");
    return c.get<ta_based_qmc_s>();
  }
  tai_based_qmc_s& tai_based()
  {
    assert_choice_type(types::tai_based, type_, "AreaScopeOfQMC");
    return c.get<tai_based_qmc_s>();
  }
  plmn_area_based_qmc_s& plmn_area_based()
  {
    assert_choice_type(types::plmn_area_based, type_, "AreaScopeOfQMC");
    return c.get<plmn_area_based_qmc_s>();
  }
  protocol_ie_single_container_s<area_scope_of_qmc_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "AreaScopeOfQMC");
    return c.get<protocol_ie_single_container_s<area_scope_of_qmc_ext_ies_o>>();
  }
  const cell_based_qmc_s& cell_based() const
  {
    assert_choice_type(types::cell_based, type_, "AreaScopeOfQMC");
    return c.get<cell_based_qmc_s>();
  }
  const ta_based_qmc_s& ta_based() const
  {
    assert_choice_type(types::ta_based, type_, "AreaScopeOfQMC");
    return c.get<ta_based_qmc_s>();
  }
  const tai_based_qmc_s& tai_based() const
  {
    assert_choice_type(types::tai_based, type_, "AreaScopeOfQMC");
    return c.get<tai_based_qmc_s>();
  }
  const plmn_area_based_qmc_s& plmn_area_based() const
  {
    assert_choice_type(types::plmn_area_based, type_, "AreaScopeOfQMC");
    return c.get<plmn_area_based_qmc_s>();
  }
  const protocol_ie_single_container_s<area_scope_of_qmc_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "AreaScopeOfQMC");
    return c.get<protocol_ie_single_container_s<area_scope_of_qmc_ext_ies_o>>();
  }
  cell_based_qmc_s&                                            set_cell_based();
  ta_based_qmc_s&                                              set_ta_based();
  tai_based_qmc_s&                                             set_tai_based();
  plmn_area_based_qmc_s&                                       set_plmn_area_based();
  protocol_ie_single_container_s<area_scope_of_qmc_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<cell_based_qmc_s,
                  plmn_area_based_qmc_s,
                  protocol_ie_single_container_s<area_scope_of_qmc_ext_ies_o>,
                  ta_based_qmc_s,
                  tai_based_qmc_s>
      c;

  void destroy_();
};

// RANPagingAttemptInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ran_paging_attempt_info_ext_ies_o = protocol_ext_empty_o;

using ran_paging_attempt_info_ext_ies_container = protocol_ext_container_empty_l;

// RANPagingAttemptInfo ::= SEQUENCE
struct ran_paging_attempt_info_s {
  struct next_paging_area_scope_opts {
    enum options { same, changed, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using next_paging_area_scope_e_ = enumerated<next_paging_area_scope_opts, true>;

  // member variables
  bool                                      next_paging_area_scope_present = false;
  bool                                      ie_exts_present                = false;
  uint8_t                                   paging_attempt_count           = 1;
  uint8_t                                   intended_nof_paging_attempts   = 1;
  next_paging_area_scope_e_                 next_paging_area_scope;
  ran_paging_attempt_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPNPagingAssistanceInformation-PNI-NPN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using npn_paging_assist_info_pni_npn_ext_ies_o = protocol_ext_empty_o;

using npn_paging_assist_info_pni_npn_ext_ies_container = protocol_ext_container_empty_l;

// NPNPagingAssistanceInformation-PNI-NPN ::= SEQUENCE
struct npn_paging_assist_info_pni_npn_s {
  bool                                             ie_ext_present = false;
  allowed_pni_npn_id_list_l                        allowed_pni_npn_id_list;
  npn_paging_assist_info_pni_npn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPNPagingAssistanceInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using npn_paging_assist_info_ext_ies_o = protocol_ies_empty_o;

// NPNPagingAssistanceInformation ::= CHOICE
struct npn_paging_assist_info_c {
  struct types_opts {
    enum options { pni_npn_info, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  npn_paging_assist_info_c() = default;
  npn_paging_assist_info_c(const npn_paging_assist_info_c& other);
  npn_paging_assist_info_c& operator=(const npn_paging_assist_info_c& other);
  ~npn_paging_assist_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  npn_paging_assist_info_pni_npn_s& pni_npn_info()
  {
    assert_choice_type(types::pni_npn_info, type_, "NPNPagingAssistanceInformation");
    return c.get<npn_paging_assist_info_pni_npn_s>();
  }
  protocol_ie_single_container_s<npn_paging_assist_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NPNPagingAssistanceInformation");
    return c.get<protocol_ie_single_container_s<npn_paging_assist_info_ext_ies_o>>();
  }
  const npn_paging_assist_info_pni_npn_s& pni_npn_info() const
  {
    assert_choice_type(types::pni_npn_info, type_, "NPNPagingAssistanceInformation");
    return c.get<npn_paging_assist_info_pni_npn_s>();
  }
  const protocol_ie_single_container_s<npn_paging_assist_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NPNPagingAssistanceInformation");
    return c.get<protocol_ie_single_container_s<npn_paging_assist_info_ext_ies_o>>();
  }
  npn_paging_assist_info_pni_npn_s&                                 set_pni_npn_info();
  protocol_ie_single_container_s<npn_paging_assist_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<npn_paging_assist_info_pni_npn_s, protocol_ie_single_container_s<npn_paging_assist_info_ext_ies_o>> c;

  void destroy_();
};

// AssistanceDataForRANPaging-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct assist_data_for_ran_paging_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct assist_data_for_ran_paging_ext_ies_container {
  bool                     npn_paging_assist_info_present = false;
  npn_paging_assist_info_c npn_paging_assist_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AssistanceDataForRANPaging ::= SEQUENCE
struct assist_data_for_ran_paging_s {
  bool                                         ran_paging_attempt_info_present = false;
  bool                                         ie_exts_present                 = false;
  ran_paging_attempt_info_s                    ran_paging_attempt_info;
  assist_data_for_ran_paging_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Associated-QoSFlowInfo-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using associated_qos_flow_info_item_ext_ies_o = protocol_ext_empty_o;

using associated_qos_flow_info_item_ext_ies_container = protocol_ext_container_empty_l;

// Associated-QoSFlowInfo-Item ::= SEQUENCE
struct associated_qos_flow_info_item_s {
  bool                                            ie_exts_present                = false;
  uint8_t                                         mbs_qos_flow_id                = 0;
  uint8_t                                         associated_unicast_qos_flow_id = 0;
  associated_qos_flow_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Associated-QoSFlowInfo-List ::= SEQUENCE (SIZE (1..64)) OF Associated-QoSFlowInfo-Item
using associated_qos_flow_info_list_l = dyn_array<associated_qos_flow_info_item_s>;

// AvailableNRCellList ::= SEQUENCE (SIZE (1..16384)) OF NR-CGI
using available_nr_cell_list_l = dyn_array<nr_cgi_s>;

// AvailableCellList-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using available_cell_list_ext_ies_o = protocol_ext_empty_o;

using available_cell_list_ext_ies_container = protocol_ext_container_empty_l;

// AvailableCellList ::= SEQUENCE
struct available_cell_list_s {
  bool                                  ie_exts_present = false;
  available_nr_cell_list_l              available_nr_cell_list;
  available_cell_list_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AvailableRVQoEMetrics-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using available_rv_qo_e_metrics_ext_ies_o = protocol_ext_empty_o;

using available_rv_qo_e_metrics_ext_ies_container = protocol_ext_container_empty_l;

// AvailableRVQoEMetrics ::= SEQUENCE
struct available_rv_qo_e_metrics_s {
  struct application_layer_buffer_level_list_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using application_layer_buffer_level_list_e_ = enumerated<application_layer_buffer_level_list_opts, true>;
  struct playout_delay_for_media_startup_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using playout_delay_for_media_startup_e_ = enumerated<playout_delay_for_media_startup_opts, true>;

  // member variables
  bool                                        application_layer_buffer_level_list_present = false;
  bool                                        playout_delay_for_media_startup_present     = false;
  bool                                        ie_exts_present                             = false;
  application_layer_buffer_level_list_e_      application_layer_buffer_level_list;
  playout_delay_for_media_startup_e_          playout_delay_for_media_startup;
  available_rv_qo_e_metrics_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AveragePacketDelay-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using average_packet_delay_ext_ies_o = protocol_ext_empty_o;

using average_packet_delay_ext_ies_container = protocol_ext_container_empty_l;

// AveragePacketDelay ::= SEQUENCE
struct average_packet_delay_s {
  bool                                   ie_exts_present         = false;
  uint16_t                               ul_average_packet_delay = 0;
  uint16_t                               dl_average_packet_delay = 0;
  average_packet_delay_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BAPControlPDURLCCH-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using bap_ctrl_pdu_rlc_ch_item_ext_ies_o = protocol_ext_empty_o;

using bap_ctrl_pdu_rlc_ch_item_ext_ies_container = protocol_ext_container_empty_l;

// BAPControlPDURLCCH-Item ::= SEQUENCE
struct bap_ctrl_pdu_rlc_ch_item_s {
  bool                                       ie_exts_present = false;
  fixed_bitstring<16, false, true>           bh_rlc_ch_id;
  fixed_bitstring<10, false, true>           nexthop_bap_address;
  bap_ctrl_pdu_rlc_ch_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BAPControlPDURLCCH-List ::= SEQUENCE (SIZE (1..2)) OF BAPControlPDURLCCH-Item
using bap_ctrl_pdu_rlc_ch_list_l = dyn_array<bap_ctrl_pdu_rlc_ch_item_s>;

// BAPRoutingID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using bap_routing_id_ext_ies_o = protocol_ext_empty_o;

using bap_routing_id_ext_ies_container = protocol_ext_container_empty_l;

// BAPRoutingID ::= SEQUENCE
struct bap_routing_id_s {
  bool                             ie_exts_present = false;
  fixed_bitstring<10, false, true> bap_address;
  fixed_bitstring<10, false, true> bap_path_id;
  bap_routing_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHInfo-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using bh_info_item_ext_ies_o = protocol_ext_empty_o;

using bh_info_item_ext_ies_container = protocol_ext_container_empty_l;

// BHInfo-Item ::= SEQUENCE
struct bh_info_item_s {
  bool                           ie_exts_present = false;
  uint16_t                       bh_info_idx     = 1;
  bh_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BHInfoList ::= SEQUENCE (SIZE (1..1024)) OF BHInfo-Item
using bh_info_list_l = dyn_array<bh_info_item_s>;

// BroadcastEUTRAPLMNs ::= SEQUENCE (SIZE (1..6)) OF OCTET STRING (SIZE (3))
using broadcast_eutra_plmns_l = bounded_array<fixed_octstring<3, true>, 6>;

// BPLMN-ID-Info-EUTRA-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using bplmn_id_info_eutra_item_ext_ies_o = protocol_ext_empty_o;

using bplmn_id_info_eutra_item_ext_ies_container = protocol_ext_container_empty_l;

// BPLMN-ID-Info-EUTRA-Item ::= SEQUENCE
struct bplmn_id_info_eutra_item_s {
  bool                                       ranac_present  = false;
  bool                                       ie_ext_present = false;
  broadcast_eutra_plmns_l                    broadcast_plmns;
  fixed_octstring<3, true>                   tac;
  fixed_bitstring<28, false, true>           e_utra_ci;
  uint16_t                                   ranac = 0;
  bplmn_id_info_eutra_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BPLMN-ID-Info-EUTRA ::= SEQUENCE (SIZE (1..6)) OF BPLMN-ID-Info-EUTRA-Item
using bplmn_id_info_eutra_l = dyn_array<bplmn_id_info_eutra_item_s>;

// BroadcastPLMNs ::= SEQUENCE (SIZE (1..12)) OF OCTET STRING (SIZE (3))
using broadcast_plmns_l = bounded_array<fixed_octstring<3, true>, 12>;

// ConfiguredTACIndication ::= ENUMERATED
struct cfg_tac_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cfg_tac_ind_e = enumerated<cfg_tac_ind_opts, true>;

// BroadcastNID-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using broadcast_n_id_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_n_id_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastNID-Item ::= SEQUENCE
struct broadcast_n_id_item_s {
  bool                                  ie_ext_present = false;
  fixed_bitstring<44, false, true>      nid;
  broadcast_n_id_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastNID-List ::= SEQUENCE (SIZE (1..12)) OF BroadcastNID-Item
using broadcast_n_id_list_l = dyn_array<broadcast_n_id_item_s>;

// BroadcastSNPNID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using broadcast_sn_pn_id_ext_ies_o = protocol_ext_empty_o;

using broadcast_sn_pn_id_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastSNPNID ::= SEQUENCE
struct broadcast_sn_pn_id_s {
  bool                                 ie_ext_present = false;
  fixed_octstring<3, true>             plmn_id;
  broadcast_n_id_list_l                broadcast_n_id_list;
  broadcast_sn_pn_id_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastSNPNID-List ::= SEQUENCE (SIZE (1..12)) OF BroadcastSNPNID
using broadcast_sn_pn_id_list_l = dyn_array<broadcast_sn_pn_id_s>;

// NPN-Broadcast-Information-SNPN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// BroadcastCAG-Identifier-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using broadcast_cag_id_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_cag_id_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastCAG-Identifier-Item ::= SEQUENCE
struct broadcast_cag_id_item_s {
  bool                                    ie_ext_present = false;
  fixed_bitstring<32, false, true>        cag_id;
  broadcast_cag_id_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastCAG-Identifier-List ::= SEQUENCE (SIZE (1..12)) OF BroadcastCAG-Identifier-Item
using broadcast_cag_id_list_l = dyn_array<broadcast_cag_id_item_s>;

// BroadcastPNI-NPN-ID-Information-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using broadcast_pni_npn_id_info_item_ext_ies_o = protocol_ext_empty_o;

using broadcast_pni_npn_id_info_item_ext_ies_container = protocol_ext_container_empty_l;

// BroadcastPNI-NPN-ID-Information-Item ::= SEQUENCE
struct broadcast_pni_npn_id_info_item_s {
  bool                                             ie_ext_present = false;
  fixed_octstring<3, true>                         plmn_id;
  broadcast_cag_id_list_l                          broadcast_cag_id_list;
  broadcast_pni_npn_id_info_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastPNI-NPN-ID-Information ::= SEQUENCE (SIZE (1..12)) OF BroadcastPNI-NPN-ID-Information-Item
using broadcast_pni_npn_id_info_l = dyn_array<broadcast_pni_npn_id_info_item_s>;

// NPN-Broadcast-Information-PNI-NPN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using npn_broadcast_info_pni_npn_ext_ies_o = protocol_ext_empty_o;

using npn_broadcast_info_pni_npn_ext_ies_container = protocol_ext_container_empty_l;

// NPN-Broadcast-Information-PNI-NPN ::= SEQUENCE
struct npn_broadcast_info_pni_npn_s {
  bool                                         ie_ext_present = false;
  broadcast_pni_npn_id_info_l                  broadcast_pni_npn_id_info;
  npn_broadcast_info_pni_npn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPN-Broadcast-Information-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using npn_broadcast_info_ext_ies_o = protocol_ies_empty_o;

// NPN-Broadcast-Information ::= CHOICE
struct npn_broadcast_info_c {
  struct types_opts {
    enum options { snpn_info, pni_npn_info, choice_ext, nulltype } value;

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
  npn_broadcast_info_sn_pn_s& snpn_info()
  {
    assert_choice_type(types::snpn_info, type_, "NPN-Broadcast-Information");
    return c.get<npn_broadcast_info_sn_pn_s>();
  }
  npn_broadcast_info_pni_npn_s& pni_npn_info()
  {
    assert_choice_type(types::pni_npn_info, type_, "NPN-Broadcast-Information");
    return c.get<npn_broadcast_info_pni_npn_s>();
  }
  protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NPN-Broadcast-Information");
    return c.get<protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>>();
  }
  const npn_broadcast_info_sn_pn_s& snpn_info() const
  {
    assert_choice_type(types::snpn_info, type_, "NPN-Broadcast-Information");
    return c.get<npn_broadcast_info_sn_pn_s>();
  }
  const npn_broadcast_info_pni_npn_s& pni_npn_info() const
  {
    assert_choice_type(types::pni_npn_info, type_, "NPN-Broadcast-Information");
    return c.get<npn_broadcast_info_pni_npn_s>();
  }
  const protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NPN-Broadcast-Information");
    return c.get<protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>>();
  }
  npn_broadcast_info_sn_pn_s&                                   set_snpn_info();
  npn_broadcast_info_pni_npn_s&                                 set_pni_npn_info();
  protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<npn_broadcast_info_pni_npn_s,
                  npn_broadcast_info_sn_pn_s,
                  protocol_ie_single_container_s<npn_broadcast_info_ext_ies_o>>
      c;

  void destroy_();
};

// BPLMN-ID-Info-NR-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct bplmn_id_info_nr_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct bplmn_id_info_nr_item_ext_ies_container {
  bool                 cfg_tac_ind_present        = false;
  bool                 npn_broadcast_info_present = false;
  cfg_tac_ind_e        cfg_tac_ind;
  npn_broadcast_info_c npn_broadcast_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BPLMN-ID-Info-NR-Item ::= SEQUENCE
struct bplmn_id_info_nr_item_s {
  bool                                    ranac_present  = false;
  bool                                    ie_ext_present = false;
  broadcast_plmns_l                       broadcast_plmns;
  fixed_octstring<3, true>                tac;
  fixed_bitstring<36, false, true>        nr_ci;
  uint16_t                                ranac = 0;
  bplmn_id_info_nr_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BPLMN-ID-Info-NR ::= SEQUENCE (SIZE (1..12)) OF BPLMN-ID-Info-NR-Item
using bplmn_id_info_nr_l = dyn_array<bplmn_id_info_nr_item_s>;

// Bandwidth ::= ENUMERATED
struct bw_opts {
  enum options { mhz10, mhz20, mhz40, mhz60, mhz80, /*...*/ mhz100, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using bw_e = enumerated<bw_opts, true, 1>;

// BarringExemptionforEmerCallInfo ::= ENUMERATED
struct barr_exemptionfor_emer_call_info_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using barr_exemptionfor_emer_call_info_e = enumerated<barr_exemptionfor_emer_call_info_opts, true>;

// BeamMeasurementIndicationM1 ::= ENUMERATED
struct beam_meas_ind_m1_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using beam_meas_ind_m1_e = enumerated<beam_meas_ind_m1_opts, true>;

// BeamMeasurementsReportQuantity-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using beam_meass_report_quant_ext_ies_o = protocol_ext_empty_o;

using beam_meass_report_quant_ext_ies_container = protocol_ext_container_empty_l;

// BeamMeasurementsReportQuantity ::= SEQUENCE
struct beam_meass_report_quant_s {
  struct rsrp_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using rsrp_e_ = enumerated<rsrp_opts, true>;
  struct rsrq_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using rsrq_e_ = enumerated<rsrq_opts, true>;
  struct sinr_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using sinr_e_ = enumerated<sinr_opts, true>;

  // member variables
  bool                                      ie_exts_present = false;
  rsrp_e_                                   rsrp;
  rsrq_e_                                   rsrq;
  sinr_e_                                   sinr;
  beam_meass_report_quant_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BeamMeasurementsReportConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using beam_meass_report_cfg_ext_ies_o = protocol_ext_empty_o;

using beam_meass_report_cfg_ext_ies_container = protocol_ext_container_empty_l;

// BeamMeasurementsReportConfiguration ::= SEQUENCE
struct beam_meass_report_cfg_s {
  bool                                    beam_meass_report_quant_present     = false;
  bool                                    max_nrof_rs_idxes_to_report_present = false;
  bool                                    ie_exts_present                     = false;
  beam_meass_report_quant_s               beam_meass_report_quant;
  uint16_t                                max_nrof_rs_idxes_to_report = 1;
  beam_meass_report_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BluetoothMeasConfig ::= ENUMERATED
struct bluetooth_meas_cfg_opts {
  enum options { setup, /*...*/ nulltype } value;

  const char* to_string() const;
};
using bluetooth_meas_cfg_e = enumerated<bluetooth_meas_cfg_opts, true>;

// BluetoothMeasConfigNameList ::= SEQUENCE (SIZE (1..4)) OF OCTET STRING (SIZE (1..248))
using bluetooth_meas_cfg_name_list_l = bounded_array<bounded_octstring<1, 248, true>, 4>;

// BluetoothMeasurementConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using bluetooth_meas_cfg_ext_ies_o = protocol_ext_empty_o;

using bluetooth_meas_cfg_ext_ies_container = protocol_ext_container_empty_l;

// BluetoothMeasurementConfiguration ::= SEQUENCE
struct bluetooth_meas_cfg_s {
  struct bt_rssi_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using bt_rssi_e_ = enumerated<bt_rssi_opts, true>;

  // member variables
  bool                                 bt_rssi_present = false;
  bool                                 ie_exts_present = false;
  bluetooth_meas_cfg_e                 bluetooth_meas_cfg;
  bluetooth_meas_cfg_name_list_l       bluetooth_meas_cfg_name_list;
  bt_rssi_e_                           bt_rssi;
  bluetooth_meas_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// S-NSSAI-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using s_nssai_ext_ies_o = protocol_ext_empty_o;

using s_nssai_ext_ies_container = protocol_ext_container_empty_l;

// S-NSSAI ::= SEQUENCE
struct s_nssai_s {
  bool                      sd_present      = false;
  bool                      ie_exts_present = false;
  fixed_octstring<1, true>  sst;
  fixed_octstring<3, true>  sd;
  s_nssai_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceSupport-List ::= SEQUENCE (SIZE (1..1024)) OF S-NSSAI
using slice_support_list_l = dyn_array<s_nssai_s>;

// NPN-Support-SNPN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using npn_support_sn_pn_ext_ies_o = protocol_ext_empty_o;

using npn_support_sn_pn_ext_ies_container = protocol_ext_container_empty_l;

// NPN-Support-SNPN ::= SEQUENCE
struct npn_support_sn_pn_s {
  bool                                ie_ext_present = false;
  fixed_bitstring<44, false, true>    nid;
  npn_support_sn_pn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPN-Support-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using npn_support_ext_ies_o = protocol_ies_empty_o;

// NPN-Support ::= CHOICE
struct npn_support_c {
  struct types_opts {
    enum options { sn_pn, choice_exts, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  npn_support_c() = default;
  npn_support_c(const npn_support_c& other);
  npn_support_c& operator=(const npn_support_c& other);
  ~npn_support_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  npn_support_sn_pn_s& sn_pn()
  {
    assert_choice_type(types::sn_pn, type_, "NPN-Support");
    return c.get<npn_support_sn_pn_s>();
  }
  protocol_ie_single_container_s<npn_support_ext_ies_o>& choice_exts()
  {
    assert_choice_type(types::choice_exts, type_, "NPN-Support");
    return c.get<protocol_ie_single_container_s<npn_support_ext_ies_o>>();
  }
  const npn_support_sn_pn_s& sn_pn() const
  {
    assert_choice_type(types::sn_pn, type_, "NPN-Support");
    return c.get<npn_support_sn_pn_s>();
  }
  const protocol_ie_single_container_s<npn_support_ext_ies_o>& choice_exts() const
  {
    assert_choice_type(types::choice_exts, type_, "NPN-Support");
    return c.get<protocol_ie_single_container_s<npn_support_ext_ies_o>>();
  }
  npn_support_sn_pn_s&                                   set_sn_pn();
  protocol_ie_single_container_s<npn_support_ext_ies_o>& set_choice_exts();

private:
  types                                                                                       type_;
  choice_buffer_t<npn_support_sn_pn_s, protocol_ie_single_container_s<npn_support_ext_ies_o>> c;

  void destroy_();
};

// ExtendedSliceSupportList ::= SEQUENCE (SIZE (1..65535)) OF S-NSSAI
using extended_slice_support_list_l = dyn_array<s_nssai_s>;

// TAINSAGSupportItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tai_nsag_support_item_ext_ies_o = protocol_ext_empty_o;

using tai_nsag_support_item_ext_ies_container = protocol_ext_container_empty_l;

// TAINSAGSupportItem ::= SEQUENCE
struct tai_nsag_support_item_s {
  bool                                    ie_exts_present = false;
  uint16_t                                nsag_id         = 0;
  extended_slice_support_list_l           nsag_slice_support_list;
  tai_nsag_support_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAINSAGSupportList ::= SEQUENCE (SIZE (1..256)) OF TAINSAGSupportItem
using tai_nsag_support_list_l = dyn_array<tai_nsag_support_item_s>;

// UnavailableNRCellList ::= SEQUENCE (SIZE (1..16384)) OF NR-CGI
using unavailable_nr_cell_list_l = dyn_array<nr_cgi_s>;

// UnavailableCellList-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using unavailable_cell_list_ext_ies_o = protocol_ext_empty_o;

using unavailable_cell_list_ext_ies_container = protocol_ext_container_empty_l;

// UnavailableCellList ::= SEQUENCE
struct unavailable_cell_list_s {
  bool                                    ie_exts_present = false;
  unavailable_nr_cell_list_l              unavailable_nr_cell_list;
  unavailable_cell_list_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceAvailabilityList-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using slice_availability_list_ext_ies_o = protocol_ies_empty_o;

// SliceAvailabilityList ::= CHOICE
struct slice_availability_list_c {
  struct types_opts {
    enum options { unavailable_cell_list, available_cell_list, choice_ext, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  slice_availability_list_c() = default;
  slice_availability_list_c(const slice_availability_list_c& other);
  slice_availability_list_c& operator=(const slice_availability_list_c& other);
  ~slice_availability_list_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  unavailable_cell_list_s& unavailable_cell_list()
  {
    assert_choice_type(types::unavailable_cell_list, type_, "SliceAvailabilityList");
    return c.get<unavailable_cell_list_s>();
  }
  available_cell_list_s& available_cell_list()
  {
    assert_choice_type(types::available_cell_list, type_, "SliceAvailabilityList");
    return c.get<available_cell_list_s>();
  }
  protocol_ie_single_container_s<slice_availability_list_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SliceAvailabilityList");
    return c.get<protocol_ie_single_container_s<slice_availability_list_ext_ies_o>>();
  }
  const unavailable_cell_list_s& unavailable_cell_list() const
  {
    assert_choice_type(types::unavailable_cell_list, type_, "SliceAvailabilityList");
    return c.get<unavailable_cell_list_s>();
  }
  const available_cell_list_s& available_cell_list() const
  {
    assert_choice_type(types::available_cell_list, type_, "SliceAvailabilityList");
    return c.get<available_cell_list_s>();
  }
  const protocol_ie_single_container_s<slice_availability_list_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SliceAvailabilityList");
    return c.get<protocol_ie_single_container_s<slice_availability_list_ext_ies_o>>();
  }
  unavailable_cell_list_s&                                           set_unavailable_cell_list();
  available_cell_list_s&                                             set_available_cell_list();
  protocol_ie_single_container_s<slice_availability_list_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<available_cell_list_s,
                  protocol_ie_single_container_s<slice_availability_list_ext_ies_o>,
                  unavailable_cell_list_s>
      c;

  void destroy_();
};

// TAISliceUnavailableCellItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tai_slice_unavailable_cell_item_ext_ies_o = protocol_ext_empty_o;

using tai_slice_unavailable_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// TAISliceUnavailableCellItem ::= SEQUENCE
struct tai_slice_unavailable_cell_item_s {
  bool                                              ie_exts_present = false;
  s_nssai_s                                         snssai;
  slice_availability_list_c                         slice_availability_list;
  tai_slice_unavailable_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAISliceUnavailableCellList ::= SEQUENCE (SIZE (1..65535)) OF TAISliceUnavailableCellItem
using tai_slice_unavailable_cell_list_l = dyn_array<tai_slice_unavailable_cell_item_s>;

// BroadcastPLMNinTAISupport-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct broadcast_plmn_in_tai_support_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct broadcast_plmn_in_tai_support_item_ext_ies_container {
  bool                              npn_support_present                     = false;
  bool                              extended_tai_slice_support_list_present = false;
  bool                              tai_nsag_support_list_present           = false;
  bool                              tai_slice_unavailable_cell_list_present = false;
  npn_support_c                     npn_support;
  extended_slice_support_list_l     extended_tai_slice_support_list;
  tai_nsag_support_list_l           tai_nsag_support_list;
  tai_slice_unavailable_cell_list_l tai_slice_unavailable_cell_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BroadcastPLMNinTAISupport-Item ::= SEQUENCE
struct broadcast_plmn_in_tai_support_item_s {
  bool                                                 ie_ext_present = false;
  fixed_octstring<3, true>                             plmn_id;
  slice_support_list_l                                 tai_slice_support_list;
  broadcast_plmn_in_tai_support_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHO-CPAC-Config-Indicator ::= ENUMERATED
struct cho_cp_ac_cfg_ind_opts {
  enum options { cho_only_not_prepared, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cho_cp_ac_cfg_ind_e = enumerated<cho_cp_ac_cfg_ind_opts, true>;

// QoSFlowsAdmitted-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flows_admitted_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flows_admitted_item_ext_ies_container {
  bool    current_qos_para_set_idx_present = false;
  uint8_t current_qos_para_set_idx;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsAdmitted-Item ::= SEQUENCE
struct qos_flows_admitted_item_s {
  bool                                      ie_ext_present = false;
  uint8_t                                   qfi            = 0;
  qos_flows_admitted_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsAdmitted-List ::= SEQUENCE (SIZE (1..64)) OF QoSFlowsAdmitted-Item
using qos_flows_admitted_list_l = dyn_array<qos_flows_admitted_item_s>;

// CauseRadioNetworkLayer ::= ENUMERATED
struct cause_radio_network_layer_opts {
  enum options {
    cell_not_available,
    ho_desirable_for_radio_reasons,
    ho_target_not_allowed,
    invalid_amf_set_id,
    no_radio_res_available_in_target_cell,
    partial_ho,
    reduce_load_in_serving_cell,
    res_optim_ho,
    time_crit_ho,
    txn_relo_coverall_expiry,
    txn_relo_cprep_expiry,
    unknown_guami_id,
    unknown_local_ng_ran_node_ue_xnap_id,
    inconsistent_remote_ng_ran_node_ue_xnap_id,
    encryption_and_or_integrity_protection_algorithms_not_supported,
    not_used_causes_value_neg1,
    multiple_pdu_session_id_instances,
    unknown_pdu_session_id,
    unknown_qos_flow_id,
    multiple_qos_flow_id_instances,
    switch_off_ongoing,
    not_supported_5qi_value,
    txn_d_coverall_expiry,
    txn_d_cprep_expiry,
    action_desirable_for_radio_reasons,
    reduce_load,
    res_optim,
    time_crit_action,
    target_not_allowed,
    no_radio_res_available,
    invalid_qos_combination,
    encryption_algorithms_not_supported,
    proc_cancelled,
    rrm_purpose,
    improve_user_bit_rate,
    user_inactivity,
    radio_conn_with_ue_lost,
    fail_in_the_radio_interface_proc,
    bearer_option_not_supported,
    up_integrity_protection_not_possible,
    up_confidentiality_protection_not_possible,
    res_not_available_for_the_slice_s,
    ue_max_ip_data_rate_reason,
    cp_integrity_protection_fail,
    up_integrity_protection_fail,
    slice_not_supported_by_ng_ran,
    mn_mob,
    sn_mob,
    count_reaches_max_value,
    unknown_old_ng_ran_node_ue_xnap_id,
    pdcp_overload,
    drb_id_not_available,
    unspecified,
    // ...
    ue_context_id_not_known,
    non_relocation_of_context,
    cho_cpc_res_tobechanged,
    rsn_not_available_for_the_up,
    npn_access_denied,
    report_characteristics_empty,
    existing_meas_id,
    meas_temporarily_not_available,
    meas_not_supported_for_the_obj,
    ue_pwr_saving,
    not_existing_ng_ran_node2_meas_id,
    insufficient_ue_cap,
    normal_release,
    value_out_of_allowed_range,
    scg_activation_deactivation_fail,
    scg_deactivation_fail_due_to_data_tx,
    ssb_not_available,
    ltm_triggered,
    no_backhaul_res,
    miab_node_not_authorized,
    iab_not_authorized,
    nulltype
  } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using cause_radio_network_layer_e = enumerated<cause_radio_network_layer_opts, true, 21>;

// CauseTransportLayer ::= ENUMERATED
struct cause_transport_layer_opts {
  enum options { transport_res_unavailable, unspecified, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cause_transport_layer_e = enumerated<cause_transport_layer_opts, true>;

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
    hardware_fail,
    o_and_m_intervention,
    not_enough_user_plane_processing_res,
    unspecified,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using cause_misc_e = enumerated<cause_misc_opts, true>;

// Cause-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
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
  cause_radio_network_layer_e& radio_network()
  {
    assert_choice_type(types::radio_network, type_, "Cause");
    return c.get<cause_radio_network_layer_e>();
  }
  cause_transport_layer_e& transport()
  {
    assert_choice_type(types::transport, type_, "Cause");
    return c.get<cause_transport_layer_e>();
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
  const cause_radio_network_layer_e& radio_network() const
  {
    assert_choice_type(types::radio_network, type_, "Cause");
    return c.get<cause_radio_network_layer_e>();
  }
  const cause_transport_layer_e& transport() const
  {
    assert_choice_type(types::transport, type_, "Cause");
    return c.get<cause_transport_layer_e>();
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
  cause_radio_network_layer_e&                     set_radio_network();
  cause_transport_layer_e&                         set_transport();
  cause_protocol_e&                                set_protocol();
  cause_misc_e&                                    set_misc();
  protocol_ie_single_container_s<cause_ext_ies_o>& set_choice_ext();

private:
  types                                                            type_;
  choice_buffer_t<protocol_ie_single_container_s<cause_ext_ies_o>> c;

  void destroy_();
};

// QoSFlowwithCause-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qos_flowwith_cause_item_ext_ies_o = protocol_ext_empty_o;

using qos_flowwith_cause_item_ext_ies_container = protocol_ext_container_empty_l;

// QoSFlowwithCause-Item ::= SEQUENCE
struct qos_flowwith_cause_item_s {
  bool                                      cause_present  = false;
  bool                                      ie_ext_present = false;
  uint8_t                                   qfi            = 0;
  cause_c                                   cause;
  qos_flowwith_cause_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlows-List-withCause ::= SEQUENCE (SIZE (1..64)) OF QoSFlowwithCause-Item
using qos_flows_list_with_cause_l = dyn_array<qos_flowwith_cause_item_s>;

// SecondarydataForwardingInfoFromTarget-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using secondarydata_forwarding_info_from_target_item_ext_ies_o = protocol_ext_empty_o;

using secondarydata_forwarding_info_from_target_item_ext_ies_container = protocol_ext_container_empty_l;

// SecondarydataForwardingInfoFromTarget-Item ::= SEQUENCE
struct secondarydata_forwarding_info_from_target_item_s {
  bool                                                             ie_exts_present = false;
  data_forwarding_info_from_target_ngran_node_s                    secondarydata_forwarding_info_from_target;
  secondarydata_forwarding_info_from_target_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SecondarydataForwardingInfoFromTarget-List ::= SEQUENCE (SIZE (1..3)) OF SecondarydataForwardingInfoFromTarget-Item
using secondarydata_forwarding_info_from_target_list_l = dyn_array<secondarydata_forwarding_info_from_target_item_s>;

// PDUSessionResourceAdmittedInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_admitted_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_admitted_info_ext_ies_container {
  bool                                             secondarydata_forwarding_info_from_target_list_present = false;
  secondarydata_forwarding_info_from_target_list_l secondarydata_forwarding_info_from_target_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceAdmittedInfo ::= SEQUENCE
struct pdu_session_res_admitted_info_s {
  struct dl_ng_u_tnl_info_unchanged_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using dl_ng_u_tnl_info_unchanged_e_ = enumerated<dl_ng_u_tnl_info_unchanged_opts, true>;

  // member variables
  bool                                            dl_ng_u_tnl_info_unchanged_present       = false;
  bool                                            data_forwarding_info_from_target_present = false;
  bool                                            ie_exts_present                          = false;
  dl_ng_u_tnl_info_unchanged_e_                   dl_ng_u_tnl_info_unchanged;
  qos_flows_admitted_list_l                       qos_flows_admitted_list;
  qos_flows_list_with_cause_l                     qos_flows_not_admitted_list;
  data_forwarding_info_from_target_ngran_node_s   data_forwarding_info_from_target;
  pdu_session_res_admitted_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourcesAdmitted-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_admitted_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_admitted_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourcesAdmitted-Item ::= SEQUENCE
struct pdu_session_res_admitted_item_s {
  bool                                            ie_exts_present = false;
  uint16_t                                        pdu_session_id  = 0;
  pdu_session_res_admitted_info_s                 pdu_session_res_admitted_info;
  pdu_session_res_admitted_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourcesAdmitted-List ::= SEQUENCE (SIZE (1..256)) OF PDUSessionResourcesAdmitted-Item
using pdu_session_res_admitted_list_l = dyn_array<pdu_session_res_admitted_item_s>;

// CHO-Candidate-PSCells-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cho_candidate_p_scells_item_ext_ies_o = protocol_ext_empty_o;

using cho_candidate_p_scells_item_ext_ies_container = protocol_ext_container_empty_l;

// CHO-Candidate-PSCells-Item ::= SEQUENCE
struct cho_candidate_p_scells_item_s {
  bool                                          ie_exts_present = false;
  nr_cgi_s                                      pscell_id;
  unbounded_octstring<true>                     target2source_ng_ran_node_container;
  cho_candidate_p_scells_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHO-Candidate-PSCells-list ::= SEQUENCE (SIZE (1..8)) OF CHO-Candidate-PSCells-Item
using cho_candidate_p_scells_list_l = dyn_array<cho_candidate_p_scells_item_s>;

// CHO-target-SN-node-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cho_target_sn_node_item_ext_ies_o = protocol_ext_empty_o;

using cho_target_sn_node_item_ext_ies_container = protocol_ext_container_empty_l;

// CHO-target-SN-node-Item ::= SEQUENCE
struct cho_target_sn_node_item_s {
  bool                                      ie_exts_present = false;
  global_ng_ran_node_id_c                   target_s_ng_ran_node_id;
  pdu_session_res_admitted_list_l           pdu_session_res_admitted_list;
  cho_candidate_p_scells_list_l             cho_candidate_p_scells_list;
  cho_target_sn_node_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHO-target-SN-node-list ::= SEQUENCE (SIZE (1..8)) OF CHO-target-SN-node-Item
using cho_target_sn_node_list_l = dyn_array<cho_target_sn_node_item_s>;

// CHO-CPAC-Information-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cho_cp_ac_info_ext_ies_o = protocol_ext_empty_o;

using cho_cp_ac_info_ext_ies_container = protocol_ext_container_empty_l;

// CHO-CPAC-Information ::= SEQUENCE
struct cho_cp_ac_info_s {
  bool                             cho_cp_ac_cfg_ind_present = false;
  bool                             ie_exts_present           = false;
  cho_cp_ac_cfg_ind_e              cho_cp_ac_cfg_ind;
  cho_target_sn_node_list_l        cho_target_sn_node_list;
  cho_cp_ac_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHO-MRDC-EarlyDataForwarding ::= ENUMERATED
struct cho_mrdc_early_data_forwarding_opts {
  enum options { stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cho_mrdc_early_data_forwarding_e = enumerated<cho_mrdc_early_data_forwarding_opts, true>;

// CHO-MRDC-Indicator ::= ENUMERATED
struct cho_mrdc_ind_opts {
  enum options { true_value, /*...*/ coordination_only, nulltype } value;

  const char* to_string() const;
};
using cho_mrdc_ind_e = enumerated<cho_mrdc_ind_opts, true, 1>;

// CHOExecutionCondition-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cho_execution_condition_item_ext_ies_o = protocol_ext_empty_o;

using cho_execution_condition_item_ext_ies_container = protocol_ext_container_empty_l;

// CHOExecutionCondition-Item ::= SEQUENCE
struct cho_execution_condition_item_s {
  bool                                           ie_exts_present = false;
  unbounded_octstring<true>                      meas_obj_container;
  unbounded_octstring<true>                      report_cfg_container;
  cho_execution_condition_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOExecutionCondition-List ::= SEQUENCE (SIZE (1..2)) OF CHOExecutionCondition-Item
using cho_execution_condition_list_l = dyn_array<cho_execution_condition_item_s>;

// CHOCandidateCell-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cho_candidate_cell_item_ext_ies_o = protocol_ext_empty_o;

using cho_candidate_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// CHOCandidateCell-Item ::= SEQUENCE
struct cho_candidate_cell_item_s {
  bool                                      ie_exts_present = false;
  global_ng_ran_cell_id_s                   cho_candidate_cell_id;
  cho_execution_condition_list_l            cho_execution_condition_list;
  cho_candidate_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOCandidateCell-List ::= SEQUENCE (SIZE (1..8)) OF CHOCandidateCell-Item
using cho_candidate_cell_list_l = dyn_array<cho_candidate_cell_item_s>;

// CHOConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cho_cfg_ext_ies_o = protocol_ext_empty_o;

using cho_cfg_ext_ies_container = protocol_ext_container_empty_l;

// CHOConfiguration ::= SEQUENCE
struct cho_cfg_s {
  bool                      ie_exts_present = false;
  cho_candidate_cell_list_l cho_candidate_cell_list;
  cho_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOTimeBasedInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cho_time_based_info_ext_ies_o = protocol_ext_empty_o;

using cho_time_based_info_ext_ies_container = protocol_ext_container_empty_l;

// CHOTimeBasedInformation ::= SEQUENCE
struct cho_time_based_info_s {
  bool                                  ie_exts_present  = false;
  uint64_t                              cho_ho_win_start = 0;
  uint16_t                              cho_ho_win_dur   = 1;
  cho_time_based_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TargetCGI-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using target_cgi_ext_ies_o = protocol_ies_empty_o;

// Target-CGI ::= CHOICE
struct target_cgi_c {
  struct types_opts {
    enum options { nr, e_utra, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  target_cgi_c() = default;
  target_cgi_c(const target_cgi_c& other);
  target_cgi_c& operator=(const target_cgi_c& other);
  ~target_cgi_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  nr_cgi_s& nr()
  {
    assert_choice_type(types::nr, type_, "Target-CGI");
    return c.get<nr_cgi_s>();
  }
  e_utra_cgi_s& e_utra()
  {
    assert_choice_type(types::e_utra, type_, "Target-CGI");
    return c.get<e_utra_cgi_s>();
  }
  protocol_ie_single_container_s<target_cgi_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "Target-CGI");
    return c.get<protocol_ie_single_container_s<target_cgi_ext_ies_o>>();
  }
  const nr_cgi_s& nr() const
  {
    assert_choice_type(types::nr, type_, "Target-CGI");
    return c.get<nr_cgi_s>();
  }
  const e_utra_cgi_s& e_utra() const
  {
    assert_choice_type(types::e_utra, type_, "Target-CGI");
    return c.get<e_utra_cgi_s>();
  }
  const protocol_ie_single_container_s<target_cgi_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "Target-CGI");
    return c.get<protocol_ie_single_container_s<target_cgi_ext_ies_o>>();
  }
  nr_cgi_s&                                             set_nr();
  e_utra_cgi_s&                                         set_e_utra();
  protocol_ie_single_container_s<target_cgi_ext_ies_o>& set_choice_ext();

private:
  types                                                                                         type_;
  choice_buffer_t<e_utra_cgi_s, nr_cgi_s, protocol_ie_single_container_s<target_cgi_ext_ies_o>> c;

  void destroy_();
};

// CHOinformation-Ack-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ch_oinfo_ack_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ch_oinfo_ack_ext_ies_container {
  bool             cho_cp_ac_info_present = false;
  cho_cp_ac_info_s cho_cp_ac_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOinformation-Ack ::= SEQUENCE
struct ch_oinfo_ack_s {
  bool                           max_ch_ooperations_present = false;
  bool                           ie_exts_present            = false;
  target_cgi_c                   requested_target_cell_global_id;
  uint8_t                        max_ch_ooperations = 1;
  ch_oinfo_ack_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOinformation-AddReq-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ch_oinfo_add_req_ext_ies_o = protocol_ext_empty_o;

using ch_oinfo_add_req_ext_ies_container = protocol_ext_container_empty_l;

// CHOinformation-AddReq ::= SEQUENCE
struct ch_oinfo_add_req_s {
  bool                               cho_estimated_arrival_probability_present = false;
  bool                               ie_exts_present                           = false;
  global_ng_ran_node_id_c            source_m_ngran_node_id;
  uint64_t                           source_m_ngran_node_ue_xnap_id    = 0;
  uint8_t                            cho_estimated_arrival_probability = 1;
  ch_oinfo_add_req_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOinformation-AddReqAck-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ch_oinfo_add_req_ack_ext_ies_o = protocol_ext_empty_o;

using ch_oinfo_add_req_ack_ext_ies_container = protocol_ext_container_empty_l;

// CHOinformation-AddReqAck ::= SEQUENCE
struct ch_oinfo_add_req_ack_s {
  bool                                   pcell_id_present = false;
  bool                                   ie_exts_present  = false;
  global_ng_ran_cell_id_s                pcell_id;
  ch_oinfo_add_req_ack_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOinformation-ModReq-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ch_oinfo_mod_req_ext_ies_o = protocol_ext_empty_o;

using ch_oinfo_mod_req_ext_ies_container = protocol_ext_container_empty_l;

// CHOinformation-ModReq ::= SEQUENCE
struct ch_oinfo_mod_req_s {
  struct conditional_recfg_opts {
    enum options { intra_mn_cho, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using conditional_recfg_e_ = enumerated<conditional_recfg_opts, true>;

  // member variables
  bool                               cho_estimated_arrival_probability_present = false;
  bool                               ie_exts_present                           = false;
  conditional_recfg_e_               conditional_recfg;
  uint8_t                            cho_estimated_arrival_probability = 1;
  ch_oinfo_mod_req_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOtrigger ::= ENUMERATED
struct ch_otrigger_opts {
  enum options { cho_initiation, cho_replace, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ch_otrigger_e = enumerated<ch_otrigger_opts, true>;

// CHOinformation-Req-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ch_oinfo_req_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ch_oinfo_req_ext_ies_container {
  bool                  cho_time_based_info_present    = false;
  bool                  cho_maxnoof_cond_recfg_present = false;
  cho_time_based_info_s cho_time_based_info;
  uint8_t               cho_maxnoof_cond_recfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CHOinformation-Req ::= SEQUENCE
struct ch_oinfo_req_s {
  bool                           target_ng_ran_node_ue_xnap_id_present     = false;
  bool                           cho_estimated_arrival_probability_present = false;
  bool                           ie_exts_present                           = false;
  ch_otrigger_e                  cho_trigger;
  uint64_t                       target_ng_ran_node_ue_xnap_id     = 0;
  uint8_t                        cho_estimated_arrival_probability = 1;
  ch_oinfo_req_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CNTypeRestrictionsForEquivalentItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cn_type_restricts_for_equivalent_item_ext_ies_o = protocol_ext_empty_o;

using cn_type_restricts_for_equivalent_item_ext_ies_container = protocol_ext_container_empty_l;

// CNTypeRestrictionsForEquivalentItem ::= SEQUENCE
struct cn_type_restricts_for_equivalent_item_s {
  struct cn_type_opts {
    enum options { epc_forbidden, five_gc_forbidden, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using cn_type_e_ = enumerated<cn_type_opts, true>;

  // member variables
  bool                                                    ie_exts_present = false;
  fixed_octstring<3, true>                                plmn_id;
  cn_type_e_                                              cn_type;
  cn_type_restricts_for_equivalent_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CNTypeRestrictionsForEquivalent ::= SEQUENCE (SIZE (1..15)) OF CNTypeRestrictionsForEquivalentItem
using cn_type_restricts_for_equivalent_l = dyn_array<cn_type_restricts_for_equivalent_item_s>;

// CNTypeRestrictionsForServing ::= ENUMERATED
struct cn_type_restricts_for_serving_opts {
  enum options { epc_forbidden, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cn_type_restricts_for_serving_e = enumerated<cn_type_restricts_for_serving_opts, true>;

// COUNT-PDCP-SN12-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using count_pdcp_sn12_ext_ies_o = protocol_ext_empty_o;

using count_pdcp_sn12_ext_ies_container = protocol_ext_container_empty_l;

// COUNT-PDCP-SN12 ::= SEQUENCE
struct count_pdcp_sn12_s {
  bool                              ie_exts_present = false;
  uint16_t                          pdcp_sn12       = 0;
  uint32_t                          hfn_pdcp_sn12   = 0;
  count_pdcp_sn12_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// COUNT-PDCP-SN18-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using count_pdcp_sn18_ext_ies_o = protocol_ext_empty_o;

using count_pdcp_sn18_ext_ies_container = protocol_ext_container_empty_l;

// COUNT-PDCP-SN18 ::= SEQUENCE
struct count_pdcp_sn18_s {
  bool                              ie_exts_present = false;
  uint32_t                          pdcp_sn18       = 0;
  uint16_t                          hfn_pdcp_sn18   = 0;
  count_pdcp_sn18_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPAC-Preparation-Type ::= ENUMERATED
struct cp_ac_prep_type_opts {
  enum options { s_cpac, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cp_ac_prep_type_e = enumerated<cp_ac_prep_type_opts, true>;

// CPACExecutionCondition-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cp_ac_execution_condition_item_ext_ies_o = protocol_ext_empty_o;

using cp_ac_execution_condition_item_ext_ies_container = protocol_ext_container_empty_l;

// CPACExecutionCondition-Item ::= SEQUENCE
struct cp_ac_execution_condition_item_s {
  bool                                             ie_exts_present = false;
  unbounded_octstring<true>                        meas_obj_container;
  unbounded_octstring<true>                        report_cfg_container;
  cp_ac_execution_condition_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPACExecutionCondition-List ::= SEQUENCE (SIZE (1..2)) OF CPACExecutionCondition-Item
using cp_ac_execution_condition_list_l = dyn_array<cp_ac_execution_condition_item_s>;

// CPACCandidateCell-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cp_ac_candidate_cell_item_ext_ies_o = protocol_ext_empty_o;

using cp_ac_candidate_cell_item_ext_ies_container = protocol_ext_container_empty_l;

// CPACCandidateCell-Item ::= SEQUENCE
struct cp_ac_candidate_cell_item_s {
  bool                                        ie_exts_present = false;
  global_ng_ran_cell_id_s                     cpac_candidate_cell_id;
  cp_ac_execution_condition_list_l            cpac_execution_condition_list;
  cp_ac_candidate_cell_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPACCandidateCell-List ::= SEQUENCE (SIZE (1..8)) OF CPACCandidateCell-Item
using cp_ac_candidate_cell_list_l = dyn_array<cp_ac_candidate_cell_item_s>;

// CPACConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cp_ac_cfg_ext_ies_o = protocol_ext_empty_o;

using cp_ac_cfg_ext_ies_container = protocol_ext_container_empty_l;

// CPACConfiguration ::= SEQUENCE
struct cp_ac_cfg_s {
  bool                        ie_exts_present = false;
  cp_ac_candidate_cell_list_l cpac_candidate_cell_list;
  cp_ac_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPACcandidatePSCells-item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cp_a_ccandidate_p_scells_item_ext_ies_o = protocol_ext_empty_o;

using cp_a_ccandidate_p_scells_item_ext_ies_container = protocol_ext_container_empty_l;

// CPACcandidatePSCells-item ::= SEQUENCE
struct cp_a_ccandidate_p_scells_item_s {
  bool                                            ie_exts_present = false;
  nr_cgi_s                                        pscell_id;
  cp_a_ccandidate_p_scells_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPACcandidatePSCells-list ::= SEQUENCE (SIZE (1..8)) OF CPACcandidatePSCells-item
using cp_a_ccandidate_p_scells_list_l = dyn_array<cp_a_ccandidate_p_scells_item_s>;

// CompleteCandidateConfig-Indicator ::= ENUMERATED
struct complete_candidate_cfg_ind_opts {
  enum options { complete_candidate_cfg, /*...*/ nulltype } value;

  const char* to_string() const;
};
using complete_candidate_cfg_ind_e = enumerated<complete_candidate_cfg_ind_opts, true>;

// CPACcandidatePSCells-wotherInfo-item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cp_a_ccandidate_p_scells_wother_info_item_ext_ies_o = protocol_ext_empty_o;

using cp_a_ccandidate_p_scells_wother_info_item_ext_ies_container = protocol_ext_container_empty_l;

// CPACcandidatePSCells-wotherInfo-item ::= SEQUENCE
struct cp_a_ccandidate_p_scells_wother_info_item_s {
  bool                                                        s_cp_ac_complete_candidate_cfg_ind_present = false;
  bool                                                        ie_exts_present                            = false;
  nr_cgi_s                                                    pscell_id;
  complete_candidate_cfg_ind_e                                s_cp_ac_complete_candidate_cfg_ind;
  cp_a_ccandidate_p_scells_wother_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPACcandidatePSCells-wotherInfo-list ::= SEQUENCE (SIZE (1..8)) OF CPACcandidatePSCells-wotherInfo-item
using cp_a_ccandidate_p_scells_wother_info_list_l = dyn_array<cp_a_ccandidate_p_scells_wother_info_item_s>;

// CPACInformationModRequired-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct cp_ac_info_mod_required_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cp_ac_info_mod_required_ext_ies_container {
  bool                                        cp_a_ccandidate_p_scells_wother_info_list_present = false;
  cp_a_ccandidate_p_scells_wother_info_list_l cp_a_ccandidate_p_scells_wother_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPACInformationModRequired ::= SEQUENCE
struct cp_ac_info_mod_required_s {
  bool                                      ie_exts_present = false;
  cp_a_ccandidate_p_scells_list_l           candidate_pscells;
  cp_ac_info_mod_required_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPAInformationAck-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct cp_a_info_ack_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cp_a_info_ack_ext_ies_container {
  bool                                        cp_a_ccandidate_p_scells_wother_info_list_present = false;
  cp_a_ccandidate_p_scells_wother_info_list_l cp_a_ccandidate_p_scells_wother_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPAInformationAck ::= SEQUENCE
struct cp_a_info_ack_s {
  bool                            ie_exts_present = false;
  cp_a_ccandidate_p_scells_list_l candidate_pscells;
  cp_a_info_ack_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// S-CPAC-SecurityConfig-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using s_cp_ac_security_cfg_item_ext_ies_o = protocol_ext_empty_o;

using s_cp_ac_security_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// S-CPAC-SecurityConfig-Item ::= SEQUENCE
struct s_cp_ac_security_cfg_item_s {
  bool                                        ie_exts_present = false;
  fixed_bitstring<256, false, true>           s_ng_ra_nnode_security_key;
  uint32_t                                    sk_counter = 0;
  s_cp_ac_security_cfg_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// S-CPAC-SecurityConfig-List ::= SEQUENCE (SIZE (1..8)) OF S-CPAC-SecurityConfig-Item
using s_cp_ac_security_cfg_list_l = dyn_array<s_cp_ac_security_cfg_item_s>;

// S-CPAC-MultiTargetSN-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using s_cp_ac_multi_target_sn_item_ext_ies_o = protocol_ext_empty_o;

using s_cp_ac_multi_target_sn_item_ext_ies_container = protocol_ext_container_empty_l;

// S-CPAC-MultiTargetSN-Item ::= SEQUENCE
struct s_cp_ac_multi_target_sn_item_s {
  bool                                           ie_exts_present = false;
  global_ng_ran_node_id_c                        target_s_ng_ran_node_id;
  unbounded_octstring<true>                      recommended_candidate_p_scells;
  s_cp_ac_multi_target_sn_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// S-CPAC-MultiTargetSN-List ::= SEQUENCE (SIZE (1..7)) OF S-CPAC-MultiTargetSN-Item
using s_cp_ac_multi_target_sn_list_l = dyn_array<s_cp_ac_multi_target_sn_item_s>;

// S-CPAC-Request-Info-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using s_cp_ac_request_info_ext_ies_o = protocol_ext_empty_o;

using s_cp_ac_request_info_ext_ies_container = protocol_ext_container_empty_l;

// S-CPAC-Request-Info ::= SEQUENCE
struct s_cp_ac_request_info_s {
  bool                                   ie_exts_present = false;
  s_cp_ac_security_cfg_list_l            s_cp_ac_security_cfg_list;
  s_cp_ac_multi_target_sn_list_l         s_cp_ac_multi_target_sn_list;
  s_cp_ac_request_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// S-CPAC-ReferenceConfig-Request ::= ENUMERATED
struct s_cp_ac_ref_cfg_request_opts {
  enum options { request, /*...*/ nulltype } value;

  const char* to_string() const;
};
using s_cp_ac_ref_cfg_request_e = enumerated<s_cp_ac_ref_cfg_request_opts, true>;

// S-CPAC-InterSN-ExecutionNotify ::= ENUMERATED
struct s_cp_ac_inter_sn_execution_notify_opts {
  enum options { executed, /*...*/ nulltype } value;

  const char* to_string() const;
};
using s_cp_ac_inter_sn_execution_notify_e = enumerated<s_cp_ac_inter_sn_execution_notify_opts, true>;

// CPAInformationModReq-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct cp_a_info_mod_req_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cp_a_info_mod_req_ext_ies_container {
  bool                                s_cp_ac_request_info_present              = false;
  bool                                s_cp_ac_ref_cfg_request_present           = false;
  bool                                s_cp_ac_inter_sn_execution_notify_present = false;
  s_cp_ac_request_info_s              s_cp_ac_request_info;
  s_cp_ac_ref_cfg_request_e           s_cp_ac_ref_cfg_request;
  s_cp_ac_inter_sn_execution_notify_e s_cp_ac_inter_sn_execution_notify;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPAInformationModReq ::= SEQUENCE
struct cp_a_info_mod_req_s {
  bool                                max_no_of_pscells_present                  = false;
  bool                                cpac_estimated_arrival_probability_present = false;
  bool                                ie_exts_present                            = false;
  uint8_t                             max_no_of_pscells                          = 1;
  uint8_t                             cpac_estimated_arrival_probability         = 1;
  cp_a_info_mod_req_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPAInformationModReqAck-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct cp_a_info_mod_req_ack_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cp_a_info_mod_req_ack_ext_ies_container {
  bool                                        cp_a_ccandidate_p_scells_wother_info_list_present = false;
  cp_a_ccandidate_p_scells_wother_info_list_l cp_a_ccandidate_p_scells_wother_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPAInformationModReqAck ::= SEQUENCE
struct cp_a_info_mod_req_ack_s {
  bool                                    ie_exts_present = false;
  cp_a_ccandidate_p_scells_list_l         candidate_pscells;
  cp_a_info_mod_req_ack_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPAInformationRequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct cp_a_info_request_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cp_a_info_request_ext_ies_container {
  bool                      s_cp_ac_request_info_present    = false;
  bool                      s_cp_ac_ref_cfg_request_present = false;
  s_cp_ac_request_info_s    s_cp_ac_request_info;
  s_cp_ac_ref_cfg_request_e s_cp_ac_ref_cfg_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPAInformationRequest ::= SEQUENCE
struct cp_a_info_request_s {
  bool                                cpac_estimated_arrival_probability_present = false;
  bool                                ie_exts_present                            = false;
  uint8_t                             max_no_of_pscells                          = 1;
  uint8_t                             cpac_estimated_arrival_probability         = 1;
  cp_a_info_request_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPC-DataForwarding-Indicator ::= ENUMERATED
struct cp_c_data_forwarding_ind_opts {
  enum options { triggered, early_data_tx_stop, /*...*/ coordination_only, nulltype } value;

  const char* to_string() const;
};
using cp_c_data_forwarding_ind_e = enumerated<cp_c_data_forwarding_ind_opts, true, 1>;

// CPC-target-SN-confirm-list-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct cp_c_target_sn_confirm_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct cp_c_target_sn_confirm_list_item_ext_ies_container {
  bool              cp_ac_prep_type_present = false;
  cp_ac_prep_type_e cp_ac_prep_type;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPC-target-SN-confirm-list-Item ::= SEQUENCE
struct cp_c_target_sn_confirm_list_item_s {
  bool                                               ie_exts_present = false;
  global_ng_ran_node_id_c                            target_s_ng_ran_node_id;
  cp_a_ccandidate_p_scells_list_l                    candidate_pscells;
  cp_c_target_sn_confirm_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPC-target-SN-confirm-list ::= SEQUENCE (SIZE (1..8)) OF CPC-target-SN-confirm-list-Item
using cp_c_target_sn_confirm_list_l = dyn_array<cp_c_target_sn_confirm_list_item_s>;

// CPCInformationUpdatePSCells-item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using c_pci_nformation_upd_p_scells_item_ext_ies_o = protocol_ext_empty_o;

using c_pci_nformation_upd_p_scells_item_ext_ies_container = protocol_ext_container_empty_l;

// CPCInformationUpdatePSCells-item ::= SEQUENCE
struct c_pci_nformation_upd_p_scells_item_s {
  bool                                                 ie_exts_present = false;
  nr_cgi_s                                             pscell_id;
  c_pci_nformation_upd_p_scells_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPCInformationUpdatePSCells-list ::= SEQUENCE (SIZE (1..8)) OF CPCInformationUpdatePSCells-item
using c_pci_nformation_upd_p_scells_list_l = dyn_array<c_pci_nformation_upd_p_scells_item_s>;

// CPC-target-SN-mod-item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cp_c_target_sn_mod_item_ext_ies_o = protocol_ext_empty_o;

using cp_c_target_sn_mod_item_ext_ies_container = protocol_ext_container_empty_l;

// CPC-target-SN-mod-item ::= SEQUENCE
struct cp_c_target_sn_mod_item_s {
  bool                                      ie_exts_present = false;
  global_ng_ran_node_id_c                   target_s_ng_ran_node_id;
  c_pci_nformation_upd_p_scells_list_l      candidate_pscells;
  cp_c_target_sn_mod_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPC-target-SN-mod-list ::= SEQUENCE (SIZE (1..8)) OF CPC-target-SN-mod-item
using cp_c_target_sn_mod_list_l = dyn_array<cp_c_target_sn_mod_item_s>;

// CPCindicator ::= ENUMERATED
struct cp_cind_opts {
  enum options { cpc_initiation, cpc_mod, cpc_cancellation, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cp_cind_e = enumerated<cp_cind_opts, true>;

// CPC-target-SN-required-list-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cp_c_target_sn_required_list_item_ext_ies_o = protocol_ext_empty_o;

using cp_c_target_sn_required_list_item_ext_ies_container = protocol_ext_container_empty_l;

// CPC-target-SN-required-list-Item ::= SEQUENCE
struct cp_c_target_sn_required_list_item_s {
  bool                                                cpac_estimated_arrival_probability_present = false;
  bool                                                ie_exts_present                            = false;
  global_ng_ran_node_id_c                             target_s_ng_ran_node_id;
  cp_cind_e                                           cpc_ind;
  uint8_t                                             max_no_of_pscells                  = 1;
  uint8_t                                             cpac_estimated_arrival_probability = 1;
  unbounded_octstring<true>                           sn_to_mn_container;
  cp_c_target_sn_required_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPC-target-SN-required-list ::= SEQUENCE (SIZE (1..8)) OF CPC-target-SN-required-list-Item
using cp_c_target_sn_required_list_l = dyn_array<cp_c_target_sn_required_list_item_s>;

// CPCInformationConfirm-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using c_pci_nformation_confirm_ext_ies_o = protocol_ext_empty_o;

using c_pci_nformation_confirm_ext_ies_container = protocol_ext_container_empty_l;

// CPCInformationConfirm ::= SEQUENCE
struct c_pci_nformation_confirm_s {
  bool                                       ie_exts_present = false;
  cp_c_target_sn_confirm_list_l              cpc_target_sn_confirm_list;
  c_pci_nformation_confirm_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// S-CPAC-Request ::= ENUMERATED
struct s_cp_ac_request_opts {
  enum options { initiation, /*...*/ nulltype } value;

  const char* to_string() const;
};
using s_cp_ac_request_e = enumerated<s_cp_ac_request_opts, true>;

// CPCInformationRequired-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct c_pci_nformation_required_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct c_pci_nformation_required_ext_ies_container {
  bool              s_cp_ac_request_present = false;
  s_cp_ac_request_e s_cp_ac_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPCInformationRequired ::= SEQUENCE
struct c_pci_nformation_required_s {
  bool                                        ie_exts_present = false;
  cp_c_target_sn_required_list_l              cpc_target_sn_required_list;
  c_pci_nformation_required_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPCInformationUpdate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using c_pci_nformation_upd_ext_ies_o = protocol_ext_empty_o;

using c_pci_nformation_upd_ext_ies_container = protocol_ext_container_empty_l;

// CPCInformationUpdate ::= SEQUENCE
struct c_pci_nformation_upd_s {
  bool                                   ie_exts_present = false;
  cp_c_target_sn_mod_list_l              cpc_target_sn_list;
  c_pci_nformation_upd_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EndpointIPAddressAndPort-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using endpoint_ip_address_and_port_ext_ies_o = protocol_ext_empty_o;

using endpoint_ip_address_and_port_ext_ies_container = protocol_ext_container_empty_l;

// EndpointIPAddressAndPort ::= SEQUENCE
struct endpoint_ip_address_and_port_s {
  bool                                           ie_exts_present = false;
  bounded_bitstring<1, 160, true, true>          endpoint_ip_address;
  fixed_bitstring<16, false, true>               port_num;
  endpoint_ip_address_and_port_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CPTransportLayerInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct cp_transport_layer_info_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { endpoint_ip_address_and_port, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::endpoint_ip_address_and_port; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    endpoint_ip_address_and_port_s&       endpoint_ip_address_and_port() { return c; }
    const endpoint_ip_address_and_port_s& endpoint_ip_address_and_port() const { return c; }

  private:
    endpoint_ip_address_and_port_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// CPTransportLayerInformation ::= CHOICE
struct cp_transport_layer_info_c {
  struct types_opts {
    enum options { endpoint_ip_address, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cp_transport_layer_info_c() = default;
  cp_transport_layer_info_c(const cp_transport_layer_info_c& other);
  cp_transport_layer_info_c& operator=(const cp_transport_layer_info_c& other);
  ~cp_transport_layer_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  bounded_bitstring<1, 160, true, true>& endpoint_ip_address()
  {
    assert_choice_type(types::endpoint_ip_address, type_, "CPTransportLayerInformation");
    return c.get<bounded_bitstring<1, 160, true, true>>();
  }
  protocol_ie_single_container_s<cp_transport_layer_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "CPTransportLayerInformation");
    return c.get<protocol_ie_single_container_s<cp_transport_layer_info_ext_ies_o>>();
  }
  const bounded_bitstring<1, 160, true, true>& endpoint_ip_address() const
  {
    assert_choice_type(types::endpoint_ip_address, type_, "CPTransportLayerInformation");
    return c.get<bounded_bitstring<1, 160, true, true>>();
  }
  const protocol_ie_single_container_s<cp_transport_layer_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "CPTransportLayerInformation");
    return c.get<protocol_ie_single_container_s<cp_transport_layer_info_ext_ies_o>>();
  }
  bounded_bitstring<1, 160, true, true>&                             set_endpoint_ip_address();
  protocol_ie_single_container_s<cp_transport_layer_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<bounded_bitstring<1, 160, true, true>,
                  protocol_ie_single_container_s<cp_transport_layer_info_ext_ies_o>>
      c;

  void destroy_();
};

// CSI-RSTransmissionIndication ::= ENUMERATED
struct csi_rs_tx_ind_opts {
  enum options { activ, deactiv, /*...*/ nulltype } value;

  const char* to_string() const;
};
using csi_rs_tx_ind_e = enumerated<csi_rs_tx_ind_opts, true>;

// CandidateRelayUEInfoItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using candidate_relay_ue_info_item_ext_ies_o = protocol_ext_empty_o;

using candidate_relay_ue_info_item_ext_ies_container = protocol_ext_container_empty_l;

// CandidateRelayUEInfoItem ::= SEQUENCE
struct candidate_relay_ue_info_item_s {
  bool                                           ie_exts_present = false;
  fixed_bitstring<24, false, true>               candidate_relay_ue_id;
  candidate_relay_ue_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CandidateRelayUEInfoList ::= SEQUENCE (SIZE (1..32)) OF CandidateRelayUEInfoItem
using candidate_relay_ue_info_list_l = dyn_array<candidate_relay_ue_info_item_s>;

// CapabilityForBATAdaptation ::= ENUMERATED
struct cap_for_bat_adaptation_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cap_for_bat_adaptation_e = enumerated<cap_for_bat_adaptation_opts, true>;

// SSBAreaCapacityValue-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ssb_area_capacity_value_list_item_ext_ies_o = protocol_ext_empty_o;

using ssb_area_capacity_value_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBAreaCapacityValue-List-Item ::= SEQUENCE
struct ssb_area_capacity_value_list_item_s {
  bool                                                ie_exts_present         = false;
  uint8_t                                             ssb_idx                 = 0;
  uint8_t                                             ssb_area_capacity_value = 0;
  ssb_area_capacity_value_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBAreaCapacityValue-List ::= SEQUENCE (SIZE (1..64)) OF SSBAreaCapacityValue-List-Item
using ssb_area_capacity_value_list_l = dyn_array<ssb_area_capacity_value_list_item_s>;

// CapacityValueInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using capacity_value_info_ext_ies_o = protocol_ext_empty_o;

using capacity_value_info_ext_ies_container = protocol_ext_container_empty_l;

// CapacityValueInfo ::= SEQUENCE
struct capacity_value_info_s {
  bool                                  ie_ext_present = false;
  uint8_t                               capacity_value = 0;
  ssb_area_capacity_value_list_l        ssb_area_capacity_value_list;
  capacity_value_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Cell-Type-Choice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using cell_type_choice_ext_ies_o = protocol_ies_empty_o;

// Cell-Type-Choice ::= CHOICE
struct cell_type_choice_c {
  struct types_opts {
    enum options { ng_ran_e_utra, ng_ran_nr, e_utran, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cell_type_choice_c() = default;
  cell_type_choice_c(const cell_type_choice_c& other);
  cell_type_choice_c& operator=(const cell_type_choice_c& other);
  ~cell_type_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<28, false, true>& ng_ran_e_utra()
  {
    assert_choice_type(types::ng_ran_e_utra, type_, "Cell-Type-Choice");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  fixed_bitstring<36, false, true>& ng_ran_nr()
  {
    assert_choice_type(types::ng_ran_nr, type_, "Cell-Type-Choice");
    return c.get<fixed_bitstring<36, false, true>>();
  }
  fixed_bitstring<28, false, true>& e_utran()
  {
    assert_choice_type(types::e_utran, type_, "Cell-Type-Choice");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  protocol_ie_single_container_s<cell_type_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "Cell-Type-Choice");
    return c.get<protocol_ie_single_container_s<cell_type_choice_ext_ies_o>>();
  }
  const fixed_bitstring<28, false, true>& ng_ran_e_utra() const
  {
    assert_choice_type(types::ng_ran_e_utra, type_, "Cell-Type-Choice");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  const fixed_bitstring<36, false, true>& ng_ran_nr() const
  {
    assert_choice_type(types::ng_ran_nr, type_, "Cell-Type-Choice");
    return c.get<fixed_bitstring<36, false, true>>();
  }
  const fixed_bitstring<28, false, true>& e_utran() const
  {
    assert_choice_type(types::e_utran, type_, "Cell-Type-Choice");
    return c.get<fixed_bitstring<28, false, true>>();
  }
  const protocol_ie_single_container_s<cell_type_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "Cell-Type-Choice");
    return c.get<protocol_ie_single_container_s<cell_type_choice_ext_ies_o>>();
  }
  fixed_bitstring<28, false, true>&                           set_ng_ran_e_utra();
  fixed_bitstring<36, false, true>&                           set_ng_ran_nr();
  fixed_bitstring<28, false, true>&                           set_e_utran();
  protocol_ie_single_container_s<cell_type_choice_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                         type_;
  choice_buffer_t<fixed_bitstring<36, false, true>, protocol_ie_single_container_s<cell_type_choice_ext_ies_o>> c;

  void destroy_();
};

// CellAssistanceInfo-EUTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using cell_assist_info_eutra_ext_ies_o = protocol_ies_empty_o;

// CellAssistanceInfo-EUTRA ::= CHOICE
struct cell_assist_info_eutra_c {
  using limited_eutra_list_l_ = dyn_array<e_utra_cgi_s>;
  struct full_list_opts {
    enum options { all_served_cells_e_utra, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using full_list_e_ = enumerated<full_list_opts, true>;
  struct types_opts {
    enum options { limited_eutra_list, full_list, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cell_assist_info_eutra_c() = default;
  cell_assist_info_eutra_c(const cell_assist_info_eutra_c& other);
  cell_assist_info_eutra_c& operator=(const cell_assist_info_eutra_c& other);
  ~cell_assist_info_eutra_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  limited_eutra_list_l_& limited_eutra_list()
  {
    assert_choice_type(types::limited_eutra_list, type_, "CellAssistanceInfo-EUTRA");
    return c.get<limited_eutra_list_l_>();
  }
  full_list_e_& full_list()
  {
    assert_choice_type(types::full_list, type_, "CellAssistanceInfo-EUTRA");
    return c.get<full_list_e_>();
  }
  protocol_ie_single_container_s<cell_assist_info_eutra_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "CellAssistanceInfo-EUTRA");
    return c.get<protocol_ie_single_container_s<cell_assist_info_eutra_ext_ies_o>>();
  }
  const limited_eutra_list_l_& limited_eutra_list() const
  {
    assert_choice_type(types::limited_eutra_list, type_, "CellAssistanceInfo-EUTRA");
    return c.get<limited_eutra_list_l_>();
  }
  const full_list_e_& full_list() const
  {
    assert_choice_type(types::full_list, type_, "CellAssistanceInfo-EUTRA");
    return c.get<full_list_e_>();
  }
  const protocol_ie_single_container_s<cell_assist_info_eutra_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "CellAssistanceInfo-EUTRA");
    return c.get<protocol_ie_single_container_s<cell_assist_info_eutra_ext_ies_o>>();
  }
  limited_eutra_list_l_&                                            set_limited_eutra_list();
  full_list_e_&                                                     set_full_list();
  protocol_ie_single_container_s<cell_assist_info_eutra_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                    type_;
  choice_buffer_t<limited_eutra_list_l_, protocol_ie_single_container_s<cell_assist_info_eutra_ext_ies_o>> c;

  void destroy_();
};

// CellAndCapacityAssistanceInfo-EUTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_and_capacity_assist_info_eutra_ext_ies_o = protocol_ext_empty_o;

using cell_and_capacity_assist_info_eutra_ext_ies_container = protocol_ext_container_empty_l;

// CellAndCapacityAssistanceInfo-EUTRA ::= SEQUENCE
struct cell_and_capacity_assist_info_eutra_s {
  bool                                                  max_cell_list_size_present     = false;
  bool                                                  cell_assist_info_eutra_present = false;
  bool                                                  ie_exts_present                = false;
  uint32_t                                              max_cell_list_size             = 1;
  cell_assist_info_eutra_c                              cell_assist_info_eutra;
  cell_and_capacity_assist_info_eutra_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellAssistanceInfo-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using cell_assist_info_nr_ext_ies_o = protocol_ies_empty_o;

// CellAssistanceInfo-NR ::= CHOICE
struct cell_assist_info_nr_c {
  using limited_nr_list_l_ = dyn_array<nr_cgi_s>;
  struct full_list_opts {
    enum options { all_served_cells_nr, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using full_list_e_ = enumerated<full_list_opts, true>;
  struct types_opts {
    enum options { limited_nr_list, full_list, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  cell_assist_info_nr_c() = default;
  cell_assist_info_nr_c(const cell_assist_info_nr_c& other);
  cell_assist_info_nr_c& operator=(const cell_assist_info_nr_c& other);
  ~cell_assist_info_nr_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  limited_nr_list_l_& limited_nr_list()
  {
    assert_choice_type(types::limited_nr_list, type_, "CellAssistanceInfo-NR");
    return c.get<limited_nr_list_l_>();
  }
  full_list_e_& full_list()
  {
    assert_choice_type(types::full_list, type_, "CellAssistanceInfo-NR");
    return c.get<full_list_e_>();
  }
  protocol_ie_single_container_s<cell_assist_info_nr_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "CellAssistanceInfo-NR");
    return c.get<protocol_ie_single_container_s<cell_assist_info_nr_ext_ies_o>>();
  }
  const limited_nr_list_l_& limited_nr_list() const
  {
    assert_choice_type(types::limited_nr_list, type_, "CellAssistanceInfo-NR");
    return c.get<limited_nr_list_l_>();
  }
  const full_list_e_& full_list() const
  {
    assert_choice_type(types::full_list, type_, "CellAssistanceInfo-NR");
    return c.get<full_list_e_>();
  }
  const protocol_ie_single_container_s<cell_assist_info_nr_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "CellAssistanceInfo-NR");
    return c.get<protocol_ie_single_container_s<cell_assist_info_nr_ext_ies_o>>();
  }
  limited_nr_list_l_&                                            set_limited_nr_list();
  full_list_e_&                                                  set_full_list();
  protocol_ie_single_container_s<cell_assist_info_nr_ext_ies_o>& set_choice_ext();

private:
  types                                                                                              type_;
  choice_buffer_t<limited_nr_list_l_, protocol_ie_single_container_s<cell_assist_info_nr_ext_ies_o>> c;

  void destroy_();
};

// CellAndCapacityAssistanceInfo-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_and_capacity_assist_info_nr_ext_ies_o = protocol_ext_empty_o;

using cell_and_capacity_assist_info_nr_ext_ies_container = protocol_ext_container_empty_l;

// CellAndCapacityAssistanceInfo-NR ::= SEQUENCE
struct cell_and_capacity_assist_info_nr_s {
  bool                                               max_cell_list_size_present  = false;
  bool                                               cell_assist_info_nr_present = false;
  bool                                               ie_exts_present             = false;
  uint32_t                                           max_cell_list_size          = 1;
  cell_assist_info_nr_c                              cell_assist_info_nr;
  cell_and_capacity_assist_info_nr_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PredictedTrajectoryNGRANCellInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using predicted_trajectory_ngran_cell_info_ext_ies_o = protocol_ext_empty_o;

using predicted_trajectory_ngran_cell_info_ext_ies_container = protocol_ext_container_empty_l;

// PredictedTrajectoryNGRANCellInfo ::= SEQUENCE
struct predicted_trajectory_ngran_cell_info_s {
  bool                                                   predicted_time_ue_stays_in_cell_present = false;
  bool                                                   ie_exts_present                         = false;
  global_ng_ran_cell_id_s                                global_ng_ran_cell_id;
  uint16_t                                               predicted_time_ue_stays_in_cell = 0;
  predicted_trajectory_ngran_cell_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PredictedTrajectoryCellInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using predicted_trajectory_cell_info_ext_ies_o = protocol_ies_empty_o;

// PredictedTrajectoryCellInfo ::= CHOICE
struct predicted_trajectory_cell_info_c {
  struct types_opts {
    enum options { ng_ran_cell_predicted, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  predicted_trajectory_cell_info_c() = default;
  predicted_trajectory_cell_info_c(const predicted_trajectory_cell_info_c& other);
  predicted_trajectory_cell_info_c& operator=(const predicted_trajectory_cell_info_c& other);
  ~predicted_trajectory_cell_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  predicted_trajectory_ngran_cell_info_s& ng_ran_cell_predicted()
  {
    assert_choice_type(types::ng_ran_cell_predicted, type_, "PredictedTrajectoryCellInfo");
    return c.get<predicted_trajectory_ngran_cell_info_s>();
  }
  protocol_ie_single_container_s<predicted_trajectory_cell_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PredictedTrajectoryCellInfo");
    return c.get<protocol_ie_single_container_s<predicted_trajectory_cell_info_ext_ies_o>>();
  }
  const predicted_trajectory_ngran_cell_info_s& ng_ran_cell_predicted() const
  {
    assert_choice_type(types::ng_ran_cell_predicted, type_, "PredictedTrajectoryCellInfo");
    return c.get<predicted_trajectory_ngran_cell_info_s>();
  }
  const protocol_ie_single_container_s<predicted_trajectory_cell_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PredictedTrajectoryCellInfo");
    return c.get<protocol_ie_single_container_s<predicted_trajectory_cell_info_ext_ies_o>>();
  }
  predicted_trajectory_ngran_cell_info_s&                                   set_ng_ran_cell_predicted();
  protocol_ie_single_container_s<predicted_trajectory_cell_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<predicted_trajectory_ngran_cell_info_s,
                  protocol_ie_single_container_s<predicted_trajectory_cell_info_ext_ies_o>>
      c;

  void destroy_();
};

// PredictedUETrajectory-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using predicted_ue_trajectory_item_ext_ies_o = protocol_ext_empty_o;

using predicted_ue_trajectory_item_ext_ies_container = protocol_ext_container_empty_l;

// PredictedUETrajectory-Item ::= SEQUENCE
struct predicted_ue_trajectory_item_s {
  bool                                           ie_exts_present = false;
  predicted_trajectory_cell_info_c               predictedtrajectory_cell_info;
  predicted_ue_trajectory_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellBasedUETrajectoryPrediction ::= SEQUENCE (SIZE (1..16)) OF PredictedUETrajectory-Item
using cell_based_ue_trajectory_prediction_l = dyn_array<predicted_ue_trajectory_item_s>;

// CellDeploymentStatusIndicator ::= ENUMERATED
struct cell_deployment_status_ind_opts {
  enum options { pre_change_notif, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cell_deployment_status_ind_e = enumerated<cell_deployment_status_ind_opts, true>;

// NG-eNB-RadioResourceStatus-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ng_enb_rr_status_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ng_enb_rr_status_ext_ies_container {
  bool    dl_sched_pdcch_cce_usage_present = false;
  bool    ul_sched_pdcch_cce_usage_present = false;
  uint8_t dl_sched_pdcch_cce_usage;
  uint8_t ul_sched_pdcch_cce_usage;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NG-eNB-RadioResourceStatus ::= SEQUENCE
struct ng_enb_rr_status_s {
  bool                               ie_exts_present      = false;
  uint8_t                            dl_gbr_prb_usage     = 0;
  uint8_t                            ul_gbr_prb_usage     = 0;
  uint8_t                            dl_non_gbr_prb_usage = 0;
  uint8_t                            ul_non_gbr_prb_usage = 0;
  uint8_t                            dl_total_prb_usage   = 0;
  uint8_t                            ul_total_prb_usage   = 0;
  ng_enb_rr_status_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBAreaRadioResourceStatus-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ssb_area_rr_status_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ssb_area_rr_status_list_item_ext_ies_container {
  bool    dl_sched_pdcch_cce_usage_present = false;
  bool    ul_sched_pdcch_cce_usage_present = false;
  uint8_t dl_sched_pdcch_cce_usage;
  uint8_t ul_sched_pdcch_cce_usage;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBAreaRadioResourceStatus-List-Item ::= SEQUENCE
struct ssb_area_rr_status_list_item_s {
  bool                                           ie_exts_present                = false;
  uint8_t                                        ssb_idx                        = 0;
  uint8_t                                        ssb_area_dl_gbr_prb_usage      = 0;
  uint8_t                                        ssb_area_ul_gbr_prb_usage      = 0;
  uint8_t                                        ssb_area_d_l_non_gbr_prb_usage = 0;
  uint8_t                                        ssb_area_u_l_non_gbr_prb_usage = 0;
  uint8_t                                        ssb_area_d_l_total_prb_usage   = 0;
  uint8_t                                        ssb_area_u_l_total_prb_usage   = 0;
  ssb_area_rr_status_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBAreaRadioResourceStatus-List ::= SEQUENCE (SIZE (1..64)) OF SSBAreaRadioResourceStatus-List-Item
using ssb_area_rr_status_list_l = dyn_array<ssb_area_rr_status_list_item_s>;

// SNSSAIRadioResourceStatus-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using snssai_rr_status_item_ext_ies_o = protocol_ext_empty_o;

using snssai_rr_status_item_ext_ies_container = protocol_ext_container_empty_l;

// SNSSAIRadioResourceStatus-Item ::= SEQUENCE
struct snssai_rr_status_item_s {
  bool                                    ie_exts_present = false;
  s_nssai_s                               snssai;
  uint8_t                                 slice_dl_gbr_prb_usage     = 0;
  uint8_t                                 slice_ul_gbr_prb_usage     = 0;
  uint8_t                                 slice_dl_non_gbr_prb_usage = 0;
  uint8_t                                 slice_ul_non_gbr_prb_usage = 0;
  uint8_t                                 slice_dl_total_prb_alloc   = 0;
  uint8_t                                 slice_ul_total_prb_alloc   = 0;
  snssai_rr_status_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNSSAIRadioResourceStatus-List ::= SEQUENCE (SIZE (1..1024)) OF SNSSAIRadioResourceStatus-Item
using snssai_rr_status_list_l = dyn_array<snssai_rr_status_item_s>;

// SliceRadioResourceStatus-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using slice_rr_status_item_ext_ies_o = protocol_ext_empty_o;

using slice_rr_status_item_ext_ies_container = protocol_ext_container_empty_l;

// SliceRadioResourceStatus-Item ::= SEQUENCE
struct slice_rr_status_item_s {
  bool                                   ie_exts_present = false;
  fixed_octstring<3, true>               plmn_id;
  snssai_rr_status_list_l                snssai_rr_status_list;
  slice_rr_status_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceRadioResourceStatus-List ::= SEQUENCE (SIZE (1..12)) OF SliceRadioResourceStatus-Item
using slice_rr_status_list_l = dyn_array<slice_rr_status_item_s>;

// MIMOPRBusageInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// GNB-RadioResourceStatus-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct gnb_rr_status_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct gnb_rr_status_ext_ies_container {
  bool                   slice_rr_status_list_present = false;
  bool                   mimo_prb_usage_info_present  = false;
  slice_rr_status_list_l slice_rr_status_list;
  mimo_prb_usage_info_s  mimo_prb_usage_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GNB-RadioResourceStatus ::= SEQUENCE
struct gnb_rr_status_s {
  bool                            ie_exts_present = false;
  ssb_area_rr_status_list_l       ssb_area_rr_status_list;
  gnb_rr_status_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RadioResourceStatus-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using rr_status_ext_ies_o = protocol_ies_empty_o;

// RadioResourceStatus ::= CHOICE
struct rr_status_c {
  struct types_opts {
    enum options { ng_enb_rr_status, gnb_rr_status, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  rr_status_c() = default;
  rr_status_c(const rr_status_c& other);
  rr_status_c& operator=(const rr_status_c& other);
  ~rr_status_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ng_enb_rr_status_s& ng_enb_rr_status()
  {
    assert_choice_type(types::ng_enb_rr_status, type_, "RadioResourceStatus");
    return c.get<ng_enb_rr_status_s>();
  }
  gnb_rr_status_s& gnb_rr_status()
  {
    assert_choice_type(types::gnb_rr_status, type_, "RadioResourceStatus");
    return c.get<gnb_rr_status_s>();
  }
  protocol_ie_single_container_s<rr_status_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "RadioResourceStatus");
    return c.get<protocol_ie_single_container_s<rr_status_ext_ies_o>>();
  }
  const ng_enb_rr_status_s& ng_enb_rr_status() const
  {
    assert_choice_type(types::ng_enb_rr_status, type_, "RadioResourceStatus");
    return c.get<ng_enb_rr_status_s>();
  }
  const gnb_rr_status_s& gnb_rr_status() const
  {
    assert_choice_type(types::gnb_rr_status, type_, "RadioResourceStatus");
    return c.get<gnb_rr_status_s>();
  }
  const protocol_ie_single_container_s<rr_status_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "RadioResourceStatus");
    return c.get<protocol_ie_single_container_s<rr_status_ext_ies_o>>();
  }
  ng_enb_rr_status_s&                                  set_ng_enb_rr_status();
  gnb_rr_status_s&                                     set_gnb_rr_status();
  protocol_ie_single_container_s<rr_status_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                     type_;
  choice_buffer_t<gnb_rr_status_s, ng_enb_rr_status_s, protocol_ie_single_container_s<rr_status_ext_ies_o>> c;

  void destroy_();
};

// RRCConnections-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using rrc_conns_ext_ies_o = protocol_ext_empty_o;

using rrc_conns_ext_ies_container = protocol_ext_container_empty_l;

// RRCConnections ::= SEQUENCE
struct rrc_conns_s {
  bool                        ie_exts_present                   = false;
  uint32_t                    noof_rrc_conns                    = 1;
  uint8_t                     available_rrc_conn_capacity_value = 0;
  rrc_conns_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellInfoResultForDataCollection-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_info_result_for_data_collection_item_ext_ies_o = protocol_ext_empty_o;

using cell_info_result_for_data_collection_item_ext_ies_container = protocol_ext_container_empty_l;

// CellInfoResultForDataCollection-Item ::= SEQUENCE
struct cell_info_result_for_data_collection_item_s {
  bool                                                        predicted_rr_status_present        = false;
  bool                                                        predicted_numof_active_ues_present = false;
  bool                                                        predicted_rrc_conns_present        = false;
  bool                                                        ie_exts_present                    = false;
  global_ng_ran_cell_id_s                                     cell_id;
  rr_status_c                                                 predicted_rr_status;
  uint32_t                                                    predicted_numof_active_ues = 0;
  rrc_conns_s                                                 predicted_rrc_conns;
  cell_info_result_for_data_collection_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellMeasurementFailureCause-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_meas_fail_cause_item_ext_ies_o = protocol_ext_empty_o;

using cell_meas_fail_cause_item_ext_ies_container = protocol_ext_container_empty_l;

// CellMeasurementFailureCause-Item ::= SEQUENCE
struct cell_meas_fail_cause_item_s {
  bool                                        ie_exts_present = false;
  fixed_bitstring<32, false, true>            cellmeas_failed_report_characteristics;
  cause_c                                     cause;
  cell_meas_fail_cause_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellMeasurementFailureCause-List ::= SEQUENCE (SIZE (1..124)) OF CellMeasurementFailureCause-Item
using cell_meas_fail_cause_list_l = dyn_array<cell_meas_fail_cause_item_s>;

// CellMeasurementInitiationResult-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_meas_initiation_result_item_ext_ies_o = protocol_ext_empty_o;

using cell_meas_initiation_result_item_ext_ies_container = protocol_ext_container_empty_l;

// CellMeasurementInitiationResult-Item ::= SEQUENCE
struct cell_meas_initiation_result_item_s {
  bool                                               ie_exts_present = false;
  global_ng_ran_cell_id_s                            cell_id;
  cell_meas_fail_cause_list_l                        cell_meas_fail_cause_list;
  cell_meas_initiation_result_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellMeasurementInitiationResult-List ::= SEQUENCE (SIZE (1..16384)) OF CellMeasurementInitiationResult-Item
using cell_meas_initiation_result_list_l = dyn_array<cell_meas_initiation_result_item_s>;

// TNLCapacityIndicator-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tnl_capacity_ind_ext_ies_o = protocol_ext_empty_o;

using tnl_capacity_ind_ext_ies_container = protocol_ext_container_empty_l;

// TNLCapacityIndicator ::= SEQUENCE
struct tnl_capacity_ind_s {
  bool                               ie_exts_present           = false;
  uint32_t                           dl_tnl_offered_capacity   = 1;
  uint16_t                           dl_tnl_available_capacity = 1;
  uint32_t                           ul_tnl_offered_capacity   = 1;
  uint16_t                           ul_tnl_available_capacity = 1;
  tnl_capacity_ind_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CompositeAvailableCapacity-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using composite_available_capacity_ext_ies_o = protocol_ext_empty_o;

using composite_available_capacity_ext_ies_container = protocol_ext_container_empty_l;

// CompositeAvailableCapacity ::= SEQUENCE
struct composite_available_capacity_s {
  bool                                           cell_capacity_class_value_present = false;
  bool                                           ie_exts_present                   = false;
  uint16_t                                       cell_capacity_class_value         = 1;
  capacity_value_info_s                          capacity_value_info;
  composite_available_capacity_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CompositeAvailableCapacityGroup-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct composite_available_capacity_group_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct composite_available_capacity_group_ext_ies_container {
  bool                           composite_available_capacity_supplementary_ul_present = false;
  composite_available_capacity_s composite_available_capacity_supplementary_ul;

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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNSSAIAvailableCapacity-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using snssai_available_capacity_item_ext_ies_o = protocol_ext_empty_o;

using snssai_available_capacity_item_ext_ies_container = protocol_ext_container_empty_l;

// SNSSAIAvailableCapacity-Item ::= SEQUENCE
struct snssai_available_capacity_item_s {
  bool                                             ie_exts_present = false;
  s_nssai_s                                        snssai;
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

// SliceAvailableCapacity-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using slice_available_capacity_item_ext_ies_o = protocol_ext_empty_o;

using slice_available_capacity_item_ext_ies_container = protocol_ext_container_empty_l;

// SliceAvailableCapacity-Item ::= SEQUENCE
struct slice_available_capacity_item_s {
  bool                                            ie_exts_present = false;
  fixed_octstring<3, true>                        plmn_id;
  snssai_available_capacity_list_l                snssai_available_capacity_list;
  slice_available_capacity_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceAvailableCapacity ::= SEQUENCE (SIZE (1..12)) OF SliceAvailableCapacity-Item
using slice_available_capacity_l = dyn_array<slice_available_capacity_item_s>;

// RadioResourceStatusNR-U-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// NR-U-Channel-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct nr_u_ch_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct nr_u_ch_item_ext_ies_container {
  bool             ch_occupancy_time_percentage_ul_present = false;
  bool             energy_detection_thres_ul_present       = false;
  bool             rr_status_nr_u_present                  = false;
  uint16_t         ch_occupancy_time_percentage_ul;
  int16_t          energy_detection_thres_ul;
  rr_status_nr_u_s rr_status_nr_u;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-Channel-Item ::= SEQUENCE
struct nr_u_ch_item_s {
  bool                           ie_ext_present                  = false;
  uint8_t                        nr_u_ch_id                      = 1;
  uint16_t                       ch_occupancy_time_percentage_dl = 0;
  int16_t                        energy_detection_thres_dl       = -100;
  nr_u_ch_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-Channel-List ::= SEQUENCE (SIZE (1..16)) OF NR-U-Channel-Item
using nr_u_ch_list_l = dyn_array<nr_u_ch_item_s>;

// CellMeasurementResult-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// CellMeasurementResult-Item ::= SEQUENCE
struct cell_meas_result_item_s {
  bool                                    rr_status_present                          = false;
  bool                                    tnl_capacity_ind_present                   = false;
  bool                                    composite_available_capacity_group_present = false;
  bool                                    numof_active_ues_present                   = false;
  bool                                    rrc_conns_present                          = false;
  bool                                    ie_exts_present                            = false;
  global_ng_ran_cell_id_s                 cell_id;
  rr_status_c                             rr_status;
  tnl_capacity_ind_s                      tnl_capacity_ind;
  composite_available_capacity_group_s    composite_available_capacity_group;
  slice_available_capacity_l              slice_available_capacity;
  uint32_t                                numof_active_ues = 0;
  rrc_conns_s                             rrc_conns;
  cell_meas_result_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellMeasurementResult ::= SEQUENCE (SIZE (1..16384)) OF CellMeasurementResult-Item
using cell_meas_result_l = dyn_array<cell_meas_result_item_s>;

// CellMeasurementResultForDataCollection-List ::= SEQUENCE (SIZE (1..16384)) OF CellInfoResultForDataCollection-Item
using cell_meas_result_for_data_collection_list_l = dyn_array<cell_info_result_for_data_collection_item_s>;

// GlobalCell-ID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using global_cell_id_ext_ies_o = protocol_ext_empty_o;

using global_cell_id_ext_ies_container = protocol_ext_container_empty_l;

// GlobalCell-ID ::= SEQUENCE
struct global_cell_id_s {
  bool                             ie_exts_present = false;
  fixed_octstring<3, true>         plmn_id;
  cell_type_choice_c               cell_type;
  global_cell_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ReplacingCells-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using replacing_cells_item_ext_ies_o = protocol_ext_empty_o;

using replacing_cells_item_ext_ies_container = protocol_ext_container_empty_l;

// ReplacingCells-Item ::= SEQUENCE
struct replacing_cells_item_s {
  bool                                   ie_exts_present = false;
  global_cell_id_s                       global_ng_ran_cell_id;
  replacing_cells_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ReplacingCells ::= SEQUENCE (SIZE (0..16384)) OF ReplacingCells-Item
using replacing_cells_l = dyn_array<replacing_cells_item_s>;

// CellReplacingInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_replacing_info_ext_ies_o = protocol_ext_empty_o;

using cell_replacing_info_ext_ies_container = protocol_ext_container_empty_l;

// CellReplacingInfo ::= SEQUENCE
struct cell_replacing_info_s {
  bool                                  ie_exts_present = false;
  replacing_cells_l                     replacing_cells;
  cell_replacing_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBToReport-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ssb_to_report_list_item_ext_ies_o = protocol_ext_empty_o;

using ssb_to_report_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBToReport-List-Item ::= SEQUENCE
struct ssb_to_report_list_item_s {
  bool                                      ie_exts_present = false;
  uint8_t                                   ssb_idx         = 0;
  ssb_to_report_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBToReport-List ::= SEQUENCE (SIZE (1..64)) OF SSBToReport-List-Item
using ssb_to_report_list_l = dyn_array<ssb_to_report_list_item_s>;

// SNSSAI-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using snssai_item_ext_ies_o = protocol_ext_empty_o;

using snssai_item_ext_ies_container = protocol_ext_container_empty_l;

// SNSSAI-Item ::= SEQUENCE
struct snssai_item_s {
  bool                          ie_exts_present = false;
  s_nssai_s                     snssai;
  snssai_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNSSAI-list ::= SEQUENCE (SIZE (1..1024)) OF SNSSAI-Item
using snssai_list_l = dyn_array<snssai_item_s>;

// SliceToReport-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using slice_to_report_list_item_ext_ies_o = protocol_ext_empty_o;

using slice_to_report_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SliceToReport-List-Item ::= SEQUENCE
struct slice_to_report_list_item_s {
  bool                                        ie_exts_present = false;
  fixed_octstring<3, true>                    plmn_id;
  snssai_list_l                               snssai_list;
  slice_to_report_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SliceToReport-List ::= SEQUENCE (SIZE (1..12)) OF SliceToReport-List-Item
using slice_to_report_list_l = dyn_array<slice_to_report_list_item_s>;

// CellToReport-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_to_report_item_ext_ies_o = protocol_ext_empty_o;

using cell_to_report_item_ext_ies_container = protocol_ext_container_empty_l;

// CellToReport-Item ::= SEQUENCE
struct cell_to_report_item_s {
  bool                                  ie_exts_present = false;
  global_ng_ran_cell_id_s               cell_id;
  ssb_to_report_list_l                  ssb_to_report_list;
  slice_to_report_list_l                slice_to_report_list;
  cell_to_report_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellToReport ::= SEQUENCE (SIZE (1..16384)) OF CellToReport-Item
using cell_to_report_l = dyn_array<cell_to_report_item_s>;

// CellToReportForDataCollection-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using cell_to_report_for_data_collection_item_ext_ies_o = protocol_ext_empty_o;

using cell_to_report_for_data_collection_item_ext_ies_container = protocol_ext_container_empty_l;

// CellToReportForDataCollection-Item ::= SEQUENCE
struct cell_to_report_for_data_collection_item_s {
  bool                                                      ie_exts_present = false;
  global_ng_ran_cell_id_s                                   cell_id;
  cell_to_report_for_data_collection_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CellToReportForDataCollection-List ::= SEQUENCE (SIZE (1..16384)) OF CellToReportForDataCollection-Item
using cell_to_report_for_data_collection_list_l = dyn_array<cell_to_report_for_data_collection_item_s>;

// ClockQualityAcceptanceCriteria-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using clock_quality_acceptance_criteria_ext_ies_o = protocol_ext_empty_o;

using clock_quality_acceptance_criteria_ext_ies_container = protocol_ext_container_empty_l;

// ClockQualityAcceptanceCriteria ::= SEQUENCE
struct clock_quality_acceptance_criteria_s {
  struct traceableto_utc_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using traceableto_utc_e_ = enumerated<traceableto_utc_opts, true>;
  struct traceableto_gnss_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using traceableto_gnss_e_ = enumerated<traceableto_gnss_opts, true>;

  // member variables
  bool                                                synchronisation_state_present = false;
  bool                                                traceableto_utc_present       = false;
  bool                                                traceableto_gnss_present      = false;
  bool                                                clock_freq_stability_present  = false;
  bool                                                clock_accuracy_present        = false;
  bool                                                parent_time_source_present    = false;
  bool                                                ie_exts_present               = false;
  fixed_bitstring<8, true, true>                      synchronisation_state;
  traceableto_utc_e_                                  traceableto_utc;
  traceableto_gnss_e_                                 traceableto_gnss;
  fixed_bitstring<16, false, true>                    clock_freq_stability;
  uint32_t                                            clock_accuracy = 1;
  fixed_bitstring<16, true, true>                     parent_time_source;
  clock_quality_acceptance_criteria_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ClockQualityDetailLevel-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using clock_quality_detail_level_ext_ies_o = protocol_ies_empty_o;

// ClockQualityDetailLevel ::= CHOICE
struct clock_quality_detail_level_c {
  struct types_opts {
    enum options { clock_quality_metrics, acceptance_ind, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  clock_quality_detail_level_c() = default;
  clock_quality_detail_level_c(const clock_quality_detail_level_c& other);
  clock_quality_detail_level_c& operator=(const clock_quality_detail_level_c& other);
  ~clock_quality_detail_level_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  clock_quality_acceptance_criteria_s& acceptance_ind()
  {
    assert_choice_type(types::acceptance_ind, type_, "ClockQualityDetailLevel");
    return c.get<clock_quality_acceptance_criteria_s>();
  }
  protocol_ie_single_container_s<clock_quality_detail_level_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ClockQualityDetailLevel");
    return c.get<protocol_ie_single_container_s<clock_quality_detail_level_ext_ies_o>>();
  }
  const clock_quality_acceptance_criteria_s& acceptance_ind() const
  {
    assert_choice_type(types::acceptance_ind, type_, "ClockQualityDetailLevel");
    return c.get<clock_quality_acceptance_criteria_s>();
  }
  const protocol_ie_single_container_s<clock_quality_detail_level_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ClockQualityDetailLevel");
    return c.get<protocol_ie_single_container_s<clock_quality_detail_level_ext_ies_o>>();
  }
  void                                                                  set_clock_quality_metrics();
  clock_quality_acceptance_criteria_s&                                  set_acceptance_ind();
  protocol_ie_single_container_s<clock_quality_detail_level_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<clock_quality_acceptance_criteria_s,
                  protocol_ie_single_container_s<clock_quality_detail_level_ext_ies_o>>
      c;

  void destroy_();
};

// ClockQualityReportingControlInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using clock_quality_report_ctrl_info_ext_ies_o = protocol_ext_empty_o;

using clock_quality_report_ctrl_info_ext_ies_container = protocol_ext_container_empty_l;

// ClockQualityReportingControlInfo ::= SEQUENCE
struct clock_quality_report_ctrl_info_s {
  bool                                             ie_exts_present = false;
  clock_quality_detail_level_c                     clock_quality_detail_level;
  clock_quality_report_ctrl_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Conditional-Reconfig-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using conditional_recfg_item_ext_ies_o = protocol_ext_empty_o;

using conditional_recfg_item_ext_ies_container = protocol_ext_container_empty_l;

// Conditional-Reconfig-Item ::= SEQUENCE
struct conditional_recfg_item_s {
  bool                                     pscell_id_present = false;
  bool                                     ie_exts_present   = false;
  target_cgi_c                             pcell_id;
  nr_cgi_s                                 pscell_id;
  conditional_recfg_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Conditional-Reconfig-List ::= SEQUENCE (SIZE (1..8)) OF Conditional-Reconfig-Item
using conditional_recfg_list_l = dyn_array<conditional_recfg_item_s>;

// CongestionInformationRequest ::= ENUMERATED
struct congestion_info_request_opts {
  enum options { ul, dl, both, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using congestion_info_request_e = enumerated<congestion_info_request_opts, true>;

// Connectivity-Support-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using connect_support_ext_ies_o = protocol_ext_empty_o;

using connect_support_ext_ies_container = protocol_ext_container_empty_l;

// Connectivity-Support ::= SEQUENCE
struct connect_support_s {
  struct endc_support_opts {
    enum options { supported, not_supported, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using endc_support_e_ = enumerated<endc_support_opts, true>;

  // member variables
  bool                              ie_exts_present = false;
  endc_support_e_                   endc_support;
  connect_support_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSB-Coverage-Modification-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ssb_coverage_mod_list_item_ext_ies_o = protocol_ext_empty_o;

using ssb_coverage_mod_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SSB-Coverage-Modification-List-Item ::= SEQUENCE
struct ssb_coverage_mod_list_item_s {
  bool                                         ie_ext_present     = false;
  uint8_t                                      ssb_idx            = 0;
  uint8_t                                      ssb_coverage_state = 0;
  ssb_coverage_mod_list_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSB-Coverage-Modification-List ::= SEQUENCE (SIZE (0..64)) OF SSB-Coverage-Modification-List-Item
using ssb_coverage_mod_list_l = dyn_array<ssb_coverage_mod_list_item_s>;

// CoverageModificationCause ::= ENUMERATED
struct coverage_mod_cause_opts {
  enum options { coverage, cell_edge_capacity, /*...*/ network_energy_saving, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using coverage_mod_cause_e = enumerated<coverage_mod_cause_opts, true, 1>;

// Coverage-Modification-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct coverage_mod_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct coverage_mod_list_item_ext_ies_container {
  bool                 coverage_mod_cause_present = false;
  coverage_mod_cause_e coverage_mod_cause;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Coverage-Modification-List-Item ::= SEQUENCE
struct coverage_mod_list_item_s {
  bool                                     cell_deployment_status_ind_present = false;
  bool                                     cell_replacing_info_present        = false;
  bool                                     ie_ext_present                     = false;
  global_cell_id_s                         global_ng_ran_cell_id;
  uint8_t                                  cell_coverage_state = 0;
  cell_deployment_status_ind_e             cell_deployment_status_ind;
  cell_replacing_info_s                    cell_replacing_info;
  ssb_coverage_mod_list_l                  ssb_coverage_mod_list;
  coverage_mod_list_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Coverage-Modification-List ::= SEQUENCE (SIZE (0..16384)) OF Coverage-Modification-List-Item
using coverage_mod_list_l = dyn_array<coverage_mod_list_item_s>;

// TypeOfError ::= ENUMERATED
struct type_of_error_opts {
  enum options { not_understood, missing, /*...*/ nulltype } value;

  const char* to_string() const;
};
using type_of_error_e = enumerated<type_of_error_opts, true>;

// CriticalityDiagnostics-IE-List-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// CriticalityDiagnostics-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using crit_diagnostics_ext_ies_o = protocol_ext_empty_o;

using crit_diagnostics_ext_ies_container = protocol_ext_container_empty_l;

// CriticalityDiagnostics ::= SEQUENCE
struct crit_diagnostics_s {
  bool                               proc_code_present   = false;
  bool                               trigger_msg_present = false;
  bool                               proc_crit_present   = false;
  bool                               ie_exts_present     = false;
  uint16_t                           proc_code           = 0;
  trigger_msg_e                      trigger_msg;
  crit_e                             proc_crit;
  crit_diagnostics_ie_list_l         ies_crit_diagnostics;
  crit_diagnostics_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// CyclicPrefix-E-UTRA-DL ::= ENUMERATED
struct cp_e_utra_dl_opts {
  enum options { normal, extended, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cp_e_utra_dl_e = enumerated<cp_e_utra_dl_opts, true>;

// CyclicPrefix-E-UTRA-UL ::= ENUMERATED
struct cp_e_utra_ul_opts {
  enum options { normal, extended, /*...*/ nulltype } value;

  const char* to_string() const;
};
using cp_e_utra_ul_e = enumerated<cp_e_utra_ul_opts, true>;

// DAPSRequestInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using daps_request_info_ext_ies_o = protocol_ext_empty_o;

using daps_request_info_ext_ies_container = protocol_ext_container_empty_l;

// DAPSRequestInfo ::= SEQUENCE
struct daps_request_info_s {
  struct daps_ind_opts {
    enum options { daps_ho_required, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using daps_ind_e_ = enumerated<daps_ind_opts, true>;

  // member variables
  bool                                ie_exts_present = false;
  daps_ind_e_                         daps_ind;
  daps_request_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DAPSResponseInfo-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using daps_resp_info_item_ext_ies_o = protocol_ext_empty_o;

using daps_resp_info_item_ext_ies_container = protocol_ext_container_empty_l;

// DAPSResponseInfo-Item ::= SEQUENCE
struct daps_resp_info_item_s {
  struct daps_resp_ind_opts {
    enum options { daps_ho_accepted, daps_ho_not_accepted, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using daps_resp_ind_e_ = enumerated<daps_resp_ind_opts, true>;

  // member variables
  bool                                  ie_exts_present = false;
  uint8_t                               drb_id          = 1;
  daps_resp_ind_e_                      daps_resp_ind;
  daps_resp_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DAPSResponseInfo-List ::= SEQUENCE (SIZE (1..32)) OF DAPSResponseInfo-Item
using daps_resp_info_list_l = dyn_array<daps_resp_info_item_s>;

// DLCountChoice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using dl_count_choice_ext_ies_o = protocol_ies_empty_o;

// DLCountChoice ::= CHOICE
struct dl_count_choice_c {
  struct types_opts {
    enum options { count12bits, count18bits, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  dl_count_choice_c() = default;
  dl_count_choice_c(const dl_count_choice_c& other);
  dl_count_choice_c& operator=(const dl_count_choice_c& other);
  ~dl_count_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  count_pdcp_sn12_s& count12bits()
  {
    assert_choice_type(types::count12bits, type_, "DLCountChoice");
    return c.get<count_pdcp_sn12_s>();
  }
  count_pdcp_sn18_s& count18bits()
  {
    assert_choice_type(types::count18bits, type_, "DLCountChoice");
    return c.get<count_pdcp_sn18_s>();
  }
  protocol_ie_single_container_s<dl_count_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "DLCountChoice");
    return c.get<protocol_ie_single_container_s<dl_count_choice_ext_ies_o>>();
  }
  const count_pdcp_sn12_s& count12bits() const
  {
    assert_choice_type(types::count12bits, type_, "DLCountChoice");
    return c.get<count_pdcp_sn12_s>();
  }
  const count_pdcp_sn18_s& count18bits() const
  {
    assert_choice_type(types::count18bits, type_, "DLCountChoice");
    return c.get<count_pdcp_sn18_s>();
  }
  const protocol_ie_single_container_s<dl_count_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "DLCountChoice");
    return c.get<protocol_ie_single_container_s<dl_count_choice_ext_ies_o>>();
  }
  count_pdcp_sn12_s&                                         set_count12bits();
  count_pdcp_sn18_s&                                         set_count18bits();
  protocol_ie_single_container_s<dl_count_choice_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<count_pdcp_sn12_s, count_pdcp_sn18_s, protocol_ie_single_container_s<dl_count_choice_ext_ies_o>> c;

  void destroy_();
};

// DLF1Terminating-BHInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using dl_f1_terminating_bh_info_ext_ies_o = protocol_ext_empty_o;

using dl_f1_terminating_bh_info_ext_ies_container = protocol_ext_container_empty_l;

// DLF1Terminating-BHInfo ::= SEQUENCE
struct dl_f1_terminating_bh_info_s {
  bool                                        ie_exts_present = false;
  bap_routing_id_s                            egress_bap_routing_id;
  fixed_bitstring<16, false, true>            egress_bh_rlc_ch_id;
  dl_f1_terminating_bh_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLForwarding ::= ENUMERATED
struct dl_forwarding_opts {
  enum options { dl_forwarding_proposed, /*...*/ nulltype } value;

  const char* to_string() const;
};
using dl_forwarding_e = enumerated<dl_forwarding_opts, true>;

// DLLBTFailureInformationList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// IAB-QoS-Mapping-Information-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_qos_map_info_ext_ies_o = protocol_ext_empty_o;

using iab_qos_map_info_ext_ies_container = protocol_ext_container_empty_l;

// IAB-QoS-Mapping-Information ::= SEQUENCE
struct iab_qos_map_info_s {
  bool                               dscp_present       = false;
  bool                               flow_label_present = false;
  bool                               ie_exts_present    = false;
  fixed_bitstring<6, false, true>    dscp;
  fixed_bitstring<20, false, true>   flow_label;
  iab_qos_map_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DLNonF1Terminating-BHInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using dl_non_f1_terminating_bh_info_ext_ies_o = protocol_ext_empty_o;

using dl_non_f1_terminating_bh_info_ext_ies_container = protocol_ext_container_empty_l;

// DLNonF1Terminating-BHInfo ::= SEQUENCE
struct dl_non_f1_terminating_bh_info_s {
  bool                                            ie_exts_present = false;
  bap_routing_id_s                                ingress_bap_routing_id;
  fixed_bitstring<16, false, true>                ingress_bh_rlc_ch_id;
  fixed_bitstring<10, false, true>                priorhop_bap_address;
  iab_qos_map_info_s                              iabqos_map_info;
  dl_non_f1_terminating_bh_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
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

// DRB-List-withCause-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using drb_list_with_cause_item_ext_ies_o = protocol_ext_empty_o;

using drb_list_with_cause_item_ext_ies_container = protocol_ext_container_empty_l;

// DRB-List-withCause-Item ::= SEQUENCE
struct drb_list_with_cause_item_s {
  bool                                       rlc_mode_present = false;
  bool                                       ie_ext_present   = false;
  uint8_t                                    drb_id           = 1;
  cause_c                                    cause;
  rlc_mode_e                                 rlc_mode;
  drb_list_with_cause_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRB-List-withCause ::= SEQUENCE (SIZE (1..32)) OF DRB-List-withCause-Item
using drb_list_with_cause_l = dyn_array<drb_list_with_cause_item_s>;

// DRBBStatusTransfer12bitsSN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using drb_b_status_transfer12bits_sn_ext_ies_o = protocol_ext_empty_o;

using drb_b_status_transfer12bits_sn_ext_ies_container = protocol_ext_container_empty_l;

// DRBBStatusTransfer12bitsSN ::= SEQUENCE
struct drb_b_status_transfer12bits_sn_s {
  bool                                             receive_statusof_pdcp_sdu_present = false;
  bool                                             ie_ext_present                    = false;
  bounded_bitstring<1, 2048, false, true>          receive_statusof_pdcp_sdu;
  count_pdcp_sn12_s                                count_value;
  drb_b_status_transfer12bits_sn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBBStatusTransfer18bitsSN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using drb_b_status_transfer18bits_sn_ext_ies_o = protocol_ext_empty_o;

using drb_b_status_transfer18bits_sn_ext_ies_container = protocol_ext_container_empty_l;

// DRBBStatusTransfer18bitsSN ::= SEQUENCE
struct drb_b_status_transfer18bits_sn_s {
  bool                                             receive_statusof_pdcp_sdu_present = false;
  bool                                             ie_ext_present                    = false;
  bounded_bitstring<1, 131072, false, true>        receive_statusof_pdcp_sdu;
  count_pdcp_sn18_s                                count_value;
  drb_b_status_transfer18bits_sn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBBStatusTransferChoice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using drb_b_status_transfer_choice_ext_ies_o = protocol_ies_empty_o;

// DRBBStatusTransferChoice ::= CHOICE
struct drb_b_status_transfer_choice_c {
  struct types_opts {
    enum options { pdcp_sn_12bits, pdcp_sn_18bits, choice_ext, nulltype } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  drb_b_status_transfer_choice_c() = default;
  drb_b_status_transfer_choice_c(const drb_b_status_transfer_choice_c& other);
  drb_b_status_transfer_choice_c& operator=(const drb_b_status_transfer_choice_c& other);
  ~drb_b_status_transfer_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  drb_b_status_transfer12bits_sn_s& pdcp_sn_12bits()
  {
    assert_choice_type(types::pdcp_sn_12bits, type_, "DRBBStatusTransferChoice");
    return c.get<drb_b_status_transfer12bits_sn_s>();
  }
  drb_b_status_transfer18bits_sn_s& pdcp_sn_18bits()
  {
    assert_choice_type(types::pdcp_sn_18bits, type_, "DRBBStatusTransferChoice");
    return c.get<drb_b_status_transfer18bits_sn_s>();
  }
  protocol_ie_single_container_s<drb_b_status_transfer_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "DRBBStatusTransferChoice");
    return c.get<protocol_ie_single_container_s<drb_b_status_transfer_choice_ext_ies_o>>();
  }
  const drb_b_status_transfer12bits_sn_s& pdcp_sn_12bits() const
  {
    assert_choice_type(types::pdcp_sn_12bits, type_, "DRBBStatusTransferChoice");
    return c.get<drb_b_status_transfer12bits_sn_s>();
  }
  const drb_b_status_transfer18bits_sn_s& pdcp_sn_18bits() const
  {
    assert_choice_type(types::pdcp_sn_18bits, type_, "DRBBStatusTransferChoice");
    return c.get<drb_b_status_transfer18bits_sn_s>();
  }
  const protocol_ie_single_container_s<drb_b_status_transfer_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "DRBBStatusTransferChoice");
    return c.get<protocol_ie_single_container_s<drb_b_status_transfer_choice_ext_ies_o>>();
  }
  drb_b_status_transfer12bits_sn_s&                                       set_pdcp_sn_12bits();
  drb_b_status_transfer18bits_sn_s&                                       set_pdcp_sn_18bits();
  protocol_ie_single_container_s<drb_b_status_transfer_choice_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<drb_b_status_transfer12bits_sn_s,
                  drb_b_status_transfer18bits_sn_s,
                  protocol_ie_single_container_s<drb_b_status_transfer_choice_ext_ies_o>>
      c;

  void destroy_();
};

// QoSFlowMappingIndication ::= ENUMERATED
struct qos_flow_map_ind_opts {
  enum options { ul, dl, /*...*/ nulltype } value;

  const char* to_string() const;
};
using qos_flow_map_ind_e = enumerated<qos_flow_map_ind_opts, true>;

// QoSFlow-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qos_flow_item_ext_ies_o = protocol_ext_empty_o;

using qos_flow_item_ext_ies_container = protocol_ext_container_empty_l;

// QoSFlow-Item ::= SEQUENCE
struct qos_flow_item_s {
  bool                            qos_flow_map_ind_present = false;
  bool                            ie_ext_present           = false;
  uint8_t                         qfi                      = 0;
  qos_flow_map_ind_e              qos_flow_map_ind;
  qos_flow_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlows-List ::= SEQUENCE (SIZE (1..64)) OF QoSFlow-Item
using qos_flows_list_l = dyn_array<qos_flow_item_s>;

// DRBToQoSFlowMapping-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drb_to_qos_flow_map_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drb_to_qos_flow_map_item_ext_ies_container {
  bool                daps_request_info_present = false;
  daps_request_info_s daps_request_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBToQoSFlowMapping-Item ::= SEQUENCE
struct drb_to_qos_flow_map_item_s {
  bool                                       rlc_mode_present = false;
  bool                                       ie_ext_present   = false;
  uint8_t                                    drb_id           = 1;
  qos_flows_list_l                           qos_flows_list;
  rlc_mode_e                                 rlc_mode;
  drb_to_qos_flow_map_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBToQoSFlowMapping-List ::= SEQUENCE (SIZE (1..32)) OF DRBToQoSFlowMapping-Item
using drb_to_qos_flow_map_list_l = dyn_array<drb_to_qos_flow_map_item_s>;

// UPTransportParametersItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using up_transport_params_item_ext_ies_o = protocol_ext_empty_o;

using up_transport_params_item_ext_ies_container = protocol_ext_container_empty_l;

// UPTransportParametersItem ::= SEQUENCE
struct up_transport_params_item_s {
  bool                                       ie_ext_present = false;
  up_transport_layer_info_c                  up_tnl_info;
  uint8_t                                    cell_group_id = 0;
  up_transport_params_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UPTransportParameters ::= SEQUENCE (SIZE (1..4)) OF UPTransportParametersItem
using up_transport_params_l = dyn_array<up_transport_params_item_s>;

// DRBsAdmittedList-ModConfirm-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_admitted_list_mod_confirm_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_admitted_list_mod_confirm_sn_terminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsAdmittedList-ModConfirm-SNterminated-Item ::= SEQUENCE
struct drbs_admitted_list_mod_confirm_sn_terminated_item_s {
  bool                                                                lcid_present    = false;
  bool                                                                ie_exts_present = false;
  uint8_t                                                             drb_id          = 1;
  up_transport_params_l                                               mn_dl_cg_up_tnl_info;
  up_transport_params_l                                               secondary_mn_dl_cg_up_tnl_info;
  uint8_t                                                             lcid = 1;
  drbs_admitted_list_mod_confirm_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsAdmittedList-ModConfirm-SNterminated ::= SEQUENCE (SIZE (1..32)) OF DRBsAdmittedList-ModConfirm-SNterminated-Item
using drbs_admitted_list_mod_confirm_sn_terminated_l = dyn_array<drbs_admitted_list_mod_confirm_sn_terminated_item_s>;

// QoSFlowsMappedtoDRB-SetupResponse-MNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qos_flows_mappedto_drb_setup_resp_m_nterminated_item_ext_ies_o = protocol_ext_empty_o;

using qos_flows_mappedto_drb_setup_resp_m_nterminated_item_ext_ies_container = protocol_ext_container_empty_l;

// QoSFlowsMappedtoDRB-SetupResponse-MNterminated-Item ::= SEQUENCE
struct qos_flows_mappedto_drb_setup_resp_m_nterminated_item_s {
  bool                                                                   ie_exts_present          = false;
  uint8_t                                                                qos_flow_id              = 0;
  uint8_t                                                                current_qos_para_set_idx = 1;
  qos_flows_mappedto_drb_setup_resp_m_nterminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsMappedtoDRB-SetupResponse-MNterminated ::= SEQUENCE (SIZE (1..64)) OF
// QoSFlowsMappedtoDRB-SetupResponse-MNterminated-Item
using qos_flows_mappedto_drb_setup_resp_m_nterminated_l =
    dyn_array<qos_flows_mappedto_drb_setup_resp_m_nterminated_item_s>;

// DRBsAdmittedList-ModificationResponse-MNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_admitted_list_mod_resp_m_nterminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_admitted_list_mod_resp_m_nterminated_item_ext_ies_container {
  bool                                              add_pdcp_dupl_tnl_list_present                          = false;
  bool                                              qos_flows_mappedto_drb_setup_resp_m_nterminated_present = false;
  add_pdcp_dupl_tnl_list_l                          add_pdcp_dupl_tnl_list;
  qos_flows_mappedto_drb_setup_resp_m_nterminated_l qos_flows_mappedto_drb_setup_resp_m_nterminated;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsAdmittedList-ModificationResponse-MNterminated-Item ::= SEQUENCE
struct drbs_admitted_list_mod_resp_m_nterminated_item_s {
  bool                                                             lcid_present    = false;
  bool                                                             ie_exts_present = false;
  uint8_t                                                          drb_id          = 1;
  up_transport_params_l                                            sn_dl_scg_up_tnl_info;
  up_transport_params_l                                            secondary_sn_dl_scg_up_tnl_info;
  uint8_t                                                          lcid = 1;
  drbs_admitted_list_mod_resp_m_nterminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsAdmittedList-ModificationResponse-MNterminated ::= SEQUENCE (SIZE (1..32)) OF
// DRBsAdmittedList-ModificationResponse-MNterminated-Item
using drbs_admitted_list_mod_resp_m_nterminated_l = dyn_array<drbs_admitted_list_mod_resp_m_nterminated_item_s>;

// DRBsAdmittedList-SetupResponse-MNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_admitted_list_setup_resp_m_nterminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_admitted_list_setup_resp_m_nterminated_item_ext_ies_container {
  bool                                              add_pdcp_dupl_tnl_list_present                          = false;
  bool                                              qos_flows_mappedto_drb_setup_resp_m_nterminated_present = false;
  add_pdcp_dupl_tnl_list_l                          add_pdcp_dupl_tnl_list;
  qos_flows_mappedto_drb_setup_resp_m_nterminated_l qos_flows_mappedto_drb_setup_resp_m_nterminated;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsAdmittedList-SetupResponse-MNterminated-Item ::= SEQUENCE
struct drbs_admitted_list_setup_resp_m_nterminated_item_s {
  bool                                                               lcid_present    = false;
  bool                                                               ie_exts_present = false;
  uint8_t                                                            drb_id          = 1;
  up_transport_params_l                                              sn_dl_scg_up_tnl_info;
  up_transport_params_l                                              secondary_sn_dl_scg_up_tnl_info;
  uint8_t                                                            lcid = 1;
  drbs_admitted_list_setup_resp_m_nterminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsAdmittedList-SetupResponse-MNterminated ::= SEQUENCE (SIZE (1..32)) OF
// DRBsAdmittedList-SetupResponse-MNterminated-Item
using drbs_admitted_list_setup_resp_m_nterminated_l = dyn_array<drbs_admitted_list_setup_resp_m_nterminated_item_s>;

// DRBsSubjectToDLDiscarding-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using drbs_subject_to_dl_discarding_item_ext_ies_o = protocol_ext_empty_o;

using drbs_subject_to_dl_discarding_item_ext_ies_container = protocol_ext_container_empty_l;

// DRBsSubjectToDLDiscarding-Item ::= SEQUENCE
struct drbs_subject_to_dl_discarding_item_s {
  bool                                                 ie_ext_present = false;
  uint8_t                                              drb_id         = 1;
  dl_count_choice_c                                    dl_count;
  drbs_subject_to_dl_discarding_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsSubjectToDLDiscarding-List ::= SEQUENCE (SIZE (1..32)) OF DRBsSubjectToDLDiscarding-Item
using drbs_subject_to_dl_discarding_list_l = dyn_array<drbs_subject_to_dl_discarding_item_s>;

// DRBsSubjectToEarlyStatusTransfer-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using drbs_subject_to_early_status_transfer_item_ext_ies_o = protocol_ext_empty_o;

using drbs_subject_to_early_status_transfer_item_ext_ies_container = protocol_ext_container_empty_l;

// DRBsSubjectToEarlyStatusTransfer-Item ::= SEQUENCE
struct drbs_subject_to_early_status_transfer_item_s {
  bool                                                         ie_ext_present = false;
  uint8_t                                                      drb_id         = 1;
  dl_count_choice_c                                            dl_count;
  drbs_subject_to_early_status_transfer_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsSubjectToEarlyStatusTransfer-List ::= SEQUENCE (SIZE (1..32)) OF DRBsSubjectToEarlyStatusTransfer-Item
using drbs_subject_to_early_status_transfer_list_l = dyn_array<drbs_subject_to_early_status_transfer_item_s>;

// DRBsSubjectToStatusTransfer-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_subject_to_status_transfer_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_subject_to_status_transfer_item_ext_ies_container {
  bool             old_qos_flow_map_ul_endmarkerexpected_present = false;
  qos_flows_list_l old_qos_flow_map_ul_endmarkerexpected;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsSubjectToStatusTransfer-Item ::= SEQUENCE
struct drbs_subject_to_status_transfer_item_s {
  bool                                                   ie_ext_present = false;
  uint8_t                                                drb_id         = 1;
  drb_b_status_transfer_choice_c                         pdcp_status_transfer_ul;
  drb_b_status_transfer_choice_c                         pdcp_status_transfer_dl;
  drbs_subject_to_status_transfer_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsSubjectToStatusTransfer-List ::= SEQUENCE (SIZE (1..32)) OF DRBsSubjectToStatusTransfer-Item
using drbs_subject_to_status_transfer_list_l = dyn_array<drbs_subject_to_status_transfer_item_s>;

// Reestablishment-Indication ::= ENUMERATED
struct reest_ind_opts {
  enum options { reestablished, /*...*/ nulltype } value;

  const char* to_string() const;
};
using reest_ind_e = enumerated<reest_ind_opts, true>;

// RLC-Status-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// DRBsToBeModified-List-ModRqd-MNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_modified_list_mod_rqd_m_nterminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_modified_list_mod_rqd_m_nterminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModified-List-ModRqd-MNterminated-Item ::= SEQUENCE
struct drbs_to_be_modified_list_mod_rqd_m_nterminated_item_s {
  bool                                                                  secondary_sn_dl_scg_up_tnl_info_present = false;
  bool                                                                  lcid_present                            = false;
  bool                                                                  rlc_status_present                      = false;
  bool                                                                  ie_exts_present                         = false;
  uint8_t                                                               drb_id                                  = 1;
  up_transport_layer_info_c                                             sn_dl_scg_up_tnl_info;
  up_transport_layer_info_c                                             secondary_sn_dl_scg_up_tnl_info;
  uint8_t                                                               lcid = 1;
  rlc_status_s                                                          rlc_status;
  drbs_to_be_modified_list_mod_rqd_m_nterminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModified-List-ModRqd-MNterminated ::= SEQUENCE (SIZE (1..32)) OF
// DRBsToBeModified-List-ModRqd-MNterminated-Item
using drbs_to_be_modified_list_mod_rqd_m_nterminated_l =
    dyn_array<drbs_to_be_modified_list_mod_rqd_m_nterminated_item_s>;

// UL-UE-Configuration ::= ENUMERATED
struct ul_ue_cfg_opts {
  enum options { no_data, shared, only, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ul_ue_cfg_e = enumerated<ul_ue_cfg_opts, true>;

// ULConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ul_cfg_ext_ies_o = protocol_ext_empty_o;

using ul_cfg_ext_ies_container = protocol_ext_container_empty_l;

// ULConfiguration ::= SEQUENCE
struct ul_cfg_s {
  bool                     ie_exts_present = false;
  ul_ue_cfg_e              ul_pdcp;
  ul_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDCPDuplicationConfiguration ::= ENUMERATED
struct pdcp_dupl_cfg_opts {
  enum options { cfg, de_cfg, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pdcp_dupl_cfg_e = enumerated<pdcp_dupl_cfg_opts, true>;

// DuplicationActivation ::= ENUMERATED
struct dupl_activation_opts {
  enum options { active, inactive, /*...*/ nulltype } value;

  const char* to_string() const;
};
using dupl_activation_e = enumerated<dupl_activation_opts, true>;

// QoSFlowsModifiedMappedtoDRB-ModRqd-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_item_ext_ies_container {
  bool               qos_flow_map_ind_present = false;
  qos_flow_map_ind_e qos_flow_map_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsModifiedMappedtoDRB-ModRqd-SNterminated-Item ::= SEQUENCE
struct qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_item_s {
  bool                mcg_requested_gbr_qos_flow_info_present = false;
  bool                ie_exts_present                         = false;
  uint8_t             qos_flow_id                             = 0;
  gbr_qos_flow_info_s mcg_requested_gbr_qos_flow_info;
  qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsModifiedMappedtoDRB-ModRqd-SNterminated ::= SEQUENCE (SIZE (1..64)) OF
// QoSFlowsModifiedMappedtoDRB-ModRqd-SNterminated-Item
using qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_l =
    dyn_array<qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_item_s>;

// RLCDuplicationState-ItemExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using rlc_dupl_state_item_ext_ies_o = protocol_ext_empty_o;

using rlc_dupl_state_item_ext_ies_container = protocol_ext_container_empty_l;

// RLCDuplicationState-Item ::= SEQUENCE
struct rlc_dupl_state_item_s {
  struct dupl_state_opts {
    enum options { active, inactive, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using dupl_state_e_ = enumerated<dupl_state_opts, true>;

  // member variables
  bool                                  ie_exts_present = false;
  dupl_state_e_                         dupl_state;
  rlc_dupl_state_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RLCDuplicationStateList ::= SEQUENCE (SIZE (1..3)) OF RLCDuplicationState-Item
using rlc_dupl_state_list_l = dyn_array<rlc_dupl_state_item_s>;

// RLCDuplicationInformation-ItemExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using rlc_dupl_info_item_ext_ies_o = protocol_ext_empty_o;

using rlc_dupl_info_item_ext_ies_container = protocol_ext_container_empty_l;

// RLCDuplicationInformation ::= SEQUENCE
struct rlc_dupl_info_s {
  struct rlc_primary_ind_opts {
    enum options { true_value, false_value, nulltype } value;

    const char* to_string() const;
  };
  using rlc_primary_ind_e_ = enumerated<rlc_primary_ind_opts>;

  // member variables
  bool                                 rlc_primary_ind_present = false;
  bool                                 ie_exts_present         = false;
  rlc_dupl_state_list_l                rlc_dupl_state_list;
  rlc_primary_ind_e_                   rlc_primary_ind;
  rlc_dupl_info_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModified-List-ModRqd-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_modified_list_mod_rqd_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_modified_list_mod_rqd_sn_terminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  bool                     rlc_dupl_info_present          = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s          rlc_dupl_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModified-List-ModRqd-SNterminated-Item ::= SEQUENCE
struct drbs_to_be_modified_list_mod_rqd_sn_terminated_item_s {
  bool                                                                  drb_qos_present         = false;
  bool                                                                  ul_cfg_present          = false;
  bool                                                                  pdcp_dupl_cfg_present   = false;
  bool                                                                  dupl_activation_present = false;
  bool                                                                  ie_exts_present         = false;
  uint8_t                                                               drb_id                  = 1;
  up_transport_params_l                                                 sn_ul_pdcp_up_tnl_info;
  qos_flow_level_qos_params_s                                           drb_qos;
  up_transport_params_l                                                 secondary_sn_ul_pdcp_up_tnl_info;
  ul_cfg_s                                                              ul_cfg;
  pdcp_dupl_cfg_e                                                       pdcp_dupl_cfg;
  dupl_activation_e                                                     dupl_activation;
  qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_l               qos_flows_mappedto_drb_mod_rqd_sn_terminated;
  drbs_to_be_modified_list_mod_rqd_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModified-List-ModRqd-SNterminated ::= SEQUENCE (SIZE (1..32)) OF
// DRBsToBeModified-List-ModRqd-SNterminated-Item
using drbs_to_be_modified_list_mod_rqd_sn_terminated_l =
    dyn_array<drbs_to_be_modified_list_mod_rqd_sn_terminated_item_s>;

// DRBsToBeModified-List-Modified-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_modified_list_modified_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_modified_list_modified_sn_terminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModified-List-Modified-SNterminated-Item ::= SEQUENCE
struct drbs_to_be_modified_list_modified_sn_terminated_item_s {
  bool                                                                   lcid_present       = false;
  bool                                                                   rlc_status_present = false;
  bool                                                                   ie_exts_present    = false;
  uint8_t                                                                drb_id             = 1;
  up_transport_params_l                                                  mn_dl_scg_up_tnl_info;
  up_transport_params_l                                                  secondary_mn_dl_scg_up_tnl_info;
  uint8_t                                                                lcid = 1;
  rlc_status_s                                                           rlc_status;
  drbs_to_be_modified_list_modified_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModified-List-Modified-SNterminated ::= SEQUENCE (SIZE (1..32)) OF
// DRBsToBeModified-List-Modified-SNterminated-Item
using drbs_to_be_modified_list_modified_sn_terminated_l =
    dyn_array<drbs_to_be_modified_list_modified_sn_terminated_item_s>;

// N6JitterInformationExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// TSCAssistanceInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct tsc_assist_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct tsc_assist_info_ext_ies_container {
  bool                     survival_time_present          = false;
  bool                     cap_for_bat_adaptation_present = false;
  bool                     n6_jitter_info_present         = false;
  uint32_t                 survival_time;
  cap_for_bat_adaptation_e cap_for_bat_adaptation;
  n6_jitter_info_s         n6_jitter_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TSCAssistanceInformation ::= SEQUENCE
struct tsc_assist_info_s {
  bool                              ie_ext_present = false;
  uint32_t                          periodicity    = 0;
  unbounded_octstring<true>         burst_arrival_time;
  tsc_assist_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TSCTrafficCharacteristics-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tsc_traffic_characteristics_ext_ies_o = protocol_ext_empty_o;

using tsc_traffic_characteristics_ext_ies_container = protocol_ext_container_empty_l;

// TSCTrafficCharacteristics ::= SEQUENCE
struct tsc_traffic_characteristics_s {
  bool                                          tsc_assist_info_dl_present = false;
  bool                                          tsc_assist_info_ul_present = false;
  bool                                          ie_ext_present             = false;
  tsc_assist_info_s                             tsc_assist_info_dl;
  tsc_assist_info_s                             tsc_assist_info_ul;
  tsc_traffic_characteristics_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsMappedtoDRB-Setup-MNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flows_mappedto_drb_setup_m_nterminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flows_mappedto_drb_setup_m_nterminated_item_ext_ies_container {
  bool                          tsc_traffic_characteristics_present = false;
  tsc_traffic_characteristics_s tsc_traffic_characteristics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsMappedtoDRB-Setup-MNterminated-Item ::= SEQUENCE
struct qos_flows_mappedto_drb_setup_m_nterminated_item_s {
  bool                                                              qos_flow_map_ind_present = false;
  bool                                                              ie_exts_present          = false;
  uint8_t                                                           qos_flow_id              = 0;
  qos_flow_level_qos_params_s                                       qos_flow_level_qos_params;
  qos_flow_map_ind_e                                                qos_flow_map_ind;
  qos_flows_mappedto_drb_setup_m_nterminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsMappedtoDRB-Setup-MNterminated ::= SEQUENCE (SIZE (1..64)) OF QoSFlowsMappedtoDRB-Setup-MNterminated-Item
using qos_flows_mappedto_drb_setup_m_nterminated_l = dyn_array<qos_flows_mappedto_drb_setup_m_nterminated_item_s>;

// DRBsToBeModifiedList-Modification-MNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_modified_list_mod_m_nterminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_modified_list_mod_m_nterminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  bool                     rlc_dupl_info_present          = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s          rlc_dupl_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModifiedList-Modification-MNterminated-Item ::= SEQUENCE
struct drbs_to_be_modified_list_mod_m_nterminated_item_s {
  bool                                                              drb_qos_present         = false;
  bool                                                              ul_cfg_present          = false;
  bool                                                              pdcp_dupl_cfg_present   = false;
  bool                                                              dupl_activation_present = false;
  bool                                                              ie_exts_present         = false;
  uint8_t                                                           drb_id                  = 1;
  up_transport_params_l                                             mn_ul_pdcp_up_tnl_info;
  qos_flow_level_qos_params_s                                       drb_qos;
  up_transport_params_l                                             secondary_mn_ul_pdcp_up_tnl_info;
  ul_cfg_s                                                          ul_cfg;
  pdcp_dupl_cfg_e                                                   pdcp_dupl_cfg;
  dupl_activation_e                                                 dupl_activation;
  qos_flows_mappedto_drb_setup_m_nterminated_l                      qos_flows_mappedto_drb_setup_m_nterminated;
  drbs_to_be_modified_list_mod_m_nterminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModifiedList-Modification-MNterminated ::= SEQUENCE (SIZE (1..32)) OF
// DRBsToBeModifiedList-Modification-MNterminated-Item
using drbs_to_be_modified_list_mod_m_nterminated_l = dyn_array<drbs_to_be_modified_list_mod_m_nterminated_item_s>;

// QoSFlowsMappedtoDRB-SetupResponse-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flows_mappedto_drb_setup_resp_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flows_mappedto_drb_setup_resp_sn_terminated_item_ext_ies_container {
  bool                                  current_qos_para_set_idx_present        = false;
  bool                                  source_dl_forwarding_ip_address_present = false;
  uint8_t                               current_qos_para_set_idx;
  bounded_bitstring<1, 160, true, true> source_dl_forwarding_ip_address;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsMappedtoDRB-SetupResponse-SNterminated-Item ::= SEQUENCE
struct qos_flows_mappedto_drb_setup_resp_sn_terminated_item_s {
  bool                mcg_requested_gbr_qos_flow_info_present = false;
  bool                qos_flow_map_ind_present                = false;
  bool                ie_exts_present                         = false;
  uint8_t             qos_flow_id                             = 0;
  gbr_qos_flow_info_s mcg_requested_gbr_qos_flow_info;
  qos_flow_map_ind_e  qos_flow_map_ind;
  qos_flows_mappedto_drb_setup_resp_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsMappedtoDRB-SetupResponse-SNterminated ::= SEQUENCE (SIZE (1..64)) OF
// QoSFlowsMappedtoDRB-SetupResponse-SNterminated-Item
using qos_flows_mappedto_drb_setup_resp_sn_terminated_l =
    dyn_array<qos_flows_mappedto_drb_setup_resp_sn_terminated_item_s>;

// DRBsToBeModifiedList-ModificationResponse-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_modified_list_mod_resp_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_modified_list_mod_resp_sn_terminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present           = false;
  bool                     rlc_dupl_info_present                    = false;
  bool                     secondary_sn_ul_pdcp_up_tnl_info_present = false;
  bool                     pdcp_dupl_cfg_present                    = false;
  bool                     dupl_activation_present                  = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s          rlc_dupl_info;
  up_transport_params_l    secondary_sn_ul_pdcp_up_tnl_info;
  pdcp_dupl_cfg_e          pdcp_dupl_cfg;
  dupl_activation_e        dupl_activation;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModifiedList-ModificationResponse-SNterminated-Item ::= SEQUENCE
struct drbs_to_be_modified_list_mod_resp_sn_terminated_item_s {
  bool                                              drb_qos_present = false;
  bool                                              ie_exts_present = false;
  uint8_t                                           drb_id          = 1;
  up_transport_params_l                             sn_ul_pdcp_up_tnl_info;
  qos_flow_level_qos_params_s                       drb_qos;
  qos_flows_mappedto_drb_setup_resp_sn_terminated_l qos_flows_mappedto_drb_setup_resp_sn_terminated;
  drbs_to_be_modified_list_mod_resp_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeModifiedList-ModificationResponse-SNterminated ::= SEQUENCE (SIZE (1..32)) OF
// DRBsToBeModifiedList-ModificationResponse-SNterminated-Item
using drbs_to_be_modified_list_mod_resp_sn_terminated_l =
    dyn_array<drbs_to_be_modified_list_mod_resp_sn_terminated_item_s>;

// PDCPSNLength-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdcp_sn_len_ext_ies_o = protocol_ext_empty_o;

using pdcp_sn_len_ext_ies_container = protocol_ext_container_empty_l;

// PDCPSNLength ::= SEQUENCE
struct pdcp_sn_len_s {
  struct ul_pdcp_sn_len_opts {
    enum options { v12bits, v18bits, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using ul_pdcp_sn_len_e_ = enumerated<ul_pdcp_sn_len_opts, true>;
  struct dl_pdcp_sn_len_opts {
    enum options { v12bits, v18bits, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using dl_pdcp_sn_len_e_ = enumerated<dl_pdcp_sn_len_opts, true>;

  // member variables
  bool                          ie_ext_present = false;
  ul_pdcp_sn_len_e_             ul_pdcp_sn_len;
  dl_pdcp_sn_len_e_             dl_pdcp_sn_len;
  pdcp_sn_len_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsSetupMappedtoDRB-ModRqd-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_item_ext_ies_container {
  bool               qos_flow_map_ind_present = false;
  qos_flow_map_ind_e qos_flow_map_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsSetupMappedtoDRB-ModRqd-SNterminated-Item ::= SEQUENCE
struct qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_item_s {
  bool                mcg_requested_gbr_qos_flow_info_present = false;
  bool                ie_exts_present                         = false;
  uint8_t             qos_flow_id                             = 0;
  gbr_qos_flow_info_s mcg_requested_gbr_qos_flow_info;
  qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsSetupMappedtoDRB-ModRqd-SNterminated ::= SEQUENCE (SIZE (1..64)) OF
// QoSFlowsSetupMappedtoDRB-ModRqd-SNterminated-Item
using qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_l =
    dyn_array<qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_item_s>;

// DRBsToBeSetup-List-ModRqd-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_setup_list_mod_rqd_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_setup_list_mod_rqd_sn_terminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  bool                     rlc_dupl_info_present          = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s          rlc_dupl_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeSetup-List-ModRqd-SNterminated-Item ::= SEQUENCE
struct drbs_to_be_setup_list_mod_rqd_sn_terminated_item_s {
  bool                                                               pdcp_sn_len_present     = false;
  bool                                                               dupl_activation_present = false;
  bool                                                               ul_cfg_present          = false;
  bool                                                               ie_exts_present         = false;
  uint8_t                                                            drb_id                  = 1;
  pdcp_sn_len_s                                                      pdcp_sn_len;
  up_transport_params_l                                              sn_ul_pdcp_up_tnl_info;
  qos_flow_level_qos_params_s                                        drb_qos;
  up_transport_params_l                                              secondary_sn_ul_pdcp_up_tnl_info;
  dupl_activation_e                                                  dupl_activation;
  ul_cfg_s                                                           ul_cfg;
  qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_l               qos_flows_mappedto_drb_mod_rqd_sn_terminated;
  rlc_mode_e                                                         rlc_mode;
  drbs_to_be_setup_list_mod_rqd_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeSetup-List-ModRqd-SNterminated ::= SEQUENCE (SIZE (1..32)) OF DRBsToBeSetup-List-ModRqd-SNterminated-Item
using drbs_to_be_setup_list_mod_rqd_sn_terminated_l = dyn_array<drbs_to_be_setup_list_mod_rqd_sn_terminated_item_s>;

// DRBsToBeSetupList-BearerSetupComplete-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_setup_list_bearer_setup_complete_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_setup_list_bearer_setup_complete_sn_terminated_item_ext_ies_container {
  bool                      secondary_mn_xn_u_tnl_infoat_m_present = false;
  up_transport_layer_info_c secondary_mn_xn_u_tnl_infoat_m;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeSetupList-BearerSetupComplete-SNterminated-Item ::= SEQUENCE
struct drbs_to_be_setup_list_bearer_setup_complete_sn_terminated_item_s {
  bool                                                                             ie_exts_present = false;
  uint8_t                                                                          drb_id          = 1;
  up_transport_layer_info_c                                                        mn_xn_u_tnl_infoat_m;
  drbs_to_be_setup_list_bearer_setup_complete_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeSetupList-Setup-MNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_setup_list_setup_m_nterminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_setup_list_setup_m_nterminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  bool                     rlc_dupl_info_present          = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s          rlc_dupl_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeSetupList-Setup-MNterminated-Item ::= SEQUENCE
struct drbs_to_be_setup_list_setup_m_nterminated_item_s {
  bool                                                             ul_cfg_present          = false;
  bool                                                             pdcp_sn_len_present     = false;
  bool                                                             dupl_activation_present = false;
  bool                                                             ie_exts_present         = false;
  uint8_t                                                          drb_id                  = 1;
  up_transport_params_l                                            mn_ul_pdcp_up_tnl_info;
  rlc_mode_e                                                       rlc_mode;
  ul_cfg_s                                                         ul_cfg;
  qos_flow_level_qos_params_s                                      drb_qos;
  pdcp_sn_len_s                                                    pdcp_sn_len;
  up_transport_params_l                                            secondary_mn_ul_pdcp_up_tnl_info;
  dupl_activation_e                                                dupl_activation;
  qos_flows_mappedto_drb_setup_m_nterminated_l                     qos_flows_mappedto_drb_setup_m_nterminated;
  drbs_to_be_setup_list_setup_m_nterminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeSetupList-Setup-MNterminated ::= SEQUENCE (SIZE (1..32)) OF DRBsToBeSetupList-Setup-MNterminated-Item
using drbs_to_be_setup_list_setup_m_nterminated_l = dyn_array<drbs_to_be_setup_list_setup_m_nterminated_item_s>;

// DRBsToBeSetupList-SetupResponse-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct drbs_to_be_setup_list_setup_resp_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct drbs_to_be_setup_list_setup_resp_sn_terminated_item_ext_ies_container {
  bool                     add_pdcp_dupl_tnl_list_present = false;
  bool                     rlc_dupl_info_present          = false;
  add_pdcp_dupl_tnl_list_l add_pdcp_dupl_tnl_list;
  rlc_dupl_info_s          rlc_dupl_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeSetupList-SetupResponse-SNterminated-Item ::= SEQUENCE
struct drbs_to_be_setup_list_setup_resp_sn_terminated_item_s {
  bool                                                                  pdcp_sn_len_present     = false;
  bool                                                                  ul_cfg_present          = false;
  bool                                                                  dupl_activation_present = false;
  bool                                                                  ie_exts_present         = false;
  uint8_t                                                               drb_id                  = 1;
  up_transport_params_l                                                 sn_ul_pdcp_up_tnl_info;
  qos_flow_level_qos_params_s                                           drb_qos;
  pdcp_sn_len_s                                                         pdcp_sn_len;
  rlc_mode_e                                                            rlc_mode;
  ul_cfg_s                                                              ul_cfg;
  up_transport_params_l                                                 secondary_sn_ul_pdcp_up_tnl_info;
  dupl_activation_e                                                     dupl_activation;
  qos_flows_mappedto_drb_setup_resp_sn_terminated_l                     qos_flows_mappedto_drb_setup_resp_sn_terminated;
  drbs_to_be_setup_list_setup_resp_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DRBsToBeSetupList-SetupResponse-SNterminated ::= SEQUENCE (SIZE (1..32)) OF
// DRBsToBeSetupList-SetupResponse-SNterminated-Item
using drbs_to_be_setup_list_setup_resp_sn_terminated_l =
    dyn_array<drbs_to_be_setup_list_setup_resp_sn_terminated_item_s>;

// DU-RX-MT-RX ::= ENUMERATED
struct du_rx_mt_rx_opts {
  enum options { supported, not_supported, supported_fdm_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_rx_mt_rx_e = enumerated<du_rx_mt_rx_opts, true>;

// DU-RX-MT-TX ::= ENUMERATED
struct du_rx_mt_tx_opts {
  enum options { supported, not_supported, supported_fdm_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_rx_mt_tx_e = enumerated<du_rx_mt_tx_opts, true>;

// DU-TX-MT-RX ::= ENUMERATED
struct du_tx_mt_rx_opts {
  enum options { supported, not_supported, supported_fdm_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_tx_mt_rx_e = enumerated<du_tx_mt_rx_opts, true>;

// DU-TX-MT-TX ::= ENUMERATED
struct du_tx_mt_tx_opts {
  enum options { supported, not_supported, supported_fdm_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_tx_mt_tx_e = enumerated<du_tx_mt_tx_opts, true>;

// Permutation ::= ENUMERATED
struct permutation_opts {
  enum options { dfu, ufd, /*...*/ nulltype } value;

  const char* to_string() const;
};
using permutation_e = enumerated<permutation_opts, true>;

// ExplicitFormat-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ImplicitFormat-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using implicit_format_ext_ies_o = protocol_ext_empty_o;

using implicit_format_ext_ies_container = protocol_ext_container_empty_l;

// ImplicitFormat ::= SEQUENCE
struct implicit_format_s {
  bool                              ie_exts_present     = false;
  uint8_t                           du_f_slotformat_idx = 0;
  implicit_format_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DUF-Slot-Config-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
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

// DUFTransmissionPeriodicity ::= ENUMERATED
struct du_f_tx_periodicity_opts {
  enum options { ms0p5, ms0p625, ms1, ms1p25, ms2, ms2p5, ms5, ms10, /*...*/ nulltype } value;

  const char* to_string() const;
};
using du_f_tx_periodicity_e = enumerated<du_f_tx_periodicity_opts, true>;

// DataCollectionID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using data_collection_id_ext_ies_o = protocol_ext_empty_o;

using data_collection_id_ext_ies_container = protocol_ext_container_empty_l;

// DataCollectionID ::= SEQUENCE
struct data_collection_id_s {
  bool                                 ie_exts_present     = false;
  uint16_t                             ngran_node1_meas_id = 1;
  uint16_t                             ngran_node2_meas_id = 1;
  data_collection_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsToBeForwarded-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qos_flows_to_be_forwarded_item_ext_ies_o = protocol_ext_empty_o;

using qos_flows_to_be_forwarded_item_ext_ies_container = protocol_ext_container_empty_l;

// QoSFlowsToBeForwarded-Item ::= SEQUENCE
struct qos_flows_to_be_forwarded_item_s {
  bool                                             ie_ext_present = false;
  uint8_t                                          qos_flow_id    = 0;
  qos_flows_to_be_forwarded_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsToBeForwarded-List ::= SEQUENCE (SIZE (1..64)) OF QoSFlowsToBeForwarded-Item
using qos_flows_to_be_forwarded_list_l = dyn_array<qos_flows_to_be_forwarded_item_s>;

// DataForwardingInfoFromTargetE-UTRANnode-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using data_forwarding_info_from_target_e_utran_node_item_ext_ies_o = protocol_ext_empty_o;

using data_forwarding_info_from_target_e_utran_node_item_ext_ies_container = protocol_ext_container_empty_l;

// DataForwardingInfoFromTargetE-UTRANnode-Item ::= SEQUENCE
struct data_forwarding_info_from_target_e_utran_node_item_s {
  bool                                                                 ie_ext_present = false;
  up_transport_layer_info_c                                            dl_forwarding_up_tnl_info;
  qos_flows_to_be_forwarded_list_l                                     qos_flows_to_be_forwarded_list;
  data_forwarding_info_from_target_e_utran_node_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataForwardingInfoFromTargetE-UTRANnode-List ::= SEQUENCE (SIZE (1..256)) OF
// DataForwardingInfoFromTargetE-UTRANnode-Item
using data_forwarding_info_from_target_e_utran_node_list_l =
    dyn_array<data_forwarding_info_from_target_e_utran_node_item_s>;

// DataForwardingInfoFromTargetE-UTRANnode-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using data_forwarding_info_from_target_e_utran_node_ext_ies_o = protocol_ext_empty_o;

using data_forwarding_info_from_target_e_utran_node_ext_ies_container = protocol_ext_container_empty_l;

// DataForwardingInfoFromTargetE-UTRANnode ::= SEQUENCE
struct data_forwarding_info_from_target_e_utran_node_s {
  bool                                                            ie_ext_present = false;
  data_forwarding_info_from_target_e_utran_node_list_l            data_forwarding_info_from_target_e_utran_node_list;
  data_forwarding_info_from_target_e_utran_node_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SharedResourceType-UL-OnlySharing-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using shared_res_type_ul_only_sharing_ext_ies_o = protocol_ext_empty_o;

using shared_res_type_ul_only_sharing_ext_ies_container = protocol_ext_container_empty_l;

// SharedResourceType-UL-OnlySharing ::= SEQUENCE
struct shared_res_type_ul_only_sharing_s {
  bool                                              ie_exts_present = false;
  bounded_bitstring<6, 17600, false, true>          ul_res_bitmap;
  shared_res_type_ul_only_sharing_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SharedResourceType-ULDL-Sharing-UL-ResourcesChanged-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using shared_res_type_ul_dl_sharing_ul_res_changed_ext_ies_o = protocol_ext_empty_o;

using shared_res_type_ul_dl_sharing_ul_res_changed_ext_ies_container = protocol_ext_container_empty_l;

// SharedResourceType-ULDL-Sharing-UL-ResourcesChanged ::= SEQUENCE
struct shared_res_type_ul_dl_sharing_ul_res_changed_s {
  bool                                                           ie_exts_present = false;
  bounded_bitstring<6, 17600, false, true>                       ul_res_bitmap;
  shared_res_type_ul_dl_sharing_ul_res_changed_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SharedResourceType-ULDL-Sharing-UL-Resources-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using shared_res_type_ul_dl_sharing_ul_res_ext_ies_o = protocol_ies_empty_o;

// SharedResourceType-ULDL-Sharing-UL-Resources ::= CHOICE
struct shared_res_type_ul_dl_sharing_ul_res_c {
  struct types_opts {
    enum options { unchanged, changed, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  shared_res_type_ul_dl_sharing_ul_res_c() = default;
  shared_res_type_ul_dl_sharing_ul_res_c(const shared_res_type_ul_dl_sharing_ul_res_c& other);
  shared_res_type_ul_dl_sharing_ul_res_c& operator=(const shared_res_type_ul_dl_sharing_ul_res_c& other);
  ~shared_res_type_ul_dl_sharing_ul_res_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  shared_res_type_ul_dl_sharing_ul_res_changed_s& changed()
  {
    assert_choice_type(types::changed, type_, "SharedResourceType-ULDL-Sharing-UL-Resources");
    return c.get<shared_res_type_ul_dl_sharing_ul_res_changed_s>();
  }
  protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ul_res_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SharedResourceType-ULDL-Sharing-UL-Resources");
    return c.get<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ul_res_ext_ies_o>>();
  }
  const shared_res_type_ul_dl_sharing_ul_res_changed_s& changed() const
  {
    assert_choice_type(types::changed, type_, "SharedResourceType-ULDL-Sharing-UL-Resources");
    return c.get<shared_res_type_ul_dl_sharing_ul_res_changed_s>();
  }
  const protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ul_res_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SharedResourceType-ULDL-Sharing-UL-Resources");
    return c.get<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ul_res_ext_ies_o>>();
  }
  void                                                                            set_unchanged();
  shared_res_type_ul_dl_sharing_ul_res_changed_s&                                 set_changed();
  protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ul_res_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ul_res_ext_ies_o>,
                  shared_res_type_ul_dl_sharing_ul_res_changed_s>
      c;

  void destroy_();
};

// SharedResourceType-ULDL-Sharing-DL-ResourcesChanged-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using shared_res_type_ul_dl_sharing_dl_res_changed_ext_ies_o = protocol_ext_empty_o;

using shared_res_type_ul_dl_sharing_dl_res_changed_ext_ies_container = protocol_ext_container_empty_l;

// SharedResourceType-ULDL-Sharing-DL-ResourcesChanged ::= SEQUENCE
struct shared_res_type_ul_dl_sharing_dl_res_changed_s {
  bool                                                           ie_exts_present = false;
  bounded_bitstring<6, 17600, false, true>                       dl_res_bitmap;
  shared_res_type_ul_dl_sharing_dl_res_changed_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SharedResourceType-ULDL-Sharing-DL-Resources-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using shared_res_type_ul_dl_sharing_dl_res_ext_ies_o = protocol_ies_empty_o;

// SharedResourceType-ULDL-Sharing-DL-Resources ::= CHOICE
struct shared_res_type_ul_dl_sharing_dl_res_c {
  struct types_opts {
    enum options { unchanged, changed, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  shared_res_type_ul_dl_sharing_dl_res_c() = default;
  shared_res_type_ul_dl_sharing_dl_res_c(const shared_res_type_ul_dl_sharing_dl_res_c& other);
  shared_res_type_ul_dl_sharing_dl_res_c& operator=(const shared_res_type_ul_dl_sharing_dl_res_c& other);
  ~shared_res_type_ul_dl_sharing_dl_res_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  shared_res_type_ul_dl_sharing_dl_res_changed_s& changed()
  {
    assert_choice_type(types::changed, type_, "SharedResourceType-ULDL-Sharing-DL-Resources");
    return c.get<shared_res_type_ul_dl_sharing_dl_res_changed_s>();
  }
  protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_dl_res_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SharedResourceType-ULDL-Sharing-DL-Resources");
    return c.get<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_dl_res_ext_ies_o>>();
  }
  const shared_res_type_ul_dl_sharing_dl_res_changed_s& changed() const
  {
    assert_choice_type(types::changed, type_, "SharedResourceType-ULDL-Sharing-DL-Resources");
    return c.get<shared_res_type_ul_dl_sharing_dl_res_changed_s>();
  }
  const protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_dl_res_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SharedResourceType-ULDL-Sharing-DL-Resources");
    return c.get<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_dl_res_ext_ies_o>>();
  }
  void                                                                            set_unchanged();
  shared_res_type_ul_dl_sharing_dl_res_changed_s&                                 set_changed();
  protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_dl_res_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_dl_res_ext_ies_o>,
                  shared_res_type_ul_dl_sharing_dl_res_changed_s>
      c;

  void destroy_();
};

// SharedResourceType-ULDL-Sharing-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using shared_res_type_ul_dl_sharing_ext_ies_o = protocol_ies_empty_o;

// SharedResourceType-ULDL-Sharing ::= CHOICE
struct shared_res_type_ul_dl_sharing_c {
  struct types_opts {
    enum options { ul_res, dl_res, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  shared_res_type_ul_dl_sharing_c() = default;
  shared_res_type_ul_dl_sharing_c(const shared_res_type_ul_dl_sharing_c& other);
  shared_res_type_ul_dl_sharing_c& operator=(const shared_res_type_ul_dl_sharing_c& other);
  ~shared_res_type_ul_dl_sharing_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  shared_res_type_ul_dl_sharing_ul_res_c& ul_res()
  {
    assert_choice_type(types::ul_res, type_, "SharedResourceType-ULDL-Sharing");
    return c.get<shared_res_type_ul_dl_sharing_ul_res_c>();
  }
  shared_res_type_ul_dl_sharing_dl_res_c& dl_res()
  {
    assert_choice_type(types::dl_res, type_, "SharedResourceType-ULDL-Sharing");
    return c.get<shared_res_type_ul_dl_sharing_dl_res_c>();
  }
  protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SharedResourceType-ULDL-Sharing");
    return c.get<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ext_ies_o>>();
  }
  const shared_res_type_ul_dl_sharing_ul_res_c& ul_res() const
  {
    assert_choice_type(types::ul_res, type_, "SharedResourceType-ULDL-Sharing");
    return c.get<shared_res_type_ul_dl_sharing_ul_res_c>();
  }
  const shared_res_type_ul_dl_sharing_dl_res_c& dl_res() const
  {
    assert_choice_type(types::dl_res, type_, "SharedResourceType-ULDL-Sharing");
    return c.get<shared_res_type_ul_dl_sharing_dl_res_c>();
  }
  const protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SharedResourceType-ULDL-Sharing");
    return c.get<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ext_ies_o>>();
  }
  shared_res_type_ul_dl_sharing_ul_res_c&                                  set_ul_res();
  shared_res_type_ul_dl_sharing_dl_res_c&                                  set_dl_res();
  protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<shared_res_type_ul_dl_sharing_ext_ies_o>,
                  shared_res_type_ul_dl_sharing_dl_res_c,
                  shared_res_type_ul_dl_sharing_ul_res_c>
      c;

  void destroy_();
};

// SharedResourceType-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using shared_res_type_ext_ies_o = protocol_ies_empty_o;

// SharedResourceType ::= CHOICE
struct shared_res_type_c {
  struct types_opts {
    enum options { ul_only_sharing, ul_and_dl_sharing, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  shared_res_type_c() = default;
  shared_res_type_c(const shared_res_type_c& other);
  shared_res_type_c& operator=(const shared_res_type_c& other);
  ~shared_res_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  shared_res_type_ul_only_sharing_s& ul_only_sharing()
  {
    assert_choice_type(types::ul_only_sharing, type_, "SharedResourceType");
    return c.get<shared_res_type_ul_only_sharing_s>();
  }
  shared_res_type_ul_dl_sharing_c& ul_and_dl_sharing()
  {
    assert_choice_type(types::ul_and_dl_sharing, type_, "SharedResourceType");
    return c.get<shared_res_type_ul_dl_sharing_c>();
  }
  protocol_ie_single_container_s<shared_res_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SharedResourceType");
    return c.get<protocol_ie_single_container_s<shared_res_type_ext_ies_o>>();
  }
  const shared_res_type_ul_only_sharing_s& ul_only_sharing() const
  {
    assert_choice_type(types::ul_only_sharing, type_, "SharedResourceType");
    return c.get<shared_res_type_ul_only_sharing_s>();
  }
  const shared_res_type_ul_dl_sharing_c& ul_and_dl_sharing() const
  {
    assert_choice_type(types::ul_and_dl_sharing, type_, "SharedResourceType");
    return c.get<shared_res_type_ul_dl_sharing_c>();
  }
  const protocol_ie_single_container_s<shared_res_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SharedResourceType");
    return c.get<protocol_ie_single_container_s<shared_res_type_ext_ies_o>>();
  }
  shared_res_type_ul_only_sharing_s&                         set_ul_only_sharing();
  shared_res_type_ul_dl_sharing_c&                           set_ul_and_dl_sharing();
  protocol_ie_single_container_s<shared_res_type_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<shared_res_type_ext_ies_o>,
                  shared_res_type_ul_dl_sharing_c,
                  shared_res_type_ul_only_sharing_s>
      c;

  void destroy_();
};

// ReservedSubframePattern-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using reserved_sf_pattern_ext_ies_o = protocol_ext_empty_o;

using reserved_sf_pattern_ext_ies_container = protocol_ext_container_empty_l;

// ReservedSubframePattern ::= SEQUENCE
struct reserved_sf_pattern_s {
  struct sf_type_opts {
    enum options { mbsfn, non_mbsfn, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using sf_type_e_ = enumerated<sf_type_opts, true>;

  // member variables
  bool                                    mbsfn_ctrl_region_len_present = false;
  bool                                    ie_ext_present                = false;
  sf_type_e_                              sf_type;
  bounded_bitstring<10, 160, false, true> reserved_sf_pattern;
  uint8_t                                 mbsfn_ctrl_region_len = 0;
  reserved_sf_pattern_ext_ies_container   ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DataTrafficResourceIndication-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using data_traffic_res_ind_ext_ies_o = protocol_ext_empty_o;

using data_traffic_res_ind_ext_ies_container = protocol_ext_container_empty_l;

// DataTrafficResourceIndication ::= SEQUENCE
struct data_traffic_res_ind_s {
  bool                                   reserved_sf_pattern_present = false;
  bool                                   ie_ext_present              = false;
  uint16_t                               activation_sfn              = 0;
  shared_res_type_c                      shared_res_type;
  reserved_sf_pattern_s                  reserved_sf_pattern;
  data_traffic_res_ind_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ULForwarding ::= ENUMERATED
struct ul_forwarding_opts {
  enum options { ul_forwarding_proposed, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ul_forwarding_e = enumerated<ul_forwarding_opts, true>;

// ULForwardingProposal ::= ENUMERATED
struct ul_forwarding_proposal_opts {
  enum options { ul_forwarding_proposed, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ul_forwarding_proposal_e = enumerated<ul_forwarding_proposal_opts, true>;

// QoSFLowsToBeForwarded-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_f_lows_to_be_forwarded_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_f_lows_to_be_forwarded_item_ext_ies_container {
  bool                                  ul_forwarding_proposal_present               = false;
  bool                                  source_dl_forwarding_ip_address_present      = false;
  bool                                  source_node_dl_forwarding_ip_address_present = false;
  ul_forwarding_proposal_e              ul_forwarding_proposal;
  bounded_bitstring<1, 160, true, true> source_dl_forwarding_ip_address;
  bounded_bitstring<1, 160, true, true> source_node_dl_forwarding_ip_address;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFLowsToBeForwarded-Item ::= SEQUENCE
struct qos_f_lows_to_be_forwarded_item_s {
  bool                                              ie_ext_present = false;
  uint8_t                                           qos_flow_id    = 0;
  dl_forwarding_e                                   dl_dataforwarding;
  ul_forwarding_e                                   ul_dataforwarding;
  qos_f_lows_to_be_forwarded_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFLowsToBeForwarded-List ::= SEQUENCE (SIZE (1..64)) OF QoSFLowsToBeForwarded-Item
using qos_f_lows_to_be_forwarded_list_l = dyn_array<qos_f_lows_to_be_forwarded_item_s>;

// DataforwardingandOffloadingInfofromSource-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using dataforwardingand_offloading_infofrom_source_ext_ies_o = protocol_ext_empty_o;

using dataforwardingand_offloading_infofrom_source_ext_ies_container = protocol_ext_container_empty_l;

// DataforwardingandOffloadingInfofromSource ::= SEQUENCE
struct dataforwardingand_offloading_infofrom_source_s {
  bool                                                           ie_ext_present = false;
  qos_f_lows_to_be_forwarded_list_l                              qos_flows_to_be_forwarded;
  drb_to_qos_flow_map_list_l                                     source_drb_to_qos_flow_map;
  dataforwardingand_offloading_infofrom_source_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DefaultDRB-Allowed ::= ENUMERATED
struct default_drb_allowed_opts {
  enum options { true_value, false_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using default_drb_allowed_e = enumerated<default_drb_allowed_opts, true>;

// DesiredActNotificationLevel ::= ENUMERATED
struct desired_act_notif_level_opts {
  enum options { none, qos_flow, pdu_session, ue_level, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using desired_act_notif_level_e = enumerated<desired_act_notif_level_opts, true>;

// DirectForwardingPathAvailabilityWithSourceMN ::= ENUMERATED
struct direct_forwarding_path_availability_with_source_mn_opts {
  enum options { direct_path_available, /*...*/ nulltype } value;

  const char* to_string() const;
};
using direct_forwarding_path_availability_with_source_mn_e =
    enumerated<direct_forwarding_path_availability_with_source_mn_opts, true>;

// E-UTRA-CoordinationAssistanceInfo ::= ENUMERATED
struct e_utra_coordination_assist_info_opts {
  enum options { coordination_not_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using e_utra_coordination_assist_info_e = enumerated<e_utra_coordination_assist_info_opts, true>;

// E-UTRA-ResourceCoordinationInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using e_utra_res_coordination_info_ext_ies_o = protocol_ext_empty_o;

using e_utra_res_coordination_info_ext_ies_container = protocol_ext_container_empty_l;

// E-UTRA-ResourceCoordinationInfo ::= SEQUENCE
struct e_utra_res_coordination_info_s {
  bool                                           dl_coordination_info_present            = false;
  bool                                           nr_cell_present                         = false;
  bool                                           e_utra_coordination_assist_info_present = false;
  bool                                           ie_ext_present                          = false;
  e_utra_cgi_s                                   e_utra_cell;
  bounded_bitstring<6, 4400, false, true>        ul_coordination_info;
  bounded_bitstring<6, 4400, false, true>        dl_coordination_info;
  nr_cgi_s                                       nr_cell;
  e_utra_coordination_assist_info_e              e_utra_coordination_assist_info;
  e_utra_res_coordination_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-UTRAMultibandInfoList ::= SEQUENCE (SIZE (1..16)) OF INTEGER (1..256,...)
using e_utra_multiband_info_list_l = bounded_array<uint16_t, 16>;

// E-UTRAPRACHConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using e_utraprach_cfg_ext_ies_o = protocol_ext_empty_o;

using e_utraprach_cfg_ext_ies_container = protocol_ext_container_empty_l;

// E-UTRAPRACHConfiguration ::= SEQUENCE
struct e_utraprach_cfg_s {
  struct high_speed_flag_opts {
    enum options { true_value, false_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using high_speed_flag_e_ = enumerated<high_speed_flag_opts, true>;

  // member variables
  bool                              prach_cfg_idx_present = false;
  bool                              ie_exts_present       = false;
  uint16_t                          root_seq_idx          = 0;
  uint8_t                           zero_correlation_idx  = 0;
  high_speed_flag_e_                high_speed_flag;
  uint8_t                           prach_freq_offset = 0;
  uint8_t                           prach_cfg_idx     = 0;
  e_utraprach_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-UTRATransmissionBandwidth ::= ENUMERATED
struct e_utra_tx_bw_opts {
  enum options { bw6, bw15, bw25, bw50, bw75, bw100, /*...*/ bw1, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using e_utra_tx_bw_e = enumerated<e_utra_tx_bw_opts, true, 1>;

// ECNMarkingAtRANRequest ::= ENUMERATED
struct ecn_marking_at_ra_nr_equest_opts {
  enum options { ul, dl, both, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ecn_marking_at_ra_nr_equest_e = enumerated<ecn_marking_at_ra_nr_equest_opts, true>;

// ECNMarkingAtUPFRequest ::= ENUMERATED
struct ecn_marking_at_up_f_request_opts {
  enum options { ul, dl, both, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ecn_marking_at_up_f_request_e = enumerated<ecn_marking_at_up_f_request_opts, true>;

// ECNMarkingorCongestionInformationReportingRequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ecn_markingor_congestion_info_report_request_ext_ies_o = protocol_ies_empty_o;

// ECNMarkingorCongestionInformationReportingRequest ::= CHOICE
struct ecn_markingor_congestion_info_report_request_c {
  struct types_opts {
    enum options {
      ecn_marking_at_ra_nr_equest,
      ecn_marking_at_up_f_request,
      congestion_info_request,
      choice_exts,
      nulltype
    } value;

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
  ecn_marking_at_ra_nr_equest_e& ecn_marking_at_ra_nr_equest()
  {
    assert_choice_type(types::ecn_marking_at_ra_nr_equest, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<ecn_marking_at_ra_nr_equest_e>();
  }
  ecn_marking_at_up_f_request_e& ecn_marking_at_up_f_request()
  {
    assert_choice_type(types::ecn_marking_at_up_f_request, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<ecn_marking_at_up_f_request_e>();
  }
  congestion_info_request_e& congestion_info_request()
  {
    assert_choice_type(types::congestion_info_request, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<congestion_info_request_e>();
  }
  protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>& choice_exts()
  {
    assert_choice_type(types::choice_exts, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>();
  }
  const ecn_marking_at_ra_nr_equest_e& ecn_marking_at_ra_nr_equest() const
  {
    assert_choice_type(types::ecn_marking_at_ra_nr_equest, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<ecn_marking_at_ra_nr_equest_e>();
  }
  const ecn_marking_at_up_f_request_e& ecn_marking_at_up_f_request() const
  {
    assert_choice_type(types::ecn_marking_at_up_f_request, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<ecn_marking_at_up_f_request_e>();
  }
  const congestion_info_request_e& congestion_info_request() const
  {
    assert_choice_type(types::congestion_info_request, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<congestion_info_request_e>();
  }
  const protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>& choice_exts() const
  {
    assert_choice_type(types::choice_exts, type_, "ECNMarkingorCongestionInformationReportingRequest");
    return c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>();
  }
  ecn_marking_at_ra_nr_equest_e& set_ecn_marking_at_ra_nr_equest();
  ecn_marking_at_up_f_request_e& set_ecn_marking_at_up_f_request();
  congestion_info_request_e&     set_congestion_info_request();
  protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>& set_choice_exts();

private:
  types                                                                                                   type_;
  choice_buffer_t<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>> c;

  void destroy_();
};

// EUTRAPaging-Time-Window ::= ENUMERATED
struct eutra_paging_time_win_opts {
  enum options { s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using eutra_paging_time_win_e = enumerated<eutra_paging_time_win_opts, true>;

// EUTRAPaging-eDRX-Cycle ::= ENUMERATED
struct eutra_paging_e_drx_cycle_opts {
  enum options {
    hfhalf,
    hf1,
    hf2,
    hf4,
    hf6,
    hf8,
    hf10,
    hf12,
    hf14,
    hf16,
    hf32,
    hf64,
    hf128,
    hf256,
    /*...*/ nulltype
  } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using eutra_paging_e_drx_cycle_e = enumerated<eutra_paging_e_drx_cycle_opts, true>;

// EUTRAPagingeDRXInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using eutra_paginge_drx_info_ext_ies_o = protocol_ext_empty_o;

using eutra_paginge_drx_info_ext_ies_container = protocol_ext_container_empty_l;

// EUTRAPagingeDRXInformation ::= SEQUENCE
struct eutra_paginge_drx_info_s {
  bool                                     eutrapaging_time_win_present = false;
  bool                                     ie_exts_present              = false;
  eutra_paging_e_drx_cycle_e               eutrapaging_e_drx_cycle;
  eutra_paging_time_win_e                  eutrapaging_time_win;
  eutra_paginge_drx_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EarlyMeasurement ::= ENUMERATED
struct early_meas_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using early_meas_e = enumerated<early_meas_opts, true>;

// SNPNIdentity-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_pn_id_ext_ies_o = protocol_ext_empty_o;

using sn_pn_id_ext_ies_container = protocol_ext_container_empty_l;

// SNPNIdentity ::= SEQUENCE
struct sn_pn_id_s {
  bool                             ie_exts_present = false;
  fixed_octstring<3, true>         plmn_id;
  fixed_bitstring<44, false, true> nid;
  sn_pn_id_ext_ies_container       ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EquivalentSNPNs ::= SEQUENCE (SIZE (1..15)) OF SNPNIdentity
using equivalent_sn_p_ns_l = dyn_array<sn_pn_id_s>;

// MeasurementThresholdL1LoggedMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using meas_thres_l1_logged_mdt_ext_ies_o = protocol_ies_empty_o;

// MeasurementThresholdL1LoggedMDT ::= CHOICE
struct meas_thres_l1_logged_mdt_c {
  struct types_opts {
    enum options { thres_rsrp, thres_rsrq, /*...*/ choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true, 1>;

  // choice methods
  meas_thres_l1_logged_mdt_c() = default;
  meas_thres_l1_logged_mdt_c(const meas_thres_l1_logged_mdt_c& other);
  meas_thres_l1_logged_mdt_c& operator=(const meas_thres_l1_logged_mdt_c& other);
  ~meas_thres_l1_logged_mdt_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& thres_rsrp()
  {
    assert_choice_type(types::thres_rsrp, type_, "MeasurementThresholdL1LoggedMDT");
    return c.get<uint8_t>();
  }
  uint8_t& thres_rsrq()
  {
    assert_choice_type(types::thres_rsrq, type_, "MeasurementThresholdL1LoggedMDT");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<meas_thres_l1_logged_mdt_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MeasurementThresholdL1LoggedMDT");
    return c.get<protocol_ie_single_container_s<meas_thres_l1_logged_mdt_ext_ies_o>>();
  }
  const uint8_t& thres_rsrp() const
  {
    assert_choice_type(types::thres_rsrp, type_, "MeasurementThresholdL1LoggedMDT");
    return c.get<uint8_t>();
  }
  const uint8_t& thres_rsrq() const
  {
    assert_choice_type(types::thres_rsrq, type_, "MeasurementThresholdL1LoggedMDT");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<meas_thres_l1_logged_mdt_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MeasurementThresholdL1LoggedMDT");
    return c.get<protocol_ie_single_container_s<meas_thres_l1_logged_mdt_ext_ies_o>>();
  }
  uint8_t&                                                            set_thres_rsrp();
  uint8_t&                                                            set_thres_rsrq();
  protocol_ie_single_container_s<meas_thres_l1_logged_mdt_ext_ies_o>& set_choice_ext();

private:
  types                                                                               type_;
  choice_buffer_t<protocol_ie_single_container_s<meas_thres_l1_logged_mdt_ext_ies_o>> c;

  void destroy_();
};

// TimeToTrigger ::= ENUMERATED
struct time_to_trigger_opts {
  enum options {
    ms0,
    ms40,
    ms64,
    ms80,
    ms100,
    ms128,
    ms160,
    ms256,
    ms320,
    ms480,
    ms512,
    ms640,
    ms1024,
    ms1280,
    ms2560,
    ms5120,
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using time_to_trigger_e = enumerated<time_to_trigger_opts>;

// EventL1-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using event_l1_ext_ies_o = protocol_ext_empty_o;

using event_l1_ext_ies_container = protocol_ext_container_empty_l;

// EventL1 ::= SEQUENCE
struct event_l1_s {
  bool                       ie_exts_present = false;
  meas_thres_l1_logged_mdt_c l1_thres;
  uint8_t                    hysteresis = 0;
  time_to_trigger_e          time_to_trigger;
  event_l1_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventTypeTrigger-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using event_type_trigger_ext_ies_o = protocol_ies_empty_o;

// EventTypeTrigger ::= CHOICE
struct event_type_trigger_c {
  struct out_of_coverage_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using out_of_coverage_e_ = enumerated<out_of_coverage_opts, true>;
  struct types_opts {
    enum options { out_of_coverage, event_l1, choice_exts, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  event_type_trigger_c() = default;
  event_type_trigger_c(const event_type_trigger_c& other);
  event_type_trigger_c& operator=(const event_type_trigger_c& other);
  ~event_type_trigger_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  out_of_coverage_e_& out_of_coverage()
  {
    assert_choice_type(types::out_of_coverage, type_, "EventTypeTrigger");
    return c.get<out_of_coverage_e_>();
  }
  event_l1_s& event_l1()
  {
    assert_choice_type(types::event_l1, type_, "EventTypeTrigger");
    return c.get<event_l1_s>();
  }
  protocol_ie_single_container_s<event_type_trigger_ext_ies_o>& choice_exts()
  {
    assert_choice_type(types::choice_exts, type_, "EventTypeTrigger");
    return c.get<protocol_ie_single_container_s<event_type_trigger_ext_ies_o>>();
  }
  const out_of_coverage_e_& out_of_coverage() const
  {
    assert_choice_type(types::out_of_coverage, type_, "EventTypeTrigger");
    return c.get<out_of_coverage_e_>();
  }
  const event_l1_s& event_l1() const
  {
    assert_choice_type(types::event_l1, type_, "EventTypeTrigger");
    return c.get<event_l1_s>();
  }
  const protocol_ie_single_container_s<event_type_trigger_ext_ies_o>& choice_exts() const
  {
    assert_choice_type(types::choice_exts, type_, "EventTypeTrigger");
    return c.get<protocol_ie_single_container_s<event_type_trigger_ext_ies_o>>();
  }
  out_of_coverage_e_&                                           set_out_of_coverage();
  event_l1_s&                                                   set_event_l1();
  protocol_ie_single_container_s<event_type_trigger_ext_ies_o>& set_choice_exts();

private:
  types                                                                                     type_;
  choice_buffer_t<event_l1_s, protocol_ie_single_container_s<event_type_trigger_ext_ies_o>> c;

  void destroy_();
};

// LoggedEventTriggeredConfig-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using logged_event_triggered_cfg_ext_ies_o = protocol_ext_empty_o;

using logged_event_triggered_cfg_ext_ies_container = protocol_ext_container_empty_l;

// LoggedEventTriggeredConfig ::= SEQUENCE
struct logged_event_triggered_cfg_s {
  bool                                         ie_exts_present = false;
  event_type_trigger_c                         event_type_trigger;
  logged_event_triggered_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventTriggered-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using event_triggered_ext_ies_o = protocol_ext_empty_o;

using event_triggered_ext_ies_container = protocol_ext_container_empty_l;

// EventTriggered ::= SEQUENCE
struct event_triggered_s {
  bool                              ie_exts_present = false;
  logged_event_triggered_cfg_s      logged_event_triggered_cfg;
  event_triggered_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventType ::= ENUMERATED
struct event_type_opts {
  enum options {
    report_upon_change_of_serving_cell,
    report_ue_moving_presence_into_or_out_of_the_area_of_interest,
    // ...
    report_upon_change_of_serving_cell_and_area_of_interest,
    nulltype
  } value;

  const char* to_string() const;
};
using event_type_e = enumerated<event_type_opts, true, 1>;

// ExcessPacketDelayThresholdValue ::= ENUMERATED
struct excess_packet_delay_thres_value_opts {
  enum options {
    ms0dot25,
    ms0dot5,
    ms1,
    ms2,
    ms4,
    ms5,
    ms10,
    ms20,
    ms30,
    ms40,
    ms50,
    ms60,
    ms70,
    ms80,
    ms90,
    ms100,
    ms150,
    ms300,
    ms500,
    // ...
    nulltype
  } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using excess_packet_delay_thres_value_e = enumerated<excess_packet_delay_thres_value_opts, true>;

// ExcessPacketDelayThresholdItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using excess_packet_delay_thres_item_ext_ies_o = protocol_ext_empty_o;

using excess_packet_delay_thres_item_ext_ies_container = protocol_ext_container_empty_l;

// ExcessPacketDelayThresholdItem ::= SEQUENCE
struct excess_packet_delay_thres_item_s {
  bool                                             ie_exts_present = false;
  uint16_t                                         five_qi         = 0;
  excess_packet_delay_thres_value_e                excess_packet_delay_thres_value;
  excess_packet_delay_thres_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExcessPacketDelayThresholdConfiguration ::= SEQUENCE (SIZE (1..255)) OF ExcessPacketDelayThresholdItem
using excess_packet_delay_thres_cfg_l = dyn_array<excess_packet_delay_thres_item_s>;

// ExpectedHOInterval ::= ENUMERATED
struct expected_ho_interv_opts {
  enum options { sec15, sec30, sec60, sec90, sec120, sec180, long_time, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using expected_ho_interv_e = enumerated<expected_ho_interv_opts, true>;

// SourceOfUEActivityBehaviourInformation ::= ENUMERATED
struct source_of_ue_activity_behaviour_info_opts {
  enum options { sub_info, stats, /*...*/ nulltype } value;

  const char* to_string() const;
};
using source_of_ue_activity_behaviour_info_e = enumerated<source_of_ue_activity_behaviour_info_opts, true>;

// ExpectedUEActivityBehaviour-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using expected_ue_activity_behaviour_ext_ies_o = protocol_ext_empty_o;

using expected_ue_activity_behaviour_ext_ies_container = protocol_ext_container_empty_l;

// ExpectedUEActivityBehaviour ::= SEQUENCE
struct expected_ue_activity_behaviour_s {
  bool                                             expected_activity_period_present             = false;
  bool                                             expected_idle_period_present                 = false;
  bool                                             source_of_ue_activity_behaviour_info_present = false;
  bool                                             ie_exts_present                              = false;
  uint16_t                                         expected_activity_period                     = 1;
  uint16_t                                         expected_idle_period                         = 1;
  source_of_ue_activity_behaviour_info_e           source_of_ue_activity_behaviour_info;
  expected_ue_activity_behaviour_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExpectedUEMobility ::= ENUMERATED
struct expected_ue_mob_opts {
  enum options { stationary, mobile, /*...*/ nulltype } value;

  const char* to_string() const;
};
using expected_ue_mob_e = enumerated<expected_ue_mob_opts, true>;

// ExpectedUEMovingTrajectoryItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using expected_ue_moving_trajectory_item_ext_ies_o = protocol_ext_empty_o;

using expected_ue_moving_trajectory_item_ext_ies_container = protocol_ext_container_empty_l;

// ExpectedUEMovingTrajectoryItem ::= SEQUENCE
struct expected_ue_moving_trajectory_item_s {
  bool                                                 time_stayed_in_cell_present = false;
  bool                                                 ie_exts_present             = false;
  global_ng_ran_cell_id_s                              ngran_cgi;
  uint16_t                                             time_stayed_in_cell = 0;
  expected_ue_moving_trajectory_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExpectedUEMovingTrajectory ::= SEQUENCE (SIZE (1..16)) OF ExpectedUEMovingTrajectoryItem
using expected_ue_moving_trajectory_l = dyn_array<expected_ue_moving_trajectory_item_s>;

// ExpectedUEBehaviour-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using expected_ue_behaviour_ext_ies_o = protocol_ext_empty_o;

using expected_ue_behaviour_ext_ies_container = protocol_ext_container_empty_l;

// ExpectedUEBehaviour ::= SEQUENCE
struct expected_ue_behaviour_s {
  bool                                    expected_ue_activity_behaviour_present = false;
  bool                                    expected_ho_interv_present             = false;
  bool                                    expected_ue_mob_present                = false;
  bool                                    ie_exts_present                        = false;
  expected_ue_activity_behaviour_s        expected_ue_activity_behaviour;
  expected_ho_interv_e                    expected_ho_interv;
  expected_ue_mob_e                       expected_ue_mob;
  expected_ue_moving_trajectory_l         expected_ue_moving_trajectory;
  expected_ue_behaviour_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GTPTLA-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using gtp_tla_item_ext_ies_o = protocol_ext_empty_o;

using gtp_tla_item_ext_ies_container = protocol_ext_container_empty_l;

// GTPTLA-Item ::= SEQUENCE
struct gtp_tla_item_s {
  bool                                  ie_exts_present = false;
  bounded_bitstring<1, 160, true, true> gtp_transport_layer_addresses;
  gtp_tla_item_ext_ies_container        ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GTPTLAs ::= SEQUENCE (SIZE (1..16)) OF GTPTLA-Item
using gtp_tlas_l = dyn_array<gtp_tla_item_s>;

// ExtTLA-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ext_tla_item_ext_ies_o = protocol_ext_empty_o;

using ext_tla_item_ext_ies_container = protocol_ext_container_empty_l;

// ExtTLA-Item ::= SEQUENCE
struct ext_tla_item_s {
  bool                                  ip_sec_tla_present = false;
  bool                                  ie_exts_present    = false;
  bounded_bitstring<1, 160, true, true> ip_sec_tla;
  gtp_tlas_l                            gtp_transport_layer_addresses;
  ext_tla_item_ext_ies_container        ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExtTLAs ::= SEQUENCE (SIZE (1..16)) OF ExtTLA-Item
using ext_tlas_l = dyn_array<ext_tla_item_s>;

// ExtendedRATRestrictionInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using extended_rat_restrict_info_ext_ies_o = protocol_ext_empty_o;

using extended_rat_restrict_info_ext_ies_container = protocol_ext_container_empty_l;

// ExtendedRATRestrictionInformation ::= SEQUENCE
struct extended_rat_restrict_info_s {
  bool                                         ie_exts_present = false;
  fixed_bitstring<8, true, true>               primary_rat_restrict;
  fixed_bitstring<8, true, true>               secondary_rat_restrict;
  extended_rat_restrict_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ExtendedReportIntervalMDT ::= ENUMERATED
struct extended_report_interv_mdt_opts {
  enum options { ms20480, ms40960, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using extended_report_interv_mdt_e = enumerated<extended_report_interv_mdt_opts, true>;

// IABTNLAddress-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
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

// ULF1Terminating-BHInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ul_f1_terminating_bh_info_ext_ies_o = protocol_ext_empty_o;

using ul_f1_terminating_bh_info_ext_ies_container = protocol_ext_container_empty_l;

// ULF1Terminating-BHInfo ::= SEQUENCE
struct ul_f1_terminating_bh_info_s {
  bool                                        ie_exts_present = false;
  bap_routing_id_s                            ingress_bap_routing_id;
  fixed_bitstring<16, false, true>            ingress_bh_rlc_ch_id;
  ul_f1_terminating_bh_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// F1TerminatingBHInformation-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using f1_terminating_bh_info_item_ext_ies_o = protocol_ext_empty_o;

using f1_terminating_bh_info_item_ext_ies_container = protocol_ext_container_empty_l;

// F1TerminatingBHInformation-Item ::= SEQUENCE
struct f1_terminating_bh_info_item_s {
  bool                                          dl_f1_terminating_bh_info_present = false;
  bool                                          ul_f1_terminating_bh_info_present = false;
  bool                                          ie_ext_present                    = false;
  uint16_t                                      bh_info_idx                       = 1;
  iab_tnl_address_c                             dl_tnl_address;
  dl_f1_terminating_bh_info_s                   dl_f1_terminating_bh_info;
  ul_f1_terminating_bh_info_s                   ul_f1_terminating_bh_info;
  f1_terminating_bh_info_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// F1TerminatingBHInformation-List ::= SEQUENCE (SIZE (1..1024)) OF F1TerminatingBHInformation-Item
using f1_terminating_bh_info_list_l = dyn_array<f1_terminating_bh_info_item_s>;

// F1-TerminatingTopologyBHInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using f1_terminating_topology_bh_info_ext_ies_o = protocol_ext_empty_o;

using f1_terminating_topology_bh_info_ext_ies_container = protocol_ext_container_empty_l;

// F1-TerminatingTopologyBHInformation ::= SEQUENCE
struct f1_terminating_topology_bh_info_s {
  bool                                              ie_exts_present = false;
  f1_terminating_bh_info_list_l                     f1_terminating_bh_info_list;
  f1_terminating_topology_bh_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// F1-terminatingIAB-donorIndicator ::= ENUMERATED
struct f1_terminating_iab_donor_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using f1_terminating_iab_donor_ind_e = enumerated<f1_terminating_iab_donor_ind_opts, true>;

// FDD-or-TDD-in-NPRACHConfiguration-Choice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using fdd_or_tdd_in_nprach_cfg_choice_ext_ies_o = protocol_ies_empty_o;

// FiveGProSeDirectDiscovery ::= ENUMERATED
struct five_g_pro_se_direct_discovery_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_direct_discovery_e = enumerated<five_g_pro_se_direct_discovery_opts, true>;

// FiveGProSeDirectCommunication ::= ENUMERATED
struct five_g_pro_se_direct_communication_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_direct_communication_e = enumerated<five_g_pro_se_direct_communication_opts, true>;

// FiveGProSeLayer2UEtoNetworkRelay ::= ENUMERATED
struct five_g_pro_se_layer2_ue_to_network_relay_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_ue_to_network_relay_e = enumerated<five_g_pro_se_layer2_ue_to_network_relay_opts, true>;

// FiveGProSeLayer3UEtoNetworkRelay ::= ENUMERATED
struct five_g_pro_se_layer3_ue_to_network_relay_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer3_ue_to_network_relay_e = enumerated<five_g_pro_se_layer3_ue_to_network_relay_opts, true>;

// FiveGProSeLayer2RemoteUE ::= ENUMERATED
struct five_g_pro_se_layer2_remote_ue_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_remote_ue_e = enumerated<five_g_pro_se_layer2_remote_ue_opts, true>;

// FiveGProSeLayer2Multipath ::= ENUMERATED
struct five_g_pro_se_layer2_multipath_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_multipath_e = enumerated<five_g_pro_se_layer2_multipath_opts, true>;

// FiveGProSeLayer2UEtoUERelay ::= ENUMERATED
struct five_g_pro_se_layer2_ue_to_ue_relay_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_ue_to_ue_relay_e = enumerated<five_g_pro_se_layer2_ue_to_ue_relay_opts, true>;

// FiveGProSeLayer2UEtoUERemote ::= ENUMERATED
struct five_g_pro_se_layer2_ue_to_ue_remote_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using five_g_pro_se_layer2_ue_to_ue_remote_e = enumerated<five_g_pro_se_layer2_ue_to_ue_remote_opts, true>;

// FiveGProSeAuthorized-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// FiveGProSeAuthorized ::= SEQUENCE
struct five_g_pro_se_authorized_s {
  bool                                       five_gpro_se_direct_discovery_present              = false;
  bool                                       five_gpro_se_direct_communication_present          = false;
  bool                                       five_gnr_pro_se_layer2_ue_to_network_relay_present = false;
  bool                                       five_gnr_pro_se_layer3_ue_to_network_relay_present = false;
  bool                                       five_gnr_pro_se_layer2_remote_ue_present           = false;
  bool                                       ie_exts_present                                    = false;
  five_g_pro_se_direct_discovery_e           five_gpro_se_direct_discovery;
  five_g_pro_se_direct_communication_e       five_gpro_se_direct_communication;
  five_g_pro_se_layer2_ue_to_network_relay_e five_gnr_pro_se_layer2_ue_to_network_relay;
  five_g_pro_se_layer3_ue_to_network_relay_e five_gnr_pro_se_layer3_ue_to_network_relay;
  five_g_pro_se_layer2_remote_ue_e           five_gnr_pro_se_layer2_remote_ue;
  five_g_pro_se_authorized_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FiveGProSePC5FlowBitRates-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using five_g_pro_se_pc5_flow_bit_rates_ext_ies_o = protocol_ext_empty_o;

using five_g_pro_se_pc5_flow_bit_rates_ext_ies_container = protocol_ext_container_empty_l;

// FiveGProSePC5FlowBitRates ::= SEQUENCE
struct five_g_pro_se_pc5_flow_bit_rates_s {
  bool                                               ie_exts_present                      = false;
  uint64_t                                           five_gpro_seguaranteed_flow_bit_rate = 0;
  uint64_t                                           five_gpro_semax_flow_bit_rate        = 0;
  five_g_pro_se_pc5_flow_bit_rates_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FiveGProSePC5QoSFlowItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using five_g_pro_se_pc5_qos_flow_item_ext_ies_o = protocol_ext_empty_o;

using five_g_pro_se_pc5_qos_flow_item_ext_ies_container = protocol_ext_container_empty_l;

// FiveGProSePC5QoSFlowItem ::= SEQUENCE
struct five_g_pro_se_pc5_qos_flow_item_s {
  bool                                              five_gpro_sepc5_flow_bit_rates_present = false;
  bool                                              five_gpro_serange_present              = false;
  bool                                              ie_exts_present                        = false;
  uint16_t                                          five_gpro_sep_qi                       = 0;
  five_g_pro_se_pc5_flow_bit_rates_s                five_gpro_sepc5_flow_bit_rates;
  range_e                                           five_gpro_serange;
  five_g_pro_se_pc5_qos_flow_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FiveGProSePC5QoSFlowList ::= SEQUENCE (SIZE (1..2064)) OF FiveGProSePC5QoSFlowItem
using five_g_pro_se_pc5_qos_flow_list_l = dyn_array<five_g_pro_se_pc5_qos_flow_item_s>;

// FiveGProSePC5QoSParameters-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using five_g_pro_se_pc5_qos_params_ext_ies_o = protocol_ext_empty_o;

using five_g_pro_se_pc5_qos_params_ext_ies_container = protocol_ext_container_empty_l;

// FiveGProSePC5QoSParameters ::= SEQUENCE
struct five_g_pro_se_pc5_qos_params_s {
  bool                                           five_gpro_sepc5_link_aggr_bit_rates_present = false;
  bool                                           ie_exts_present                             = false;
  five_g_pro_se_pc5_qos_flow_list_l              five_g_pro_sepc5_qos_flow_list;
  uint64_t                                       five_gpro_sepc5_link_aggr_bit_rates = 0;
  five_g_pro_se_pc5_qos_params_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Flows-Mapped-To-DRB-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using flows_mapped_to_drb_item_ext_ies_o = protocol_ext_empty_o;

using flows_mapped_to_drb_item_ext_ies_container = protocol_ext_container_empty_l;

// Flows-Mapped-To-DRB-Item ::= SEQUENCE
struct flows_mapped_to_drb_item_s {
  bool                                       qos_flow_map_ind_present = false;
  bool                                       ie_exts_present          = false;
  uint8_t                                    qos_flow_id              = 0;
  qos_flow_level_qos_params_s                qos_flow_level_qos_params;
  qos_flow_map_ind_e                         qos_flow_map_ind;
  flows_mapped_to_drb_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Flows-Mapped-To-DRB-List ::= SEQUENCE (SIZE (1..64)) OF Flows-Mapped-To-DRB-Item
using flows_mapped_to_drb_list_l = dyn_array<flows_mapped_to_drb_item_s>;

// ForbiddenAreaItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using forbidden_area_item_ext_ies_o = protocol_ext_empty_o;

using forbidden_area_item_ext_ies_container = protocol_ext_container_empty_l;

// ForbiddenAreaItem ::= SEQUENCE
struct forbidden_area_item_s {
  using forbidden_tacs_l_ = dyn_array<fixed_octstring<3, true>>;

  // member variables
  bool                                  ie_exts_present = false;
  fixed_octstring<3, true>              plmn_id;
  forbidden_tacs_l_                     forbidden_tacs;
  forbidden_area_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ForbiddenAreaList ::= SEQUENCE (SIZE (1..16)) OF ForbiddenAreaItem
using forbidden_area_list_l = dyn_array<forbidden_area_item_s>;

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

// FreqDomainSlotHSNAconfiguration-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using freq_domain_slot_h_sn_acfg_list_item_ext_ies_o = protocol_ext_empty_o;

using freq_domain_slot_h_sn_acfg_list_item_ext_ies_container = protocol_ext_container_empty_l;

// FreqDomainSlotHSNAconfiguration-List-Item ::= SEQUENCE
struct freq_domain_slot_h_sn_acfg_list_item_s {
  bool                                                   hsn_a_dl_present   = false;
  bool                                                   hsn_a_ul_present   = false;
  bool                                                   hsn_a_flex_present = false;
  bool                                                   ie_exts_present    = false;
  uint16_t                                               slot_idx           = 1;
  h_sn_a_dl_e                                            hsn_a_dl;
  h_sn_a_ul_e                                            hsn_a_ul;
  h_sn_a_flex_e                                          hsn_a_flex;
  freq_domain_slot_h_sn_acfg_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FreqDomainSlotHSNAconfiguration-List ::= SEQUENCE (SIZE (1..5120)) OF FreqDomainSlotHSNAconfiguration-List-Item
using freq_domain_slot_h_sn_acfg_list_l = dyn_array<freq_domain_slot_h_sn_acfg_list_item_s>;

// FreqDomainHSNAconfiguration-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using freq_domain_h_sn_acfg_list_item_ext_ies_o = protocol_ext_empty_o;

using freq_domain_h_sn_acfg_list_item_ext_ies_container = protocol_ext_container_empty_l;

// FreqDomainHSNAconfiguration-List-Item ::= SEQUENCE
struct freq_domain_h_sn_acfg_list_item_s {
  bool                                              ie_exts_present = false;
  uint8_t                                           rbset_idx       = 0;
  freq_domain_slot_h_sn_acfg_list_l                 freq_domain_slot_h_sn_acfg_list;
  freq_domain_h_sn_acfg_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FreqDomainHSNAconfiguration-List ::= SEQUENCE (SIZE (1..5120)) OF FreqDomainHSNAconfiguration-List-Item
using freq_domain_h_sn_acfg_list_l = dyn_array<freq_domain_h_sn_acfg_list_item_s>;

// Full-I-RNTI-Profile-List-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using full_i_rnti_profile_list_ext_ies_o = protocol_ies_empty_o;

// Full-I-RNTI-Profile-List ::= CHOICE
struct full_i_rnti_profile_list_c {
  struct types_opts {
    enum options {
      full_i_rnti_profile_0,
      full_i_rnti_profile_1,
      full_i_rnti_profile_2,
      full_i_rnti_profile_3,
      choice_ext,
      nulltype
    } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  full_i_rnti_profile_list_c() = default;
  full_i_rnti_profile_list_c(const full_i_rnti_profile_list_c& other);
  full_i_rnti_profile_list_c& operator=(const full_i_rnti_profile_list_c& other);
  ~full_i_rnti_profile_list_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<21, false, true>& full_i_rnti_profile_0()
  {
    assert_choice_type(types::full_i_rnti_profile_0, type_, "Full-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<21, false, true>>();
  }
  fixed_bitstring<18, false, true>& full_i_rnti_profile_1()
  {
    assert_choice_type(types::full_i_rnti_profile_1, type_, "Full-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<18, false, true>>();
  }
  fixed_bitstring<15, false, true>& full_i_rnti_profile_2()
  {
    assert_choice_type(types::full_i_rnti_profile_2, type_, "Full-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<15, false, true>>();
  }
  fixed_bitstring<12, false, true>& full_i_rnti_profile_3()
  {
    assert_choice_type(types::full_i_rnti_profile_3, type_, "Full-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<12, false, true>>();
  }
  protocol_ie_single_container_s<full_i_rnti_profile_list_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "Full-I-RNTI-Profile-List");
    return c.get<protocol_ie_single_container_s<full_i_rnti_profile_list_ext_ies_o>>();
  }
  const fixed_bitstring<21, false, true>& full_i_rnti_profile_0() const
  {
    assert_choice_type(types::full_i_rnti_profile_0, type_, "Full-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<21, false, true>>();
  }
  const fixed_bitstring<18, false, true>& full_i_rnti_profile_1() const
  {
    assert_choice_type(types::full_i_rnti_profile_1, type_, "Full-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<18, false, true>>();
  }
  const fixed_bitstring<15, false, true>& full_i_rnti_profile_2() const
  {
    assert_choice_type(types::full_i_rnti_profile_2, type_, "Full-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<15, false, true>>();
  }
  const fixed_bitstring<12, false, true>& full_i_rnti_profile_3() const
  {
    assert_choice_type(types::full_i_rnti_profile_3, type_, "Full-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<12, false, true>>();
  }
  const protocol_ie_single_container_s<full_i_rnti_profile_list_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "Full-I-RNTI-Profile-List");
    return c.get<protocol_ie_single_container_s<full_i_rnti_profile_list_ext_ies_o>>();
  }
  fixed_bitstring<21, false, true>&                                   set_full_i_rnti_profile_0();
  fixed_bitstring<18, false, true>&                                   set_full_i_rnti_profile_1();
  fixed_bitstring<15, false, true>&                                   set_full_i_rnti_profile_2();
  fixed_bitstring<12, false, true>&                                   set_full_i_rnti_profile_3();
  protocol_ie_single_container_s<full_i_rnti_profile_list_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<21, false, true>, protocol_ie_single_container_s<full_i_rnti_profile_list_ext_ies_o>>
      c;

  void destroy_();
};

// Short-I-RNTI-Profile-List-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using short_i_rnti_profile_list_ext_ies_o = protocol_ies_empty_o;

// Short-I-RNTI-Profile-List ::= CHOICE
struct short_i_rnti_profile_list_c {
  struct types_opts {
    enum options { short_i_rnti_profile_0, short_i_rnti_profile_1, choice_ext, nulltype } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  short_i_rnti_profile_list_c() = default;
  short_i_rnti_profile_list_c(const short_i_rnti_profile_list_c& other);
  short_i_rnti_profile_list_c& operator=(const short_i_rnti_profile_list_c& other);
  ~short_i_rnti_profile_list_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<8, false, true>& short_i_rnti_profile_0()
  {
    assert_choice_type(types::short_i_rnti_profile_0, type_, "Short-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  fixed_bitstring<6, false, true>& short_i_rnti_profile_1()
  {
    assert_choice_type(types::short_i_rnti_profile_1, type_, "Short-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<6, false, true>>();
  }
  protocol_ie_single_container_s<short_i_rnti_profile_list_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "Short-I-RNTI-Profile-List");
    return c.get<protocol_ie_single_container_s<short_i_rnti_profile_list_ext_ies_o>>();
  }
  const fixed_bitstring<8, false, true>& short_i_rnti_profile_0() const
  {
    assert_choice_type(types::short_i_rnti_profile_0, type_, "Short-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<8, false, true>>();
  }
  const fixed_bitstring<6, false, true>& short_i_rnti_profile_1() const
  {
    assert_choice_type(types::short_i_rnti_profile_1, type_, "Short-I-RNTI-Profile-List");
    return c.get<fixed_bitstring<6, false, true>>();
  }
  const protocol_ie_single_container_s<short_i_rnti_profile_list_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "Short-I-RNTI-Profile-List");
    return c.get<protocol_ie_single_container_s<short_i_rnti_profile_list_ext_ies_o>>();
  }
  fixed_bitstring<8, false, true>&                                     set_short_i_rnti_profile_0();
  fixed_bitstring<6, false, true>&                                     set_short_i_rnti_profile_1();
  protocol_ie_single_container_s<short_i_rnti_profile_list_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<8, false, true>, protocol_ie_single_container_s<short_i_rnti_profile_list_ext_ies_o>>
      c;

  void destroy_();
};

// Full-and-Short-I-RNTI-Profile-List-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using full_and_short_i_rnti_profile_list_ext_ies_o = protocol_ext_empty_o;

using full_and_short_i_rnti_profile_list_ext_ies_container = protocol_ext_container_empty_l;

// Full-and-Short-I-RNTI-Profile-List ::= SEQUENCE
struct full_and_short_i_rnti_profile_list_s {
  bool                                                 ie_exts_present = false;
  full_i_rnti_profile_list_c                           full_i_rnti_profile_list;
  short_i_rnti_profile_list_c                          short_i_rnti_profile_list;
  full_and_short_i_rnti_profile_list_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SubcarrierSpacing ::= ENUMERATED
struct subcarrier_spacing_opts {
  enum options { khz15, khz30, khz120, khz240, spare3, spare2, spare1, /*...*/ khz60, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using subcarrier_spacing_e = enumerated<subcarrier_spacing_opts, true, 1>;

// HSNATransmissionPeriodicity ::= ENUMERATED
struct h_sn_a_tx_periodicity_opts {
  enum options { ms0p5, ms0p625, ms1, ms1p25, ms2, ms2p5, ms5, ms10, ms20, ms40, ms80, ms160, /*...*/ nulltype } value;

  const char* to_string() const;
};
using h_sn_a_tx_periodicity_e = enumerated<h_sn_a_tx_periodicity_opts, true>;

// HSNASlotConfigItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HSNASlotConfigList ::= SEQUENCE (SIZE (1..5120)) OF HSNASlotConfigItem
using hs_nas_lot_cfg_list_l = dyn_array<hs_nas_lot_cfg_item_s>;

// RBsetConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using r_bset_cfg_ext_ies_o = protocol_ext_empty_o;

using r_bset_cfg_ext_ies_container = protocol_ext_container_empty_l;

// RBsetConfiguration ::= SEQUENCE
struct r_bset_cfg_s {
  struct rbset_size_opts {
    enum options { rb2, rb4, rb8, rb16, rb32, rb64, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using rbset_size_e_ = enumerated<rbset_size_opts>;

  // member variables
  bool                         ie_exts_present = false;
  subcarrier_spacing_e         subcarrier_spacing;
  rbset_size_e_                rbset_size;
  uint8_t                      numof_rb_sets = 1;
  r_bset_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NACellResourceConfiguration-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using na_cell_res_cfg_item_ext_ies_o = protocol_ext_empty_o;

using na_cell_res_cfg_item_ext_ies_container = protocol_ext_container_empty_l;

// NACellResourceConfiguration-Item ::= SEQUENCE
struct na_cell_res_cfg_item_s {
  struct nadl_opts {
    enum options { true_value, false_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using nadl_e_ = enumerated<nadl_opts, true>;
  struct naul_opts {
    enum options { true_value, false_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using naul_e_ = enumerated<naul_opts, true>;
  struct naflex_opts {
    enum options { true_value, false_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using naflex_e_ = enumerated<naflex_opts, true>;

  // member variables
  bool                                   nadl_present    = false;
  bool                                   naul_present    = false;
  bool                                   naflex_present  = false;
  bool                                   ie_exts_present = false;
  nadl_e_                                nadl;
  naul_e_                                naul;
  naflex_e_                              naflex;
  na_cell_res_cfg_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NACellResourceConfigurationList ::= SEQUENCE (SIZE (1..5120)) OF NACellResourceConfiguration-Item
using na_cell_res_cfg_list_l = dyn_array<na_cell_res_cfg_item_s>;

// GNB-DU-Cell-Resource-Configuration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using gnb_du_cell_res_cfg_ext_ies_o = protocol_ext_empty_o;

using gnb_du_cell_res_cfg_ext_ies_container = protocol_ext_container_empty_l;

// GNB-DU-Cell-Resource-Configuration ::= SEQUENCE
struct gnb_du_cell_res_cfg_s {
  bool                                  du_f_tx_periodicity_present = false;
  bool                                  rbset_cfg_present           = false;
  bool                                  ie_exts_present             = false;
  subcarrier_spacing_e                  subcarrier_spacing;
  du_f_tx_periodicity_e                 du_f_tx_periodicity;
  du_f_slot_cfg_list_l                  du_f_slot_cfg_list;
  h_sn_a_tx_periodicity_e               hsn_a_tx_periodicity;
  hs_nas_lot_cfg_list_l                 hnsa_slot_cfg_list;
  r_bset_cfg_s                          rbset_cfg;
  freq_domain_h_sn_acfg_list_l          freq_domain_h_sn_acfg_list;
  na_cell_res_cfg_list_l                na_cell_res_cfg_list;
  gnb_du_cell_res_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// GUAMI-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using guami_ext_ies_o = protocol_ext_empty_o;

using guami_ext_ies_container = protocol_ext_container_empty_l;

// GUAMI ::= SEQUENCE
struct guami_s {
  bool                             ie_exts_present = false;
  fixed_octstring<3, true>         plmn_id;
  fixed_bitstring<8, false, true>  amf_region_id;
  fixed_bitstring<10, false, true> amf_set_id;
  fixed_bitstring<6, false, true>  amf_pointer;
  guami_ext_ies_container          ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// HandoverReportType ::= ENUMERATED
struct ho_report_type_opts {
  enum options { ho_too_early, ho_to_wrong_cell, intersyspingpong, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ho_report_type_e = enumerated<ho_report_type_opts, true>;

// I-RNTI-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using i_rnti_ext_ies_o = protocol_ies_empty_o;

// I-RNTI ::= CHOICE
struct i_rnti_c {
  struct types_opts {
    enum options { i_rnti_full, i_rnti_short, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  i_rnti_c() = default;
  i_rnti_c(const i_rnti_c& other);
  i_rnti_c& operator=(const i_rnti_c& other);
  ~i_rnti_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<40, false, true>& i_rnti_full()
  {
    assert_choice_type(types::i_rnti_full, type_, "I-RNTI");
    return c.get<fixed_bitstring<40, false, true>>();
  }
  fixed_bitstring<24, false, true>& i_rnti_short()
  {
    assert_choice_type(types::i_rnti_short, type_, "I-RNTI");
    return c.get<fixed_bitstring<24, false, true>>();
  }
  protocol_ie_single_container_s<i_rnti_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "I-RNTI");
    return c.get<protocol_ie_single_container_s<i_rnti_ext_ies_o>>();
  }
  const fixed_bitstring<40, false, true>& i_rnti_full() const
  {
    assert_choice_type(types::i_rnti_full, type_, "I-RNTI");
    return c.get<fixed_bitstring<40, false, true>>();
  }
  const fixed_bitstring<24, false, true>& i_rnti_short() const
  {
    assert_choice_type(types::i_rnti_short, type_, "I-RNTI");
    return c.get<fixed_bitstring<24, false, true>>();
  }
  const protocol_ie_single_container_s<i_rnti_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "I-RNTI");
    return c.get<protocol_ie_single_container_s<i_rnti_ext_ies_o>>();
  }
  fixed_bitstring<40, false, true>&                 set_i_rnti_full();
  fixed_bitstring<24, false, true>&                 set_i_rnti_short();
  protocol_ie_single_container_s<i_rnti_ext_ies_o>& set_choice_ext();

private:
  types                                                                                               type_;
  choice_buffer_t<fixed_bitstring<40, false, true>, protocol_ie_single_container_s<i_rnti_ext_ies_o>> c;

  void destroy_();
};

// IAB-DU-Cell-Resource-Configuration-FDD-Info-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_du_cell_res_cfg_fdd_info_ext_ies_o = protocol_ext_empty_o;

using iab_du_cell_res_cfg_fdd_info_ext_ies_container = protocol_ext_container_empty_l;

// IAB-DU-Cell-Resource-Configuration-FDD-Info ::= SEQUENCE
struct iab_du_cell_res_cfg_fdd_info_s {
  bool                                           ul_freq_info_present = false;
  bool                                           dl_freq_info_present = false;
  bool                                           ul_tx_bw_present     = false;
  bool                                           dl_tx_bw_present     = false;
  bool                                           ie_exts_present      = false;
  gnb_du_cell_res_cfg_s                          gnb_du_cell_res_cfg_fdd_ul;
  gnb_du_cell_res_cfg_s                          gnb_du_cell_res_cfg_fdd_dl;
  nr_freq_info_s                                 ul_freq_info;
  nr_freq_info_s                                 dl_freq_info;
  nr_tx_bw_s                                     ul_tx_bw;
  nr_tx_bw_s                                     dl_tx_bw;
  nr_carrier_list_l                              ul_carrier_list;
  nr_carrier_list_l                              dl_carrier_list;
  iab_du_cell_res_cfg_fdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-DU-Cell-Resource-Configuration-TDD-Info-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_du_cell_res_cfg_tdd_info_ext_ies_o = protocol_ext_empty_o;

using iab_du_cell_res_cfg_tdd_info_ext_ies_container = protocol_ext_container_empty_l;

// IAB-DU-Cell-Resource-Configuration-TDD-Info ::= SEQUENCE
struct iab_du_cell_res_cfg_tdd_info_s {
  bool                                           freq_info_present = false;
  bool                                           tx_bw_present     = false;
  bool                                           ie_exts_present   = false;
  gnb_du_cell_res_cfg_s                          gnb_du_cell_res_cfg_tdd;
  nr_freq_info_s                                 freq_info;
  nr_tx_bw_s                                     tx_bw;
  nr_carrier_list_l                              carrier_list;
  iab_du_cell_res_cfg_tdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-DU-Cell-Resource-Configuration-Mode-Info-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using iab_du_cell_res_cfg_mode_info_ext_ies_o = protocol_ies_empty_o;

// IAB-DU-Cell-Resource-Configuration-Mode-Info ::= CHOICE
struct iab_du_cell_res_cfg_mode_info_c {
  struct types_opts {
    enum options { tdd, fdd, choice_ext, nulltype } value;

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
  iab_du_cell_res_cfg_tdd_info_s& tdd()
  {
    assert_choice_type(types::tdd, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<iab_du_cell_res_cfg_tdd_info_s>();
  }
  iab_du_cell_res_cfg_fdd_info_s& fdd()
  {
    assert_choice_type(types::fdd, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<iab_du_cell_res_cfg_fdd_info_s>();
  }
  protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>>();
  }
  const iab_du_cell_res_cfg_tdd_info_s& tdd() const
  {
    assert_choice_type(types::tdd, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<iab_du_cell_res_cfg_tdd_info_s>();
  }
  const iab_du_cell_res_cfg_fdd_info_s& fdd() const
  {
    assert_choice_type(types::fdd, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<iab_du_cell_res_cfg_fdd_info_s>();
  }
  const protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "IAB-DU-Cell-Resource-Configuration-Mode-Info");
    return c.get<protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>>();
  }
  iab_du_cell_res_cfg_tdd_info_s&                                          set_tdd();
  iab_du_cell_res_cfg_fdd_info_s&                                          set_fdd();
  protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<iab_du_cell_res_cfg_fdd_info_s,
                  iab_du_cell_res_cfg_tdd_info_s,
                  protocol_ie_single_container_s<iab_du_cell_res_cfg_mode_info_ext_ies_o>>
      c;

  void destroy_();
};

// IAB-MT-Cell-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_mt_cell_list_item_ext_ies_o = protocol_ext_empty_o;

using iab_mt_cell_list_item_ext_ies_container = protocol_ext_container_empty_l;

// IAB-MT-Cell-List-Item ::= SEQUENCE
struct iab_mt_cell_list_item_s {
  bool                                    ie_exts_present = false;
  fixed_bitstring<36, false, true>        nr_cell_id;
  du_rx_mt_rx_e                           du_rx_mt_rx;
  du_tx_mt_tx_e                           du_tx_mt_tx;
  du_rx_mt_tx_e                           du_rx_mt_tx;
  du_tx_mt_rx_e                           du_tx_mt_rx;
  iab_mt_cell_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-MT-Cell-List ::= SEQUENCE (SIZE (1..32)) OF IAB-MT-Cell-List-Item
using iab_mt_cell_list_l = dyn_array<iab_mt_cell_list_item_s>;

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

// SSB-transmisisonBitmap-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
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

// IAB-STC-Info-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IAB-STC-Info-List ::= SEQUENCE (SIZE (1..45)) OF IAB-STC-Info-Item
using iab_stc_info_list_l = dyn_array<iab_stc_info_item_s>;

// IAB-STC-Info-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_stc_info_ext_ies_o = protocol_ext_empty_o;

using iab_stc_info_ext_ies_container = protocol_ext_container_empty_l;

// IAB-STC-Info ::= SEQUENCE
struct iab_stc_info_s {
  bool                           ie_exts_present = false;
  iab_stc_info_list_l            iab_stc_info_list;
  iab_stc_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTNLAddressesRequested-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// IABIPv6RequestType-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
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

// IABTNLAddressToRemove-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_tnl_address_to_rem_item_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_address_to_rem_item_ext_ies_container = protocol_ext_container_empty_l;

// IABTNLAddressToRemove-Item ::= SEQUENCE
struct iab_tnl_address_to_rem_item_s {
  bool                                          ie_ext_present = false;
  iab_tnl_address_c                             iab_tnl_address;
  iab_tnl_address_to_rem_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTNLAddressToRemove-List ::= SEQUENCE (SIZE (1..1024)) OF IABTNLAddressToRemove-Item
using iab_tnl_address_to_rem_list_l = dyn_array<iab_tnl_address_to_rem_item_s>;

// IAB-TNL-Address-Request-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_tnl_address_request_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_address_request_ext_ies_container = protocol_ext_container_empty_l;

// IAB-TNL-Address-Request ::= SEQUENCE
struct iab_tnl_address_request_s {
  bool                                      ie_exts_present = false;
  iab_tnl_addresses_requested_s             iab_ip_v4_addresses_requested;
  iab_ip_v6_request_type_c                  iab_ip_v6_request_type;
  iab_tnl_address_to_rem_list_l             iab_tnl_address_to_rem_list;
  iab_tnl_address_request_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTNLAddressUsage ::= ENUMERATED
struct iab_tnl_address_usage_opts {
  enum options { f1_c, f1_u, non_f1, /*...*/ all, nulltype } value;

  const char* to_string() const;
};
using iab_tnl_address_usage_e = enumerated<iab_tnl_address_usage_opts, true, 1>;

// IABAllocatedTNLAddress-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_allocated_tnl_address_item_ext_ies_o = protocol_ext_empty_o;

using iab_allocated_tnl_address_item_ext_ies_container = protocol_ext_container_empty_l;

// IABAllocatedTNLAddress-Item ::= SEQUENCE
struct iab_allocated_tnl_address_item_s {
  bool                                             iab_tnl_address_usage_present       = false;
  bool                                             associated_donor_du_address_present = false;
  bool                                             ie_exts_present                     = false;
  iab_tnl_address_c                                iab_tnl_address;
  iab_tnl_address_usage_e                          iab_tnl_address_usage;
  fixed_bitstring<10, false, true>                 associated_donor_du_address;
  iab_allocated_tnl_address_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABAllocatedTNLAddress-List ::= SEQUENCE (SIZE (1..1024)) OF IABAllocatedTNLAddress-Item
using iab_allocated_tnl_address_list_l = dyn_array<iab_allocated_tnl_address_item_s>;

// IAB-TNL-Address-Response-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_tnl_address_resp_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_address_resp_ext_ies_container = protocol_ext_container_empty_l;

// IAB-TNL-Address-Response ::= SEQUENCE
struct iab_tnl_address_resp_s {
  bool                                   ie_exts_present = false;
  iab_allocated_tnl_address_list_l       iab_allocated_tnl_address_list;
  iab_tnl_address_resp_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABAuthorizationStatus ::= ENUMERATED
struct iab_authorization_status_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using iab_authorization_status_e = enumerated<iab_authorization_status_opts, true>;

// MultiplexingInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mux_info_ext_ies_o = protocol_ext_empty_o;

using mux_info_ext_ies_container = protocol_ext_container_empty_l;

// MultiplexingInfo ::= SEQUENCE
struct mux_info_s {
  bool                       ie_exts_present = false;
  iab_mt_cell_list_l         iab_mt_cell_list;
  mux_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABCellInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_cell_info_ext_ies_o = protocol_ext_empty_o;

using iab_cell_info_ext_ies_container = protocol_ext_container_empty_l;

// IABCellInformation ::= SEQUENCE
struct iab_cell_info_s {
  bool                            iab_du_cell_res_cfg_mode_info_present = false;
  bool                            iab_stc_info_present                  = false;
  bool                            mux_info_present                      = false;
  bool                            ie_exts_present                       = false;
  nr_cgi_s                        nr_cgi;
  iab_du_cell_res_cfg_mode_info_c iab_du_cell_res_cfg_mode_info;
  iab_stc_info_s                  iab_stc_info;
  unbounded_octstring<true>       rach_cfg_common;
  unbounded_octstring<true>       rach_cfg_common_iab;
  unbounded_octstring<true>       csi_rs_cfg;
  unbounded_octstring<true>       sr_cfg;
  unbounded_octstring<true>       pdcch_cfg_sib1;
  unbounded_octstring<true>       scs_common;
  mux_info_s                      mux_info;
  iab_cell_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABNodeIndication ::= ENUMERATED
struct iab_node_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using iab_node_ind_e = enumerated<iab_node_ind_opts, true>;

// IABTNLAddress-ItemExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using iab_tnl_address_item_ext_ies_o = protocol_ext_empty_o;

using iab_tnl_address_item_ext_ies_container = protocol_ext_container_empty_l;

// IABTNLAddress-Item ::= SEQUENCE
struct iab_tnl_address_item_s {
  bool                                   ie_exts_present = false;
  iab_tnl_address_c                      iab_tnl_address;
  iab_tnl_address_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// IABTNLAddressException ::= SEQUENCE (SIZE (1..1024)) OF IABTNLAddress-Item
using iab_tnl_address_exception_l = dyn_array<iab_tnl_address_item_s>;

// M1ReportingTrigger ::= ENUMERATED
struct m1_report_trigger_opts {
  enum options { periodic, a2eventtriggered, a2eventtriggered_periodic, /*...*/ nulltype } value;

  const char* to_string() const;
};
using m1_report_trigger_e = enumerated<m1_report_trigger_opts, true>;

// MeasurementThresholdA2-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using meas_thres_a2_ext_ies_o = protocol_ies_empty_o;

// MeasurementThresholdA2 ::= CHOICE
struct meas_thres_a2_c {
  struct types_opts {
    enum options { thres_rsrp, thres_rsrq, thres_sinr, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  meas_thres_a2_c() = default;
  meas_thres_a2_c(const meas_thres_a2_c& other);
  meas_thres_a2_c& operator=(const meas_thres_a2_c& other);
  ~meas_thres_a2_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& thres_rsrp()
  {
    assert_choice_type(types::thres_rsrp, type_, "MeasurementThresholdA2");
    return c.get<uint8_t>();
  }
  uint8_t& thres_rsrq()
  {
    assert_choice_type(types::thres_rsrq, type_, "MeasurementThresholdA2");
    return c.get<uint8_t>();
  }
  uint8_t& thres_sinr()
  {
    assert_choice_type(types::thres_sinr, type_, "MeasurementThresholdA2");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<meas_thres_a2_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MeasurementThresholdA2");
    return c.get<protocol_ie_single_container_s<meas_thres_a2_ext_ies_o>>();
  }
  const uint8_t& thres_rsrp() const
  {
    assert_choice_type(types::thres_rsrp, type_, "MeasurementThresholdA2");
    return c.get<uint8_t>();
  }
  const uint8_t& thres_rsrq() const
  {
    assert_choice_type(types::thres_rsrq, type_, "MeasurementThresholdA2");
    return c.get<uint8_t>();
  }
  const uint8_t& thres_sinr() const
  {
    assert_choice_type(types::thres_sinr, type_, "MeasurementThresholdA2");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<meas_thres_a2_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MeasurementThresholdA2");
    return c.get<protocol_ie_single_container_s<meas_thres_a2_ext_ies_o>>();
  }
  uint8_t&                                                 set_thres_rsrp();
  uint8_t&                                                 set_thres_rsrq();
  uint8_t&                                                 set_thres_sinr();
  protocol_ie_single_container_s<meas_thres_a2_ext_ies_o>& set_choice_ext();

private:
  types                                                                    type_;
  choice_buffer_t<protocol_ie_single_container_s<meas_thres_a2_ext_ies_o>> c;

  void destroy_();
};

// M1ThresholdEventA2-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using m1_thres_event_a2_ext_ies_o = protocol_ext_empty_o;

using m1_thres_event_a2_ext_ies_container = protocol_ext_container_empty_l;

// M1ThresholdEventA2 ::= SEQUENCE
struct m1_thres_event_a2_s {
  bool                                ie_exts_present = false;
  meas_thres_a2_c                     meas_thres;
  m1_thres_event_a2_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ReportIntervalMDT ::= ENUMERATED
struct report_interv_mdt_opts {
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
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using report_interv_mdt_e = enumerated<report_interv_mdt_opts, true>;

// ReportAmountMDT ::= ENUMERATED
struct report_amount_mdt_opts {
  enum options { r1, r2, r4, r8, r16, r32, r64, infinity, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using report_amount_mdt_e = enumerated<report_amount_mdt_opts, true>;

// M1PeriodicReporting-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct m1_periodic_report_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m1_periodic_report_ext_ies_container {
  bool                         extended_report_interv_mdt_present = false;
  extended_report_interv_mdt_e extended_report_interv_mdt;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M1PeriodicReporting ::= SEQUENCE
struct m1_periodic_report_s {
  bool                                 ie_exts_present = false;
  report_interv_mdt_e                  report_interv;
  report_amount_mdt_e                  report_amount;
  m1_periodic_report_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M1Configuration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct m1_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m1_cfg_ext_ies_container {
  bool                    beam_meas_ind_m1_present      = false;
  bool                    beam_meass_report_cfg_present = false;
  beam_meas_ind_m1_e      beam_meas_ind_m1;
  beam_meass_report_cfg_s beam_meass_report_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M1Configuration ::= SEQUENCE
struct m1_cfg_s {
  bool                     m1thresevent_a2_present   = false;
  bool                     m1periodic_report_present = false;
  bool                     ie_exts_present           = false;
  m1_report_trigger_e      m1report_trigger;
  m1_thres_event_a2_s      m1thresevent_a2;
  m1_periodic_report_s     m1periodic_report;
  m1_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M4period ::= ENUMERATED
struct m4period_opts {
  enum options { ms1024, ms2048, ms5120, ms10240, min1, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using m4period_e = enumerated<m4period_opts, true>;

// Links-to-log ::= ENUMERATED
struct links_to_log_opts {
  enum options { ul, dl, both_ul_and_dl, /*...*/ nulltype } value;

  const char* to_string() const;
};
using links_to_log_e = enumerated<links_to_log_opts, true>;

// M4ReportAmountMDT ::= ENUMERATED
struct m4_report_amount_mdt_opts {
  enum options { r1, r2, r4, r8, r16, r32, r64, infinity, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using m4_report_amount_mdt_e = enumerated<m4_report_amount_mdt_opts, true>;

// M4Configuration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct m4_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m4_cfg_ext_ies_container {
  bool                   m4_report_amount_present = false;
  m4_report_amount_mdt_e m4_report_amount;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M4Configuration ::= SEQUENCE
struct m4_cfg_s {
  bool                     ie_exts_present = false;
  m4period_e               m4period;
  links_to_log_e           m4_links_to_log;
  m4_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M5period ::= ENUMERATED
struct m5period_opts {
  enum options { ms1024, ms2048, ms5120, ms10240, min1, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using m5period_e = enumerated<m5period_opts, true>;

// M5ReportAmountMDT ::= ENUMERATED
struct m5_report_amount_mdt_opts {
  enum options { r1, r2, r4, r8, r16, r32, r64, infinity, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using m5_report_amount_mdt_e = enumerated<m5_report_amount_mdt_opts, true>;

// M5Configuration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct m5_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m5_cfg_ext_ies_container {
  bool                   m5_report_amount_present = false;
  m5_report_amount_mdt_e m5_report_amount;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M5Configuration ::= SEQUENCE
struct m5_cfg_s {
  bool                     ie_exts_present = false;
  m5period_e               m5period;
  links_to_log_e           m5_links_to_log;
  m5_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M6report-Interval ::= ENUMERATED
struct m6report_interv_opts {
  enum options {
    ms120,
    ms240,
    ms480,
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
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using m6report_interv_e = enumerated<m6report_interv_opts, true>;

// M6ReportAmountMDT ::= ENUMERATED
struct m6_report_amount_mdt_opts {
  enum options { r1, r2, r4, r8, r16, r32, r64, infinity, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using m6_report_amount_mdt_e = enumerated<m6_report_amount_mdt_opts, true>;

// M6Configuration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct m6_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m6_cfg_ext_ies_container {
  bool                            m6_report_amount_present              = false;
  bool                            excess_packet_delay_thres_cfg_present = false;
  m6_report_amount_mdt_e          m6_report_amount;
  excess_packet_delay_thres_cfg_l excess_packet_delay_thres_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M6Configuration ::= SEQUENCE
struct m6_cfg_s {
  bool                     ie_exts_present = false;
  m6report_interv_e        m6report_interv;
  links_to_log_e           m6_links_to_log;
  m6_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M7ReportAmountMDT ::= ENUMERATED
struct m7_report_amount_mdt_opts {
  enum options { r1, r2, r4, r8, r16, r32, r64, infinity, /*...*/ nulltype } value;
  typedef int8_t number_type;

  const char* to_string() const;
  int8_t      to_number() const;
};
using m7_report_amount_mdt_e = enumerated<m7_report_amount_mdt_opts, true>;

// M7Configuration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct m7_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct m7_cfg_ext_ies_container {
  bool                   m7_report_amount_present = false;
  m7_report_amount_mdt_e m7_report_amount;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// M7Configuration ::= SEQUENCE
struct m7_cfg_s {
  bool                     ie_exts_present = false;
  uint8_t                  m7period        = 1;
  links_to_log_e           m7_links_to_log;
  m7_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// WLANMeasConfig ::= ENUMERATED
struct wlan_meas_cfg_opts {
  enum options { setup, /*...*/ nulltype } value;

  const char* to_string() const;
};
using wlan_meas_cfg_e = enumerated<wlan_meas_cfg_opts, true>;

// WLANMeasConfigNameList ::= SEQUENCE (SIZE (1..4)) OF OCTET STRING (SIZE (1..32))
using wlan_meas_cfg_name_list_l = bounded_array<bounded_octstring<1, 32, true>, 4>;

// WLANMeasurementConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using wlan_meas_cfg_ext_ies_o = protocol_ext_empty_o;

using wlan_meas_cfg_ext_ies_container = protocol_ext_container_empty_l;

// WLANMeasurementConfiguration ::= SEQUENCE
struct wlan_meas_cfg_s {
  struct wlan_rssi_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using wlan_rssi_e_ = enumerated<wlan_rssi_opts, true>;
  struct wlan_rtt_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using wlan_rtt_e_ = enumerated<wlan_rtt_opts, true>;

  // member variables
  bool                            wlan_rssi_present = false;
  bool                            wlan_rtt_present  = false;
  bool                            ie_exts_present   = false;
  wlan_meas_cfg_e                 wlan_meas_cfg;
  wlan_meas_cfg_name_list_l       wlan_meas_cfg_name_list;
  wlan_rssi_e_                    wlan_rssi;
  wlan_rtt_e_                     wlan_rtt;
  wlan_meas_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SensorMeasConfig ::= ENUMERATED
struct sensor_meas_cfg_opts {
  enum options { setup, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sensor_meas_cfg_e = enumerated<sensor_meas_cfg_opts, true>;

// SensorNameConfig-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sensor_name_cfg_ext_ies_o = protocol_ext_empty_o;

using sensor_name_cfg_ext_ies_container = protocol_ext_container_empty_l;

// SensorName ::= SEQUENCE
struct sensor_name_s {
  struct uncompensated_barometric_cfg_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using uncompensated_barometric_cfg_e_ = enumerated<uncompensated_barometric_cfg_opts, true>;
  struct ue_speed_cfg_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using ue_speed_cfg_e_ = enumerated<ue_speed_cfg_opts, true>;
  struct ue_orientation_cfg_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using ue_orientation_cfg_e_ = enumerated<ue_orientation_cfg_opts, true>;

  // member variables
  bool                              uncompensated_barometric_cfg_present = false;
  bool                              ue_speed_cfg_present                 = false;
  bool                              ue_orientation_cfg_present           = false;
  bool                              ie_exts_present                      = false;
  uncompensated_barometric_cfg_e_   uncompensated_barometric_cfg;
  ue_speed_cfg_e_                   ue_speed_cfg;
  ue_orientation_cfg_e_             ue_orientation_cfg;
  sensor_name_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SensorMeasConfigNameList ::= SEQUENCE (SIZE (1..3)) OF SensorName
using sensor_meas_cfg_name_list_l = dyn_array<sensor_name_s>;

// SensorMeasurementConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sensor_meas_cfg_ext_ies_o = protocol_ext_empty_o;

using sensor_meas_cfg_ext_ies_container = protocol_ext_container_empty_l;

// SensorMeasurementConfiguration ::= SEQUENCE
struct sensor_meas_cfg_s {
  bool                              ie_exts_present = false;
  sensor_meas_cfg_e                 sensor_meas_cfg;
  sensor_meas_cfg_name_list_l       sensor_meas_cfg_name_list;
  sensor_meas_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ImmediateMDT-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using immediate_mdt_nr_ext_ies_o = protocol_ext_empty_o;

using immediate_mdt_nr_ext_ies_container = protocol_ext_container_empty_l;

// ImmediateMDT-NR ::= SEQUENCE
struct immediate_mdt_nr_s {
  bool                               m1_cfg_present             = false;
  bool                               m4_cfg_present             = false;
  bool                               m5_cfg_present             = false;
  bool                               mdt_location_info_present  = false;
  bool                               m6_cfg_present             = false;
  bool                               m7_cfg_present             = false;
  bool                               bluetooth_meas_cfg_present = false;
  bool                               wlan_meas_cfg_present      = false;
  bool                               sensor_meas_cfg_present    = false;
  bool                               ie_exts_present            = false;
  fixed_bitstring<8, false, true>    meass_to_activ;
  m1_cfg_s                           m1_cfg;
  m4_cfg_s                           m4_cfg;
  m5_cfg_s                           m5_cfg;
  fixed_bitstring<8, false, true>    mdt_location_info;
  m6_cfg_s                           m6_cfg;
  m7_cfg_s                           m7_cfg;
  bluetooth_meas_cfg_s               bluetooth_meas_cfg;
  wlan_meas_cfg_s                    wlan_meas_cfg;
  sensor_meas_cfg_s                  sensor_meas_cfg;
  immediate_mdt_nr_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NG-RAN-CellPCI-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ng_ran_cell_pci_ext_ies_o = protocol_ies_empty_o;

// NG-RAN-CellPCI ::= CHOICE
struct ng_ran_cell_pci_c {
  struct types_opts {
    enum options { nr, e_utra, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ng_ran_cell_pci_c() = default;
  ng_ran_cell_pci_c(const ng_ran_cell_pci_c& other);
  ng_ran_cell_pci_c& operator=(const ng_ran_cell_pci_c& other);
  ~ng_ran_cell_pci_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint16_t& nr()
  {
    assert_choice_type(types::nr, type_, "NG-RAN-CellPCI");
    return c.get<uint16_t>();
  }
  uint16_t& e_utra()
  {
    assert_choice_type(types::e_utra, type_, "NG-RAN-CellPCI");
    return c.get<uint16_t>();
  }
  protocol_ie_single_container_s<ng_ran_cell_pci_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NG-RAN-CellPCI");
    return c.get<protocol_ie_single_container_s<ng_ran_cell_pci_ext_ies_o>>();
  }
  const uint16_t& nr() const
  {
    assert_choice_type(types::nr, type_, "NG-RAN-CellPCI");
    return c.get<uint16_t>();
  }
  const uint16_t& e_utra() const
  {
    assert_choice_type(types::e_utra, type_, "NG-RAN-CellPCI");
    return c.get<uint16_t>();
  }
  const protocol_ie_single_container_s<ng_ran_cell_pci_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NG-RAN-CellPCI");
    return c.get<protocol_ie_single_container_s<ng_ran_cell_pci_ext_ies_o>>();
  }
  uint16_t&                                                  set_nr();
  uint16_t&                                                  set_e_utra();
  protocol_ie_single_container_s<ng_ran_cell_pci_ext_ies_o>& set_choice_ext();

private:
  types                                                                      type_;
  choice_buffer_t<protocol_ie_single_container_s<ng_ran_cell_pci_ext_ies_o>> c;

  void destroy_();
};

// RRCConnReestab-Indicator ::= ENUMERATED
struct rrc_conn_reestab_ind_opts {
  enum options { recfg_fail, ho_fail, other_fail, /*...*/ nulltype } value;

  const char* to_string() const;
};
using rrc_conn_reestab_ind_e = enumerated<rrc_conn_reestab_ind_opts, true>;

// RRCReestab-Initiated-Reporting-wo-UERLFReport-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct rrc_reestab_initiated_report_wo_ue_rlf_report_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct rrc_reestab_initiated_report_wo_ue_rlf_report_ext_ies_container {
  bool                   rrc_conn_reestab_ind_present = false;
  rrc_conn_reestab_ind_e rrc_conn_reestab_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCReestab-Initiated-Reporting-wo-UERLFReport ::= SEQUENCE
struct rrc_reestab_initiated_report_wo_ue_rlf_report_s {
  bool                                                            ie_exts_present = false;
  ng_ran_cell_pci_c                                               fail_cell_pci;
  global_ng_ran_cell_id_s                                         reestab_cell_cgi;
  fixed_bitstring<16, false, true>                                c_rnti;
  fixed_bitstring<16, false, true>                                short_mac_i;
  rrc_reestab_initiated_report_wo_ue_rlf_report_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UERLFReportContainerLTEExtension-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_rlf_report_container_lte_ext_ext_ies_o = protocol_ext_empty_o;

using ue_rlf_report_container_lte_ext_ext_ies_container = protocol_ext_container_empty_l;

// UERLFReportContainerLTEExtension ::= SEQUENCE
struct ue_rlf_report_container_lte_ext_s {
  bool                                              ie_exts_present = false;
  unbounded_octstring<true>                         ue_rlf_report_container_lte;
  unbounded_octstring<true>                         ue_rlf_report_container_lte_extend_band;
  ue_rlf_report_container_lte_ext_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UERLFReportContainer-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct ue_rlf_report_container_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { ue_rlf_report_container_lte_ext, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::ue_rlf_report_container_lte_ext; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    ue_rlf_report_container_lte_ext_s&       ue_rlf_report_container_lte_ext() { return c; }
    const ue_rlf_report_container_lte_ext_s& ue_rlf_report_container_lte_ext() const { return c; }

  private:
    ue_rlf_report_container_lte_ext_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// UERLFReportContainer ::= CHOICE
struct ue_rlf_report_container_c {
  struct types_opts {
    enum options { nr_ue_rlf_report_container, lte_ue_rlf_report_container, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ue_rlf_report_container_c() = default;
  ue_rlf_report_container_c(const ue_rlf_report_container_c& other);
  ue_rlf_report_container_c& operator=(const ue_rlf_report_container_c& other);
  ~ue_rlf_report_container_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  unbounded_octstring<true>& nr_ue_rlf_report_container()
  {
    assert_choice_type(types::nr_ue_rlf_report_container, type_, "UERLFReportContainer");
    return c.get<unbounded_octstring<true>>();
  }
  unbounded_octstring<true>& lte_ue_rlf_report_container()
  {
    assert_choice_type(types::lte_ue_rlf_report_container, type_, "UERLFReportContainer");
    return c.get<unbounded_octstring<true>>();
  }
  protocol_ie_single_container_s<ue_rlf_report_container_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UERLFReportContainer");
    return c.get<protocol_ie_single_container_s<ue_rlf_report_container_ext_ies_o>>();
  }
  const unbounded_octstring<true>& nr_ue_rlf_report_container() const
  {
    assert_choice_type(types::nr_ue_rlf_report_container, type_, "UERLFReportContainer");
    return c.get<unbounded_octstring<true>>();
  }
  const unbounded_octstring<true>& lte_ue_rlf_report_container() const
  {
    assert_choice_type(types::lte_ue_rlf_report_container, type_, "UERLFReportContainer");
    return c.get<unbounded_octstring<true>>();
  }
  const protocol_ie_single_container_s<ue_rlf_report_container_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UERLFReportContainer");
    return c.get<protocol_ie_single_container_s<ue_rlf_report_container_ext_ies_o>>();
  }
  unbounded_octstring<true>&                                         set_nr_ue_rlf_report_container();
  unbounded_octstring<true>&                                         set_lte_ue_rlf_report_container();
  protocol_ie_single_container_s<ue_rlf_report_container_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                         type_;
  choice_buffer_t<protocol_ie_single_container_s<ue_rlf_report_container_ext_ies_o>, unbounded_octstring<true>> c;

  void destroy_();
};

// RRCReestab-Initiated-Reporting-with-UERLFReport-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using rrc_reestab_initiated_report_with_ue_rlf_report_ext_ies_o = protocol_ext_empty_o;

using rrc_reestab_initiated_report_with_ue_rlf_report_ext_ies_container = protocol_ext_container_empty_l;

// RRCReestab-Initiated-Reporting-with-UERLFReport ::= SEQUENCE
struct rrc_reestab_initiated_report_with_ue_rlf_report_s {
  bool                                                              ie_exts_present = false;
  ue_rlf_report_container_c                                         ue_rlf_report_container;
  rrc_reestab_initiated_report_with_ue_rlf_report_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCReestab-Initiated-Reporting-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using rrc_reestab_initiated_report_ext_ies_o = protocol_ies_empty_o;

// RRCReestab-Initiated-Reporting ::= CHOICE
struct rrc_reestab_initiated_report_c {
  struct types_opts {
    enum options {
      rrc_reestab_report_wo_ue_rlf_report,
      rrc_reestab_report_with_ue_rlf_report,
      choice_ext,
      nulltype
    } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  rrc_reestab_initiated_report_c() = default;
  rrc_reestab_initiated_report_c(const rrc_reestab_initiated_report_c& other);
  rrc_reestab_initiated_report_c& operator=(const rrc_reestab_initiated_report_c& other);
  ~rrc_reestab_initiated_report_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  rrc_reestab_initiated_report_wo_ue_rlf_report_s& rrc_reestab_report_wo_ue_rlf_report()
  {
    assert_choice_type(types::rrc_reestab_report_wo_ue_rlf_report, type_, "RRCReestab-Initiated-Reporting");
    return c.get<rrc_reestab_initiated_report_wo_ue_rlf_report_s>();
  }
  rrc_reestab_initiated_report_with_ue_rlf_report_s& rrc_reestab_report_with_ue_rlf_report()
  {
    assert_choice_type(types::rrc_reestab_report_with_ue_rlf_report, type_, "RRCReestab-Initiated-Reporting");
    return c.get<rrc_reestab_initiated_report_with_ue_rlf_report_s>();
  }
  protocol_ie_single_container_s<rrc_reestab_initiated_report_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "RRCReestab-Initiated-Reporting");
    return c.get<protocol_ie_single_container_s<rrc_reestab_initiated_report_ext_ies_o>>();
  }
  const rrc_reestab_initiated_report_wo_ue_rlf_report_s& rrc_reestab_report_wo_ue_rlf_report() const
  {
    assert_choice_type(types::rrc_reestab_report_wo_ue_rlf_report, type_, "RRCReestab-Initiated-Reporting");
    return c.get<rrc_reestab_initiated_report_wo_ue_rlf_report_s>();
  }
  const rrc_reestab_initiated_report_with_ue_rlf_report_s& rrc_reestab_report_with_ue_rlf_report() const
  {
    assert_choice_type(types::rrc_reestab_report_with_ue_rlf_report, type_, "RRCReestab-Initiated-Reporting");
    return c.get<rrc_reestab_initiated_report_with_ue_rlf_report_s>();
  }
  const protocol_ie_single_container_s<rrc_reestab_initiated_report_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "RRCReestab-Initiated-Reporting");
    return c.get<protocol_ie_single_container_s<rrc_reestab_initiated_report_ext_ies_o>>();
  }
  rrc_reestab_initiated_report_wo_ue_rlf_report_s&                        set_rrc_reestab_report_wo_ue_rlf_report();
  rrc_reestab_initiated_report_with_ue_rlf_report_s&                      set_rrc_reestab_report_with_ue_rlf_report();
  protocol_ie_single_container_s<rrc_reestab_initiated_report_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<rrc_reestab_initiated_report_ext_ies_o>,
                  rrc_reestab_initiated_report_with_ue_rlf_report_s,
                  rrc_reestab_initiated_report_wo_ue_rlf_report_s>
      c;

  void destroy_();
};

// RRCReestab-initiated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using rrc_reestab_initiated_ext_ies_o = protocol_ext_empty_o;

using rrc_reestab_initiated_ext_ies_container = protocol_ext_container_empty_l;

// RRCReestab-initiated ::= SEQUENCE
struct rrc_reestab_initiated_s {
  bool                                    ie_exts_present = false;
  rrc_reestab_initiated_report_c          rrrc_reestab_initiated_report;
  rrc_reestab_initiated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCSetup-Initiated-Reporting-with-UERLFReport-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using rrc_setup_initiated_report_with_ue_rlf_report_ext_ies_o = protocol_ext_empty_o;

using rrc_setup_initiated_report_with_ue_rlf_report_ext_ies_container = protocol_ext_container_empty_l;

// RRCSetup-Initiated-Reporting-with-UERLFReport ::= SEQUENCE
struct rrc_setup_initiated_report_with_ue_rlf_report_s {
  bool                                                            ie_exts_present = false;
  ue_rlf_report_container_c                                       ue_rlf_report_container;
  rrc_setup_initiated_report_with_ue_rlf_report_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCSetup-Initiated-Reporting-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using rrc_setup_initiated_report_ext_ies_o = protocol_ies_empty_o;

// RRCSetup-Initiated-Reporting ::= CHOICE
struct rrc_setup_initiated_report_c {
  struct types_opts {
    enum options { rrc_setup_report_with_ue_rlf_report, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  rrc_setup_initiated_report_c() = default;
  rrc_setup_initiated_report_c(const rrc_setup_initiated_report_c& other);
  rrc_setup_initiated_report_c& operator=(const rrc_setup_initiated_report_c& other);
  ~rrc_setup_initiated_report_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  rrc_setup_initiated_report_with_ue_rlf_report_s& rrc_setup_report_with_ue_rlf_report()
  {
    assert_choice_type(types::rrc_setup_report_with_ue_rlf_report, type_, "RRCSetup-Initiated-Reporting");
    return c.get<rrc_setup_initiated_report_with_ue_rlf_report_s>();
  }
  protocol_ie_single_container_s<rrc_setup_initiated_report_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "RRCSetup-Initiated-Reporting");
    return c.get<protocol_ie_single_container_s<rrc_setup_initiated_report_ext_ies_o>>();
  }
  const rrc_setup_initiated_report_with_ue_rlf_report_s& rrc_setup_report_with_ue_rlf_report() const
  {
    assert_choice_type(types::rrc_setup_report_with_ue_rlf_report, type_, "RRCSetup-Initiated-Reporting");
    return c.get<rrc_setup_initiated_report_with_ue_rlf_report_s>();
  }
  const protocol_ie_single_container_s<rrc_setup_initiated_report_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "RRCSetup-Initiated-Reporting");
    return c.get<protocol_ie_single_container_s<rrc_setup_initiated_report_ext_ies_o>>();
  }
  rrc_setup_initiated_report_with_ue_rlf_report_s&                      set_rrc_setup_report_with_ue_rlf_report();
  protocol_ie_single_container_s<rrc_setup_initiated_report_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<rrc_setup_initiated_report_ext_ies_o>,
                  rrc_setup_initiated_report_with_ue_rlf_report_s>
      c;

  void destroy_();
};

// RRCSetup-initiated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using rrc_setup_initiated_ext_ies_o = protocol_ext_empty_o;

using rrc_setup_initiated_ext_ies_container = protocol_ext_container_empty_l;

// RRCSetup-initiated ::= SEQUENCE
struct rrc_setup_initiated_s {
  bool                                  ue_rlf_report_container_present = false;
  bool                                  ie_exts_present                 = false;
  rrc_setup_initiated_report_c          rrrc_setup_initiated_report;
  ue_rlf_report_container_c             ue_rlf_report_container;
  rrc_setup_initiated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// InitiatingCondition-FailureIndication-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using init_condition_fail_ind_ext_ies_o = protocol_ies_empty_o;

// InitiatingCondition-FailureIndication ::= CHOICE
struct init_condition_fail_ind_c {
  struct types_opts {
    enum options { rrc_reestab, rrc_setup, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  init_condition_fail_ind_c() = default;
  init_condition_fail_ind_c(const init_condition_fail_ind_c& other);
  init_condition_fail_ind_c& operator=(const init_condition_fail_ind_c& other);
  ~init_condition_fail_ind_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  rrc_reestab_initiated_s& rrc_reestab()
  {
    assert_choice_type(types::rrc_reestab, type_, "InitiatingCondition-FailureIndication");
    return c.get<rrc_reestab_initiated_s>();
  }
  rrc_setup_initiated_s& rrc_setup()
  {
    assert_choice_type(types::rrc_setup, type_, "InitiatingCondition-FailureIndication");
    return c.get<rrc_setup_initiated_s>();
  }
  protocol_ie_single_container_s<init_condition_fail_ind_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "InitiatingCondition-FailureIndication");
    return c.get<protocol_ie_single_container_s<init_condition_fail_ind_ext_ies_o>>();
  }
  const rrc_reestab_initiated_s& rrc_reestab() const
  {
    assert_choice_type(types::rrc_reestab, type_, "InitiatingCondition-FailureIndication");
    return c.get<rrc_reestab_initiated_s>();
  }
  const rrc_setup_initiated_s& rrc_setup() const
  {
    assert_choice_type(types::rrc_setup, type_, "InitiatingCondition-FailureIndication");
    return c.get<rrc_setup_initiated_s>();
  }
  const protocol_ie_single_container_s<init_condition_fail_ind_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "InitiatingCondition-FailureIndication");
    return c.get<protocol_ie_single_container_s<init_condition_fail_ind_ext_ies_o>>();
  }
  rrc_reestab_initiated_s&                                           set_rrc_reestab();
  rrc_setup_initiated_s&                                             set_rrc_setup();
  protocol_ie_single_container_s<init_condition_fail_ind_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<init_condition_fail_ind_ext_ies_o>,
                  rrc_reestab_initiated_s,
                  rrc_setup_initiated_s>
      c;

  void destroy_();
};

// NRCyclicPrefix ::= ENUMERATED
struct nr_cp_opts {
  enum options { normal, extended, /*...*/ nulltype } value;

  const char* to_string() const;
};
using nr_cp_e = enumerated<nr_cp_opts, true>;

// NRDL-ULTransmissionPeriodicity ::= ENUMERATED
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
using nr_dl_ul_tx_periodicity_e = enumerated<nr_dl_ul_tx_periodicity_opts, true>;

// SymbolAllocation-in-Slot-AllDL-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using symbol_alloc_in_slot_all_dl_ext_ies_o = protocol_ext_empty_o;

using symbol_alloc_in_slot_all_dl_ext_ies_container = protocol_ext_container_empty_l;

// SymbolAllocation-in-Slot-AllDL ::= SEQUENCE
struct symbol_alloc_in_slot_all_dl_s {
  bool                                          ie_ext_present = false;
  symbol_alloc_in_slot_all_dl_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SymbolAllocation-in-Slot-AllUL-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using symbol_alloc_in_slot_all_ul_ext_ies_o = protocol_ext_empty_o;

using symbol_alloc_in_slot_all_ul_ext_ies_container = protocol_ext_container_empty_l;

// SymbolAllocation-in-Slot-AllUL ::= SEQUENCE
struct symbol_alloc_in_slot_all_ul_s {
  bool                                          ie_ext_present = false;
  symbol_alloc_in_slot_all_ul_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SymbolAllocation-in-Slot-BothDLandUL-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct symbol_alloc_in_slot_both_dl_and_ul_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct symbol_alloc_in_slot_both_dl_and_ul_ext_ies_container {
  bool          permutation_present = false;
  permutation_e permutation;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SymbolAllocation-in-Slot-BothDLandUL ::= SEQUENCE
struct symbol_alloc_in_slot_both_dl_and_ul_s {
  bool                                                  ie_ext_present   = false;
  uint8_t                                               numof_dl_symbols = 0;
  uint8_t                                               numof_ul_symbols = 0;
  symbol_alloc_in_slot_both_dl_and_ul_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SymbolAllocation-in-Slot-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using symbol_alloc_in_slot_ext_ies_o = protocol_ies_empty_o;

// SymbolAllocation-in-Slot ::= CHOICE
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
  symbol_alloc_in_slot_all_dl_s& all_dl()
  {
    assert_choice_type(types::all_dl, type_, "SymbolAllocation-in-Slot");
    return c.get<symbol_alloc_in_slot_all_dl_s>();
  }
  symbol_alloc_in_slot_all_ul_s& all_ul()
  {
    assert_choice_type(types::all_ul, type_, "SymbolAllocation-in-Slot");
    return c.get<symbol_alloc_in_slot_all_ul_s>();
  }
  symbol_alloc_in_slot_both_dl_and_ul_s& both_dl_and_ul()
  {
    assert_choice_type(types::both_dl_and_ul, type_, "SymbolAllocation-in-Slot");
    return c.get<symbol_alloc_in_slot_both_dl_and_ul_s>();
  }
  protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SymbolAllocation-in-Slot");
    return c.get<protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>>();
  }
  const symbol_alloc_in_slot_all_dl_s& all_dl() const
  {
    assert_choice_type(types::all_dl, type_, "SymbolAllocation-in-Slot");
    return c.get<symbol_alloc_in_slot_all_dl_s>();
  }
  const symbol_alloc_in_slot_all_ul_s& all_ul() const
  {
    assert_choice_type(types::all_ul, type_, "SymbolAllocation-in-Slot");
    return c.get<symbol_alloc_in_slot_all_ul_s>();
  }
  const symbol_alloc_in_slot_both_dl_and_ul_s& both_dl_and_ul() const
  {
    assert_choice_type(types::both_dl_and_ul, type_, "SymbolAllocation-in-Slot");
    return c.get<symbol_alloc_in_slot_both_dl_and_ul_s>();
  }
  const protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SymbolAllocation-in-Slot");
    return c.get<protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>>();
  }
  symbol_alloc_in_slot_all_dl_s&                                  set_all_dl();
  symbol_alloc_in_slot_all_ul_s&                                  set_all_ul();
  symbol_alloc_in_slot_both_dl_and_ul_s&                          set_both_dl_and_ul();
  protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<symbol_alloc_in_slot_ext_ies_o>,
                  symbol_alloc_in_slot_all_dl_s,
                  symbol_alloc_in_slot_all_ul_s,
                  symbol_alloc_in_slot_both_dl_and_ul_s>
      c;

  void destroy_();
};

// SlotConfiguration-List-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using slot_cfg_list_item_ext_ies_o = protocol_ext_empty_o;

using slot_cfg_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SlotConfiguration-List-Item ::= SEQUENCE
struct slot_cfg_list_item_s {
  bool                                 ie_exts_present = false;
  uint16_t                             slot_idx        = 0;
  symbol_alloc_in_slot_c               symbol_alloc_in_slot;
  slot_cfg_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SlotConfiguration-List ::= SEQUENCE (SIZE (1..5120)) OF SlotConfiguration-List-Item
using slot_cfg_list_l = dyn_array<slot_cfg_list_item_s>;

// IntendedTDD-DL-ULConfiguration-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using intended_tdd_dl_ul_cfg_nr_ext_ies_o = protocol_ext_empty_o;

using intended_tdd_dl_ul_cfg_nr_ext_ies_container = protocol_ext_container_empty_l;

// IntendedTDD-DL-ULConfiguration-NR ::= SEQUENCE
struct intended_tdd_dl_ul_cfg_nr_s {
  bool                                        ie_exts_present = false;
  nr_scs_e                                    nrscs;
  nr_cp_e                                     nr_cp;
  nr_dl_ul_tx_periodicity_e                   nr_dl_ul_tx_periodicity;
  slot_cfg_list_l                             slot_cfg_list;
  intended_tdd_dl_ul_cfg_nr_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTEA2XServicesAuthorized-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ltea2_x_services_authorized_ext_ies_o = protocol_ext_empty_o;

using ltea2_x_services_authorized_ext_ies_container = protocol_ext_container_empty_l;

// LTEA2XServicesAuthorized ::= SEQUENCE
struct ltea2_x_services_authorized_s {
  bool                                          aerial_ue_present         = false;
  bool                                          aerial_ctrller_ue_present = false;
  bool                                          ie_exts_present           = false;
  aerial_ue_e                                   aerial_ue;
  aerial_ctrller_ue_e                           aerial_ctrller_ue;
  ltea2_x_services_authorized_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LTEUESidelinkAggregateMaximumBitRate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using lte_ue_sidelink_aggr_max_bit_rate_ext_ies_o = protocol_ext_empty_o;

using lte_ue_sidelink_aggr_max_bit_rate_ext_ies_container = protocol_ext_container_empty_l;

// LTEUESidelinkAggregateMaximumBitRate ::= SEQUENCE
struct lte_ue_sidelink_aggr_max_bit_rate_s {
  bool                                                ie_exts_present               = false;
  uint64_t                                            ue_sidelink_aggr_max_bit_rate = 0;
  lte_ue_sidelink_aggr_max_bit_rate_ext_ies_container ie_exts;
  // ...

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

// LTEV2XServicesAuthorized-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LastVisitedCell-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using last_visited_cell_item_ext_ies_o = protocol_ies_empty_o;

// LastVisitedCell-Item ::= CHOICE
struct last_visited_cell_item_c {
  struct types_opts {
    enum options { ng_ran_cell, e_utran_cell, utran_cell, geran_cell, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  last_visited_cell_item_c() = default;
  last_visited_cell_item_c(const last_visited_cell_item_c& other);
  last_visited_cell_item_c& operator=(const last_visited_cell_item_c& other);
  ~last_visited_cell_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  unbounded_octstring<true>& ng_ran_cell()
  {
    assert_choice_type(types::ng_ran_cell, type_, "LastVisitedCell-Item");
    return c.get<unbounded_octstring<true>>();
  }
  unbounded_octstring<true>& e_utran_cell()
  {
    assert_choice_type(types::e_utran_cell, type_, "LastVisitedCell-Item");
    return c.get<unbounded_octstring<true>>();
  }
  unbounded_octstring<true>& utran_cell()
  {
    assert_choice_type(types::utran_cell, type_, "LastVisitedCell-Item");
    return c.get<unbounded_octstring<true>>();
  }
  unbounded_octstring<true>& geran_cell()
  {
    assert_choice_type(types::geran_cell, type_, "LastVisitedCell-Item");
    return c.get<unbounded_octstring<true>>();
  }
  protocol_ie_single_container_s<last_visited_cell_item_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "LastVisitedCell-Item");
    return c.get<protocol_ie_single_container_s<last_visited_cell_item_ext_ies_o>>();
  }
  const unbounded_octstring<true>& ng_ran_cell() const
  {
    assert_choice_type(types::ng_ran_cell, type_, "LastVisitedCell-Item");
    return c.get<unbounded_octstring<true>>();
  }
  const unbounded_octstring<true>& e_utran_cell() const
  {
    assert_choice_type(types::e_utran_cell, type_, "LastVisitedCell-Item");
    return c.get<unbounded_octstring<true>>();
  }
  const unbounded_octstring<true>& utran_cell() const
  {
    assert_choice_type(types::utran_cell, type_, "LastVisitedCell-Item");
    return c.get<unbounded_octstring<true>>();
  }
  const unbounded_octstring<true>& geran_cell() const
  {
    assert_choice_type(types::geran_cell, type_, "LastVisitedCell-Item");
    return c.get<unbounded_octstring<true>>();
  }
  const protocol_ie_single_container_s<last_visited_cell_item_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "LastVisitedCell-Item");
    return c.get<protocol_ie_single_container_s<last_visited_cell_item_ext_ies_o>>();
  }
  unbounded_octstring<true>&                                        set_ng_ran_cell();
  unbounded_octstring<true>&                                        set_e_utran_cell();
  unbounded_octstring<true>&                                        set_utran_cell();
  unbounded_octstring<true>&                                        set_geran_cell();
  protocol_ie_single_container_s<last_visited_cell_item_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                        type_;
  choice_buffer_t<protocol_ie_single_container_s<last_visited_cell_item_ext_ies_o>, unbounded_octstring<true>> c;

  void destroy_();
};

// LastVisitedPSCellList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using last_visited_pscell_list_item_ext_ies_o = protocol_ext_empty_o;

using last_visited_pscell_list_item_ext_ies_container = protocol_ext_container_empty_l;

// LastVisitedPSCellList-Item ::= SEQUENCE
struct last_visited_pscell_list_item_s {
  bool                                            ie_exts_present = false;
  unbounded_octstring<true>                       last_visited_pscell_info;
  last_visited_pscell_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LastVisitedPSCellList ::= SEQUENCE (SIZE (1..8)) OF LastVisitedPSCellList-Item
using last_visited_pscell_list_l = dyn_array<last_visited_pscell_list_item_s>;

// Local-NG-RAN-Node-Identifier-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
struct local_ng_ran_node_id_ext_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { full_and_short_i_rnti_profile_list, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::full_and_short_i_rnti_profile_list; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    full_and_short_i_rnti_profile_list_s&       full_and_short_i_rnti_profile_list() { return c; }
    const full_and_short_i_rnti_profile_list_s& full_and_short_i_rnti_profile_list() const { return c; }

  private:
    full_and_short_i_rnti_profile_list_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// Local-NG-RAN-Node-Identifier ::= CHOICE
struct local_ng_ran_node_id_c {
  struct types_opts {
    enum options { full_i_rnti_profile_list, short_i_rnti_profile_list, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  local_ng_ran_node_id_c() = default;
  local_ng_ran_node_id_c(const local_ng_ran_node_id_c& other);
  local_ng_ran_node_id_c& operator=(const local_ng_ran_node_id_c& other);
  ~local_ng_ran_node_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  full_i_rnti_profile_list_c& full_i_rnti_profile_list()
  {
    assert_choice_type(types::full_i_rnti_profile_list, type_, "Local-NG-RAN-Node-Identifier");
    return c.get<full_i_rnti_profile_list_c>();
  }
  short_i_rnti_profile_list_c& short_i_rnti_profile_list()
  {
    assert_choice_type(types::short_i_rnti_profile_list, type_, "Local-NG-RAN-Node-Identifier");
    return c.get<short_i_rnti_profile_list_c>();
  }
  protocol_ie_single_container_s<local_ng_ran_node_id_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "Local-NG-RAN-Node-Identifier");
    return c.get<protocol_ie_single_container_s<local_ng_ran_node_id_ext_ies_o>>();
  }
  const full_i_rnti_profile_list_c& full_i_rnti_profile_list() const
  {
    assert_choice_type(types::full_i_rnti_profile_list, type_, "Local-NG-RAN-Node-Identifier");
    return c.get<full_i_rnti_profile_list_c>();
  }
  const short_i_rnti_profile_list_c& short_i_rnti_profile_list() const
  {
    assert_choice_type(types::short_i_rnti_profile_list, type_, "Local-NG-RAN-Node-Identifier");
    return c.get<short_i_rnti_profile_list_c>();
  }
  const protocol_ie_single_container_s<local_ng_ran_node_id_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "Local-NG-RAN-Node-Identifier");
    return c.get<protocol_ie_single_container_s<local_ng_ran_node_id_ext_ies_o>>();
  }
  full_i_rnti_profile_list_c&                                     set_full_i_rnti_profile_list();
  short_i_rnti_profile_list_c&                                    set_short_i_rnti_profile_list();
  protocol_ie_single_container_s<local_ng_ran_node_id_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<full_i_rnti_profile_list_c,
                  protocol_ie_single_container_s<local_ng_ran_node_id_ext_ies_o>,
                  short_i_rnti_profile_list_c>
      c;

  void destroy_();
};

// LocationInformationSNReporting ::= ENUMERATED
struct location_info_snr_eporting_opts {
  enum options { pscell, /*...*/ nulltype } value;

  const char* to_string() const;
};
using location_info_snr_eporting_e = enumerated<location_info_snr_eporting_opts, true>;

// ReportArea ::= ENUMERATED
struct report_area_opts {
  enum options { cell, /*...*/ nulltype } value;

  const char* to_string() const;
};
using report_area_e = enumerated<report_area_opts, true>;

// LocationReportingInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct location_report_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct location_report_info_ext_ies_container {
  bool                     addition_location_info_present = false;
  addition_location_info_e addition_location_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LocationReportingInformation ::= SEQUENCE
struct location_report_info_s {
  bool                                   ie_exts_present = false;
  event_type_e                           event_type;
  report_area_e                          report_area;
  area_of_interest_info_l                area_of_interest;
  location_report_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LoggingInterval ::= ENUMERATED
struct logging_interv_opts {
  enum options {
    ms320,
    ms640,
    ms1280,
    ms2560,
    ms5120,
    ms10240,
    ms20480,
    ms30720,
    ms40960,
    ms61440,
    infinity,
    // ...
    nulltype
  } value;
  typedef int32_t number_type;

  const char* to_string() const;
  int32_t     to_number() const;
};
using logging_interv_e = enumerated<logging_interv_opts, true>;

// LoggingDuration ::= ENUMERATED
struct logging_dur_opts {
  enum options { m10, m20, m40, m60, m90, m120, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using logging_dur_e = enumerated<logging_dur_opts>;

// Periodical-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using periodical_ext_ies_o = protocol_ext_empty_o;

using periodical_ext_ies_container = protocol_ext_container_empty_l;

// Periodical ::= SEQUENCE
struct periodical_s {
  bool                         ie_exts_present = false;
  periodical_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ReportType-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using report_type_ext_ies_o = protocol_ies_empty_o;

// ReportType ::= CHOICE
struct report_type_c {
  struct types_opts {
    enum options { periodical, event_triggered, /*...*/ choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true, 1>;

  // choice methods
  report_type_c() = default;
  report_type_c(const report_type_c& other);
  report_type_c& operator=(const report_type_c& other);
  ~report_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  periodical_s& periodical()
  {
    assert_choice_type(types::periodical, type_, "ReportType");
    return c.get<periodical_s>();
  }
  event_triggered_s& event_triggered()
  {
    assert_choice_type(types::event_triggered, type_, "ReportType");
    return c.get<event_triggered_s>();
  }
  protocol_ie_single_container_s<report_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ReportType");
    return c.get<protocol_ie_single_container_s<report_type_ext_ies_o>>();
  }
  const periodical_s& periodical() const
  {
    assert_choice_type(types::periodical, type_, "ReportType");
    return c.get<periodical_s>();
  }
  const event_triggered_s& event_triggered() const
  {
    assert_choice_type(types::event_triggered, type_, "ReportType");
    return c.get<event_triggered_s>();
  }
  const protocol_ie_single_container_s<report_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ReportType");
    return c.get<protocol_ie_single_container_s<report_type_ext_ies_o>>();
  }
  periodical_s&                                          set_periodical();
  event_triggered_s&                                     set_event_triggered();
  protocol_ie_single_container_s<report_type_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                   type_;
  choice_buffer_t<event_triggered_s, periodical_s, protocol_ie_single_container_s<report_type_ext_ies_o>> c;

  void destroy_();
};

// LoggedMDT-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct logged_mdt_nr_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct logged_mdt_nr_ext_ies_container {
  bool         early_meas_present = false;
  early_meas_e early_meas;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LoggedMDT-NR ::= SEQUENCE
struct logged_mdt_nr_s {
  bool                             bluetooth_meas_cfg_present = false;
  bool                             wlan_meas_cfg_present      = false;
  bool                             sensor_meas_cfg_present    = false;
  bool                             ie_exts_present            = false;
  logging_interv_e                 logging_interv;
  logging_dur_e                    logging_dur;
  report_type_c                    report_type;
  bluetooth_meas_cfg_s             bluetooth_meas_cfg;
  wlan_meas_cfg_s                  wlan_meas_cfg;
  sensor_meas_cfg_s                sensor_meas_cfg;
  area_scope_of_neigh_cells_list_l area_scope_of_neigh_cells_list;
  logged_mdt_nr_ext_ies_container  ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LowerLayerPresenceStatusChange ::= ENUMERATED
struct lower_layer_presence_status_change_opts {
  enum options {
    release_lower_layers,
    re_establish_lower_layers,
    // ...
    suspend_lower_layers,
    resume_lower_layers,
    nulltype
  } value;

  const char* to_string() const;
};
using lower_layer_presence_status_change_e = enumerated<lower_layer_presence_status_change_opts, true, 2>;

// MBS-AssistanceInformation ::= ENUMERATED
struct mbs_assist_info_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mbs_assist_info_e = enumerated<mbs_assist_info_opts, true>;

// MBS-DataForwarding-Indicator ::= ENUMERATED
struct mbs_data_forwarding_ind_opts {
  enum options { mbs_only, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mbs_data_forwarding_ind_e = enumerated<mbs_data_forwarding_ind_opts, true>;

// MBS-DataForwardingResponseInfofromTarget-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mbs_data_forwarding_resp_infofrom_target_item_ext_ies_o = protocol_ext_empty_o;

using mbs_data_forwarding_resp_infofrom_target_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-DataForwardingResponseInfofromTarget-Item ::= SEQUENCE
struct mbs_data_forwarding_resp_infofrom_target_item_s {
  bool                                                            mrb_progress_info_present = false;
  bool                                                            ie_exts_present           = false;
  uint16_t                                                        mrb_id                    = 1;
  up_transport_layer_info_c                                       dl_forwarding_up_tnl;
  mrb_progress_info_c                                             mrb_progress_info;
  mbs_data_forwarding_resp_infofrom_target_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-DataForwardingResponseInfofromTarget ::= SEQUENCE (SIZE (1..32)) OF MBS-DataForwardingResponseInfofromTarget-Item
using mbs_data_forwarding_resp_infofrom_target_l = dyn_array<mbs_data_forwarding_resp_infofrom_target_item_s>;

// MBS-Session-ID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// MBS-SessionAssociatedInformation-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mbs_session_associated_info_item_ext_ies_o = protocol_ext_empty_o;

using mbs_session_associated_info_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-SessionAssociatedInformation-Item ::= SEQUENCE
struct mbs_session_associated_info_item_s {
  bool                                               ie_exts_present = false;
  mbs_session_id_s                                   mbs_session_id;
  associated_qos_flow_info_list_l                    associated_qos_flow_info_list;
  mbs_session_associated_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-SessionAssociatedInformation ::= SEQUENCE (SIZE (1..32)) OF MBS-SessionAssociatedInformation-Item
using mbs_session_associated_info_l = dyn_array<mbs_session_associated_info_item_s>;

// MBS-SessionInformation-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct mbs_session_info_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct mbs_session_info_item_ext_ies_container {
  bool              mbs_assist_info_present = false;
  mbs_assist_info_e mbs_assist_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-SessionInformation-Item ::= SEQUENCE
struct mbs_session_info_item_s {
  bool                                    mbs_area_session_id_present    = false;
  bool                                    active_mbs_sessio_info_present = false;
  bool                                    ie_exts_present                = false;
  mbs_session_id_s                        mbs_session_id;
  uint32_t                                mbs_area_session_id = 0;
  active_mbs_session_info_s               active_mbs_sessio_info;
  mbs_session_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-SessionInformation-List ::= SEQUENCE (SIZE (1..256)) OF MBS-SessionInformation-Item
using mbs_session_info_list_l = dyn_array<mbs_session_info_item_s>;

// MBS-SessionInformationResponse-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mbs_session_info_resp_item_ext_ies_o = protocol_ext_empty_o;

using mbs_session_info_resp_item_ext_ies_container = protocol_ext_container_empty_l;

// MBS-SessionInformationResponse-Item ::= SEQUENCE
struct mbs_session_info_resp_item_s {
  bool                                         ie_exts_present = false;
  mbs_session_id_s                             mbs_session_id;
  mbs_data_forwarding_resp_infofrom_target_l   mbs_data_forwarding_resp_infofrom_target;
  mbs_session_info_resp_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBS-SessionInformationResponse-List ::= SEQUENCE (SIZE (1..256)) OF MBS-SessionInformationResponse-Item
using mbs_session_info_resp_list_l = dyn_array<mbs_session_info_resp_item_s>;

// MBSCommServiceType ::= ENUMERATED
struct mbs_comm_service_type_opts {
  enum options { multicast, broadcast, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mbs_comm_service_type_e = enumerated<mbs_comm_service_type_opts, true>;

// MBSFNSubframeAllocation-E-UTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using mbsfn_sf_alloc_e_utra_ext_ies_o = protocol_ies_empty_o;

// MBSFNSubframeAllocation-E-UTRA ::= CHOICE
struct mbsfn_sf_alloc_e_utra_c {
  struct types_opts {
    enum options { oneframe, fourframes, choice_ext, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mbsfn_sf_alloc_e_utra_c() = default;
  mbsfn_sf_alloc_e_utra_c(const mbsfn_sf_alloc_e_utra_c& other);
  mbsfn_sf_alloc_e_utra_c& operator=(const mbsfn_sf_alloc_e_utra_c& other);
  ~mbsfn_sf_alloc_e_utra_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<6, false, true>& oneframe()
  {
    assert_choice_type(types::oneframe, type_, "MBSFNSubframeAllocation-E-UTRA");
    return c.get<fixed_bitstring<6, false, true>>();
  }
  fixed_bitstring<24, false, true>& fourframes()
  {
    assert_choice_type(types::fourframes, type_, "MBSFNSubframeAllocation-E-UTRA");
    return c.get<fixed_bitstring<24, false, true>>();
  }
  protocol_ie_single_container_s<mbsfn_sf_alloc_e_utra_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MBSFNSubframeAllocation-E-UTRA");
    return c.get<protocol_ie_single_container_s<mbsfn_sf_alloc_e_utra_ext_ies_o>>();
  }
  const fixed_bitstring<6, false, true>& oneframe() const
  {
    assert_choice_type(types::oneframe, type_, "MBSFNSubframeAllocation-E-UTRA");
    return c.get<fixed_bitstring<6, false, true>>();
  }
  const fixed_bitstring<24, false, true>& fourframes() const
  {
    assert_choice_type(types::fourframes, type_, "MBSFNSubframeAllocation-E-UTRA");
    return c.get<fixed_bitstring<24, false, true>>();
  }
  const protocol_ie_single_container_s<mbsfn_sf_alloc_e_utra_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MBSFNSubframeAllocation-E-UTRA");
    return c.get<protocol_ie_single_container_s<mbsfn_sf_alloc_e_utra_ext_ies_o>>();
  }
  fixed_bitstring<6, false, true>&                                 set_oneframe();
  fixed_bitstring<24, false, true>&                                set_fourframes();
  protocol_ie_single_container_s<mbsfn_sf_alloc_e_utra_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<24, false, true>, protocol_ie_single_container_s<mbsfn_sf_alloc_e_utra_ext_ies_o>> c;

  void destroy_();
};

// MBSFNSubframeInfo-E-UTRA-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mbsfn_sf_info_e_utra_item_ext_ies_o = protocol_ext_empty_o;

using mbsfn_sf_info_e_utra_item_ext_ies_container = protocol_ext_container_empty_l;

// MBSFNSubframeInfo-E-UTRA-Item ::= SEQUENCE
struct mbsfn_sf_info_e_utra_item_s {
  struct radioframe_alloc_period_opts {
    enum options { n1, n2, n4, n8, n16, n32, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using radioframe_alloc_period_e_ = enumerated<radioframe_alloc_period_opts, true>;

  // member variables
  bool                                        ie_exts_present = false;
  radioframe_alloc_period_e_                  radioframe_alloc_period;
  uint8_t                                     radioframe_alloc_offset = 0;
  mbsfn_sf_alloc_e_utra_c                     sf_alloc;
  mbsfn_sf_info_e_utra_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MBSFNSubframeInfo-E-UTRA ::= SEQUENCE (SIZE (1..8)) OF MBSFNSubframeInfo-E-UTRA-Item
using mbsfn_sf_info_e_utra_l = dyn_array<mbsfn_sf_info_e_utra_item_s>;

// MDT-Activation ::= ENUMERATED
struct mdt_activation_opts {
  enum options { immediate_mdt_only, immediate_mdt_and_trace, logged_mdt_only, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mdt_activation_e = enumerated<mdt_activation_opts, true>;

// MDTMode-NR-ExtensionIE ::= OBJECT SET OF XNAP-PROTOCOL-IES
using mdt_mode_nr_ext_ie_o = protocol_ies_empty_o;

// MDTMode-NR-Extension ::= ProtocolIE-Single-Container{{MDTMode-NR-ExtensionIE}}
using mdt_mode_nr_ext_s = protocol_ie_single_container_s<mdt_mode_nr_ext_ie_o>;

// MDTMode-NR ::= CHOICE
struct mdt_mode_nr_c {
  struct types_opts {
    enum options { immediate_mdt, logged_mdt, /*...*/ mdt_mode_nr_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true, 1>;

  // choice methods
  mdt_mode_nr_c() = default;
  mdt_mode_nr_c(const mdt_mode_nr_c& other);
  mdt_mode_nr_c& operator=(const mdt_mode_nr_c& other);
  ~mdt_mode_nr_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  immediate_mdt_nr_s& immediate_mdt()
  {
    assert_choice_type(types::immediate_mdt, type_, "MDTMode-NR");
    return c.get<immediate_mdt_nr_s>();
  }
  logged_mdt_nr_s& logged_mdt()
  {
    assert_choice_type(types::logged_mdt, type_, "MDTMode-NR");
    return c.get<logged_mdt_nr_s>();
  }
  mdt_mode_nr_ext_s& mdt_mode_nr_ext()
  {
    assert_choice_type(types::mdt_mode_nr_ext, type_, "MDTMode-NR");
    return c.get<mdt_mode_nr_ext_s>();
  }
  const immediate_mdt_nr_s& immediate_mdt() const
  {
    assert_choice_type(types::immediate_mdt, type_, "MDTMode-NR");
    return c.get<immediate_mdt_nr_s>();
  }
  const logged_mdt_nr_s& logged_mdt() const
  {
    assert_choice_type(types::logged_mdt, type_, "MDTMode-NR");
    return c.get<logged_mdt_nr_s>();
  }
  const mdt_mode_nr_ext_s& mdt_mode_nr_ext() const
  {
    assert_choice_type(types::mdt_mode_nr_ext, type_, "MDTMode-NR");
    return c.get<mdt_mode_nr_ext_s>();
  }
  immediate_mdt_nr_s& set_immediate_mdt();
  logged_mdt_nr_s&    set_logged_mdt();
  mdt_mode_nr_ext_s&  set_mdt_mode_nr_ext();

private:
  types                                                                   type_;
  choice_buffer_t<immediate_mdt_nr_s, logged_mdt_nr_s, mdt_mode_nr_ext_s> c;

  void destroy_();
};

// MDTPLMNList ::= SEQUENCE (SIZE (1..16)) OF OCTET STRING (SIZE (3))
using mdt_plmn_list_l = bounded_array<fixed_octstring<3, true>, 16>;

// PNI-NPN-AreaScopeofMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pni_npn_area_scopeof_mdt_ext_ies_o = protocol_ext_empty_o;

using pni_npn_area_scopeof_mdt_ext_ies_container = protocol_ext_container_empty_l;

// PNI-NPN-AreaScopeofMDT ::= SEQUENCE
struct pni_npn_area_scopeof_mdt_s {
  bool                                       ie_exts_present = false;
  cag_listfor_mdt_l                          cag_listfor_mdt;
  pni_npn_area_scopeof_mdt_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MDT-Configuration-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct mdt_cfg_nr_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct mdt_cfg_nr_ext_ies_container {
  bool                       pni_npn_area_scopeof_mdt_present = false;
  pni_npn_area_scopeof_mdt_s pni_npn_area_scopeof_mdt;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MDT-Configuration-NR ::= SEQUENCE
struct mdt_cfg_nr_s {
  bool                         area_scope_of_mdt_nr_present = false;
  bool                         ie_exts_present              = false;
  mdt_activation_e             mdt_activation;
  area_scope_of_mdt_nr_c       area_scope_of_mdt_nr;
  mdt_mode_nr_c                mdt_mode_nr;
  mdt_plmn_list_l              sig_based_mdt_plmn_list;
  mdt_cfg_nr_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MDT-Configuration-EUTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mdt_cfg_eutra_ext_ies_o = protocol_ext_empty_o;

using mdt_cfg_eutra_ext_ies_container = protocol_ext_container_empty_l;

// MDT-Configuration-EUTRA ::= SEQUENCE
struct mdt_cfg_eutra_s {
  bool                            area_scope_of_mdt_eutra_present = false;
  bool                            ie_exts_present                 = false;
  mdt_activation_e                mdt_activation;
  area_scope_of_mdt_eutra_c       area_scope_of_mdt_eutra;
  unbounded_octstring<true>       mdt_mode_eutra;
  mdt_plmn_list_l                 sig_based_mdt_plmn_list;
  mdt_cfg_eutra_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MN-only-MDT-collection ::= ENUMERATED
struct mn_only_mdt_collection_opts {
  enum options { mn_only, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mn_only_mdt_collection_e = enumerated<mn_only_mdt_collection_opts, true>;

// MDT-Configuration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct mdt_cfg_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct mdt_cfg_ext_ies_container {
  bool                     mn_only_mdt_collection_present = false;
  mn_only_mdt_collection_e mn_only_mdt_collection;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MDT-Configuration ::= SEQUENCE
struct mdt_cfg_s {
  bool                      mdt_cfg_nr_present    = false;
  bool                      mdt_cfg_eutra_present = false;
  bool                      ie_exts_present       = false;
  mdt_cfg_nr_s              mdt_cfg_nr;
  mdt_cfg_eutra_s           mdt_cfg_eutra;
  mdt_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// S-BasedMDT-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using s_based_mdt_ext_ies_o = protocol_ext_empty_o;

using s_based_mdt_ext_ies_container = protocol_ext_container_empty_l;

// S-BasedMDT ::= SEQUENCE
struct s_based_mdt_s {
  bool                          ie_ext_present = false;
  fixed_octstring<8, true>      ng_ran_trace_id;
  s_based_mdt_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MDTAlignmentInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using mdt_align_info_ext_ies_o = protocol_ies_empty_o;

// MDTAlignmentInfo ::= CHOICE
struct mdt_align_info_c {
  struct types_opts {
    enum options { s_based_mdt, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  mdt_align_info_c() = default;
  mdt_align_info_c(const mdt_align_info_c& other);
  mdt_align_info_c& operator=(const mdt_align_info_c& other);
  ~mdt_align_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  s_based_mdt_s& s_based_mdt()
  {
    assert_choice_type(types::s_based_mdt, type_, "MDTAlignmentInfo");
    return c.get<s_based_mdt_s>();
  }
  protocol_ie_single_container_s<mdt_align_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MDTAlignmentInfo");
    return c.get<protocol_ie_single_container_s<mdt_align_info_ext_ies_o>>();
  }
  const s_based_mdt_s& s_based_mdt() const
  {
    assert_choice_type(types::s_based_mdt, type_, "MDTAlignmentInfo");
    return c.get<s_based_mdt_s>();
  }
  const protocol_ie_single_container_s<mdt_align_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MDTAlignmentInfo");
    return c.get<protocol_ie_single_container_s<mdt_align_info_ext_ies_o>>();
  }
  s_based_mdt_s&                                            set_s_based_mdt();
  protocol_ie_single_container_s<mdt_align_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                    type_;
  choice_buffer_t<protocol_ie_single_container_s<mdt_align_info_ext_ies_o>, s_based_mdt_s> c;

  void destroy_();
};

// MDTPLMNModificationList ::= SEQUENCE (SIZE (0..16)) OF OCTET STRING (SIZE (3))
using mdt_plmn_mod_list_l = bounded_array<fixed_octstring<3, true>, 16>;

// RVQoEReportingPeriodicity ::= ENUMERATED
struct rv_qo_e_report_periodicity_opts {
  enum options { ms120, ms240, ms480, ms640, ms1024, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using rv_qo_e_report_periodicity_e = enumerated<rv_qo_e_report_periodicity_opts, true>;

// RVQoEConfig-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using rv_qo_e_cfg_ext_ies_o = protocol_ext_empty_o;

using rv_qo_e_cfg_ext_ies_container = protocol_ext_container_empty_l;

// RVQoEConfig ::= SEQUENCE
struct rv_qo_e_cfg_s {
  bool                          available_ran_visible_qo_e_metrics_present = false;
  bool                          report_periodicity_present                 = false;
  bool                          ie_exts_present                            = false;
  available_rv_qo_e_metrics_s   available_ran_visible_qo_e_metrics;
  rv_qo_e_report_periodicity_e  report_periodicity;
  rv_qo_e_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MN-to-SN-QMCCoordRequestList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mn_to_sn_q_mcc_oord_request_list_item_ext_ies_o = protocol_ext_empty_o;

using mn_to_sn_q_mcc_oord_request_list_item_ext_ies_container = protocol_ext_container_empty_l;

// MN-to-SN-QMCCoordRequestList-Item ::= SEQUENCE
struct mn_to_sn_q_mcc_oord_request_list_item_s {
  struct qo_e_report_path_request_opts {
    enum options { srb4, srb5, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using qo_e_report_path_request_e_ = enumerated<qo_e_report_path_request_opts, true>;
  struct rv_qo_e_report_path_request_opts {
    enum options { srb4, srb5, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using rv_qo_e_report_path_request_e_ = enumerated<rv_qo_e_report_path_request_opts, true>;
  struct further_rv_qo_e_interest_inquiry_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using further_rv_qo_e_interest_inquiry_e_ = enumerated<further_rv_qo_e_interest_inquiry_opts, true>;
  struct further_rv_qo_e_report_path_inquiry_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using further_rv_qo_e_report_path_inquiry_e_ = enumerated<further_rv_qo_e_report_path_inquiry_opts, true>;
  struct cfg_release_ind_opts {
    enum options { rvqoe, qoe_and_rvqoe, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using cfg_release_ind_e_ = enumerated<cfg_release_ind_opts, true>;

  // member variables
  bool                                                    qoe_meas_cfg_app_layer_id_present           = false;
  bool                                                    meas_collection_entity_ip_address_present   = false;
  bool                                                    qo_e_report_path_request_present            = false;
  bool                                                    rv_qo_e_report_path_request_present         = false;
  bool                                                    further_rv_qo_e_interest_inquiry_present    = false;
  bool                                                    further_rv_qo_e_report_path_inquiry_present = false;
  bool                                                    current_rv_qo_e_cfg_present                 = false;
  bool                                                    available_rv_qo_e_metrics_present           = false;
  bool                                                    cfg_release_ind_present                     = false;
  bool                                                    ie_exts_present                             = false;
  fixed_octstring<6, true>                                qoe_ref;
  uint8_t                                                 qoe_meas_cfg_app_layer_id = 0;
  bounded_bitstring<1, 160, true, true>                   meas_collection_entity_ip_address;
  qo_e_report_path_request_e_                             qo_e_report_path_request;
  rv_qo_e_report_path_request_e_                          rv_qo_e_report_path_request;
  further_rv_qo_e_interest_inquiry_e_                     further_rv_qo_e_interest_inquiry;
  further_rv_qo_e_report_path_inquiry_e_                  further_rv_qo_e_report_path_inquiry;
  rv_qo_e_cfg_s                                           current_rv_qo_e_cfg;
  available_rv_qo_e_metrics_s                             available_rv_qo_e_metrics;
  cfg_release_ind_e_                                      cfg_release_ind;
  mn_to_sn_q_mcc_oord_request_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MN-to-SN-QMCCoordRequestList ::= SEQUENCE (SIZE (1..16)) OF MN-to-SN-QMCCoordRequestList-Item
using mn_to_sn_q_mcc_oord_request_list_l = dyn_array<mn_to_sn_q_mcc_oord_request_list_item_s>;

// MN-to-SN-QMCCoordResponseList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mn_to_sn_q_mcc_oord_resp_list_item_ext_ies_o = protocol_ext_empty_o;

using mn_to_sn_q_mcc_oord_resp_list_item_ext_ies_container = protocol_ext_container_empty_l;

// MN-to-SN-QMCCoordResponseList-Item ::= SEQUENCE
struct mn_to_sn_q_mcc_oord_resp_list_item_s {
  struct qo_e_cfg_sending_path_opts {
    enum options { mn, sn, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using qo_e_cfg_sending_path_e_ = enumerated<qo_e_cfg_sending_path_opts, true>;
  struct qo_e_report_path_resp_opts {
    enum options { accepted, rejected, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using qo_e_report_path_resp_e_ = enumerated<qo_e_report_path_resp_opts, true>;
  struct rv_qo_e_report_path_resp_opts {
    enum options { accepted, rejected, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using rv_qo_e_report_path_resp_e_ = enumerated<rv_qo_e_report_path_resp_opts, true>;
  struct further_rv_qo_e_interest_resp_opts {
    enum options { interested, not_interested, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using further_rv_qo_e_interest_resp_e_ = enumerated<further_rv_qo_e_interest_resp_opts, true>;
  struct further_rv_qo_e_report_path_resp_opts {
    enum options { srb4, srb5, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using further_rv_qo_e_report_path_resp_e_ = enumerated<further_rv_qo_e_report_path_resp_opts, true>;

  // member variables
  bool                                                 qoe_meas_cfg_app_layer_id_present        = false;
  bool                                                 qo_e_cfg_sending_path_present            = false;
  bool                                                 qo_e_report_path_resp_present            = false;
  bool                                                 rv_qo_e_report_path_resp_present         = false;
  bool                                                 further_rv_qo_e_interest_resp_present    = false;
  bool                                                 further_rv_qo_e_report_path_resp_present = false;
  bool                                                 preferred_rv_qo_e_cfg_present            = false;
  bool                                                 ie_exts_present                          = false;
  fixed_octstring<6, true>                             qoe_ref;
  uint8_t                                              qoe_meas_cfg_app_layer_id = 0;
  qo_e_cfg_sending_path_e_                             qo_e_cfg_sending_path;
  qo_e_report_path_resp_e_                             qo_e_report_path_resp;
  rv_qo_e_report_path_resp_e_                          rv_qo_e_report_path_resp;
  further_rv_qo_e_interest_resp_e_                     further_rv_qo_e_interest_resp;
  further_rv_qo_e_report_path_resp_e_                  further_rv_qo_e_report_path_resp;
  rv_qo_e_cfg_s                                        preferred_rv_qo_e_cfg;
  mn_to_sn_q_mcc_oord_resp_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MN-to-SN-QMCCoordResponseList ::= SEQUENCE (SIZE (1..16)) OF MN-to-SN-QMCCoordResponseList-Item
using mn_to_sn_q_mcc_oord_resp_list_l = dyn_array<mn_to_sn_q_mcc_oord_resp_list_item_s>;

// NR-CoordinationAssistanceInfo ::= ENUMERATED
struct nr_coordination_assist_info_opts {
  enum options { coordination_not_required, /*...*/ nulltype } value;

  const char* to_string() const;
};
using nr_coordination_assist_info_e = enumerated<nr_coordination_assist_info_opts, true>;

// NR-ResourceCoordinationInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_res_coordination_info_ext_ies_o = protocol_ext_empty_o;

using nr_res_coordination_info_ext_ies_container = protocol_ext_container_empty_l;

// NR-ResourceCoordinationInfo ::= SEQUENCE
struct nr_res_coordination_info_s {
  bool                                       dl_coordination_info_present        = false;
  bool                                       e_utra_cell_present                 = false;
  bool                                       nr_coordination_assist_info_present = false;
  bool                                       ie_ext_present                      = false;
  nr_cgi_s                                   nr_cell;
  bounded_bitstring<6, 4400, false, true>    ul_coordination_info;
  bounded_bitstring<6, 4400, false, true>    dl_coordination_info;
  e_utra_cgi_s                               e_utra_cell;
  nr_coordination_assist_info_e              nr_coordination_assist_info;
  nr_res_coordination_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NG-RAN-Node-ResourceCoordinationInfo ::= CHOICE
struct ng_ran_node_res_coordination_info_c {
  struct types_opts {
    enum options { eutra_res_coordination_info, nr_res_coordination_info, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ng_ran_node_res_coordination_info_c() = default;
  ng_ran_node_res_coordination_info_c(const ng_ran_node_res_coordination_info_c& other);
  ng_ran_node_res_coordination_info_c& operator=(const ng_ran_node_res_coordination_info_c& other);
  ~ng_ran_node_res_coordination_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  e_utra_res_coordination_info_s& eutra_res_coordination_info()
  {
    assert_choice_type(types::eutra_res_coordination_info, type_, "NG-RAN-Node-ResourceCoordinationInfo");
    return c.get<e_utra_res_coordination_info_s>();
  }
  nr_res_coordination_info_s& nr_res_coordination_info()
  {
    assert_choice_type(types::nr_res_coordination_info, type_, "NG-RAN-Node-ResourceCoordinationInfo");
    return c.get<nr_res_coordination_info_s>();
  }
  const e_utra_res_coordination_info_s& eutra_res_coordination_info() const
  {
    assert_choice_type(types::eutra_res_coordination_info, type_, "NG-RAN-Node-ResourceCoordinationInfo");
    return c.get<e_utra_res_coordination_info_s>();
  }
  const nr_res_coordination_info_s& nr_res_coordination_info() const
  {
    assert_choice_type(types::nr_res_coordination_info, type_, "NG-RAN-Node-ResourceCoordinationInfo");
    return c.get<nr_res_coordination_info_s>();
  }
  e_utra_res_coordination_info_s& set_eutra_res_coordination_info();
  nr_res_coordination_info_s&     set_nr_res_coordination_info();

private:
  types                                                                       type_;
  choice_buffer_t<e_utra_res_coordination_info_s, nr_res_coordination_info_s> c;

  void destroy_();
};

// MR-DC-ResourceCoordinationInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mr_dc_res_coordination_info_ext_ies_o = protocol_ext_empty_o;

using mr_dc_res_coordination_info_ext_ies_container = protocol_ext_container_empty_l;

// MR-DC-ResourceCoordinationInfo ::= SEQUENCE
struct mr_dc_res_coordination_info_s {
  bool                                          ie_ext_present = false;
  ng_ran_node_res_coordination_info_c           ng_ran_node_res_coordination_info;
  mr_dc_res_coordination_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MT-SDT-Indicator ::= ENUMERATED
struct mt_sdt_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mt_sdt_ind_e = enumerated<mt_sdt_ind_opts, true>;

// MT-SDT-Information-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using mt_sdt_info_ext_ies_o = protocol_ext_empty_o;

using mt_sdt_info_ext_ies_container = protocol_ext_container_empty_l;

// MT-SDT-Information ::= SEQUENCE
struct mt_sdt_info_s {
  bool                          ie_exts_present = false;
  mt_sdt_ind_e                  mt_sdt_ind;
  uint32_t                      mt_sdt_data_size = 1;
  mt_sdt_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MaxIPrate ::= ENUMERATED
struct max_ip_rate_opts {
  enum options { bitrate64kbs, max_ue_rate, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using max_ip_rate_e = enumerated<max_ip_rate_opts, true>;

// MaximumIPdatarate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct max_ip_datarate_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct max_ip_datarate_ext_ies_container {
  bool          max_ip_rate_dl_present = false;
  max_ip_rate_e max_ip_rate_dl;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MaximumIPdatarate ::= SEQUENCE
struct max_ip_datarate_s {
  bool                              ie_exts_present = false;
  max_ip_rate_e                     max_ip_rate_ul;
  max_ip_datarate_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasCollectionEntityIPAddress ::= BIT STRING
using meas_collection_entity_ip_address = bounded_bitstring<1, 160, true, true>;

// MeasuredTrajectoryNGRANCellInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using measured_trajectory_ngran_cell_info_ext_ies_o = protocol_ext_empty_o;

using measured_trajectory_ngran_cell_info_ext_ies_container = protocol_ext_container_empty_l;

// MeasuredTrajectoryNGRANCellInfo ::= SEQUENCE
struct measured_trajectory_ngran_cell_info_s {
  bool                                                  ie_exts_present = false;
  global_ng_ran_cell_id_s                               global_ng_ran_cell_id;
  uint16_t                                              time_ue_stays_in_cell = 0;
  measured_trajectory_ngran_cell_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasuredTrajectoryCellInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using measured_trajectory_cell_info_ext_ies_o = protocol_ies_empty_o;

// MeasuredTrajectoryCellInfo ::= CHOICE
struct measured_trajectory_cell_info_c {
  struct types_opts {
    enum options { ng_ran_cell, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  measured_trajectory_cell_info_c() = default;
  measured_trajectory_cell_info_c(const measured_trajectory_cell_info_c& other);
  measured_trajectory_cell_info_c& operator=(const measured_trajectory_cell_info_c& other);
  ~measured_trajectory_cell_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  measured_trajectory_ngran_cell_info_s& ng_ran_cell()
  {
    assert_choice_type(types::ng_ran_cell, type_, "MeasuredTrajectoryCellInfo");
    return c.get<measured_trajectory_ngran_cell_info_s>();
  }
  protocol_ie_single_container_s<measured_trajectory_cell_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "MeasuredTrajectoryCellInfo");
    return c.get<protocol_ie_single_container_s<measured_trajectory_cell_info_ext_ies_o>>();
  }
  const measured_trajectory_ngran_cell_info_s& ng_ran_cell() const
  {
    assert_choice_type(types::ng_ran_cell, type_, "MeasuredTrajectoryCellInfo");
    return c.get<measured_trajectory_ngran_cell_info_s>();
  }
  const protocol_ie_single_container_s<measured_trajectory_cell_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "MeasuredTrajectoryCellInfo");
    return c.get<protocol_ie_single_container_s<measured_trajectory_cell_info_ext_ies_o>>();
  }
  measured_trajectory_ngran_cell_info_s&                                   set_ng_ran_cell();
  protocol_ie_single_container_s<measured_trajectory_cell_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<measured_trajectory_ngran_cell_info_s,
                  protocol_ie_single_container_s<measured_trajectory_cell_info_ext_ies_o>>
      c;

  void destroy_();
};

// MeasuredUETrajectory-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using measured_ue_trajectory_item_ext_ies_o = protocol_ext_empty_o;

using measured_ue_trajectory_item_ext_ies_container = protocol_ext_container_empty_l;

// MeasuredUETrajectory-Item ::= SEQUENCE
struct measured_ue_trajectory_item_s {
  bool                                          ie_exts_present = false;
  measured_trajectory_cell_info_c               measuredtrajectory_cell_info;
  measured_ue_trajectory_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasuredUETrajectory ::= SEQUENCE (SIZE (1..16)) OF MeasuredUETrajectory-Item
using measured_ue_trajectory_l = dyn_array<measured_ue_trajectory_item_s>;

// MessageOversizeNotification-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using msg_oversize_notif_ext_ies_o = protocol_ext_empty_o;

using msg_oversize_notif_ext_ies_container = protocol_ext_container_empty_l;

// MessageOversizeNotification ::= SEQUENCE
struct msg_oversize_notif_s {
  bool                                 ie_ext_present     = false;
  uint32_t                             max_cell_list_size = 1;
  msg_oversize_notif_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobileIAB-AuthorizationStatus ::= ENUMERATED
struct mobile_iab_authorization_status_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mobile_iab_authorization_status_e = enumerated<mobile_iab_authorization_status_opts, true>;

// MobileIABCell ::= ENUMERATED
struct mobile_iab_cell_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using mobile_iab_cell_e = enumerated<mobile_iab_cell_opts, true>;

// MobilityParametersInformation ::= SEQUENCE
struct mob_params_info_s {
  int8_t ho_trigger_change = -20;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityParametersModificationRange ::= SEQUENCE
struct mob_params_mod_range_s {
  int8_t ho_trigger_change_lower_limit = -20;
  int8_t ho_trigger_change_upper_limit = -20;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RAT-RestrictionsItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct rat_restricts_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct rat_restricts_item_ext_ies_container {
  bool                         extended_rat_restrict_info_present = false;
  extended_rat_restrict_info_s extended_rat_restrict_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RAT-RestrictionsItem ::= SEQUENCE
struct rat_restricts_item_s {
  bool                                 ie_exts_present = false;
  fixed_octstring<3, true>             plmn_id;
  fixed_bitstring<8, true, true>       rat_restrict_info;
  rat_restricts_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RAT-RestrictionsList ::= SEQUENCE (SIZE (1..16)) OF RAT-RestrictionsItem
using rat_restricts_list_l = dyn_array<rat_restricts_item_s>;

// ServiceAreaItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using service_area_item_ext_ies_o = protocol_ext_empty_o;

using service_area_item_ext_ies_container = protocol_ext_container_empty_l;

// ServiceAreaItem ::= SEQUENCE
struct service_area_item_s {
  using allowed_tacs_service_area_l_     = bounded_array<fixed_octstring<3, true>, 16>;
  using not_allowed_tacs_service_area_l_ = bounded_array<fixed_octstring<3, true>, 16>;

  // member variables
  bool                                ie_exts_present = false;
  fixed_octstring<3, true>            plmn_id;
  allowed_tacs_service_area_l_        allowed_tacs_service_area;
  not_allowed_tacs_service_area_l_    not_allowed_tacs_service_area;
  service_area_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServiceAreaList ::= SEQUENCE (SIZE (1..16)) OF ServiceAreaItem
using service_area_list_l = dyn_array<service_area_item_s>;

// NPNMobilityInformation-SNPN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct npn_mob_info_sn_pn_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct npn_mob_info_sn_pn_ext_ies_container {
  bool                 equivalent_sn_p_ns_present = false;
  equivalent_sn_p_ns_l equivalent_sn_p_ns;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPNMobilityInformation-SNPN ::= SEQUENCE
struct npn_mob_info_sn_pn_s {
  bool                                 ie_ext_present = false;
  fixed_bitstring<44, false, true>     serving_n_id;
  npn_mob_info_sn_pn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPNMobilityInformation-PNI-NPN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using npn_mob_info_pni_npn_ext_ies_o = protocol_ext_empty_o;

using npn_mob_info_pni_npn_ext_ies_container = protocol_ext_container_empty_l;

// NPNMobilityInformation-PNI-NPN ::= SEQUENCE
struct npn_mob_info_pni_npn_s {
  bool                                   ie_ext_present = false;
  allowed_pni_npn_id_list_l              allowed_pni_npn_id_list;
  npn_mob_info_pni_npn_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPNMobilityInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using npn_mob_info_ext_ies_o = protocol_ies_empty_o;

// NPNMobilityInformation ::= CHOICE
struct npn_mob_info_c {
  struct types_opts {
    enum options { snpn_mob_info, pni_npn_mob_info, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  npn_mob_info_c() = default;
  npn_mob_info_c(const npn_mob_info_c& other);
  npn_mob_info_c& operator=(const npn_mob_info_c& other);
  ~npn_mob_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  npn_mob_info_sn_pn_s& snpn_mob_info()
  {
    assert_choice_type(types::snpn_mob_info, type_, "NPNMobilityInformation");
    return c.get<npn_mob_info_sn_pn_s>();
  }
  npn_mob_info_pni_npn_s& pni_npn_mob_info()
  {
    assert_choice_type(types::pni_npn_mob_info, type_, "NPNMobilityInformation");
    return c.get<npn_mob_info_pni_npn_s>();
  }
  protocol_ie_single_container_s<npn_mob_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NPNMobilityInformation");
    return c.get<protocol_ie_single_container_s<npn_mob_info_ext_ies_o>>();
  }
  const npn_mob_info_sn_pn_s& snpn_mob_info() const
  {
    assert_choice_type(types::snpn_mob_info, type_, "NPNMobilityInformation");
    return c.get<npn_mob_info_sn_pn_s>();
  }
  const npn_mob_info_pni_npn_s& pni_npn_mob_info() const
  {
    assert_choice_type(types::pni_npn_mob_info, type_, "NPNMobilityInformation");
    return c.get<npn_mob_info_pni_npn_s>();
  }
  const protocol_ie_single_container_s<npn_mob_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NPNMobilityInformation");
    return c.get<protocol_ie_single_container_s<npn_mob_info_ext_ies_o>>();
  }
  npn_mob_info_sn_pn_s&                                   set_snpn_mob_info();
  npn_mob_info_pni_npn_s&                                 set_pni_npn_mob_info();
  protocol_ie_single_container_s<npn_mob_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<npn_mob_info_pni_npn_s, npn_mob_info_sn_pn_s, protocol_ie_single_container_s<npn_mob_info_ext_ies_o>>
      c;

  void destroy_();
};

// MobilityRestrictionList-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct mob_restrict_list_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct mob_restrict_list_ext_ies_container {
  bool                               last_e_utran_plmn_id_present             = false;
  bool                               cn_type_restricts_for_serving_present    = false;
  bool                               cn_type_restricts_for_equivalent_present = false;
  bool                               npn_mob_info_present                     = false;
  fixed_octstring<3, true>           last_e_utran_plmn_id;
  cn_type_restricts_for_serving_e    cn_type_restricts_for_serving;
  cn_type_restricts_for_equivalent_l cn_type_restricts_for_equivalent;
  npn_mob_info_c                     npn_mob_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MobilityRestrictionList ::= SEQUENCE
struct mob_restrict_list_s {
  using equivalent_plmns_l_ = bounded_array<fixed_octstring<3, true>, 15>;

  // member variables
  bool                                ie_exts_present = false;
  fixed_octstring<3, true>            serving_plmn;
  equivalent_plmns_l_                 equivalent_plmns;
  rat_restricts_list_l                rat_restricts;
  forbidden_area_list_l               forbidden_area_info;
  service_area_list_l                 service_area_info;
  mob_restrict_list_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NBIoT-UL-DL-AlignmentOffset ::= ENUMERATED
struct nb_iot_ul_dl_align_offset_opts {
  enum options { khz_neg7dot5, khz0, khz7dot5, /*...*/ nulltype } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using nb_iot_ul_dl_align_offset_e = enumerated<nb_iot_ul_dl_align_offset_opts, true>;

// NE-DC-TDM-Pattern-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ne_dc_tdm_pattern_ext_ies_o = protocol_ext_empty_o;

using ne_dc_tdm_pattern_ext_ies_container = protocol_ext_container_empty_l;

// NE-DC-TDM-Pattern ::= SEQUENCE
struct ne_dc_tdm_pattern_s {
  struct sf_assign_opts {
    enum options { sa0, sa1, sa2, sa3, sa4, sa5, sa6, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sf_assign_e_ = enumerated<sf_assign_opts>;

  // member variables
  bool                                ie_ext_present = false;
  sf_assign_e_                        sf_assign;
  uint8_t                             harq_offset = 0;
  ne_dc_tdm_pattern_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NG-RAN-Cell-Identity-ListinRANPagingArea ::= SEQUENCE (SIZE (1..32)) OF NG-RAN-Cell-Identity
using ng_ran_cell_id_listin_ran_paging_area_l = dyn_array<ng_ran_cell_id_c>;

// SSBOffsetModificationRange-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ssb_offset_mod_range_ext_ies_o = protocol_ext_empty_o;

using ssb_offset_mod_range_ext_ies_container = protocol_ext_container_empty_l;

// SSBOffsetModificationRange ::= SEQUENCE
struct ssb_offset_mod_range_s {
  bool                                   ie_exts_present = false;
  uint8_t                                ssb_idx         = 0;
  mob_params_mod_range_s                 ss_bobility_params_mod_range;
  ssb_offset_mod_range_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NG-RANnode2SSBOffsetsModificationRange ::= SEQUENCE (SIZE (1..64)) OF SSBOffsetModificationRange
using ng_ran_node2_ssb_offsets_mod_range_l = dyn_array<ssb_offset_mod_range_s>;

// NPRACH-CP-Length ::= ENUMERATED
struct nprach_cp_len_opts {
  enum options { us66dot7, us266dot7, /*...*/ nulltype } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using nprach_cp_len_e = enumerated<nprach_cp_len_opts, true>;

// NPRACH-preambleFormat ::= ENUMERATED
struct nprach_preamb_format_opts {
  enum options { fmt0, fmt1, fmt2, fmt0a, fmt1a, /*...*/ nulltype } value;

  const char* to_string() const;
};
using nprach_preamb_format_e = enumerated<nprach_preamb_format_opts, true>;

// NPRACHConfiguration-FDD-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nprach_cfg_fdd_ext_ies_o = protocol_ext_empty_o;

using nprach_cfg_fdd_ext_ies_container = protocol_ext_container_empty_l;

// NPRACHConfiguration-FDD ::= SEQUENCE
struct nprach_cfg_fdd_s {
  bool                             ie_exts_present = false;
  nprach_cp_len_e                  nprach_cp_len;
  unbounded_octstring<true>        anchor_carrier_nprach_cfg;
  unbounded_octstring<true>        anchor_carrier_edt_nprach_cfg;
  unbounded_octstring<true>        anchor_carrier_format2_nprach_cfg;
  unbounded_octstring<true>        anchor_carrier_format2_edt_nprach_cfg;
  unbounded_octstring<true>        non_anchor_carrier_nprach_cfg;
  unbounded_octstring<true>        non_anchor_carrier_format2_nprach_cfg;
  nprach_cfg_fdd_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Non-AnchorCarrierFrequencylist-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using non_anchor_carrier_freqlist_ext_ies_o = protocol_ext_empty_o;

using non_anchor_carrier_freqlist_ext_ies_container = protocol_ext_container_empty_l;

struct non_anchor_carrier_freqlist_item_s_ {
  bool                                          ie_exts_present = false;
  unbounded_octstring<true>                     non_anchor_carrier_frquency;
  non_anchor_carrier_freqlist_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Non-AnchorCarrierFrequencylist ::= SEQUENCE (SIZE (1..15)) OF Non-AnchorCarrierFrequencylist-item
using non_anchor_carrier_freqlist_l = dyn_array<non_anchor_carrier_freqlist_item_s_>;

// NPRACHConfiguration-TDD-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nprach_cfg_tdd_ext_ies_o = protocol_ext_empty_o;

using nprach_cfg_tdd_ext_ies_container = protocol_ext_container_empty_l;

// NPRACHConfiguration-TDD ::= SEQUENCE
struct nprach_cfg_tdd_s {
  bool                             ie_exts_present = false;
  nprach_preamb_format_e           nprach_preamb_format;
  unbounded_octstring<true>        anchor_carrier_nprach_cfg_tdd;
  non_anchor_carrier_freqlist_l    non_anchor_carrier_fequency_cfglist;
  unbounded_octstring<true>        non_anchor_carrier_nprach_cfg_tdd;
  nprach_cfg_tdd_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NPRACHConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nprach_cfg_ext_ies_o = protocol_ext_empty_o;

using nprach_cfg_ext_ies_container = protocol_ext_container_empty_l;

// NPRACHConfiguration ::= SEQUENCE
struct nprach_cfg_s {
  struct fdd_or_tdd_c_ {
    struct types_opts {
      enum options { fdd, tdd, choice_ext, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    fdd_or_tdd_c_() = default;
    fdd_or_tdd_c_(const fdd_or_tdd_c_& other);
    fdd_or_tdd_c_& operator=(const fdd_or_tdd_c_& other);
    ~fdd_or_tdd_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    nprach_cfg_fdd_s& fdd()
    {
      assert_choice_type(types::fdd, type_, "fdd-or-tdd");
      return c.get<nprach_cfg_fdd_s>();
    }
    nprach_cfg_tdd_s& tdd()
    {
      assert_choice_type(types::tdd, type_, "fdd-or-tdd");
      return c.get<nprach_cfg_tdd_s>();
    }
    protocol_ie_single_container_s<fdd_or_tdd_in_nprach_cfg_choice_ext_ies_o>& choice_ext()
    {
      assert_choice_type(types::choice_ext, type_, "fdd-or-tdd");
      return c.get<protocol_ie_single_container_s<fdd_or_tdd_in_nprach_cfg_choice_ext_ies_o>>();
    }
    const nprach_cfg_fdd_s& fdd() const
    {
      assert_choice_type(types::fdd, type_, "fdd-or-tdd");
      return c.get<nprach_cfg_fdd_s>();
    }
    const nprach_cfg_tdd_s& tdd() const
    {
      assert_choice_type(types::tdd, type_, "fdd-or-tdd");
      return c.get<nprach_cfg_tdd_s>();
    }
    const protocol_ie_single_container_s<fdd_or_tdd_in_nprach_cfg_choice_ext_ies_o>& choice_ext() const
    {
      assert_choice_type(types::choice_ext, type_, "fdd-or-tdd");
      return c.get<protocol_ie_single_container_s<fdd_or_tdd_in_nprach_cfg_choice_ext_ies_o>>();
    }
    nprach_cfg_fdd_s&                                                          set_fdd();
    nprach_cfg_tdd_s&                                                          set_tdd();
    protocol_ie_single_container_s<fdd_or_tdd_in_nprach_cfg_choice_ext_ies_o>& set_choice_ext();

  private:
    types type_;
    choice_buffer_t<nprach_cfg_fdd_s,
                    nprach_cfg_tdd_s,
                    protocol_ie_single_container_s<fdd_or_tdd_in_nprach_cfg_choice_ext_ies_o>>
        c;

    void destroy_();
  };

  // member variables
  bool                         ie_exts_present = false;
  fdd_or_tdd_c_                fdd_or_tdd;
  nprach_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-ChannelInfo-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_u_ch_info_item_ext_ies_o = protocol_ext_empty_o;

using nr_u_ch_info_item_ext_ies_container = protocol_ext_container_empty_l;

// NR-U-ChannelInfo-Item ::= SEQUENCE
struct nr_u_ch_info_item_s {
  bool                                ie_ext_present = false;
  uint8_t                             nr_u_ch_id     = 1;
  uint32_t                            nr_arfcn       = 0;
  bw_e                                bw;
  nr_u_ch_info_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NR-U-ChannelInfo-List ::= SEQUENCE (SIZE (1..16)) OF NR-U-ChannelInfo-Item
using nr_u_ch_info_list_l = dyn_array<nr_u_ch_info_item_s>;

// NRA2XServicesAuthorized-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_a2_x_services_authorized_ext_ies_o = protocol_ext_empty_o;

using nr_a2_x_services_authorized_ext_ies_container = protocol_ext_container_empty_l;

// NRA2XServicesAuthorized ::= SEQUENCE
struct nr_a2_x_services_authorized_s {
  bool                                          aerial_ue_present         = false;
  bool                                          aerial_ctrller_ue_present = false;
  bool                                          ie_exts_present           = false;
  aerial_ue_e                                   aerial_ue;
  aerial_ctrller_ue_e                           aerial_ctrller_ue;
  nr_a2_x_services_authorized_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRModeInfoFDD-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct nr_mode_info_fdd_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct nr_mode_info_fdd_ext_ies_container {
  bool                  ul_carrier_list_present        = false;
  bool                  dl_carrier_list_present        = false;
  bool                  ul_gnb_du_cell_res_cfg_present = false;
  bool                  dl_gnb_du_cell_res_cfg_present = false;
  nr_carrier_list_l     ul_carrier_list;
  nr_carrier_list_l     dl_carrier_list;
  gnb_du_cell_res_cfg_s ul_gnb_du_cell_res_cfg;
  gnb_du_cell_res_cfg_s dl_gnb_du_cell_res_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRModeInfoFDD ::= SEQUENCE
struct nr_mode_info_fdd_s {
  bool                               ie_ext_present = false;
  nr_freq_info_s                     ul_nr_freq_info;
  nr_freq_info_s                     dl_nr_freq_info;
  nr_tx_bw_s                         ul_nr_transmisson_bw;
  nr_tx_bw_s                         dl_nr_transmisson_bw;
  nr_mode_info_fdd_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Transmission-Bandwidth-asymmetric-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tx_bw_asymmetric_ext_ies_o = protocol_ext_empty_o;

using tx_bw_asymmetric_ext_ies_container = protocol_ext_container_empty_l;

// Transmission-Bandwidth-asymmetric ::= SEQUENCE
struct tx_bw_asymmetric_s {
  bool                               ie_exts_present = false;
  nr_tx_bw_s                         ul_tx_bw;
  nr_tx_bw_s                         dl_tx_bw;
  tx_bw_asymmetric_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRModeInfoTDD-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct nr_mode_info_tdd_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct nr_mode_info_tdd_ext_ies_container {
  bool                        intended_tdd_dl_ul_cfg_nr_present = false;
  bool                        tdd_ul_dl_cfg_common_nr_present   = false;
  bool                        carrier_list_present              = false;
  bool                        tdd_gnb_du_cell_res_cfg_present   = false;
  bool                        tx_bw_asymmetric_present          = false;
  intended_tdd_dl_ul_cfg_nr_s intended_tdd_dl_ul_cfg_nr;
  unbounded_octstring<true>   tdd_ul_dl_cfg_common_nr;
  nr_carrier_list_l           carrier_list;
  gnb_du_cell_res_cfg_s       tdd_gnb_du_cell_res_cfg;
  tx_bw_asymmetric_s          tx_bw_asymmetric;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRModeInfoTDD ::= SEQUENCE
struct nr_mode_info_tdd_s {
  bool                               ie_ext_present = false;
  nr_freq_info_s                     nr_freq_info;
  nr_tx_bw_s                         nr_transmisson_bw;
  nr_mode_info_tdd_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRModeInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using nr_mode_info_ext_ies_o = protocol_ies_empty_o;

// NRModeInfo ::= CHOICE
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
  nr_mode_info_fdd_s& fdd()
  {
    assert_choice_type(types::fdd, type_, "NRModeInfo");
    return c.get<nr_mode_info_fdd_s>();
  }
  nr_mode_info_tdd_s& tdd()
  {
    assert_choice_type(types::tdd, type_, "NRModeInfo");
    return c.get<nr_mode_info_tdd_s>();
  }
  protocol_ie_single_container_s<nr_mode_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NRModeInfo");
    return c.get<protocol_ie_single_container_s<nr_mode_info_ext_ies_o>>();
  }
  const nr_mode_info_fdd_s& fdd() const
  {
    assert_choice_type(types::fdd, type_, "NRModeInfo");
    return c.get<nr_mode_info_fdd_s>();
  }
  const nr_mode_info_tdd_s& tdd() const
  {
    assert_choice_type(types::tdd, type_, "NRModeInfo");
    return c.get<nr_mode_info_tdd_s>();
  }
  const protocol_ie_single_container_s<nr_mode_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NRModeInfo");
    return c.get<protocol_ie_single_container_s<nr_mode_info_ext_ies_o>>();
  }
  nr_mode_info_fdd_s&                                     set_fdd();
  nr_mode_info_tdd_s&                                     set_tdd();
  protocol_ie_single_container_s<nr_mode_info_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                           type_;
  choice_buffer_t<nr_mode_info_fdd_s, nr_mode_info_tdd_s, protocol_ie_single_container_s<nr_mode_info_ext_ies_o>> c;

  void destroy_();
};

// NRPPaPositioningInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nrppa_positioning_info_ext_ies_o = protocol_ext_empty_o;

using nrppa_positioning_info_ext_ies_container = protocol_ext_container_empty_l;

// NRPPaPositioningInformation ::= SEQUENCE
struct nrppa_positioning_info_s {
  bool                                     ie_ext_present = false;
  unbounded_octstring<true>                routing_id;
  uint16_t                                 nrppa_transaction_id = 0;
  nrppa_positioning_info_ext_ies_container ie_ext;
  // ...

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

// NRPaging-eDRX-Cycle ::= ENUMERATED
struct nr_paging_e_drx_cycle_opts {
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
using nr_paging_e_drx_cycle_e = enumerated<nr_paging_e_drx_cycle_opts, true>;

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

// NRPagingLongeDRXInformationforRRCINACTIVE-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_paging_longe_drx_infofor_rrc_inactive_ext_ies_o = protocol_ext_empty_o;

using nr_paging_longe_drx_infofor_rrc_inactive_ext_ies_container = protocol_ext_container_empty_l;

// NRPagingLongeDRXInformationforRRCINACTIVE ::= SEQUENCE
struct nr_paging_longe_drx_infofor_rrc_inactive_s {
  bool                                                       ie_exts_present = false;
  nr_paging_long_e_drx_cycle_inactive_e                      nr_paging_long_e_drx_cycle_inactive;
  nr_paging_time_win_inactive_e                              nr_paging_time_win_inactive;
  nr_paging_longe_drx_infofor_rrc_inactive_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRPagingeDRXInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_paginge_drx_info_ext_ies_o = protocol_ext_empty_o;

using nr_paginge_drx_info_ext_ies_container = protocol_ext_container_empty_l;

// NRPagingeDRXInformation ::= SEQUENCE
struct nr_paginge_drx_info_s {
  bool                                  nr_paging_time_win_present = false;
  bool                                  ie_exts_present            = false;
  nr_paging_e_drx_cycle_e               nr_paging_e_drx_cycle;
  nr_paging_time_win_e                  nr_paging_time_win;
  nr_paginge_drx_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRPagingeDRXInformationforRRCINACTIVE-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_paginge_drx_infofor_rrc_inactive_ext_ies_o = protocol_ext_empty_o;

using nr_paginge_drx_infofor_rrc_inactive_ext_ies_container = protocol_ext_container_empty_l;

// NRPagingeDRXInformationforRRCINACTIVE ::= SEQUENCE
struct nr_paginge_drx_infofor_rrc_inactive_s {
  bool                                                  ie_exts_present = false;
  nr_paging_e_drx_cycle_inactive_e                      nr_paging_e_drx_cycle_inactive;
  nr_paginge_drx_infofor_rrc_inactive_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRUESidelinkAggregateMaximumBitRate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using nr_ue_sidelink_aggr_max_bit_rate_ext_ies_o = protocol_ext_empty_o;

using nr_ue_sidelink_aggr_max_bit_rate_ext_ies_container = protocol_ext_container_empty_l;

// NRUESidelinkAggregateMaximumBitRate ::= SEQUENCE
struct nr_ue_sidelink_aggr_max_bit_rate_s {
  bool                                               ie_exts_present               = false;
  uint64_t                                           ue_sidelink_aggr_max_bit_rate = 0;
  nr_ue_sidelink_aggr_max_bit_rate_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NRV2XServicesAuthorized-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Neighbour-NG-RAN-Node-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using neighbour_ng_ran_node_item_ext_ies_o = protocol_ext_empty_o;

using neighbour_ng_ran_node_item_ext_ies_container = protocol_ext_container_empty_l;

// Neighbour-NG-RAN-Node-Item ::= SEQUENCE
struct neighbour_ng_ran_node_item_s {
  bool                                         ie_exts_present = false;
  global_ng_ran_node_id_c                      global_ng_ran_node_id;
  local_ng_ran_node_id_c                       local_ng_ran_node_id;
  neighbour_ng_ran_node_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Neighbour-NG-RAN-Node-List ::= SEQUENCE (SIZE (0..256)) OF Neighbour-NG-RAN-Node-Item
using neighbour_ng_ran_node_list_l = dyn_array<neighbour_ng_ran_node_item_s>;

// NeighbourInformation-E-UTRA-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using neighbour_info_e_utra_item_ext_ies_o = protocol_ext_empty_o;

using neighbour_info_e_utra_item_ext_ies_container = protocol_ext_container_empty_l;

// NeighbourInformation-E-UTRA-Item ::= SEQUENCE
struct neighbour_info_e_utra_item_s {
  bool                                         ranac_present   = false;
  bool                                         ie_exts_present = false;
  uint16_t                                     e_utra_pci      = 0;
  e_utra_cgi_s                                 e_utra_cgi;
  uint32_t                                     earfcn = 0;
  fixed_octstring<3, true>                     tac;
  uint16_t                                     ranac = 0;
  neighbour_info_e_utra_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NeighbourInformation-E-UTRA ::= SEQUENCE (SIZE (1..1024)) OF NeighbourInformation-E-UTRA-Item
using neighbour_info_e_utra_l = dyn_array<neighbour_info_e_utra_item_s>;

// NeighbourInformation-NR-ModeFDDInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using neighbour_info_nr_mode_fdd_info_ext_ies_o = protocol_ext_empty_o;

using neighbour_info_nr_mode_fdd_info_ext_ies_container = protocol_ext_container_empty_l;

// NeighbourInformation-NR-ModeFDDInfo ::= SEQUENCE
struct neighbour_info_nr_mode_fdd_info_s {
  bool                                              ie_exts_present = false;
  nr_freq_info_s                                    ul_nr_freq_info;
  nr_freq_info_s                                    dl_nr_fequ_info;
  neighbour_info_nr_mode_fdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NeighbourInformation-NR-ModeTDDInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using neighbour_info_nr_mode_tdd_info_ext_ies_o = protocol_ext_empty_o;

using neighbour_info_nr_mode_tdd_info_ext_ies_container = protocol_ext_container_empty_l;

// NeighbourInformation-NR-ModeTDDInfo ::= SEQUENCE
struct neighbour_info_nr_mode_tdd_info_s {
  bool                                              ie_exts_present = false;
  nr_freq_info_s                                    nr_freq_info;
  neighbour_info_nr_mode_tdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NeighbourInformation-NR-ModeInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using neighbour_info_nr_mode_info_ext_ies_o = protocol_ies_empty_o;

// NeighbourInformation-NR-ModeInfo ::= CHOICE
struct neighbour_info_nr_mode_info_c {
  struct types_opts {
    enum options { fdd_info, tdd_info, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  neighbour_info_nr_mode_info_c() = default;
  neighbour_info_nr_mode_info_c(const neighbour_info_nr_mode_info_c& other);
  neighbour_info_nr_mode_info_c& operator=(const neighbour_info_nr_mode_info_c& other);
  ~neighbour_info_nr_mode_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  neighbour_info_nr_mode_fdd_info_s& fdd_info()
  {
    assert_choice_type(types::fdd_info, type_, "NeighbourInformation-NR-ModeInfo");
    return c.get<neighbour_info_nr_mode_fdd_info_s>();
  }
  neighbour_info_nr_mode_tdd_info_s& tdd_info()
  {
    assert_choice_type(types::tdd_info, type_, "NeighbourInformation-NR-ModeInfo");
    return c.get<neighbour_info_nr_mode_tdd_info_s>();
  }
  protocol_ie_single_container_s<neighbour_info_nr_mode_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NeighbourInformation-NR-ModeInfo");
    return c.get<protocol_ie_single_container_s<neighbour_info_nr_mode_info_ext_ies_o>>();
  }
  const neighbour_info_nr_mode_fdd_info_s& fdd_info() const
  {
    assert_choice_type(types::fdd_info, type_, "NeighbourInformation-NR-ModeInfo");
    return c.get<neighbour_info_nr_mode_fdd_info_s>();
  }
  const neighbour_info_nr_mode_tdd_info_s& tdd_info() const
  {
    assert_choice_type(types::tdd_info, type_, "NeighbourInformation-NR-ModeInfo");
    return c.get<neighbour_info_nr_mode_tdd_info_s>();
  }
  const protocol_ie_single_container_s<neighbour_info_nr_mode_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NeighbourInformation-NR-ModeInfo");
    return c.get<protocol_ie_single_container_s<neighbour_info_nr_mode_info_ext_ies_o>>();
  }
  neighbour_info_nr_mode_fdd_info_s&                                     set_fdd_info();
  neighbour_info_nr_mode_tdd_info_s&                                     set_tdd_info();
  protocol_ie_single_container_s<neighbour_info_nr_mode_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<neighbour_info_nr_mode_fdd_info_s,
                  neighbour_info_nr_mode_tdd_info_s,
                  protocol_ie_single_container_s<neighbour_info_nr_mode_info_ext_ies_o>>
      c;

  void destroy_();
};

// NeighbourInformation-NR-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct neighbour_info_nr_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct neighbour_info_nr_item_ext_ies_container {
  bool              mobile_iab_cell_present = false;
  mobile_iab_cell_e mobile_iab_cell;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NeighbourInformation-NR-Item ::= SEQUENCE
struct neighbour_info_nr_item_s {
  bool                                     ranac_present   = false;
  bool                                     ie_exts_present = false;
  uint16_t                                 nr_pci          = 0;
  nr_cgi_s                                 nr_cgi;
  fixed_octstring<3, true>                 tac;
  uint16_t                                 ranac = 0;
  neighbour_info_nr_mode_info_c            nr_mode_info;
  connect_support_s                        connect_support;
  unbounded_octstring<true>                meas_timing_cfg;
  neighbour_info_nr_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NeighbourInformation-NR ::= SEQUENCE (SIZE (1..1024)) OF NeighbourInformation-NR-Item
using neighbour_info_nr_l = dyn_array<neighbour_info_nr_item_s>;

// NoPDUSessionIndication ::= ENUMERATED
struct no_pdu_session_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using no_pdu_session_ind_e = enumerated<no_pdu_session_ind_opts, true>;

// NodeAssociatedInfoResult-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using node_associated_info_result_ext_ies_o = protocol_ext_empty_o;

using node_associated_info_result_ext_ies_container = protocol_ext_container_empty_l;

// NodeAssociatedInfoResult ::= SEQUENCE
struct node_associated_info_result_s {
  bool                                          energy_cost_present = false;
  bool                                          ie_exts_present     = false;
  uint16_t                                      energy_cost         = 0;
  node_associated_info_result_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NodeMeasurementInitiationResult-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using node_meas_initiation_result_item_ext_ies_o = protocol_ext_empty_o;

using node_meas_initiation_result_item_ext_ies_container = protocol_ext_container_empty_l;

// NodeMeasurementInitiationResult-Item ::= SEQUENCE
struct node_meas_initiation_result_item_s {
  bool                                               ie_exts_present = false;
  fixed_bitstring<32, false, true>                   nodemeas_failed_report_characteristics;
  cause_c                                            cause;
  node_meas_initiation_result_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NodeMeasurementInitiationResult-List ::= SEQUENCE (SIZE (1..124)) OF NodeMeasurementInitiationResult-Item
using node_meas_initiation_result_list_l = dyn_array<node_meas_initiation_result_item_s>;

// ULNonF1Terminating-BHInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ul_non_f1_terminating_bh_info_ext_ies_o = protocol_ext_empty_o;

using ul_non_f1_terminating_bh_info_ext_ies_container = protocol_ext_container_empty_l;

// ULNonF1Terminating-BHInfo ::= SEQUENCE
struct ul_non_f1_terminating_bh_info_s {
  bool                                            ie_exts_present = false;
  bap_routing_id_s                                egress_bap_routing_id;
  fixed_bitstring<16, false, true>                egress_bh_rlc_ch_id;
  fixed_bitstring<10, false, true>                nexthop_bap_address;
  ul_non_f1_terminating_bh_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NonF1TerminatingBHInformation-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using non_f1_terminating_bh_info_item_ext_ies_o = protocol_ext_empty_o;

using non_f1_terminating_bh_info_item_ext_ies_container = protocol_ext_container_empty_l;

// NonF1TerminatingBHInformation-Item ::= SEQUENCE
struct non_f1_terminating_bh_info_item_s {
  bool                                              dl_non_f1_terminating_bh_info_present = false;
  bool                                              ul_non_f1_terminating_bh_info_present = false;
  bool                                              ie_ext_present                        = false;
  uint16_t                                          bh_info_idx                           = 1;
  dl_non_f1_terminating_bh_info_s                   dl_non_f1_terminating_bh_info;
  ul_non_f1_terminating_bh_info_s                   ul_non_f1_terminating_bh_info;
  non_f1_terminating_bh_info_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NonF1TerminatingBHInformation-List ::= SEQUENCE (SIZE (1..1024)) OF NonF1TerminatingBHInformation-Item
using non_f1_terminating_bh_info_list_l = dyn_array<non_f1_terminating_bh_info_item_s>;

// Non-F1-TerminatingTopologyBHInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using non_f1_terminating_topology_bh_info_ext_ies_o = protocol_ext_empty_o;

using non_f1_terminating_topology_bh_info_ext_ies_container = protocol_ext_container_empty_l;

// Non-F1-TerminatingTopologyBHInformation ::= SEQUENCE
struct non_f1_terminating_topology_bh_info_s {
  bool                                                  ie_exts_present = false;
  non_f1_terminating_bh_info_list_l                     non_f1_terminating_bh_info_list;
  bap_ctrl_pdu_rlc_ch_list_l                            bap_ctrl_pdu_rlc_ch_list;
  non_f1_terminating_topology_bh_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NonGBRResources-Offered ::= ENUMERATED
struct non_gbr_res_offered_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using non_gbr_res_offered_e = enumerated<non_gbr_res_offered_opts, true>;

// NonUPTrafficType ::= ENUMERATED
struct non_up_traffic_type_opts {
  enum options { ueassociatedf1ap, nonueassociatedf1ap, nonf1, /*...*/ nulltype } value;

  const char* to_string() const;
};
using non_up_traffic_type_e = enumerated<non_up_traffic_type_opts, true>;

// NonUPTraffic-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using non_up_traffic_ext_ies_o = protocol_ies_empty_o;

// NonUPTraffic ::= CHOICE
struct non_up_traffic_c {
  struct types_opts {
    enum options { non_up_traffic_type, ctrl_plane_traffic_type, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  non_up_traffic_c() = default;
  non_up_traffic_c(const non_up_traffic_c& other);
  non_up_traffic_c& operator=(const non_up_traffic_c& other);
  ~non_up_traffic_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  non_up_traffic_type_e& non_up_traffic_type()
  {
    assert_choice_type(types::non_up_traffic_type, type_, "NonUPTraffic");
    return c.get<non_up_traffic_type_e>();
  }
  uint8_t& ctrl_plane_traffic_type()
  {
    assert_choice_type(types::ctrl_plane_traffic_type, type_, "NonUPTraffic");
    return c.get<uint8_t>();
  }
  protocol_ie_single_container_s<non_up_traffic_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "NonUPTraffic");
    return c.get<protocol_ie_single_container_s<non_up_traffic_ext_ies_o>>();
  }
  const non_up_traffic_type_e& non_up_traffic_type() const
  {
    assert_choice_type(types::non_up_traffic_type, type_, "NonUPTraffic");
    return c.get<non_up_traffic_type_e>();
  }
  const uint8_t& ctrl_plane_traffic_type() const
  {
    assert_choice_type(types::ctrl_plane_traffic_type, type_, "NonUPTraffic");
    return c.get<uint8_t>();
  }
  const protocol_ie_single_container_s<non_up_traffic_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "NonUPTraffic");
    return c.get<protocol_ie_single_container_s<non_up_traffic_ext_ies_o>>();
  }
  non_up_traffic_type_e&                                    set_non_up_traffic_type();
  uint8_t&                                                  set_ctrl_plane_traffic_type();
  protocol_ie_single_container_s<non_up_traffic_ext_ies_o>& set_choice_ext();

private:
  types                                                                     type_;
  choice_buffer_t<protocol_ie_single_container_s<non_up_traffic_ext_ies_o>> c;

  void destroy_();
};

// NumberOfAntennaPorts-E-UTRA ::= ENUMERATED
struct nof_ant_ports_e_utra_opts {
  enum options { an1, an2, an4, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using nof_ant_ports_e_utra_e = enumerated<nof_ant_ports_e_utra_opts, true>;

// OffsetOfNbiotChannelNumberToEARFCN ::= ENUMERATED
struct offset_of_nbiot_ch_num_to_earfcn_opts {
  enum options {
    minus_ten,
    minus_nine,
    minus_eight_dot_five,
    minus_eight,
    minus_seven,
    minus_six,
    minus_five,
    minus_four_dot_five,
    minus_four,
    minus_three,
    minus_two,
    minus_one,
    minus_zero_dot_five,
    zero,
    one,
    two,
    three,
    three_dot_five,
    four,
    five,
    six,
    seven,
    seven_dot_five,
    eight,
    nine,
    // ...
    nulltype
  } value;

  const char* to_string() const;
};
using offset_of_nbiot_ch_num_to_earfcn_e = enumerated<offset_of_nbiot_ch_num_to_earfcn_opts, true>;

// PC5FlowBitRates-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// PC5QoSFlowItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pc5_qos_flow_item_ext_ies_o = protocol_ext_empty_o;

using pc5_qos_flow_item_ext_ies_container = protocol_ext_container_empty_l;

// PC5QoSFlowItem ::= SEQUENCE
struct pc5_qos_flow_item_s {
  bool                                pc5_flow_bit_rates_present = false;
  bool                                range_present              = false;
  bool                                ie_exts_present            = false;
  uint16_t                            pqi                        = 0;
  pc5_flow_bit_rates_s                pc5_flow_bit_rates;
  range_e                             range;
  pc5_qos_flow_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PC5QoSFlowList ::= SEQUENCE (SIZE (1..2064)) OF PC5QoSFlowItem
using pc5_qos_flow_list_l = dyn_array<pc5_qos_flow_item_s>;

// PC5QoSParameters-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pc5_qos_params_ext_ies_o = protocol_ext_empty_o;

using pc5_qos_params_ext_ies_container = protocol_ext_container_empty_l;

// PC5QoSParameters ::= SEQUENCE
struct pc5_qos_params_s {
  bool                             pc5_link_aggr_bit_rates_present = false;
  bool                             ie_exts_present                 = false;
  pc5_qos_flow_list_l              pc5_qos_flow_list;
  uint64_t                         pc5_link_aggr_bit_rates = 0;
  pc5_qos_params_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDCPChangeIndication-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using pdcp_change_ind_ext_ies_o = protocol_ies_empty_o;

// PDCPChangeIndication ::= CHOICE
struct pdcp_change_ind_c {
  struct from_s_ng_ran_node_opts {
    enum options { s_ng_ran_node_key_upd_required, pdcp_data_recovery_required, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using from_s_ng_ran_node_e_ = enumerated<from_s_ng_ran_node_opts, true>;
  struct from_m_ng_ran_node_opts {
    enum options { pdcp_data_recovery_required, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using from_m_ng_ran_node_e_ = enumerated<from_m_ng_ran_node_opts, true>;
  struct types_opts {
    enum options { from_s_ng_ran_node, from_m_ng_ran_node, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  pdcp_change_ind_c() = default;
  pdcp_change_ind_c(const pdcp_change_ind_c& other);
  pdcp_change_ind_c& operator=(const pdcp_change_ind_c& other);
  ~pdcp_change_ind_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  from_s_ng_ran_node_e_& from_s_ng_ran_node()
  {
    assert_choice_type(types::from_s_ng_ran_node, type_, "PDCPChangeIndication");
    return c.get<from_s_ng_ran_node_e_>();
  }
  from_m_ng_ran_node_e_& from_m_ng_ran_node()
  {
    assert_choice_type(types::from_m_ng_ran_node, type_, "PDCPChangeIndication");
    return c.get<from_m_ng_ran_node_e_>();
  }
  protocol_ie_single_container_s<pdcp_change_ind_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "PDCPChangeIndication");
    return c.get<protocol_ie_single_container_s<pdcp_change_ind_ext_ies_o>>();
  }
  const from_s_ng_ran_node_e_& from_s_ng_ran_node() const
  {
    assert_choice_type(types::from_s_ng_ran_node, type_, "PDCPChangeIndication");
    return c.get<from_s_ng_ran_node_e_>();
  }
  const from_m_ng_ran_node_e_& from_m_ng_ran_node() const
  {
    assert_choice_type(types::from_m_ng_ran_node, type_, "PDCPChangeIndication");
    return c.get<from_m_ng_ran_node_e_>();
  }
  const protocol_ie_single_container_s<pdcp_change_ind_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "PDCPChangeIndication");
    return c.get<protocol_ie_single_container_s<pdcp_change_ind_ext_ies_o>>();
  }
  from_s_ng_ran_node_e_&                                     set_from_s_ng_ran_node();
  from_m_ng_ran_node_e_&                                     set_from_m_ng_ran_node();
  protocol_ie_single_container_s<pdcp_change_ind_ext_ies_o>& set_choice_ext();

private:
  types                                                                      type_;
  choice_buffer_t<protocol_ie_single_container_s<pdcp_change_ind_ext_ies_o>> c;

  void destroy_();
};

// PDUSession-List ::= SEQUENCE (SIZE (1..256)) OF INTEGER (0..255)
using pdu_session_list_l = dyn_array<uint16_t>;

// PDUSession-List-withCause-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_list_with_cause_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_list_with_cause_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSession-List-withCause-Item ::= SEQUENCE
struct pdu_session_list_with_cause_item_s {
  bool                                               cause_present  = false;
  bool                                               ie_ext_present = false;
  uint16_t                                           pdu_session_id = 0;
  cause_c                                            cause;
  pdu_session_list_with_cause_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSession-List-withCause ::= SEQUENCE (SIZE (1..256)) OF PDUSession-List-withCause-Item
using pdu_session_list_with_cause_l = dyn_array<pdu_session_list_with_cause_item_s>;

// PDUSession-List-withDataForwardingFromTarget-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_list_with_data_forwarding_from_target_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_list_with_data_forwarding_from_target_item_ext_ies_container {
  bool       drb_ids_takenintouse_present = false;
  drb_list_l drb_ids_takenintouse;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSession-List-withDataForwardingFromTarget-Item ::= SEQUENCE
struct pdu_session_list_with_data_forwarding_from_target_item_s {
  bool                                                                     ie_ext_present = false;
  uint16_t                                                                 pdu_session_id = 0;
  data_forwarding_info_from_target_ngran_node_s                            dataforwardinginfo_target;
  pdu_session_list_with_data_forwarding_from_target_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSession-List-withDataForwardingFromTarget ::= SEQUENCE (SIZE (1..256)) OF
// PDUSession-List-withDataForwardingFromTarget-Item
using pdu_session_list_with_data_forwarding_from_target_l =
    dyn_array<pdu_session_list_with_data_forwarding_from_target_item_s>;

// PDUSession-List-withDataForwardingRequest-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_list_with_data_forwarding_request_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_list_with_data_forwarding_request_item_ext_ies_container {
  bool    cause_present = false;
  cause_c cause;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSession-List-withDataForwardingRequest-Item ::= SEQUENCE
struct pdu_session_list_with_data_forwarding_request_item_s {
  bool                                                                 dataforwarding_infofrom_source_present = false;
  bool                                                                 ie_ext_present                         = false;
  uint16_t                                                             pdu_session_id                         = 0;
  dataforwardingand_offloading_infofrom_source_s                       dataforwarding_infofrom_source;
  drb_to_qos_flow_map_list_l                                           drb_to_be_released_list;
  pdu_session_list_with_data_forwarding_request_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSession-List-withDataForwardingRequest ::= SEQUENCE (SIZE (1..256)) OF
// PDUSession-List-withDataForwardingRequest-Item
using pdu_session_list_with_data_forwarding_request_l = dyn_array<pdu_session_list_with_data_forwarding_request_item_s>;

// PDUSessionAggregateMaximumBitRate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_aggr_max_bit_rate_ext_ies_o = protocol_ext_empty_o;

using pdu_session_aggr_max_bit_rate_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionAggregateMaximumBitRate ::= SEQUENCE
struct pdu_session_aggr_max_bit_rate_s {
  bool                                            ie_exts_present = false;
  uint64_t                                        dl_session_ambr = 0;
  uint64_t                                        ul_session_ambr = 0;
  pdu_session_aggr_max_bit_rate_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceBearerSetupCompleteInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_bearer_setup_complete_info_sn_terminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_bearer_setup_complete_info_sn_terminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceBearerSetupCompleteInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_bearer_setup_complete_info_sn_terminated_s {
  using drbs_to_be_setup_list_l_ = dyn_array<drbs_to_be_setup_list_bearer_setup_complete_sn_terminated_item_s>;

  // member variables
  bool                                                                       ie_exts_present = false;
  drbs_to_be_setup_list_l_                                                   drbs_to_be_setup_list;
  pdu_session_res_bearer_setup_complete_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceChangeConfirmInfo-MNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_change_confirm_info_m_nterminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_change_confirm_info_m_nterminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceChangeConfirmInfo-MNterminated ::= SEQUENCE
struct pdu_session_res_change_confirm_info_m_nterminated_s {
  bool                                                                ie_exts_present = false;
  pdu_session_res_change_confirm_info_m_nterminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceChangeRequiredInfo-MNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_change_required_info_m_nterminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_change_required_info_m_nterminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceChangeRequiredInfo-MNterminated ::= SEQUENCE
struct pdu_session_res_change_required_info_m_nterminated_s {
  bool                                                                 ie_exts_present = false;
  pdu_session_res_change_required_info_m_nterminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceChangeRequiredInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_change_required_info_sn_terminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_change_required_info_sn_terminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceChangeRequiredInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_change_required_info_sn_terminated_s {
  bool                                                                 dataforwardinginfofrom_source_present = false;
  bool                                                                 ie_exts_present                       = false;
  dataforwardingand_offloading_infofrom_source_s                       dataforwardinginfofrom_source;
  pdu_session_res_change_required_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModConfirmInfo-MNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_mod_confirm_info_m_nterminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_mod_confirm_info_m_nterminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceModConfirmInfo-MNterminated ::= SEQUENCE
struct pdu_session_res_mod_confirm_info_m_nterminated_s {
  bool                                                             ie_exts_present = false;
  pdu_session_res_mod_confirm_info_m_nterminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModConfirmInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_mod_confirm_info_sn_terminated_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_mod_confirm_info_sn_terminated_ext_ies_container {
  bool       drb_ids_takenintouse_present = false;
  drb_list_l drb_ids_takenintouse;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModConfirmInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_mod_confirm_info_sn_terminated_s {
  bool                                                             ul_ng_u_tnl_at_up_f_present       = false;
  bool                                                             dataforwardinginfo_target_present = false;
  bool                                                             ie_exts_present                   = false;
  up_transport_layer_info_c                                        ul_ng_u_tnl_at_up_f;
  drbs_admitted_list_mod_confirm_sn_terminated_l                   drbs_admitted_list;
  drb_list_with_cause_l                                            drbs_not_admitted_setup_modify_list;
  data_forwarding_info_from_target_ngran_node_s                    dataforwardinginfo_target;
  pdu_session_res_mod_confirm_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModRqdInfo-MNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_mod_rqd_info_m_nterminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_mod_rqd_info_m_nterminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceModRqdInfo-MNterminated ::= SEQUENCE
struct pdu_session_res_mod_rqd_info_m_nterminated_s {
  bool                                                         ie_exts_present = false;
  drbs_to_be_modified_list_mod_rqd_m_nterminated_l             drbs_to_be_modified;
  drb_list_with_cause_l                                        drbs_to_be_released;
  pdu_session_res_mod_rqd_info_m_nterminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModRqdInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_mod_rqd_info_sn_terminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_mod_rqd_info_sn_terminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceModRqdInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_mod_rqd_info_sn_terminated_s {
  bool                                                         dl_ng_u_tnl_at_ng_ran_present         = false;
  bool                                                         dataforwardinginfofrom_source_present = false;
  bool                                                         ie_exts_present                       = false;
  up_transport_layer_info_c                                    dl_ng_u_tnl_at_ng_ran;
  qos_flows_list_with_cause_l                                  qos_flows_to_be_released_list;
  dataforwardingand_offloading_infofrom_source_s               dataforwardinginfofrom_source;
  drbs_to_be_setup_list_mod_rqd_sn_terminated_l                drbs_to_be_setup_list;
  drbs_to_be_modified_list_mod_rqd_sn_terminated_l             drbs_to_be_modified_list;
  drb_list_with_cause_l                                        drbs_to_be_released;
  pdu_session_res_mod_rqd_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionType ::= ENUMERATED
struct pdu_session_type_opts {
  enum options { ipv4, ipv6, ipv4v6, ethernet, unstructured, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pdu_session_type_e = enumerated<pdu_session_type_opts, true>;

// PDUSessionResourceModificationInfo-MNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_mod_info_m_nterminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_mod_info_m_nterminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceModificationInfo-MNterminated ::= SEQUENCE
struct pdu_session_res_mod_info_m_nterminated_s {
  bool                                                     ie_exts_present = false;
  pdu_session_type_e                                       pdu_session_type;
  drbs_to_be_setup_list_setup_m_nterminated_l              drbs_to_be_setup;
  drbs_to_be_modified_list_mod_m_nterminated_l             drbs_to_be_modified;
  drb_list_with_cause_l                                    drbs_to_be_released;
  pdu_session_res_mod_info_m_nterminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RedundantQoSFlowIndicator ::= ENUMERATED
struct redundant_qos_flow_ind_opts {
  enum options { true_value, false_value, nulltype } value;

  const char* to_string() const;
};
using redundant_qos_flow_ind_e = enumerated<redundant_qos_flow_ind_opts>;

// QoSFlowsToBeSetup-List-Setup-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flows_to_be_setup_list_setup_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flows_to_be_setup_list_setup_sn_terminated_item_ext_ies_container {
  bool                          tsc_traffic_characteristics_present = false;
  bool                          redundant_qos_flow_ind_present      = false;
  tsc_traffic_characteristics_s tsc_traffic_characteristics;
  redundant_qos_flow_ind_e      redundant_qos_flow_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsToBeSetup-List-Setup-SNterminated-Item ::= SEQUENCE
struct qos_flows_to_be_setup_list_setup_sn_terminated_item_s {
  bool                                                                  offered_gbr_qos_flow_info_present = false;
  bool                                                                  ie_exts_present                   = false;
  uint8_t                                                               qfi                               = 0;
  qos_flow_level_qos_params_s                                           qos_flow_level_qos_params;
  gbr_qos_flow_info_s                                                   offered_gbr_qos_flow_info;
  qos_flows_to_be_setup_list_setup_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsToBeSetup-List-Setup-SNterminated ::= SEQUENCE (SIZE (1..64)) OF
// QoSFlowsToBeSetup-List-Setup-SNterminated-Item
using qos_flows_to_be_setup_list_setup_sn_terminated_l =
    dyn_array<qos_flows_to_be_setup_list_setup_sn_terminated_item_s>;

// QoSFlowsToBeSetup-List-Modified-SNterminated-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flows_to_be_setup_list_modified_sn_terminated_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flows_to_be_setup_list_modified_sn_terminated_item_ext_ies_container {
  bool                          tsc_traffic_characteristics_present = false;
  bool                          redundant_qos_flow_ind_present      = false;
  tsc_traffic_characteristics_s tsc_traffic_characteristics;
  redundant_qos_flow_ind_e      redundant_qos_flow_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsToBeSetup-List-Modified-SNterminated-Item ::= SEQUENCE
struct qos_flows_to_be_setup_list_modified_sn_terminated_item_s {
  bool                                                                     qos_flow_level_qos_params_present = false;
  bool                                                                     offered_gbr_qos_flow_info_present = false;
  bool                                                                     qos_flow_map_ind_present          = false;
  bool                                                                     ie_exts_present                   = false;
  uint8_t                                                                  qfi                               = 0;
  qos_flow_level_qos_params_s                                              qos_flow_level_qos_params;
  gbr_qos_flow_info_s                                                      offered_gbr_qos_flow_info;
  qos_flow_map_ind_e                                                       qos_flow_map_ind;
  qos_flows_to_be_setup_list_modified_sn_terminated_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsToBeSetup-List-Modified-SNterminated ::= SEQUENCE (SIZE (1..64)) OF
// QoSFlowsToBeSetup-List-Modified-SNterminated-Item
using qos_flows_to_be_setup_list_modified_sn_terminated_l =
    dyn_array<qos_flows_to_be_setup_list_modified_sn_terminated_item_s>;

// SecurityIndication-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using security_ind_ext_ies_o = protocol_ext_empty_o;

using security_ind_ext_ies_container = protocol_ext_container_empty_l;

// SecurityIndication ::= SEQUENCE
struct security_ind_s {
  struct integrity_protection_ind_opts {
    enum options { required, preferred, not_needed, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using integrity_protection_ind_e_ = enumerated<integrity_protection_ind_opts, true>;
  struct confidentiality_protection_ind_opts {
    enum options { required, preferred, not_needed, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using confidentiality_protection_ind_e_ = enumerated<confidentiality_protection_ind_opts, true>;

  // member variables
  bool                              max_ip_datarate_present = false;
  bool                              ie_exts_present         = false;
  integrity_protection_ind_e_       integrity_protection_ind;
  confidentiality_protection_ind_e_ confidentiality_protection_ind;
  max_ip_datarate_s                 max_ip_datarate;
  security_ind_ext_ies_container    ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModificationInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_mod_info_sn_terminated_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_mod_info_sn_terminated_ext_ies_container {
  bool                      pdu_session_common_network_instance_present = false;
  bool                      default_drb_allowed_present                 = false;
  bool                      non_gbr_res_offered_present                 = false;
  bool                      redundant_ul_ng_u_tnl_at_up_f_present       = false;
  bool                      redundant_common_network_instance_present   = false;
  bool                      security_ind_present                        = false;
  unbounded_octstring<true> pdu_session_common_network_instance;
  default_drb_allowed_e     default_drb_allowed;
  non_gbr_res_offered_e     non_gbr_res_offered;
  up_transport_layer_info_c redundant_ul_ng_u_tnl_at_up_f;
  unbounded_octstring<true> redundant_common_network_instance;
  security_ind_s            security_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModificationInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_mod_info_sn_terminated_s {
  bool                                                     ul_ng_u_tnl_at_up_f_present           = false;
  bool                                                     pdu_session_network_instance_present  = false;
  bool                                                     dataforwardinginfofrom_source_present = false;
  bool                                                     ie_exts_present                       = false;
  up_transport_layer_info_c                                ul_ng_u_tnl_at_up_f;
  uint16_t                                                 pdu_session_network_instance = 1;
  qos_flows_to_be_setup_list_setup_sn_terminated_l         qos_flows_to_be_setup_list;
  dataforwardingand_offloading_infofrom_source_s           dataforwardinginfofrom_source;
  qos_flows_to_be_setup_list_modified_sn_terminated_l      qos_flows_to_be_modified_list;
  qos_flows_list_with_cause_l                              qos_flows_to_be_released_list;
  drbs_to_be_modified_list_modified_sn_terminated_l        drbs_to_be_modified_list;
  drb_list_with_cause_l                                    drbs_to_be_released;
  pdu_session_res_mod_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModificationResponseInfo-MNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_mod_resp_info_m_nterminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_mod_resp_info_m_nterminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceModificationResponseInfo-MNterminated ::= SEQUENCE
struct pdu_session_res_mod_resp_info_m_nterminated_s {
  bool                                                          ie_exts_present = false;
  drbs_admitted_list_mod_resp_m_nterminated_l                   drbs_admitted_list;
  drb_list_l                                                    drbs_released_list;
  drb_list_with_cause_l                                         drbs_not_admitted_setup_modify_list;
  pdu_session_res_mod_resp_info_m_nterminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SecurityResult-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using security_result_ext_ies_o = protocol_ext_empty_o;

using security_result_ext_ies_container = protocol_ext_container_empty_l;

// SecurityResult ::= SEQUENCE
struct security_result_s {
  struct integrity_protection_result_opts {
    enum options { performed, not_performed, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using integrity_protection_result_e_ = enumerated<integrity_protection_result_opts, true>;
  struct confidentiality_protection_result_opts {
    enum options { performed, not_performed, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using confidentiality_protection_result_e_ = enumerated<confidentiality_protection_result_opts, true>;

  // member variables
  bool                                 ie_exts_present = false;
  integrity_protection_result_e_       integrity_protection_result;
  confidentiality_protection_result_e_ confidentiality_protection_result;
  security_result_ext_ies_container    ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModificationResponseInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_mod_resp_info_sn_terminated_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_mod_resp_info_sn_terminated_ext_ies_container {
  bool                      drb_ids_takenintouse_present            = false;
  bool                      redundant_dl_ng_u_tnl_at_ng_ran_present = false;
  bool                      security_result_present                 = false;
  drb_list_l                drb_ids_takenintouse;
  up_transport_layer_info_c redundant_dl_ng_u_tnl_at_ng_ran;
  security_result_s         security_result;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceModificationResponseInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_mod_resp_info_sn_terminated_s {
  bool                                                          dl_ng_u_tnl_at_ng_ran_present         = false;
  bool                                                          dataforwardinginfo_target_present     = false;
  bool                                                          dataforwardinginfofrom_source_present = false;
  bool                                                          ie_exts_present                       = false;
  up_transport_layer_info_c                                     dl_ng_u_tnl_at_ng_ran;
  drbs_to_be_setup_list_setup_resp_sn_terminated_l              drbs_to_be_setup;
  data_forwarding_info_from_target_ngran_node_s                 dataforwardinginfo_target;
  drbs_to_be_modified_list_mod_resp_sn_terminated_l             drbs_to_be_modified;
  drb_list_with_cause_l                                         drbs_to_be_released;
  dataforwardingand_offloading_infofrom_source_s                dataforwardinginfofrom_source;
  qos_flows_list_with_cause_l                                   qos_flows_not_admitted_tb_added;
  qos_flows_list_with_cause_l                                   qos_flows_released;
  pdu_session_res_mod_resp_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// VolumeTimedReport-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using volume_timed_report_item_ext_ies_o = protocol_ext_empty_o;

using volume_timed_report_item_ext_ies_container = protocol_ext_container_empty_l;

// VolumeTimedReport-Item ::= SEQUENCE
struct volume_timed_report_item_s {
  bool                                       ie_exts_present = false;
  fixed_octstring<4, true>                   start_time_stamp;
  fixed_octstring<4, true>                   end_time_stamp;
  uint64_t                                   usage_count_ul = 0;
  uint64_t                                   usage_count_dl = 0;
  volume_timed_report_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// VolumeTimedReportList ::= SEQUENCE (SIZE (1..2)) OF VolumeTimedReport-Item
using volume_timed_report_list_l = dyn_array<volume_timed_report_item_s>;

// PDUSessionUsageReport-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_usage_report_ext_ies_o = protocol_ext_empty_o;

using pdu_session_usage_report_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionUsageReport ::= SEQUENCE
struct pdu_session_usage_report_s {
  struct rat_type_opts {
    enum options { nr, eutra, /*...*/ nr_unlicensed, e_utra_unlicensed, nulltype } value;

    const char* to_string() const;
  };
  using rat_type_e_ = enumerated<rat_type_opts, true, 2>;

  // member variables
  bool                                       ie_exts_present = false;
  rat_type_e_                                rat_type;
  volume_timed_report_list_l                 pdu_session_timed_report_list;
  pdu_session_usage_report_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsUsageReport-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qos_flows_usage_report_item_ext_ies_o = protocol_ext_empty_o;

using qos_flows_usage_report_item_ext_ies_container = protocol_ext_container_empty_l;

// QoSFlowsUsageReport-Item ::= SEQUENCE
struct qos_flows_usage_report_item_s {
  struct rat_type_opts {
    enum options { nr, eutra, /*...*/ nr_unlicensed, e_utra_unlicensed, nulltype } value;

    const char* to_string() const;
  };
  using rat_type_e_ = enumerated<rat_type_opts, true, 2>;

  // member variables
  bool                                          ie_exts_present = false;
  uint8_t                                       qos_flow_id     = 0;
  rat_type_e_                                   rat_type;
  volume_timed_report_list_l                    qos_flows_timed_report_list;
  qos_flows_usage_report_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsUsageReportList ::= SEQUENCE (SIZE (1..64)) OF QoSFlowsUsageReport-Item
using qos_flows_usage_report_list_l = dyn_array<qos_flows_usage_report_item_s>;

// SecondaryRATUsageInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using secondary_rat_usage_info_ext_ies_o = protocol_ext_empty_o;

using secondary_rat_usage_info_ext_ies_container = protocol_ext_container_empty_l;

// SecondaryRATUsageInformation ::= SEQUENCE
struct secondary_rat_usage_info_s {
  bool                                       pdu_session_usage_report_present = false;
  bool                                       ie_ext_present                   = false;
  pdu_session_usage_report_s                 pdu_session_usage_report;
  qos_flows_usage_report_list_l              qos_flows_usage_report_list;
  secondary_rat_usage_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceSecondaryRATUsageItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_secondary_rat_usage_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_secondary_rat_usage_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceSecondaryRATUsageItem ::= SEQUENCE
struct pdu_session_res_secondary_rat_usage_item_s {
  bool                                                       ie_exts_present = false;
  uint16_t                                                   pdu_session_id  = 0;
  secondary_rat_usage_info_s                                 secondary_rat_usage_info;
  pdu_session_res_secondary_rat_usage_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceSecondaryRATUsageList ::= SEQUENCE (SIZE (1..256)) OF PDUSessionResourceSecondaryRATUsageItem
using pdu_session_res_secondary_rat_usage_list_l = dyn_array<pdu_session_res_secondary_rat_usage_item_s>;

// PDUSessionResourceSetupInfo-MNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_setup_info_m_nterminated_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_setup_info_m_nterminated_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourceSetupInfo-MNterminated ::= SEQUENCE
struct pdu_session_res_setup_info_m_nterminated_s {
  bool                                                       ie_exts_present = false;
  pdu_session_type_e                                         pdu_session_type;
  drbs_to_be_setup_list_setup_m_nterminated_l                drbs_to_be_setup;
  pdu_session_res_setup_info_m_nterminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SplitSessionIndicator ::= ENUMERATED
struct split_session_ind_opts {
  enum options { split, /*...*/ nulltype } value;

  const char* to_string() const;
};
using split_session_ind_e = enumerated<split_session_ind_opts, true>;

// RSN ::= ENUMERATED
struct r_sn_opts {
  enum options { v1, v2, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using r_sn_e = enumerated<r_sn_opts, true>;

// RedundantPDUSessionInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct redundant_pdu_session_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct redundant_pdu_session_info_ext_ies_container {
  bool     pdu_session_pair_id_present = false;
  uint16_t pdu_session_pair_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RedundantPDUSessionInformation ::= SEQUENCE
struct redundant_pdu_session_info_s {
  bool                                         ie_exts_present = false;
  r_sn_e                                       rsn;
  redundant_pdu_session_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceSetupInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_setup_info_sn_terminated_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_setup_info_sn_terminated_ext_ies_container {
  bool                         security_result_present                     = false;
  bool                         pdu_session_common_network_instance_present = false;
  bool                         default_drb_allowed_present                 = false;
  bool                         split_session_ind_present                   = false;
  bool                         non_gbr_res_offered_present                 = false;
  bool                         redundant_ul_ng_u_tnl_at_up_f_present       = false;
  bool                         redundant_common_network_instance_present   = false;
  bool                         redundant_pdu_session_info_present          = false;
  security_result_s            security_result;
  unbounded_octstring<true>    pdu_session_common_network_instance;
  default_drb_allowed_e        default_drb_allowed;
  split_session_ind_e          split_session_ind;
  non_gbr_res_offered_e        non_gbr_res_offered;
  up_transport_layer_info_c    redundant_ul_ng_u_tnl_at_up_f;
  unbounded_octstring<true>    redundant_common_network_instance;
  redundant_pdu_session_info_s redundant_pdu_session_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceSetupInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_setup_info_sn_terminated_s {
  bool                                                       pdu_session_network_instance_present  = false;
  bool                                                       dataforwardinginfofrom_source_present = false;
  bool                                                       security_ind_present                  = false;
  bool                                                       ie_exts_present                       = false;
  up_transport_layer_info_c                                  ul_ng_u_tnl_at_up_f;
  pdu_session_type_e                                         pdu_session_type;
  uint16_t                                                   pdu_session_network_instance = 1;
  qos_flows_to_be_setup_list_setup_sn_terminated_l           qos_flows_to_be_setup_list;
  dataforwardingand_offloading_infofrom_source_s             dataforwardinginfofrom_source;
  security_ind_s                                             security_ind;
  pdu_session_res_setup_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceSetupResponseInfo-MNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_setup_resp_info_m_nterminated_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_setup_resp_info_m_nterminated_ext_ies_container {
  bool                  drbs_not_admitted_setup_modify_list_present = false;
  drb_list_with_cause_l drbs_not_admitted_setup_modify_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceSetupResponseInfo-MNterminated ::= SEQUENCE
struct pdu_session_res_setup_resp_info_m_nterminated_s {
  bool                                                            ie_exts_present = false;
  drbs_admitted_list_setup_resp_m_nterminated_l                   drbs_admitted_list;
  pdu_session_res_setup_resp_info_m_nterminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceSetupResponseInfo-SNterminated-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_setup_resp_info_sn_terminated_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_setup_resp_info_sn_terminated_ext_ies_container {
  bool                                           drb_ids_takenintouse_present                  = false;
  bool                                           redundant_dl_ng_u_tnl_at_ng_ran_present       = false;
  bool                                           used_r_sn_info_present                        = false;
  bool                                           s_cp_ac_dataforwardinginfofrom_source_present = false;
  drb_list_l                                     drb_ids_takenintouse;
  up_transport_layer_info_c                      redundant_dl_ng_u_tnl_at_ng_ran;
  redundant_pdu_session_info_s                   used_r_sn_info;
  dataforwardingand_offloading_infofrom_source_s s_cp_ac_dataforwardinginfofrom_source;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourceSetupResponseInfo-SNterminated ::= SEQUENCE
struct pdu_session_res_setup_resp_info_sn_terminated_s {
  bool                                                            dataforwardinginfo_target_present = false;
  bool                                                            security_result_present           = false;
  bool                                                            ie_exts_present                   = false;
  up_transport_layer_info_c                                       dl_ng_u_tnl_at_ng_ran;
  drbs_to_be_setup_list_setup_resp_sn_terminated_l                drbs_to_be_setup;
  data_forwarding_info_from_target_ngran_node_s                   dataforwardinginfo_target;
  qos_flows_list_with_cause_l                                     qos_flows_not_admitted_list;
  security_result_s                                               security_result;
  pdu_session_res_setup_resp_info_sn_terminated_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourcesNotAdmitted-Item-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_session_res_not_admitted_item_item_ext_ies_o = protocol_ext_empty_o;

using pdu_session_res_not_admitted_item_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionResourcesNotAdmitted-Item ::= SEQUENCE
struct pdu_session_res_not_admitted_item_s {
  bool                                                     cause_present  = false;
  bool                                                     ie_ext_present = false;
  uint16_t                                                 pdu_session_id = 0;
  cause_c                                                  cause;
  pdu_session_res_not_admitted_item_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourcesNotAdmitted-List ::= SEQUENCE (SIZE (1..256)) OF PDUSessionResourcesNotAdmitted-Item
using pdu_session_res_not_admitted_list_l = dyn_array<pdu_session_res_not_admitted_item_s>;

// QoSFlowsToBeSetup-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flows_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flows_to_be_setup_item_ext_ies_container {
  bool                                           tsc_traffic_characteristics_present                  = false;
  bool                                           redundant_qos_flow_ind_present                       = false;
  bool                                           ecn_markingor_congestion_info_report_request_present = false;
  tsc_traffic_characteristics_s                  tsc_traffic_characteristics;
  redundant_qos_flow_ind_e                       redundant_qos_flow_ind;
  ecn_markingor_congestion_info_report_request_c ecn_markingor_congestion_info_report_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsToBeSetup-Item ::= SEQUENCE
struct qos_flows_to_be_setup_item_s {
  bool                                         erab_id_present = false;
  bool                                         ie_ext_present  = false;
  uint8_t                                      qfi             = 0;
  qos_flow_level_qos_params_s                  qos_flow_level_qos_params;
  uint8_t                                      erab_id = 0;
  qos_flows_to_be_setup_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowsToBeSetup-List ::= SEQUENCE (SIZE (1..64)) OF QoSFlowsToBeSetup-Item
using qos_flows_to_be_setup_list_l = dyn_array<qos_flows_to_be_setup_item_s>;

// PDUSessionResourcesToBeSetup-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct pdu_session_res_to_be_setup_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct pdu_session_res_to_be_setup_item_ext_ies_container {
  bool                           add_ul_ng_u_tnl_at_up_f_list_present           = false;
  bool                           pdu_session_common_network_instance_present    = false;
  bool                           redundant_ul_ng_u_tnl_at_up_f_present          = false;
  bool                           add_redundant_ul_ng_u_tnl_at_up_f_list_present = false;
  bool                           redundant_common_network_instance_present      = false;
  bool                           redundant_pdu_session_info_present             = false;
  bool                           mbs_session_associated_info_present            = false;
  add_ul_ng_u_tnl_at_up_f_list_l add_ul_ng_u_tnl_at_up_f_list;
  unbounded_octstring<true>      pdu_session_common_network_instance;
  up_transport_layer_info_c      redundant_ul_ng_u_tnl_at_up_f;
  add_ul_ng_u_tnl_at_up_f_list_l add_redundant_ul_ng_u_tnl_at_up_f_list;
  unbounded_octstring<true>      redundant_common_network_instance;
  redundant_pdu_session_info_s   redundant_pdu_session_info;
  mbs_session_associated_info_l  mbs_session_associated_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourcesToBeSetup-Item ::= SEQUENCE
struct pdu_session_res_to_be_setup_item_s {
  bool                                               pdu_session_ambr_present              = false;
  bool                                               source_dl_ng_u_tnl_info_present       = false;
  bool                                               security_ind_present                  = false;
  bool                                               pdu_session_network_instance_present  = false;
  bool                                               dataforwardinginfofrom_source_present = false;
  bool                                               ie_exts_present                       = false;
  uint16_t                                           pdu_session_id                        = 0;
  s_nssai_s                                          s_nssai;
  pdu_session_aggr_max_bit_rate_s                    pdu_session_ambr;
  up_transport_layer_info_c                          ul_ng_u_tnl_at_up_f;
  up_transport_layer_info_c                          source_dl_ng_u_tnl_info;
  security_ind_s                                     security_ind;
  pdu_session_type_e                                 pdu_session_type;
  uint16_t                                           pdu_session_network_instance = 1;
  qos_flows_to_be_setup_list_l                       qos_flows_to_be_setup_list;
  dataforwardingand_offloading_infofrom_source_s     dataforwardinginfofrom_source;
  pdu_session_res_to_be_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionResourcesToBeSetup-List ::= SEQUENCE (SIZE (1..256)) OF PDUSessionResourcesToBeSetup-Item
using pdu_session_res_to_be_setup_list_l = dyn_array<pdu_session_res_to_be_setup_item_s>;

// UserPlaneErrorIndicator ::= ENUMERATED
struct user_plane_error_ind_opts {
  enum options { gtpu_error_ind_rx, /*...*/ nulltype } value;

  const char* to_string() const;
};
using user_plane_error_ind_e = enumerated<user_plane_error_ind_opts, true>;

// PDUSessionsListToBeReleased-UPError-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pdu_sessions_list_to_be_released_up_error_item_ext_ies_o = protocol_ext_empty_o;

using pdu_sessions_list_to_be_released_up_error_item_ext_ies_container = protocol_ext_container_empty_l;

// PDUSessionsListToBeReleased-UPError-Item ::= SEQUENCE
struct pdu_sessions_list_to_be_released_up_error_item_s {
  bool                                                             ie_ext_present = false;
  uint16_t                                                         pdu_session_id = 0;
  user_plane_error_ind_e                                           user_plane_error_ind;
  pdu_sessions_list_to_be_released_up_error_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PDUSessionsListToBeReleased-UPError ::= SEQUENCE (SIZE (1..256)) OF PDUSessionsListToBeReleased-UPError-Item
using pdu_sessions_list_to_be_released_up_error_l = dyn_array<pdu_sessions_list_to_be_released_up_error_item_s>;

// PDUSetbasedHandlingIndicator ::= ENUMERATED
struct pdu_setbased_handling_ind_opts {
  enum options { supported, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pdu_setbased_handling_ind_e = enumerated<pdu_setbased_handling_ind_opts, true>;

// PEIPSassistanceInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pe_ip_sassist_info_ext_ies_o = protocol_ext_empty_o;

using pe_ip_sassist_info_ext_ies_container = protocol_ext_container_empty_l;

// PEIPSassistanceInformation ::= SEQUENCE
struct pe_ip_sassist_info_s {
  bool                                 ie_exts_present = false;
  uint8_t                              cnsubgroup_id   = 0;
  pe_ip_sassist_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PSCellChangeHistory ::= ENUMERATED
struct pscell_change_history_opts {
  enum options { report_full_history, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pscell_change_history_e = enumerated<pscell_change_history_opts, true>;

// PSCellHistoryInformationRetrieve ::= ENUMERATED
struct pscell_history_info_retrieve_opts {
  enum options { query, /*...*/ nulltype } value;

  const char* to_string() const;
};
using pscell_history_info_retrieve_e = enumerated<pscell_history_info_retrieve_opts, true>;

// PagingCause ::= ENUMERATED
struct paging_cause_opts {
  enum options { voice, /*...*/ nulltype } value;

  const char* to_string() const;
};
using paging_cause_e = enumerated<paging_cause_opts, true>;

// PagingDRX ::= ENUMERATED
struct paging_drx_opts {
  enum options { v32, v64, v128, v256, /*...*/ v512, v1024, nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using paging_drx_e = enumerated<paging_drx_opts, true, 2>;

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

// PartialListIndicator ::= ENUMERATED
struct partial_list_ind_opts {
  enum options { partial, /*...*/ nulltype } value;

  const char* to_string() const;
};
using partial_list_ind_e = enumerated<partial_list_ind_opts, true>;

// PosPartialUEContextInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using pos_partial_ue_context_info_ext_ies_o = protocol_ext_empty_o;

using pos_partial_ue_context_info_ext_ies_container = protocol_ext_container_empty_l;

// PosPartialUEContextInfo ::= SEQUENCE
struct pos_partial_ue_context_info_s {
  bool                                          ie_exts_present = false;
  unbounded_octstring<true>                     requested_srs_tx_characteristics;
  pos_partial_ue_context_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using positioning_info_ext_ies_o = protocol_ext_empty_o;

using positioning_info_ext_ies_container = protocol_ext_container_empty_l;

// PositioningInformation ::= SEQUENCE
struct positioning_info_s {
  bool                               ie_ext_present = false;
  unbounded_octstring<true>          requested_srs_tx_characteristics;
  unbounded_octstring<true>          routing_id;
  uint16_t                           nrppa_transaction_id = 0;
  positioning_info_ext_ies_container ie_ext;
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

// ProtectedE-UTRAFootprintTimePattern-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using protected_e_utra_footprint_time_pattern_ext_ies_o = protocol_ext_empty_o;

using protected_e_utra_footprint_time_pattern_ext_ies_container = protocol_ext_container_empty_l;

// ProtectedE-UTRAFootprintTimePattern ::= SEQUENCE
struct protected_e_utra_footprint_time_pattern_s {
  bool                                                      ie_exts_present                     = false;
  uint16_t                                                  protected_footprint_timeperiodicity = 1;
  uint8_t                                                   protected_footrpint_start_time      = 1;
  protected_e_utra_footprint_time_pattern_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ProtectedE-UTRAResource-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using protected_e_utra_res_item_ext_ies_o = protocol_ext_empty_o;

using protected_e_utra_res_item_ext_ies_container = protocol_ext_container_empty_l;

// ProtectedE-UTRAResource-Item ::= SEQUENCE
struct protected_e_utra_res_item_s {
  struct res_type_opts {
    enum options { dlnon_crs, crs, ul, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using res_type_e_ = enumerated<res_type_opts, true>;

  // member variables
  bool                                        ie_exts_present = false;
  res_type_e_                                 res_type;
  fixed_bitstring<84, true, true>             intra_prb_protected_res_footprint;
  bounded_bitstring<6, 110, true, true>       protected_footprint_freq_pattern;
  protected_e_utra_footprint_time_pattern_s   protected_footprint_time_pattern;
  protected_e_utra_res_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ProtectedE-UTRAResourceList ::= SEQUENCE (SIZE (1..16)) OF ProtectedE-UTRAResource-Item
using protected_e_utra_res_list_l = dyn_array<protected_e_utra_res_item_s>;

// ProtectedE-UTRAResourceIndication-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using protected_e_utra_res_ind_ext_ies_o = protocol_ext_empty_o;

using protected_e_utra_res_ind_ext_ies_container = protocol_ext_container_empty_l;

// ProtectedE-UTRAResourceIndication ::= SEQUENCE
struct protected_e_utra_res_ind_s {
  bool                                       mbsfn_ctrl_region_len_present = false;
  bool                                       ie_exts_present               = false;
  uint16_t                                   activation_sfn                = 0;
  protected_e_utra_res_list_l                protected_res_list;
  uint8_t                                    mbsfn_ctrl_region_len = 0;
  uint8_t                                    pdcch_region_len      = 1;
  protected_e_utra_res_ind_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServiceType ::= ENUMERATED
struct service_type_opts {
  enum options { qmc_for_streaming_service, qmc_for_mt_si_service, qmc_for_vr_service, /*...*/ nulltype } value;

  const char* to_string() const;
};
using service_type_e = enumerated<service_type_opts, true>;

// QOEMeasStatus ::= ENUMERATED
struct qoe_meas_status_opts {
  enum options { ongoing, /*...*/ nulltype } value;

  const char* to_string() const;
};
using qoe_meas_status_e = enumerated<qoe_meas_status_opts, true>;

// S-NSSAIListQoE ::= SEQUENCE (SIZE (1..16)) OF S-NSSAI
using s_nssai_list_qo_e_l = dyn_array<s_nssai_s>;

// QoERVQoEReportingPaths-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using qo_erv_qo_e_report_paths_ext_ies_o = protocol_ext_empty_o;

using qo_erv_qo_e_report_paths_ext_ies_container = protocol_ext_container_empty_l;

// QoERVQoEReportingPaths ::= SEQUENCE
struct qo_erv_qo_e_report_paths_s {
  struct qo_e_report_path_opts {
    enum options { srb4, srb5, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using qo_e_report_path_e_ = enumerated<qo_e_report_path_opts, true>;
  struct rv_qo_e_report_path_opts {
    enum options { srb4, srb5, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using rv_qo_e_report_path_e_ = enumerated<rv_qo_e_report_path_opts, true>;

  // member variables
  bool                                       qo_e_report_path_present    = false;
  bool                                       rv_qo_e_report_path_present = false;
  bool                                       ie_exts_present             = false;
  qo_e_report_path_e_                        qo_e_report_path;
  rv_qo_e_report_path_e_                     rv_qo_e_report_path;
  qo_erv_qo_e_report_paths_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEAppLayerMeasConfigInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ue_app_layer_meas_cfg_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_app_layer_meas_cfg_info_ext_ies_container {
  bool                       mbs_comm_service_type_present    = false;
  bool                       assist_info_qo_e_meas_present    = false;
  bool                       qo_erv_qo_e_report_paths_present = false;
  mbs_comm_service_type_e    mbs_comm_service_type;
  uint8_t                    assist_info_qo_e_meas;
  qo_erv_qo_e_report_paths_s qo_erv_qo_e_report_paths;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEAppLayerMeasConfigInfo ::= SEQUENCE
struct ue_app_layer_meas_cfg_info_s {
  bool                                         qoe_meas_cfg_app_layer_id_present         = false;
  bool                                         qoe_meas_status_present                   = false;
  bool                                         mdt_align_info_present                    = false;
  bool                                         meas_collection_entity_ip_address_present = false;
  bool                                         area_scope_of_qmc_present                 = false;
  bool                                         available_rv_qo_e_metrics_present         = false;
  bool                                         ie_ext_present                            = false;
  fixed_octstring<6, true>                     qoe_ref;
  uint8_t                                      qoe_meas_cfg_app_layer_id = 0;
  service_type_e                               service_type;
  qoe_meas_status_e                            qoe_meas_status;
  bounded_octstring<1, 8000, true>             container_app_layer_meas_cfg;
  mdt_align_info_c                             mdt_align_info;
  bounded_bitstring<1, 160, true, true>        meas_collection_entity_ip_address;
  area_scope_of_qmc_c                          area_scope_of_qmc;
  s_nssai_list_qo_e_l                          s_nssai_list_qo_e;
  available_rv_qo_e_metrics_s                  available_rv_qo_e_metrics;
  ue_app_layer_meas_cfg_info_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEAppLayerMeasInfo-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_app_layer_meas_info_item_ext_ies_o = protocol_ext_empty_o;

using ue_app_layer_meas_info_item_ext_ies_container = protocol_ext_container_empty_l;

// UEAppLayerMeasInfo-Item ::= SEQUENCE
struct ue_app_layer_meas_info_item_s {
  bool                                          ie_exts_present = false;
  ue_app_layer_meas_cfg_info_s                  ue_app_layer_meas_cfg_info;
  ue_app_layer_meas_info_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEAppLayerMeasInfoList ::= SEQUENCE (SIZE (1..16)) OF UEAppLayerMeasInfo-Item
using ue_app_layer_meas_info_list_l = dyn_array<ue_app_layer_meas_info_item_s>;

// QMCConfigInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using q_mcc_onfig_info_ext_ies_o = protocol_ext_empty_o;

using q_mcc_onfig_info_ext_ies_container = protocol_ext_container_empty_l;

// QMCConfigInfo ::= SEQUENCE
struct q_mcc_onfig_info_s {
  bool                               ie_exts_present = false;
  ue_app_layer_meas_info_list_l      ue_app_layer_meas_info_list;
  q_mcc_onfig_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SN-to-MN-QMCCoordRequestList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_to_mn_q_mcc_oord_request_list_item_ext_ies_o = protocol_ext_empty_o;

using sn_to_mn_q_mcc_oord_request_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SN-to-MN-QMCCoordRequestList-Item ::= SEQUENCE
struct sn_to_mn_q_mcc_oord_request_list_item_s {
  struct qo_e_report_path_request_opts {
    enum options { srb4, srb5, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using qo_e_report_path_request_e_ = enumerated<qo_e_report_path_request_opts, true>;
  struct rv_qo_e_report_path_request_opts {
    enum options { srb4, srb5, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using rv_qo_e_report_path_request_e_ = enumerated<rv_qo_e_report_path_request_opts, true>;
  struct further_rv_qo_e_interest_inquiry_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using further_rv_qo_e_interest_inquiry_e_ = enumerated<further_rv_qo_e_interest_inquiry_opts, true>;
  struct further_rv_qo_e_report_path_inquiry_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using further_rv_qo_e_report_path_inquiry_e_ = enumerated<further_rv_qo_e_report_path_inquiry_opts, true>;
  struct cfg_release_ind_opts {
    enum options { rvqoe, qoe_and_rvqoe, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using cfg_release_ind_e_ = enumerated<cfg_release_ind_opts, true>;

  // member variables
  bool                                                    meas_collection_entity_ip_address_present   = false;
  bool                                                    qo_e_report_path_request_present            = false;
  bool                                                    rv_qo_e_report_path_request_present         = false;
  bool                                                    further_rv_qo_e_interest_inquiry_present    = false;
  bool                                                    further_rv_qo_e_report_path_inquiry_present = false;
  bool                                                    current_rv_qo_e_cfg_present                 = false;
  bool                                                    available_rv_qo_e_metrics_present           = false;
  bool                                                    cfg_release_ind_present                     = false;
  bool                                                    ie_exts_present                             = false;
  fixed_octstring<6, true>                                qoe_ref;
  bounded_bitstring<1, 160, true, true>                   meas_collection_entity_ip_address;
  qo_e_report_path_request_e_                             qo_e_report_path_request;
  rv_qo_e_report_path_request_e_                          rv_qo_e_report_path_request;
  further_rv_qo_e_interest_inquiry_e_                     further_rv_qo_e_interest_inquiry;
  further_rv_qo_e_report_path_inquiry_e_                  further_rv_qo_e_report_path_inquiry;
  rv_qo_e_cfg_s                                           current_rv_qo_e_cfg;
  available_rv_qo_e_metrics_s                             available_rv_qo_e_metrics;
  cfg_release_ind_e_                                      cfg_release_ind;
  sn_to_mn_q_mcc_oord_request_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SN-to-MN-QMCCoordRequestList ::= SEQUENCE (SIZE (1..16)) OF SN-to-MN-QMCCoordRequestList-Item
using sn_to_mn_q_mcc_oord_request_list_l = dyn_array<sn_to_mn_q_mcc_oord_request_list_item_s>;

// QMCCoordinationRequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using q_mcc_oordination_request_ext_ies_o = protocol_ext_empty_o;

using q_mcc_oordination_request_ext_ies_container = protocol_ext_container_empty_l;

// QMCCoordinationRequest ::= SEQUENCE
struct q_mcc_oordination_request_s {
  bool                                        ie_exts_present = false;
  mn_to_sn_q_mcc_oord_request_list_l          mn_to_sn_q_mcc_oord_request_list;
  sn_to_mn_q_mcc_oord_request_list_l          sn_to_mn_q_mcc_oord_request_list;
  q_mcc_oordination_request_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SN-to-MN-QMCCoordResponseList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sn_to_mn_q_mcc_oord_resp_list_item_ext_ies_o = protocol_ext_empty_o;

using sn_to_mn_q_mcc_oord_resp_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SN-to-MN-QMCCoordResponseList-Item ::= SEQUENCE
struct sn_to_mn_q_mcc_oord_resp_list_item_s {
  struct qo_e_report_path_resp_opts {
    enum options { accepted, rejected, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using qo_e_report_path_resp_e_ = enumerated<qo_e_report_path_resp_opts, true>;
  struct rv_qo_e_report_path_resp_opts {
    enum options { accepted, rejected, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using rv_qo_e_report_path_resp_e_ = enumerated<rv_qo_e_report_path_resp_opts, true>;
  struct further_rv_qo_e_interest_resp_opts {
    enum options { interested, not_interested, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using further_rv_qo_e_interest_resp_e_ = enumerated<further_rv_qo_e_interest_resp_opts, true>;
  struct further_rv_qo_e_report_path_resp_opts {
    enum options { srb4, srb5, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using further_rv_qo_e_report_path_resp_e_ = enumerated<further_rv_qo_e_report_path_resp_opts, true>;

  // member variables
  bool                                                 qo_e_report_path_resp_present            = false;
  bool                                                 rv_qo_e_report_path_resp_present         = false;
  bool                                                 further_rv_qo_e_interest_resp_present    = false;
  bool                                                 further_rv_qo_e_report_path_resp_present = false;
  bool                                                 preferred_rv_qo_e_cfg_present            = false;
  bool                                                 ie_exts_present                          = false;
  fixed_octstring<6, true>                             qoe_ref;
  qo_e_report_path_resp_e_                             qo_e_report_path_resp;
  rv_qo_e_report_path_resp_e_                          rv_qo_e_report_path_resp;
  further_rv_qo_e_interest_resp_e_                     further_rv_qo_e_interest_resp;
  further_rv_qo_e_report_path_resp_e_                  further_rv_qo_e_report_path_resp;
  rv_qo_e_cfg_s                                        preferred_rv_qo_e_cfg;
  sn_to_mn_q_mcc_oord_resp_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SN-to-MN-QMCCoordResponseList ::= SEQUENCE (SIZE (1..16)) OF SN-to-MN-QMCCoordResponseList-Item
using sn_to_mn_q_mcc_oord_resp_list_l = dyn_array<sn_to_mn_q_mcc_oord_resp_list_item_s>;

// QMCCoordinationResponse-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using q_mcc_oordination_resp_ext_ies_o = protocol_ext_empty_o;

using q_mcc_oordination_resp_ext_ies_container = protocol_ext_container_empty_l;

// QMCCoordinationResponse ::= SEQUENCE
struct q_mcc_oordination_resp_s {
  bool                                     ie_exts_present = false;
  mn_to_sn_q_mcc_oord_resp_list_l          mn_to_sn_q_mcc_oord_resp_list;
  sn_to_mn_q_mcc_oord_resp_list_l          sn_to_mn_q_mcc_oord_resp_list;
  q_mcc_oordination_resp_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowNotificationControlIndicationInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct qos_flow_notif_ctrl_ind_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct qos_flow_notif_ctrl_ind_info_ext_ies_container {
  bool    current_qos_para_set_idx_present = false;
  uint8_t current_qos_para_set_idx;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowNotify-Item ::= SEQUENCE
struct qos_flow_notify_item_s {
  struct notif_info_opts {
    enum options { fulfilled, not_fulfilled, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using notif_info_e_ = enumerated<notif_info_opts, true>;

  // member variables
  bool                                           ie_exts_present = false;
  uint8_t                                        qos_flow_id     = 0;
  notif_info_e_                                  notif_info;
  qos_flow_notif_ctrl_ind_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// QoSFlowNotificationControlIndicationInfo ::= SEQUENCE (SIZE (1..64)) OF QoSFlowNotify-Item
using qos_flow_notif_ctrl_ind_info_l = dyn_array<qos_flow_notify_item_s>;

// RANAreaID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ran_area_id_ext_ies_o = protocol_ext_empty_o;

using ran_area_id_ext_ies_container = protocol_ext_container_empty_l;

// RANAreaID ::= SEQUENCE
struct ran_area_id_s {
  bool                          ranac_present   = false;
  bool                          ie_exts_present = false;
  fixed_octstring<3, true>      tac;
  uint16_t                      ranac = 0;
  ran_area_id_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANAreaID-List ::= SEQUENCE (SIZE (1..16)) OF RANAreaID
using ran_area_id_list_l = dyn_array<ran_area_id_s>;

// RANPagingAreaChoice-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ran_paging_area_choice_ext_ies_o = protocol_ies_empty_o;

// RANPagingAreaChoice ::= CHOICE
struct ran_paging_area_choice_c {
  struct types_opts {
    enum options { cell_list, ran_area_id_list, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ran_paging_area_choice_c() = default;
  ran_paging_area_choice_c(const ran_paging_area_choice_c& other);
  ran_paging_area_choice_c& operator=(const ran_paging_area_choice_c& other);
  ~ran_paging_area_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ng_ran_cell_id_listin_ran_paging_area_l& cell_list()
  {
    assert_choice_type(types::cell_list, type_, "RANPagingAreaChoice");
    return c.get<ng_ran_cell_id_listin_ran_paging_area_l>();
  }
  ran_area_id_list_l& ran_area_id_list()
  {
    assert_choice_type(types::ran_area_id_list, type_, "RANPagingAreaChoice");
    return c.get<ran_area_id_list_l>();
  }
  protocol_ie_single_container_s<ran_paging_area_choice_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "RANPagingAreaChoice");
    return c.get<protocol_ie_single_container_s<ran_paging_area_choice_ext_ies_o>>();
  }
  const ng_ran_cell_id_listin_ran_paging_area_l& cell_list() const
  {
    assert_choice_type(types::cell_list, type_, "RANPagingAreaChoice");
    return c.get<ng_ran_cell_id_listin_ran_paging_area_l>();
  }
  const ran_area_id_list_l& ran_area_id_list() const
  {
    assert_choice_type(types::ran_area_id_list, type_, "RANPagingAreaChoice");
    return c.get<ran_area_id_list_l>();
  }
  const protocol_ie_single_container_s<ran_paging_area_choice_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "RANPagingAreaChoice");
    return c.get<protocol_ie_single_container_s<ran_paging_area_choice_ext_ies_o>>();
  }
  ng_ran_cell_id_listin_ran_paging_area_l&                          set_cell_list();
  ran_area_id_list_l&                                               set_ran_area_id_list();
  protocol_ie_single_container_s<ran_paging_area_choice_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<ng_ran_cell_id_listin_ran_paging_area_l,
                  protocol_ie_single_container_s<ran_paging_area_choice_ext_ies_o>,
                  ran_area_id_list_l>
      c;

  void destroy_();
};

// RANPagingArea-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ran_paging_area_ext_ies_o = protocol_ext_empty_o;

using ran_paging_area_ext_ies_container = protocol_ext_container_empty_l;

// RANPagingArea ::= SEQUENCE
struct ran_paging_area_s {
  bool                              ie_exts_present = false;
  fixed_octstring<3, true>          plmn_id;
  ran_paging_area_choice_c          ran_paging_area_choice;
  ran_paging_area_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RANPagingFailure ::= ENUMERATED
struct ran_paging_fail_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ran_paging_fail_e = enumerated<ran_paging_fail_opts, true>;

// RAReportList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ra_report_list_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ra_report_list_item_ext_ies_container {
  bool                      ue_assistant_id_present       = false;
  bool                      pscell_list_container_present = false;
  uint64_t                  ue_assistant_id;
  unbounded_octstring<true> pscell_list_container;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RAReportList-Item ::= SEQUENCE
struct ra_report_list_item_s {
  bool                                  ie_exts_present = false;
  unbounded_octstring<true>             ra_report;
  ra_report_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RAReport ::= SEQUENCE (SIZE (1..64)) OF RAReportList-Item
using ra_report_l = dyn_array<ra_report_list_item_s>;

// RRCConfigIndication ::= ENUMERATED
struct rrc_cfg_ind_opts {
  enum options { full_cfg, delta_cfg, /*...*/ nulltype } value;

  const char* to_string() const;
};
using rrc_cfg_ind_e = enumerated<rrc_cfg_ind_opts, true>;

// RRCResumeCause ::= ENUMERATED
struct rrc_resume_cause_opts {
  enum options { rna_upd, /*...*/ nulltype } value;

  const char* to_string() const;
};
using rrc_resume_cause_e = enumerated<rrc_resume_cause_opts, true>;

// RSPPFlowBitRates-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// RSPPQoSFlowItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// RSPP-transport-QoS-parameters-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// RaReportIndicationList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ra_report_ind_list_item_ext_ies_o = protocol_ext_empty_o;

using ra_report_ind_list_item_ext_ies_container = protocol_ext_container_empty_l;

// RaReportIndicationList-Item ::= SEQUENCE
struct ra_report_ind_list_item_s {
  bool                                      ie_exts_present          = false;
  uint64_t                                  m_ng_ran_node_ue_xnap_id = 0;
  ra_report_ind_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RaReportIndicationList ::= SEQUENCE (SIZE (1..64)) OF RaReportIndicationList-Item
using ra_report_ind_list_l = dyn_array<ra_report_ind_list_item_s>;

// RegistrationRequest ::= ENUMERATED
struct regist_request_opts {
  enum options { start, stop, add, /*...*/ nulltype } value;

  const char* to_string() const;
};
using regist_request_e = enumerated<regist_request_opts, true>;

// RegistrationRequestForDataCollection ::= ENUMERATED
struct regist_request_for_data_collection_opts {
  enum options { start, stop, /*...*/ nulltype } value;

  const char* to_string() const;
};
using regist_request_for_data_collection_e = enumerated<regist_request_for_data_collection_opts, true>;

// ReportingPeriodicity ::= ENUMERATED
struct report_periodicity_opts {
  enum options {
    half_thousand_ms,
    one_thousand_ms,
    two_thousand_ms,
    five_thousand_ms,
    ten_thousand_ms,
    /*...*/ nulltype
  } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using report_periodicity_e = enumerated<report_periodicity_opts, true>;

// ReportingPeriodicityForDataCollection ::= ENUMERATED
struct report_periodicity_for_data_collection_opts {
  enum options {
    half_thousand_ms,
    one_thousand_ms,
    two_thousand_ms,
    five_thousand_ms,
    ten_thousand_ms,
    /*...*/ nulltype
  } value;
  typedef float number_type;

  const char* to_string() const;
  float       to_number() const;
  const char* to_number_string() const;
};
using report_periodicity_for_data_collection_e = enumerated<report_periodicity_for_data_collection_opts, true>;

// ResetRequestPartialReleaseItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using reset_request_partial_release_item_ext_ies_o = protocol_ext_empty_o;

using reset_request_partial_release_item_ext_ies_container = protocol_ext_container_empty_l;

// ResetRequestPartialReleaseItem ::= SEQUENCE
struct reset_request_partial_release_item_s {
  bool                                                 ng_ran_node1_ue_xnap_id_present = false;
  bool                                                 ng_ran_node2_ue_xnap_id_present = false;
  bool                                                 ie_exts_present                 = false;
  uint64_t                                             ng_ran_node1_ue_xnap_id         = 0;
  uint64_t                                             ng_ran_node2_ue_xnap_id         = 0;
  reset_request_partial_release_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetRequestPartialReleaseList ::= SEQUENCE (SIZE (1..8192)) OF ResetRequestPartialReleaseItem
using reset_request_partial_release_list_l = dyn_array<reset_request_partial_release_item_s>;

// ResetRequestTypeInfo-Full-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using reset_request_type_info_full_ext_ies_o = protocol_ext_empty_o;

using reset_request_type_info_full_ext_ies_container = protocol_ext_container_empty_l;

// ResetRequestTypeInfo-Full ::= SEQUENCE
struct reset_request_type_info_full_s {
  bool                                           ie_ext_present = false;
  reset_request_type_info_full_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetRequestTypeInfo-Partial-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using reset_request_type_info_partial_ext_ies_o = protocol_ext_empty_o;

using reset_request_type_info_partial_ext_ies_container = protocol_ext_container_empty_l;

// ResetRequestTypeInfo-Partial ::= SEQUENCE
struct reset_request_type_info_partial_s {
  bool                                              ie_ext_present = false;
  reset_request_partial_release_list_l              ue_contexts_to_be_released_list;
  reset_request_type_info_partial_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetRequestTypeInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using reset_request_type_info_ext_ies_o = protocol_ies_empty_o;

// ResetRequestTypeInfo ::= CHOICE
struct reset_request_type_info_c {
  struct types_opts {
    enum options { full_reset, partial_reset, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  reset_request_type_info_c() = default;
  reset_request_type_info_c(const reset_request_type_info_c& other);
  reset_request_type_info_c& operator=(const reset_request_type_info_c& other);
  ~reset_request_type_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  reset_request_type_info_full_s& full_reset()
  {
    assert_choice_type(types::full_reset, type_, "ResetRequestTypeInfo");
    return c.get<reset_request_type_info_full_s>();
  }
  reset_request_type_info_partial_s& partial_reset()
  {
    assert_choice_type(types::partial_reset, type_, "ResetRequestTypeInfo");
    return c.get<reset_request_type_info_partial_s>();
  }
  protocol_ie_single_container_s<reset_request_type_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ResetRequestTypeInfo");
    return c.get<protocol_ie_single_container_s<reset_request_type_info_ext_ies_o>>();
  }
  const reset_request_type_info_full_s& full_reset() const
  {
    assert_choice_type(types::full_reset, type_, "ResetRequestTypeInfo");
    return c.get<reset_request_type_info_full_s>();
  }
  const reset_request_type_info_partial_s& partial_reset() const
  {
    assert_choice_type(types::partial_reset, type_, "ResetRequestTypeInfo");
    return c.get<reset_request_type_info_partial_s>();
  }
  const protocol_ie_single_container_s<reset_request_type_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ResetRequestTypeInfo");
    return c.get<protocol_ie_single_container_s<reset_request_type_info_ext_ies_o>>();
  }
  reset_request_type_info_full_s&                                    set_full_reset();
  reset_request_type_info_partial_s&                                 set_partial_reset();
  protocol_ie_single_container_s<reset_request_type_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<reset_request_type_info_ext_ies_o>,
                  reset_request_type_info_full_s,
                  reset_request_type_info_partial_s>
      c;

  void destroy_();
};

// ResetResponsePartialReleaseItem-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using reset_resp_partial_release_item_ext_ies_o = protocol_ext_empty_o;

using reset_resp_partial_release_item_ext_ies_container = protocol_ext_container_empty_l;

// ResetResponsePartialReleaseItem ::= SEQUENCE
struct reset_resp_partial_release_item_s {
  bool                                              ng_ran_node1_ue_xnap_id_present = false;
  bool                                              ng_ran_node2_ue_xnap_id_present = false;
  bool                                              ie_exts_present                 = false;
  uint64_t                                          ng_ran_node1_ue_xnap_id         = 0;
  uint64_t                                          ng_ran_node2_ue_xnap_id         = 0;
  reset_resp_partial_release_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetResponsePartialReleaseList ::= SEQUENCE (SIZE (1..8192)) OF ResetResponsePartialReleaseItem
using reset_resp_partial_release_list_l = dyn_array<reset_resp_partial_release_item_s>;

// ResetResponseTypeInfo-Full-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using reset_resp_type_info_full_ext_ies_o = protocol_ext_empty_o;

using reset_resp_type_info_full_ext_ies_container = protocol_ext_container_empty_l;

// ResetResponseTypeInfo-Full ::= SEQUENCE
struct reset_resp_type_info_full_s {
  bool                                        ie_ext_present = false;
  reset_resp_type_info_full_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetResponseTypeInfo-Partial-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using reset_resp_type_info_partial_ext_ies_o = protocol_ext_empty_o;

using reset_resp_type_info_partial_ext_ies_container = protocol_ext_container_empty_l;

// ResetResponseTypeInfo-Partial ::= SEQUENCE
struct reset_resp_type_info_partial_s {
  bool                                           ie_ext_present = false;
  reset_resp_partial_release_list_l              ue_contexts_admitted_to_be_released_list;
  reset_resp_type_info_partial_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ResetResponseTypeInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using reset_resp_type_info_ext_ies_o = protocol_ies_empty_o;

// ResetResponseTypeInfo ::= CHOICE
struct reset_resp_type_info_c {
  struct types_opts {
    enum options { full_reset, partial_reset, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  reset_resp_type_info_c() = default;
  reset_resp_type_info_c(const reset_resp_type_info_c& other);
  reset_resp_type_info_c& operator=(const reset_resp_type_info_c& other);
  ~reset_resp_type_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  reset_resp_type_info_full_s& full_reset()
  {
    assert_choice_type(types::full_reset, type_, "ResetResponseTypeInfo");
    return c.get<reset_resp_type_info_full_s>();
  }
  reset_resp_type_info_partial_s& partial_reset()
  {
    assert_choice_type(types::partial_reset, type_, "ResetResponseTypeInfo");
    return c.get<reset_resp_type_info_partial_s>();
  }
  protocol_ie_single_container_s<reset_resp_type_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ResetResponseTypeInfo");
    return c.get<protocol_ie_single_container_s<reset_resp_type_info_ext_ies_o>>();
  }
  const reset_resp_type_info_full_s& full_reset() const
  {
    assert_choice_type(types::full_reset, type_, "ResetResponseTypeInfo");
    return c.get<reset_resp_type_info_full_s>();
  }
  const reset_resp_type_info_partial_s& partial_reset() const
  {
    assert_choice_type(types::partial_reset, type_, "ResetResponseTypeInfo");
    return c.get<reset_resp_type_info_partial_s>();
  }
  const protocol_ie_single_container_s<reset_resp_type_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ResetResponseTypeInfo");
    return c.get<protocol_ie_single_container_s<reset_resp_type_info_ext_ies_o>>();
  }
  reset_resp_type_info_full_s&                                    set_full_reset();
  reset_resp_type_info_partial_s&                                 set_partial_reset();
  protocol_ie_single_container_s<reset_resp_type_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<reset_resp_type_info_ext_ies_o>,
                  reset_resp_type_info_full_s,
                  reset_resp_type_info_partial_s>
      c;

  void destroy_();
};

// S-NG-RANnode-Addition-Trigger-Ind ::= ENUMERATED
struct s_ng_ran_node_addition_trigger_ind_opts {
  enum options { sn_change, inter_mn_ho, intra_mn_ho, /*...*/ nulltype } value;

  const char* to_string() const;
};
using s_ng_ran_node_addition_trigger_ind_e = enumerated<s_ng_ran_node_addition_trigger_ind_opts, true>;

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

// SCGConfigurationQuery ::= ENUMERATED
struct scg_cfg_query_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using scg_cfg_query_e = enumerated<scg_cfg_query_opts, true>;

// SCGIndicator ::= ENUMERATED
struct scg_ind_opts {
  enum options { released, /*...*/ nulltype } value;

  const char* to_string() const;
};
using scg_ind_e = enumerated<scg_ind_opts, true>;

// SCGUEHistoryInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using scg_ue_history_info_ext_ies_o = protocol_ext_empty_o;

using scg_ue_history_info_ext_ies_container = protocol_ext_container_empty_l;

// SCGUEHistoryInformation ::= SEQUENCE
struct scg_ue_history_info_s {
  bool                                  ie_exts_present = false;
  last_visited_pscell_list_l            last_visited_pscell_list;
  scg_ue_history_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SCGreconfigNotification ::= ENUMERATED
struct sc_grecfg_notif_opts {
  enum options { executed, /*...*/ executed_deleted, deleted, nulltype } value;

  const char* to_string() const;
};
using sc_grecfg_notif_e = enumerated<sc_grecfg_notif_opts, true, 2>;

// SDT-DRBsToBeSetupList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sdt_drbs_to_be_setup_list_item_ext_ies_o = protocol_ext_empty_o;

using sdt_drbs_to_be_setup_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SDT-DRBsToBeSetupList-Item ::= SEQUENCE
struct sdt_drbs_to_be_setup_list_item_s {
  bool                                             ie_exts_present = false;
  uint8_t                                          drb_id          = 1;
  up_transport_layer_info_c                        ul_tnl_info;
  unbounded_octstring<true>                        drb_rlc_bearer_cfg;
  qos_flow_level_qos_params_s                      drb_qos;
  rlc_mode_e                                       rlc_mode;
  s_nssai_s                                        s_nssai;
  pdcp_sn_len_s                                    pdcp_sn_len;
  flows_mapped_to_drb_list_l                       flows_mapped_to_drb_list;
  sdt_drbs_to_be_setup_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SDT-DRBsToBeSetupList ::= SEQUENCE (SIZE (1..32)) OF SDT-DRBsToBeSetupList-Item
using sdt_drbs_to_be_setup_list_l = dyn_array<sdt_drbs_to_be_setup_list_item_s>;

// SDT-SRBsToBeSetupList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sdt_srbs_to_be_setup_list_item_ext_ies_o = protocol_ext_empty_o;

using sdt_srbs_to_be_setup_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SDT-SRBsToBeSetupList-Item ::= SEQUENCE
struct sdt_srbs_to_be_setup_list_item_s {
  bool                                             ie_exts_present = false;
  uint8_t                                          srb_id          = 0;
  unbounded_octstring<true>                        srb_rlc_bearer_cfg;
  sdt_srbs_to_be_setup_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SDT-SRBsToBeSetupList ::= SEQUENCE (SIZE (1..5)) OF SDT-SRBsToBeSetupList-Item
using sdt_srbs_to_be_setup_list_l = dyn_array<sdt_srbs_to_be_setup_list_item_s>;

// SDT-Termination-Request ::= ENUMERATED
struct sdt_termination_request_opts {
  enum options { radio_link_problem, normal, /*...*/ large_sdt_volume_from_bsr, nulltype } value;

  const char* to_string() const;
};
using sdt_termination_request_e = enumerated<sdt_termination_request_opts, true, 1>;

// SDTAssistantInfo ::= ENUMERATED
struct sdt_assistant_info_opts {
  enum options { single_packet, multiple_packets, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sdt_assistant_info_e = enumerated<sdt_assistant_info_opts, true>;

// SDTDataForwardingDRBList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sdt_data_forwarding_drb_list_item_ext_ies_o = protocol_ext_empty_o;

using sdt_data_forwarding_drb_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SDTDataForwardingDRBList-Item ::= SEQUENCE
struct sdt_data_forwarding_drb_list_item_s {
  bool                                                dl_tnl_info_present = false;
  bool                                                ie_exts_present     = false;
  uint8_t                                             drb_id              = 1;
  up_transport_layer_info_c                           dl_tnl_info;
  sdt_data_forwarding_drb_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SDTDataForwardingDRBList ::= SEQUENCE (SIZE (1..32)) OF SDTDataForwardingDRBList-Item
using sdt_data_forwarding_drb_list_l = dyn_array<sdt_data_forwarding_drb_list_item_s>;

// SDTIndicator ::= ENUMERATED
struct sdt_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sdt_ind_e = enumerated<sdt_ind_opts, true>;

// SDTPartialUEContextInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sdt_partial_ue_context_info_ext_ies_o = protocol_ext_empty_o;

using sdt_partial_ue_context_info_ext_ies_container = protocol_ext_container_empty_l;

// SDTPartialUEContextInfo ::= SEQUENCE
struct sdt_partial_ue_context_info_s {
  bool                                          ie_exts_present = false;
  sdt_drbs_to_be_setup_list_l                   drbs_to_be_setup;
  sdt_srbs_to_be_setup_list_l                   srbs_to_be_setup;
  sdt_partial_ue_context_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SDTSupportRequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sdt_support_request_ext_ies_o = protocol_ext_empty_o;

using sdt_support_request_ext_ies_container = protocol_ext_container_empty_l;

// SDTSupportRequest ::= SEQUENCE
struct sdt_support_request_s {
  bool                                  sdt_assistant_info_present = false;
  bool                                  ie_exts_present            = false;
  sdt_ind_e                             sdtind;
  sdt_assistant_info_e                  sdt_assistant_info;
  sdt_support_request_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SFN-Offset-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
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

// SLPositioning-Ranging-Authorized ::= ENUMERATED
struct sl_positioning_ranging_authorized_opts {
  enum options { authorized, not_authorized, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sl_positioning_ranging_authorized_e = enumerated<sl_positioning_ranging_authorized_opts, true>;

// SLPositioning-Ranging-Services-Info-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using sl_positioning_ranging_services_info_ext_ies_o = protocol_ext_empty_o;

using sl_positioning_ranging_services_info_ext_ies_container = protocol_ext_container_empty_l;

// SLPositioning-Ranging-Services-Info ::= SEQUENCE
struct sl_positioning_ranging_services_info_s {
  bool                                                   rspp_transport_qos_params_present = false;
  bool                                                   ie_exts_present                   = false;
  sl_positioning_ranging_authorized_e                    sl_positioning_ranging_authorized;
  rspp_transport_qos_params_s                            rspp_transport_qos_params;
  sl_positioning_ranging_services_info_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SNTriggered ::= ENUMERATED
struct sn_triggered_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using sn_triggered_e = enumerated<sn_triggered_opts, true>;

// SPRAvailability ::= ENUMERATED
struct spr_availability_opts {
  enum options { spr_available, /*...*/ nulltype } value;

  const char* to_string() const;
};
using spr_availability_e = enumerated<spr_availability_opts, true>;

// SRSPositioningConfigOrActivationRequest ::= ENUMERATED
struct srs_positioning_cfg_or_activation_request_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using srs_positioning_cfg_or_activation_request_e = enumerated<srs_positioning_cfg_or_activation_request_opts, true>;

// SSB-PositionsInBurst-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
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

// SSBOffsetInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ssb_offset_info_ext_ies_o = protocol_ext_empty_o;

using ssb_offset_info_ext_ies_container = protocol_ext_container_empty_l;

// SSBOffsetInformation ::= SEQUENCE
struct ssb_offset_info_s {
  bool                              ie_exts_present = false;
  uint8_t                           ssb_idx         = 0;
  mob_params_info_s                 ssb_trigger_offset;
  ssb_offset_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBOffsets-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ssb_offsets_item_ext_ies_o = protocol_ext_empty_o;

using ssb_offsets_item_ext_ies_container = protocol_ext_container_empty_l;

// SSBOffsets-Item ::= SEQUENCE
struct ssb_offsets_item_s {
  bool                               ng_ran_node1_ssb_offsets_present = false;
  bool                               ie_exts_present                  = false;
  ssb_offset_info_s                  ng_ran_node1_ssb_offsets;
  ssb_offset_info_s                  ng_ran_node2_proposed_ssb_offsets;
  ssb_offsets_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SSBOffsets-List ::= SEQUENCE (SIZE (1..64)) OF SSBOffsets-Item
using ssb_offsets_list_l = dyn_array<ssb_offsets_item_s>;

// SemipersistentPositioningInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using semipersistent_positioning_info_ext_ies_o = protocol_ext_empty_o;

using semipersistent_positioning_info_ext_ies_container = protocol_ext_container_empty_l;

// SemipersistentPositioningInformation ::= SEQUENCE
struct semipersistent_positioning_info_s {
  struct srs_tx_type_opts {
    enum options { activ, deactiv, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using srs_tx_type_e_ = enumerated<srs_tx_type_opts, true>;

  // member variables
  bool                                              ie_exts_present = false;
  srs_tx_type_e_                                    srs_tx_type;
  unbounded_octstring<true>                         srs_res_set_id;
  unbounded_octstring<true>                         srs_spatial_relation;
  unbounded_octstring<true>                         spatial_relation_inforper_srs_res;
  semipersistent_positioning_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// Served-cells-ToModify-E-UTRA-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct served_cells_to_modify_e_utra_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// Served-cells-ToModify-NR-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using served_cells_to_modify_nr_item_ext_ies_o = protocol_ext_empty_o;

// ServedCellInformation-E-UTRA-perBPLMN-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using served_cell_info_e_utra_per_bplmn_ext_ies_o = protocol_ext_empty_o;

using served_cell_info_e_utra_per_bplmn_ext_ies_container = protocol_ext_container_empty_l;

// ServedCellInformation-E-UTRA-perBPLMN ::= SEQUENCE
struct served_cell_info_e_utra_per_bplmn_s {
  bool                                                ie_exts_present = false;
  fixed_octstring<3, true>                            plmn_id;
  served_cell_info_e_utra_per_bplmn_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellInformation-E-UTRA-FDDInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct served_cell_info_e_utra_fdd_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct served_cell_info_e_utra_fdd_info_ext_ies_container {
  bool                               offset_of_nbiot_ch_num_to_dl_earfcn_present = false;
  bool                               offset_of_nbiot_ch_num_to_ul_earfcn_present = false;
  offset_of_nbiot_ch_num_to_earfcn_e offset_of_nbiot_ch_num_to_dl_earfcn;
  offset_of_nbiot_ch_num_to_earfcn_e offset_of_nbiot_ch_num_to_ul_earfcn;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellInformation-E-UTRA-FDDInfo ::= SEQUENCE
struct served_cell_info_e_utra_fdd_info_s {
  bool                                               ie_exts_present = false;
  uint32_t                                           ul_earfcn       = 0;
  uint32_t                                           dl_earfcn       = 0;
  e_utra_tx_bw_e                                     ul_e_utra_tx_bw;
  e_utra_tx_bw_e                                     dl_e_utra_tx_bw;
  served_cell_info_e_utra_fdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SpecialSubframePatterns-E-UTRA ::= ENUMERATED
struct special_sf_patterns_e_utra_opts {
  enum options { ssp0, ssp1, ssp2, ssp3, ssp4, ssp5, ssp6, ssp7, ssp8, ssp9, ssp10, /*...*/ nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using special_sf_patterns_e_utra_e = enumerated<special_sf_patterns_e_utra_opts, true>;

// SpecialSubframeInfo-E-UTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using special_sf_info_e_utra_ext_ies_o = protocol_ext_empty_o;

using special_sf_info_e_utra_ext_ies_container = protocol_ext_container_empty_l;

// SpecialSubframeInfo-E-UTRA ::= SEQUENCE
struct special_sf_info_e_utra_s {
  bool                                     ie_exts_present = false;
  special_sf_patterns_e_utra_e             special_sf_pattern;
  cp_e_utra_dl_e                           cp_dl;
  cp_e_utra_ul_e                           cp_ul;
  special_sf_info_e_utra_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellInformation-E-UTRA-TDDInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct served_cell_info_e_utra_tdd_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct served_cell_info_e_utra_tdd_info_ext_ies_container {
  bool                               offset_of_nbiot_ch_num_to_dl_earfcn_present = false;
  bool                               nb_iot_ul_dl_align_offset_present           = false;
  offset_of_nbiot_ch_num_to_earfcn_e offset_of_nbiot_ch_num_to_dl_earfcn;
  nb_iot_ul_dl_align_offset_e        nb_iot_ul_dl_align_offset;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellInformation-E-UTRA-TDDInfo ::= SEQUENCE
struct served_cell_info_e_utra_tdd_info_s {
  struct sf_assignmnet_opts {
    enum options { sa0, sa1, sa2, sa3, sa4, sa5, sa6, /*...*/ nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sf_assignmnet_e_ = enumerated<sf_assignmnet_opts, true>;

  // member variables
  bool                                               ie_exts_present = false;
  uint32_t                                           earfcn          = 0;
  e_utra_tx_bw_e                                     e_utra_tx_bw;
  sf_assignmnet_e_                                   sf_assignmnet;
  special_sf_info_e_utra_s                           special_sf_info;
  served_cell_info_e_utra_tdd_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellInformation-E-UTRA-ModeInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using served_cell_info_e_utra_mode_info_ext_ies_o = protocol_ies_empty_o;

// ServedCellInformation-E-UTRA-ModeInfo ::= CHOICE
struct served_cell_info_e_utra_mode_info_c {
  struct types_opts {
    enum options { fdd, tdd, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  served_cell_info_e_utra_mode_info_c() = default;
  served_cell_info_e_utra_mode_info_c(const served_cell_info_e_utra_mode_info_c& other);
  served_cell_info_e_utra_mode_info_c& operator=(const served_cell_info_e_utra_mode_info_c& other);
  ~served_cell_info_e_utra_mode_info_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  served_cell_info_e_utra_fdd_info_s& fdd()
  {
    assert_choice_type(types::fdd, type_, "ServedCellInformation-E-UTRA-ModeInfo");
    return c.get<served_cell_info_e_utra_fdd_info_s>();
  }
  served_cell_info_e_utra_tdd_info_s& tdd()
  {
    assert_choice_type(types::tdd, type_, "ServedCellInformation-E-UTRA-ModeInfo");
    return c.get<served_cell_info_e_utra_tdd_info_s>();
  }
  protocol_ie_single_container_s<served_cell_info_e_utra_mode_info_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "ServedCellInformation-E-UTRA-ModeInfo");
    return c.get<protocol_ie_single_container_s<served_cell_info_e_utra_mode_info_ext_ies_o>>();
  }
  const served_cell_info_e_utra_fdd_info_s& fdd() const
  {
    assert_choice_type(types::fdd, type_, "ServedCellInformation-E-UTRA-ModeInfo");
    return c.get<served_cell_info_e_utra_fdd_info_s>();
  }
  const served_cell_info_e_utra_tdd_info_s& tdd() const
  {
    assert_choice_type(types::tdd, type_, "ServedCellInformation-E-UTRA-ModeInfo");
    return c.get<served_cell_info_e_utra_tdd_info_s>();
  }
  const protocol_ie_single_container_s<served_cell_info_e_utra_mode_info_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "ServedCellInformation-E-UTRA-ModeInfo");
    return c.get<protocol_ie_single_container_s<served_cell_info_e_utra_mode_info_ext_ies_o>>();
  }
  served_cell_info_e_utra_fdd_info_s&                                          set_fdd();
  served_cell_info_e_utra_tdd_info_s&                                          set_tdd();
  protocol_ie_single_container_s<served_cell_info_e_utra_mode_info_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<served_cell_info_e_utra_mode_info_ext_ies_o>,
                  served_cell_info_e_utra_fdd_info_s,
                  served_cell_info_e_utra_tdd_info_s>
      c;

  void destroy_();
};

// ServedCellInformation-E-UTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct served_cell_info_e_utra_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct served_cell_info_e_utra_ext_ies_container {
  bool                  bplmn_id_info_eutra_present = false;
  bool                  nprach_cfg_present          = false;
  bplmn_id_info_eutra_l bplmn_id_info_eutra;
  nprach_cfg_s          nprach_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellInformation-E-UTRA ::= SEQUENCE
struct served_cell_info_e_utra_s {
  using broadcast_plmns_l_ = dyn_array<served_cell_info_e_utra_per_bplmn_s>;
  struct freq_band_ind_prio_opts {
    enum options { not_broadcast, broadcast, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using freq_band_ind_prio_e_ = enumerated<freq_band_ind_prio_opts, true>;
  struct bw_reduced_si_opts {
    enum options { sched, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using bw_reduced_si_e_ = enumerated<bw_reduced_si_opts, true>;

  // member variables
  bool                                      ranac_present                    = false;
  bool                                      numof_ant_ports_present          = false;
  bool                                      prach_cfg_present                = false;
  bool                                      freq_band_ind_prio_present       = false;
  bool                                      bw_reduced_si_present            = false;
  bool                                      protected_e_utra_res_ind_present = false;
  bool                                      ie_exts_present                  = false;
  uint16_t                                  e_utra_pci                       = 0;
  e_utra_cgi_s                              e_utra_cgi;
  fixed_octstring<3, true>                  tac;
  uint16_t                                  ranac = 0;
  broadcast_plmns_l_                        broadcast_plmns;
  served_cell_info_e_utra_mode_info_c       e_utra_mode_info;
  nof_ant_ports_e_utra_e                    numof_ant_ports;
  e_utraprach_cfg_s                         prach_cfg;
  mbsfn_sf_info_e_utra_l                    mbsfn_sf_info;
  e_utra_multiband_info_list_l              multiband_info;
  freq_band_ind_prio_e_                     freq_band_ind_prio;
  bw_reduced_si_e_                          bw_reduced_si;
  protected_e_utra_res_ind_s                protected_e_utra_res_ind;
  served_cell_info_e_utra_ext_ies_container ie_exts;
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

// ServedCellInformation-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct served_cell_info_nr_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct served_cell_info_nr_ext_ies_container {
  bool                               bplmn_id_info_nr_present                 = false;
  bool                               cfg_tac_ind_present                      = false;
  bool                               ssb_positions_in_burst_present           = false;
  bool                               nr_cell_prach_cfg_present                = false;
  bool                               npn_broadcast_info_present               = false;
  bool                               csi_rs_tx_ind_present                    = false;
  bool                               sfn_offset_present                       = false;
  bool                               supported_mbs_fsa_id_list_present        = false;
  bool                               nr_u_ch_info_list_present                = false;
  bool                               add_meas_timing_cfg_list_present         = false;
  bool                               redcap_bcast_info_present                = false;
  bool                               eredcap_bcast_info_present               = false;
  bool                               mobile_iab_cell_present                  = false;
  bool                               xr_bcast_info_present                    = false;
  bool                               barr_exemptionfor_emer_call_info_present = false;
  bplmn_id_info_nr_l                 bplmn_id_info_nr;
  cfg_tac_ind_e                      cfg_tac_ind;
  ssb_positions_in_burst_c           ssb_positions_in_burst;
  unbounded_octstring<true>          nr_cell_prach_cfg;
  npn_broadcast_info_c               npn_broadcast_info;
  csi_rs_tx_ind_e                    csi_rs_tx_ind;
  sfn_offset_s                       sfn_offset;
  supported_mbs_fsa_id_list_l        supported_mbs_fsa_id_list;
  nr_u_ch_info_list_l                nr_u_ch_info_list;
  add_meas_timing_cfg_list_l         add_meas_timing_cfg_list;
  fixed_bitstring<8, false, true>    redcap_bcast_info;
  fixed_bitstring<8, false, true>    eredcap_bcast_info;
  mobile_iab_cell_e                  mobile_iab_cell;
  xr_bcast_info_e                    xr_bcast_info;
  barr_exemptionfor_emer_call_info_e barr_exemptionfor_emer_call_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellInformation-NR ::= SEQUENCE
struct served_cell_info_nr_s {
  bool                                  ranac_present   = false;
  bool                                  ie_exts_present = false;
  uint16_t                              nr_pci          = 0;
  nr_cgi_s                              cell_id;
  fixed_octstring<3, true>              tac;
  uint16_t                              ranac = 0;
  broadcast_plmns_l                     broadcast_plmn;
  nr_mode_info_c                        nr_mode_info;
  unbounded_octstring<true>             meas_timing_cfg;
  connect_support_s                     connect_support;
  served_cell_info_nr_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellSpecificInfoReq-NR-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using served_cell_specific_info_req_nr_item_ext_ies_o = protocol_ext_empty_o;

using served_cell_specific_info_req_nr_item_ext_ies_container = protocol_ext_container_empty_l;

// ServedCellSpecificInfoReq-NR-Item ::= SEQUENCE
struct served_cell_specific_info_req_nr_item_s {
  struct add_mtc_list_request_ind_opts {
    enum options { add_mtc_list_requested, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using add_mtc_list_request_ind_e_ = enumerated<add_mtc_list_request_ind_opts, true>;

  // member variables
  bool                                                    add_mtc_list_request_ind_present = false;
  bool                                                    ie_exts_present                  = false;
  nr_cgi_s                                                nr_cgi;
  add_mtc_list_request_ind_e_                             add_mtc_list_request_ind;
  served_cell_specific_info_req_nr_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellSpecificInfoReq-NR ::= SEQUENCE (SIZE (1..16384)) OF ServedCellSpecificInfoReq-NR-Item
using served_cell_specific_info_req_nr_l = dyn_array<served_cell_specific_info_req_nr_item_s>;

// ServedCells-E-UTRA-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct served_cells_e_utra_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct served_cells_e_utra_item_ext_ies_container {
  bool         sfn_offset_present = false;
  sfn_offset_s sfn_offset;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCells-E-UTRA-Item ::= SEQUENCE
struct served_cells_e_utra_item_s {
  bool                                       ie_exts_present = false;
  served_cell_info_e_utra_s                  served_cell_info_e_utra;
  neighbour_info_nr_l                        neighbour_info_nr;
  neighbour_info_e_utra_l                    neighbour_info_e_utra;
  served_cells_e_utra_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCells-E-UTRA ::= SEQUENCE (SIZE (1..16384)) OF ServedCells-E-UTRA-Item
using served_cells_e_utra_l = dyn_array<served_cells_e_utra_item_s>;

// ServedCells-NR-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct served_cells_nr_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct served_cells_nr_item_ext_ies_container {
  bool                               served_cell_specific_info_req_nr_present = false;
  served_cell_specific_info_req_nr_l served_cell_specific_info_req_nr;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCells-NR-Item ::= SEQUENCE
struct served_cells_nr_item_s {
  bool                                   ie_exts_present = false;
  served_cell_info_nr_s                  served_cell_info_nr;
  neighbour_info_nr_l                    neighbour_info_nr;
  neighbour_info_e_utra_l                neighbour_info_e_utra;
  served_cells_nr_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCells-NR ::= SEQUENCE (SIZE (1..16384)) OF ServedCells-NR-Item
using served_cells_nr_l = dyn_array<served_cells_nr_item_s>;

struct served_cells_to_modify_e_utra_item_ext_ies_container {
  bool         sfn_offset_present = false;
  sfn_offset_s sfn_offset;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCells-ToModify-E-UTRA-Item ::= SEQUENCE
struct served_cells_to_modify_e_utra_item_s {
  struct deactivation_ind_opts {
    enum options { deactiv, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using deactivation_ind_e_ = enumerated<deactivation_ind_opts, true>;

  // member variables
  bool                                                 deactivation_ind_present = false;
  bool                                                 ie_exts_present          = false;
  e_utra_cgi_s                                         old_ecgi;
  served_cell_info_e_utra_s                            served_cell_info_e_utra;
  neighbour_info_nr_l                                  neighbour_info_nr;
  neighbour_info_e_utra_l                              neighbour_info_e_utra;
  deactivation_ind_e_                                  deactivation_ind;
  served_cells_to_modify_e_utra_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCells-ToModify-E-UTRA ::= SEQUENCE (SIZE (1..16384)) OF ServedCells-ToModify-E-UTRA-Item
using served_cells_to_modify_e_utra_l = dyn_array<served_cells_to_modify_e_utra_item_s>;

using served_cells_to_modify_nr_item_ext_ies_container = protocol_ext_container_empty_l;

// ServedCells-ToModify-NR-Item ::= SEQUENCE
struct served_cells_to_modify_nr_item_s {
  struct deactivation_ind_opts {
    enum options { deactiv, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using deactivation_ind_e_ = enumerated<deactivation_ind_opts, true>;

  // member variables
  bool                                             deactivation_ind_present = false;
  bool                                             ie_exts_present          = false;
  nr_cgi_s                                         old_nr_cgi;
  served_cell_info_nr_s                            served_cell_info_nr;
  neighbour_info_nr_l                              neighbour_info_nr;
  neighbour_info_e_utra_l                          neighbour_info_e_utra;
  deactivation_ind_e_                              deactivation_ind;
  served_cells_to_modify_nr_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCells-ToModify-NR ::= SEQUENCE (SIZE (1..16384)) OF ServedCells-ToModify-NR-Item
using served_cells_to_modify_nr_l = dyn_array<served_cells_to_modify_nr_item_s>;

// ServedCellsToUpdate-E-UTRA-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using served_cells_to_upd_e_utra_ext_ies_o = protocol_ext_empty_o;

using served_cells_to_upd_e_utra_ext_ies_container = protocol_ext_container_empty_l;

// ServedCellsToUpdate-E-UTRA ::= SEQUENCE
struct served_cells_to_upd_e_utra_s {
  using served_cells_to_delete_e_utra_l_ = dyn_array<e_utra_cgi_s>;

  // member variables
  bool                                         ie_exts_present = false;
  served_cells_e_utra_l                        served_cells_to_add_e_utra;
  served_cells_to_modify_e_utra_l              served_cells_to_modify_e_utra;
  served_cells_to_delete_e_utra_l_             served_cells_to_delete_e_utra;
  served_cells_to_upd_e_utra_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ServedCellsToUpdate-NR-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using served_cells_to_upd_nr_ext_ies_o = protocol_ext_empty_o;

using served_cells_to_upd_nr_ext_ies_container = protocol_ext_container_empty_l;

// ServedCellsToUpdate-NR ::= SEQUENCE
struct served_cells_to_upd_nr_s {
  using served_cells_to_delete_nr_l_ = dyn_array<nr_cgi_s>;

  // member variables
  bool                                     ie_exts_present = false;
  served_cells_nr_l                        served_cells_to_add_nr;
  served_cells_to_modify_nr_l              served_cells_to_modify_nr;
  served_cells_to_delete_nr_l_             served_cells_to_delete_nr;
  served_cells_to_upd_nr_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SplitSRBsTypes ::= ENUMERATED
struct split_srbs_types_opts {
  enum options { srb1, srb2, srb1and2, /*...*/ nulltype } value;

  const char* to_string() const;
};
using split_srbs_types_e = enumerated<split_srbs_types_opts, true>;

// SuccessfulHOReportList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using successful_ho_report_list_item_ext_ies_o = protocol_ext_empty_o;

using successful_ho_report_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SuccessfulHOReportList-Item ::= SEQUENCE
struct successful_ho_report_list_item_s {
  bool                                             ie_exts_present = false;
  unbounded_octstring<true>                        successful_ho_report;
  successful_ho_report_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SuccessfulHOReportInformation ::= SEQUENCE (SIZE (1..64)) OF SuccessfulHOReportList-Item
using successful_ho_report_info_l = dyn_array<successful_ho_report_list_item_s>;

// SuccessfulPSCellChangeReportList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using successful_pscell_change_report_list_item_ext_ies_o = protocol_ext_empty_o;

using successful_pscell_change_report_list_item_ext_ies_container = protocol_ext_container_empty_l;

// SuccessfulPSCellChangeReportList-Item ::= SEQUENCE
struct successful_pscell_change_report_list_item_s {
  bool                                                        sn_mob_info_present = false;
  bool                                                        ie_exts_present     = false;
  unbounded_octstring<true>                                   successful_pscell_change_report;
  fixed_bitstring<32, false, true>                            sn_mob_info;
  successful_pscell_change_report_list_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SuccessfulPSCellChangeReportInformation ::= SEQUENCE (SIZE (1..64)) OF SuccessfulPSCellChangeReportList-Item
using successful_pscell_change_report_info_l = dyn_array<successful_pscell_change_report_list_item_s>;

// TAISupport-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tai_support_item_ext_ies_o = protocol_ext_empty_o;

using tai_support_item_ext_ies_container = protocol_ext_container_empty_l;

// TAISupport-Item ::= SEQUENCE
struct tai_support_item_s {
  using broadcast_plmns_l_ = dyn_array<broadcast_plmn_in_tai_support_item_s>;

  // member variables
  bool                               ie_exts_present = false;
  fixed_octstring<3, true>           tac;
  broadcast_plmns_l_                 broadcast_plmns;
  tai_support_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TAISupport-List ::= SEQUENCE (SIZE (1..256)) OF TAISupport-Item
using tai_support_list_l = dyn_array<tai_support_item_s>;

// TNLA-Failed-To-Setup-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tnl_a_failed_to_setup_item_ext_ies_o = protocol_ext_empty_o;

using tnl_a_failed_to_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// TNLA-Failed-To-Setup-Item ::= SEQUENCE
struct tnl_a_failed_to_setup_item_s {
  bool                                         ie_exts_present = false;
  cp_transport_layer_info_c                    tnl_assoc_transport_layer_address;
  cause_c                                      cause;
  tnl_a_failed_to_setup_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TNLA-Failed-To-Setup-List ::= SEQUENCE (SIZE (1..32)) OF TNLA-Failed-To-Setup-Item
using tnl_a_failed_to_setup_list_l = dyn_array<tnl_a_failed_to_setup_item_s>;

// TNLA-Setup-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tnl_a_setup_item_ext_ies_o = protocol_ext_empty_o;

using tnl_a_setup_item_ext_ies_container = protocol_ext_container_empty_l;

// TNLA-Setup-Item ::= SEQUENCE
struct tnl_a_setup_item_s {
  bool                               ie_exts_present = false;
  cp_transport_layer_info_c          tnl_assoc_transport_layer_address;
  tnl_a_setup_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TNLA-Setup-List ::= SEQUENCE (SIZE (1..32)) OF TNLA-Setup-Item
using tnl_a_setup_list_l = dyn_array<tnl_a_setup_item_s>;

// TNLAssociationUsage ::= ENUMERATED
struct tnl_assoc_usage_opts {
  enum options { ue, non_ue, both, /*...*/ nulltype } value;

  const char* to_string() const;
};
using tnl_assoc_usage_e = enumerated<tnl_assoc_usage_opts, true>;

// TNLA-To-Add-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tnl_a_to_add_item_ext_ies_o = protocol_ext_empty_o;

using tnl_a_to_add_item_ext_ies_container = protocol_ext_container_empty_l;

// TNLA-To-Add-Item ::= SEQUENCE
struct tnl_a_to_add_item_s {
  bool                                ie_exts_present = false;
  cp_transport_layer_info_c           tnl_assoc_transport_layer_address;
  tnl_assoc_usage_e                   tnl_assoc_usage;
  tnl_a_to_add_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TNLA-To-Add-List ::= SEQUENCE (SIZE (1..32)) OF TNLA-To-Add-Item
using tnl_a_to_add_list_l = dyn_array<tnl_a_to_add_item_s>;

// TNLA-To-Remove-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tnl_a_to_rem_item_ext_ies_o = protocol_ext_empty_o;

using tnl_a_to_rem_item_ext_ies_container = protocol_ext_container_empty_l;

// TNLA-To-Remove-Item ::= SEQUENCE
struct tnl_a_to_rem_item_s {
  bool                                ie_exts_present = false;
  cp_transport_layer_info_c           tnl_assoc_transport_layer_address;
  tnl_a_to_rem_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TNLA-To-Remove-List ::= SEQUENCE (SIZE (1..32)) OF TNLA-To-Remove-Item
using tnl_a_to_rem_list_l = dyn_array<tnl_a_to_rem_item_s>;

// TNLA-To-Update-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tnl_a_to_upd_item_ext_ies_o = protocol_ext_empty_o;

using tnl_a_to_upd_item_ext_ies_container = protocol_ext_container_empty_l;

// TNLA-To-Update-Item ::= SEQUENCE
struct tnl_a_to_upd_item_s {
  bool                                tnl_assoc_usage_present = false;
  bool                                ie_exts_present         = false;
  cp_transport_layer_info_c           tnl_assoc_transport_layer_address;
  tnl_assoc_usage_e                   tnl_assoc_usage;
  tnl_a_to_upd_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TNLA-To-Update-List ::= SEQUENCE (SIZE (1..32)) OF TNLA-To-Update-Item
using tnl_a_to_upd_list_l = dyn_array<tnl_a_to_upd_item_s>;

// TNLConfigurationInfo-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using tnl_cfg_info_ext_ies_o = protocol_ext_empty_o;

using tnl_cfg_info_ext_ies_container = protocol_ext_container_empty_l;

// TNLConfigurationInfo ::= SEQUENCE
struct tnl_cfg_info_s {
  bool                           ie_exts_present = false;
  ext_tlas_l                     extended_up_transport_layer_addresses_to_add;
  ext_tlas_l                     extended_up_transport_layer_addresses_to_rem;
  tnl_cfg_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TargetCellList-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using target_cell_list_item_ext_ies_o = protocol_ext_empty_o;

using target_cell_list_item_ext_ies_container = protocol_ext_container_empty_l;

// TargetCellList-Item ::= SEQUENCE
struct target_cell_list_item_s {
  bool                                    ie_exts_present = false;
  target_cgi_c                            target_cell;
  target_cell_list_item_ext_ies_container ie_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TargetCellList ::= SEQUENCE (SIZE (1..8)) OF TargetCellList-Item
using target_cell_list_l = dyn_array<target_cell_list_item_s>;

// TimeSynchronizationAssistanceInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct time_sync_assist_info_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct time_sync_assist_info_ext_ies_container {
  bool                             clock_quality_report_ctrl_info_present = false;
  clock_quality_report_ctrl_info_s clock_quality_report_ctrl_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TimeSynchronizationAssistanceInformation ::= SEQUENCE
struct time_sync_assist_info_s {
  struct time_distribution_ind_opts {
    enum options { enabled, disabled, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using time_distribution_ind_e_ = enumerated<time_distribution_ind_opts, true>;

  // member variables
  bool                                    uu_time_sync_error_budget_present = false;
  bool                                    ie_ext_present                    = false;
  time_distribution_ind_e_                time_distribution_ind;
  uint32_t                                uu_time_sync_error_budget = 0;
  time_sync_assist_info_ext_ies_container ie_ext;
  // ...

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

// Trace-Depth ::= ENUMERATED
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

// TraceActivation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct trace_activation_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trace_activation_ext_ies_container {
  bool                                                  trace_collection_entity_uri_present = false;
  bool                                                  mdt_cfg_present                     = false;
  visible_string<0, MAX_ASN_STRING_LENGTH, false, true> trace_collection_entity_uri;
  mdt_cfg_s                                             mdt_cfg;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TraceActivation ::= SEQUENCE
struct trace_activation_s {
  bool                                  ie_ext_present = false;
  fixed_octstring<8, true>              ng_ran_trace_id;
  fixed_bitstring<8, false, true>       interfaces_to_trace;
  trace_depth_e                         trace_depth;
  bounded_bitstring<1, 160, true, true> trace_coll_address;
  trace_activation_ext_ies_container    ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficProfile-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using traffic_profile_ext_ies_o = protocol_ies_empty_o;

// TrafficProfile ::= CHOICE
struct traffic_profile_c {
  struct types_opts {
    enum options { up_traffic, non_up_traffic, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  traffic_profile_c() = default;
  traffic_profile_c(const traffic_profile_c& other);
  traffic_profile_c& operator=(const traffic_profile_c& other);
  ~traffic_profile_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  qos_flow_level_qos_params_s& up_traffic()
  {
    assert_choice_type(types::up_traffic, type_, "TrafficProfile");
    return c.get<qos_flow_level_qos_params_s>();
  }
  non_up_traffic_c& non_up_traffic()
  {
    assert_choice_type(types::non_up_traffic, type_, "TrafficProfile");
    return c.get<non_up_traffic_c>();
  }
  protocol_ie_single_container_s<traffic_profile_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TrafficProfile");
    return c.get<protocol_ie_single_container_s<traffic_profile_ext_ies_o>>();
  }
  const qos_flow_level_qos_params_s& up_traffic() const
  {
    assert_choice_type(types::up_traffic, type_, "TrafficProfile");
    return c.get<qos_flow_level_qos_params_s>();
  }
  const non_up_traffic_c& non_up_traffic() const
  {
    assert_choice_type(types::non_up_traffic, type_, "TrafficProfile");
    return c.get<non_up_traffic_c>();
  }
  const protocol_ie_single_container_s<traffic_profile_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TrafficProfile");
    return c.get<protocol_ie_single_container_s<traffic_profile_ext_ies_o>>();
  }
  qos_flow_level_qos_params_s&                               set_up_traffic();
  non_up_traffic_c&                                          set_non_up_traffic();
  protocol_ie_single_container_s<traffic_profile_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<non_up_traffic_c,
                  protocol_ie_single_container_s<traffic_profile_ext_ies_o>,
                  qos_flow_level_qos_params_s>
      c;

  void destroy_();
};

// TrafficToBeRelease-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_to_be_release_item_ext_ies_o = protocol_ext_empty_o;

using traffic_to_be_release_item_ext_ies_container = protocol_ext_container_empty_l;

// TrafficToBeRelease-Item ::= SEQUENCE
struct traffic_to_be_release_item_s {
  bool                                         ie_ext_present = false;
  uint16_t                                     traffic_idx    = 1;
  bh_info_list_l                               bh_info_list;
  traffic_to_be_release_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TrafficToBeRelease-List ::= SEQUENCE (SIZE (1..1024)) OF TrafficToBeRelease-Item
using traffic_to_be_release_list_l = dyn_array<traffic_to_be_release_item_s>;

// TrafficReleaseType-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using traffic_release_type_ext_ies_o = protocol_ies_empty_o;

// TrafficReleaseType ::= CHOICE
struct traffic_release_type_c {
  struct types_opts {
    enum options { full_release, partial_release, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  traffic_release_type_c() = default;
  traffic_release_type_c(const traffic_release_type_c& other);
  traffic_release_type_c& operator=(const traffic_release_type_c& other);
  ~traffic_release_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  all_traffic_ind_e& full_release()
  {
    assert_choice_type(types::full_release, type_, "TrafficReleaseType");
    return c.get<all_traffic_ind_e>();
  }
  traffic_to_be_release_list_l& partial_release()
  {
    assert_choice_type(types::partial_release, type_, "TrafficReleaseType");
    return c.get<traffic_to_be_release_list_l>();
  }
  protocol_ie_single_container_s<traffic_release_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "TrafficReleaseType");
    return c.get<protocol_ie_single_container_s<traffic_release_type_ext_ies_o>>();
  }
  const all_traffic_ind_e& full_release() const
  {
    assert_choice_type(types::full_release, type_, "TrafficReleaseType");
    return c.get<all_traffic_ind_e>();
  }
  const traffic_to_be_release_list_l& partial_release() const
  {
    assert_choice_type(types::partial_release, type_, "TrafficReleaseType");
    return c.get<traffic_to_be_release_list_l>();
  }
  const protocol_ie_single_container_s<traffic_release_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "TrafficReleaseType");
    return c.get<protocol_ie_single_container_s<traffic_release_type_ext_ies_o>>();
  }
  all_traffic_ind_e&                                              set_full_release();
  traffic_to_be_release_list_l&                                   set_partial_release();
  protocol_ie_single_container_s<traffic_release_type_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                         type_;
  choice_buffer_t<protocol_ie_single_container_s<traffic_release_type_ext_ies_o>, traffic_to_be_release_list_l> c;

  void destroy_();
};

// TrafficToBeReleaseInformation-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using traffic_to_be_release_info_ext_ies_o = protocol_ext_empty_o;

using traffic_to_be_release_info_ext_ies_container = protocol_ext_container_empty_l;

// TrafficToBeReleaseInformation ::= SEQUENCE
struct traffic_to_be_release_info_s {
  bool                                         ie_exts_present = false;
  traffic_release_type_c                       release_type;
  traffic_to_be_release_info_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEAggregateMaximumBitRate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_aggr_max_bit_rate_ext_ies_o = protocol_ext_empty_o;

using ue_aggr_max_bit_rate_ext_ies_container = protocol_ext_container_empty_l;

// UEAggregateMaximumBitRate ::= SEQUENCE
struct ue_aggr_max_bit_rate_s {
  bool                                   ie_ext_present = false;
  uint64_t                               dl_ue_ambr     = 0;
  uint64_t                               ul_ue_ambr     = 0;
  ue_aggr_max_bit_rate_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEPerformance-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ue_performance_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_performance_ext_ies_container {
  bool     ue_average_packet_loss_ul_present = false;
  uint32_t ue_average_packet_loss_ul;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEPerformance ::= SEQUENCE
struct ue_performance_s {
  bool                             dl_ue_average_throughput_present  = false;
  bool                             ul_ue_average_throughput_present  = false;
  bool                             ue_average_packet_delay_present   = false;
  bool                             ue_average_packet_drop_dl_present = false;
  bool                             ie_exts_present                   = false;
  uint64_t                         dl_ue_average_throughput          = 0;
  uint64_t                         ul_ue_average_throughput          = 0;
  average_packet_delay_s           ue_average_packet_delay;
  uint32_t                         ue_average_packet_drop_dl = 0;
  ue_performance_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEAssociatedInfoResult-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_associated_info_result_item_ext_ies_o = protocol_ext_empty_o;

using ue_associated_info_result_item_ext_ies_container = protocol_ext_container_empty_l;

// UEAssociatedInfoResult-Item ::= SEQUENCE
struct ue_associated_info_result_item_s {
  bool                                             ue_performance_present = false;
  bool                                             ie_exts_present        = false;
  uint64_t                                         ue_assistant_id        = 0;
  ue_performance_s                                 ue_performance;
  measured_ue_trajectory_l                         measured_ue_trajectory;
  ue_associated_info_result_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEAssociatedInfoResult-List ::= SEQUENCE (SIZE (1..16)) OF UEAssociatedInfoResult-Item
using ue_associated_info_result_list_l = dyn_array<ue_associated_info_result_item_s>;

// UEContextIDforRRCResume-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_context_id_for_rrc_resume_ext_ies_o = protocol_ext_empty_o;

using ue_context_id_for_rrc_resume_ext_ies_container = protocol_ext_container_empty_l;

// UEContextIDforRRCResume ::= SEQUENCE
struct ue_context_id_for_rrc_resume_s {
  bool                                           ie_ext_present = false;
  i_rnti_c                                       i_rnti;
  fixed_bitstring<16, false, true>               allocated_c_rnti;
  ng_ran_cell_pci_c                              access_pci;
  ue_context_id_for_rrc_resume_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextIDforRRCReestablishment-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_context_id_for_rrc_reest_ext_ies_o = protocol_ext_empty_o;

using ue_context_id_for_rrc_reest_ext_ies_container = protocol_ext_container_empty_l;

// UEContextIDforRRCReestablishment ::= SEQUENCE
struct ue_context_id_for_rrc_reest_s {
  bool                                          ie_ext_present = false;
  fixed_bitstring<16, false, true>              c_rnti;
  ng_ran_cell_pci_c                             fail_cell_pci;
  ue_context_id_for_rrc_reest_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextID-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ue_context_id_ext_ies_o = protocol_ies_empty_o;

// UEContextID ::= CHOICE
struct ue_context_id_c {
  struct types_opts {
    enum options { rrc_resume, rrrc_reest, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ue_context_id_c() = default;
  ue_context_id_c(const ue_context_id_c& other);
  ue_context_id_c& operator=(const ue_context_id_c& other);
  ~ue_context_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  ue_context_id_for_rrc_resume_s& rrc_resume()
  {
    assert_choice_type(types::rrc_resume, type_, "UEContextID");
    return c.get<ue_context_id_for_rrc_resume_s>();
  }
  ue_context_id_for_rrc_reest_s& rrrc_reest()
  {
    assert_choice_type(types::rrrc_reest, type_, "UEContextID");
    return c.get<ue_context_id_for_rrc_reest_s>();
  }
  protocol_ie_single_container_s<ue_context_id_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UEContextID");
    return c.get<protocol_ie_single_container_s<ue_context_id_ext_ies_o>>();
  }
  const ue_context_id_for_rrc_resume_s& rrc_resume() const
  {
    assert_choice_type(types::rrc_resume, type_, "UEContextID");
    return c.get<ue_context_id_for_rrc_resume_s>();
  }
  const ue_context_id_for_rrc_reest_s& rrrc_reest() const
  {
    assert_choice_type(types::rrrc_reest, type_, "UEContextID");
    return c.get<ue_context_id_for_rrc_reest_s>();
  }
  const protocol_ie_single_container_s<ue_context_id_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UEContextID");
    return c.get<protocol_ie_single_container_s<ue_context_id_ext_ies_o>>();
  }
  ue_context_id_for_rrc_resume_s&                          set_rrc_resume();
  ue_context_id_for_rrc_reest_s&                           set_rrrc_reest();
  protocol_ie_single_container_s<ue_context_id_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<ue_context_id_ext_ies_o>,
                  ue_context_id_for_rrc_reest_s,
                  ue_context_id_for_rrc_resume_s>
      c;

  void destroy_();
};

// UESecurityCapabilities-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_security_cap_ext_ies_o = protocol_ext_empty_o;

using ue_security_cap_ext_ies_container = protocol_ext_container_empty_l;

// UESecurityCapabilities ::= SEQUENCE
struct ue_security_cap_s {
  bool                              ie_ext_present = false;
  fixed_bitstring<16, true, true>   nr_encyption_algorithms;
  fixed_bitstring<16, true, true>   nr_integrity_protection_algorithms;
  fixed_bitstring<16, true, true>   e_utra_encyption_algorithms;
  fixed_bitstring<16, true, true>   e_utra_integrity_protection_algorithms;
  ue_security_cap_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UESliceMaximumBitRate-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_slice_max_bit_rate_item_ext_ies_o = protocol_ext_empty_o;

using ue_slice_max_bit_rate_item_ext_ies_container = protocol_ext_container_empty_l;

// UESliceMaximumBitRate-Item ::= SEQUENCE
struct ue_slice_max_bit_rate_item_s {
  bool                                         ie_exts_present = false;
  s_nssai_s                                    s_nssai;
  uint64_t                                     dl_ue_slice_mbr = 0;
  uint64_t                                     ul_ue_slice_mbr = 0;
  ue_slice_max_bit_rate_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UESliceMaximumBitRateList ::= SEQUENCE (SIZE (1..8)) OF UESliceMaximumBitRate-Item
using ue_slice_max_bit_rate_list_l = dyn_array<ue_slice_max_bit_rate_item_s>;

// UEContextInfoRetrUECtxtResp-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct ue_context_info_retr_ue_ctxt_resp_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct ue_context_info_retr_ue_ctxt_resp_ext_ies_container {
  bool                                five_gc_mob_restrict_list_container_present    = false;
  bool                                nr_ue_sidelink_aggr_max_bit_rate_present       = false;
  bool                                lte_ue_sidelink_aggr_max_bit_rate_present      = false;
  bool                                ue_radio_cap_id_present                        = false;
  bool                                mbs_session_info_list_present                  = false;
  bool                                no_pdu_session_ind_present                     = false;
  bool                                five_g_pro_se_ue_pc5_aggr_max_bit_rate_present = false;
  bool                                ue_slice_max_bit_rate_list_present             = false;
  bool                                positioning_info_present                       = false;
  bool                                nr_a2_x_ue_pc5_aggr_max_bit_rate_present       = false;
  bool                                ltea2_x_ue_pc5_aggr_max_bit_rate_present       = false;
  bool                                nrppa_positioning_info_present                 = false;
  bool                                semipersistent_positioning_info_present        = false;
  unbounded_octstring<true>           five_gc_mob_restrict_list_container;
  nr_ue_sidelink_aggr_max_bit_rate_s  nr_ue_sidelink_aggr_max_bit_rate;
  lte_ue_sidelink_aggr_max_bit_rate_s lte_ue_sidelink_aggr_max_bit_rate;
  unbounded_octstring<true>           ue_radio_cap_id;
  mbs_session_info_list_l             mbs_session_info_list;
  no_pdu_session_ind_e                no_pdu_session_ind;
  nr_ue_sidelink_aggr_max_bit_rate_s  five_g_pro_se_ue_pc5_aggr_max_bit_rate;
  ue_slice_max_bit_rate_list_l        ue_slice_max_bit_rate_list;
  positioning_info_s                  positioning_info;
  nr_ue_sidelink_aggr_max_bit_rate_s  nr_a2_x_ue_pc5_aggr_max_bit_rate;
  lte_ue_sidelink_aggr_max_bit_rate_s ltea2_x_ue_pc5_aggr_max_bit_rate;
  nrppa_positioning_info_s            nrppa_positioning_info;
  semipersistent_positioning_info_s   semipersistent_positioning_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextInfoRetrUECtxtResp ::= SEQUENCE
struct ue_context_info_retr_ue_ctxt_resp_s {
  bool                                                mob_restrict_list_present        = false;
  bool                                                idx_to_rat_freq_sel_prio_present = false;
  bool                                                ie_ext_present                   = false;
  uint64_t                                            ng_c_ue_sig_ref                  = 0;
  cp_transport_layer_info_c                           sig_tnl_at_source;
  ue_security_cap_s                                   ue_security_cap;
  as_security_info_s                                  security_info;
  ue_aggr_max_bit_rate_s                              ue_ambr;
  pdu_session_res_to_be_setup_list_l                  pdu_session_res_to_be_setup_list;
  unbounded_octstring<true>                           rrc_context;
  mob_restrict_list_s                                 mob_restrict_list;
  uint16_t                                            idx_to_rat_freq_sel_prio = 1;
  ue_context_info_retr_ue_ctxt_resp_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEContextKeptIndicator ::= ENUMERATED
struct ue_context_kept_ind_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using ue_context_kept_ind_e = enumerated<ue_context_kept_ind_opts, true>;

// UEHistoryInformation ::= SEQUENCE (SIZE (1..16)) OF LastVisitedCell-Item
using ue_history_info_l = dyn_array<last_visited_cell_item_c>;

// UEHistoryInformationFromTheUE-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ue_history_info_from_the_ue_ext_ies_o = protocol_ies_empty_o;

// UEHistoryInformationFromTheUE ::= CHOICE
struct ue_history_info_from_the_ue_c {
  struct types_opts {
    enum options { nr, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ue_history_info_from_the_ue_c() = default;
  ue_history_info_from_the_ue_c(const ue_history_info_from_the_ue_c& other);
  ue_history_info_from_the_ue_c& operator=(const ue_history_info_from_the_ue_c& other);
  ~ue_history_info_from_the_ue_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  unbounded_octstring<true>& nr()
  {
    assert_choice_type(types::nr, type_, "UEHistoryInformationFromTheUE");
    return c.get<unbounded_octstring<true>>();
  }
  protocol_ie_single_container_s<ue_history_info_from_the_ue_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UEHistoryInformationFromTheUE");
    return c.get<protocol_ie_single_container_s<ue_history_info_from_the_ue_ext_ies_o>>();
  }
  const unbounded_octstring<true>& nr() const
  {
    assert_choice_type(types::nr, type_, "UEHistoryInformationFromTheUE");
    return c.get<unbounded_octstring<true>>();
  }
  const protocol_ie_single_container_s<ue_history_info_from_the_ue_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UEHistoryInformationFromTheUE");
    return c.get<protocol_ie_single_container_s<ue_history_info_from_the_ue_ext_ies_o>>();
  }
  unbounded_octstring<true>&                                             set_nr();
  protocol_ie_single_container_s<ue_history_info_from_the_ue_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<protocol_ie_single_container_s<ue_history_info_from_the_ue_ext_ies_o>, unbounded_octstring<true>> c;

  void destroy_();
};

// UEIdentityIndexValueMBSGroupPaging-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ue_id_idx_value_mbs_group_paging_ext_ies_o = protocol_ies_empty_o;

// UEIdentityIndexList-MBSGroupPagingValue ::= CHOICE
struct ue_id_idx_list_mbs_group_paging_value_c {
  struct types_opts {
    enum options { ue_id_idx_value_mbs_group_paging, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ue_id_idx_list_mbs_group_paging_value_c() = default;
  ue_id_idx_list_mbs_group_paging_value_c(const ue_id_idx_list_mbs_group_paging_value_c& other);
  ue_id_idx_list_mbs_group_paging_value_c& operator=(const ue_id_idx_list_mbs_group_paging_value_c& other);
  ~ue_id_idx_list_mbs_group_paging_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<10, false, true>& ue_id_idx_value_mbs_group_paging()
  {
    assert_choice_type(types::ue_id_idx_value_mbs_group_paging, type_, "UEIdentityIndexList-MBSGroupPagingValue");
    return c.get<fixed_bitstring<10, false, true>>();
  }
  protocol_ie_single_container_s<ue_id_idx_value_mbs_group_paging_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UEIdentityIndexList-MBSGroupPagingValue");
    return c.get<protocol_ie_single_container_s<ue_id_idx_value_mbs_group_paging_ext_ies_o>>();
  }
  const fixed_bitstring<10, false, true>& ue_id_idx_value_mbs_group_paging() const
  {
    assert_choice_type(types::ue_id_idx_value_mbs_group_paging, type_, "UEIdentityIndexList-MBSGroupPagingValue");
    return c.get<fixed_bitstring<10, false, true>>();
  }
  const protocol_ie_single_container_s<ue_id_idx_value_mbs_group_paging_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UEIdentityIndexList-MBSGroupPagingValue");
    return c.get<protocol_ie_single_container_s<ue_id_idx_value_mbs_group_paging_ext_ies_o>>();
  }
  fixed_bitstring<10, false, true>&                                           set_ue_id_idx_value_mbs_group_paging();
  protocol_ie_single_container_s<ue_id_idx_value_mbs_group_paging_ext_ies_o>& set_choice_ext();

private:
  types type_;
  choice_buffer_t<fixed_bitstring<10, false, true>,
                  protocol_ie_single_container_s<ue_id_idx_value_mbs_group_paging_ext_ies_o>>
      c;

  void destroy_();
};

// UESpecificDRX ::= ENUMERATED
struct ue_specific_drx_opts {
  enum options { v32, v64, v128, v256, /*...*/ nulltype } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using ue_specific_drx_e = enumerated<ue_specific_drx_opts, true>;

// UEIdentityIndexList-MBSGroupPaging-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_id_idx_list_mbs_group_paging_item_ext_ies_o = protocol_ext_empty_o;

using ue_id_idx_list_mbs_group_paging_item_ext_ies_container = protocol_ext_container_empty_l;

// UEIdentityIndexList-MBSGroupPaging-Item ::= SEQUENCE
struct ue_id_idx_list_mbs_group_paging_item_s {
  bool                                                   paging_drx_present = false;
  bool                                                   ie_ext_present     = false;
  ue_id_idx_list_mbs_group_paging_value_c                ue_id_idx_list_mbs_group_paging_value;
  ue_specific_drx_e                                      paging_drx;
  ue_id_idx_list_mbs_group_paging_item_ext_ies_container ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEIdentityIndexList-MBSGroupPaging ::= SEQUENCE (SIZE (1..4096)) OF UEIdentityIndexList-MBSGroupPaging-Item
using ue_id_idx_list_mbs_group_paging_l = dyn_array<ue_id_idx_list_mbs_group_paging_item_s>;

// UEIdentityIndexValue-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ue_id_idx_value_ext_ies_o = protocol_ies_empty_o;

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
  protocol_ie_single_container_s<ue_id_idx_value_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UEIdentityIndexValue");
    return c.get<protocol_ie_single_container_s<ue_id_idx_value_ext_ies_o>>();
  }
  const fixed_bitstring<10, false, true>& idx_len10() const
  {
    assert_choice_type(types::idx_len10, type_, "UEIdentityIndexValue");
    return c.get<fixed_bitstring<10, false, true>>();
  }
  const protocol_ie_single_container_s<ue_id_idx_value_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UEIdentityIndexValue");
    return c.get<protocol_ie_single_container_s<ue_id_idx_value_ext_ies_o>>();
  }
  fixed_bitstring<10, false, true>&                          set_idx_len10();
  protocol_ie_single_container_s<ue_id_idx_value_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                        type_;
  choice_buffer_t<fixed_bitstring<10, false, true>, protocol_ie_single_container_s<ue_id_idx_value_ext_ies_o>> c;

  void destroy_();
};

// UEPerformanceCollectionConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_performance_collection_cfg_ext_ies_o = protocol_ext_empty_o;

using ue_performance_collection_cfg_ext_ies_container = protocol_ext_container_empty_l;

// UEPerformanceCollectionConfiguration ::= SEQUENCE
struct ue_performance_collection_cfg_s {
  bool                                            ie_exts_present                        = false;
  uint16_t                                        collection_time_dur_for_ue_performance = 1;
  ue_performance_collection_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UERANPagingIdentity-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
using ue_ran_paging_id_ext_ies_o = protocol_ies_empty_o;

// UERANPagingIdentity ::= CHOICE
struct ue_ran_paging_id_c {
  struct types_opts {
    enum options { i_rnti_full, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  ue_ran_paging_id_c() = default;
  ue_ran_paging_id_c(const ue_ran_paging_id_c& other);
  ue_ran_paging_id_c& operator=(const ue_ran_paging_id_c& other);
  ~ue_ran_paging_id_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  fixed_bitstring<40, false, true>& i_rnti_full()
  {
    assert_choice_type(types::i_rnti_full, type_, "UERANPagingIdentity");
    return c.get<fixed_bitstring<40, false, true>>();
  }
  protocol_ie_single_container_s<ue_ran_paging_id_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "UERANPagingIdentity");
    return c.get<protocol_ie_single_container_s<ue_ran_paging_id_ext_ies_o>>();
  }
  const fixed_bitstring<40, false, true>& i_rnti_full() const
  {
    assert_choice_type(types::i_rnti_full, type_, "UERANPagingIdentity");
    return c.get<fixed_bitstring<40, false, true>>();
  }
  const protocol_ie_single_container_s<ue_ran_paging_id_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "UERANPagingIdentity");
    return c.get<protocol_ie_single_container_s<ue_ran_paging_id_ext_ies_o>>();
  }
  fixed_bitstring<40, false, true>&                           set_i_rnti_full();
  protocol_ie_single_container_s<ue_ran_paging_id_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                         type_;
  choice_buffer_t<fixed_bitstring<40, false, true>, protocol_ie_single_container_s<ue_ran_paging_id_ext_ies_o>> c;

  void destroy_();
};

// UERadioCapabilityForPaging-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_radio_cap_for_paging_ext_ies_o = protocol_ext_empty_o;

using ue_radio_cap_for_paging_ext_ies_container = protocol_ext_container_empty_l;

// UERadioCapabilityForPaging ::= SEQUENCE
struct ue_radio_cap_for_paging_s {
  bool                                      ie_exts_present = false;
  unbounded_octstring<true>                 ue_radio_cap_for_paging_of_nr;
  unbounded_octstring<true>                 ue_radio_cap_for_paging_of_eutra;
  ue_radio_cap_for_paging_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UETrajectoryCollectionConfiguration-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using ue_trajectory_collection_cfg_ext_ies_o = protocol_ext_empty_o;

using ue_trajectory_collection_cfg_ext_ies_container = protocol_ext_container_empty_l;

// UETrajectoryCollectionConfiguration ::= SEQUENCE
struct ue_trajectory_collection_cfg_s {
  bool                                           nof_visited_cells_present             = false;
  bool                                           ie_exts_present                       = false;
  uint16_t                                       collection_time_dur_for_ue_trajectory = 1;
  uint8_t                                        nof_visited_cells                     = 1;
  ue_trajectory_collection_cfg_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UserPlaneFailureType ::= ENUMERATED
struct user_plane_fail_type_opts {
  enum options { gtp_u_error_ind_rx, up_path_fail, /*...*/ nulltype } value;

  const char* to_string() const;
};
using user_plane_fail_type_e = enumerated<user_plane_fail_type_opts, true>;

// UserPlaneFailureIndication-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
using user_plane_fail_ind_ext_ies_o = protocol_ext_empty_o;

using user_plane_fail_ind_ext_ies_container = protocol_ext_container_empty_l;

// UserPlaneFailureIndication ::= SEQUENCE
struct user_plane_fail_ind_s {
  bool                                  ie_exts_present = false;
  user_plane_fail_type_e                user_plane_fail_type;
  up_transport_layer_info_c             dl_ng_u_tnl_at_ng_ran;
  up_transport_layer_info_c             ul_ng_u_tnl_at_ng_ran;
  user_plane_fail_ind_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UserPlaneTrafficActivityReport ::= ENUMERATED
struct user_plane_traffic_activity_report_opts {
  enum options { inactive, re_activ, /*...*/ nulltype } value;

  const char* to_string() const;
};
using user_plane_traffic_activity_report_e = enumerated<user_plane_traffic_activity_report_opts, true>;

// XnUAddressInfoperPDUSession-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
struct xn_u_address_infoper_pdu_session_item_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct xn_u_address_infoper_pdu_session_item_ext_ies_container {
  bool                                             secondarydata_forwarding_info_from_target_list_present = false;
  bool                                             drb_ids_takenintouse_present                           = false;
  bool                                             data_forwarding_info_from_target_e_utran_node_present  = false;
  secondarydata_forwarding_info_from_target_list_l secondarydata_forwarding_info_from_target_list;
  drb_list_l                                       drb_ids_takenintouse;
  data_forwarding_info_from_target_e_utran_node_s  data_forwarding_info_from_target_e_utran_node;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnUAddressInfoperPDUSession-Item ::= SEQUENCE
struct xn_u_address_infoper_pdu_session_item_s {
  bool                                          data_forwarding_info_from_target_ngran_node_present = false;
  bool                                          pdu_session_res_setup_complete_info_sn_term_present = false;
  bool                                          ie_ext_present                                      = false;
  uint16_t                                      pdu_session_id                                      = 0;
  data_forwarding_info_from_target_ngran_node_s data_forwarding_info_from_target_ngran_node;
  pdu_session_res_bearer_setup_complete_info_sn_terminated_s pdu_session_res_setup_complete_info_sn_term;
  xn_u_address_infoper_pdu_session_item_ext_ies_container    ie_ext;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// XnUAddressInfoperPDUSession-List ::= SEQUENCE (SIZE (1..256)) OF XnUAddressInfoperPDUSession-Item
using xn_u_address_infoper_pdu_session_list_l = dyn_array<xn_u_address_infoper_pdu_session_item_s>;

} // namespace xnap
} // namespace asn1

extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::a2_xpc5_qos_flow_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::amf_region_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::alt_qos_para_set_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_qos_flows_to_add_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_service_area_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_service_area_tai_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_service_area_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_mapand_data_forwarding_request_infofrom_source_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::csi_rs_mtc_neighbour_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::csi_rs_neighbour_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::csi_rs_mtc_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::add_meas_timing_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::add_pdcp_dupl_tnl_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::add_ul_ng_u_tnl_at_up_f_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_f_lows_accepted_to_be_forwarded_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::data_forwarding_resp_drb_item_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::add_listof_pdu_session_res_change_confirm_info_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::allowed_pni_npn_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::list_of_tai_sin_ao_i_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::list_of_cells_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::list_of_ran_nodesin_ao_i_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::area_of_interest_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_id_listfor_mdt_eutra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tai_listfor_mdt_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cag_listfor_mdt_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sn_pn_cell_id_listfor_mdt_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sn_pn_tai_listfor_mdt_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::sn_pn_listfor_mdt_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::nr_carrier_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::supported_sul_band_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::nr_freq_band_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::area_scope_of_neigh_cells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cell_id_listfor_qmc_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tai_listfor_qmc_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::associated_qos_flow_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::bap_ctrl_pdu_rlc_ch_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::bh_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::bplmn_id_info_eutra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::broadcast_n_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::broadcast_sn_pn_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::broadcast_cag_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::broadcast_pni_npn_id_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::bplmn_id_info_nr_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slice_support_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tai_nsag_support_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::tai_slice_unavailable_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_admitted_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_list_with_cause_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::secondarydata_forwarding_info_from_target_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_admitted_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cho_candidate_p_scells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cho_target_sn_node_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cho_execution_condition_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cho_candidate_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cn_type_restricts_for_equivalent_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_ac_execution_condition_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_ac_candidate_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_a_ccandidate_p_scells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_a_ccandidate_p_scells_wother_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::s_cp_ac_security_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::s_cp_ac_multi_target_sn_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_c_target_sn_confirm_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::c_pci_nformation_upd_p_scells_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_c_target_sn_mod_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cp_c_target_sn_required_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::candidate_relay_ue_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ssb_area_capacity_value_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_based_ue_trajectory_prediction_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ssb_area_rr_status_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::snssai_rr_status_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slice_rr_status_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_meas_fail_cause_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_meas_initiation_result_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::snssai_available_capacity_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::slice_available_capacity_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::nr_u_ch_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cell_meas_result_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_meas_result_for_data_collection_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::replacing_cells_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ssb_to_report_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::snssai_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slice_to_report_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::cell_to_report_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::cell_to_report_for_data_collection_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::conditional_recfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ssb_coverage_mod_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::coverage_mod_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::crit_diagnostics_ie_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::daps_resp_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::dl_lbt_fail_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::drb_list_with_cause_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::qos_flows_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drb_to_qos_flow_map_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::up_transport_params_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_admitted_list_mod_confirm_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_mappedto_drb_setup_resp_m_nterminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_admitted_list_mod_resp_m_nterminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_admitted_list_setup_resp_m_nterminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_subject_to_dl_discarding_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_subject_to_early_status_transfer_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_subject_to_status_transfer_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_mod_rqd_m_nterminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_modified_mappedto_drb_mod_rqd_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::rlc_dupl_state_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_mod_rqd_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_modified_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_mappedto_drb_setup_m_nterminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_mod_m_nterminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_mappedto_drb_setup_resp_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_modified_list_mod_resp_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_setup_mappedto_drb_mod_rqd_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_setup_list_mod_rqd_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_setup_list_setup_m_nterminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::drbs_to_be_setup_list_setup_resp_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::du_f_slot_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_to_be_forwarded_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::data_forwarding_info_from_target_e_utran_node_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_f_lows_to_be_forwarded_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::equivalent_sn_p_ns_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::excess_packet_delay_thres_cfg_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::expected_ue_moving_trajectory_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::gtp_tlas_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ext_tlas_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::f1_terminating_bh_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::five_g_pro_se_pc5_qos_flow_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::flows_mapped_to_drb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::forbidden_area_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::freq_domain_slot_h_sn_acfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::freq_domain_h_sn_acfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::hs_nas_lot_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::na_cell_res_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::iab_mt_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::iab_stc_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::iab_tnl_address_to_rem_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::iab_allocated_tnl_address_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::iab_tnl_address_exception_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sensor_meas_cfg_name_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::slot_cfg_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::last_visited_pscell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_data_forwarding_resp_infofrom_target_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_session_associated_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbs_session_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mbs_session_info_resp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::mbsfn_sf_info_e_utra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mn_to_sn_q_mcc_oord_request_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::mn_to_sn_q_mcc_oord_resp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::measured_ue_trajectory_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::rat_restricts_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::service_area_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ng_ran_cell_id_listin_ran_paging_area_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ng_ran_node2_ssb_offsets_mod_range_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::non_anchor_carrier_freqlist_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::nr_u_ch_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::neighbour_ng_ran_node_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::neighbour_info_e_utra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::neighbour_info_nr_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::node_meas_initiation_result_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::non_f1_terminating_bh_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::pc5_qos_flow_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_list_with_cause_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_list_with_data_forwarding_from_target_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_list_with_data_forwarding_request_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<
    asn1::xnap::pdu_session_res_bearer_setup_complete_info_sn_terminated_s::drbs_to_be_setup_list_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_to_be_setup_list_setup_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_to_be_setup_list_modified_sn_terminated_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::volume_timed_report_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_usage_report_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_secondary_rat_usage_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_not_admitted_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flows_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_session_res_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::pdu_sessions_list_to_be_released_up_error_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::protected_e_utra_res_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ue_app_layer_meas_info_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sn_to_mn_q_mcc_oord_request_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sn_to_mn_q_mcc_oord_resp_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::qos_flow_notif_ctrl_ind_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ran_area_id_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ra_report_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::rspp_qos_flow_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ra_report_ind_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::reset_request_partial_release_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::reset_resp_partial_release_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sdt_drbs_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sdt_srbs_to_be_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::sdt_data_forwarding_drb_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ssb_offsets_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::served_cell_info_e_utra_s::broadcast_plmns_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::served_cell_specific_info_req_nr_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::served_cells_e_utra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::served_cells_nr_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::served_cells_to_modify_e_utra_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::served_cells_to_modify_nr_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::successful_ho_report_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::successful_pscell_change_report_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::tai_support_item_s::broadcast_plmns_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tai_support_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::tnl_a_failed_to_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_setup_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_to_add_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_to_rem_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::tnl_a_to_upd_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::target_cell_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::traffic_to_be_release_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ue_associated_info_result_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ue_slice_max_bit_rate_list_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops asn1::dyn_array<asn1::xnap::ue_history_info_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::ue_id_idx_list_mbs_group_paging_l::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::xnap::xn_u_address_infoper_pdu_session_list_l::value_type>::ops;
