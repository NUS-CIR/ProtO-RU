// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "rrc_asn1_converters.h"

using namespace ocudu;
using namespace ocucp;

asn1::rrc_nr::pdcp_cfg_s::drb_s_::discard_timer_e_ ocudu::ocucp::discard_timer_to_asn1(pdcp_discard_timer discard_timer)
{
  asn1::rrc_nr::pdcp_cfg_s::drb_s_::discard_timer_e_ asn1_discard_timer;

  asn1::number_to_enum(asn1_discard_timer, to_underlying(discard_timer));

  return asn1_discard_timer;
}

asn1::rrc_nr::pdcp_cfg_s::t_reordering_e_ ocudu::ocucp::t_reordering_to_asn1(pdcp_t_reordering t_reordering)
{
  asn1::rrc_nr::pdcp_cfg_s::t_reordering_e_ asn1_t_reordering;

  asn1::number_to_enum(asn1_t_reordering, to_underlying(t_reordering));

  return asn1_t_reordering;
}

asn1::rrc_nr::pdcp_cfg_s ocudu::ocucp::pdcp_config_to_rrc_nr_asn1(const pdcp_config& pdcp_cfg)
{
  asn1::rrc_nr::pdcp_cfg_s rrc_pdcp_cfg;

  // Fill t reordering -- Need S.
  if (pdcp_cfg.rx.t_reordering != pdcp_t_reordering::infinity) {
    rrc_pdcp_cfg.t_reordering_present = true;
    rrc_pdcp_cfg.t_reordering         = t_reordering_to_asn1(pdcp_cfg.rx.t_reordering);
  }

  // Fill ciphering disabled present -- Cond ConnectedTo5GC.
  if (!pdcp_cfg.ciphering_required) {
    rrc_pdcp_cfg.ciphering_disabled_present = true;
    // This is an extension field.
  }

  // Fill more than one RLC.
  rrc_pdcp_cfg.more_than_one_rlc_present = false; // not supported.

  // Fill no more configurable parameters for SRBs.
  if (pdcp_cfg.rb_type == pdcp_rb_type::srb) {
    return rrc_pdcp_cfg;
  }

  // Fill DRB -- Cond DRB.
  rrc_pdcp_cfg.drb_present = true;

  // Fill header compression
  if (pdcp_cfg.header_compression.has_value()) {
    const auto& rohc_cfg_in = *pdcp_cfg.header_compression;
    switch (rohc_cfg_in.rohc_type) {
      case rohc_type_t::rohc: {
        rrc_pdcp_cfg.drb.hdr_compress.set_rohc();
        auto& rrc_rohc = rrc_pdcp_cfg.drb.hdr_compress.rohc();
        // Only pack non-default max_cid (see TS 38.331 Sec. A.3.5 and Sec. 6.3.2)
        if (rrc_rohc.max_c_id != default_rohc_max_cid) {
          rrc_rohc.max_c_id_present = true;
          rrc_rohc.max_c_id         = rohc_cfg_in.max_cid;
        }
        rrc_rohc.profiles.profile0x0001 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0001);
        rrc_rohc.profiles.profile0x0002 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0002);
        rrc_rohc.profiles.profile0x0003 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0003);
        rrc_rohc.profiles.profile0x0004 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0004);
        rrc_rohc.profiles.profile0x0006 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0006);
        rrc_rohc.profiles.profile0x0101 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0101);
        rrc_rohc.profiles.profile0x0102 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0102);
        rrc_rohc.profiles.profile0x0103 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0103);
        rrc_rohc.profiles.profile0x0104 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0104);
        if (rohc_cfg_in.continue_rohc) {
          rrc_rohc.drb_continue_rohc_present = true;
        }
      } break;
      case rohc_type_t::uplink_only_rohc: {
        rrc_pdcp_cfg.drb.hdr_compress.set_ul_only_rohc();
        auto& rrc_rohc = rrc_pdcp_cfg.drb.hdr_compress.ul_only_rohc();
        // Only pack non-default max_cid (see TS 38.331 Sec. A.3.5 and Sec. 6.3.2)
        if (rrc_rohc.max_c_id != default_rohc_max_cid) {
          rrc_rohc.max_c_id_present = true;
          rrc_rohc.max_c_id         = rohc_cfg_in.max_cid;
        }
        rrc_rohc.max_c_id_present       = true;
        rrc_rohc.max_c_id               = rohc_cfg_in.max_cid;
        rrc_rohc.profiles.profile0x0006 = rohc_cfg_in.profiles.is_profile_enabled(rohc_profile::profile0x0006);
        if (rohc_cfg_in.continue_rohc) {
          rrc_rohc.drb_continue_rohc_present = true;
        }
      } break;
    }
  } else {
    rrc_pdcp_cfg.drb.hdr_compress.set_not_used();
  }

  // Fill discard timer -- Cond Setup.
  if (pdcp_cfg.tx.discard_timer.has_value()) {
    rrc_pdcp_cfg.drb.discard_timer_present = true;
    rrc_pdcp_cfg.drb.discard_timer         = discard_timer_to_asn1(pdcp_cfg.tx.discard_timer.value());
  }

  // Fill PDCP SN size UL -- Cond Setup2.
  rrc_pdcp_cfg.drb.pdcp_sn_size_ul_present = true;
  asn1::number_to_enum(rrc_pdcp_cfg.drb.pdcp_sn_size_ul, pdcp_sn_size_to_uint(pdcp_cfg.rx.sn_size));

  // Fill PDCP SN size DL -- Cond Setup2.
  rrc_pdcp_cfg.drb.pdcp_sn_size_dl_present = true;
  asn1::number_to_enum(rrc_pdcp_cfg.drb.pdcp_sn_size_dl, pdcp_sn_size_to_uint(pdcp_cfg.tx.sn_size));

  // Fill integrity protection present.
  rrc_pdcp_cfg.drb.integrity_protection_present = pdcp_cfg.integrity_protection_required;

  // Fill status report required present.
  rrc_pdcp_cfg.drb.status_report_required_present = pdcp_cfg.tx.status_report_required;

  // Fill out of order delivery present.
  rrc_pdcp_cfg.drb.out_of_order_delivery_present = pdcp_cfg.rx.out_of_order_delivery;

  return rrc_pdcp_cfg;
}

