// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "nrppa_test_messages.h"
#include "lib/nrppa/nrppa_asn1_converters.h"
#include "ocudu/adt/byte_buffer.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/nrppa/common.h"
#include "ocudu/asn1/nrppa/nrppa.h"
#include "ocudu/asn1/nrppa/nrppa_pdu_contents.h"
#include <vector>

using namespace ocudu;
using namespace ocucp;
using namespace asn1::nrppa;

byte_buffer ocudu::ocucp::generate_valid_nrppa_e_cid_measurement_initiation_request(
    lmf_ue_meas_id_t                               lmf_ue_meas_id,
    const std::vector<nrppa_meas_quantities_item>& meas_quantities)
{
  asn1::nrppa::nr_ppa_pdu_c pdu;

  pdu.set_init_msg().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_INITIATION);
  pdu.init_msg().nrppatransaction_id                 = 5;
  asn1::nrppa::e_c_id_meas_initiation_request_s& req = pdu.init_msg().value.e_c_id_meas_initiation_request();

  req->lmf_ue_meas_id = to_underlying(lmf_ue_meas_id);

  req->report_characteristics = report_characteristics_opts::on_demand;

  for (const auto& meas_quant : meas_quantities) {
    asn1::protocol_ie_single_container_s<asn1::nrppa::meas_quantities_item_ies_o> meas_quantities_item_container;
    asn1::nrppa::meas_quantities_item_s& quantities_item = meas_quantities_item_container->meas_quantities_item();
    quantities_item                                      = meas_quantities_item_to_asn1(meas_quant);

    req->meas_quantities.push_back(meas_quantities_item_container);
  }

  return pack_into_pdu(pdu, "ECIDMeasInitiationRequest");
}

byte_buffer ocudu::ocucp::generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_reports(
    lmf_ue_meas_id_t lmf_ue_meas_id,
    bool             request_aoa)
{
  asn1::nrppa::nr_ppa_pdu_c pdu;

  pdu.set_init_msg().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_INITIATION);
  pdu.init_msg().nrppatransaction_id                 = 5;
  asn1::nrppa::e_c_id_meas_initiation_request_s& req = pdu.init_msg().value.e_c_id_meas_initiation_request();

  req->lmf_ue_meas_id = to_underlying(lmf_ue_meas_id);

  req->report_characteristics   = report_characteristics_opts::periodic;
  req->meas_periodicity_present = true;
  req->meas_periodicity         = meas_periodicity_opts::ms120;

  asn1::protocol_ie_single_container_s<asn1::nrppa::meas_quantities_item_ies_o> meas_quantities_item_container;
  asn1::nrppa::meas_quantities_item_s& quantities_item = meas_quantities_item_container->meas_quantities_item();
  quantities_item.meas_quantities_value = request_aoa ? asn1::nrppa::meas_quantities_value_opts::angle_of_arrival_nr
                                                      : asn1::nrppa::meas_quantities_value_opts::ss_rsrp;

  req->meas_quantities.push_back(meas_quantities_item_container);

  return pack_into_pdu(pdu, "ECIDMeasInitiationRequest");
}

byte_buffer ocudu::ocucp::generate_valid_nrppa_e_cid_measurement_initiation_request_with_periodic_aoa(
    lmf_ue_meas_id_t                                                     lmf_ue_meas_id,
    const std::vector<asn1::nrppa::meas_quantities_value_opts::options>& other_quantities)
{
  asn1::nrppa::nr_ppa_pdu_c pdu;

  pdu.set_init_msg().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_INITIATION);
  pdu.init_msg().nrppatransaction_id                 = 5;
  asn1::nrppa::e_c_id_meas_initiation_request_s& req = pdu.init_msg().value.e_c_id_meas_initiation_request();

  req->lmf_ue_meas_id = to_underlying(lmf_ue_meas_id);

  req->report_characteristics   = report_characteristics_opts::periodic;
  req->meas_periodicity_present = true;
  req->meas_periodicity         = meas_periodicity_opts::ms120;

  // NR Angle of Arrival carries its own periodicity, as per TS 38.455 section 9.1.1.1.
  req->meas_periodicity_nr_ao_a_present = true;
  req->meas_periodicity_nr_ao_a         = asn1::nrppa::meas_periodicity_nr_ao_a_opts::ms1280;

  asn1::protocol_ie_single_container_s<asn1::nrppa::meas_quantities_item_ies_o> meas_quantities_item_container;
  asn1::nrppa::meas_quantities_item_s& quantities_item = meas_quantities_item_container->meas_quantities_item();
  quantities_item.meas_quantities_value                = asn1::nrppa::meas_quantities_value_opts::angle_of_arrival_nr;

  req->meas_quantities.push_back(meas_quantities_item_container);

  for (auto quantity : other_quantities) {
    asn1::protocol_ie_single_container_s<asn1::nrppa::meas_quantities_item_ies_o> other_container;
    other_container->meas_quantities_item().meas_quantities_value = quantity;
    req->meas_quantities.push_back(other_container);
  }

  return pack_into_pdu(pdu, "ECIDMeasInitiationRequest");
}

