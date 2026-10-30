// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/*******************************************************************************
 *
 *                      3GPP TS ASN1 E2SM v07.00 (2025-10)
 *
 ******************************************************************************/

#pragma once

#include "ocudu/asn1/e2sm/e2sm_common_ies.h"

namespace asn1 {
namespace e2sm {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// AggregationFunctionType ::= ENUMERATED
struct aggregation_function_type_opts {
  enum options { min, max, avg, sum, /*...*/ nulltype } value;

  const char* to_string() const;
};
using aggregation_function_type_e = enumerated<aggregation_function_type_opts, true>;

// BinRangeValue ::= CHOICE
struct bin_range_value_c {
  struct types_opts {
    enum options { value_int, value_real, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  bin_range_value_c() = default;
  bin_range_value_c(const bin_range_value_c& other);
  bin_range_value_c& operator=(const bin_range_value_c& other);
  ~bin_range_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  int64_t& value_int()
  {
    assert_choice_type(types::value_int, type_, "BinRangeValue");
    return c.get<int64_t>();
  }
  real_s& value_real()
  {
    assert_choice_type(types::value_real, type_, "BinRangeValue");
    return c.get<real_s>();
  }
  const int64_t& value_int() const
  {
    assert_choice_type(types::value_int, type_, "BinRangeValue");
    return c.get<int64_t>();
  }
  const real_s& value_real() const
  {
    assert_choice_type(types::value_real, type_, "BinRangeValue");
    return c.get<real_s>();
  }
  int64_t& set_value_int();
  real_s&  set_value_real();

private:
  types                   type_;
  choice_buffer_t<real_s> c;

  void destroy_();
};

// BinRangeItem ::= SEQUENCE
struct bin_range_item_s {
  uint32_t          bin_idx = 1;
  bin_range_value_c start_value;
  bin_range_value_c end_value;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// BinRangeList ::= SEQUENCE (SIZE (1..65535)) OF BinRangeItem
using bin_range_list_l = dyn_array<bin_range_item_s>;

// BinRangeDefinition ::= SEQUENCE
struct bin_range_definition_s {
  bin_range_list_l bin_range_list_x;
  bin_range_list_l bin_range_list_y;
  bin_range_list_l bin_range_list_z;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementType ::= CHOICE
struct meas_type_c {
  struct types_opts {
    enum options { meas_name, meas_id, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  meas_type_c() = default;
  meas_type_c(const meas_type_c& other);
  meas_type_c& operator=(const meas_type_c& other);
  ~meas_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  printable_string<1, 150, true, true>& meas_name()
  {
    assert_choice_type(types::meas_name, type_, "MeasurementType");
    return c.get<printable_string<1, 150, true, true>>();
  }
  uint32_t& meas_id()
  {
    assert_choice_type(types::meas_id, type_, "MeasurementType");
    return c.get<uint32_t>();
  }
  const printable_string<1, 150, true, true>& meas_name() const
  {
    assert_choice_type(types::meas_name, type_, "MeasurementType");
    return c.get<printable_string<1, 150, true, true>>();
  }
  const uint32_t& meas_id() const
  {
    assert_choice_type(types::meas_id, type_, "MeasurementType");
    return c.get<uint32_t>();
  }
  printable_string<1, 150, true, true>& set_meas_name();
  uint32_t&                             set_meas_id();

private:
  types                                                 type_;
  choice_buffer_t<printable_string<1, 150, true, true>> c;

  void destroy_();
};

// DistMeasurementBinRangeItem ::= SEQUENCE
struct dist_meas_bin_range_item_s {
  meas_type_c            meas_type;
  bin_range_definition_s bin_range_def;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// DistMeasurementBinRangeList ::= SEQUENCE (SIZE (1..65535)) OF DistMeasurementBinRangeItem
using dist_meas_bin_range_list_l = dyn_array<dist_meas_bin_range_item_s>;

// MeasurementLabel ::= SEQUENCE
struct meas_label_s {
  struct no_label_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using no_label_e_ = enumerated<no_label_opts, true>;
  struct sum_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using sum_e_ = enumerated<sum_opts, true>;
  struct pre_label_override_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using pre_label_override_e_ = enumerated<pre_label_override_opts, true>;
  struct start_end_ind_opts {
    enum options { start, end, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using start_end_ind_e_ = enumerated<start_end_ind_opts, true>;
  struct min_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using min_e_ = enumerated<min_opts, true>;
  struct max_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using max_e_ = enumerated<max_opts, true>;
  struct avg_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using avg_e_ = enumerated<avg_opts, true>;

  // member variables
  bool                     no_label_present           = false;
  bool                     plmn_id_present            = false;
  bool                     slice_id_present           = false;
  bool                     five_qi_present            = false;
  bool                     qfi_present                = false;
  bool                     qci_present                = false;
  bool                     qc_imax_present            = false;
  bool                     qc_imin_present            = false;
  bool                     ar_pmax_present            = false;
  bool                     ar_pmin_present            = false;
  bool                     bitrate_range_present      = false;
  bool                     layer_mu_mimo_present      = false;
  bool                     sum_present                = false;
  bool                     dist_bin_x_present         = false;
  bool                     dist_bin_y_present         = false;
  bool                     dist_bin_z_present         = false;
  bool                     pre_label_override_present = false;
  bool                     start_end_ind_present      = false;
  bool                     min_present                = false;
  bool                     max_present                = false;
  bool                     avg_present                = false;
  no_label_e_              no_label;
  fixed_octstring<3, true> plmn_id;
  s_nssai_s                slice_id;
  uint16_t                 five_qi       = 0;
  uint8_t                  qfi           = 0;
  uint16_t                 qci           = 0;
  uint16_t                 qc_imax       = 0;
  uint16_t                 qc_imin       = 0;
  uint8_t                  ar_pmax       = 1;
  uint8_t                  ar_pmin       = 1;
  uint32_t                 bitrate_range = 1;
  uint32_t                 layer_mu_mimo = 1;
  sum_e_                   sum;
  uint32_t                 dist_bin_x = 1;
  uint32_t                 dist_bin_y = 1;
  uint32_t                 dist_bin_z = 1;
  pre_label_override_e_    pre_label_override;
  start_end_ind_e_         start_end_ind;
  min_e_                   min;
  max_e_                   max;
  avg_e_                   avg;
  // ...
  bool                ssb_idx_present              = false;
  bool                non_go_b_b_fmode_idx_present = false;
  bool                mimo_mode_idx_present        = false;
  uint32_t            ssb_idx                      = 1;
  uint32_t            non_go_b_b_fmode_idx         = 1;
  uint8_t             mimo_mode_idx                = 1;
  copy_ptr<cgi_c>     cell_global_id;
  copy_ptr<beam_id_c> beam_id;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LabelInfoItem ::= SEQUENCE
struct label_info_item_s {
  meas_label_s meas_label;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LabelInfoList ::= SEQUENCE (SIZE (1..2147483647)) OF LabelInfoItem
using label_info_list_l = dyn_array<label_info_item_s>;

// MeasValueTestCond-Expression ::= ENUMERATED
struct meas_value_test_cond_expression_opts {
  enum options { equal, greaterthan, lessthan, contains, present, /*...*/ nulltype } value;