asn1::rrc_nr::sdap_cfg_s::sdap_hdr_ul_opts::options ocudu::ocucp::sdap_hdr_ul_cfg_to_rrc_asn1(sdap_hdr_ul_cfg hdr_cfg)
{
  asn1::rrc_nr::sdap_cfg_s::sdap_hdr_ul_opts::options asn1_hdr_ul_opts;

  if (hdr_cfg == sdap_hdr_ul_cfg::absent) {
    asn1_hdr_ul_opts = asn1::rrc_nr::sdap_cfg_s::sdap_hdr_ul_opts::options::absent;
  } else {
    asn1_hdr_ul_opts = asn1::rrc_nr::sdap_cfg_s::sdap_hdr_ul_opts::options::present;
  }

  return asn1_hdr_ul_opts;
}

asn1::rrc_nr::sdap_cfg_s::sdap_hdr_dl_opts::options ocudu::ocucp::sdap_hdr_dl_cfg_to_rrc_asn1(sdap_hdr_dl_cfg hdr_cfg)
{
  asn1::rrc_nr::sdap_cfg_s::sdap_hdr_dl_opts::options asn1_hdr_dl_opts;

  if (hdr_cfg == sdap_hdr_dl_cfg::absent) {
    asn1_hdr_dl_opts = asn1::rrc_nr::sdap_cfg_s::sdap_hdr_dl_opts::options::absent;
  } else {
    asn1_hdr_dl_opts = asn1::rrc_nr::sdap_cfg_s::sdap_hdr_dl_opts::options::present;
  }

  return asn1_hdr_dl_opts;
}

