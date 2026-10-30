// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

/*******************************************************************************
 *
 *                    3GPP TS ASN1 RRC NR v18.8.0 (2025-12)
 *
 ******************************************************************************/

#pragma once

#include "ocudu/asn1/rrc_nr/radio_bearer_cfg.h"
#include "ocudu/asn1/rrc_nr/serving_cell.h"

namespace asn1 {
namespace rrc_nr {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

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

// ReportInterval ::= ENUMERATED
struct report_interv_opts {
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
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using report_interv_e = enumerated<report_interv_opts>;

// SL-MeasReportQuantity-r16 ::= CHOICE
struct sl_meas_report_quant_r16_c {
  struct types_opts {
    enum options { sl_rsrp_r16, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  types         type() const { return types::sl_rsrp_r16; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  bool&       sl_rsrp_r16() { return c; }
  const bool& sl_rsrp_r16() const { return c; }

private:
  bool c;
};

// SL-MeasTriggerQuantity-r16 ::= CHOICE
struct sl_meas_trigger_quant_r16_c {
  struct types_opts {
    enum options { sl_rsrp_r16, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  types         type() const { return types::sl_rsrp_r16; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t&       sl_rsrp_r16() { return c; }
  const uint8_t& sl_rsrp_r16() const { return c; }

private:
  uint8_t c;
};

// SN-FieldLengthAM ::= ENUMERATED
struct sn_field_len_am_opts {
  enum options { size12, size18, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using sn_field_len_am_e = enumerated<sn_field_len_am_opts>;

// T-PollRetransmit ::= ENUMERATED
struct t_poll_retx_opts {
  enum options {
    ms5,
    ms10,
    ms15,
    ms20,
    ms25,
    ms30,
    ms35,
    ms40,
    ms45,
    ms50,
    ms55,
    ms60,
    ms65,
    ms70,
    ms75,
    ms80,
    ms85,
    ms90,
    ms95,
    ms100,
    ms105,
    ms110,
    ms115,
    ms120,
    ms125,
    ms130,
    ms135,
    ms140,
    ms145,
    ms150,
    ms155,
    ms160,
    ms165,
    ms170,
    ms175,
    ms180,
    ms185,
    ms190,
    ms195,
    ms200,
    ms205,
    ms210,
    ms215,
    ms220,
    ms225,
    ms230,
    ms235,
    ms240,
    ms245,
    ms250,
    ms300,
    ms350,
    ms400,
    ms450,
    ms500,
    ms800,
    ms1000,
    ms2000,
    ms4000,
    ms1_v1610,
    ms2_v1610,
    ms3_v1610,
    ms4_v1610,
    spare1,
    nulltype
  } value;
  typedef uint16_t number_type;

  const char* to_string() const;
  uint16_t    to_number() const;
};
using t_poll_retx_e = enumerated<t_poll_retx_opts>;

// PollPDU ::= ENUMERATED
struct poll_pdu_opts {
  enum options {
    p4,
    p8,
    p16,
    p32,
    p64,
    p128,
    p256,
    p512,
    p1024,
    p2048,
    p4096,
    p6144,
    p8192,
    p12288,
    p16384,
    p20480,
    p24576,
    p28672,
    p32768,
    p40960,
    p49152,
    p57344,
    p65536,
    infinity,
    spare8,
    spare7,
    spare6,
    spare5,
    spare4,
    spare3,
    spare2,
    spare1,
    nulltype
  } value;
  typedef int32_t number_type;

  const char* to_string() const;
  int32_t     to_number() const;
};
using poll_pdu_e = enumerated<poll_pdu_opts>;

// PollByte ::= ENUMERATED
struct poll_byte_opts {
  enum options {
    kb1,
    kb2,
    kb5,
    kb8,
    kb10,
    kb15,
    kb25,
    kb50,
    kb75,
    kb100,
    kb125,
    kb250,
    kb375,
    kb500,
    kb750,
    kb1000,
    kb1250,
    kb1500,
    kb2000,
    kb3000,
    kb4000,
    kb4500,
    kb5000,
    kb5500,
    kb6000,
    kb6500,
    kb7000,
    kb7500,
    mb8,
    mb9,
    mb10,
    mb11,
    mb12,
    mb13,
    mb14,
    mb15,
    mb16,
    mb17,
    mb18,
    mb20,
    mb25,
    mb30,
    mb40,
    infinity,
    spare20,
    spare19,
    spare18,
    spare17,
    spare16,
    spare15,
    spare14,
    spare13,
    spare12,
    spare11,
    spare10,
    spare9,
    spare8,
    spare7,
    spare6,
    spare5,
    spare4,
    spare3,
    spare2,
    spare1,
    nulltype
  } value;
  typedef int32_t number_type;

  const char* to_string() const;
  int32_t     to_number() const;
};
using poll_byte_e = enumerated<poll_byte_opts>;

// SN-FieldLengthUM ::= ENUMERATED
struct sn_field_len_um_opts {
  enum options { size6, size12, nulltype } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using sn_field_len_um_e = enumerated<sn_field_len_um_opts>;

// BSR-Config ::= SEQUENCE
struct bsr_cfg_s {
  struct periodic_bsr_timer_opts {
    enum options {
      sf1,
      sf5,
      sf10,
      sf16,
      sf20,
      sf32,
      sf40,
      sf64,
      sf80,
      sf128,
      sf160,
      sf320,
      sf640,
      sf1280,
      sf2560,
      infinity,
      nulltype
    } value;
    typedef int16_t number_type;

    const char* to_string() const;
    int16_t     to_number() const;
  };
  using periodic_bsr_timer_e_ = enumerated<periodic_bsr_timer_opts>;
  struct retx_bsr_timer_opts {
    enum options {
      sf10,
      sf20,
      sf40,
      sf80,
      sf160,
      sf320,
      sf640,
      sf1280,
      sf2560,
      sf5120,
      sf10240,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using retx_bsr_timer_e_ = enumerated<retx_bsr_timer_opts>;
  struct lc_ch_sr_delay_timer_opts {
    enum options { sf20, sf40, sf64, sf128, sf512, sf1024, sf2560, spare1, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using lc_ch_sr_delay_timer_e_ = enumerated<lc_ch_sr_delay_timer_opts>;

  // member variables
  bool                    lc_ch_sr_delay_timer_present = false;
  periodic_bsr_timer_e_   periodic_bsr_timer;
  retx_bsr_timer_e_       retx_bsr_timer;
  lc_ch_sr_delay_timer_e_ lc_ch_sr_delay_timer;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCReject-IEs ::= SEQUENCE
struct rrc_reject_ies_s {
  bool          wait_time_present    = false;
  bool          non_crit_ext_present = false;
  uint8_t       wait_time            = 1;
  dyn_octstring late_non_crit_ext;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCReject ::= SEQUENCE
struct rrc_reject_s {
  struct crit_exts_c_ {
    struct types_opts {
      enum options { rrc_reject, crit_exts_future, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    crit_exts_c_() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    rrc_reject_ies_s& rrc_reject()
    {
      assert_choice_type(types::rrc_reject, type_, "criticalExtensions");
      return c;
    }
    const rrc_reject_ies_s& rrc_reject() const
    {
      assert_choice_type(types::rrc_reject, type_, "criticalExtensions");
      return c;
    }
    rrc_reject_ies_s& set_rrc_reject();
    void              set_crit_exts_future();

  private:
    types            type_;
    rrc_reject_ies_s c;
  };

  // member variables
  crit_exts_c_ crit_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// MAC-MainConfigSL-r16 ::= SEQUENCE
struct mac_main_cfg_sl_r16_s {
  bool      sl_bsr_cfg_r16_present              = false;
  bool      ul_prioritization_thres_r16_present = false;
  bool      sl_prioritization_thres_r16_present = false;
  bsr_cfg_s sl_bsr_cfg_r16;
  uint8_t   ul_prioritization_thres_r16 = 1;
  uint8_t   sl_prioritization_thres_r16 = 1;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PeriodCG-r16 ::= CHOICE
struct sl_period_cg_r16_c {
  struct sl_period_cg1_r16_opts {
    enum options {
      ms100,
      ms200,
      ms300,
      ms400,
      ms500,
      ms600,
      ms700,
      ms800,
      ms900,
      ms1000,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_period_cg1_r16_e_ = enumerated<sl_period_cg1_r16_opts>;
  struct types_opts {
    enum options { sl_period_cg1_r16, sl_period_cg2_r16, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  sl_period_cg_r16_c() = default;
  sl_period_cg_r16_c(const sl_period_cg_r16_c& other);
  sl_period_cg_r16_c& operator=(const sl_period_cg_r16_c& other);
  ~sl_period_cg_r16_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  sl_period_cg1_r16_e_& sl_period_cg1_r16()
  {
    assert_choice_type(types::sl_period_cg1_r16, type_, "SL-PeriodCG-r16");
    return c.get<sl_period_cg1_r16_e_>();
  }
  uint8_t& sl_period_cg2_r16()
  {
    assert_choice_type(types::sl_period_cg2_r16, type_, "SL-PeriodCG-r16");
    return c.get<uint8_t>();
  }
  const sl_period_cg1_r16_e_& sl_period_cg1_r16() const
  {
    assert_choice_type(types::sl_period_cg1_r16, type_, "SL-PeriodCG-r16");
    return c.get<sl_period_cg1_r16_e_>();
  }
  const uint8_t& sl_period_cg2_r16() const
  {
    assert_choice_type(types::sl_period_cg2_r16, type_, "SL-PeriodCG-r16");
    return c.get<uint8_t>();
  }
  sl_period_cg1_r16_e_& set_sl_period_cg1_r16();
  uint8_t&              set_sl_period_cg2_r16();

private:
  types               type_;
  pod_choice_buffer_t c;

  void destroy_();
};

// SL-CG-MaxTransNum-r16 ::= SEQUENCE
struct sl_cg_max_trans_num_r16_s {
  uint8_t sl_prio_r16          = 1;
  uint8_t sl_max_trans_num_r16 = 1;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-CG-MaxTransNumList-r16 ::= SEQUENCE (SIZE (1..8)) OF SL-CG-MaxTransNum-r16
using sl_cg_max_trans_num_list_r16_l = dyn_array<sl_cg_max_trans_num_r16_s>;

// SL-ConfiguredGrantConfig-r16 ::= SEQUENCE
struct sl_cfg_grant_cfg_r16_s {
  struct rrc_cfg_sidelink_grant_r16_s_ {
    bool     sl_time_res_cg_type1_r16_present       = false;
    bool     sl_start_subch_cg_type1_r16_present    = false;
    bool     sl_freq_res_cg_type1_r16_present       = false;
    bool     sl_time_offset_cg_type1_r16_present    = false;
    bool     sl_n1_pucch_an_r16_present             = false;
    bool     sl_psfch_to_pucch_cg_type1_r16_present = false;
    bool     sl_res_pool_id_r16_present             = false;
    bool     sl_time_ref_sfn_type1_r16_present      = false;
    uint16_t sl_time_res_cg_type1_r16               = 0;
    uint8_t  sl_start_subch_cg_type1_r16            = 0;
    uint16_t sl_freq_res_cg_type1_r16               = 0;
    uint16_t sl_time_offset_cg_type1_r16            = 0;
    uint8_t  sl_n1_pucch_an_r16                     = 0;
    uint8_t  sl_psfch_to_pucch_cg_type1_r16         = 0;
    uint8_t  sl_res_pool_id_r16                     = 1;
  };

  // member variables
  bool                           sl_period_cg_r16_present            = false;
  bool                           sl_nr_of_harq_processes_r16_present = false;
  bool                           sl_harq_proc_id_offset_r16_present  = false;
  bool                           rrc_cfg_sidelink_grant_r16_present  = false;
  uint8_t                        sl_cfg_idx_cg_r16                   = 0;
  sl_period_cg_r16_c             sl_period_cg_r16;
  uint8_t                        sl_nr_of_harq_processes_r16 = 1;
  uint8_t                        sl_harq_proc_id_offset_r16  = 0;
  sl_cg_max_trans_num_list_r16_l sl_cg_max_trans_num_list_r16;
  rrc_cfg_sidelink_grant_r16_s_  rrc_cfg_sidelink_grant_r16;
  // ...
  // group 0
  bool    sl_n1_pucch_an_type2_r16_present = false;
  uint8_t sl_n1_pucch_an_type2_r16         = 0;
  // group 1
  bool    sl_start_r_bset_cg_type1_r18_present = false;
  uint8_t sl_start_r_bset_cg_type1_r18         = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ConfiguredGrantConfigList-r16 ::= SEQUENCE
struct sl_cfg_grant_cfg_list_r16_s {
  using sl_cfg_grant_cfg_to_release_list_r16_l_ = bounded_array<uint8_t, 8>;
  using sl_cfg_grant_cfg_to_add_mod_list_r16_l_ = dyn_array<sl_cfg_grant_cfg_r16_s>;

  // member variables
  sl_cfg_grant_cfg_to_release_list_r16_l_ sl_cfg_grant_cfg_to_release_list_r16;
  sl_cfg_grant_cfg_to_add_mod_list_r16_l_ sl_cfg_grant_cfg_to_add_mod_list_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ConfiguredGrantConfigDedicatedSL-PRS-RP-r18 ::= SEQUENCE
struct sl_cfg_grant_cfg_ded_sl_prs_rp_r18_s {
  struct rrc_cfg_sidelink_grant_ded_sl_prs_rp_r18_s_ {
    bool     sl_time_offset_cg_type1_r18_present     = false;
    bool     sl_time_ref_sfn_type1_r18_present       = false;
    bool     sl_time_res_cg_type1_r18_present        = false;
    bool     sl_prs_res_ind_first_type1_r18_present  = false;
    bool     sl_prs_res_ind_future_type1_r18_present = false;
    uint16_t sl_time_offset_cg_type1_r18             = 0;
    uint16_t sl_time_res_cg_type1_r18                = 0;
    uint8_t  sl_prs_res_ind_first_type1_r18          = 0;
    uint8_t  sl_prs_res_ind_future_type1_r18         = 0;
  };

  // member variables
  bool                                        sl_prs_period_cg_r18_present   = false;
  bool                                        sl_prs_res_pool_id_r18_present = false;
  uint8_t                                     sl_prs_cfg_idx_cg_r18          = 0;
  sl_period_cg_r16_c                          sl_prs_period_cg_r18;
  uint8_t                                     sl_prs_res_pool_id_r18 = 1;
  rrc_cfg_sidelink_grant_ded_sl_prs_rp_r18_s_ rrc_cfg_sidelink_grant_ded_sl_prs_rp_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ConfiguredGrantConfigDedicatedSL-PRS-RP-List-r18 ::= SEQUENCE
struct sl_cfg_grant_cfg_ded_sl_prs_rp_list_r18_s {
  using sl_cfg_grant_cfg_ded_sl_prs_rp_to_release_list_r18_l_ = bounded_array<uint8_t, 8>;
  using sl_cfg_grant_cfg_ded_sl_prs_rp_to_add_mod_list_r18_l_ = dyn_array<sl_cfg_grant_cfg_ded_sl_prs_rp_r18_s>;

  // member variables
  sl_cfg_grant_cfg_ded_sl_prs_rp_to_release_list_r18_l_ sl_cfg_grant_cfg_ded_sl_prs_rp_to_release_list_r18;
  sl_cfg_grant_cfg_ded_sl_prs_rp_to_add_mod_list_r18_l_ sl_cfg_grant_cfg_ded_sl_prs_rp_to_add_mod_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ScheduledConfig-r16 ::= SEQUENCE
struct sl_sched_cfg_r16_s {
  using sl_psfch_to_pucch_r16_l_  = bounded_array<uint8_t, 8>;
  using sl_dci_to_sl_trans_r16_l_ = bounded_array<uint8_t, 8>;

  // member variables
  bool                        mac_main_cfg_sl_r16_present       = false;
  bool                        sl_cs_rnti_r16_present            = false;
  bool                        sl_cfg_grant_cfg_list_r16_present = false;
  uint32_t                    sl_rnti_r16                       = 0;
  mac_main_cfg_sl_r16_s       mac_main_cfg_sl_r16;
  uint32_t                    sl_cs_rnti_r16 = 0;
  sl_psfch_to_pucch_r16_l_    sl_psfch_to_pucch_r16;
  sl_cfg_grant_cfg_list_r16_s sl_cfg_grant_cfg_list_r16;
  // ...
  // group 0
  copy_ptr<sl_dci_to_sl_trans_r16_l_> sl_dci_to_sl_trans_r16;
  // group 1
  bool                                                sl_prs_rnti_r18_present    = false;
  bool                                                sl_prs_cs_rnti_r18_present = false;
  copy_ptr<sl_cfg_grant_cfg_ded_sl_prs_rp_list_r18_s> sl_cfg_grant_cfg_ded_sl_prs_rp_list_r18;
  uint32_t                                            sl_prs_rnti_r18    = 0;
  uint32_t                                            sl_prs_cs_rnti_r18 = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-TypeTxSync-r16 ::= ENUMERATED
struct sl_type_tx_sync_r16_opts {
  enum options { gnss, gnb_enb, ue, nulltype } value;

  const char* to_string() const;
};
using sl_type_tx_sync_r16_e = enumerated<sl_type_tx_sync_r16_opts>;

// SL-TxPower-r16 ::= CHOICE
struct sl_tx_pwr_r16_c {
  struct types_opts {
    enum options { minusinfinity_r16, tx_pwr_r16, nulltype } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  sl_tx_pwr_r16_c() = default;
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  int8_t& tx_pwr_r16()
  {
    assert_choice_type(types::tx_pwr_r16, type_, "SL-TxPower-r16");
    return c;
  }
  const int8_t& tx_pwr_r16() const
  {
    assert_choice_type(types::tx_pwr_r16, type_, "SL-TxPower-r16");
    return c;
  }
  void    set_minusinfinity_r16();
  int8_t& set_tx_pwr_r16();

private:
  types  type_;
  int8_t c;
};

// SL-PSSCH-TxParameters-r16 ::= SEQUENCE
struct sl_pssch_tx_params_r16_s {
  bool            sl_max_tx_pwr_r16_present     = false;
  uint8_t         sl_min_mcs_pssch_r16          = 0;
  uint8_t         sl_max_mcs_pssch_r16          = 0;
  uint8_t         sl_min_sub_ch_num_pssch_r16   = 1;
  uint8_t         sl_max_subch_num_pssch_r16    = 1;
  uint8_t         sl_max_tx_trans_num_pssch_r16 = 1;
  sl_tx_pwr_r16_c sl_max_tx_pwr_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MinMaxMCS-Config-r16 ::= SEQUENCE
struct sl_min_max_mcs_cfg_r16_s {
  struct sl_mcs_table_r16_opts {
    enum options { qam64, qam256, qam64_low_se, nulltype } value;

    const char* to_string() const;
  };
  using sl_mcs_table_r16_e_ = enumerated<sl_mcs_table_r16_opts>;

  // member variables
  sl_mcs_table_r16_e_ sl_mcs_table_r16;
  uint8_t             sl_min_mcs_pssch_r16 = 0;
  uint8_t             sl_max_mcs_pssch_r16 = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MinMaxMCS-List-r16 ::= SEQUENCE (SIZE (1..3)) OF SL-MinMaxMCS-Config-r16
using sl_min_max_mcs_list_r16_l = dyn_array<sl_min_max_mcs_cfg_r16_s>;

// SL-PSSCH-TxConfig-r16 ::= SEQUENCE
struct sl_pssch_tx_cfg_r16_s {
  struct sl_thres_ue_speed_r16_opts {
    enum options { kmph60, kmph80, kmph100, kmph120, kmph140, kmph160, kmph180, kmph200, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_thres_ue_speed_r16_e_ = enumerated<sl_thres_ue_speed_r16_opts>;

  // member variables
  bool                     sl_type_tx_sync_r16_present = false;
  sl_type_tx_sync_r16_e    sl_type_tx_sync_r16;
  sl_thres_ue_speed_r16_e_ sl_thres_ue_speed_r16;
  sl_pssch_tx_params_r16_s sl_params_above_thres_r16;
  sl_pssch_tx_params_r16_s sl_params_below_thres_r16;
  // ...
  // group 0
  copy_ptr<sl_min_max_mcs_list_r16_l> sl_params_above_thres_v1650;
  copy_ptr<sl_min_max_mcs_list_r16_l> sl_params_below_thres_v1650;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PSSCH-TxConfigList-r16 ::= SEQUENCE (SIZE (1..16)) OF SL-PSSCH-TxConfig-r16
using sl_pssch_tx_cfg_list_r16_l = dyn_array<sl_pssch_tx_cfg_r16_s>;

// SL-CBR-LevelsConfig-r16 ::= SEQUENCE (SIZE (1..16)) OF INTEGER (0..100)
using sl_cbr_levels_cfg_r16_l = bounded_array<uint8_t, 16>;

// SL-CBR-PSSCH-TxConfig-r16 ::= SEQUENCE
struct sl_cbr_pssch_tx_cfg_r16_s {
  bool                     sl_cr_limit_r16_present  = false;
  bool                     sl_tx_params_r16_present = false;
  uint16_t                 sl_cr_limit_r16          = 0;
  sl_pssch_tx_params_r16_s sl_tx_params_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-CBR-CommonTxConfigList-r16 ::= SEQUENCE
struct sl_cbr_common_tx_cfg_list_r16_s {
  using sl_cbr_range_cfg_list_r16_l_    = dyn_array<sl_cbr_levels_cfg_r16_l>;
  using sl_cbr_pssch_tx_cfg_list_r16_l_ = dyn_array<sl_cbr_pssch_tx_cfg_r16_s>;