  const char* to_string() const;
};
using meas_value_test_cond_expression_e = enumerated<meas_value_test_cond_expression_opts, true>;

// TestCond-Value ::= CHOICE
struct test_cond_value_c {
  struct types_opts {
    enum options {
      value_int,
      value_enum,
      value_bool,
      value_bit_s,
      value_oct_s,
      value_prt_s,
      /*...*/ value_real,
      nulltype
    } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true, 1>;

  // choice methods
  test_cond_value_c() = default;
  test_cond_value_c(const test_cond_value_c& other);
  test_cond_value_c& operator=(const test_cond_value_c& other);
  ~test_cond_value_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  int64_t& value_int()
  {
    assert_choice_type(types::value_int, type_, "TestCond-Value");
    return c.get<int64_t>();
  }
  int64_t& value_enum()
  {
    assert_choice_type(types::value_enum, type_, "TestCond-Value");
    return c.get<int64_t>();
  }
  bool& value_bool()
  {
    assert_choice_type(types::value_bool, type_, "TestCond-Value");
    return c.get<bool>();
  }
  unbounded_bitstring<false, true>& value_bit_s()
  {
    assert_choice_type(types::value_bit_s, type_, "TestCond-Value");
    return c.get<unbounded_bitstring<false, true>>();
  }
  unbounded_octstring<true>& value_oct_s()
  {
    assert_choice_type(types::value_oct_s, type_, "TestCond-Value");
    return c.get<unbounded_octstring<true>>();
  }
  printable_string<0, MAX_ASN_STRING_LENGTH, false, true>& value_prt_s()
  {
    assert_choice_type(types::value_prt_s, type_, "TestCond-Value");
    return c.get<printable_string<0, MAX_ASN_STRING_LENGTH, false, true>>();
  }
  real_s& value_real()
  {
    assert_choice_type(types::value_real, type_, "TestCond-Value");
    return c.get<real_s>();
  }
  const int64_t& value_int() const
  {
    assert_choice_type(types::value_int, type_, "TestCond-Value");
    return c.get<int64_t>();
  }
  const int64_t& value_enum() const
  {
    assert_choice_type(types::value_enum, type_, "TestCond-Value");
    return c.get<int64_t>();
  }
  const bool& value_bool() const
  {
    assert_choice_type(types::value_bool, type_, "TestCond-Value");
    return c.get<bool>();
  }
  const unbounded_bitstring<false, true>& value_bit_s() const
  {
    assert_choice_type(types::value_bit_s, type_, "TestCond-Value");
    return c.get<unbounded_bitstring<false, true>>();
  }
  const unbounded_octstring<true>& value_oct_s() const
  {
    assert_choice_type(types::value_oct_s, type_, "TestCond-Value");
    return c.get<unbounded_octstring<true>>();
  }
  const printable_string<0, MAX_ASN_STRING_LENGTH, false, true>& value_prt_s() const
  {
    assert_choice_type(types::value_prt_s, type_, "TestCond-Value");
    return c.get<printable_string<0, MAX_ASN_STRING_LENGTH, false, true>>();
  }
  const real_s& value_real() const
  {
    assert_choice_type(types::value_real, type_, "TestCond-Value");
    return c.get<real_s>();
  }
  int64_t&                                                 set_value_int();
  int64_t&                                                 set_value_enum();
  bool&                                                    set_value_bool();
  unbounded_bitstring<false, true>&                        set_value_bit_s();
  unbounded_octstring<true>&                               set_value_oct_s();
  printable_string<0, MAX_ASN_STRING_LENGTH, false, true>& set_value_prt_s();
  real_s&                                                  set_value_real();

private:
  types type_;
  choice_buffer_t<bool,
                  printable_string<0, MAX_ASN_STRING_LENGTH, false, true>,
                  real_s,
                  unbounded_bitstring<false, true>,
                  unbounded_octstring<true>>
      c;

  void destroy_();
};

// MeasValueReportCond ::= SEQUENCE
struct meas_value_report_cond_s {
  meas_value_test_cond_expression_e test_expr;
  test_cond_value_c                 test_value;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// LogicalOR ::= ENUMERATED
struct lc_or_opts {
  enum options { true_value, /*...*/ nulltype } value;

  const char* to_string() const;
};
using lc_or_e = enumerated<lc_or_opts, true>;

// MatchCondReportItem ::= SEQUENCE
struct match_cond_report_item_s {
  bool                     lc_or_present = false;
  meas_value_report_cond_s meas_value_report_cond;
  lc_or_e                  lc_or;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchCondReportList ::= SEQUENCE (SIZE (1..32768)) OF MatchCondReportItem
using match_cond_report_list_l = dyn_array<match_cond_report_item_s>;

// MeasurementInfoItem ::= SEQUENCE
struct meas_info_item_s {
  meas_type_c       meas_type;
  label_info_list_l label_info_list;
  // ...
  copy_ptr<match_cond_report_list_l> match_cond_report_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementInfoList ::= SEQUENCE (SIZE (1..65535)) OF MeasurementInfoItem
using meas_info_list_l = dyn_array<meas_info_item_s>;

// E2SM-KPM-ActionDefinition-Format1 ::= SEQUENCE
struct e2sm_kpm_action_definition_format1_s {
  bool             cell_global_id_present = false;
  meas_info_list_l meas_info_list;
  uint64_t         granul_period = 1;
  cgi_c            cell_global_id;
  // ...
  copy_ptr<dist_meas_bin_range_list_l> dist_meas_bin_range_info;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-ActionDefinition-Format2 ::= SEQUENCE
struct e2sm_kpm_action_definition_format2_s {
  ue_id_c                              ue_id;
  e2sm_kpm_action_definition_format1_s subscript_info;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// TestCond-Type ::= CHOICE
struct test_cond_type_c {
  struct gbr_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using gbr_e_ = enumerated<gbr_opts, true>;
  struct ambr_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using ambr_e_ = enumerated<ambr_opts, true>;
  struct is_stat_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using is_stat_e_ = enumerated<is_stat_opts, true>;
  struct is_cat_m_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using is_cat_m_e_ = enumerated<is_cat_m_opts, true>;
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
  struct ul_r_srp_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using ul_r_srp_e_ = enumerated<ul_r_srp_opts, true>;
  struct cqi_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using cqi_e_ = enumerated<cqi_opts, true>;
  struct five_qi_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using five_qi_e_ = enumerated<five_qi_opts, true>;
  struct qci_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using qci_e_ = enumerated<qci_opts, true>;
  struct snssai_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using snssai_e_ = enumerated<snssai_opts, true>;
  struct types_opts {
    enum options {
      gbr,
      ambr,
      is_stat,
      is_cat_m,
      rsrp,
      rsrq,
      /*...*/ ul_r_srp,
      cqi,
      five_qi,
      qci,
      snssai,
      nulltype
    } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts, true, 5>;

