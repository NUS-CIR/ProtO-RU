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

#include "ocudu/asn1/rrc_nr/dl_ccch_msg_ies.h"

namespace asn1 {
namespace rrc_nr {

/*******************************************************************************
 *                              Struct Definitions
 ******************************************************************************/

// SL-BWP-PoolConfigCommon-r16 ::= SEQUENCE
struct sl_bwp_pool_cfg_common_r16_s {
  using sl_rx_pool_r16_l_            = dyn_array<sl_res_pool_r16_s>;
  using sl_tx_pool_sel_normal_r16_l_ = dyn_array<sl_res_pool_cfg_r16_s>;

  // member variables
  bool                         sl_tx_pool_exceptional_r16_present = false;
  sl_rx_pool_r16_l_            sl_rx_pool_r16;
  sl_tx_pool_sel_normal_r16_l_ sl_tx_pool_sel_normal_r16;
  sl_res_pool_cfg_r16_s        sl_tx_pool_exceptional_r16;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-BWP-DiscPoolConfigCommon-r17 ::= SEQUENCE
struct sl_bwp_disc_pool_cfg_common_r17_s {
  using sl_disc_rx_pool_r17_l_     = dyn_array<sl_res_pool_r16_s>;
  using sl_disc_tx_pool_sel_r17_l_ = dyn_array<sl_res_pool_cfg_r16_s>;

  // member variables
  sl_disc_rx_pool_r17_l_     sl_disc_rx_pool_r17;
  sl_disc_tx_pool_sel_r17_l_ sl_disc_tx_pool_sel_r17;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-BWP-ConfigCommon-r16 ::= SEQUENCE
struct sl_bwp_cfg_common_r16_s {
  bool                         sl_bwp_generic_r16_present         = false;
  bool                         sl_bwp_pool_cfg_common_r16_present = false;
  sl_bwp_generic_r16_s         sl_bwp_generic_r16;
  sl_bwp_pool_cfg_common_r16_s sl_bwp_pool_cfg_common_r16;
  // ...
  // group 0
  copy_ptr<sl_bwp_pool_cfg_common_r16_s>      sl_bwp_pool_cfg_common_ps_r17;
  copy_ptr<sl_bwp_disc_pool_cfg_common_r17_s> sl_bwp_disc_pool_cfg_common_r17;
  // group 1
  copy_ptr<sl_bwp_pool_cfg_common_r16_s> sl_bwp_pool_cfg_common_a2_x_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-BWP-PRS-PoolConfigCommon-r18 ::= SEQUENCE
struct sl_bwp_prs_pool_cfg_common_r18_s {
  using sl_prs_rx_pool_r18_l_            = dyn_array<sl_prs_res_pool_r18_s>;
  using sl_prs_tx_pool_sel_normal_r18_l_ = dyn_array<sl_prs_res_pool_cfg_r18_s>;

  // member variables
  bool                             sl_prs_tx_pool_exceptional_r18_present = false;
  sl_prs_rx_pool_r18_l_            sl_prs_rx_pool_r18;
  sl_prs_tx_pool_sel_normal_r18_l_ sl_prs_tx_pool_sel_normal_r18;
  sl_prs_res_pool_cfg_r18_s        sl_prs_tx_pool_exceptional_r18;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PosBWP-ConfigCommon-r18 ::= SEQUENCE
struct sl_pos_bwp_cfg_common_r18_s {
  bool                             sl_bwp_generic_r18_present             = false;
  bool                             sl_bwp_prs_pool_cfg_common_r18_present = false;
  sl_bwp_generic_r16_s             sl_bwp_generic_r18;
  sl_bwp_prs_pool_cfg_common_r18_s sl_bwp_prs_pool_cfg_common_r18;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-FreqConfigCommon-r16 ::= SEQUENCE
struct sl_freq_cfg_common_r16_s {
  using sl_scs_specific_carrier_list_r16_l_ = dyn_array<scs_specific_carrier_s>;
  using sl_bwp_list_r16_l_                  = dyn_array<sl_bwp_cfg_common_r16_s>;
  struct sl_sync_prio_r16_opts {
    enum options { gnss, gnb_enb, nulltype } value;