  // member variables
  sl_cbr_range_cfg_list_r16_l_    sl_cbr_range_cfg_list_r16;
  sl_cbr_pssch_tx_cfg_list_r16_l_ sl_cbr_pssch_tx_cfg_list_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-CBR-LevelsDedicatedSL-PRS-RP-r18 ::= SEQUENCE (SIZE (0..15)) OF INTEGER (0..100)
using sl_cbr_levels_ded_sl_prs_rp_r18_l = bounded_array<uint8_t, 15>;

// SL-CBR-SL-PRS-TxConfig-r18 ::= SEQUENCE
struct sl_cbr_sl_prs_tx_cfg_r18_s {
  bool     sl_prs_cr_limit_r18_present    = false;
  bool     sl_prs_max_tx_pwr_r18_present  = false;
  bool     sl_prs_max_num_txs_r18_present = false;
  uint16_t sl_prs_cr_limit_r18            = 0;
  int8_t   sl_prs_max_tx_pwr_r18          = -30;
  uint8_t  sl_prs_max_num_txs_r18         = 1;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-CBR-CommonTxDedicatedSL-PRS-RP-List-r18 ::= SEQUENCE
struct sl_cbr_common_tx_ded_sl_prs_rp_list_r18_s {
  using sl_cbr_range_ded_sl_prs_rp_list_r18_l_ = dyn_array<sl_cbr_levels_ded_sl_prs_rp_r18_l>;
  using sl_cbr_sl_prs_tx_cfg_list_r18_l_       = dyn_array<sl_cbr_sl_prs_tx_cfg_r18_s>;

  // member variables
  sl_cbr_range_ded_sl_prs_rp_list_r18_l_ sl_cbr_range_ded_sl_prs_rp_list_r18;
  sl_cbr_sl_prs_tx_cfg_list_r18_l_       sl_cbr_sl_prs_tx_cfg_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-UE-SelectedConfig-r16 ::= SEQUENCE
struct sl_ue_sel_cfg_r16_s {
  struct sl_prob_res_keep_r16_opts {
    enum options { v0, v0dot2, v0dot4, v0dot6, v0dot8, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using sl_prob_res_keep_r16_e_ = enumerated<sl_prob_res_keep_r16_opts>;
  struct sl_reselect_after_r16_opts {
    enum options { n1, n2, n3, n4, n5, n6, n7, n8, n9, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_reselect_after_r16_e_ = enumerated<sl_reselect_after_r16_opts>;

  // member variables
  bool                            sl_prob_res_keep_r16_present          = false;
  bool                            sl_reselect_after_r16_present         = false;
  bool                            sl_cbr_common_tx_cfg_list_r16_present = false;
  bool                            ul_prioritization_thres_r16_present   = false;
  bool                            sl_prioritization_thres_r16_present   = false;
  sl_pssch_tx_cfg_list_r16_l      sl_pssch_tx_cfg_list_r16;
  sl_prob_res_keep_r16_e_         sl_prob_res_keep_r16;
  sl_reselect_after_r16_e_        sl_reselect_after_r16;
  sl_cbr_common_tx_cfg_list_r16_s sl_cbr_common_tx_cfg_list_r16;
  uint8_t                         ul_prioritization_thres_r16 = 1;
  uint8_t                         sl_prioritization_thres_r16 = 1;
  // ...
  // group 0
  copy_ptr<sl_cbr_common_tx_ded_sl_prs_rp_list_r18_s> sl_cbr_common_tx_ded_sl_prs_rp_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PSBCH-Config-r16 ::= SEQUENCE
struct sl_ps_bch_cfg_r16_s {
  struct dl_alpha_ps_bch_r16_opts {
    enum options { alpha0, alpha04, alpha05, alpha06, alpha07, alpha08, alpha09, alpha1, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using dl_alpha_ps_bch_r16_e_ = enumerated<dl_alpha_ps_bch_r16_opts>;

  // member variables
  bool                   dl_p0_ps_bch_r16_present    = false;
  bool                   dl_alpha_ps_bch_r16_present = false;
  int8_t                 dl_p0_ps_bch_r16            = -16;
  dl_alpha_ps_bch_r16_e_ dl_alpha_ps_bch_r16;
  // ...
  // group 0
  bool    dl_p0_ps_bch_r17_present = false;
  int16_t dl_p0_ps_bch_r17         = -202;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-LBT-FailureRecoveryConfig-r18 ::= SEQUENCE
struct sl_lbt_fail_recovery_cfg_r18_s {
  struct sl_lbt_fail_instance_max_count_r18_opts {
    enum options { n4, n8, n16, n32, n64, n128, spare2, spare1, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_lbt_fail_instance_max_count_r18_e_ = enumerated<sl_lbt_fail_instance_max_count_r18_opts>;
  struct sl_lbt_fail_detection_timer_r18_opts {
    enum options { ms10, ms20, ms40, ms80, ms160, ms320, spare2, spare1, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_lbt_fail_detection_timer_r18_e_ = enumerated<sl_lbt_fail_detection_timer_r18_opts>;
  struct sl_lbt_recovery_timer_r18_opts {
    enum options { ms10, ms20, ms40, ms80, ms160, ms320, spare2, spare1, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_lbt_recovery_timer_r18_e_ = enumerated<sl_lbt_recovery_timer_r18_opts>;

  // member variables
  bool                                  sl_lbt_fail_instance_max_count_r18_present = false;
  bool                                  sl_lbt_fail_detection_timer_r18_present    = false;
  bool                                  sl_lbt_recovery_timer_r18_present          = false;
  sl_lbt_fail_instance_max_count_r18_e_ sl_lbt_fail_instance_max_count_r18;
  sl_lbt_fail_detection_timer_r18_e_    sl_lbt_fail_detection_timer_r18;
  sl_lbt_recovery_timer_r18_e_          sl_lbt_recovery_timer_r18;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RBSetConfig-r18 ::= SEQUENCE
struct sl_rb_set_cfg_r18_s {
  bool    sl_num_of_s_ssb_repeat_r18_present      = false;
  bool    sl_gap_between_s_ssb_repeat_r18_present = false;
  uint8_t sl_rb_set_idx_r18                       = 0;
  uint8_t sl_num_of_s_ssb_repeat_r18              = 2;
  uint8_t sl_gap_between_s_ssb_repeat_r18         = 1;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-Unlicensed-r18 ::= SEQUENCE
struct sl_unlicensed_r18_s {
  struct sl_start_symbol_first_r18_opts {
    enum options { sym0, sym1, sym2, sym3, sym4, sym5, sym6, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_start_symbol_first_r18_e_ = enumerated<sl_start_symbol_first_r18_opts>;
  struct sl_start_symbol_second_r18_opts {
    enum options { sym3, sym4, sym5, sym6, sym7, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_start_symbol_second_r18_e_ = enumerated<sl_start_symbol_second_r18_opts>;
  struct sl_tx_structure_for_pscch_and_pssch_r18_opts {
    enum options { contiguous_rb, interlace_rb, nulltype } value;

    const char* to_string() const;
  };
  using sl_tx_structure_for_pscch_and_pssch_r18_e_  = enumerated<sl_tx_structure_for_pscch_and_pssch_r18_opts>;
  using sl_absolute_freq_ssb_non_anchor_list_r18_l_ = bounded_array<uint32_t, 4>;
  struct sl_cws_for_pssch_without_harq_ack_r18_opts {
    enum options { t1, t8, t16, t32, infinity, nulltype } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using sl_cws_for_pssch_without_harq_ack_r18_e_ = enumerated<sl_cws_for_pssch_without_harq_ack_r18_opts>;
  struct sl_s_ssb_pwr_offset_of_anchor_rb_set_r18_opts {
    enum options { value1, value2, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_s_ssb_pwr_offset_of_anchor_rb_set_r18_e_ = enumerated<sl_s_ssb_pwr_offset_of_anchor_rb_set_r18_opts>;
  using sl_rb_set_cfg_list_r18_l_                   = dyn_array<sl_rb_set_cfg_r18_s>;
  using sl_intra_cell_guard_bands_sl_list_r18_l_    = dyn_array<intra_cell_guard_bands_per_scs_r16_s>;

  // member variables
  bool                                            sl_lbt_fail_recovery_cfg_r18_present             = false;
  bool                                            sl_start_symbol_first_r18_present                = false;
  bool                                            sl_start_symbol_second_r18_present               = false;
  bool                                            sl_tx_structure_for_pscch_and_pssch_r18_present  = false;
  bool                                            sl_gap_of_add_s_ssb_occasion_r18_present         = false;
  bool                                            sl_cp_e_start_position_s_ssb_r18_present         = false;
  bool                                            sl_cws_for_pssch_without_harq_ack_r18_present    = false;
  bool                                            sl_num_of_add_s_ssb_occasion_r18_present         = false;
  bool                                            sl_s_ssb_pwr_offset_of_anchor_rb_set_r18_present = false;
  setup_release_c<sl_lbt_fail_recovery_cfg_r18_s> sl_lbt_fail_recovery_cfg_r18;
  sl_start_symbol_first_r18_e_                    sl_start_symbol_first_r18;
  sl_start_symbol_second_r18_e_                   sl_start_symbol_second_r18;
  sl_tx_structure_for_pscch_and_pssch_r18_e_      sl_tx_structure_for_pscch_and_pssch_r18;
  uint16_t                                        sl_gap_of_add_s_ssb_occasion_r18 = 0;
  sl_absolute_freq_ssb_non_anchor_list_r18_l_     sl_absolute_freq_ssb_non_anchor_list_r18;
  uint8_t                                         sl_cp_e_start_position_s_ssb_r18 = 1;
  sl_cws_for_pssch_without_harq_ack_r18_e_        sl_cws_for_pssch_without_harq_ack_r18;
  uint8_t                                         sl_num_of_add_s_ssb_occasion_r18 = 0;
  sl_s_ssb_pwr_offset_of_anchor_rb_set_r18_e_     sl_s_ssb_pwr_offset_of_anchor_rb_set_r18;
  sl_rb_set_cfg_list_r18_l_                       sl_rb_set_cfg_list_r18;
  sl_intra_cell_guard_bands_sl_list_r18_l_        sl_intra_cell_guard_bands_sl_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-BWP-Generic-r16 ::= SEQUENCE
struct sl_bwp_generic_r16_s {
  struct sl_len_symbols_r16_opts {
    enum options { sym7, sym8, sym9, sym10, sym11, sym12, sym13, sym14, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_len_symbols_r16_e_ = enumerated<sl_len_symbols_r16_opts>;
  struct sl_start_symbol_r16_opts {
    enum options { sym0, sym1, sym2, sym3, sym4, sym5, sym6, sym7, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_start_symbol_r16_e_ = enumerated<sl_start_symbol_r16_opts>;

  // member variables
  bool                                 sl_bwp_r16_present                        = false;
  bool                                 sl_len_symbols_r16_present                = false;
  bool                                 sl_start_symbol_r16_present               = false;
  bool                                 sl_ps_bch_cfg_r16_present                 = false;
  bool                                 sl_tx_direct_current_location_r16_present = false;
  bwp_s                                sl_bwp_r16;
  sl_len_symbols_r16_e_                sl_len_symbols_r16;
  sl_start_symbol_r16_e_               sl_start_symbol_r16;
  setup_release_c<sl_ps_bch_cfg_r16_s> sl_ps_bch_cfg_r16;
  uint16_t                             sl_tx_direct_current_location_r16 = 0;
  // ...
  // group 0
  copy_ptr<setup_release_c<sl_unlicensed_r18_s>> sl_unlicensed_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PSCCH-Config-r16 ::= SEQUENCE
struct sl_pscch_cfg_r16_s {
  struct sl_time_res_pscch_r16_opts {
    enum options { n2, n3, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_time_res_pscch_r16_e_ = enumerated<sl_time_res_pscch_r16_opts>;
  struct sl_freq_res_pscch_r16_opts {
    enum options { n10, n12, n15, n20, n25, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_freq_res_pscch_r16_e_ = enumerated<sl_freq_res_pscch_r16_opts>;

  // member variables
  bool                     sl_time_res_pscch_r16_present    = false;
  bool                     sl_freq_res_pscch_r16_present    = false;
  bool                     sl_dmrs_scramble_id_r16_present  = false;
  bool                     sl_num_reserved_bits_r16_present = false;
  sl_time_res_pscch_r16_e_ sl_time_res_pscch_r16;
  sl_freq_res_pscch_r16_e_ sl_freq_res_pscch_r16;
  uint32_t                 sl_dmrs_scramble_id_r16  = 0;
  uint8_t                  sl_num_reserved_bits_r16 = 2;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PSSCH-Config-r16 ::= SEQUENCE
struct sl_pssch_cfg_r16_s {
  using sl_pssch_dmrs_time_pattern_list_r16_l_ = bounded_array<uint8_t, 3>;
  using sl_beta_offsets2nd_sci_r16_l_          = std::array<uint8_t, 4>;
  struct sl_scaling_r16_opts {
    enum options { f0p5, f0p65, f0p8, f1, nulltype } value;

    const char* to_string() const;
  };
  using sl_scaling_r16_e_ = enumerated<sl_scaling_r16_opts>;

  // member variables
  bool                                   sl_beta_offsets2nd_sci_r16_present = false;
  bool                                   sl_scaling_r16_present             = false;
  sl_pssch_dmrs_time_pattern_list_r16_l_ sl_pssch_dmrs_time_pattern_list_r16;
  sl_beta_offsets2nd_sci_r16_l_          sl_beta_offsets2nd_sci_r16;
  sl_scaling_r16_e_                      sl_scaling_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PSFCH-Config-r16 ::= SEQUENCE
struct sl_psfch_cfg_r16_s {
  struct sl_psfch_period_r16_opts {
    enum options { sl0, sl1, sl2, sl4, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_psfch_period_r16_e_ = enumerated<sl_psfch_period_r16_opts>;
  struct sl_num_mux_cs_pair_r16_opts {
    enum options { n1, n2, n3, n6, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_num_mux_cs_pair_r16_e_ = enumerated<sl_num_mux_cs_pair_r16_opts>;
  struct sl_min_time_gap_psfch_r16_opts {
    enum options { sl2, sl3, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_min_time_gap_psfch_r16_e_ = enumerated<sl_min_time_gap_psfch_r16_opts>;
  struct sl_psfch_candidate_res_type_r16_opts {
    enum options { start_sub_ch, alloc_sub_ch, nulltype } value;

    const char* to_string() const;
  };
  using sl_psfch_candidate_res_type_r16_e_ = enumerated<sl_psfch_candidate_res_type_r16_opts>;

  // member variables
  bool                               sl_psfch_period_r16_present             = false;
  bool                               sl_psfch_rb_set_r16_present             = false;
  bool                               sl_num_mux_cs_pair_r16_present          = false;
  bool                               sl_min_time_gap_psfch_r16_present       = false;
  bool                               sl_psfch_hop_id_r16_present             = false;
  bool                               sl_psfch_candidate_res_type_r16_present = false;
  sl_psfch_period_r16_e_             sl_psfch_period_r16;
  bounded_bitstring<10, 275>         sl_psfch_rb_set_r16;
  sl_num_mux_cs_pair_r16_e_          sl_num_mux_cs_pair_r16;
  sl_min_time_gap_psfch_r16_e_       sl_min_time_gap_psfch_r16;
  uint16_t                           sl_psfch_hop_id_r16 = 0;
  sl_psfch_candidate_res_type_r16_e_ sl_psfch_candidate_res_type_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SyncAllowed-r16 ::= SEQUENCE
struct sl_sync_allowed_r16_s {
  bool gnss_sync_r16_present    = false;
  bool gnb_enb_sync_r16_present = false;
  bool ue_sync_r16_present      = false;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PTRS-Config-r16 ::= SEQUENCE
struct sl_ptrs_cfg_r16_s {
  using sl_ptrs_freq_density_r16_l_ = std::array<uint16_t, 2>;
  using sl_ptrs_time_density_r16_l_ = std::array<uint8_t, 3>;
  struct sl_ptrs_re_offset_r16_opts {
    enum options { offset01, offset10, offset11, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using sl_ptrs_re_offset_r16_e_ = enumerated<sl_ptrs_re_offset_r16_opts>;

  // member variables
  bool                        sl_ptrs_freq_density_r16_present = false;
  bool                        sl_ptrs_time_density_r16_present = false;
  bool                        sl_ptrs_re_offset_r16_present    = false;
  sl_ptrs_freq_density_r16_l_ sl_ptrs_freq_density_r16;
  sl_ptrs_time_density_r16_l_ sl_ptrs_time_density_r16;
  sl_ptrs_re_offset_r16_e_    sl_ptrs_re_offset_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PriorityTxConfigIndex-r16 ::= SEQUENCE
struct sl_prio_tx_cfg_idx_r16_s {
  using sl_tx_cfg_idx_list_r16_l_ = bounded_array<uint8_t, 16>;

  // member variables
  bool                      sl_prio_thres_r16_present         = false;
  bool                      sl_default_tx_cfg_idx_r16_present = false;
  bool                      sl_cbr_cfg_idx_r16_present        = false;
  uint8_t                   sl_prio_thres_r16                 = 1;
  uint8_t                   sl_default_tx_cfg_idx_r16         = 0;
  uint8_t                   sl_cbr_cfg_idx_r16                = 0;
  sl_tx_cfg_idx_list_r16_l_ sl_tx_cfg_idx_list_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-CBR-PriorityTxConfigList-r16 ::= SEQUENCE (SIZE (1..8)) OF SL-PriorityTxConfigIndex-r16
using sl_cbr_prio_tx_cfg_list_r16_l = dyn_array<sl_prio_tx_cfg_idx_r16_s>;

// SL-Thres-RSRP-List-r16 ::= SEQUENCE (SIZE (64)) OF INTEGER (0..66)
using sl_thres_rsrp_list_r16_l = std::array<uint8_t, 64>;

// SL-SelectionWindowConfig-r16 ::= SEQUENCE
struct sl_sel_win_cfg_r16_s {
  struct sl_sel_win_r16_opts {
    enum options { n1, n5, n10, n20, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_sel_win_r16_e_ = enumerated<sl_sel_win_r16_opts>;

  // member variables
  uint8_t           sl_prio_r16 = 1;
  sl_sel_win_r16_e_ sl_sel_win_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SelectionWindowList-r16 ::= SEQUENCE (SIZE (8)) OF SL-SelectionWindowConfig-r16
using sl_sel_win_list_r16_l = std::array<sl_sel_win_cfg_r16_s, 8>;

// SL-ResourceReservePeriod-r16 ::= CHOICE
struct sl_res_reserve_period_r16_c {
  struct sl_res_reserve_period1_r16_opts {
    enum options { ms0, ms100, ms200, ms300, ms400, ms500, ms600, ms700, ms800, ms900, ms1000, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_res_reserve_period1_r16_e_ = enumerated<sl_res_reserve_period1_r16_opts>;
  struct types_opts {
    enum options { sl_res_reserve_period1_r16, sl_res_reserve_period2_r16, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  sl_res_reserve_period_r16_c() = default;
  sl_res_reserve_period_r16_c(const sl_res_reserve_period_r16_c& other);
  sl_res_reserve_period_r16_c& operator=(const sl_res_reserve_period_r16_c& other);
  ~sl_res_reserve_period_r16_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  sl_res_reserve_period1_r16_e_& sl_res_reserve_period1_r16()
  {
    assert_choice_type(types::sl_res_reserve_period1_r16, type_, "SL-ResourceReservePeriod-r16");
    return c.get<sl_res_reserve_period1_r16_e_>();
  }
  uint8_t& sl_res_reserve_period2_r16()
  {
    assert_choice_type(types::sl_res_reserve_period2_r16, type_, "SL-ResourceReservePeriod-r16");
    return c.get<uint8_t>();
  }
  const sl_res_reserve_period1_r16_e_& sl_res_reserve_period1_r16() const
  {
    assert_choice_type(types::sl_res_reserve_period1_r16, type_, "SL-ResourceReservePeriod-r16");
    return c.get<sl_res_reserve_period1_r16_e_>();
  }
  const uint8_t& sl_res_reserve_period2_r16() const
  {
    assert_choice_type(types::sl_res_reserve_period2_r16, type_, "SL-ResourceReservePeriod-r16");
    return c.get<uint8_t>();
  }
  sl_res_reserve_period1_r16_e_& set_sl_res_reserve_period1_r16();
  uint8_t&                       set_sl_res_reserve_period2_r16();

private:
  types               type_;
  pod_choice_buffer_t c;