  // choice methods
  test_cond_type_c() = default;
  test_cond_type_c(const test_cond_type_c& other);
  test_cond_type_c& operator=(const test_cond_type_c& other);
  ~test_cond_type_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  gbr_e_& gbr()
  {
    assert_choice_type(types::gbr, type_, "TestCond-Type");
    return c.get<gbr_e_>();
  }
  ambr_e_& ambr()
  {
    assert_choice_type(types::ambr, type_, "TestCond-Type");
    return c.get<ambr_e_>();
  }
  is_stat_e_& is_stat()
  {
    assert_choice_type(types::is_stat, type_, "TestCond-Type");
    return c.get<is_stat_e_>();
  }
  is_cat_m_e_& is_cat_m()
  {
    assert_choice_type(types::is_cat_m, type_, "TestCond-Type");
    return c.get<is_cat_m_e_>();
  }
  rsrp_e_& rsrp()
  {
    assert_choice_type(types::rsrp, type_, "TestCond-Type");
    return c.get<rsrp_e_>();
  }
  rsrq_e_& rsrq()
  {
    assert_choice_type(types::rsrq, type_, "TestCond-Type");
    return c.get<rsrq_e_>();
  }
  ul_r_srp_e_& ul_r_srp()
  {
    assert_choice_type(types::ul_r_srp, type_, "TestCond-Type");
    return c.get<ul_r_srp_e_>();
  }
  cqi_e_& cqi()
  {
    assert_choice_type(types::cqi, type_, "TestCond-Type");
    return c.get<cqi_e_>();
  }
  five_qi_e_& five_qi()
  {
    assert_choice_type(types::five_qi, type_, "TestCond-Type");
    return c.get<five_qi_e_>();
  }
  qci_e_& qci()
  {
    assert_choice_type(types::qci, type_, "TestCond-Type");
    return c.get<qci_e_>();
  }
  snssai_e_& snssai()
  {
    assert_choice_type(types::snssai, type_, "TestCond-Type");
    return c.get<snssai_e_>();
  }
  const gbr_e_& gbr() const
  {
    assert_choice_type(types::gbr, type_, "TestCond-Type");
    return c.get<gbr_e_>();
  }
  const ambr_e_& ambr() const
  {
    assert_choice_type(types::ambr, type_, "TestCond-Type");
    return c.get<ambr_e_>();
  }
  const is_stat_e_& is_stat() const
  {
    assert_choice_type(types::is_stat, type_, "TestCond-Type");
    return c.get<is_stat_e_>();
  }
  const is_cat_m_e_& is_cat_m() const
  {
    assert_choice_type(types::is_cat_m, type_, "TestCond-Type");
    return c.get<is_cat_m_e_>();
  }
  const rsrp_e_& rsrp() const
  {
    assert_choice_type(types::rsrp, type_, "TestCond-Type");
    return c.get<rsrp_e_>();
  }
  const rsrq_e_& rsrq() const
  {
    assert_choice_type(types::rsrq, type_, "TestCond-Type");
    return c.get<rsrq_e_>();
  }
  const ul_r_srp_e_& ul_r_srp() const
  {
    assert_choice_type(types::ul_r_srp, type_, "TestCond-Type");
    return c.get<ul_r_srp_e_>();
  }
  const cqi_e_& cqi() const
  {
    assert_choice_type(types::cqi, type_, "TestCond-Type");
    return c.get<cqi_e_>();
  }
  const five_qi_e_& five_qi() const
  {
    assert_choice_type(types::five_qi, type_, "TestCond-Type");
    return c.get<five_qi_e_>();
  }
  const qci_e_& qci() const
  {
    assert_choice_type(types::qci, type_, "TestCond-Type");
    return c.get<qci_e_>();
  }
  const snssai_e_& snssai() const
  {
    assert_choice_type(types::snssai, type_, "TestCond-Type");
    return c.get<snssai_e_>();
  }
  gbr_e_&      set_gbr();
  ambr_e_&     set_ambr();
  is_stat_e_&  set_is_stat();
  is_cat_m_e_& set_is_cat_m();
  rsrp_e_&     set_rsrp();
  rsrq_e_&     set_rsrq();
  ul_r_srp_e_& set_ul_r_srp();
  cqi_e_&      set_cqi();
  five_qi_e_&  set_five_qi();
  qci_e_&      set_qci();
  snssai_e_&   set_snssai();

private:
  types               type_;
  pod_choice_buffer_t c;

  void destroy_();
};

// TestCond-Expression ::= ENUMERATED
struct test_cond_expression_opts {
  enum options { equal, greaterthan, lessthan, contains, present, /*...*/ nulltype } value;

  const char* to_string() const;
};
using test_cond_expression_e = enumerated<test_cond_expression_opts, true>;

// TestCondInfo ::= SEQUENCE
struct test_cond_info_s {
  bool                   test_expr_present  = false;
  bool                   test_value_present = false;
  test_cond_type_c       test_type;
  test_cond_expression_e test_expr;
  test_cond_value_c      test_value;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingCondItem-Choice ::= CHOICE
struct matching_cond_item_choice_c {
  struct types_opts {
    enum options { meas_label, test_cond_info, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  matching_cond_item_choice_c() = default;
  matching_cond_item_choice_c(const matching_cond_item_choice_c& other);
  matching_cond_item_choice_c& operator=(const matching_cond_item_choice_c& other);
  ~matching_cond_item_choice_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  meas_label_s& meas_label()
  {
    assert_choice_type(types::meas_label, type_, "MatchingCondItem-Choice");
    return c.get<meas_label_s>();
  }
  test_cond_info_s& test_cond_info()
  {
    assert_choice_type(types::test_cond_info, type_, "MatchingCondItem-Choice");
    return c.get<test_cond_info_s>();
  }
  const meas_label_s& meas_label() const
  {
    assert_choice_type(types::meas_label, type_, "MatchingCondItem-Choice");
    return c.get<meas_label_s>();
  }
  const test_cond_info_s& test_cond_info() const
  {
    assert_choice_type(types::test_cond_info, type_, "MatchingCondItem-Choice");
    return c.get<test_cond_info_s>();
  }
  meas_label_s&     set_meas_label();
  test_cond_info_s& set_test_cond_info();

private:
  types                                           type_;
  choice_buffer_t<meas_label_s, test_cond_info_s> c;