asn1::rrc_nr::sdap_cfg_s ocudu::ocucp::sdap_config_to_rrc_asn1(const sdap_config_t& sdap_cfg)
{
  asn1::rrc_nr::sdap_cfg_s asn1_sdap_cfg;

  // Fill PDU session.
  asn1_sdap_cfg.pdu_session = to_underlying(sdap_cfg.pdu_session);

  // Fill SDAP hdr DL.
  asn1_sdap_cfg.sdap_hdr_dl = sdap_hdr_dl_cfg_to_rrc_asn1(sdap_cfg.sdap_hdr_dl);

  // Fill SDAP hdr UL.
  asn1_sdap_cfg.sdap_hdr_ul = sdap_hdr_ul_cfg_to_rrc_asn1(sdap_cfg.sdap_hdr_ul);

  // Fill default DRB.
  asn1_sdap_cfg.default_drb = sdap_cfg.default_drb;

  // Fill mapped QoS flows to add.
  for (const auto& mapped_qos_flow_to_add : sdap_cfg.mapped_qos_flows_to_add) {
    asn1_sdap_cfg.mapped_qos_flows_to_add.push_back(to_underlying(mapped_qos_flow_to_add));
  }

  // Fill mapped QoS flows to release.
  for (const auto& mapped_qos_flow_to_release : sdap_cfg.mapped_qos_flows_to_release) {
    asn1_sdap_cfg.mapped_qos_flows_to_release.push_back(to_underlying(mapped_qos_flow_to_release));
  }

  return asn1_sdap_cfg;
}

asn1::rrc_nr::ciphering_algorithm_e
ocudu::ocucp::ciphering_algorithm_to_rrc_asn1(const security::ciphering_algorithm& ciphering_algo)
{
  asn1::rrc_nr::ciphering_algorithm_e asn1_ciphering_algo;

  switch (ciphering_algo) {
    case ocudu::security::ciphering_algorithm::nea0:
      asn1_ciphering_algo = asn1::rrc_nr::ciphering_algorithm_opts::options::nea0;
      break;
    case ocudu::security::ciphering_algorithm::nea1:
      asn1_ciphering_algo = asn1::rrc_nr::ciphering_algorithm_opts::options::nea1;
      break;
    case ocudu::security::ciphering_algorithm::nea2:
      asn1_ciphering_algo = asn1::rrc_nr::ciphering_algorithm_opts::options::nea2;
      break;
    case ocudu::security::ciphering_algorithm::nea3:
      asn1_ciphering_algo = asn1::rrc_nr::ciphering_algorithm_opts::options::nea3;
      break;
    default:
      // Error.
      report_fatal_error("Cannot convert ciphering algorithm {} to ASN.1 type", ciphering_algo);
  }

  return asn1_ciphering_algo;
}

asn1::rrc_nr::integrity_prot_algorithm_e
ocudu::ocucp::integrity_prot_algorithm_to_rrc_asn1(const security::integrity_algorithm& integrity_prot_algo)
{
  asn1::rrc_nr::integrity_prot_algorithm_e asn1_integrity_prot_algo;

  switch (integrity_prot_algo) {
    case ocudu::security::integrity_algorithm::nia0:
      asn1_integrity_prot_algo = asn1::rrc_nr::integrity_prot_algorithm_opts::options::nia0;
      break;
    case ocudu::security::integrity_algorithm::nia1:
      asn1_integrity_prot_algo = asn1::rrc_nr::integrity_prot_algorithm_opts::options::nia1;
      break;
    case ocudu::security::integrity_algorithm::nia2:
      asn1_integrity_prot_algo = asn1::rrc_nr::integrity_prot_algorithm_opts::options::nia2;
      break;
    case ocudu::security::integrity_algorithm::nia3:
      asn1_integrity_prot_algo = asn1::rrc_nr::integrity_prot_algorithm_opts::options::nia3;
      break;
    default:
      // Error.
      report_fatal_error("Cannot convert integrity_prot algorithm {} to ASN.1 type", integrity_prot_algo);
  }

  return asn1_integrity_prot_algo;
}

five_g_s_tmsi_t ocudu::ocucp::asn1_to_five_g_s_tmsi(const asn1::fixed_bitstring<48>& asn1_five_g_s_tmsi)
{
  bounded_bitset<48> five_g_s_tmsi(48);
  five_g_s_tmsi.from_uint64(asn1_five_g_s_tmsi.to_number());

  return five_g_s_tmsi_t{five_g_s_tmsi};
}