  void destroy_();
};

// SL-PriorityTxConfigIndex-v1650 ::= SEQUENCE
struct sl_prio_tx_cfg_idx_v1650_s {
  using sl_mcs_range_list_r16_l_ = dyn_array<sl_min_max_mcs_list_r16_l>;

  // member variables
  sl_mcs_range_list_r16_l_ sl_mcs_range_list_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-CBR-PriorityTxConfigList-v1650 ::= SEQUENCE (SIZE (1..8)) OF SL-PriorityTxConfigIndex-v1650
using sl_cbr_prio_tx_cfg_list_v1650_l = dyn_array<sl_prio_tx_cfg_idx_v1650_s>;

// SL-UE-SelectedConfigRP-r16 ::= SEQUENCE
struct sl_ue_sel_cfg_rp_r16_s {
  struct sl_max_num_per_reserve_r16_opts {
    enum options { n2, n3, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_max_num_per_reserve_r16_e_ = enumerated<sl_max_num_per_reserve_r16_opts>;
  struct sl_sensing_win_r16_opts {
    enum options { ms100, ms1100, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_sensing_win_r16_e_             = enumerated<sl_sensing_win_r16_opts>;
  using sl_res_reserve_period_list_r16_l_ = dyn_array<sl_res_reserve_period_r16_c>;
  struct sl_rs_for_sensing_r16_opts {
    enum options { pscch, pssch, nulltype } value;

    const char* to_string() const;
  };
  using sl_rs_for_sensing_r16_e_ = enumerated<sl_rs_for_sensing_r16_opts>;

  // member variables
  bool                              sl_thres_rsrp_list_r16_present     = false;
  bool                              sl_multi_reserve_res_r16_present   = false;
  bool                              sl_max_num_per_reserve_r16_present = false;
  bool                              sl_sensing_win_r16_present         = false;
  bool                              sl_sel_win_list_r16_present        = false;
  sl_cbr_prio_tx_cfg_list_r16_l     sl_cbr_prio_tx_cfg_list_r16;
  sl_thres_rsrp_list_r16_l          sl_thres_rsrp_list_r16;
  sl_max_num_per_reserve_r16_e_     sl_max_num_per_reserve_r16;
  sl_sensing_win_r16_e_             sl_sensing_win_r16;
  sl_sel_win_list_r16_l             sl_sel_win_list_r16;
  sl_res_reserve_period_list_r16_l_ sl_res_reserve_period_list_r16;
  sl_rs_for_sensing_r16_e_          sl_rs_for_sensing_r16;
  // ...
  // group 0
  copy_ptr<sl_cbr_prio_tx_cfg_list_v1650_l> sl_cbr_prio_tx_cfg_list_v1650;
  // group 1
  copy_ptr<sl_thres_rsrp_list_r16_l> sl_nr_pssch_eutra_thres_rsrp_list_r18;
  copy_ptr<sl_thres_rsrp_list_r16_l> sl_nr_psfch_eutra_thres_rsrp_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ZoneConfig-r16 ::= SEQUENCE
struct sl_zone_cfg_r16_s {
  struct sl_zone_len_r16_opts {
    enum options { m5, m10, m20, m30, m40, m50, spare2, spare1, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_zone_len_r16_e_ = enumerated<sl_zone_len_r16_opts>;

  // member variables
  sl_zone_len_r16_e_ sl_zone_len_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ZoneConfigMCR-r16 ::= SEQUENCE
struct sl_zone_cfg_mcr_r16_s {
  struct sl_trans_range_r16_opts {
    enum options {
      m20,
      m50,
      m80,
      m100,
      m120,
      m150,
      m180,
      m200,
      m220,
      m250,
      m270,
      m300,
      m350,
      m370,
      m400,
      m420,
      m450,
      m480,
      m500,
      m550,
      m600,
      m700,
      m1000,
      spare9,
      spare8,
      spare7,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_trans_range_r16_e_ = enumerated<sl_trans_range_r16_opts>;

  // member variables
  bool                  sl_trans_range_r16_present = false;
  bool                  sl_zone_cfg_r16_present    = false;
  uint8_t               sl_zone_cfg_mcr_idx_r16    = 0;
  sl_trans_range_r16_e_ sl_trans_range_r16;
  sl_zone_cfg_r16_s     sl_zone_cfg_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// FilterCoefficient ::= ENUMERATED
struct filt_coef_opts {
  enum options {
    fc0,
    fc1,
    fc2,
    fc3,
    fc4,
    fc5,
    fc6,
    fc7,
    fc8,
    fc9,
    fc11,
    fc13,
    fc15,
    fc17,
    fc19,
    spare1,
    /*...*/ nulltype
  } value;
  typedef uint8_t number_type;

  const char* to_string() const;
  uint8_t     to_number() const;
};
using filt_coef_e = enumerated<filt_coef_opts, true>;

// SL-PowerControl-r16 ::= SEQUENCE
struct sl_pwr_ctrl_r16_s {
  struct sl_alpha_pssch_pscch_r16_opts {
    enum options { alpha0, alpha04, alpha05, alpha06, alpha07, alpha08, alpha09, alpha1, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using sl_alpha_pssch_pscch_r16_e_ = enumerated<sl_alpha_pssch_pscch_r16_opts>;
  struct dl_alpha_pssch_pscch_r16_opts {
    enum options { alpha0, alpha04, alpha05, alpha06, alpha07, alpha08, alpha09, alpha1, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using dl_alpha_pssch_pscch_r16_e_ = enumerated<dl_alpha_pssch_pscch_r16_opts>;
  struct dl_alpha_psfch_r16_opts {
    enum options { alpha0, alpha04, alpha05, alpha06, alpha07, alpha08, alpha09, alpha1, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using dl_alpha_psfch_r16_e_ = enumerated<dl_alpha_psfch_r16_opts>;

  // member variables
  bool                        sl_alpha_pssch_pscch_r16_present = false;
  bool                        dl_alpha_pssch_pscch_r16_present = false;
  bool                        sl_p0_pssch_pscch_r16_present    = false;
  bool                        dl_p0_pssch_pscch_r16_present    = false;
  bool                        dl_alpha_psfch_r16_present       = false;
  bool                        dl_p0_psfch_r16_present          = false;
  int8_t                      sl_max_trans_pwr_r16             = -30;
  sl_alpha_pssch_pscch_r16_e_ sl_alpha_pssch_pscch_r16;
  dl_alpha_pssch_pscch_r16_e_ dl_alpha_pssch_pscch_r16;
  int8_t                      sl_p0_pssch_pscch_r16 = -16;
  int8_t                      dl_p0_pssch_pscch_r16 = -16;
  dl_alpha_psfch_r16_e_       dl_alpha_psfch_r16;
  int8_t                      dl_p0_psfch_r16 = -16;
  // ...
  // group 0
  bool    dl_p0_pssch_pscch_r17_present = false;
  bool    sl_p0_pssch_pscch_r17_present = false;
  bool    dl_p0_psfch_r17_present       = false;
  int16_t dl_p0_pssch_pscch_r17         = -202;
  int16_t sl_p0_pssch_pscch_r17         = -202;
  int16_t dl_p0_psfch_r17               = -202;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-TxPercentageConfig-r16 ::= SEQUENCE
struct sl_tx_percentage_cfg_r16_s {
  struct sl_tx_percentage_r16_opts {
    enum options { p20, p35, p50, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_tx_percentage_r16_e_ = enumerated<sl_tx_percentage_r16_opts>;

  // member variables
  uint8_t                 sl_prio_r16 = 1;
  sl_tx_percentage_r16_e_ sl_tx_percentage_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-TxPercentageList-r16 ::= SEQUENCE (SIZE (8)) OF SL-TxPercentageConfig-r16
using sl_tx_percentage_list_r16_l = std::array<sl_tx_percentage_cfg_r16_s, 8>;

// SL-PBPS-CPS-Config-r17 ::= SEQUENCE
struct sl_pbps_cp_s_cfg_r17_s {
  struct sl_allowed_res_sel_cfg_r17_opts {
    enum options { c1, c2, c3, c4, c5, c6, c7, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_allowed_res_sel_cfg_r17_e_               = enumerated<sl_allowed_res_sel_cfg_r17_opts>;
  using sl_pbps_occasion_reserve_period_list_r17_l_ = bounded_array<uint8_t, 16>;
  struct sl_partial_sensing_inactive_time_r17_opts {
    enum options { enabled, disabled, nulltype } value;

    const char* to_string() const;
  };
  using sl_partial_sensing_inactive_time_r17_e_ = enumerated<sl_partial_sensing_inactive_time_r17_opts>;

  // member variables
  bool                                        sl_allowed_res_sel_cfg_r17_present               = false;
  bool                                        sl_min_num_candidate_slots_periodic_r17_present  = false;
  bool                                        sl_add_pbps_occasion_r17_present                 = false;
  bool                                        sl_cp_s_win_periodic_r17_present                 = false;
  bool                                        sl_min_num_candidate_slots_aperiodic_r17_present = false;
  bool                                        sl_min_num_rssi_meas_slots_r17_present           = false;
  bool                                        sl_default_cbr_random_sel_r17_present            = false;
  bool                                        sl_default_cbr_partial_sensing_r17_present       = false;
  bool                                        sl_cp_s_win_aperiodic_r17_present                = false;
  bool                                        sl_partial_sensing_inactive_time_r17_present     = false;
  sl_allowed_res_sel_cfg_r17_e_               sl_allowed_res_sel_cfg_r17;
  uint8_t                                     sl_min_num_candidate_slots_periodic_r17 = 1;
  sl_pbps_occasion_reserve_period_list_r17_l_ sl_pbps_occasion_reserve_period_list_r17;
  uint8_t                                     sl_cp_s_win_periodic_r17                 = 5;
  uint8_t                                     sl_min_num_candidate_slots_aperiodic_r17 = 1;
  uint16_t                                    sl_min_num_rssi_meas_slots_r17           = 1;
  uint8_t                                     sl_default_cbr_random_sel_r17            = 0;
  uint8_t                                     sl_default_cbr_partial_sensing_r17       = 0;
  uint8_t                                     sl_cp_s_win_aperiodic_r17                = 0;
  sl_partial_sensing_inactive_time_r17_e_     sl_partial_sensing_inactive_time_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ThresholdRSRP-Condition1-B-1-r17 ::= SEQUENCE
struct sl_thres_rsrp_condition1_b_1_r17_s {
  uint8_t sl_prio_r17                      = 1;
  uint8_t sl_thres_rsrp_condition1_b_1_r17 = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-InterUE-CoordinationScheme1-r17 ::= SEQUENCE
struct sl_inter_ue_coordination_scheme1_r17_s {
  struct sl_iuc_explicit_r17_opts {
    enum options { enabled, disabled, nulltype } value;

    const char* to_string() const;
  };
  using sl_iuc_explicit_r17_e_ = enumerated<sl_iuc_explicit_r17_opts>;
  struct sl_iuc_condition_r17_opts {
    enum options { enabled, disabled, nulltype } value;

    const char* to_string() const;
  };
  using sl_iuc_condition_r17_e_                          = enumerated<sl_iuc_condition_r17_opts>;
  using sl_thres_rsrp_condition1_b_1_option1_list_r17_l_ = dyn_array<sl_thres_rsrp_condition1_b_1_r17_s>;
  using sl_thres_rsrp_condition1_b_1_option2_list_r17_l_ = dyn_array<sl_thres_rsrp_condition1_b_1_r17_s>;
  struct sl_container_coord_info_r17_opts {
    enum options { enabled, disabled, nulltype } value;

    const char* to_string() const;
  };
  using sl_container_coord_info_r17_e_ = enumerated<sl_container_coord_info_r17_opts>;
  struct sl_container_request_r17_opts {
    enum options { enabled, disabled, nulltype } value;

    const char* to_string() const;
  };
  using sl_container_request_r17_e_ = enumerated<sl_container_request_r17_opts>;
  struct sl_determine_res_type_r17_opts {
    enum options { uea, ueb, nulltype } value;

    const char* to_string() const;
  };
  using sl_determine_res_type_r17_e_ = enumerated<sl_determine_res_type_r17_opts>;

  // member variables
  bool                                             sl_iuc_explicit_r17_present                      = false;
  bool                                             sl_iuc_condition_r17_present                     = false;
  bool                                             sl_condition1_a_2_r17_present                    = false;
  bool                                             sl_container_coord_info_r17_present              = false;
  bool                                             sl_container_request_r17_present                 = false;
  bool                                             sl_trigger_condition_coord_info_r17_present      = false;
  bool                                             sl_trigger_condition_request_r17_present         = false;
  bool                                             sl_prio_coord_info_explicit_r17_present          = false;
  bool                                             sl_prio_coord_info_condition_r17_present         = false;
  bool                                             sl_prio_request_r17_present                      = false;
  bool                                             sl_prio_preferred_res_set_r17_present            = false;
  bool                                             sl_max_slot_offset_triv_r17_present              = false;
  bool                                             sl_num_sub_ch_preferred_res_set_r17_present      = false;
  bool                                             sl_reserved_period_preferred_res_set_r17_present = false;
  bool                                             sl_determine_res_type_r17_present                = false;
  sl_iuc_explicit_r17_e_                           sl_iuc_explicit_r17;
  sl_iuc_condition_r17_e_                          sl_iuc_condition_r17;
  sl_thres_rsrp_condition1_b_1_option1_list_r17_l_ sl_thres_rsrp_condition1_b_1_option1_list_r17;
  sl_thres_rsrp_condition1_b_1_option2_list_r17_l_ sl_thres_rsrp_condition1_b_1_option2_list_r17;
  sl_container_coord_info_r17_e_                   sl_container_coord_info_r17;
  sl_container_request_r17_e_                      sl_container_request_r17;
  uint8_t                                          sl_trigger_condition_coord_info_r17      = 0;
  uint8_t                                          sl_trigger_condition_request_r17         = 0;
  uint8_t                                          sl_prio_coord_info_explicit_r17          = 1;
  uint8_t                                          sl_prio_coord_info_condition_r17         = 1;
  uint8_t                                          sl_prio_request_r17                      = 1;
  uint8_t                                          sl_prio_preferred_res_set_r17            = 1;
  uint16_t                                         sl_max_slot_offset_triv_r17              = 1;
  uint8_t                                          sl_num_sub_ch_preferred_res_set_r17      = 1;
  uint8_t                                          sl_reserved_period_preferred_res_set_r17 = 1;
  sl_determine_res_type_r17_e_                     sl_determine_res_type_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-InterUE-CoordinationScheme2-r17 ::= SEQUENCE
struct sl_inter_ue_coordination_scheme2_r17_s {
  struct sl_ind_ue_b_r17_opts {
    enum options { enabled, disabled, nulltype } value;

    const char* to_string() const;
  };
  using sl_ind_ue_b_r17_e_ = enumerated<sl_ind_ue_b_r17_opts>;

  // member variables
  bool                       sl_iuc_scheme2_r17_present               = false;
  bool                       sl_rb_set_psfch_r17_present              = false;
  bool                       sl_type_ue_a_r17_present                 = false;
  bool                       sl_psfch_occasion_r17_present            = false;
  bool                       sl_slot_level_res_exclusion_r17_present  = false;
  bool                       sl_option_for_condition2_a_1_r17_present = false;
  bool                       sl_ind_ue_b_r17_present                  = false;
  bounded_bitstring<10, 275> sl_rb_set_psfch_r17;
  uint8_t                    sl_psfch_occasion_r17            = 0;
  uint8_t                    sl_option_for_condition2_a_1_r17 = 0;
  sl_ind_ue_b_r17_e_         sl_ind_ue_b_r17;
  // ...
  // group 0
  bool   sl_delta_rsrp_thresh_v1720_present = false;
  int8_t sl_delta_rsrp_thresh_v1720         = -30;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-InterUE-CoordinationConfig-r17 ::= SEQUENCE
struct sl_inter_ue_coordination_cfg_r17_s {
  bool                                   sl_inter_ue_coordination_scheme1_r17_present = false;
  bool                                   sl_inter_ue_coordination_scheme2_r17_present = false;
  sl_inter_ue_coordination_scheme1_r17_s sl_inter_ue_coordination_scheme1_r17;
  sl_inter_ue_coordination_scheme2_r17_s sl_inter_ue_coordination_scheme2_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-CPE-StartingPositionsPSCCH-PSSCH-r18 ::= SEQUENCE
struct sl_cp_e_start_positions_pscch_pssch_r18_s {
  using sl_cp_e_start_positions_r18_l_ = bounded_array<uint8_t, 9>;

  // member variables
  uint8_t                        sl_prio_r18 = 1;
  sl_cp_e_start_positions_r18_l_ sl_cp_e_start_positions_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-CPE-StartingPositionsPSCCH-PSSCH-List-r18 ::= SEQUENCE (SIZE (8)) OF SL-CPE-StartingPositionsPSCCH-PSSCH-r18
using sl_cp_e_start_positions_pscch_pssch_list_r18_l = std::array<sl_cp_e_start_positions_pscch_pssch_r18_s, 8>;

// SL-PRS-ResourceSharedSL-PRS-RP-r18 ::= SEQUENCE
struct sl_prs_res_shared_sl_prs_rp_r18_s {
  struct sl_prs_comb_size_n_and_re_offset_r18_c_ {
    struct types_opts {
      enum options { n2_r18, n4_r18, dummy1, /*...*/ nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts, true>;

    // choice methods
    sl_prs_comb_size_n_and_re_offset_r18_c_() = default;
    sl_prs_comb_size_n_and_re_offset_r18_c_(const sl_prs_comb_size_n_and_re_offset_r18_c_& other);
    sl_prs_comb_size_n_and_re_offset_r18_c_& operator=(const sl_prs_comb_size_n_and_re_offset_r18_c_& other);
    ~sl_prs_comb_size_n_and_re_offset_r18_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint8_t& n2_r18()
    {
      assert_choice_type(types::n2_r18, type_, "sl-PRS-CombSizeN-AndReOffset-r18");
      return c.get<uint8_t>();
    }
    uint8_t& n4_r18()
    {
      assert_choice_type(types::n4_r18, type_, "sl-PRS-CombSizeN-AndReOffset-r18");
      return c.get<uint8_t>();
    }
    uint8_t& dummy1()
    {
      assert_choice_type(types::dummy1, type_, "sl-PRS-CombSizeN-AndReOffset-r18");
      return c.get<uint8_t>();
    }
    const uint8_t& n2_r18() const
    {
      assert_choice_type(types::n2_r18, type_, "sl-PRS-CombSizeN-AndReOffset-r18");
      return c.get<uint8_t>();
    }
    const uint8_t& n4_r18() const
    {
      assert_choice_type(types::n4_r18, type_, "sl-PRS-CombSizeN-AndReOffset-r18");
      return c.get<uint8_t>();
    }
    const uint8_t& dummy1() const
    {
      assert_choice_type(types::dummy1, type_, "sl-PRS-CombSizeN-AndReOffset-r18");
      return c.get<uint8_t>();
    }
    uint8_t& set_n2_r18();
    uint8_t& set_n4_r18();
    uint8_t& set_dummy1();

  private:
    types               type_;
    pod_choice_buffer_t c;

    void destroy_();
  };

  // member variables
  bool                                    sl_prs_comb_size_n_and_re_offset_r18_present = false;
  uint8_t                                 sl_prs_res_id_r18                            = 0;
  uint8_t                                 mnof_symbols_r18                             = 1;
  sl_prs_comb_size_n_and_re_offset_r18_c_ sl_prs_comb_size_n_and_re_offset_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ResourcePool-r16 ::= SEQUENCE
struct sl_res_pool_r16_s {
  struct sl_subch_size_r16_opts {
    enum options { n10, n12, n15, n20, n25, n50, n75, n100, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_subch_size_r16_e_ = enumerated<sl_subch_size_r16_opts>;
  struct sl_add_mcs_table_r16_opts {
    enum options { qam256, qam64_low_se, qam256_qam64_low_se, nulltype } value;

    const char* to_string() const;
  };
  using sl_add_mcs_table_r16_e_ = enumerated<sl_add_mcs_table_r16_opts>;
  struct sl_time_win_size_cbr_r16_opts {
    enum options { ms100, slot100, nulltype } value;

    const char* to_string() const;
  };
  using sl_time_win_size_cbr_r16_e_ = enumerated<sl_time_win_size_cbr_r16_opts>;
  struct sl_time_win_size_cr_r16_opts {
    enum options { ms1000, slot1000, nulltype } value;