    const char* to_string() const;
  };
  using sl_sync_prio_r16_e_ = enumerated<sl_sync_prio_r16_opts>;
  struct sl_unlicensed_freq_cfg_common_r18_s_ {
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
  };
  using sl_pos_bwp_list_r18_l_ = dyn_array<sl_pos_bwp_cfg_common_r18_s>;

  // member variables
  bool                                sl_absolute_freq_ssb_r16_present = false;
  bool                                freq_shift7p5khz_sl_r16_present  = false;
  bool                                sl_sync_prio_r16_present         = false;
  bool                                sl_nb_as_sync_r16_present        = false;
  sl_scs_specific_carrier_list_r16_l_ sl_scs_specific_carrier_list_r16;
  uint32_t                            sl_absolute_freq_point_a_r16 = 0;
  uint32_t                            sl_absolute_freq_ssb_r16     = 0;
  int8_t                              value_n_r16                  = -1;
  sl_bwp_list_r16_l_                  sl_bwp_list_r16;
  sl_sync_prio_r16_e_                 sl_sync_prio_r16;
  bool                                sl_nb_as_sync_r16 = false;
  sl_sync_cfg_list_r16_l              sl_sync_cfg_list_r16;
  // ...
  // group 0
  copy_ptr<sl_unlicensed_freq_cfg_common_r18_s_> sl_unlicensed_freq_cfg_common_r18;
  copy_ptr<sl_pos_bwp_list_r18_l_>               sl_pos_bwp_list_r18;
  // group 1
  bool    add_spec_emission_v1860_present = false;
  uint8_t add_spec_emission_v1860         = 8;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-NR-AnchorCarrierFreqList-r16 ::= SEQUENCE (SIZE (1..8)) OF INTEGER (0..3279165)
using sl_nr_anchor_carrier_freq_list_r16_l = bounded_array<uint32_t, 8>;

// SL-EUTRA-AnchorCarrierFreqList-r16 ::= SEQUENCE (SIZE (1..8)) OF INTEGER (0..262143)
using sl_eutra_anchor_carrier_freq_list_r16_l = bounded_array<uint32_t, 8>;

// SL-MeasConfigCommon-r16 ::= SEQUENCE
struct sl_meas_cfg_common_r16_s {
  bool                     sl_quant_cfg_common_r16_present = false;
  sl_meas_obj_list_r16_l   sl_meas_obj_list_common_r16;
  sl_report_cfg_list_r16_l sl_report_cfg_list_common_r16;
  sl_meas_id_list_r16_l    sl_meas_id_list_common_r16;
  sl_quant_cfg_r16_s       sl_quant_cfg_common_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-FreqConfigCommonExt-v16k0 ::= SEQUENCE
struct sl_freq_cfg_common_ext_v16k0_s {
  bool    add_spec_emission_r16_present = false;
  uint8_t add_spec_emission_r16         = 0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PreconfigDiscConfig-v1800 ::= SEQUENCE
struct sl_precfg_disc_cfg_v1800_s {
  sl_relay_ue_cfg_u2_u_r18_s  sl_relay_ue_precfg_u2_u_r18;
  sl_remote_ue_cfg_u2_u_r18_s sl_remote_ue_precfg_u2_u_r18;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PreconfigDiscConfig-v1840 ::= SEQUENCE
struct sl_precfg_disc_cfg_v1840_s {
  sl_relay_ue_cfg_u2_u_v1840_s  sl_relay_ue_precfg_u2_u_v1840;
  sl_remote_ue_cfg_u2_u_v1830_s sl_remote_ue_precfg_u2_u_v1840;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PreconfigGeneral-r16 ::= SEQUENCE
struct sl_precfg_general_r16_s {
  bool                   sl_tdd_cfg_r16_present    = false;
  bool                   reserved_bits_r16_present = false;
  tdd_ul_dl_cfg_common_s sl_tdd_cfg_r16;
  fixed_bitstring<2>     reserved_bits_r16;
  // ...

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-RoHC-Profiles-r16 ::= SEQUENCE
struct sl_ro_hc_profiles_r16_s {
  bool profile0x0001_r16 = false;
  bool profile0x0002_r16 = false;
  bool profile0x0003_r16 = false;
  bool profile0x0004_r16 = false;
  bool profile0x0006_r16 = false;
  bool profile0x0101_r16 = false;
  bool profile0x0102_r16 = false;
  bool profile0x0103_r16 = false;
  bool profile0x0104_r16 = false;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-TxProfile-r17 ::= ENUMERATED
struct sl_tx_profile_r17_opts {
  enum options { drx_compatible, drx_incompatible, spare6, spare5, spare4, spare3, spare2, spare1, nulltype } value;