  void destroy_();
};

// MatchingCondItem ::= SEQUENCE
struct matching_cond_item_s {
  bool                        lc_or_present = false;
  matching_cond_item_choice_c matching_cond_choice;
  lc_or_e                     lc_or;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingCondList ::= SEQUENCE (SIZE (1..32768)) OF MatchingCondItem
using matching_cond_list_l = dyn_array<matching_cond_item_s>;

// MeasurementCondItem ::= SEQUENCE
struct meas_cond_item_s {
  meas_type_c          meas_type;
  matching_cond_list_l matching_cond;
  // ...
  copy_ptr<bin_range_definition_s> bin_range_def;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementCondList ::= SEQUENCE (SIZE (1..65535)) OF MeasurementCondItem
using meas_cond_list_l = dyn_array<meas_cond_item_s>;

// E2SM-KPM-ActionDefinition-Format3 ::= SEQUENCE
struct e2sm_kpm_action_definition_format3_s {
  bool             cell_global_id_present = false;
  meas_cond_list_l meas_cond_list;
  uint64_t         granul_period = 1;
  cgi_c            cell_global_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingUeCondPerSubItem ::= SEQUENCE
struct matching_ue_cond_per_sub_item_s {
  test_cond_info_s test_cond_info;
  // ...
  bool    lc_or_present = false;
  lc_or_e lc_or;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingUeCondPerSubList ::= SEQUENCE (SIZE (1..32768)) OF MatchingUeCondPerSubItem
using matching_ue_cond_per_sub_list_l = dyn_array<matching_ue_cond_per_sub_item_s>;

// E2SM-KPM-ActionDefinition-Format4 ::= SEQUENCE
struct e2sm_kpm_action_definition_format4_s {
  matching_ue_cond_per_sub_list_l      matching_ue_cond_list;
  e2sm_kpm_action_definition_format1_s sub_info;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingUEidPerSubItem ::= SEQUENCE
struct matching_ue_id_per_sub_item_s {
  ue_id_c ue_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingUEidPerSubList ::= SEQUENCE (SIZE (2..65535)) OF MatchingUEidPerSubItem
using matching_ue_id_per_sub_list_l = dyn_array<matching_ue_id_per_sub_item_s>;

// E2SM-KPM-ActionDefinition-Format5 ::= SEQUENCE
struct e2sm_kpm_action_definition_format5_s {
  matching_ue_id_per_sub_list_l        matching_ue_id_list;
  e2sm_kpm_action_definition_format1_s sub_info;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-ActionDefinition-Fundamental ::= SEQUENCE
struct e2sm_kpm_action_definition_fundamental_s {
  struct action_definition_fundamental_formats_c_ {
    struct types_opts {
      enum options {
        action_definition_format1,
        action_definition_format2,
        action_definition_format3,
        action_definition_format4,
        action_definition_format5,
        // ...
        nulltype
      } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts, true>;