    const char* to_string() const;
  };
  using sl_time_win_size_cr_r16_e_ = enumerated<sl_time_win_size_cr_r16_opts>;
  struct sl_rx_params_ncell_r16_s_ {
    bool                   sl_tdd_cfg_r16_present = false;
    tdd_ul_dl_cfg_common_s sl_tdd_cfg_r16;
    uint8_t                sl_sync_cfg_idx_r16 = 0;
  };
  using sl_zone_cfg_mcr_list_r16_l_ = std::array<sl_zone_cfg_mcr_r16_s, 16>;
  struct sl_preemption_enable_r16_opts {
    enum options { enabled, pl1, pl2, pl3, pl4, pl5, pl6, pl7, pl8, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_preemption_enable_r16_e_ = enumerated<sl_preemption_enable_r16_opts>;
  struct sl_x_overhead_r16_opts {
    enum options { n0, n3, n6, n9, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_x_overhead_r16_e_ = enumerated<sl_x_overhead_r16_opts>;
  struct sl_num_interlace_per_subch_r18_opts {
    enum options { sc1, sc2, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_num_interlace_per_subch_r18_e_ = enumerated<sl_num_interlace_per_subch_r18_opts>;
  struct sl_num_ref_prbs_of_interlace_r18_opts {
    enum options { prb10, prb11, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_num_ref_prbs_of_interlace_r18_e_ = enumerated<sl_num_ref_prbs_of_interlace_r18_opts>;
  struct sl_tx_structure_for_psfch_r18_opts {
    enum options { common_interlace, ded_interlace, nulltype } value;

    const char* to_string() const;
  };
  using sl_tx_structure_for_psfch_r18_e_ = enumerated<sl_tx_structure_for_psfch_r18_opts>;
  struct sl_num_ded_prbs_for_psfch_r18_opts {
    enum options { prb1, prb2, prb5, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_num_ded_prbs_for_psfch_r18_e_ = enumerated<sl_num_ded_prbs_for_psfch_r18_opts>;
  struct sl_num_psfch_occasions_r18_opts {
    enum options { o1, o2, o3, o4, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_num_psfch_occasions_r18_e_ = enumerated<sl_num_psfch_occasions_r18_opts>;
  struct sl_num_ref_symbol_len_r18_opts {
    enum options { sym7, sym8, sym9, sym10, sym11, sym12, sym13, sym14, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_num_ref_symbol_len_r18_e_     = enumerated<sl_num_ref_symbol_len_r18_opts>;
  using sl_psfch_rb_set_list_r18_l_      = bounded_array<bounded_bitstring<10, 275>, 4>;
  using sl_iuc_rb_set_list_r18_l_        = bounded_array<bounded_bitstring<10, 275>, 4>;
  using sl_rb_set_idx_of_res_pool_r18_l_ = bounded_array<uint8_t, 5>;
  struct sl_a2_x_service_r18_opts {
    enum options { brid, daa, brid_and_daa, spare1, nulltype } value;

    const char* to_string() const;
  };
  using sl_a2_x_service_r18_e_             = enumerated<sl_a2_x_service_r18_opts>;
  using sl_prs_res_shared_sl_prs_rp_r18_l_ = dyn_array<sl_prs_res_shared_sl_prs_rp_r18_s>;

  // member variables
  bool                                sl_pscch_cfg_r16_present           = false;
  bool                                sl_pssch_cfg_r16_present           = false;
  bool                                sl_psfch_cfg_r16_present           = false;
  bool                                sl_sync_allowed_r16_present        = false;
  bool                                sl_subch_size_r16_present          = false;
  bool                                dummy_present                      = false;
  bool                                sl_start_rb_subch_r16_present      = false;
  bool                                sl_num_subch_r16_present           = false;
  bool                                sl_add_mcs_table_r16_present       = false;
  bool                                sl_thresh_s_rssi_cbr_r16_present   = false;
  bool                                sl_time_win_size_cbr_r16_present   = false;
  bool                                sl_time_win_size_cr_r16_present    = false;
  bool                                sl_ptrs_cfg_r16_present            = false;
  bool                                sl_ue_sel_cfg_rp_r16_present       = false;
  bool                                sl_rx_params_ncell_r16_present     = false;
  bool                                sl_zone_cfg_mcr_list_r16_present   = false;
  bool                                sl_filt_coef_r16_present           = false;
  bool                                sl_rb_num_r16_present              = false;
  bool                                sl_preemption_enable_r16_present   = false;
  bool                                sl_prio_thres_ul_urllc_r16_present = false;
  bool                                sl_prio_thres_r16_present          = false;
  bool                                sl_x_overhead_r16_present          = false;
  bool                                sl_pwr_ctrl_r16_present            = false;
  bool                                sl_tx_percentage_list_r16_present  = false;
  setup_release_c<sl_pscch_cfg_r16_s> sl_pscch_cfg_r16;
  setup_release_c<sl_pssch_cfg_r16_s> sl_pssch_cfg_r16;
  setup_release_c<sl_psfch_cfg_r16_s> sl_psfch_cfg_r16;
  sl_sync_allowed_r16_s               sl_sync_allowed_r16;
  sl_subch_size_r16_e_                sl_subch_size_r16;
  uint8_t                             dummy                 = 10;
  uint16_t                            sl_start_rb_subch_r16 = 0;
  uint8_t                             sl_num_subch_r16      = 1;
  sl_add_mcs_table_r16_e_             sl_add_mcs_table_r16;
  uint8_t                             sl_thresh_s_rssi_cbr_r16 = 0;
  sl_time_win_size_cbr_r16_e_         sl_time_win_size_cbr_r16;
  sl_time_win_size_cr_r16_e_          sl_time_win_size_cr_r16;
  sl_ptrs_cfg_r16_s                   sl_ptrs_cfg_r16;
  sl_ue_sel_cfg_rp_r16_s              sl_ue_sel_cfg_rp_r16;
  sl_rx_params_ncell_r16_s_           sl_rx_params_ncell_r16;
  sl_zone_cfg_mcr_list_r16_l_         sl_zone_cfg_mcr_list_r16;
  filt_coef_e                         sl_filt_coef_r16;
  uint16_t                            sl_rb_num_r16 = 10;
  sl_preemption_enable_r16_e_         sl_preemption_enable_r16;
  uint8_t                             sl_prio_thres_ul_urllc_r16 = 1;
  uint8_t                             sl_prio_thres_r16          = 1;
  sl_x_overhead_r16_e_                sl_x_overhead_r16;
  sl_pwr_ctrl_r16_s                   sl_pwr_ctrl_r16;
  sl_tx_percentage_list_r16_l         sl_tx_percentage_list_r16;
  sl_min_max_mcs_list_r16_l           sl_min_max_mcs_list_r16;
  // ...
  // group 0
  bool                       sl_time_res_r16_present = false;
  bounded_bitstring<10, 160> sl_time_res_r16;
  // group 1
  copy_ptr<setup_release_c<sl_pbps_cp_s_cfg_r17_s>>             sl_pbps_cp_s_cfg_r17;
  copy_ptr<setup_release_c<sl_inter_ue_coordination_cfg_r17_s>> sl_inter_ue_coordination_cfg_r17;
  // group 2
  bool sl_cp_e_start_positions_pscch_pssch_initiate_cot_default_r18_present = false;
  bool sl_cp_e_start_positions_pscch_pssch_within_cot_default_r18_present   = false;
  bool sl_type1_lbt_blocking_option1_r18_present                            = false;
  bool sl_type1_lbt_blocking_option2_r18_present                            = false;
  bool sl_num_interlace_per_subch_r18_present                               = false;
  bool sl_num_ref_prbs_of_interlace_r18_present                             = false;
  bool sl_tx_structure_for_psfch_r18_present                                = false;
  bool sl_num_ded_prbs_for_psfch_r18_present                                = false;
  bool sl_num_psfch_occasions_r18_present                                   = false;
  bool sl_psfch_common_interlace_idx_r18_present                            = false;
  bool sl_cp_e_start_position_psfch_r18_present                             = false;
  bool sl_num_ref_symbol_len_r18_present                                    = false;
  bool sl_psfch_pwr_offset_r18_present                                      = false;
  bool sl_a2_x_service_r18_present                                          = false;
  bool num_sym_sl_prs_2nd_stage_sci_r18_present                             = false;
  bool sl_sci_based_sl_prs_tx_trigger_sci2_d_r18_present                    = false;
  copy_ptr<setup_release_c<sl_cp_e_start_positions_pscch_pssch_list_r18_l>>
          sl_cp_e_start_positions_pscch_pssch_initiate_cot_list_r18;
  uint8_t sl_cp_e_start_positions_pscch_pssch_initiate_cot_default_r18 = 1;
  copy_ptr<setup_release_c<sl_cp_e_start_positions_pscch_pssch_list_r18_l>>
                                               sl_cp_e_start_positions_pscch_pssch_within_cot_list_r18;
  uint8_t                                      sl_cp_e_start_positions_pscch_pssch_within_cot_default_r18 = 1;
  sl_num_interlace_per_subch_r18_e_            sl_num_interlace_per_subch_r18;
  sl_num_ref_prbs_of_interlace_r18_e_          sl_num_ref_prbs_of_interlace_r18;
  sl_tx_structure_for_psfch_r18_e_             sl_tx_structure_for_psfch_r18;
  sl_num_ded_prbs_for_psfch_r18_e_             sl_num_ded_prbs_for_psfch_r18;
  sl_num_psfch_occasions_r18_e_                sl_num_psfch_occasions_r18;
  uint8_t                                      sl_psfch_common_interlace_idx_r18 = 0;
  uint8_t                                      sl_cp_e_start_position_psfch_r18  = 1;
  sl_num_ref_symbol_len_r18_e_                 sl_num_ref_symbol_len_r18;
  copy_ptr<sl_psfch_rb_set_list_r18_l_>        sl_psfch_rb_set_list_r18;
  copy_ptr<sl_iuc_rb_set_list_r18_l_>          sl_iuc_rb_set_list_r18;
  uint8_t                                      sl_psfch_pwr_offset_r18 = 0;
  copy_ptr<sl_rb_set_idx_of_res_pool_r18_l_>   sl_rb_set_idx_of_res_pool_r18;
  sl_a2_x_service_r18_e_                       sl_a2_x_service_r18;
  copy_ptr<sl_prs_res_shared_sl_prs_rp_r18_l_> sl_prs_res_shared_sl_prs_rp_r18;
  uint8_t                                      num_sym_sl_prs_2nd_stage_sci_r18          = 1;
  bool                                         sl_sci_based_sl_prs_tx_trigger_sci2_d_r18 = false;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ResourcePoolConfig-r16 ::= SEQUENCE
struct sl_res_pool_cfg_r16_s {
  bool              sl_res_pool_r16_present = false;
  uint8_t           sl_res_pool_id_r16      = 1;
  sl_res_pool_r16_s sl_res_pool_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-TxPoolDedicated-r16 ::= SEQUENCE
struct sl_tx_pool_ded_r16_s {
  using sl_pool_to_release_list_r16_l_ = bounded_array<uint8_t, 8>;
  using sl_pool_to_add_mod_list_r16_l_ = dyn_array<sl_res_pool_cfg_r16_s>;

  // member variables
  sl_pool_to_release_list_r16_l_ sl_pool_to_release_list_r16;
  sl_pool_to_add_mod_list_r16_l_ sl_pool_to_add_mod_list_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-BWP-PoolConfig-r16 ::= SEQUENCE
struct sl_bwp_pool_cfg_r16_s {
  using sl_rx_pool_r16_l_ = dyn_array<sl_res_pool_r16_s>;

  // member variables
  bool                  sl_tx_pool_sel_normal_r16_present  = false;
  bool                  sl_tx_pool_sched_r16_present       = false;
  bool                  sl_tx_pool_exceptional_r16_present = false;
  sl_rx_pool_r16_l_     sl_rx_pool_r16;
  sl_tx_pool_ded_r16_s  sl_tx_pool_sel_normal_r16;
  sl_tx_pool_ded_r16_s  sl_tx_pool_sched_r16;
  sl_res_pool_cfg_r16_s sl_tx_pool_exceptional_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-BWP-DiscPoolConfig-r17 ::= SEQUENCE
struct sl_bwp_disc_pool_cfg_r17_s {
  using sl_disc_rx_pool_r17_l_ = dyn_array<sl_res_pool_r16_s>;

  // member variables
  bool                   sl_disc_tx_pool_sel_r17_present   = false;
  bool                   sl_disc_tx_pool_sched_r17_present = false;
  sl_disc_rx_pool_r17_l_ sl_disc_rx_pool_r17;
  sl_tx_pool_ded_r16_s   sl_disc_tx_pool_sel_r17;
  sl_tx_pool_ded_r16_s   sl_disc_tx_pool_sched_r17;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PSCCH-ConfigDedicatedSL-PRS-RP-r18 ::= SEQUENCE
struct sl_pscch_cfg_ded_sl_prs_rp_r18_s {
  struct sl_time_res_pscch_ded_sl_prs_rp_r18_opts {
    enum options { n2, n3, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_time_res_pscch_ded_sl_prs_rp_r18_e_ = enumerated<sl_time_res_pscch_ded_sl_prs_rp_r18_opts>;
  struct sl_freq_res_pscch_ded_sl_prs_rp_r18_opts {
    enum options { n10, n12, n15, n20, n25, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_freq_res_pscch_ded_sl_prs_rp_r18_e_ = enumerated<sl_freq_res_pscch_ded_sl_prs_rp_r18_opts>;

  // member variables
  bool                                   sl_time_res_pscch_ded_sl_prs_rp_r18_present = false;
  bool                                   sl_freq_res_pscch_ded_sl_prs_rp_r18_present = false;
  sl_time_res_pscch_ded_sl_prs_rp_r18_e_ sl_time_res_pscch_ded_sl_prs_rp_r18;
  sl_freq_res_pscch_ded_sl_prs_rp_r18_e_ sl_freq_res_pscch_ded_sl_prs_rp_r18;
  // ...
  // group 0
  bool     sl_dmrs_scramble_id_ded_sl_prs_rp_r18_present = false;
  uint32_t sl_dmrs_scramble_id_ded_sl_prs_rp_r18         = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ReservationPeriodAllowedDedicatedSL-PRS-RP-r18 ::= CHOICE
struct sl_reserv_period_allowed_ded_sl_prs_rp_r18_c {
  struct sl_res_reserve_period1_r18_opts {
    enum options {
      ms0,
      ms100,
      ms160,
      ms200,
      ms300,
      ms320,
      ms400,
      ms500,
      ms600,
      ms640,
      ms700,
      ms800,
      ms900,
      ms1000,
      ms1280,
      ms2560,
      ms5120,
      ms10240,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_res_reserve_period1_r18_e_ = enumerated<sl_res_reserve_period1_r18_opts>;
  struct types_opts {
    enum options { sl_res_reserve_period1_r18, sl_res_reserve_period2_r18, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  sl_reserv_period_allowed_ded_sl_prs_rp_r18_c() = default;
  sl_reserv_period_allowed_ded_sl_prs_rp_r18_c(const sl_reserv_period_allowed_ded_sl_prs_rp_r18_c& other);
  sl_reserv_period_allowed_ded_sl_prs_rp_r18_c& operator=(const sl_reserv_period_allowed_ded_sl_prs_rp_r18_c& other);
  ~sl_reserv_period_allowed_ded_sl_prs_rp_r18_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  sl_res_reserve_period1_r18_e_& sl_res_reserve_period1_r18()
  {
    assert_choice_type(types::sl_res_reserve_period1_r18, type_, "SL-ReservationPeriodAllowedDedicatedSL-PRS-RP-r18");
    return c.get<sl_res_reserve_period1_r18_e_>();
  }
  uint8_t& sl_res_reserve_period2_r18()
  {
    assert_choice_type(types::sl_res_reserve_period2_r18, type_, "SL-ReservationPeriodAllowedDedicatedSL-PRS-RP-r18");
    return c.get<uint8_t>();
  }
  const sl_res_reserve_period1_r18_e_& sl_res_reserve_period1_r18() const
  {
    assert_choice_type(types::sl_res_reserve_period1_r18, type_, "SL-ReservationPeriodAllowedDedicatedSL-PRS-RP-r18");
    return c.get<sl_res_reserve_period1_r18_e_>();
  }
  const uint8_t& sl_res_reserve_period2_r18() const
  {
    assert_choice_type(types::sl_res_reserve_period2_r18, type_, "SL-ReservationPeriodAllowedDedicatedSL-PRS-RP-r18");
    return c.get<uint8_t>();
  }
  sl_res_reserve_period1_r18_e_& set_sl_res_reserve_period1_r18();
  uint8_t&                       set_sl_res_reserve_period2_r18();

private:
  types               type_;
  pod_choice_buffer_t c;

  void destroy_();
};

// SL-PRS-ResourceDedicatedSL-PRS-RP-r18 ::= SEQUENCE
struct sl_prs_res_ded_sl_prs_rp_r18_s {
  struct sl_comb_size_r18_opts {
    enum options { n2, n4, n6, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_comb_size_r18_e_ = enumerated<sl_comb_size_r18_opts>;

  // member variables
  bool                sl_prs_res_id_r18_present       = false;
  bool                sl_nof_symbols_r18_present      = false;
  bool                sl_comb_size_r18_present        = false;
  bool                sl_prs_start_symbol_r18_present = false;
  bool                sl_prs_comb_offset_r18_present  = false;
  uint8_t             sl_prs_res_id_r18               = 0;
  uint8_t             sl_nof_symbols_r18              = 1;
  sl_comb_size_r18_e_ sl_comb_size_r18;
  uint8_t             sl_prs_start_symbol_r18 = 4;
  uint8_t             sl_prs_comb_offset_r18  = 1;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PRS-PowerControl-r18 ::= SEQUENCE
struct sl_prs_pwr_ctrl_r18_s {
  struct dl_alpha_sl_prs_r18_opts {
    enum options { alpha0, alpha04, alpha05, alpha06, alpha07, alpha08, alpha09, alpha1, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using dl_alpha_sl_prs_r18_e_ = enumerated<dl_alpha_sl_prs_r18_opts>;
  struct sl_alpha_sl_prs_r18_opts {
    enum options { alpha0, alpha04, alpha05, alpha06, alpha07, alpha08, alpha09, alpha1, nulltype } value;
    typedef float number_type;

    const char* to_string() const;
    float       to_number() const;
    const char* to_number_string() const;
  };
  using sl_alpha_sl_prs_r18_e_ = enumerated<sl_alpha_sl_prs_r18_opts>;

  // member variables
  bool                   dl_p0_sl_prs_r18_present    = false;
  bool                   dl_alpha_sl_prs_r18_present = false;
  bool                   sl_p0_sl_prs_r18_present    = false;
  bool                   sl_alpha_sl_prs_r18_present = false;
  int16_t                dl_p0_sl_prs_r18            = -202;
  dl_alpha_sl_prs_r18_e_ dl_alpha_sl_prs_r18;
  int16_t                sl_p0_sl_prs_r18 = -202;
  sl_alpha_sl_prs_r18_e_ sl_alpha_sl_prs_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-TxPercentageDedicatedSL-PRS-RP-Config-r18 ::= SEQUENCE
struct sl_tx_percentage_ded_sl_prs_rp_cfg_r18_s {
  struct sl_prio_ded_sl_prs_rp_opts {
    enum options { p20, p35, p50, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_prio_ded_sl_prs_rp_e_ = enumerated<sl_prio_ded_sl_prs_rp_opts>;

  // member variables
  bool                     sl_tx_percentage_ded_sl_prs_rp_r18_present = false;
  bool                     sl_prio_ded_sl_prs_rp_present              = false;
  uint8_t                  sl_tx_percentage_ded_sl_prs_rp_r18         = 1;
  sl_prio_ded_sl_prs_rp_e_ sl_prio_ded_sl_prs_rp;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PriorityTxConfigIndexDedicatedSL-PRS-RP-r18 ::= SEQUENCE
struct sl_prio_tx_cfg_idx_ded_sl_prs_rp_r18_s {
  using sl_prs_tx_cfg_idx_list_r18_l_ = bounded_array<uint8_t, 15>;

  // member variables
  bool                          sl_prio_thres_ded_sl_prs_rp_r18_present         = false;
  bool                          sl_default_tx_cfg_idx_ded_sl_prs_rp_r18_present = false;
  bool                          sl_cbr_cfg_idx_ded_sl_prs_rp_r18_present        = false;
  uint8_t                       sl_prio_thres_ded_sl_prs_rp_r18                 = 1;
  uint8_t                       sl_default_tx_cfg_idx_ded_sl_prs_rp_r18         = 0;
  uint8_t                       sl_cbr_cfg_idx_ded_sl_prs_rp_r18                = 0;
  sl_prs_tx_cfg_idx_list_r18_l_ sl_prs_tx_cfg_idx_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SelectionWindowConfigDedicatedSL-PRS-RP-r18 ::= SEQUENCE
struct sl_sel_win_cfg_ded_sl_prs_rp_r18_s {
  struct sl_prs_sel_win_r18_opts {
    enum options { n1, n5, n10, n20, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_prs_sel_win_r18_e_ = enumerated<sl_prs_sel_win_r18_opts>;