  const char* to_string() const;
};
using sl_tx_profile_r17_e = enumerated<sl_tx_profile_r17_opts>;

// SL-TxProfileList-r17 ::= SEQUENCE (SIZE (1..256)) OF SL-TxProfile-r17
using sl_tx_profile_list_r17_l = dyn_array<sl_tx_profile_r17_e>;

// SidelinkPreconfigNR-r16 ::= SEQUENCE
struct sidelink_precfg_nr_r16_s {
  using sl_precfg_freq_info_list_r16_l_     = dyn_array<sl_freq_cfg_common_r16_s>;
  using sl_radio_bearer_pre_cfg_list_r16_l_ = dyn_array<sl_radio_bearer_cfg_r16_s>;
  using sl_rlc_bearer_pre_cfg_list_r16_l_   = dyn_array<sl_rlc_bearer_cfg_r16_s>;
  struct t400_r16_opts {
    enum options { ms100, ms200, ms300, ms400, ms600, ms1000, ms1500, ms2000, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using t400_r16_e_ = enumerated<t400_r16_opts>;
  struct sl_max_num_consecutive_dtx_r16_opts {
    enum options { n1, n2, n3, n4, n6, n8, n16, n32, nulltype } value;
    typedef uint8_t number_type;

    const char* to_string() const;
    uint8_t     to_number() const;
  };
  using sl_max_num_consecutive_dtx_r16_e_          = enumerated<sl_max_num_consecutive_dtx_r16_opts>;
  using sl_precfg_freq_info_list_size_ext_v1800_l_ = dyn_array<sl_freq_cfg_common_r16_s>;
  using sl_rlc_bearer_cfg_list_size_ext_v1800_l_   = dyn_array<sl_rlc_bearer_cfg_r16_s>;
  using sl_sync_freq_list_r18_l_                   = bounded_array<uint8_t, 8>;
  using sl_pos_precfg_freq_info_list_r18_l_        = dyn_array<sl_freq_cfg_common_r16_s>;
  struct t400_u2_u_r18_opts {
    enum options { ms200, ms400, ms600, ms800, ms1200, ms2000, ms3000, ms4000, nulltype } value;
    typedef uint16_t number_type;

    const char* to_string() const;
    uint16_t    to_number() const;
  };
  using t400_u2_u_r18_e_                          = enumerated<t400_u2_u_r18_opts>;
  using sl_pos_precfg_freq_info_list_ext_v1880_l_ = dyn_array<sl_freq_cfg_common_ext_v16k0_s>;