five_g_s_tmsi_t ocudu::ocucp::asn1_to_five_g_s_tmsi(const asn1::fixed_bitstring<39>& asn1_five_g_s_tmsi_part1,
                                                    const asn1::fixed_bitstring<9>&  asn1_five_g_s_tmsi_part2)
{
  bounded_bitset<48> five_g_s_tmsi(48);
  five_g_s_tmsi.from_uint64((asn1_five_g_s_tmsi_part2.to_number() << 39) + asn1_five_g_s_tmsi_part1.to_number());

  return five_g_s_tmsi_t{five_g_s_tmsi};
}

s_nssai_t ocudu::ocucp::asn1_to_s_nssai(const asn1::rrc_nr::s_nssai_c& asn1_s_nssai)
{
  s_nssai_t s_nssai;
  if (asn1_s_nssai.type().value == asn1::rrc_nr::s_nssai_c::types_opts::sst) {
    s_nssai.sst = slice_service_type{static_cast<uint8_t>(asn1_s_nssai.sst().to_number())};
    return s_nssai;
  }

  // The SST comes first, then the SD (TS 23.003 section 28.4.2, TS 24.501 section 9.11.2.8).
  uint32_t sst_sd = asn1_s_nssai.sst_sd().to_number();
  s_nssai.sst     = slice_service_type{static_cast<uint8_t>(sst_sd >> 24U)};
  s_nssai.sd      = slice_differentiator::create(sst_sd & 0xffffffU).value();
  return s_nssai;
}

amf_identifier_t ocudu::ocucp::asn1_to_amf_identifier(const asn1::fixed_bitstring<24>& asn1_amf_id)
{
  amf_identifier_t amf_id;

  uint32_t amf_identifier = 0;
  amf_identifier          = (uint32_t)asn1_amf_id.to_number();

  amf_id.amf_region_id = amf_identifier >> 16U;
  amf_id.amf_set_id    = (amf_identifier - (amf_id.amf_region_id << 16U)) >> 6U;
  amf_id.amf_pointer   = (amf_identifier << 26U) >> 26U;

  return amf_id;
}

establishment_cause_t
ocudu::ocucp::asn1_to_establishment_cause(const asn1::rrc_nr::establishment_cause_opts::options& asn1_cause)
{
  establishment_cause_t cause;
  switch (asn1_cause) {
    case asn1::rrc_nr::establishment_cause_opts::options::emergency:
      cause = establishment_cause_t::emergency;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::high_prio_access:
      cause = establishment_cause_t::high_prio_access;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::mt_access:
      cause = establishment_cause_t::mt_access;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::mo_sig:
      cause = establishment_cause_t::mo_sig;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::mo_data:
      cause = establishment_cause_t::mo_data;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::mo_voice_call:
      cause = establishment_cause_t::mo_voice_call;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::mo_video_call:
      cause = establishment_cause_t::mo_video_call;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::mo_sms:
      cause = establishment_cause_t::mo_sms;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::mps_prio_access:
      cause = establishment_cause_t::mps_prio_access;
      break;
    case asn1::rrc_nr::establishment_cause_opts::options::mcs_prio_access:
      cause = establishment_cause_t::mcs_prio_access;
      break;
    default:
      cause = establishment_cause_t::unknown;
      break;
  }

  return cause;
}

resume_cause_t ocudu::ocucp::asn1_to_resume_cause(const asn1::rrc_nr::resume_cause_opts::options& asn1_cause)
{
  resume_cause_t cause;
  switch (asn1_cause) {
    case asn1::rrc_nr::resume_cause_opts::options::emergency:
      cause = resume_cause_t::emergency;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::high_prio_access:
      cause = resume_cause_t::high_prio_access;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mt_access:
      cause = resume_cause_t::mt_access;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_sig:
      cause = resume_cause_t::mo_sig;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_data:
      cause = resume_cause_t::mo_data;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_voice_call:
      cause = resume_cause_t::mo_voice_call;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_video_call:
      cause = resume_cause_t::mo_video_call;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_sms:
      cause = resume_cause_t::mo_sms;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::rna_upd:
      cause = resume_cause_t::rna_upd;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mps_prio_access:
      cause = resume_cause_t::mps_prio_access;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mcs_prio_access:
      cause = resume_cause_t::mcs_prio_access;
      break;
    default:
      cause = resume_cause_t::unknown;
      break;
  }

  return cause;
}