  // member variables
  uint8_t               sl_prs_prio_r18 = 1;
  sl_prs_sel_win_r18_e_ sl_prs_sel_win_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PRS-ResourcePool-r18 ::= SEQUENCE
struct sl_prs_res_pool_r18_s {
  struct sl_pos_allowed_res_sel_cfg_r18_opts {
    enum options { c1, c2, c3, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_pos_allowed_res_sel_cfg_r18_e_     = enumerated<sl_pos_allowed_res_sel_cfg_r18_opts>;
  using sl_prs_res_reserve_period_list_r18_l_ = dyn_array<sl_reserv_period_allowed_ded_sl_prs_rp_r18_c>;
  using sl_prs_res_ded_sl_prs_rp_r18_l_       = dyn_array<sl_prs_res_ded_sl_prs_rp_r18_s>;
  struct sl_sensing_win_ded_sl_prs_rp_r18_opts {
    enum options { ms100, ms1100, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_sensing_win_ded_sl_prs_rp_r18_e_        = enumerated<sl_sensing_win_ded_sl_prs_rp_r18_opts>;
  using sl_tx_percentage_ded_sl_prs_rp_list_r18_l_ = std::array<sl_tx_percentage_ded_sl_prs_rp_cfg_r18_s, 8>;
  struct sl_subch_size_ded_sl_prs_rp_r18_opts {
    enum options { n10, n12, n15, n20, n25, n50, n75, n100, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_subch_size_ded_sl_prs_rp_r18_e_ = enumerated<sl_subch_size_ded_sl_prs_rp_r18_opts>;
  struct sl_max_num_per_reserve_ded_sl_prs_rp_r18_opts {
    enum options { n2, n3, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_max_num_per_reserve_ded_sl_prs_rp_r18_e_ = enumerated<sl_max_num_per_reserve_ded_sl_prs_rp_r18_opts>;
  struct sl_src_id_len_ded_sl_prs_rp_r18_opts {
    enum options { n12, n24, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_src_id_len_ded_sl_prs_rp_r18_e_           = enumerated<sl_src_id_len_ded_sl_prs_rp_r18_opts>;
  using sl_cbr_prio_tx_cfg_ded_sl_prs_rp_list_r18_l_ = dyn_array<sl_prio_tx_cfg_idx_ded_sl_prs_rp_r18_s>;
  struct sl_time_win_size_cbr_ded_sl_prs_rp_r18_opts {
    enum options { ms100, slot100, nulltype } value;

    const char* to_string() const;
  };
  using sl_time_win_size_cbr_ded_sl_prs_rp_r18_e_ = enumerated<sl_time_win_size_cbr_ded_sl_prs_rp_r18_opts>;
  struct sl_time_win_size_cr_ded_sl_prs_rp_r18_opts {
    enum options { ms1000, slot1000, nulltype } value;

    const char* to_string() const;
  };
  using sl_time_win_size_cr_ded_sl_prs_rp_r18_e_ = enumerated<sl_time_win_size_cr_ded_sl_prs_rp_r18_opts>;
  using sl_sel_win_list_ded_sl_prs_rp_r18_l_     = std::array<sl_sel_win_cfg_ded_sl_prs_rp_r18_s, 8>;
  using sl_thres_rsrp_list_ded_sl_prs_rp_r18_l_  = std::array<uint8_t, 64>;
  struct sl_preemption_enable_ded_sl_prs_rp_r18_opts {
    enum options { enabled, pl1, pl2, pl3, pl4, pl5, pl6, pl7, pl8, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_preemption_enable_ded_sl_prs_rp_r18_e_ = enumerated<sl_preemption_enable_ded_sl_prs_rp_r18_opts>;

  // member variables
  bool                                              sl_prs_pscch_cfg_r18_present                          = false;
  bool                                              sl_start_rb_subch_ded_sl_prs_rp_r18_present           = false;
  bool                                              sl_filt_coef_r18_present                              = false;
  bool                                              sl_thresh_s_rssi_prs_cbr_r18_present                  = false;
  bool                                              sl_rb_num_r18_present                                 = false;
  bool                                              sl_time_res_r18_present                               = false;
  bool                                              sl_pos_allowed_res_sel_cfg_r18_present                = false;
  bool                                              sl_prs_pwr_ctrl_r18_present                           = false;
  bool                                              sl_sensing_win_ded_sl_prs_rp_r18_present              = false;
  bool                                              sl_tx_percentage_ded_sl_prs_rp_list_r18_present       = false;
  bool                                              sl_sci_based_sl_prs_tx_trigger_sci1_b_r18_present     = false;
  bool                                              sl_num_subch_ded_sl_prs_rp_r18_present                = false;
  bool                                              sl_subch_size_ded_sl_prs_rp_r18_present               = false;
  bool                                              sl_max_num_per_reserve_ded_sl_prs_rp_r18_present      = false;
  bool                                              sl_num_reserved_bits_sci1_b_ded_sl_prs_rp_r18_present = false;
  bool                                              sl_src_id_len_ded_sl_prs_rp_r18_present               = false;
  bool                                              sl_time_win_size_cbr_ded_sl_prs_rp_r18_present        = false;
  bool                                              sl_time_win_size_cr_ded_sl_prs_rp_r18_present         = false;
  bool                                              sl_cbr_common_tx_ded_sl_prs_rp_list_r18_present       = false;
  bool                                              sl_prio_thres_ul_urllc_r18_present                    = false;
  bool                                              sl_prio_thres_r18_present                             = false;
  bool                                              sl_sel_win_list_ded_sl_prs_rp_r18_present             = false;
  bool                                              sl_thres_rsrp_list_ded_sl_prs_rp_r18_present          = false;
  bool                                              sl_preemption_enable_ded_sl_prs_rp_r18_present        = false;
  setup_release_c<sl_pscch_cfg_ded_sl_prs_rp_r18_s> sl_prs_pscch_cfg_r18;
  uint16_t                                          sl_start_rb_subch_ded_sl_prs_rp_r18 = 0;
  filt_coef_e                                       sl_filt_coef_r18;
  uint8_t                                           sl_thresh_s_rssi_prs_cbr_r18 = 0;
  uint16_t                                          sl_rb_num_r18                = 10;
  bounded_bitstring<10, 160>                        sl_time_res_r18;
  sl_pos_allowed_res_sel_cfg_r18_e_                 sl_pos_allowed_res_sel_cfg_r18;
  sl_prs_res_reserve_period_list_r18_l_             sl_prs_res_reserve_period_list_r18;
  sl_prs_res_ded_sl_prs_rp_r18_l_                   sl_prs_res_ded_sl_prs_rp_r18;
  sl_prs_pwr_ctrl_r18_s                             sl_prs_pwr_ctrl_r18;
  sl_sensing_win_ded_sl_prs_rp_r18_e_               sl_sensing_win_ded_sl_prs_rp_r18;
  sl_tx_percentage_ded_sl_prs_rp_list_r18_l_        sl_tx_percentage_ded_sl_prs_rp_list_r18;
  bool                                              sl_sci_based_sl_prs_tx_trigger_sci1_b_r18 = false;
  uint8_t                                           sl_num_subch_ded_sl_prs_rp_r18            = 1;
  sl_subch_size_ded_sl_prs_rp_r18_e_                sl_subch_size_ded_sl_prs_rp_r18;
  sl_max_num_per_reserve_ded_sl_prs_rp_r18_e_       sl_max_num_per_reserve_ded_sl_prs_rp_r18;
  uint8_t                                           sl_num_reserved_bits_sci1_b_ded_sl_prs_rp_r18 = 0;
  sl_src_id_len_ded_sl_prs_rp_r18_e_                sl_src_id_len_ded_sl_prs_rp_r18;
  sl_cbr_prio_tx_cfg_ded_sl_prs_rp_list_r18_l_      sl_cbr_prio_tx_cfg_ded_sl_prs_rp_list_r18;
  sl_time_win_size_cbr_ded_sl_prs_rp_r18_e_         sl_time_win_size_cbr_ded_sl_prs_rp_r18;
  sl_time_win_size_cr_ded_sl_prs_rp_r18_e_          sl_time_win_size_cr_ded_sl_prs_rp_r18;
  sl_cbr_common_tx_ded_sl_prs_rp_list_r18_s         sl_cbr_common_tx_ded_sl_prs_rp_list_r18;
  uint8_t                                           sl_prio_thres_ul_urllc_r18 = 1;
  uint8_t                                           sl_prio_thres_r18          = 1;
  sl_sel_win_list_ded_sl_prs_rp_r18_l_              sl_sel_win_list_ded_sl_prs_rp_r18;
  sl_thres_rsrp_list_ded_sl_prs_rp_r18_l_           sl_thres_rsrp_list_ded_sl_prs_rp_r18;
  sl_preemption_enable_ded_sl_prs_rp_r18_e_         sl_preemption_enable_ded_sl_prs_rp_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PRS-ResourcePoolConfig-r18 ::= SEQUENCE
struct sl_prs_res_pool_cfg_r18_s {
  bool                  sl_prs_res_pool_r18_present = false;
  uint8_t               sl_prs_res_pool_id_r18      = 1;
  sl_prs_res_pool_r18_s sl_prs_res_pool_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PRS-TxPoolDedicated-r18 ::= SEQUENCE
struct sl_prs_tx_pool_ded_r18_s {
  using sl_prs_pool_to_release_list_r1_l_  = bounded_array<uint8_t, 8>;
  using sl_prs_pool_to_add_mod_list_r18_l_ = dyn_array<sl_prs_res_pool_cfg_r18_s>;

  // member variables
  sl_prs_pool_to_release_list_r1_l_  sl_prs_pool_to_release_list_r1;
  sl_prs_pool_to_add_mod_list_r18_l_ sl_prs_pool_to_add_mod_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-BWP-PRS-PoolConfig-r18 ::= SEQUENCE
struct sl_bwp_prs_pool_cfg_r18_s {
  using sl_prs_rx_pool_r18_l_ = dyn_array<sl_prs_res_pool_r18_s>;

  // member variables
  bool                      sl_prs_tx_pool_sel_normal_r18_present  = false;
  bool                      sl_prs_tx_pool_sched_r18_present       = false;
  bool                      sl_prs_tx_pool_exceptional_r18_present = false;
  sl_prs_rx_pool_r18_l_     sl_prs_rx_pool_r18;
  sl_prs_tx_pool_ded_r18_s  sl_prs_tx_pool_sel_normal_r18;
  sl_prs_tx_pool_ded_r18_s  sl_prs_tx_pool_sched_r18;
  sl_prs_res_pool_cfg_r18_s sl_prs_tx_pool_exceptional_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-BWP-Config-r16 ::= SEQUENCE
struct sl_bwp_cfg_r16_s {
  bool                  sl_bwp_generic_r16_present  = false;
  bool                  sl_bwp_pool_cfg_r16_present = false;
  uint8_t               sl_bwp_id                   = 0;
  sl_bwp_generic_r16_s  sl_bwp_generic_r16;
  sl_bwp_pool_cfg_r16_s sl_bwp_pool_cfg_r16;
  // ...
  // group 0
  copy_ptr<setup_release_c<sl_bwp_pool_cfg_r16_s>>      sl_bwp_pool_cfg_ps_r17;
  copy_ptr<setup_release_c<sl_bwp_disc_pool_cfg_r17_s>> sl_bwp_disc_pool_cfg_r17;
  // group 1
  copy_ptr<setup_release_c<sl_bwp_pool_cfg_r16_s>>     sl_bwp_pool_cfg_a2_x_r18;
  copy_ptr<setup_release_c<sl_bwp_prs_pool_cfg_r18_s>> sl_bwp_prs_pool_cfg_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SSB-TimeAllocation-r16 ::= SEQUENCE
struct sl_ssb_time_alloc_r16_s {
  struct sl_num_ssb_within_period_r16_opts {
    enum options { n1, n2, n4, n8, n16, n32, n64, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_num_ssb_within_period_r16_e_ = enumerated<sl_num_ssb_within_period_r16_opts>;

  // member variables
  bool                            sl_num_ssb_within_period_r16_present = false;
  bool                            sl_time_offset_ssb_r16_present       = false;
  bool                            sl_time_interv_r16_present           = false;
  sl_num_ssb_within_period_r16_e_ sl_num_ssb_within_period_r16;
  uint16_t                        sl_time_offset_ssb_r16 = 0;
  uint16_t                        sl_time_interv_r16     = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SyncConfig-r16 ::= SEQUENCE
struct sl_sync_cfg_r16_s {
  struct sl_sync_ref_min_hyst_r16_opts {
    enum options { db0, db3, db6, db9, db12, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_sync_ref_min_hyst_r16_e_ = enumerated<sl_sync_ref_min_hyst_r16_opts>;
  struct sl_sync_ref_diff_hyst_r16_opts {
    enum options { db0, db3, db6, db9, db12, dbinf, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_sync_ref_diff_hyst_r16_e_ = enumerated<sl_sync_ref_diff_hyst_r16_opts>;
  struct tx_params_r16_s_ {
    bool               sync_tx_thresh_ic_r16_present   = false;
    bool               sync_tx_thresh_oo_c_r16_present = false;
    bool               sync_info_reserved_r16_present  = false;
    uint8_t            sync_tx_thresh_ic_r16           = 0;
    uint8_t            sync_tx_thresh_oo_c_r16         = 0;
    fixed_bitstring<2> sync_info_reserved_r16;
  };

  // member variables
  bool                         sl_sync_ref_min_hyst_r16_present  = false;
  bool                         sl_sync_ref_diff_hyst_r16_present = false;
  bool                         sl_filt_coef_r16_present          = false;
  bool                         sl_ssb_time_alloc1_r16_present    = false;
  bool                         sl_ssb_time_alloc2_r16_present    = false;
  bool                         sl_ssb_time_alloc3_r16_present    = false;
  bool                         sl_s_si_d_r16_present             = false;
  bool                         gnss_sync_r16_present             = false;
  sl_sync_ref_min_hyst_r16_e_  sl_sync_ref_min_hyst_r16;
  sl_sync_ref_diff_hyst_r16_e_ sl_sync_ref_diff_hyst_r16;
  filt_coef_e                  sl_filt_coef_r16;
  sl_ssb_time_alloc_r16_s      sl_ssb_time_alloc1_r16;
  sl_ssb_time_alloc_r16_s      sl_ssb_time_alloc2_r16;
  sl_ssb_time_alloc_r16_s      sl_ssb_time_alloc3_r16;
  uint16_t                     sl_s_si_d_r16 = 0;
  tx_params_r16_s_             tx_params_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SyncConfigList-r16 ::= SEQUENCE (SIZE (1..16)) OF SL-SyncConfig-r16
using sl_sync_cfg_list_r16_l = dyn_array<sl_sync_cfg_r16_s>;

// SL-FreqConfig-r16 ::= SEQUENCE
struct sl_freq_cfg_r16_s {
  using sl_scs_specific_carrier_list_r16_l_ = dyn_array<scs_specific_carrier_s>;
  using sl_bwp_to_release_list_r16_l_       = bounded_array<uint8_t, 4>;
  using sl_bwp_to_add_mod_list_r16_l_       = dyn_array<sl_bwp_cfg_r16_s>;
  struct sl_sync_prio_r16_opts {
    enum options { gnss, gnb_enb, nulltype } value;

    const char* to_string() const;
  };
  using sl_sync_prio_r16_e_ = enumerated<sl_sync_prio_r16_opts>;

  // member variables
  bool                                sl_absolute_freq_point_a_r16_present = false;
  bool                                sl_absolute_freq_ssb_r16_present     = false;
  bool                                freq_shift7p5khz_sl_r16_present      = false;
  bool                                sl_sync_prio_r16_present             = false;
  uint8_t                             sl_freq_id_r16                       = 1;
  sl_scs_specific_carrier_list_r16_l_ sl_scs_specific_carrier_list_r16;
  uint32_t                            sl_absolute_freq_point_a_r16 = 0;
  uint32_t                            sl_absolute_freq_ssb_r16     = 0;
  int8_t                              value_n_r16                  = -1;
  sl_bwp_to_release_list_r16_l_       sl_bwp_to_release_list_r16;
  sl_bwp_to_add_mod_list_r16_l_       sl_bwp_to_add_mod_list_r16;
  sl_sync_cfg_list_r16_l              sl_sync_cfg_list_r16;
  sl_sync_prio_r16_e_                 sl_sync_prio_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RLC-Config-r16 ::= CHOICE
struct sl_rlc_cfg_r16_c {
  struct sl_am_rlc_r16_s_ {
    struct sl_max_retx_thres_r16_opts {
      enum options { t1, t2, t3, t4, t6, t8, t16, t32, nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using sl_max_retx_thres_r16_e_ = enumerated<sl_max_retx_thres_r16_opts>;

    // member variables
    bool                     sl_sn_field_len_am_r16_present = false;
    sn_field_len_am_e        sl_sn_field_len_am_r16;
    t_poll_retx_e            sl_t_poll_retx_r16;
    poll_pdu_e               sl_poll_pdu_r16;
    poll_byte_e              sl_poll_byte_r16;
    sl_max_retx_thres_r16_e_ sl_max_retx_thres_r16;
    // ...
  };
  struct sl_um_rlc_r16_s_ {
    bool              sl_sn_field_len_um_r16_present = false;
    sn_field_len_um_e sl_sn_field_len_um_r16;
    // ...
  };
  struct types_opts {
    enum options { sl_am_rlc_r16, sl_um_rlc_r16, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  sl_rlc_cfg_r16_c() = default;
  sl_rlc_cfg_r16_c(const sl_rlc_cfg_r16_c& other);
  sl_rlc_cfg_r16_c& operator=(const sl_rlc_cfg_r16_c& other);
  ~sl_rlc_cfg_r16_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  sl_am_rlc_r16_s_& sl_am_rlc_r16()
  {
    assert_choice_type(types::sl_am_rlc_r16, type_, "SL-RLC-Config-r16");
    return c.get<sl_am_rlc_r16_s_>();
  }
  sl_um_rlc_r16_s_& sl_um_rlc_r16()
  {
    assert_choice_type(types::sl_um_rlc_r16, type_, "SL-RLC-Config-r16");
    return c.get<sl_um_rlc_r16_s_>();
  }
  const sl_am_rlc_r16_s_& sl_am_rlc_r16() const
  {
    assert_choice_type(types::sl_am_rlc_r16, type_, "SL-RLC-Config-r16");
    return c.get<sl_am_rlc_r16_s_>();
  }
  const sl_um_rlc_r16_s_& sl_um_rlc_r16() const
  {
    assert_choice_type(types::sl_um_rlc_r16, type_, "SL-RLC-Config-r16");
    return c.get<sl_um_rlc_r16_s_>();
  }
  sl_am_rlc_r16_s_& set_sl_am_rlc_r16();
  sl_um_rlc_r16_s_& set_sl_um_rlc_r16();

private:
  types                                               type_;
  choice_buffer_t<sl_am_rlc_r16_s_, sl_um_rlc_r16_s_> c;

  void destroy_();
};

// SL-LogicalChannelConfig-r16 ::= SEQUENCE
struct sl_lc_ch_cfg_r16_s {
  struct sl_prioritised_bit_rate_r16_opts {
    enum options {
      kbps0,
      kbps8,
      kbps16,
      kbps32,
      kbps64,
      kbps128,
      kbps256,
      kbps512,
      kbps1024,
      kbps2048,
      kbps4096,
      kbps8192,
      kbps16384,
      kbps32768,
      kbps65536,
      infinity,
      nulltype
    } value;
    typedef int32_t number_type;

    const char* to_string() const;
    int32_t     to_number() const;
  };
  using sl_prioritised_bit_rate_r16_e_ = enumerated<sl_prioritised_bit_rate_r16_opts>;
  struct sl_bucket_size_dur_r16_opts {
    enum options {
      ms5,
      ms10,
      ms20,
      ms50,
      ms100,
      ms150,
      ms300,
      ms500,
      ms1000,
      spare7,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_bucket_size_dur_r16_e_ = enumerated<sl_bucket_size_dur_r16_opts>;
  struct sl_harq_feedback_enabled_r16_opts {
    enum options { enabled, disabled, nulltype } value;

    const char* to_string() const;
  };
  using sl_harq_feedback_enabled_r16_e_ = enumerated<sl_harq_feedback_enabled_r16_opts>;
  using sl_allowed_cg_list_r16_l_       = bounded_array<uint8_t, 7>;
  using sl_allowed_scs_list_r16_l_      = bounded_array<subcarrier_spacing_e, 5>;
  struct sl_max_pusch_dur_r16_opts {
    enum options { ms0p02, ms0p04, ms0p0625, ms0p125, ms0p25, ms0p5, spare2, spare1, nulltype } value;

    const char* to_string() const;
  };
  using sl_max_pusch_dur_r16_e_    = enumerated<sl_max_pusch_dur_r16_opts>;
  using sl_allowed_carriers_r18_l_ = bounded_array<uint8_t, 8>;

