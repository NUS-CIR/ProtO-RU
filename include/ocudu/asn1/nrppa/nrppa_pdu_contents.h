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

#include "ocudu/asn1/nrppa/common.h"
#include "ocudu/asn1/nrppa/nrppa_ies.h"

namespace asn1 {
namespace nrppa {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// AperiodicSRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct aperiodic_srs_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct aperiodic_srs_ext_ies_container {
  bool                               pos_srs_res_set_aggregation_list_present = false;
  pos_srs_res_set_aggregation_list_l pos_srs_res_set_aggregation_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AperiodicSRS ::= SEQUENCE
struct aperiodic_srs_s {
  struct aperiodic_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using aperiodic_e_ = enumerated<aperiodic_opts, true>;

  // member variables
  bool                            srs_res_trigger_present = false;
  bool                            ie_exts_present         = false;
  aperiodic_e_                    aperiodic;
  srs_res_trigger_s               srs_res_trigger;
  aperiodic_srs_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AssistanceInformationControl-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct assist_info_ctrl_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct assist_info_ctrl_ies_container {
  bool                          assist_info_present                 = false;
  bool                          broadcast_present                   = false;
  bool                          positioning_broadcast_cells_present = false;
  assist_info_s                 assist_info;
  broadcast_e                   broadcast;
  positioning_broadcast_cells_l positioning_broadcast_cells;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AssistanceInformationControl ::= SEQUENCE
using assist_info_ctrl_s = elementary_procedure_option<assist_info_ctrl_ies_container>;

// AssistanceInformationFeedback-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct assist_info_feedback_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct assist_info_feedback_ies_container {
  bool                          assist_info_fail_list_present       = false;
  bool                          positioning_broadcast_cells_present = false;
  bool                          crit_diagnostics_present            = false;
  assist_info_fail_list_l       assist_info_fail_list;
  positioning_broadcast_cells_l positioning_broadcast_cells;
  crit_diagnostics_s            crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// AssistanceInformationFeedback ::= SEQUENCE
using assist_info_feedback_s = elementary_procedure_option<assist_info_feedback_ies_container>;

// E-CIDMeasurementFailureIndication-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct e_c_id_meas_fail_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_c_id_meas_fail_ind_ies_container {
  uint8_t lmf_ue_meas_id;
  uint8_t ran_ue_meas_id;
  cause_c cause;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CIDMeasurementFailureIndication ::= SEQUENCE
using e_c_id_meas_fail_ind_s = elementary_procedure_option<e_c_id_meas_fail_ind_ies_container>;

// E-CIDMeasurementInitiationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct e_c_id_meas_initiation_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_c_id_meas_initiation_fail_ies_container {
  bool               crit_diagnostics_present = false;
  uint8_t            lmf_ue_meas_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CIDMeasurementInitiationFailure ::= SEQUENCE
using e_c_id_meas_initiation_fail_s = elementary_procedure_option<e_c_id_meas_initiation_fail_ies_container>;

// E-CIDMeasurementInitiationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct e_c_id_meas_initiation_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_c_id_meas_initiation_request_ies_container {
  bool                        meas_periodicity_present          = false;
  bool                        other_rat_meas_quantities_present = false;
  bool                        wlan_meas_quantities_present      = false;
  bool                        meas_periodicity_nr_ao_a_present  = false;
  uint8_t                     lmf_ue_meas_id;
  report_characteristics_e    report_characteristics;
  meas_periodicity_e          meas_periodicity;
  meas_quantities_l           meas_quantities;
  other_rat_meas_quantities_l other_rat_meas_quantities;
  wlan_meas_quantities_l      wlan_meas_quantities;
  meas_periodicity_nr_ao_a_e  meas_periodicity_nr_ao_a;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CIDMeasurementInitiationRequest ::= SEQUENCE
using e_c_id_meas_initiation_request_s = elementary_procedure_option<e_c_id_meas_initiation_request_ies_container>;

// E-CIDMeasurementInitiationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct e_c_id_meas_initiation_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_c_id_meas_initiation_resp_ies_container {
  bool                    e_c_id_meas_result_present    = false;
  bool                    crit_diagnostics_present      = false;
  bool                    cell_portion_id_present       = false;
  bool                    other_rat_meas_result_present = false;
  bool                    wlan_meas_result_present      = false;
  uint8_t                 lmf_ue_meas_id;
  uint8_t                 ran_ue_meas_id;
  e_c_id_meas_result_s    e_c_id_meas_result;
  crit_diagnostics_s      crit_diagnostics;
  uint16_t                cell_portion_id;
  other_rat_meas_result_l other_rat_meas_result;
  wlan_meas_result_l      wlan_meas_result;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CIDMeasurementInitiationResponse ::= SEQUENCE
using e_c_id_meas_initiation_resp_s = elementary_procedure_option<e_c_id_meas_initiation_resp_ies_container>;

// E-CIDMeasurementReport-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct e_c_id_meas_report_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_c_id_meas_report_ies_container {
  bool                 cell_portion_id_present = false;
  uint8_t              lmf_ue_meas_id;
  uint8_t              ran_ue_meas_id;
  e_c_id_meas_result_s e_c_id_meas_result;
  uint16_t             cell_portion_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CIDMeasurementReport ::= SEQUENCE
using e_c_id_meas_report_s = elementary_procedure_option<e_c_id_meas_report_ies_container>;

// E-CIDMeasurementTerminationCommand-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct e_c_id_meas_termination_cmd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct e_c_id_meas_termination_cmd_ies_container {
  uint8_t lmf_ue_meas_id;
  uint8_t ran_ue_meas_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E-CIDMeasurementTerminationCommand ::= SEQUENCE
using e_c_id_meas_termination_cmd_s = elementary_procedure_option<e_c_id_meas_termination_cmd_ies_container>;

// ErrorIndication-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct error_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct error_ind_ies_container {
  bool               cause_present            = false;
  bool               crit_diagnostics_present = false;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// ErrorIndication ::= SEQUENCE
using error_ind_s = elementary_procedure_option<error_ind_ies_container>;

// MeasurementAbort-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_abort_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_abort_ies_container {
  uint32_t lmf_meas_id;
  uint32_t ran_meas_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementAbort ::= SEQUENCE
using meas_abort_s = elementary_procedure_option<meas_abort_ies_container>;

// MeasurementActivation-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_activation_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_activation_ies_container {
  bool                  prs_meass_info_list_present = false;
  request_type_e        request_type;
  prs_meass_info_list_l prs_meass_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementActivation ::= SEQUENCE
using meas_activation_s = elementary_procedure_option<meas_activation_ies_container>;

// MeasurementFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_fail_ies_container {
  bool               crit_diagnostics_present = false;
  uint32_t           lmf_meas_id;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementFailure ::= SEQUENCE
using meas_fail_s = elementary_procedure_option<meas_fail_ies_container>;

// MeasurementFailureIndication-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_fail_ind_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_fail_ind_ies_container {
  uint32_t lmf_meas_id;
  uint32_t ran_meas_id;
  cause_c  cause;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementFailureIndication ::= SEQUENCE
using meas_fail_ind_s = elementary_procedure_option<meas_fail_ind_ies_container>;

// MeasurementPreconfigurationConfirm-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_precfg_confirm_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_precfg_confirm_ies_container {
  bool                            crit_diagnostics_present = false;
  fixed_bitstring<8, false, true> precfg_result;
  crit_diagnostics_s              crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementPreconfigurationConfirm ::= SEQUENCE
using meas_precfg_confirm_s = elementary_procedure_option<meas_precfg_confirm_ies_container>;

// MeasurementPreconfigurationRefuse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_precfg_refuse_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_precfg_refuse_ies_container {
  bool               crit_diagnostics_present = false;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementPreconfigurationRefuse ::= SEQUENCE
using meas_precfg_refuse_s = elementary_procedure_option<meas_precfg_refuse_ies_container>;

// MeasurementPreconfigurationRequired-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_precfg_required_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_precfg_required_ies_container {
  trp_prs_info_list_l trp_prs_info_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementPreconfigurationRequired ::= SEQUENCE
using meas_precfg_required_s = elementary_procedure_option<meas_precfg_required_ies_container>;

// MeasurementReport-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_report_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_report_ies_container {
  uint32_t             lmf_meas_id;
  uint32_t             ran_meas_id;
  trp_meas_resp_list_l trp_meas_report_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementReport ::= SEQUENCE
using meas_report_s = elementary_procedure_option<meas_report_ies_container>;

// MeasurementRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_request_ies_container {
  bool                             meas_periodicity_present                 = false;
  bool                             sfn_initisation_time_present             = false;
  bool                             srs_cfg_present                          = false;
  bool                             meas_beam_info_request_present           = false;
  bool                             sys_frame_num_present                    = false;
  bool                             slot_num_present                         = false;
  bool                             meas_periodicity_extended_present        = false;
  bool                             resp_time_present                        = false;
  bool                             meas_characteristics_request_ind_present = false;
  bool                             meas_time_occasion_present               = false;
  bool                             meas_amount_present                      = false;
  bool                             time_win_info_meas_list_present          = false;
  uint32_t                         lmf_meas_id;
  trp_meas_request_list_l          trp_meas_request_list;
  report_characteristics_e         report_characteristics;
  meas_periodicity_e               meas_periodicity;
  trp_meas_quantities_l            trp_meas_quantities;
  fixed_bitstring<64, false, true> sfn_initisation_time;
  srs_configuration_s              srs_cfg;
  meas_beam_info_request_e         meas_beam_info_request;
  uint16_t                         sys_frame_num;
  uint8_t                          slot_num;
  meas_periodicity_extended_e      meas_periodicity_extended;
  resp_time_s                      resp_time;
  fixed_bitstring<16, false, true> meas_characteristics_request_ind;
  meas_time_occasion_e             meas_time_occasion;
  meas_amount_e                    meas_amount;
  time_win_info_meas_list_l        time_win_info_meas_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementRequest ::= SEQUENCE
using meas_request_s = elementary_procedure_option<meas_request_ies_container>;

// MeasurementResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_resp_ies_container {
  bool                 trp_meas_resp_list_present = false;
  bool                 crit_diagnostics_present   = false;
  uint32_t             lmf_meas_id;
  uint32_t             ran_meas_id;
  trp_meas_resp_list_l trp_meas_resp_list;
  crit_diagnostics_s   crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementResponse ::= SEQUENCE
using meas_resp_s = elementary_procedure_option<meas_resp_ies_container>;

// MeasurementUpdate-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct meas_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct meas_upd_ies_container {
  bool                             srs_cfg_present                          = false;
  bool                             trp_meas_upd_list_present                = false;
  bool                             meas_characteristics_request_ind_present = false;
  bool                             meas_time_occasion_present               = false;
  uint32_t                         lmf_meas_id;
  uint32_t                         ran_meas_id;
  srs_configuration_s              srs_cfg;
  trp_meas_upd_list_l              trp_meas_upd_list;
  fixed_bitstring<16, false, true> meas_characteristics_request_ind;
  meas_time_occasion_e             meas_time_occasion;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementUpdate ::= SEQUENCE
using meas_upd_s = elementary_procedure_option<meas_upd_ies_container>;

// OTDOA-Information-Type-ItemExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
using otdoa_info_type_item_ext_ies_o = protocol_ext_empty_o;

using otdoa_info_type_item_ext_ies_container = protocol_ext_container_empty_l;

// OTDOA-Information-Type-Item ::= SEQUENCE
struct otdoa_info_type_item_s {
  bool                                   ie_exts_present = false;
  otdoa_info_item_e                      otdoa_info_item;
  otdoa_info_type_item_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// OTDOA-Information-Type-ItemIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct otdoa_info_type_item_ies_o {
  // Value ::= OPEN TYPE
  struct value_c {
    struct types_opts {
      enum options { otdoa_info_type_item, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    types         type() const { return types::otdoa_info_type_item; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    otdoa_info_type_item_s&       otdoa_info_type_item() { return c; }
    const otdoa_info_type_item_s& otdoa_info_type_item() const { return c; }

  private:
    otdoa_info_type_item_s c;
  };

  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static value_c    get_value(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

// OTDOA-Information-Type ::= SEQUENCE (SIZE (1..63)) OF ProtocolIE-Field{NRPPA-PROTOCOL-IES : IEsSetParam}
using otdoa_info_type_l = dyn_array<protocol_ie_single_container_s<otdoa_info_type_item_ies_o>>;

// OTDOAInformationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct otdoa_info_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct otdoa_info_fail_ies_container {
  bool               crit_diagnostics_present = false;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// OTDOAInformationFailure ::= SEQUENCE
using otdoa_info_fail_s = elementary_procedure_option<otdoa_info_fail_ies_container>;

// OTDOAInformationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct otdoa_info_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct otdoa_info_request_ies_container {
  otdoa_info_type_l otdoa_info_type_group;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// OTDOAInformationRequest ::= SEQUENCE
using otdoa_info_request_s = elementary_procedure_option<otdoa_info_request_ies_container>;

// OTDOAInformationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct otdoa_info_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct otdoa_info_resp_ies_container {
  bool               crit_diagnostics_present = false;
  otdoa_cells_l      otdoa_cells;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// OTDOAInformationResponse ::= SEQUENCE
using otdoa_info_resp_s = elementary_procedure_option<otdoa_info_resp_ies_container>;

// PRSConfigurationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct prs_cfg_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct prs_cfg_fail_ies_container {
  bool               crit_diagnostics_present = false;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSConfigurationFailure ::= SEQUENCE
using prs_cfg_fail_s = elementary_procedure_option<prs_cfg_fail_ies_container>;

// PRSConfigurationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct prs_cfg_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct prs_cfg_request_ies_container {
  prs_cfg_request_type_e prs_cfg_request_type;
  prstrp_list_l          prstrp_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSConfigurationRequest ::= SEQUENCE
using prs_cfg_request_s = elementary_procedure_option<prs_cfg_request_ies_container>;

// PRSConfigurationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct prs_cfg_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct prs_cfg_resp_ies_container {
  bool               prs_tx_trp_list_present  = false;
  bool               crit_diagnostics_present = false;
  prs_tx_trp_list_l  prs_tx_trp_list;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PRSConfigurationResponse ::= SEQUENCE
using prs_cfg_resp_s = elementary_procedure_option<prs_cfg_resp_ies_container>;

// PositioningActivationFailureIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct positioning_activation_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct positioning_activation_fail_ies_container {
  bool               crit_diagnostics_present = false;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningActivationFailure ::= SEQUENCE
using positioning_activation_fail_s = elementary_procedure_option<positioning_activation_fail_ies_container>;

// SemipersistentSRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
struct semipersistent_srs_ext_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct semipersistent_srs_ext_ies_container {
  bool                               srs_spatial_relation_present             = false;
  bool                               srs_spatial_relation_per_srs_res_present = false;
  bool                               pos_srs_res_set_aggregation_list_present = false;
  spatial_relation_info_s            srs_spatial_relation;
  spatial_relation_per_srs_res_s     srs_spatial_relation_per_srs_res;
  pos_srs_res_set_aggregation_list_l pos_srs_res_set_aggregation_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SemipersistentSRS ::= SEQUENCE
struct semipersistent_srs_s {
  bool                                 ie_exts_present = false;
  uint8_t                              srs_res_set_id  = 0;
  semipersistent_srs_ext_ies_container ie_exts;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSType-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
using srs_type_ext_ies_o = protocol_ies_empty_o;

// SRSType ::= CHOICE
struct srs_type_c {
  struct types_opts {
    enum options { semipersistent_srs, aperiodic_srs, choice_ext, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  srs_type_c() = default;
  srs_type_c(const srs_type_c& other);
  srs_type_c& operator=(const srs_type_c& other);
  ~srs_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  semipersistent_srs_s& semipersistent_srs()
  {
    assert_choice_type(types::semipersistent_srs, type_, "SRSType");
    return c.get<semipersistent_srs_s>();
  }
  aperiodic_srs_s& aperiodic_srs()
  {
    assert_choice_type(types::aperiodic_srs, type_, "SRSType");
    return c.get<aperiodic_srs_s>();
  }
  protocol_ie_single_container_s<srs_type_ext_ies_o>& choice_ext()
  {
    assert_choice_type(types::choice_ext, type_, "SRSType");
    return c.get<protocol_ie_single_container_s<srs_type_ext_ies_o>>();
  }
  const semipersistent_srs_s& semipersistent_srs() const
  {
    assert_choice_type(types::semipersistent_srs, type_, "SRSType");
    return c.get<semipersistent_srs_s>();
  }
  const aperiodic_srs_s& aperiodic_srs() const
  {
    assert_choice_type(types::aperiodic_srs, type_, "SRSType");
    return c.get<aperiodic_srs_s>();
  }
  const protocol_ie_single_container_s<srs_type_ext_ies_o>& choice_ext() const
  {
    assert_choice_type(types::choice_ext, type_, "SRSType");
    return c.get<protocol_ie_single_container_s<srs_type_ext_ies_o>>();
  }
  semipersistent_srs_s&                               set_semipersistent_srs();
  aperiodic_srs_s&                                    set_aperiodic_srs();
  protocol_ie_single_container_s<srs_type_ext_ies_o>& set_choice_ext();

private:
  types                                                                                                      type_;
  choice_buffer_t<aperiodic_srs_s, protocol_ie_single_container_s<srs_type_ext_ies_o>, semipersistent_srs_s> c;

  void destroy_();
};

// PositioningActivationRequestIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct positioning_activation_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct positioning_activation_request_ies_container {
  bool                             activation_time_present = false;
  srs_type_c                       srs_type;
  fixed_bitstring<64, false, true> activation_time;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningActivationRequest ::= SEQUENCE
using positioning_activation_request_s = elementary_procedure_option<positioning_activation_request_ies_container>;

// PositioningActivationResponseIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct positioning_activation_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct positioning_activation_resp_ies_container {
  bool               crit_diagnostics_present = false;
  bool               sys_frame_num_present    = false;
  bool               slot_num_present         = false;
  crit_diagnostics_s crit_diagnostics;
  uint16_t           sys_frame_num;
  uint8_t            slot_num;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningActivationResponse ::= SEQUENCE
using positioning_activation_resp_s = elementary_procedure_option<positioning_activation_resp_ies_container>;

// PositioningDeactivationIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct positioning_deactivation_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct positioning_deactivation_ies_container {
  abort_tx_c abort_tx;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningDeactivation ::= SEQUENCE
using positioning_deactivation_s = elementary_procedure_option<positioning_deactivation_ies_container>;

// PositioningInformationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct positioning_info_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct positioning_info_fail_ies_container {
  bool               crit_diagnostics_present = false;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningInformationFailure ::= SEQUENCE
using positioning_info_fail_s = elementary_procedure_option<positioning_info_fail_ies_container>;

// PositioningInformationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct positioning_info_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct positioning_info_request_ies_container {
  bool                                        requested_srs_tx_characteristics_present          = false;
  bool                                        ue_report_info_present                            = false;
  bool                                        ue_teg_info_request_present                       = false;
  bool                                        ue_teg_report_periodicity_present                 = false;
  bool                                        time_win_info_srs_list_present                    = false;
  bool                                        requested_srs_precfg_characteristics_list_present = false;
  bool                                        remote_ue_ind_request_present                     = false;
  requested_srs_tx_characteristics_s          requested_srs_tx_characteristics;
  ue_report_info_s                            ue_report_info;
  ue_teg_info_request_e                       ue_teg_info_request;
  ue_teg_report_periodicity_e                 ue_teg_report_periodicity;
  time_win_info_srs_list_l                    time_win_info_srs_list;
  requested_srs_precfg_characteristics_list_l requested_srs_precfg_characteristics_list;
  remote_ue_ind_request_e                     remote_ue_ind_request;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningInformationRequest ::= SEQUENCE
using positioning_info_request_s = elementary_procedure_option<positioning_info_request_ies_container>;

// PositioningInformationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct positioning_info_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct positioning_info_resp_ies_container {
  bool                             srs_cfg_present                     = false;
  bool                             sfn_initisation_time_present        = false;
  bool                             crit_diagnostics_present            = false;
  bool                             ue_tx_teg_assoc_list_present        = false;
  bool                             new_nr_cgi_present                  = false;
  bool                             pos_validity_area_cell_list_present = false;
  bool                             srs_precfg_list_present             = false;
  bool                             remote_ue_ind_present               = false;
  srs_configuration_s              srs_cfg;
  fixed_bitstring<64, false, true> sfn_initisation_time;
  crit_diagnostics_s               crit_diagnostics;
  ue_tx_teg_assoc_list_l           ue_tx_teg_assoc_list;
  cgi_nr_s                         new_nr_cgi;
  pos_validity_area_cell_list_l    pos_validity_area_cell_list;
  srs_precfg_list_l                srs_precfg_list;
  remote_ue_ind_e                  remote_ue_ind;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningInformationResponse ::= SEQUENCE
using positioning_info_resp_s = elementary_procedure_option<positioning_info_resp_ies_container>;

// PositioningInformationUpdate-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct positioning_info_upd_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct positioning_info_upd_ies_container {
  bool                             srs_cfg_present              = false;
  bool                             sfn_initisation_time_present = false;
  bool                             ue_tx_teg_assoc_list_present = false;
  bool                             srs_tx_status_present        = false;
  bool                             new_cell_id_present          = false;
  bool                             remote_ue_status_present     = false;
  srs_configuration_s              srs_cfg;
  fixed_bitstring<64, false, true> sfn_initisation_time;
  ue_tx_teg_assoc_list_l           ue_tx_teg_assoc_list;
  srs_tx_status_e                  srs_tx_status;
  cgi_nr_s                         new_cell_id;
  remote_ue_status_e               remote_ue_status;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// PositioningInformationUpdate ::= SEQUENCE
using positioning_info_upd_s = elementary_procedure_option<positioning_info_upd_ies_container>;

struct nr_ppa_private_ies_empty_o {
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
// PrivateMessage-IEs ::= OBJECT SET OF NRPPA-PRIVATE-IES
using private_msg_ies_o = nr_ppa_private_ies_empty_o;

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

// SRSInformationReservationNotification-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct srs_info_reserv_notif_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct srs_info_reserv_notif_ies_container {
  bool                                        srs_info_present        = false;
  bool                                        precfg_srs_info_present = false;
  srs_reserv_type_e                           srs_reserv_type;
  requested_srs_tx_characteristics_s          srs_info;
  requested_srs_precfg_characteristics_list_l precfg_srs_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SRSInformationReservationNotification ::= SEQUENCE
using srs_info_reserv_notif_s = elementary_procedure_option<srs_info_reserv_notif_ies_container>;

// TRPInformationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct trp_info_fail_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_info_fail_ies_container {
  bool               crit_diagnostics_present = false;
  cause_c            cause;
  crit_diagnostics_s crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPInformationFailure ::= SEQUENCE
using trp_info_fail_s = elementary_procedure_option<trp_info_fail_ies_container>;

// TRPInformationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct trp_info_request_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_info_request_ies_container {
  bool                         trp_list_present = false;
  trp_list_l                   trp_list;
  trp_info_type_list_trp_req_l trp_info_type_list_trp_req;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPInformationRequest ::= SEQUENCE
using trp_info_request_s = elementary_procedure_option<trp_info_request_ies_container>;

// TRPInformationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
struct trp_info_resp_ies_o {
  // members lookup methods
  static uint32_t   idx_to_id(uint32_t idx);
  static bool       is_id_valid(const uint32_t& id);
  static crit_e     get_crit(const uint32_t& id);
  static presence_e get_presence(const uint32_t& id);
};

struct trp_info_resp_ies_container {
  bool                     crit_diagnostics_present = false;
  trp_info_list_trp_resp_l trp_info_list_trp_resp;
  crit_diagnostics_s       crit_diagnostics;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TRPInformationResponse ::= SEQUENCE
using trp_info_resp_s = elementary_procedure_option<trp_info_resp_ies_container>;

} // namespace nrppa
} // namespace asn1