    // choice methods
    action_definition_fundamental_formats_c_() = default;
    action_definition_fundamental_formats_c_(const action_definition_fundamental_formats_c_& other);
    action_definition_fundamental_formats_c_& operator=(const action_definition_fundamental_formats_c_& other);
    ~action_definition_fundamental_formats_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2sm_kpm_action_definition_format1_s& action_definition_format1()
    {
      assert_choice_type(types::action_definition_format1, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format1_s>();
    }
    e2sm_kpm_action_definition_format2_s& action_definition_format2()
    {
      assert_choice_type(types::action_definition_format2, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format2_s>();
    }
    e2sm_kpm_action_definition_format3_s& action_definition_format3()
    {
      assert_choice_type(types::action_definition_format3, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format3_s>();
    }
    e2sm_kpm_action_definition_format4_s& action_definition_format4()
    {
      assert_choice_type(types::action_definition_format4, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format4_s>();
    }
    e2sm_kpm_action_definition_format5_s& action_definition_format5()
    {
      assert_choice_type(types::action_definition_format5, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format5_s>();
    }
    const e2sm_kpm_action_definition_format1_s& action_definition_format1() const
    {
      assert_choice_type(types::action_definition_format1, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format1_s>();
    }
    const e2sm_kpm_action_definition_format2_s& action_definition_format2() const
    {
      assert_choice_type(types::action_definition_format2, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format2_s>();
    }
    const e2sm_kpm_action_definition_format3_s& action_definition_format3() const
    {
      assert_choice_type(types::action_definition_format3, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format3_s>();
    }
    const e2sm_kpm_action_definition_format4_s& action_definition_format4() const
    {
      assert_choice_type(types::action_definition_format4, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format4_s>();
    }
    const e2sm_kpm_action_definition_format5_s& action_definition_format5() const
    {
      assert_choice_type(types::action_definition_format5, type_, "actionDefinitionFundamental-formats");
      return c.get<e2sm_kpm_action_definition_format5_s>();
    }
    e2sm_kpm_action_definition_format1_s& set_action_definition_format1();
    e2sm_kpm_action_definition_format2_s& set_action_definition_format2();
    e2sm_kpm_action_definition_format3_s& set_action_definition_format3();
    e2sm_kpm_action_definition_format4_s& set_action_definition_format4();
    e2sm_kpm_action_definition_format5_s& set_action_definition_format5();

  private:
    types type_;
    choice_buffer_t<e2sm_kpm_action_definition_format1_s,
                    e2sm_kpm_action_definition_format2_s,
                    e2sm_kpm_action_definition_format3_s,
                    e2sm_kpm_action_definition_format4_s,
                    e2sm_kpm_action_definition_format5_s>
        c;

    void destroy_();
  };

  // member variables
  action_definition_fundamental_formats_c_ action_definition_fundamental_formats;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MultiReportActionDefinitionItem ::= SEQUENCE
struct multi_report_action_definition_item_s {
  bool                                     job_action_definition_present = false;
  bool                                     job_cell_global_id_present    = false;
  bool                                     job_ue_id_present             = false;
  uint32_t                                 job_id                        = 1;
  e2sm_kpm_action_definition_fundamental_s job_action_definition;
  cgi_c                                    job_cell_global_id;
  ue_id_c                                  job_ue_id;
  matching_ue_id_per_sub_list_l            job_matching_ue_id_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MultiReportActionDefinitionList ::= SEQUENCE (SIZE (1..65535)) OF MultiReportActionDefinitionItem
using multi_report_action_definition_list_l = dyn_array<multi_report_action_definition_item_s>;

// MultiReportApproach ::= ENUMERATED
struct multi_report_approach_opts {
  enum options { single, multiple, /*...*/ nulltype } value;

  const char* to_string() const;
};
using multi_report_approach_e = enumerated<multi_report_approach_opts, true>;

// E2SM-KPM-ActionDefinition-Format255 ::= SEQUENCE
struct e2sm_kpm_action_definition_format255_s {
  bool                                     common_action_definition_present = false;
  multi_report_action_definition_list_l    multi_report_action_definition_list;
  multi_report_approach_e                  multi_report_approach;
  e2sm_kpm_action_definition_fundamental_s common_action_definition;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-ActionDefinition ::= SEQUENCE
struct e2sm_kpm_action_definition_s {
  struct action_definition_formats_c_ {
    struct types_opts {
      enum options {
        action_definition_format1,
        action_definition_format2,
        action_definition_format3,
        // ...
        action_definition_format4,
        action_definition_format5,
        action_definition_format255,
        nulltype
      } value;
      typedef uint16_t number_type;

      const char* to_string() const;
      uint16_t    to_number() const;
    };
    using types = enumerated<types_opts, true, 3>;

    // choice methods
    action_definition_formats_c_() = default;
    action_definition_formats_c_(const action_definition_formats_c_& other);
    action_definition_formats_c_& operator=(const action_definition_formats_c_& other);
    ~action_definition_formats_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2sm_kpm_action_definition_format1_s& action_definition_format1()
    {
      assert_choice_type(types::action_definition_format1, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format1_s>();
    }
    e2sm_kpm_action_definition_format2_s& action_definition_format2()
    {
      assert_choice_type(types::action_definition_format2, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format2_s>();
    }
    e2sm_kpm_action_definition_format3_s& action_definition_format3()
    {
      assert_choice_type(types::action_definition_format3, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format3_s>();
    }
    e2sm_kpm_action_definition_format4_s& action_definition_format4()
    {
      assert_choice_type(types::action_definition_format4, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format4_s>();
    }
    e2sm_kpm_action_definition_format5_s& action_definition_format5()
    {
      assert_choice_type(types::action_definition_format5, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format5_s>();
    }
    e2sm_kpm_action_definition_format255_s& action_definition_format255()
    {
      assert_choice_type(types::action_definition_format255, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format255_s>();
    }
    const e2sm_kpm_action_definition_format1_s& action_definition_format1() const
    {
      assert_choice_type(types::action_definition_format1, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format1_s>();
    }
    const e2sm_kpm_action_definition_format2_s& action_definition_format2() const
    {
      assert_choice_type(types::action_definition_format2, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format2_s>();
    }
    const e2sm_kpm_action_definition_format3_s& action_definition_format3() const
    {
      assert_choice_type(types::action_definition_format3, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format3_s>();
    }
    const e2sm_kpm_action_definition_format4_s& action_definition_format4() const
    {
      assert_choice_type(types::action_definition_format4, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format4_s>();
    }
    const e2sm_kpm_action_definition_format5_s& action_definition_format5() const
    {
      assert_choice_type(types::action_definition_format5, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format5_s>();
    }
    const e2sm_kpm_action_definition_format255_s& action_definition_format255() const
    {
      assert_choice_type(types::action_definition_format255, type_, "actionDefinition-formats");
      return c.get<e2sm_kpm_action_definition_format255_s>();
    }
    e2sm_kpm_action_definition_format1_s&   set_action_definition_format1();
    e2sm_kpm_action_definition_format2_s&   set_action_definition_format2();
    e2sm_kpm_action_definition_format3_s&   set_action_definition_format3();
    e2sm_kpm_action_definition_format4_s&   set_action_definition_format4();
    e2sm_kpm_action_definition_format5_s&   set_action_definition_format5();
    e2sm_kpm_action_definition_format255_s& set_action_definition_format255();

  private:
    types type_;
    choice_buffer_t<e2sm_kpm_action_definition_format1_s,
                    e2sm_kpm_action_definition_format255_s,
                    e2sm_kpm_action_definition_format2_s,
                    e2sm_kpm_action_definition_format3_s,
                    e2sm_kpm_action_definition_format4_s,
                    e2sm_kpm_action_definition_format5_s>
        c;

    void destroy_();
  };

  // member variables
  int64_t                      ric_style_type = 0;
  action_definition_formats_c_ action_definition_formats;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-EventTriggerDefinition-Format1 ::= SEQUENCE
struct e2sm_kpm_event_trigger_definition_format1_s {
  uint64_t report_period = 1;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventMeasurementDefinition ::= SEQUENCE
struct event_meas_definition_s {
  bool              cell_global_id_present = false;
  meas_type_c       meas_type;
  label_info_list_l label_info_list;
  cgi_c             cell_global_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventWindowDefinition ::= SEQUENCE
struct event_win_definition_s {
  uint8_t                     win_size = 1;
  aggregation_function_type_e aggre_func;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventParameterMeasurement ::= SEQUENCE
struct event_param_meas_s {
  bool                    win_func_present = false;
  event_meas_definition_s meas_def;
  event_win_definition_s  win_func;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventParameterValue ::= SEQUENCE
struct event_param_value_s {
  bool              value_present = false;
  test_cond_value_c value;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// NextMathOperator ::= ENUMERATED
struct next_math_operator_opts {
  enum options { plus, minus, product, divide_by, /*...*/ nulltype } value;

  const char* to_string() const;
};
using next_math_operator_e = enumerated<next_math_operator_opts, true>;

// EventParameterItem ::= SEQUENCE
struct event_param_item_s {
  struct param_type_c_ {
    struct types_opts {
      enum options { meas, val, /*...*/ nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts, true>;

    // choice methods
    param_type_c_() = default;
    param_type_c_(const param_type_c_& other);
    param_type_c_& operator=(const param_type_c_& other);
    ~param_type_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    event_param_meas_s& meas()
    {
      assert_choice_type(types::meas, type_, "paramType");
      return c.get<event_param_meas_s>();
    }
    event_param_value_s& value()
    {
      assert_choice_type(types::val, type_, "paramType");
      return c.get<event_param_value_s>();
    }
    const event_param_meas_s& meas() const
    {
      assert_choice_type(types::meas, type_, "paramType");
      return c.get<event_param_meas_s>();
    }
    const event_param_value_s& value() const
    {
      assert_choice_type(types::val, type_, "paramType");
      return c.get<event_param_value_s>();
    }
    event_param_meas_s&  set_meas();
    event_param_value_s& set_value();

  private:
    types                                                    type_;
    choice_buffer_t<event_param_meas_s, event_param_value_s> c;

    void destroy_();
  };

  // member variables
  bool                 next_math_op_present = false;
  param_type_c_        param_type;
  next_math_operator_e next_math_op;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventParameterList ::= SEQUENCE (SIZE (1..65535)) OF EventParameterItem
using event_param_list_l = dyn_array<event_param_item_s>;

// EventExpression ::= SEQUENCE
struct event_expression_s {
  bool                        aggre_func_present = false;
  event_param_list_l          param_list;
  aggregation_function_type_e aggre_func;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventLogicalOperator ::= ENUMERATED
struct event_lc_operator_opts {
  enum options { equal, greaterthan, lessthan, /*...*/ nulltype } value;

  const char* to_string() const;
};
using event_lc_operator_e = enumerated<event_lc_operator_opts, true>;

// EventFormula ::= SEQUENCE
struct event_formula_s {
  event_expression_s  left_expr;
  event_lc_operator_e logic_op;
  event_expression_s  right_expr;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventTriggerItem ::= SEQUENCE
struct event_trigger_item_s {
  bool            event_eval_period_present = false;
  bool            lc_or_present             = false;
  event_formula_s event_cond;
  uint64_t        event_eval_period = 1;
  lc_or_e         lc_or;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// EventTrigerList ::= SEQUENCE (SIZE (1..65535)) OF EventTriggerItem
using event_triger_list_l = dyn_array<event_trigger_item_s>;

// E2SM-KPM-EventTriggerDefinition-Format2 ::= SEQUENCE
struct e2sm_kpm_event_trigger_definition_format2_s {
  event_triger_list_l event_trigger_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-EventTriggerDefinition ::= SEQUENCE
struct e2sm_kpm_event_trigger_definition_s {
  struct event_definition_formats_c_ {
    struct types_opts {
      enum options { event_definition_format1, /*...*/ event_definition_format2, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts, true, 1>;

    // choice methods
    event_definition_formats_c_() = default;
    event_definition_formats_c_(const event_definition_formats_c_& other);
    event_definition_formats_c_& operator=(const event_definition_formats_c_& other);
    ~event_definition_formats_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2sm_kpm_event_trigger_definition_format1_s& event_definition_format1()
    {
      assert_choice_type(types::event_definition_format1, type_, "eventDefinition-formats");
      return c.get<e2sm_kpm_event_trigger_definition_format1_s>();
    }
    e2sm_kpm_event_trigger_definition_format2_s& event_definition_format2()
    {
      assert_choice_type(types::event_definition_format2, type_, "eventDefinition-formats");
      return c.get<e2sm_kpm_event_trigger_definition_format2_s>();
    }
    const e2sm_kpm_event_trigger_definition_format1_s& event_definition_format1() const
    {
      assert_choice_type(types::event_definition_format1, type_, "eventDefinition-formats");
      return c.get<e2sm_kpm_event_trigger_definition_format1_s>();
    }
    const e2sm_kpm_event_trigger_definition_format2_s& event_definition_format2() const
    {
      assert_choice_type(types::event_definition_format2, type_, "eventDefinition-formats");
      return c.get<e2sm_kpm_event_trigger_definition_format2_s>();
    }
    e2sm_kpm_event_trigger_definition_format1_s& set_event_definition_format1();
    e2sm_kpm_event_trigger_definition_format2_s& set_event_definition_format2();

  private:
    types                                                                                                     type_;
    choice_buffer_t<e2sm_kpm_event_trigger_definition_format1_s, e2sm_kpm_event_trigger_definition_format2_s> c;

    void destroy_();
  };

  // member variables
  event_definition_formats_c_ event_definition_formats;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-IndicationHeader-Format1 ::= SEQUENCE
struct e2sm_kpm_ind_hdr_format1_s {
  bool                                 file_formatversion_present = false;
  bool                                 sender_name_present        = false;
  bool                                 sender_type_present        = false;
  bool                                 vendor_name_present        = false;
  fixed_octstring<8, true>             collet_start_time;
  printable_string<0, 15, true, true>  file_formatversion;
  printable_string<0, 400, true, true> sender_name;
  printable_string<0, 8, true, true>   sender_type;
  printable_string<0, 32, true, true>  vendor_name;
  // ...
  bool     job_id_present = false;
  uint32_t job_id         = 1;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-IndicationHeader ::= SEQUENCE
struct e2sm_kpm_ind_hdr_s {
  struct ind_hdr_formats_c_ {
    struct types_opts {
      enum options { ind_hdr_format1, /*...*/ nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts, true>;

    // choice methods
    types         type() const { return types::ind_hdr_format1; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2sm_kpm_ind_hdr_format1_s&       ind_hdr_format1() { return c; }
    const e2sm_kpm_ind_hdr_format1_s& ind_hdr_format1() const { return c; }

  private:
    e2sm_kpm_ind_hdr_format1_s c;
  };

  // member variables
  ind_hdr_formats_c_ ind_hdr_formats;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementRecordItem ::= CHOICE
struct meas_record_item_c {
  struct types_opts {
    enum options { integer, real, no_value, /*...*/ not_satisfied, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true, 1>;

  // choice methods
  meas_record_item_c() = default;
  meas_record_item_c(const meas_record_item_c& other);
  meas_record_item_c& operator=(const meas_record_item_c& other);
  ~meas_record_item_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint64_t& integer()
  {
    assert_choice_type(types::integer, type_, "MeasurementRecordItem");
    return c.get<uint64_t>();
  }
  real_s& real()
  {
    assert_choice_type(types::real, type_, "MeasurementRecordItem");
    return c.get<real_s>();
  }
  const uint64_t& integer() const
  {
    assert_choice_type(types::integer, type_, "MeasurementRecordItem");
    return c.get<uint64_t>();
  }
  const real_s& real() const
  {
    assert_choice_type(types::real, type_, "MeasurementRecordItem");
    return c.get<real_s>();
  }
  uint64_t& set_integer();
  real_s&   set_real();
  void      set_no_value();
  void      set_not_satisfied();

private:
  types                   type_;
  choice_buffer_t<real_s> c;

  void destroy_();
};

// MeasurementRecord ::= SEQUENCE (SIZE (1..2147483647)) OF MeasurementRecordItem
using meas_record_l = dyn_array<meas_record_item_c>;

// MeasurementDataItem ::= SEQUENCE
struct meas_data_item_s {
  struct incomplete_flag_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using incomplete_flag_e_ = enumerated<incomplete_flag_opts, true>;

  // member variables
  bool               incomplete_flag_present = false;
  meas_record_l      meas_record;
  incomplete_flag_e_ incomplete_flag;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementData ::= SEQUENCE (SIZE (1..65535)) OF MeasurementDataItem
using meas_data_l = dyn_array<meas_data_item_s>;

// E2SM-KPM-IndicationMessage-Format1 ::= SEQUENCE
struct e2sm_kpm_ind_msg_format1_s {
  bool             granul_period_present = false;
  meas_data_l      meas_data;
  meas_info_list_l meas_info_list;
  uint64_t         granul_period = 1;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingUEidItem ::= SEQUENCE
struct matching_ue_id_item_s {
  ue_id_c ue_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingUEidList ::= SEQUENCE (SIZE (1..65535)) OF MatchingUEidItem
using matching_ue_id_list_l = dyn_array<matching_ue_id_item_s>;

// MatchingUEidItem-PerGP ::= SEQUENCE
struct matching_ue_id_item_per_gp_s {
  ue_id_c ue_id;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingUEidList-PerGP ::= SEQUENCE (SIZE (1..65535)) OF MatchingUEidItem-PerGP
using matching_ue_id_list_per_gp_l = dyn_array<matching_ue_id_item_per_gp_s>;

// MatchingUEidPerGP-Item ::= SEQUENCE
struct matching_ue_id_per_gp_item_s {
  struct matched_per_gp_c_ {
    struct no_ue_matched_opts {
      enum options { true_value, /*...*/ nulltype } value;

      const char* to_string() const;
    };
    using no_ue_matched_e_ = enumerated<no_ue_matched_opts, true>;
    struct types_opts {
      enum options { no_ue_matched, one_or_more_ue_matched, /*...*/ nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts, true>;

    // choice methods
    matched_per_gp_c_() = default;
    matched_per_gp_c_(const matched_per_gp_c_& other);
    matched_per_gp_c_& operator=(const matched_per_gp_c_& other);
    ~matched_per_gp_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    no_ue_matched_e_& no_ue_matched()
    {
      assert_choice_type(types::no_ue_matched, type_, "matchedPerGP");
      return c.get<no_ue_matched_e_>();
    }
    matching_ue_id_list_per_gp_l& one_or_more_ue_matched()
    {
      assert_choice_type(types::one_or_more_ue_matched, type_, "matchedPerGP");
      return c.get<matching_ue_id_list_per_gp_l>();
    }
    const no_ue_matched_e_& no_ue_matched() const
    {
      assert_choice_type(types::no_ue_matched, type_, "matchedPerGP");
      return c.get<no_ue_matched_e_>();
    }
    const matching_ue_id_list_per_gp_l& one_or_more_ue_matched() const
    {
      assert_choice_type(types::one_or_more_ue_matched, type_, "matchedPerGP");
      return c.get<matching_ue_id_list_per_gp_l>();
    }
    no_ue_matched_e_&             set_no_ue_matched();
    matching_ue_id_list_per_gp_l& set_one_or_more_ue_matched();

  private:
    types                                         type_;
    choice_buffer_t<matching_ue_id_list_per_gp_l> c;

    void destroy_();
  };

  // member variables
  matched_per_gp_c_ matched_per_gp;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MatchingUEidPerGP ::= SEQUENCE (SIZE (1..65535)) OF MatchingUEidPerGP-Item
using matching_ue_id_per_gp_l = dyn_array<matching_ue_id_per_gp_item_s>;

// MeasurementCondUEidItem ::= SEQUENCE
struct meas_cond_ue_id_item_s {
  meas_type_c           meas_type;
  matching_cond_list_l  matching_cond;
  matching_ue_id_list_l matching_ue_id_list;
  // ...
  copy_ptr<matching_ue_id_per_gp_l> matching_ue_id_per_gp;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementCondUEidList ::= SEQUENCE (SIZE (1..65535)) OF MeasurementCondUEidItem
using meas_cond_ue_id_list_l = dyn_array<meas_cond_ue_id_item_s>;

// E2SM-KPM-IndicationMessage-Format2 ::= SEQUENCE
struct e2sm_kpm_ind_msg_format2_s {
  bool                   granul_period_present = false;
  meas_data_l            meas_data;
  meas_cond_ue_id_list_l meas_cond_ue_id_list;
  uint64_t               granul_period = 1;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEMeasurementReportItem ::= SEQUENCE
struct ue_meas_report_item_s {
  ue_id_c                    ue_id;
  e2sm_kpm_ind_msg_format1_s meas_report;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// UEMeasurementReportList ::= SEQUENCE (SIZE (1..65535)) OF UEMeasurementReportItem
using ue_meas_report_list_l = dyn_array<ue_meas_report_item_s>;

// E2SM-KPM-IndicationMessage-Format3 ::= SEQUENCE
struct e2sm_kpm_ind_msg_format3_s {
  ue_meas_report_list_l ue_meas_report_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-IndicationMessage-Fundamental ::= SEQUENCE
struct e2sm_kpm_ind_msg_fundamental_s {
  struct ind_msg_fundamental_formats_c_ {
    struct types_opts {
      enum options { ind_msg_format1, ind_msg_format2, ind_msg_format3, /*...*/ nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts, true>;

    // choice methods
    ind_msg_fundamental_formats_c_() = default;
    ind_msg_fundamental_formats_c_(const ind_msg_fundamental_formats_c_& other);
    ind_msg_fundamental_formats_c_& operator=(const ind_msg_fundamental_formats_c_& other);
    ~ind_msg_fundamental_formats_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2sm_kpm_ind_msg_format1_s& ind_msg_format1()
    {
      assert_choice_type(types::ind_msg_format1, type_, "indicationMessageFundamental-formats");
      return c.get<e2sm_kpm_ind_msg_format1_s>();
    }
    e2sm_kpm_ind_msg_format2_s& ind_msg_format2()
    {
      assert_choice_type(types::ind_msg_format2, type_, "indicationMessageFundamental-formats");
      return c.get<e2sm_kpm_ind_msg_format2_s>();
    }
    e2sm_kpm_ind_msg_format3_s& ind_msg_format3()
    {
      assert_choice_type(types::ind_msg_format3, type_, "indicationMessageFundamental-formats");
      return c.get<e2sm_kpm_ind_msg_format3_s>();
    }
    const e2sm_kpm_ind_msg_format1_s& ind_msg_format1() const
    {
      assert_choice_type(types::ind_msg_format1, type_, "indicationMessageFundamental-formats");
      return c.get<e2sm_kpm_ind_msg_format1_s>();
    }
    const e2sm_kpm_ind_msg_format2_s& ind_msg_format2() const
    {
      assert_choice_type(types::ind_msg_format2, type_, "indicationMessageFundamental-formats");
      return c.get<e2sm_kpm_ind_msg_format2_s>();
    }
    const e2sm_kpm_ind_msg_format3_s& ind_msg_format3() const
    {
      assert_choice_type(types::ind_msg_format3, type_, "indicationMessageFundamental-formats");
      return c.get<e2sm_kpm_ind_msg_format3_s>();
    }
    e2sm_kpm_ind_msg_format1_s& set_ind_msg_format1();
    e2sm_kpm_ind_msg_format2_s& set_ind_msg_format2();
    e2sm_kpm_ind_msg_format3_s& set_ind_msg_format3();

  private:
    types                                                                                               type_;
    choice_buffer_t<e2sm_kpm_ind_msg_format1_s, e2sm_kpm_ind_msg_format2_s, e2sm_kpm_ind_msg_format3_s> c;

    void destroy_();
  };

  // member variables
  ind_msg_fundamental_formats_c_ ind_msg_fundamental_formats;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MultiReportIndicationMessageItem ::= SEQUENCE
struct multi_report_ind_msg_item_s {
  uint32_t                       job_id = 1;
  e2sm_kpm_ind_msg_fundamental_s ind_msg;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MultiReportIndicationMessageList ::= SEQUENCE (SIZE (1..65535)) OF MultiReportIndicationMessageItem
using multi_report_ind_msg_list_l = dyn_array<multi_report_ind_msg_item_s>;

// E2SM-KPM-IndicationMessage-Format255 ::= SEQUENCE
struct e2sm_kpm_ind_msg_format255_s {
  multi_report_ind_msg_list_l multi_report_ind_msg_list;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-IndicationMessage ::= SEQUENCE
struct e2sm_kpm_ind_msg_s {
  struct ind_msg_formats_c_ {
    struct types_opts {
      enum options { ind_msg_format1, ind_msg_format2, /*...*/ ind_msg_format3, ind_msg_format255, nulltype } value;
      typedef uint16_t number_type;

      const char* to_string() const;
      uint16_t    to_number() const;
    };
    using types = enumerated<types_opts, true, 2>;

    // choice methods
    ind_msg_formats_c_() = default;
    ind_msg_formats_c_(const ind_msg_formats_c_& other);
    ind_msg_formats_c_& operator=(const ind_msg_formats_c_& other);
    ~ind_msg_formats_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    e2sm_kpm_ind_msg_format1_s& ind_msg_format1()
    {
      assert_choice_type(types::ind_msg_format1, type_, "indicationMessage-formats");
      return c.get<e2sm_kpm_ind_msg_format1_s>();
    }
    e2sm_kpm_ind_msg_format2_s& ind_msg_format2()
    {
      assert_choice_type(types::ind_msg_format2, type_, "indicationMessage-formats");
      return c.get<e2sm_kpm_ind_msg_format2_s>();
    }
    e2sm_kpm_ind_msg_format3_s& ind_msg_format3()
    {
      assert_choice_type(types::ind_msg_format3, type_, "indicationMessage-formats");
      return c.get<e2sm_kpm_ind_msg_format3_s>();
    }
    e2sm_kpm_ind_msg_format255_s& ind_msg_format255()
    {
      assert_choice_type(types::ind_msg_format255, type_, "indicationMessage-formats");
      return c.get<e2sm_kpm_ind_msg_format255_s>();
    }
    const e2sm_kpm_ind_msg_format1_s& ind_msg_format1() const
    {
      assert_choice_type(types::ind_msg_format1, type_, "indicationMessage-formats");
      return c.get<e2sm_kpm_ind_msg_format1_s>();
    }
    const e2sm_kpm_ind_msg_format2_s& ind_msg_format2() const
    {
      assert_choice_type(types::ind_msg_format2, type_, "indicationMessage-formats");
      return c.get<e2sm_kpm_ind_msg_format2_s>();
    }
    const e2sm_kpm_ind_msg_format3_s& ind_msg_format3() const
    {
      assert_choice_type(types::ind_msg_format3, type_, "indicationMessage-formats");
      return c.get<e2sm_kpm_ind_msg_format3_s>();
    }
    const e2sm_kpm_ind_msg_format255_s& ind_msg_format255() const
    {
      assert_choice_type(types::ind_msg_format255, type_, "indicationMessage-formats");
      return c.get<e2sm_kpm_ind_msg_format255_s>();
    }
    e2sm_kpm_ind_msg_format1_s&   set_ind_msg_format1();
    e2sm_kpm_ind_msg_format2_s&   set_ind_msg_format2();
    e2sm_kpm_ind_msg_format3_s&   set_ind_msg_format3();
    e2sm_kpm_ind_msg_format255_s& set_ind_msg_format255();

  private:
    types type_;
    choice_buffer_t<e2sm_kpm_ind_msg_format1_s,
                    e2sm_kpm_ind_msg_format255_s,
                    e2sm_kpm_ind_msg_format2_s,
                    e2sm_kpm_ind_msg_format3_s>
        c;

    void destroy_();
  };

  // member variables
  ind_msg_formats_c_ ind_msg_formats;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RIC-EventTriggerStyle-Item ::= SEQUENCE
struct ric_event_trigger_style_item_s {
  int64_t                              ric_event_trigger_style_type = 0;
  printable_string<1, 150, true, true> ric_event_trigger_style_name;
  int64_t                              ric_event_trigger_format_type = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementInfo-Action-Item ::= SEQUENCE
struct meas_info_action_item_s {
  bool                                 meas_id_present = false;
  printable_string<1, 150, true, true> meas_name;
  uint32_t                             meas_id = 1;
  // ...
  copy_ptr<bin_range_definition_s> bin_range_def;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MeasurementInfo-Action-List ::= SEQUENCE (SIZE (1..65535)) OF MeasurementInfo-Action-Item
using meas_info_action_list_l = dyn_array<meas_info_action_item_s>;

// RIC-ReportStyle-Item ::= SEQUENCE
struct ric_report_style_item_s {
  int64_t                              ric_report_style_type = 0;
  printable_string<1, 150, true, true> ric_report_style_name;
  int64_t                              ric_action_format_type = 0;
  meas_info_action_list_l              meas_info_action_list;
  int64_t                              ric_ind_hdr_format_type = 0;
  int64_t                              ric_ind_msg_format_type = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RIC-FundamentalLevelReportStyle-Item ::= SEQUENCE
struct ric_fundamental_level_report_style_item_s {
  int64_t ric_report_style_type = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RIC-IntegratedReportStyle-Item ::= SEQUENCE
struct ric_integrated_report_style_item_s {
  using ric_fundamental_level_report_style_list_l_ = dyn_array<ric_fundamental_level_report_style_item_s>;
  struct ric_multiple_individual_report_support_opts {
    enum options { true_value, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using ric_multiple_individual_report_support_e_ = enumerated<ric_multiple_individual_report_support_opts, true>;

  // member variables
  bool                                       ric_multiple_individual_report_support_present = false;
  int64_t                                    ric_report_style_type                          = 0;
  printable_string<1, 150, true, true>       ric_report_style_name;
  int64_t                                    ric_action_format_type  = 0;
  int64_t                                    ric_ind_hdr_format_type = 0;
  int64_t                                    ric_ind_msg_format_type = 0;
  ric_fundamental_level_report_style_list_l_ ric_fundamental_level_report_style_list;
  ric_multiple_individual_report_support_e_  ric_multiple_individual_report_support;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// E2SM-KPM-RANfunction-Description ::= SEQUENCE
struct e2sm_kpm_ra_nfunction_description_s {
  using ric_event_trigger_style_list_l_     = dyn_array<ric_event_trigger_style_item_s>;
  using ric_report_style_list_l_            = dyn_array<ric_report_style_item_s>;
  using ric_integrated_report_style_list_l_ = dyn_array<ric_integrated_report_style_item_s>;

  // member variables
  ra_nfunction_name_s             ran_function_name;
  ric_event_trigger_style_list_l_ ric_event_trigger_style_list;
  ric_report_style_list_l_        ric_report_style_list;
  // ...
  copy_ptr<ric_integrated_report_style_list_l_> ric_integrated_report_style_list;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

} // namespace e2sm
} // namespace asn1