  // member variables
  bool                            sl_cfg_grant_type1_allowed_r16_present      = false;
  bool                            sl_harq_feedback_enabled_r16_present        = false;
  bool                            sl_allowed_cg_list_r16_present              = false;
  bool                            sl_max_pusch_dur_r16_present                = false;
  bool                            sl_lc_ch_group_r16_present                  = false;
  bool                            sl_sched_request_id_r16_present             = false;
  bool                            sl_lc_ch_sr_delay_timer_applied_r16_present = false;
  uint8_t                         sl_prio_r16                                 = 1;
  sl_prioritised_bit_rate_r16_e_  sl_prioritised_bit_rate_r16;
  sl_bucket_size_dur_r16_e_       sl_bucket_size_dur_r16;
  sl_harq_feedback_enabled_r16_e_ sl_harq_feedback_enabled_r16;
  sl_allowed_cg_list_r16_l_       sl_allowed_cg_list_r16;
  sl_allowed_scs_list_r16_l_      sl_allowed_scs_list_r16;
  sl_max_pusch_dur_r16_e_         sl_max_pusch_dur_r16;
  uint8_t                         sl_lc_ch_group_r16                  = 0;
  uint8_t                         sl_sched_request_id_r16             = 0;
  bool                            sl_lc_ch_sr_delay_timer_applied_r16 = false;
  // ...
  // group 0
  bool                                 sl_ch_access_prio_r18_present = false;
  uint8_t                              sl_ch_access_prio_r18         = 1;
  copy_ptr<sl_allowed_carriers_r18_l_> sl_allowed_carriers_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RLC-BearerConfig-r16 ::= SEQUENCE
struct sl_rlc_bearer_cfg_r16_s {
  bool               sl_served_radio_bearer_r16_present = false;
  bool               sl_rlc_cfg_r16_present             = false;
  bool               sl_mac_lc_ch_cfg_r16_present       = false;
  uint16_t           sl_rlc_bearer_cfg_idx_r16          = 1;
  uint16_t           sl_served_radio_bearer_r16         = 1;
  sl_rlc_cfg_r16_c   sl_rlc_cfg_r16;
  sl_lc_ch_cfg_r16_s sl_mac_lc_ch_cfg_r16;
  // ...
  // group 0
  bool     sl_rlc_bearer_cfg_idx_v1800_present = false;
  uint16_t sl_rlc_bearer_cfg_idx_v1800         = 513;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PHY-MAC-RLC-Config-r16 ::= SEQUENCE
struct sl_phy_mac_rlc_cfg_r16_s {
  using sl_freq_info_to_release_list_r16_l_  = bounded_array<uint8_t, 8>;
  using sl_freq_info_to_add_mod_list_r16_l_  = dyn_array<sl_freq_cfg_r16_s>;
  using sl_rlc_bearer_to_release_list_r16_l_ = dyn_array<uint16_t>;
  using sl_rlc_bearer_to_add_mod_list_r16_l_ = dyn_array<sl_rlc_bearer_cfg_r16_s>;
  struct sl_max_num_consecutive_dtx_r16_opts {
    enum options { n1, n2, n3, n4, n6, n8, n16, n32, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_max_num_consecutive_dtx_r16_e_ = enumerated<sl_max_num_consecutive_dtx_r16_opts>;
  struct network_ctrl_sync_tx_r16_opts {
    enum options { on, off, nulltype } value;

    const char* to_string() const;
  };
  using network_ctrl_sync_tx_r16_e_ = enumerated<network_ctrl_sync_tx_r16_opts>;

  // member variables
  bool                                    sl_sched_cfg_r16_present               = false;
  bool                                    sl_ue_sel_cfg_r16_present              = false;
  bool                                    sl_max_num_consecutive_dtx_r16_present = false;
  bool                                    sl_csi_acquisition_r16_present         = false;
  bool                                    sl_csi_sched_request_id_r16_present    = false;
  bool                                    sl_ssb_prio_nr_r16_present             = false;
  bool                                    network_ctrl_sync_tx_r16_present       = false;
  setup_release_c<sl_sched_cfg_r16_s>     sl_sched_cfg_r16;
  setup_release_c<sl_ue_sel_cfg_r16_s>    sl_ue_sel_cfg_r16;
  sl_freq_info_to_release_list_r16_l_     sl_freq_info_to_release_list_r16;
  sl_freq_info_to_add_mod_list_r16_l_     sl_freq_info_to_add_mod_list_r16;
  sl_rlc_bearer_to_release_list_r16_l_    sl_rlc_bearer_to_release_list_r16;
  sl_rlc_bearer_to_add_mod_list_r16_l_    sl_rlc_bearer_to_add_mod_list_r16;
  sl_max_num_consecutive_dtx_r16_e_       sl_max_num_consecutive_dtx_r16;
  setup_release_c<integer<uint8_t, 0, 7>> sl_csi_sched_request_id_r16;
  uint8_t                                 sl_ssb_prio_nr_r16 = 1;
  network_ctrl_sync_tx_r16_e_             network_ctrl_sync_tx_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PQI-r16 ::= CHOICE
struct sl_pqi_r16_c {
  struct sl_non_standardized_pqi_r16_s_ {
    struct sl_res_type_r16_opts {
      enum options { gbr, non_gbr, delay_crit_gbr, spare1, nulltype } value;

      const char* to_string() const;
    };
    using sl_res_type_r16_e_ = enumerated<sl_res_type_r16_opts>;

    // member variables
    bool               sl_res_type_r16_present              = false;
    bool               sl_prio_level_r16_present            = false;
    bool               sl_packet_delay_budget_r16_present   = false;
    bool               sl_packet_error_rate_r16_present     = false;
    bool               sl_averaging_win_r16_present         = false;
    bool               sl_max_data_burst_volume_r16_present = false;
    sl_res_type_r16_e_ sl_res_type_r16;
    uint8_t            sl_prio_level_r16            = 1;
    uint16_t           sl_packet_delay_budget_r16   = 0;
    uint8_t            sl_packet_error_rate_r16     = 0;
    uint16_t           sl_averaging_win_r16         = 0;
    uint16_t           sl_max_data_burst_volume_r16 = 0;
    // ...
  };
  struct types_opts {
    enum options { sl_standardized_pqi_r16, sl_non_standardized_pqi_r16, nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts>;

  // choice methods
  sl_pqi_r16_c() = default;
  sl_pqi_r16_c(const sl_pqi_r16_c& other);
  sl_pqi_r16_c& operator=(const sl_pqi_r16_c& other);
  ~sl_pqi_r16_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint16_t& sl_standardized_pqi_r16()
  {
    assert_choice_type(types::sl_standardized_pqi_r16, type_, "SL-PQI-r16");
    return c.get<uint16_t>();
  }
  sl_non_standardized_pqi_r16_s_& sl_non_standardized_pqi_r16()
  {
    assert_choice_type(types::sl_non_standardized_pqi_r16, type_, "SL-PQI-r16");
    return c.get<sl_non_standardized_pqi_r16_s_>();
  }
  const uint16_t& sl_standardized_pqi_r16() const
  {
    assert_choice_type(types::sl_standardized_pqi_r16, type_, "SL-PQI-r16");
    return c.get<uint16_t>();
  }
  const sl_non_standardized_pqi_r16_s_& sl_non_standardized_pqi_r16() const
  {
    assert_choice_type(types::sl_non_standardized_pqi_r16, type_, "SL-PQI-r16");
    return c.get<sl_non_standardized_pqi_r16_s_>();
  }
  uint16_t&                       set_sl_standardized_pqi_r16();
  sl_non_standardized_pqi_r16_s_& set_sl_non_standardized_pqi_r16();

private:
  types                                           type_;
  choice_buffer_t<sl_non_standardized_pqi_r16_s_> c;

  void destroy_();
};

// SL-QoS-Profile-r16 ::= SEQUENCE
struct sl_qos_profile_r16_s {
  bool         sl_pqi_r16_present   = false;
  bool         sl_gfbr_r16_present  = false;
  bool         sl_mfbr_r16_present  = false;
  bool         sl_range_r16_present = false;
  sl_pqi_r16_c sl_pqi_r16;
  uint32_t     sl_gfbr_r16  = 0;
  uint32_t     sl_mfbr_r16  = 0;
  uint16_t     sl_range_r16 = 1;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MappedQoS-FlowsListDedicated-r16 ::= SEQUENCE
struct sl_mapped_qos_flows_list_ded_r16_s {
  using sl_mapped_qos_flows_to_add_list_r16_l_     = dyn_array<uint16_t>;
  using sl_mapped_qos_flows_to_release_list_r16_l_ = dyn_array<uint16_t>;

  // member variables
  sl_mapped_qos_flows_to_add_list_r16_l_     sl_mapped_qos_flows_to_add_list_r16;
  sl_mapped_qos_flows_to_release_list_r16_l_ sl_mapped_qos_flows_to_release_list_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SDAP-Config-r16 ::= SEQUENCE
struct sl_sdap_cfg_r16_s {
  struct sl_sdap_hdr_r16_opts {
    enum options { present, absent, nulltype } value;

    const char* to_string() const;
  };
  using sl_sdap_hdr_r16_e_ = enumerated<sl_sdap_hdr_r16_opts>;
  struct sl_mapped_qos_flows_r16_c_ {
    using sl_mapped_qos_flows_list_r16_l_ = dyn_array<sl_qos_profile_r16_s>;
    struct types_opts {
      enum options { sl_mapped_qos_flows_list_r16, sl_mapped_qos_flows_list_ded_r16, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    sl_mapped_qos_flows_r16_c_() = default;
    sl_mapped_qos_flows_r16_c_(const sl_mapped_qos_flows_r16_c_& other);
    sl_mapped_qos_flows_r16_c_& operator=(const sl_mapped_qos_flows_r16_c_& other);
    ~sl_mapped_qos_flows_r16_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    sl_mapped_qos_flows_list_r16_l_& sl_mapped_qos_flows_list_r16()
    {
      assert_choice_type(types::sl_mapped_qos_flows_list_r16, type_, "sl-MappedQoS-Flows-r16");
      return c.get<sl_mapped_qos_flows_list_r16_l_>();
    }
    sl_mapped_qos_flows_list_ded_r16_s& sl_mapped_qos_flows_list_ded_r16()
    {
      assert_choice_type(types::sl_mapped_qos_flows_list_ded_r16, type_, "sl-MappedQoS-Flows-r16");
      return c.get<sl_mapped_qos_flows_list_ded_r16_s>();
    }
    const sl_mapped_qos_flows_list_r16_l_& sl_mapped_qos_flows_list_r16() const
    {
      assert_choice_type(types::sl_mapped_qos_flows_list_r16, type_, "sl-MappedQoS-Flows-r16");
      return c.get<sl_mapped_qos_flows_list_r16_l_>();
    }
    const sl_mapped_qos_flows_list_ded_r16_s& sl_mapped_qos_flows_list_ded_r16() const
    {
      assert_choice_type(types::sl_mapped_qos_flows_list_ded_r16, type_, "sl-MappedQoS-Flows-r16");
      return c.get<sl_mapped_qos_flows_list_ded_r16_s>();
    }
    sl_mapped_qos_flows_list_r16_l_&    set_sl_mapped_qos_flows_list_r16();
    sl_mapped_qos_flows_list_ded_r16_s& set_sl_mapped_qos_flows_list_ded_r16();

  private:
    types                                                                                type_;
    choice_buffer_t<sl_mapped_qos_flows_list_ded_r16_s, sl_mapped_qos_flows_list_r16_l_> c;

    void destroy_();
  };
  struct sl_cast_type_r16_opts {
    enum options { broadcast, groupcast, unicast, spare1, nulltype } value;

    const char* to_string() const;
  };
  using sl_cast_type_r16_e_ = enumerated<sl_cast_type_r16_opts>;

  // member variables
  bool                       sl_mapped_qos_flows_r16_present = false;
  bool                       sl_cast_type_r16_present        = false;
  sl_sdap_hdr_r16_e_         sl_sdap_hdr_r16;
  bool                       sl_default_rb_r16 = false;
  sl_mapped_qos_flows_r16_c_ sl_mapped_qos_flows_r16;
  sl_cast_type_r16_e_        sl_cast_type_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PDCP-Config-r16 ::= SEQUENCE
struct sl_pdcp_cfg_r16_s {
  struct sl_discard_timer_r16_opts {
    enum options {
      ms3,
      ms10,
      ms20,
      ms25,
      ms30,
      ms40,
      ms50,
      ms60,
      ms75,
      ms100,
      ms150,
      ms200,
      ms250,
      ms300,
      ms500,
      ms750,
      ms1500,
      infinity,
      nulltype
    } value;
    typedef int16_t number_type;

    const char* to_string() const;
    int16_t     to_number() const;
  };
  using sl_discard_timer_r16_e_ = enumerated<sl_discard_timer_r16_opts>;
  struct sl_pdcp_sn_size_r16_opts {
    enum options { len12bits, len18bits, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_pdcp_sn_size_r16_e_ = enumerated<sl_pdcp_sn_size_r16_opts>;

  // member variables
  bool                    sl_discard_timer_r16_present     = false;
  bool                    sl_pdcp_sn_size_r16_present      = false;
  bool                    sl_out_of_order_delivery_present = false;
  sl_discard_timer_r16_e_ sl_discard_timer_r16;
  sl_pdcp_sn_size_r16_e_  sl_pdcp_sn_size_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RadioBearerConfig-r16 ::= SEQUENCE
struct sl_radio_bearer_cfg_r16_s {
  struct sl_trans_range_r16_opts {
    enum options {
      m20,
      m50,
      m80,
      m100,
      m120,
      m150,
      m180,
      m200,
      m220,
      m250,
      m270,
      m300,
      m350,
      m370,
      m400,
      m420,
      m450,
      m480,
      m500,
      m550,
      m600,
      m700,
      m1000,
      spare9,
      spare8,
      spare7,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_trans_range_r16_e_ = enumerated<sl_trans_range_r16_opts>;

  // member variables
  bool                  sl_sdap_cfg_r16_present    = false;
  bool                  sl_pdcp_cfg_r16_present    = false;
  bool                  sl_trans_range_r16_present = false;
  uint16_t              slrb_uu_cfg_idx_r16        = 1;
  sl_sdap_cfg_r16_s     sl_sdap_cfg_r16;
  sl_pdcp_cfg_r16_s     sl_pdcp_cfg_r16;
  sl_trans_range_r16_e_ sl_trans_range_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MeasObjectToRemoveList-r16 ::= SEQUENCE (SIZE (1..64)) OF INTEGER (1..64)
using sl_meas_obj_to_rem_list_r16_l = dyn_array<uint8_t>;

// SL-MeasObject-r16 ::= SEQUENCE
struct sl_meas_obj_r16_s {
  uint32_t freq_info_sl_r16 = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MeasObjectInfo-r16 ::= SEQUENCE
struct sl_meas_obj_info_r16_s {
  uint8_t           sl_meas_obj_id_r16 = 1;
  sl_meas_obj_r16_s sl_meas_obj_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MeasObjectList-r16 ::= SEQUENCE (SIZE (1..64)) OF SL-MeasObjectInfo-r16
using sl_meas_obj_list_r16_l = dyn_array<sl_meas_obj_info_r16_s>;

// SL-ReportConfigToRemoveList-r16 ::= SEQUENCE (SIZE (1..64)) OF INTEGER (1..64)
using sl_report_cfg_to_rem_list_r16_l = dyn_array<uint8_t>;

// SL-RS-Type-r16 ::= ENUMERATED
struct sl_rs_type_r16_opts {
  enum options { dmrs, sl_prs, spare2, spare1, nulltype } value;

  const char* to_string() const;
};
using sl_rs_type_r16_e = enumerated<sl_rs_type_r16_opts>;

// SL-PeriodicalReportConfig-r16 ::= SEQUENCE
struct sl_periodical_report_cfg_r16_s {
  struct sl_report_amount_r16_opts {
    enum options { r1, r2, r4, r8, r16, r32, r64, infinity, nulltype } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using sl_report_amount_r16_e_ = enumerated<sl_report_amount_r16_opts>;

  // member variables
  report_interv_e            sl_report_interv_r16;
  sl_report_amount_r16_e_    sl_report_amount_r16;
  sl_meas_report_quant_r16_c sl_report_quant_r16;
  sl_rs_type_r16_e           sl_rs_type_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-EventTriggerConfig-r16 ::= SEQUENCE
struct sl_event_trigger_cfg_r16_s {
  struct sl_event_id_r16_c_ {
    struct event_s1_r16_s_ {
      sl_meas_trigger_quant_r16_c s1_thres_r16;
      bool                        sl_report_on_leave_r16 = false;
      uint8_t                     sl_hysteresis_r16      = 0;
      time_to_trigger_e           sl_time_to_trigger_r16;
      // ...
    };
    struct event_s2_r16_s_ {
      sl_meas_trigger_quant_r16_c s2_thres_r16;
      bool                        sl_report_on_leave_r16 = false;
      uint8_t                     sl_hysteresis_r16      = 0;
      time_to_trigger_e           sl_time_to_trigger_r16;
      // ...
    };
    struct types_opts {
      enum options { event_s1_r16, event_s2_r16, /*...*/ nulltype } value;
      typedef uint8_t number_type;

      const char* to_string() const;
      uint8_t     to_number() const;
    };
    using types = enumerated<types_opts, true>;

    // choice methods
    sl_event_id_r16_c_() = default;
    sl_event_id_r16_c_(const sl_event_id_r16_c_& other);
    sl_event_id_r16_c_& operator=(const sl_event_id_r16_c_& other);
    ~sl_event_id_r16_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    event_s1_r16_s_& event_s1_r16()
    {
      assert_choice_type(types::event_s1_r16, type_, "sl-EventId-r16");
      return c.get<event_s1_r16_s_>();
    }
    event_s2_r16_s_& event_s2_r16()
    {
      assert_choice_type(types::event_s2_r16, type_, "sl-EventId-r16");
      return c.get<event_s2_r16_s_>();
    }
    const event_s1_r16_s_& event_s1_r16() const
    {
      assert_choice_type(types::event_s1_r16, type_, "sl-EventId-r16");
      return c.get<event_s1_r16_s_>();
    }
    const event_s2_r16_s_& event_s2_r16() const
    {
      assert_choice_type(types::event_s2_r16, type_, "sl-EventId-r16");
      return c.get<event_s2_r16_s_>();
    }
    event_s1_r16_s_& set_event_s1_r16();
    event_s2_r16_s_& set_event_s2_r16();

  private:
    types                                             type_;
    choice_buffer_t<event_s1_r16_s_, event_s2_r16_s_> c;

    void destroy_();
  };
  struct sl_report_amount_r16_opts {
    enum options { r1, r2, r4, r8, r16, r32, r64, infinity, nulltype } value;
    typedef int8_t number_type;

    const char* to_string() const;
    int8_t      to_number() const;
  };
  using sl_report_amount_r16_e_ = enumerated<sl_report_amount_r16_opts>;

  // member variables
  sl_event_id_r16_c_         sl_event_id_r16;
  report_interv_e            sl_report_interv_r16;
  sl_report_amount_r16_e_    sl_report_amount_r16;
  sl_meas_report_quant_r16_c sl_report_quant_r16;
  sl_rs_type_r16_e           sl_rs_type_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ReportConfig-r16 ::= SEQUENCE
struct sl_report_cfg_r16_s {
  struct sl_report_type_r16_c_ {
    struct types_opts {
      enum options { sl_periodical_r16, sl_event_triggered_r16, /*...*/ nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts, true>;

    // choice methods
    sl_report_type_r16_c_() = default;
    sl_report_type_r16_c_(const sl_report_type_r16_c_& other);
    sl_report_type_r16_c_& operator=(const sl_report_type_r16_c_& other);
    ~sl_report_type_r16_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    sl_periodical_report_cfg_r16_s& sl_periodical_r16()
    {
      assert_choice_type(types::sl_periodical_r16, type_, "sl-ReportType-r16");
      return c.get<sl_periodical_report_cfg_r16_s>();
    }
    sl_event_trigger_cfg_r16_s& sl_event_triggered_r16()
    {
      assert_choice_type(types::sl_event_triggered_r16, type_, "sl-ReportType-r16");
      return c.get<sl_event_trigger_cfg_r16_s>();
    }
    const sl_periodical_report_cfg_r16_s& sl_periodical_r16() const
    {
      assert_choice_type(types::sl_periodical_r16, type_, "sl-ReportType-r16");
      return c.get<sl_periodical_report_cfg_r16_s>();
    }
    const sl_event_trigger_cfg_r16_s& sl_event_triggered_r16() const
    {
      assert_choice_type(types::sl_event_triggered_r16, type_, "sl-ReportType-r16");
      return c.get<sl_event_trigger_cfg_r16_s>();
    }
    sl_periodical_report_cfg_r16_s& set_sl_periodical_r16();
    sl_event_trigger_cfg_r16_s&     set_sl_event_triggered_r16();