establishment_cause_t
ocudu::ocucp::asn1_resume_cause_to_establishment_cause(const asn1::rrc_nr::resume_cause_e& asn1_cause)
{
  switch (asn1_cause) {
    case asn1::rrc_nr::resume_cause_opts::options::emergency:
      return establishment_cause_t::emergency;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::high_prio_access:
      return establishment_cause_t::high_prio_access;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mt_access:
      return establishment_cause_t::mt_access;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_sig:
      return establishment_cause_t::mo_sig;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_data:
      return establishment_cause_t::mo_data;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_voice_call:
      return establishment_cause_t::mo_voice_call;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_video_call:
      return establishment_cause_t::mo_video_call;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mo_sms:
      return establishment_cause_t::mo_sms;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mps_prio_access:
      return establishment_cause_t::mps_prio_access;
      break;
    case asn1::rrc_nr::resume_cause_opts::options::mcs_prio_access:
      return establishment_cause_t::mcs_prio_access;
      break;
    default:
      return establishment_cause_t::unknown;
      break;
  }
}

asn1::enumerated<asn1::rrc_nr::paging_cycle_opts> ocudu::ocucp::ran_paging_cycle_to_asn1(uint16_t ran_paging_cycle)
{
  if (ran_paging_cycle == 32) {
    return asn1::rrc_nr::paging_cycle_opts::options::rf32;
  }
  if (ran_paging_cycle == 64) {
    return asn1::rrc_nr::paging_cycle_opts::options::rf64;
  }
  if (ran_paging_cycle == 128) {
    return asn1::rrc_nr::paging_cycle_opts::options::rf128;
  }

  return asn1::rrc_nr::paging_cycle_opts::options::rf256;
}

asn1::rrc_nr::plmn_id_s ocudu::ocucp::plmn_to_asn1(const plmn_identity& plmn)
{
  asn1::rrc_nr::plmn_id_s asn1_plmn;

  asn1_plmn.mcc_present         = true;
  asn1_plmn.mcc                 = plmn.mcc().to_bytes();
  static_vector<uint8_t, 3> mnc = plmn.mnc().to_bytes();
  asn1_plmn.mnc.resize(mnc.size());
  for (unsigned i = 0, sz = mnc.size(); i != sz; ++i) {
    asn1_plmn.mnc[i] = mnc[i];
  }

  return asn1_plmn;
}

asn1::rrc_nr::ran_notif_area_info_c
ocudu::ocucp::ran_notification_area_info_to_asn1(const rrc_ran_notification_area_info_t& ran_notification_area_info)
{
  asn1::rrc_nr::ran_notif_area_info_c asn1_ran_notification_area_info;

  if (std::holds_alternative<std::vector<rrc_plmn_ran_area_cell_t>>(ran_notification_area_info)) {
    // Fill PLMN RAN Area Cell List.
    auto& asn1_cell_list = asn1_ran_notification_area_info.set_cell_list();

    for (const auto& plmn_ran_area_cell : std::get<std::vector<rrc_plmn_ran_area_cell_t>>(ran_notification_area_info)) {
      asn1::rrc_nr::plmn_ran_area_cell_s asn1_plmn_ran_area_cell;
      // Fill PLMN Identity.
      if (plmn_ran_area_cell.plmn_id.has_value()) {
        asn1_plmn_ran_area_cell.plmn_id_present = true;
        asn1_plmn_ran_area_cell.plmn_id         = plmn_to_asn1(plmn_ran_area_cell.plmn_id.value());
      }

      // Fill RAN area cells.
      for (const auto& ran_area_cell : plmn_ran_area_cell.ran_area_cells) {
        asn1::fixed_bitstring<36> asn1_ran_area_cell;
        asn1_plmn_ran_area_cell.ran_area_cells.push_back(asn1_ran_area_cell.from_number(ran_area_cell.value()));
      }

      asn1_cell_list.push_back(asn1_plmn_ran_area_cell);
    }

  } else {
    // Fill RAN area config list.
    auto& asn1_ran_area_config_list = asn1_ran_notification_area_info.set_ran_area_cfg_list();

    for (const auto& plmn_ran_area_config :
         std::get<std::vector<rrc_plmn_ran_area_cfg_t>>(ran_notification_area_info)) {
      asn1::rrc_nr::plmn_ran_area_cfg_s asn1_plmn_ran_area_config;

      // Fill PLMN Identity.
      if (plmn_ran_area_config.plmn_id.has_value()) {
        asn1_plmn_ran_area_config.plmn_id_present = true;
        asn1_plmn_ran_area_config.plmn_id         = plmn_to_asn1(plmn_ran_area_config.plmn_id.value());
      }

      // Fill RAN area.
      for (const auto& ran_area : plmn_ran_area_config.ran_area) {
        asn1::rrc_nr::ran_area_cfg_s asn1_ran_area;
        asn1_ran_area.tac.from_number(ran_area.tac);

        for (const auto& ran_area_code : ran_area.ran_area_code_list) {
          asn1_ran_area.ran_area_code_list.push_back(ran_area_code);
        }

        asn1_plmn_ran_area_config.ran_area.push_back(asn1_ran_area);
      }

      asn1_ran_area_config_list.push_back(asn1_plmn_ran_area_config);
    }
  }

  return asn1_ran_notification_area_info;
}