  // member variables
  bool                                    sl_meas_pre_cfg_r16_present            = false;
  bool                                    sl_offset_dfn_r16_present              = false;
  bool                                    t400_r16_present                       = false;
  bool                                    sl_max_num_consecutive_dtx_r16_present = false;
  bool                                    sl_ssb_prio_nr_r16_present             = false;
  bool                                    sl_precfg_general_r16_present          = false;
  bool                                    sl_ue_sel_pre_cfg_r16_present          = false;
  bool                                    sl_csi_acquisition_r16_present         = false;
  bool                                    sl_ro_hc_profiles_r16_present          = false;
  bool                                    sl_max_c_id_r16_present                = false;
  sl_precfg_freq_info_list_r16_l_         sl_precfg_freq_info_list_r16;
  sl_nr_anchor_carrier_freq_list_r16_l    sl_precfg_nr_anchor_carrier_freq_list_r16;
  sl_eutra_anchor_carrier_freq_list_r16_l sl_precfg_eutra_anchor_carrier_freq_list_r16;
  sl_radio_bearer_pre_cfg_list_r16_l_     sl_radio_bearer_pre_cfg_list_r16;
  sl_rlc_bearer_pre_cfg_list_r16_l_       sl_rlc_bearer_pre_cfg_list_r16;
  sl_meas_cfg_common_r16_s                sl_meas_pre_cfg_r16;
  uint16_t                                sl_offset_dfn_r16 = 1;
  t400_r16_e_                             t400_r16;
  sl_max_num_consecutive_dtx_r16_e_       sl_max_num_consecutive_dtx_r16;
  uint8_t                                 sl_ssb_prio_nr_r16 = 1;
  sl_precfg_general_r16_s                 sl_precfg_general_r16;
  sl_ue_sel_cfg_r16_s                     sl_ue_sel_pre_cfg_r16;
  sl_ro_hc_profiles_r16_s                 sl_ro_hc_profiles_r16;
  uint16_t                                sl_max_c_id_r16 = 1;
  // ...
  // group 0
  copy_ptr<sl_drx_cfg_gc_bc_r17_s>   sl_drx_pre_cfg_gc_bc_r17;
  copy_ptr<sl_tx_profile_list_r17_l> sl_tx_profile_list_r17;
  copy_ptr<sl_remote_ue_cfg_r17_s>   sl_precfg_disc_cfg_r17;
  // group 1
  bool                                                 sl_sync_tx_multi_freq_r18_present = false;
  copy_ptr<sl_precfg_freq_info_list_size_ext_v1800_l_> sl_precfg_freq_info_list_size_ext_v1800;
  copy_ptr<sl_rlc_bearer_cfg_list_size_ext_v1800_l_>   sl_rlc_bearer_cfg_list_size_ext_v1800;
  copy_ptr<sl_sync_freq_list_r18_l_>                   sl_sync_freq_list_r18;
  copy_ptr<sl_precfg_disc_cfg_v1800_s>                 sl_precfg_disc_cfg_v1800;
  copy_ptr<sl_pos_precfg_freq_info_list_r18_l_>        sl_pos_precfg_freq_info_list_r18;
  // group 2
  bool             t400_u2_u_r18_present = false;
  t400_u2_u_r18_e_ t400_u2_u_r18;
  // group 3
  copy_ptr<sl_precfg_disc_cfg_v1840_s> sl_precfg_disc_cfg_v1840;
  // group 4
  copy_ptr<sl_pos_precfg_freq_info_list_ext_v1880_l_> sl_pos_precfg_freq_info_list_ext_v1880;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SidelinkPreconfigNR-v16k0 ::= SEQUENCE
struct sidelink_precfg_nr_v16k0_s {
  using sl_precfg_freq_info_list_ext_v16k0_l_ = dyn_array<sl_freq_cfg_common_ext_v16k0_s>;

  // member variables
  sl_precfg_freq_info_list_ext_v16k0_l_ sl_precfg_freq_info_list_ext_v16k0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

// SL-PreconfigurationNR-r16 ::= SEQUENCE
struct sl_precfg_nr_r16_s {
  sidelink_precfg_nr_r16_s sidelink_precfg_nr_r16;
  // ...
  // group 0
  copy_ptr<sidelink_precfg_nr_v16k0_s> sidelink_precfg_nr_v16k0;

  // sequence methods
  OCUDUASN_CODE pack(bit_ref& bref) const;
  OCUDUASN_CODE unpack(cbit_ref& bref);
  void          to_json(json_writer& j) const;
};

} // namespace rrc_nr
} // namespace asn1

extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_freq_cfg_common_r16_s::sl_bwp_list_r16_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_freq_cfg_common_r16_s::sl_pos_bwp_list_r18_l_::value_type>::ops;
extern template const asn1::detail::dyn_array_ops
    asn1::dyn_array<asn1::rrc_nr::sl_tx_profile_list_r17_l::value_type>::ops;