  private:
    types                                                                       type_;
    choice_buffer_t<sl_event_trigger_cfg_r16_s, sl_periodical_report_cfg_r16_s> c;

    void destroy_();
  };

  // member variables
  sl_report_type_r16_c_ sl_report_type_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ReportConfigInfo-r16 ::= SEQUENCE
struct sl_report_cfg_info_r16_s {
  uint8_t             sl_report_cfg_id_r16 = 1;
  sl_report_cfg_r16_s sl_report_cfg_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ReportConfigList-r16 ::= SEQUENCE (SIZE (1..64)) OF SL-ReportConfigInfo-r16
using sl_report_cfg_list_r16_l = dyn_array<sl_report_cfg_info_r16_s>;

// SL-MeasIdToRemoveList-r16 ::= SEQUENCE (SIZE (1..64)) OF INTEGER (1..64)
using sl_meas_id_to_rem_list_r16_l = dyn_array<uint8_t>;

// SL-MeasIdInfo-r16 ::= SEQUENCE
struct sl_meas_id_info_r16_s {
  uint8_t sl_meas_id_r16       = 1;
  uint8_t sl_meas_obj_id_r16   = 1;
  uint8_t sl_report_cfg_id_r16 = 1;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MeasIdList-r16 ::= SEQUENCE (SIZE (1..64)) OF SL-MeasIdInfo-r16
using sl_meas_id_list_r16_l = dyn_array<sl_meas_id_info_r16_s>;

// SL-QuantityConfig-r16 ::= SEQUENCE
struct sl_quant_cfg_r16_s {
  bool        sl_filt_coef_dmrs_r16_present = false;
  filt_coef_e sl_filt_coef_dmrs_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MeasConfig-r16 ::= SEQUENCE
struct sl_meas_cfg_r16_s {
  bool                            sl_quant_cfg_r16_present = false;
  sl_meas_obj_to_rem_list_r16_l   sl_meas_obj_to_rem_list_r16;
  sl_meas_obj_list_r16_l          sl_meas_obj_to_add_mod_list_r16;
  sl_report_cfg_to_rem_list_r16_l sl_report_cfg_to_rem_list_r16;
  sl_report_cfg_list_r16_l        sl_report_cfg_to_add_mod_list_r16;
  sl_meas_id_to_rem_list_r16_l    sl_meas_id_to_rem_list_r16;
  sl_meas_id_list_r16_l           sl_meas_id_to_add_mod_list_r16;
  sl_quant_cfg_r16_s              sl_quant_cfg_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MeasConfigInfo-r16 ::= SEQUENCE
struct sl_meas_cfg_info_r16_s {
  uint8_t           sl_dest_idx_r16 = 0;
  sl_meas_cfg_r16_s sl_meas_cfg_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DRX-GC-BC-QoS-r17 ::= SEQUENCE
struct sl_drx_gc_bc_qos_r17_s {
  using sl_drx_gc_bc_mapped_qos_flow_list_r17_l_ = dyn_array<sl_qos_profile_r16_s>;
  struct sl_drx_gc_bc_on_dur_timer_r17_c_ {
    struct milli_seconds_opts {
      enum options {
        ms1,
        ms2,
        ms3,
        ms4,
        ms5,
        ms6,
        ms8,
        ms10,
        ms20,
        ms30,
        ms40,
        ms50,
        ms60,
        ms80,
        ms100,
        ms200,
        ms300,
        ms400,
        ms500,
        ms600,
        ms800,
        ms1000,
        ms1200,
        ms1600,
        spare8,
        spare7,
        spare6,
        spare5,
        spare4,
        spare3,
        spare2,
        spare1,
        nulltype
      } value;
      typedef uint16_t number_type;

      const char* to_string() const;
      uint16_t    to_number() const;
    };
    using milli_seconds_e_ = enumerated<milli_seconds_opts>;
    struct types_opts {
      enum options { sub_milli_seconds, milli_seconds, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    sl_drx_gc_bc_on_dur_timer_r17_c_() = default;
    sl_drx_gc_bc_on_dur_timer_r17_c_(const sl_drx_gc_bc_on_dur_timer_r17_c_& other);
    sl_drx_gc_bc_on_dur_timer_r17_c_& operator=(const sl_drx_gc_bc_on_dur_timer_r17_c_& other);
    ~sl_drx_gc_bc_on_dur_timer_r17_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint8_t& sub_milli_seconds()
    {
      assert_choice_type(types::sub_milli_seconds, type_, "sl-DRX-GC-BC-OnDurationTimer-r17");
      return c.get<uint8_t>();
    }
    milli_seconds_e_& milli_seconds()
    {
      assert_choice_type(types::milli_seconds, type_, "sl-DRX-GC-BC-OnDurationTimer-r17");
      return c.get<milli_seconds_e_>();
    }
    const uint8_t& sub_milli_seconds() const
    {
      assert_choice_type(types::sub_milli_seconds, type_, "sl-DRX-GC-BC-OnDurationTimer-r17");
      return c.get<uint8_t>();
    }
    const milli_seconds_e_& milli_seconds() const
    {
      assert_choice_type(types::milli_seconds, type_, "sl-DRX-GC-BC-OnDurationTimer-r17");
      return c.get<milli_seconds_e_>();
    }
    uint8_t&          set_sub_milli_seconds();
    milli_seconds_e_& set_milli_seconds();

  private:
    types               type_;
    pod_choice_buffer_t c;

    void destroy_();
  };
  struct sl_drx_gc_inactivity_timer_r17_opts {
    enum options {
      ms0,
      ms1,
      ms2,
      ms3,
      ms4,
      ms5,
      ms6,
      ms8,
      ms10,
      ms20,
      ms30,
      ms40,
      ms50,
      ms60,
      ms80,
      ms100,
      ms200,
      ms300,
      ms500,
      ms750,
      ms1280,
      ms1920,
      ms2560,
      spare9,
      spare8,
      spare7,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_drx_gc_inactivity_timer_r17_e_ = enumerated<sl_drx_gc_inactivity_timer_r17_opts>;
  struct sl_drx_gc_bc_cycle_r17_opts {
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
      spare12,
      spare11,
      spare10,
      spare9,
      spare8,
      spare7,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_drx_gc_bc_cycle_r17_e_ = enumerated<sl_drx_gc_bc_cycle_r17_opts>;

  // member variables
  sl_drx_gc_bc_mapped_qos_flow_list_r17_l_ sl_drx_gc_bc_mapped_qos_flow_list_r17;
  sl_drx_gc_bc_on_dur_timer_r17_c_         sl_drx_gc_bc_on_dur_timer_r17;
  sl_drx_gc_inactivity_timer_r17_e_        sl_drx_gc_inactivity_timer_r17;
  sl_drx_gc_bc_cycle_r17_e_                sl_drx_gc_bc_cycle_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DRX-GC-Generic-r17 ::= SEQUENCE
struct sl_drx_gc_generic_r17_s {
  struct sl_drx_gc_harq_rtt_timer1_r17_opts {
    enum options { sl0, sl1, sl2, sl4, spare4, spare3, spare2, spare1, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_drx_gc_harq_rtt_timer1_r17_e_ = enumerated<sl_drx_gc_harq_rtt_timer1_r17_opts>;
  struct sl_drx_gc_harq_rtt_timer2_r17_opts {
    enum options { sl0, sl1, sl2, sl4, spare4, spare3, spare2, spare1, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_drx_gc_harq_rtt_timer2_r17_e_ = enumerated<sl_drx_gc_harq_rtt_timer2_r17_opts>;
  struct sl_drx_gc_retx_timer_r17_opts {
    enum options {
      sl0,
      sl1,
      sl2,
      sl4,
      sl6,
      sl8,
      sl16,
      sl24,
      sl33,
      sl40,
      sl64,
      sl80,
      sl96,
      sl112,
      sl128,
      sl160,
      sl320,
      spare15,
      spare14,
      spare13,
      spare12,
      spare11,
      spare10,
      spare9,
      spare8,
      spare7,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_drx_gc_retx_timer_r17_e_ = enumerated<sl_drx_gc_retx_timer_r17_opts>;

  // member variables
  bool                             sl_drx_gc_harq_rtt_timer1_r17_present = false;
  bool                             sl_drx_gc_harq_rtt_timer2_r17_present = false;
  sl_drx_gc_harq_rtt_timer1_r17_e_ sl_drx_gc_harq_rtt_timer1_r17;
  sl_drx_gc_harq_rtt_timer2_r17_e_ sl_drx_gc_harq_rtt_timer2_r17;
  sl_drx_gc_retx_timer_r17_e_      sl_drx_gc_retx_timer_r17;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DRX-ConfigGC-BC-r17 ::= SEQUENCE
struct sl_drx_cfg_gc_bc_r17_s {
  using sl_drx_gc_bc_per_qos_list_r17_l_ = dyn_array<sl_drx_gc_bc_qos_r17_s>;

  // member variables
  bool                             sl_drx_gc_generic_r17_present    = false;
  bool                             sl_default_drx_gc_bc_r17_present = false;
  sl_drx_gc_bc_per_qos_list_r17_l_ sl_drx_gc_bc_per_qos_list_r17;
  sl_drx_gc_generic_r17_s          sl_drx_gc_generic_r17;
  sl_drx_gc_bc_qos_r17_s           sl_default_drx_gc_bc_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DRX-ConfigUC-r17 ::= SEQUENCE
struct sl_drx_cfg_uc_r17_s {
  struct sl_drx_on_dur_timer_r17_c_ {
    struct milli_seconds_opts {
      enum options {
        ms1,
        ms2,
        ms3,
        ms4,
        ms5,
        ms6,
        ms8,
        ms10,
        ms20,
        ms30,
        ms40,
        ms50,
        ms60,
        ms80,
        ms100,
        ms200,
        ms300,
        ms400,
        ms500,
        ms600,
        ms800,
        ms1000,
        ms1200,
        ms1600,
        spare8,
        spare7,
        spare6,
        spare5,
        spare4,
        spare3,
        spare2,
        spare1,
        nulltype
      } value;
      typedef uint16_t number_type;

      const char* to_string() const;
      uint16_t    to_number() const;
    };
    using milli_seconds_e_ = enumerated<milli_seconds_opts>;
    struct types_opts {
      enum options { sub_milli_seconds, milli_seconds, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    sl_drx_on_dur_timer_r17_c_() = default;
    sl_drx_on_dur_timer_r17_c_(const sl_drx_on_dur_timer_r17_c_& other);
    sl_drx_on_dur_timer_r17_c_& operator=(const sl_drx_on_dur_timer_r17_c_& other);
    ~sl_drx_on_dur_timer_r17_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint8_t& sub_milli_seconds()
    {
      assert_choice_type(types::sub_milli_seconds, type_, "sl-drx-onDurationTimer-r17");
      return c.get<uint8_t>();
    }
    milli_seconds_e_& milli_seconds()
    {
      assert_choice_type(types::milli_seconds, type_, "sl-drx-onDurationTimer-r17");
      return c.get<milli_seconds_e_>();
    }
    const uint8_t& sub_milli_seconds() const
    {
      assert_choice_type(types::sub_milli_seconds, type_, "sl-drx-onDurationTimer-r17");
      return c.get<uint8_t>();
    }
    const milli_seconds_e_& milli_seconds() const
    {
      assert_choice_type(types::milli_seconds, type_, "sl-drx-onDurationTimer-r17");
      return c.get<milli_seconds_e_>();
    }
    uint8_t&          set_sub_milli_seconds();
    milli_seconds_e_& set_milli_seconds();

  private:
    types               type_;
    pod_choice_buffer_t c;

    void destroy_();
  };
  struct sl_drx_inactivity_timer_r17_opts {
    enum options {
      ms0,
      ms1,
      ms2,
      ms3,
      ms4,
      ms5,
      ms6,
      ms8,
      ms10,
      ms20,
      ms30,
      ms40,
      ms50,
      ms60,
      ms80,
      ms100,
      ms200,
      ms300,
      ms500,
      ms750,
      ms1280,
      ms1920,
      ms2560,
      spare9,
      spare8,
      spare7,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_drx_inactivity_timer_r17_e_ = enumerated<sl_drx_inactivity_timer_r17_opts>;
  struct sl_drx_harq_rtt_timer1_r17_opts {
    enum options { sl0, sl1, sl2, sl4, spare4, spare3, spare2, spare1, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_drx_harq_rtt_timer1_r17_e_ = enumerated<sl_drx_harq_rtt_timer1_r17_opts>;
  struct sl_drx_harq_rtt_timer2_r17_opts {
    enum options { sl0, sl1, sl2, sl4, spare4, spare3, spare2, spare1, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_drx_harq_rtt_timer2_r17_e_ = enumerated<sl_drx_harq_rtt_timer2_r17_opts>;
  struct sl_drx_retx_timer_r17_opts {
    enum options {
      sl0,
      sl1,
      sl2,
      sl4,
      sl6,
      sl8,
      sl16,
      sl24,
      sl33,
      sl40,
      sl64,
      sl80,
      sl96,
      sl112,
      sl128,
      sl160,
      sl320,
      spare15,
      spare14,
      spare13,
      spare12,
      spare11,
      spare10,
      spare9,
      spare8,
      spare7,
      spare6,
      spare5,
      spare4,
      spare3,
      spare2,
      spare1,
      nulltype
    } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using sl_drx_retx_timer_r17_e_ = enumerated<sl_drx_retx_timer_r17_opts>;
  struct sl_drx_cycle_start_offset_r17_c_ {
    struct types_opts {
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
        nulltype
      } value;
      typedef uint16_t number_type;