void ocudu::ocucp::radio_bearer_config_to_asn1(const rrc_radio_bearer_config&    radio_bearer_cfg,
                                               asn1::rrc_nr::radio_bearer_cfg_s& asn1_radio_bearer_cfg)
{
  // Fill SRB to add mod list.
  for (const auto& srb_to_add : radio_bearer_cfg.srb_to_add_mod_list) {
    ocudu_assert(srb_to_add.srb_id != srb_id_t::nulltype, "Invalid SRB ID");

    asn1::rrc_nr::srb_to_add_mod_s asn1_srb_to_add;
    asn1_srb_to_add.srb_id = srb_id_to_uint(srb_to_add.srb_id);

    asn1_srb_to_add.reestablish_pdcp_present = srb_to_add.reestablish_pdcp_present;

    asn1_srb_to_add.discard_on_pdcp_present = srb_to_add.discard_on_pdcp_present;

    // Fill PDCP config.
    if (srb_to_add.pdcp_cfg.has_value()) {
      asn1_srb_to_add.pdcp_cfg_present = true;
      asn1_srb_to_add.pdcp_cfg         = pdcp_config_to_rrc_nr_asn1(srb_to_add.pdcp_cfg.value());
    }

    asn1_radio_bearer_cfg.srb_to_add_mod_list.push_back(asn1_srb_to_add);
  }

  // Fill DRB to add mod list.
  for (const auto& drb_to_add : radio_bearer_cfg.drb_to_add_mod_list) {
    ocudu_assert(drb_to_add.drb_id != drb_id_t::invalid, "Invalid DRB ID");

    asn1::rrc_nr::drb_to_add_mod_s asn1_drb_to_add;
    asn1_drb_to_add.drb_id = to_underlying(drb_to_add.drb_id);

    asn1_drb_to_add.reestablish_pdcp_present = drb_to_add.reestablish_pdcp_present;

    // Fill PDCP config.
    if (drb_to_add.pdcp_cfg.has_value()) {
      asn1_drb_to_add.pdcp_cfg_present = true;
      asn1_drb_to_add.pdcp_cfg         = pdcp_config_to_rrc_nr_asn1(drb_to_add.pdcp_cfg.value());
    }

    // Fill CN association and SDAP config.
    if (drb_to_add.cn_assoc.has_value()) {
      asn1_drb_to_add.cn_assoc_present = true;
      if (drb_to_add.cn_assoc.value().sdap_cfg.has_value()) {
        asn1_drb_to_add.cn_assoc.set_sdap_cfg();
        asn1_drb_to_add.cn_assoc.sdap_cfg() = sdap_config_to_rrc_asn1(drb_to_add.cn_assoc.value().sdap_cfg.value());
      } else {
        asn1_drb_to_add.cn_assoc.set_eps_bearer_id();
        asn1_drb_to_add.cn_assoc.eps_bearer_id() = drb_to_add.cn_assoc.value().eps_bearer_id.value();
      }
    }

    asn1_radio_bearer_cfg.drb_to_add_mod_list.push_back(asn1_drb_to_add);
  }

  // Fill DRB to release list.
  for (const auto& drb_to_release : radio_bearer_cfg.drb_to_release_list) {
    ocudu_assert(drb_to_release != drb_id_t::invalid, "Invalid DRB ID");
    asn1_radio_bearer_cfg.drb_to_release_list.push_back(to_underlying(drb_to_release));
  }

  // Fill security config.
  if (radio_bearer_cfg.security_cfg.has_value()) {
    asn1_radio_bearer_cfg.security_cfg_present = true;

    const auto& security_cfg = radio_bearer_cfg.security_cfg.value();

    // Fill security algorithm config.
    if (security_cfg.security_algorithm_cfg.has_value()) {
      asn1_radio_bearer_cfg.security_cfg.security_algorithm_cfg_present = true;

      // Fill ciphering algorithm config.
      asn1_radio_bearer_cfg.security_cfg.security_algorithm_cfg.ciphering_algorithm =
          ciphering_algorithm_to_rrc_asn1(security_cfg.security_algorithm_cfg.value().ciphering_algorithm);

      // Fill integrity prot algorithm config.
      if (security_cfg.security_algorithm_cfg.value().integrity_prot_algorithm.has_value()) {
        asn1_radio_bearer_cfg.security_cfg.security_algorithm_cfg.integrity_prot_algorithm_present = true;
        asn1_radio_bearer_cfg.security_cfg.security_algorithm_cfg.integrity_prot_algorithm =
            integrity_prot_algorithm_to_rrc_asn1(
                security_cfg.security_algorithm_cfg.value().integrity_prot_algorithm.value());
      }
    }
    // Fill key to use.
    if (security_cfg.key_to_use.has_value()) {
      asn1_radio_bearer_cfg.security_cfg.key_to_use_present = true;
      asn1::string_to_enum(asn1_radio_bearer_cfg.security_cfg.key_to_use, security_cfg.key_to_use.value());
    }
  }

  // Fill SRB3 to release present.
  asn1_radio_bearer_cfg.srb3_to_release_present = radio_bearer_cfg.srb3_to_release_present;
}