byte_buffer ocudu::ocucp::generate_valid_nrppa_e_cid_measurement_termination_command(lmf_ue_meas_id_t lmf_ue_meas_id,
                                                                                     ran_ue_meas_id_t ran_ue_meas_id)
{
  asn1::nrppa::nr_ppa_pdu_c pdu;

  pdu.set_init_msg().load_info_obj(ASN1_NRPPA_ID_E_C_ID_MEAS_TERMINATION);
  pdu.init_msg().nrppatransaction_id              = 5;
  asn1::nrppa::e_c_id_meas_termination_cmd_s& cmd = pdu.init_msg().value.e_c_id_meas_termination_cmd();

  cmd->lmf_ue_meas_id = to_underlying(lmf_ue_meas_id);
  cmd->ran_ue_meas_id = to_underlying(ran_ue_meas_id);

  return pack_into_pdu(pdu, "ECIDMeasTerminationCommand");
}

byte_buffer ocudu::ocucp::generate_valid_trp_information_request()
{
  asn1::nrppa::nr_ppa_pdu_c pdu;

  pdu.set_init_msg().load_info_obj(ASN1_NRPPA_ID_T_RP_INFO_EXCHANGE);
  pdu.init_msg().nrppatransaction_id   = 5;
  asn1::nrppa::trp_info_request_s& req = pdu.init_msg().value.trp_info_request();

  std::vector<asn1::nrppa::trp_info_type_item_opts::options> trp_info_type_items = {
      asn1::nrppa::trp_info_type_item_opts::options::nr_pci,
      asn1::nrppa::trp_info_type_item_opts::options::ng_ran_cgi,
      asn1::nrppa::trp_info_type_item_opts::options::arfcn,
      asn1::nrppa::trp_info_type_item_opts::options::prs_cfg,
      asn1::nrppa::trp_info_type_item_opts::options::ssb_info,
      asn1::nrppa::trp_info_type_item_opts::options::sfn_init_time,
      asn1::nrppa::trp_info_type_item_opts::options::spatial_direct_info,
      asn1::nrppa::trp_info_type_item_opts::options::geo_coord,
      asn1::nrppa::trp_info_type_item_opts::options::trp_type};

  for (const auto& trp_info_type_item : trp_info_type_items) {
    asn1::protocol_ie_single_container_s<asn1::nrppa::trp_info_type_item_trp_req_o> trp_info_type_item_container;
    trp_info_type_item_container->trp_info_type_item() = trp_info_type_item;

    req->trp_info_type_list_trp_req.push_back(trp_info_type_item_container);
  }

  return pack_into_pdu(pdu, "TRPInformationRequest");
}

byte_buffer ocudu::ocucp::generate_valid_positioning_information_request()
{
  asn1::nrppa::nr_ppa_pdu_c pdu;

  pdu.set_init_msg().load_info_obj(ASN1_NRPPA_ID_POSITIONING_INFO_EXCHANGE);
  pdu.init_msg().nrppatransaction_id = 5;

  asn1::nrppa::positioning_info_request_s& req          = pdu.init_msg().value.positioning_info_request();
  req->requested_srs_tx_characteristics_present         = true;
  req->requested_srs_tx_characteristics.nof_txs_present = true;
  req->requested_srs_tx_characteristics.nof_txs         = 0;
  req->requested_srs_tx_characteristics.res_type =
      asn1::nrppa::requested_srs_tx_characteristics_s::res_type_opts::options::periodic;
  req->requested_srs_tx_characteristics.bw.set_fr1() = asn1::nrppa::bw_srs_c::fr1_opts::options::mhz100;

  return pack_into_pdu(pdu, "PositioningInformationRequest");
}

byte_buffer ocudu::ocucp::generate_valid_positioning_activation_request()
{
  asn1::nrppa::nr_ppa_pdu_c pdu;

  pdu.set_init_msg().load_info_obj(ASN1_NRPPA_ID_POSITIONING_ACTIVATION);
  pdu.init_msg().nrppatransaction_id = 5;

  asn1::nrppa::positioning_activation_request_s& req       = pdu.init_msg().value.positioning_activation_request();
  asn1::nrppa::aperiodic_srs_s&                  aperiodic = req->srs_type.set_aperiodic_srs();
  aperiodic.aperiodic = asn1::nrppa::aperiodic_srs_s::aperiodic_opts::options::true_value;

  return pack_into_pdu(pdu, "PositioningActivationRequest");
}