      const char* to_string() const;
      uint16_t    to_number() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    sl_drx_cycle_start_offset_r17_c_() = default;
    sl_drx_cycle_start_offset_r17_c_(const sl_drx_cycle_start_offset_r17_c_& other);
    sl_drx_cycle_start_offset_r17_c_& operator=(const sl_drx_cycle_start_offset_r17_c_& other);
    ~sl_drx_cycle_start_offset_r17_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    uint8_t& ms10()
    {
      assert_choice_type(types::ms10, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms20()
    {
      assert_choice_type(types::ms20, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms32()
    {
      assert_choice_type(types::ms32, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms40()
    {
      assert_choice_type(types::ms40, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms60()
    {
      assert_choice_type(types::ms60, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms64()
    {
      assert_choice_type(types::ms64, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms70()
    {
      assert_choice_type(types::ms70, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms80()
    {
      assert_choice_type(types::ms80, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms128()
    {
      assert_choice_type(types::ms128, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint8_t& ms160()
    {
      assert_choice_type(types::ms160, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    uint16_t& ms256()
    {
      assert_choice_type(types::ms256, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms320()
    {
      assert_choice_type(types::ms320, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms512()
    {
      assert_choice_type(types::ms512, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms640()
    {
      assert_choice_type(types::ms640, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms1024()
    {
      assert_choice_type(types::ms1024, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms1280()
    {
      assert_choice_type(types::ms1280, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms2048()
    {
      assert_choice_type(types::ms2048, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms2560()
    {
      assert_choice_type(types::ms2560, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms5120()
    {
      assert_choice_type(types::ms5120, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint16_t& ms10240()
    {
      assert_choice_type(types::ms10240, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint8_t& ms10() const
    {
      assert_choice_type(types::ms10, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms20() const
    {
      assert_choice_type(types::ms20, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms32() const
    {
      assert_choice_type(types::ms32, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms40() const
    {
      assert_choice_type(types::ms40, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms60() const
    {
      assert_choice_type(types::ms60, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms64() const
    {
      assert_choice_type(types::ms64, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms70() const
    {
      assert_choice_type(types::ms70, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms80() const
    {
      assert_choice_type(types::ms80, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms128() const
    {
      assert_choice_type(types::ms128, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint8_t& ms160() const
    {
      assert_choice_type(types::ms160, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint8_t>();
    }
    const uint16_t& ms256() const
    {
      assert_choice_type(types::ms256, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms320() const
    {
      assert_choice_type(types::ms320, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms512() const
    {
      assert_choice_type(types::ms512, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms640() const
    {
      assert_choice_type(types::ms640, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms1024() const
    {
      assert_choice_type(types::ms1024, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms1280() const
    {
      assert_choice_type(types::ms1280, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms2048() const
    {
      assert_choice_type(types::ms2048, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms2560() const
    {
      assert_choice_type(types::ms2560, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms5120() const
    {
      assert_choice_type(types::ms5120, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    const uint16_t& ms10240() const
    {
      assert_choice_type(types::ms10240, type_, "sl-drx-CycleStartOffset-r17");
      return c.get<uint16_t>();
    }
    uint8_t&  set_ms10();
    uint8_t&  set_ms20();
    uint8_t&  set_ms32();
    uint8_t&  set_ms40();
    uint8_t&  set_ms60();
    uint8_t&  set_ms64();
    uint8_t&  set_ms70();
    uint8_t&  set_ms80();
    uint8_t&  set_ms128();
    uint8_t&  set_ms160();
    uint16_t& set_ms256();
    uint16_t& set_ms320();
    uint16_t& set_ms512();
    uint16_t& set_ms640();
    uint16_t& set_ms1024();
    uint16_t& set_ms1280();
    uint16_t& set_ms2048();
    uint16_t& set_ms2560();
    uint16_t& set_ms5120();
    uint16_t& set_ms10240();

  private:
    types               type_;
    pod_choice_buffer_t c;

    void destroy_();
  };

  // member variables
  bool                             sl_drx_harq_rtt_timer1_r17_present = false;
  bool                             sl_drx_harq_rtt_timer2_r17_present = false;
  sl_drx_on_dur_timer_r17_c_       sl_drx_on_dur_timer_r17;
  sl_drx_inactivity_timer_r17_e_   sl_drx_inactivity_timer_r17;
  sl_drx_harq_rtt_timer1_r17_e_    sl_drx_harq_rtt_timer1_r17;
  sl_drx_harq_rtt_timer2_r17_e_    sl_drx_harq_rtt_timer2_r17;
  sl_drx_retx_timer_r17_e_         sl_drx_retx_timer_r17;
  sl_drx_cycle_start_offset_r17_c_ sl_drx_cycle_start_offset_r17;
  uint8_t                          sl_drx_slot_offset = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DRX-ConfigUC-Info-r17 ::= SEQUENCE
struct sl_drx_cfg_uc_info_r17_s {
  bool                sl_dest_idx_r17_present   = false;
  bool                sl_drx_cfg_uc_r17_present = false;
  uint8_t             sl_dest_idx_r17           = 0;
  sl_drx_cfg_uc_r17_s sl_drx_cfg_uc_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DRX-Config-r17 ::= SEQUENCE
struct sl_drx_cfg_r17_s {
  using sl_drx_cfg_uc_to_release_list_r17_l_ = bounded_array<uint8_t, 32>;
  using sl_drx_cfg_uc_to_add_mod_list_r17_l_ = dyn_array<sl_drx_cfg_uc_info_r17_s>;

  // member variables
  bool                                 sl_drx_cfg_gc_bc_r17_present = false;
  sl_drx_cfg_gc_bc_r17_s               sl_drx_cfg_gc_bc_r17;
  sl_drx_cfg_uc_to_release_list_r17_l_ sl_drx_cfg_uc_to_release_list_r17;
  sl_drx_cfg_uc_to_add_mod_list_r17_l_ sl_drx_cfg_uc_to_add_mod_list_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RLC-ChannelConfig-r17 ::= SEQUENCE
struct sl_rlc_ch_cfg_r17_s {
  bool               sl_rlc_cfg_r17_present             = false;
  bool               sl_mac_lc_ch_cfg_r17_present       = false;
  bool               sl_packet_delay_budget_r17_present = false;
  uint16_t           sl_rlc_ch_id_r17                   = 1;
  sl_rlc_cfg_r16_c   sl_rlc_cfg_r17;
  sl_lc_ch_cfg_r16_s sl_mac_lc_ch_cfg_r17;
  uint16_t           sl_packet_delay_budget_r17 = 0;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-FreqSelectionConfig-r18 ::= SEQUENCE
struct sl_freq_sel_cfg_r18_s {
  using sl_prio_list_r18_l_ = bounded_array<uint8_t, 8>;

  // member variables
  sl_prio_list_r18_l_ sl_prio_list_r18;
  uint8_t             sl_thresh_cbr_freq_resel_r18   = 0;
  uint8_t             sl_thresh_cbr_freq_keeping_r18 = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-FreqConfigExt-v1800 ::= SEQUENCE
struct sl_freq_cfg_ext_v1800_s {
  using sl_freq_sel_cfg_list_r18_l_ = dyn_array<sl_freq_sel_cfg_r18_s>;
  struct sl_energy_detection_cfg_r18_c_ {
    struct types_opts {
      enum options { sl_max_energy_detection_thres_r18, sl_energy_detection_thres_offset_r18, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    sl_energy_detection_cfg_r18_c_() = default;
    sl_energy_detection_cfg_r18_c_(const sl_energy_detection_cfg_r18_c_& other);
    sl_energy_detection_cfg_r18_c_& operator=(const sl_energy_detection_cfg_r18_c_& other);
    ~sl_energy_detection_cfg_r18_c_() { destroy_(); }
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    int8_t& sl_max_energy_detection_thres_r18()
    {
      assert_choice_type(types::sl_max_energy_detection_thres_r18, type_, "sl-EnergyDetectionConfig-r18");
      return c.get<int8_t>();
    }
    int8_t& sl_energy_detection_thres_offset_r18()
    {
      assert_choice_type(types::sl_energy_detection_thres_offset_r18, type_, "sl-EnergyDetectionConfig-r18");
      return c.get<int8_t>();
    }
    const int8_t& sl_max_energy_detection_thres_r18() const
    {
      assert_choice_type(types::sl_max_energy_detection_thres_r18, type_, "sl-EnergyDetectionConfig-r18");
      return c.get<int8_t>();
    }
    const int8_t& sl_energy_detection_thres_offset_r18() const
    {
      assert_choice_type(types::sl_energy_detection_thres_offset_r18, type_, "sl-EnergyDetectionConfig-r18");
      return c.get<int8_t>();
    }
    int8_t& set_sl_max_energy_detection_thres_r18();
    int8_t& set_sl_energy_detection_thres_offset_r18();

  private:
    types               type_;
    pod_choice_buffer_t c;

    void destroy_();
  };

  // member variables
  bool                           absence_of_any_other_technology_r18_present                     = false;
  bool                           sl_sync_tx_disabled_r18_present                                 = false;
  bool                           sl_energy_detection_cfg_r18_present                             = false;
  bool                           ue_to_ue_cot_sharing_ed_thres_r18_present                       = false;
  bool                           harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18_present = false;
  sl_freq_sel_cfg_list_r18_l_    sl_freq_sel_cfg_list_r18;
  sl_energy_detection_cfg_r18_c_ sl_energy_detection_cfg_r18;
  int8_t                         ue_to_ue_cot_sharing_ed_thres_r18                       = -85;
  uint8_t                        harq_ack_feedback_ratiofor_cw_adjustment_gc_option2_r18 = 10;
  // ...
  // group 0
  bool    add_spec_emission_v1860_present = false;
  uint8_t add_spec_emission_v1860         = 8;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SCCH-CarrierSetConfig-r18 ::= SEQUENCE
struct sl_s_cch_carrier_set_cfg_r18_s {
  using sl_dest_list_r18_l_                 = bounded_array<fixed_bitstring<24>, 32>;
  using sl_srb_id_r18_l_                    = bounded_array<uint8_t, 3>;
  using sl_allowed_carrier_freq_set1_r18_l_ = bounded_array<uint8_t, 8>;
  using sl_allowed_carrier_freq_set2_r18_l_ = bounded_array<uint8_t, 8>;

  // member variables
  sl_dest_list_r18_l_                 sl_dest_list_r18;
  sl_srb_id_r18_l_                    sl_srb_id_r18;
  sl_allowed_carrier_freq_set1_r18_l_ sl_allowed_carrier_freq_set1_r18;
  sl_allowed_carrier_freq_set2_r18_l_ sl_allowed_carrier_freq_set2_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SCCH-CarrierSetConfigList-r18 ::= SEQUENCE (SIZE (1..96)) OF SL-SCCH-CarrierSetConfig-r18
using sl_s_cch_carrier_set_cfg_list_r18_l = dyn_array<sl_s_cch_carrier_set_cfg_r18_s>;

// SL-PHY-MAC-RLC-Config-v1700 ::= SEQUENCE
struct sl_phy_mac_rlc_cfg_v1700_s {
  using sl_rlc_ch_to_release_list_r17_l_                = dyn_array<uint16_t>;
  using sl_rlc_ch_to_add_mod_list_r17_l_                = dyn_array<sl_rlc_ch_cfg_r17_s>;
  using sl_rlc_bearer_to_add_mod_list_size_ext_v1800_l_ = dyn_array<sl_rlc_bearer_cfg_r16_s>;
  using sl_rlc_bearer_to_release_list_size_ext_v1800_l_ = dyn_array<uint16_t>;
  using sl_freq_info_to_add_mod_list_ext_v1800_l_       = dyn_array<sl_freq_cfg_ext_v1800_s>;
  using sl_sync_freq_list_r18_l_                        = bounded_array<uint8_t, 8>;

  // member variables
  bool                             sl_drx_cfg_r17_present = false;
  sl_drx_cfg_r17_s                 sl_drx_cfg_r17;
  sl_rlc_ch_to_release_list_r17_l_ sl_rlc_ch_to_release_list_r17;
  sl_rlc_ch_to_add_mod_list_r17_l_ sl_rlc_ch_to_add_mod_list_r17;
  // ...
  // group 0
  bool                                                      sl_sync_tx_multi_freq_r18_present = false;
  bool                                                      sl_max_trans_pwr_ca_r18_present   = false;
  copy_ptr<sl_rlc_bearer_to_add_mod_list_size_ext_v1800_l_> sl_rlc_bearer_to_add_mod_list_size_ext_v1800;
  copy_ptr<sl_rlc_bearer_to_release_list_size_ext_v1800_l_> sl_rlc_bearer_to_release_list_size_ext_v1800;
  copy_ptr<sl_freq_info_to_add_mod_list_ext_v1800_l_>       sl_freq_info_to_add_mod_list_ext_v1800;
  copy_ptr<setup_release_c<integer<uint8_t, 0, 7>>>         sl_lbt_sched_request_id_r18;
  copy_ptr<sl_sync_freq_list_r18_l_>                        sl_sync_freq_list_r18;
  int8_t                                                    sl_max_trans_pwr_ca_r18 = -30;
  copy_ptr<setup_release_c<dyn_seq_of<sl_s_cch_carrier_set_cfg_r18_s, 1, 96>>> sl_s_cch_carrier_set_cfg_r18;
  copy_ptr<setup_release_c<integer<uint8_t, 0, 7>>>                            sl_prs_sched_request_id_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RelayUE-Config-r17 ::= SEQUENCE
struct sl_relay_ue_cfg_r17_s {
  bool    thresh_high_relay_r17_present = false;
  bool    thresh_low_relay_r17_present  = false;
  bool    hyst_max_relay_r17_present    = false;
  bool    hyst_min_relay_r17_present    = false;
  uint8_t thresh_high_relay_r17         = 0;
  uint8_t thresh_low_relay_r17          = 0;
  uint8_t hyst_max_relay_r17            = 0;
  uint8_t hyst_min_relay_r17            = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ReselectionConfig-r17 ::= SEQUENCE
struct sl_resel_cfg_r17_s {
  bool        sl_rsrp_thresh_r17_present    = false;
  bool        sl_filt_coef_rsrp_r17_present = false;
  bool        sl_hyst_min_r17_present       = false;
  uint8_t     sl_rsrp_thresh_r17            = 0;
  filt_coef_e sl_filt_coef_rsrp_r17;
  uint8_t     sl_hyst_min_r17 = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RemoteUE-Config-r17 ::= SEQUENCE
struct sl_remote_ue_cfg_r17_s {
  bool               thresh_high_remote_r17_present = false;
  bool               hyst_max_remote_r17_present    = false;
  bool               sl_resel_cfg_r17_present       = false;
  uint8_t            thresh_high_remote_r17         = 0;
  uint8_t            hyst_max_remote_r17            = 0;
  sl_resel_cfg_r17_s sl_resel_cfg_r17;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DiscConfig-r17 ::= SEQUENCE
struct sl_disc_cfg_r17_s {
  bool                                    sl_relay_ue_cfg_r17_present  = false;
  bool                                    sl_remote_ue_cfg_r17_present = false;
  setup_release_c<sl_relay_ue_cfg_r17_s>  sl_relay_ue_cfg_r17;
  setup_release_c<sl_remote_ue_cfg_r17_s> sl_remote_ue_cfg_r17;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RelayUE-ConfigU2U-r18 ::= SEQUENCE
struct sl_relay_ue_cfg_u2_u_r18_s {
  bool    sl_rsrp_thresh_disc_cfg_r18_present = false;
  bool    sd_rsrp_thresh_disc_cfg_r18_present = false;
  bool    sd_hyst_max_relay_r18_present       = false;
  uint8_t sl_rsrp_thresh_disc_cfg_r18         = 0;
  uint8_t sd_rsrp_thresh_disc_cfg_r18         = 0;
  uint8_t sd_hyst_max_relay_r18               = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RemoteUE-ConfigU2U-r18 ::= SEQUENCE
struct sl_remote_ue_cfg_u2_u_r18_s {
  bool        sl_rsrp_thresh_u2_u_r18_present = false;
  bool        sl_hyst_min_u2_u_r18_present    = false;
  bool        sd_rsrp_thresh_u2_u_r18_present = false;
  bool        sd_filt_coef_u2_u_r18_present   = false;
  bool        sd_hyst_min_u2_u_r18_present    = false;
  uint8_t     sl_rsrp_thresh_u2_u_r18         = 0;
  uint8_t     sl_hyst_min_u2_u_r18            = 0;
  uint8_t     sd_rsrp_thresh_u2_u_r18         = 0;
  filt_coef_e sd_filt_coef_u2_u_r18;
  uint8_t     sd_hyst_min_u2_u_r18 = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DiscConfig-v1800 ::= SEQUENCE
struct sl_disc_cfg_v1800_s {
  bool                                         sl_relay_ue_cfg_u2_u_r18_present  = false;
  bool                                         sl_remote_ue_cfg_u2_u_r18_present = false;
  setup_release_c<sl_relay_ue_cfg_u2_u_r18_s>  sl_relay_ue_cfg_u2_u_r18;
  setup_release_c<sl_remote_ue_cfg_u2_u_r18_s> sl_remote_ue_cfg_u2_u_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RemoteUE-ConfigU2U-v1830 ::= SEQUENCE
struct sl_remote_ue_cfg_u2_u_v1830_s {
  bool        sl_filt_coef_u2_u_r18_present = false;
  filt_coef_e sl_filt_coef_u2_u_r18;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DiscConfig-v1830 ::= SEQUENCE
struct sl_disc_cfg_v1830_s {
  bool                                           sl_remote_ue_cfg_u2_u_v1830_present = false;
  setup_release_c<sl_remote_ue_cfg_u2_u_v1830_s> sl_remote_ue_cfg_u2_u_v1830;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RelayUE-ConfigU2U-v1840 ::= SEQUENCE
struct sl_relay_ue_cfg_u2_u_v1840_s {
  bool        sl_filt_coef_u2_u_r18_present = false;
  bool        sd_filt_coef_u2_u_r18_present = false;
  filt_coef_e sl_filt_coef_u2_u_r18;
  filt_coef_e sd_filt_coef_u2_u_r18;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-DiscConfig-v1840 ::= SEQUENCE
struct sl_disc_cfg_v1840_s {
  bool                                          sl_relay_ue_cfg_u2_u_v1840_present = false;
  setup_release_c<sl_relay_ue_cfg_u2_u_v1840_s> sl_relay_ue_cfg_u2_u_v1840;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-ConfigDedicatedNR-r16 ::= SEQUENCE
struct sl_cfg_ded_nr_r16_s {
  using sl_radio_bearer_to_release_list_r16_l_  = dyn_array<uint16_t>;
  using sl_radio_bearer_to_add_mod_list_r16_l_  = dyn_array<sl_radio_bearer_cfg_r16_s>;
  using sl_meas_cfg_info_to_release_list_r16_l_ = bounded_array<uint8_t, 32>;
  using sl_meas_cfg_info_to_add_mod_list_r16_l_ = dyn_array<sl_meas_cfg_info_r16_s>;
  struct t400_r16_opts {
    enum options { ms100, ms200, ms300, ms400, ms600, ms1000, ms1500, ms2000, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using t400_r16_e_ = enumerated<t400_r16_opts>;

  // member variables
  bool                                    sl_phy_mac_rlc_cfg_r16_present = false;
  bool                                    t400_r16_present               = false;
  sl_phy_mac_rlc_cfg_r16_s                sl_phy_mac_rlc_cfg_r16;
  sl_radio_bearer_to_release_list_r16_l_  sl_radio_bearer_to_release_list_r16;
  sl_radio_bearer_to_add_mod_list_r16_l_  sl_radio_bearer_to_add_mod_list_r16;
  sl_meas_cfg_info_to_release_list_r16_l_ sl_meas_cfg_info_to_release_list_r16;
  sl_meas_cfg_info_to_add_mod_list_r16_l_ sl_meas_cfg_info_to_add_mod_list_r16;
  t400_r16_e_                             t400_r16;
  // ...
  // group 0
  copy_ptr<setup_release_c<sl_phy_mac_rlc_cfg_v1700_s>> sl_phy_mac_rlc_cfg_v1700;
  copy_ptr<setup_release_c<sl_disc_cfg_r17_s>>          sl_disc_cfg_r17;
  // group 1
  copy_ptr<sl_disc_cfg_v1800_s> sl_disc_cfg_v1800;
  // group 2
  copy_ptr<sl_disc_cfg_v1830_s> sl_disc_cfg_v1830;
  // group 3
  copy_ptr<sl_disc_cfg_v1840_s> sl_disc_cfg_v1840;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RemoteUE-RB-Identity-r17 ::= CHOICE
struct sl_remote_ue_rb_id_r17_c {
  struct types_opts {
    enum options { srb_id_r17, drb_id_r17, /*...*/ nulltype } value;

    const char* to_string() const;
  };
  using types = enumerated<types_opts, true>;

  // choice methods
  sl_remote_ue_rb_id_r17_c() = default;
  sl_remote_ue_rb_id_r17_c(const sl_remote_ue_rb_id_r17_c& other);
  sl_remote_ue_rb_id_r17_c& operator=(const sl_remote_ue_rb_id_r17_c& other);
  ~sl_remote_ue_rb_id_r17_c() { destroy_(); }
  void          set(types::options e = types::nulltype);
  types         type() const { return type_; }
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
  // getters
  uint8_t& srb_id_r17()
  {
    assert_choice_type(types::srb_id_r17, type_, "SL-RemoteUE-RB-Identity-r17");
    return c.get<uint8_t>();
  }
  uint8_t& drb_id_r17()
  {
    assert_choice_type(types::drb_id_r17, type_, "SL-RemoteUE-RB-Identity-r17");
    return c.get<uint8_t>();
  }
  const uint8_t& srb_id_r17() const
  {
    assert_choice_type(types::srb_id_r17, type_, "SL-RemoteUE-RB-Identity-r17");
    return c.get<uint8_t>();
  }
  const uint8_t& drb_id_r17() const
  {
    assert_choice_type(types::drb_id_r17, type_, "SL-RemoteUE-RB-Identity-r17");
    return c.get<uint8_t>();
  }
  uint8_t& set_srb_id_r17();
  uint8_t& set_drb_id_r17();

private:
  types               type_;
  pod_choice_buffer_t c;

  void destroy_();
};

// SL-MappingToAddMod-r17 ::= SEQUENCE
struct sl_map_to_add_mod_r17_s {
  bool                     sl_egress_rlc_ch_uu_r17_present  = false;
  bool                     sl_egress_rlc_ch_pc5_r17_present = false;
  sl_remote_ue_rb_id_r17_c sl_remote_ue_rb_id_r17;
  uint8_t                  sl_egress_rlc_ch_uu_r17  = 1;
  uint16_t                 sl_egress_rlc_ch_pc5_r17 = 1;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SRAP-Config-r17 ::= SEQUENCE
struct sl_srap_cfg_r17_s {
  using sl_map_to_add_mod_list_r17_l_ = dyn_array<sl_map_to_add_mod_r17_s>;
  using sl_map_to_release_list_r17_l_ = dyn_array<sl_remote_ue_rb_id_r17_c>;

  // member variables
  bool                          sl_local_id_r17_present = false;
  uint16_t                      sl_local_id_r17         = 0;
  sl_map_to_add_mod_list_r17_l_ sl_map_to_add_mod_list_r17;
  sl_map_to_release_list_r17_l_ sl_map_to_release_list_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-MappingConfig-U2U-r18 ::= SEQUENCE
struct sl_map_cfg_u2_u_r18_s {
  uint16_t sl_remote_ue_slrb_id_r18 = 1;
  uint16_t sl_egress_rlc_ch_pc5_r18 = 1;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-SRAP-ConfigU2U-r18 ::= SEQUENCE
struct sl_srap_cfg_u2_u_r18_s {
  using sl_map_to_add_mod_u2_u_list_r18_l_ = dyn_array<sl_map_cfg_u2_u_r18_s>;
  using sl_map_to_release_u2_u_list_r18_l_ = dyn_array<uint16_t>;

  // member variables
  sl_map_to_add_mod_u2_u_list_r18_l_ sl_map_to_add_mod_u2_u_list_r18;
  sl_map_to_release_u2_u_list_r18_l_ sl_map_to_release_u2_u_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-TargetRemoteUE-Config-r18 ::= SEQUENCE
struct sl_target_remote_ue_cfg_r18_s {
  fixed_bitstring<24>    sl_target_ue_id_r18;
  sl_srap_cfg_u2_u_r18_s sl_srap_cfg_u2_u_r18;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-U2U-RelayUE-Config-r18 ::= SEQUENCE
struct sl_u2_u_relay_ue_cfg_r18_s {
  using sl_target_remote_ue_to_add_mod_list_r18_l_ = dyn_array<sl_target_remote_ue_cfg_r18_s>;
  using sl_target_remote_ue_to_release_list_r18_l_ = bounded_array<fixed_bitstring<24>, 32>;

  // member variables
  fixed_bitstring<24>                        sl_l2_id_relay_r18;
  sl_target_remote_ue_to_add_mod_list_r18_l_ sl_target_remote_ue_to_add_mod_list_r18;
  sl_target_remote_ue_to_release_list_r18_l_ sl_target_remote_ue_to_release_list_r18;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-L2RemoteUE-Config-r17 ::= SEQUENCE
struct sl_l2_remote_ue_cfg_r17_s {
  using sl_u2_u_relay_ue_to_add_mod_list_r18_l_ = dyn_array<sl_u2_u_relay_ue_cfg_r18_s>;
  using sl_u2_u_relay_ue_to_release_list_r18_l_ = bounded_array<fixed_bitstring<24>, 32>;

  // member variables
  bool              sl_srap_cfg_remote_r17_present = false;
  bool              sl_ue_id_remote_r17_present    = false;
  sl_srap_cfg_r17_s sl_srap_cfg_remote_r17;
  uint32_t          sl_ue_id_remote_r17 = 0;
  // ...
  // group 0
  copy_ptr<sl_u2_u_relay_ue_to_add_mod_list_r18_l_> sl_u2_u_relay_ue_to_add_mod_list_r18;
  copy_ptr<sl_u2_u_relay_ue_to_release_list_r18_l_> sl_u2_u_relay_ue_to_release_list_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCSetup-v1700-IEs ::= SEQUENCE
struct rrc_setup_v1700_ies_s {
  bool                      sl_cfg_ded_nr_r17_present       = false;
  bool                      sl_l2_remote_ue_cfg_r17_present = false;
  bool                      non_crit_ext_present            = false;
  sl_cfg_ded_nr_r16_s       sl_cfg_ded_nr_r17;
  sl_l2_remote_ue_cfg_r17_s sl_l2_remote_ue_cfg_r17;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCSetup-IEs ::= SEQUENCE
struct rrc_setup_ies_s {
  bool                  non_crit_ext_present = false;
  radio_bearer_cfg_s    radio_bearer_cfg;
  dyn_octstring         master_cell_group;
  dyn_octstring         late_non_crit_ext;
  rrc_setup_v1700_ies_s non_crit_ext;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// RRCSetup ::= SEQUENCE
struct rrc_setup_s {
  struct crit_exts_c_ {
    struct types_opts {
      enum options { rrc_setup, crit_exts_future, nulltype } value;

      const char* to_string() const;
    };
    using types = enumerated<types_opts>;

    // choice methods
    crit_exts_c_() = default;
    void          set(types::options e = types::nulltype);
    types         type() const { return type_; }
    OCUDUASN_CODE pack(bit_ref& bref) const;
    OCUDUASN_CODE unpack(cbit_ref& bref);
    void          to_json(json_writer& j) const;
    // getters
    rrc_setup_ies_s& rrc_setup()
    {
      assert_choice_type(types::rrc_setup, type_, "criticalExtensions");
      return c;
    }
    const rrc_setup_ies_s& rrc_setup() const
    {
      assert_choice_type(types::rrc_setup, type_, "criticalExtensions");
      return c;
    }
    rrc_setup_ies_s& set_rrc_setup();
    void             set_crit_exts_future();

  private:
    types           type_;
    rrc_setup_ies_s c;
  };

  // member variables
  uint8_t      rrc_transaction_id = 0;
  crit_exts_c_ crit_exts;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

} // namespace rrc_nr
} // namespace asn1