rrc_ue_cap_rat_container_list_t
ocudu::ocucp::asn1_to_ue_cap_rat_container_list(const asn1::rrc_nr::ue_cap_rat_container_list_l& asn1_capabilities_list)
{
  rrc_ue_cap_rat_container_list_t capabilities_list;
  for (const auto& asn1_container : asn1_capabilities_list) {
    if (capabilities_list.size() == capabilities_list.capacity()) {
      break;
    }
    rrc_ue_cap_rat_container_t container;
    container.rat_type             = static_cast<rat_type_t>(asn1_container.rat_type.value);
    container.ue_cap_rat_container = asn1_container.ue_cap_rat_container.copy();
    capabilities_list.push_back(std::move(container));
  }
  return capabilities_list;
}

asn1::rrc_nr::ue_cap_rat_container_list_l
ocudu::ocucp::ue_cap_rat_container_list_to_asn1(const rrc_ue_cap_rat_container_list_t& capabilities_list)
{
  asn1::rrc_nr::ue_cap_rat_container_list_l asn1_capabilities_list;
  asn1_capabilities_list.resize(capabilities_list.size());
  for (unsigned i = 0, e = capabilities_list.size(); i != e; ++i) {
    asn1_capabilities_list[i].rat_type.value =
        static_cast<asn1::rrc_nr::rat_type_opts::options>(capabilities_list[i].rat_type);
    asn1_capabilities_list[i].ue_cap_rat_container = capabilities_list[i].ue_cap_rat_container.copy();
  }
  return asn1_capabilities_list;
}

expected<plmn_identity> ocudu::ocucp::asn1_to_plmn_identity(const asn1::rrc_nr::plmn_id_s& asn1_plmn,
                                                            const plmn_identity&           plmn_of_absent_mcc)
{
  expected<mobile_country_code> mcc = plmn_of_absent_mcc.mcc();
  if (asn1_plmn.mcc_present) {
    mcc = mobile_country_code::from_bytes(asn1_plmn.mcc);
  }
  expected<mobile_network_code> mnc =
      mobile_network_code::from_bytes(span<const uint8_t>(asn1_plmn.mnc.data(), asn1_plmn.mnc.size()));
  if (not mcc.has_value() or not mnc.has_value()) {
    return make_unexpected(default_error_t{});
  }
  return plmn_identity{mcc.value(), mnc.value()};
}