static inline asn1::nrppa::srs_configuration_s generate_srscfg()
{
  asn1::nrppa::srs_configuration_s srs_configuration;

  asn1::nrppa::srs_carrier_list_item_s srs_carrier_item;
  srs_carrier_item.point_a = 632016;

  // Fill UL CH BW per SCS list.
  scs_specific_carrier_s scs_specific_carrier;
  scs_specific_carrier.offset_to_carrier = 0;
  scs_specific_carrier.subcarrier_spacing =
      asn1::nrppa::scs_specific_carrier_s::subcarrier_spacing_opts::options::khz30;
  scs_specific_carrier.carrier_bw = 51;

  srs_carrier_item.ul_ch_bw_per_scs_list.push_back(scs_specific_carrier);

  // Fill Active UL BWP.
  srs_carrier_item.active_ul_bwp.location_and_bw = 13750;
  srs_carrier_item.active_ul_bwp.subcarrier_spacing =
      asn1::nrppa::active_ul_bwp_s::subcarrier_spacing_opts::options::khz30;
  srs_carrier_item.active_ul_bwp.cp                         = asn1::nrppa::active_ul_bwp_s::cp_opts::options::normal;
  srs_carrier_item.active_ul_bwp.tx_direct_current_location = 3300;

  asn1::nrppa::srs_res_s srs_res;
  srs_res.srs_res_id       = 0;
  srs_res.nrof_srs_ports   = asn1::nrppa::srs_res_s::nrof_srs_ports_opts::options::port1;
  srs_res.tx_comb.set_n2() = asn1::nrppa::tx_comb_c::n2_s_{
      .comb_offset_n2  = 0,
      .cyclic_shift_n2 = 0,
  };
  srs_res.start_position        = 0;
  srs_res.nrof_symbols          = asn1::nrppa::srs_res_s::nrof_symbols_opts::options::n1;
  srs_res.repeat_factor         = asn1::nrppa::srs_res_s::repeat_factor_opts::options::n1;
  srs_res.freq_domain_position  = 0;
  srs_res.freq_domain_shift     = 0;
  srs_res.c_srs                 = 0;
  srs_res.b_srs                 = 0;
  srs_res.b_hop                 = 0;
  srs_res.group_or_seq_hop      = asn1::nrppa::srs_res_s::group_or_seq_hop_opts::options::neither;
  auto& periodic_res_type       = srs_res.res_type.set_periodic();
  periodic_res_type.periodicity = asn1::nrppa::res_type_periodic_s::periodicity_opts::options::slot80;
  periodic_res_type.offset      = 7;
  srs_carrier_item.active_ul_bwp.srs_cfg.srs_res_list.push_back(srs_res);

  asn1::nrppa::srs_res_set_s srs_res_set;
  srs_res_set.srs_res_set_id1 = 0;
  srs_res_set.srs_res_id_list.push_back(0);
  auto& periodic_res_set_type        = srs_res_set.res_set_type.set_periodic();
  periodic_res_set_type.periodic_set = asn1::nrppa::res_set_type_periodic_s::periodic_set_opts::options::true_value;

  srs_carrier_item.active_ul_bwp.srs_cfg.srs_res_set_list.push_back(srs_res_set);

  srs_carrier_item.pci_nr_present = true;
  srs_carrier_item.pci_nr         = 0;

  srs_configuration.srs_carrier_list.push_back(srs_carrier_item);

  return srs_configuration;
}

byte_buffer ocudu::ocucp::generate_valid_nrppa_measurement_request(
    lmf_meas_id_t                                       lmf_meas_id,
    const std::vector<trp_meas_request_item_t>&         trp_meas_request_list,
    const std::vector<trp_meas_quantities_list_item_t>& trp_meas_quantities)
{
  asn1::nrppa::nr_ppa_pdu_c pdu;

  pdu.set_init_msg().load_info_obj(ASN1_NRPPA_ID_MEAS);
  pdu.init_msg().nrppatransaction_id = 5;
  asn1::nrppa::meas_request_s& req   = pdu.init_msg().value.meas_request();

  req->lmf_meas_id = to_underlying(lmf_meas_id);

  for (const auto& trp_meas_request_item : trp_meas_request_list) {
    req->trp_meas_request_list.push_back(trp_meas_request_item_to_asn1(trp_meas_request_item));
  }

  req->report_characteristics = report_characteristics_opts::on_demand;

  for (const auto& trp_meas_quant : trp_meas_quantities) {
    req->trp_meas_quantities.push_back(trp_meas_quantities_list_item_to_asn1(trp_meas_quant));
  }

  req->srs_cfg_present = true;
  req->srs_cfg         = generate_srscfg();

  return pack_into_pdu(pdu, "MeasurementRequest");
}
