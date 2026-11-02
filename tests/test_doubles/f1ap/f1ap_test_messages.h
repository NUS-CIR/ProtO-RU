// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

///\file
///\brief f1ap_test_messages: Contains helper functions to generate dummy F1AP messages for testing purposes.

#pragma once

#include "../rrc/rrc_packed_test_messages.h"
#include "ocudu/adt/byte_buffer.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/f1ap/f1ap_ue_id_types.h"
#include "ocudu/ran/arfcn.h"
#include "ocudu/ran/gnb_du_id.h"
#include "ocudu/ran/nr_band.h"
#include "ocudu/ran/nr_cell_identity.h"
#include "ocudu/ran/pci.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/ran/positioning/common.h"
#include "ocudu/ran/positioning/positioning_ids.h"
#include "ocudu/ran/rb_id.h"
#include "ocudu/ran/rnti.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include "ocudu/ran/tac.h"

namespace ocudu {
struct f1ap_cell_failed_to_activate;
struct f1ap_cell_to_bar;
struct nr_cell_global_id_t;

struct f1ap_message;

namespace test_helpers {

struct served_cell_item_info {
  struct meas_timing_config {
    uint32_t           carrier_freq = 620928;
    subcarrier_spacing scs          = subcarrier_spacing::kHz30;
  };

  plmn_identity      plmn_id  = plmn_identity::test_value();
  nr_cell_identity   nci      = nr_cell_identity::create(gnb_id_t{411, 22}, 0U).value();
  pci_t              pci      = 0;
  tac_t              tac      = 7;
  nr_band            band     = nr_band::n78;
  arfcn_t            nr_arfcn = 620688;
  meas_timing_config meas_timing_cfg;
  std::string        sib1_str = create_sib1_hex_string(plmn_identity::test_value());
};

/// \brief Generates dummy F1 RESET message, sent by the CU to the DU, as per TS 38.473 section 8.2.1.1.
f1ap_message generate_f1ap_reset_message(
    const std::vector<std::pair<std::optional<gnb_du_ue_f1ap_id_t>, std::optional<gnb_cu_ue_f1ap_id_t>>>& ues_to_reset =
        {});

/// \brief Generate a dummy F1AP Served Cell Item.
asn1::f1ap::gnb_du_served_cells_item_s generate_served_cells_item(const served_cell_item_info& info);

/// \brief Generates dummy F1AP SETUP REQUEST message, sent by the DU to the CU, as per TS 38.473 section 8.2.3.1.
f1ap_message generate_f1_setup_request(gnb_du_id_t                               gnb_du_id = int_to_gnb_du_id(0x11),
                                       const std::vector<served_cell_item_info>& cells     = {served_cell_item_info{}});

/// \brief Generates dummy F1 SETUP RESPONSE message based on the request, sent by the CU to the DU, as per TS 38.473
/// section 8.2.3.2.
f1ap_message generate_f1_setup_response(const f1ap_message& f1_setup_request, bool activate_cells = true);

/// \brief Generates dummy F1 SETUP FAILURE message based on the request, sent by the CU to the DU, as per TS 38.473
/// section 8.2.3.3.
f1ap_message generate_f1_setup_failure(const f1ap_message& f1_setup_request);

/// \brief Generates a GNB-DU CONFIGURATION UPDATE message, sent by the DU to the CU, as per TS 38.473, 8.2.4.1.
/// \param[in] gnb_du_id The gNB-DU ID the DU reports.
/// \param[in] cells_to_add The cells the DU starts serving.
/// \param[in] cells_to_mod The cells the DU reconfigures, each with the identity it had before.
/// \param[in] cells_to_rem The cells the DU stops serving.
f1ap_message generate_gnb_du_configuration_update(
    gnb_du_id_t                                                               gnb_du_id    = int_to_gnb_du_id(0x11),
    const std::vector<served_cell_item_info>&                                 cells_to_add = {},
    const std::vector<std::pair<nr_cell_global_id_t, served_cell_item_info>>& cells_to_mod = {},
    const std::vector<nr_cell_global_id_t>&                                   cells_to_rem = {});

/// \brief Generates dummy GNB-DU CONFIGURATION UPDATE ACKNOWLEDGE message based on the request, sent by the CU to the
/// DU, as per TS 38.473, 8.2.4.2.
f1ap_message generate_gnb_du_configuration_update_acknowledge(const f1ap_message& gnb_du_config_update);

/// \brief Generates dummy GNB-DU CONFIGURATION UPDATE FAILURE message based on the request, sent by the CU to the DU,
/// as per TS 38.473, 8.2.4.3.
f1ap_message generate_gnb_du_configuration_update_failure(const f1ap_message& gnb_du_config_update);

/// \brief Generates dummy GNB-CU CONFIGURATION UPDATE REQUEST message, sent by the CU to the DU, as per
/// TS 38.473, 8.2.5.1.
f1ap_message generate_gnb_cu_configuration_update_request(unsigned                        transaction_id,
                                                          span<const nr_cell_global_id_t> cgis_to_activate,
                                                          span<const nr_cell_global_id_t> cgis_to_deactivate = {},
                                                          span<const f1ap_cell_to_bar>    cells_to_bar       = {});

/// \brief Generate a dummy GNB-CU CONFIGURATION ACKNOWLEDGEMENT message, sent by the DU to the CU, as per
/// TS 38.473, 8.2.5.2.
f1ap_message generate_gnb_cu_configuration_update_acknowledgement(
    const std::vector<f1ap_cell_failed_to_activate>& cells_failed_to_activate);

/// \brief Generate a dummy GNB-CU CONFIGURATION FAILURE message, sent by the DU to the CU, as per
/// TS 38.473, 8.2.5.3.
f1ap_message generate_gnb_cu_configuration_update_failure();

/// \brief Generates dummy F1 REMOVAL REQUEST message, as per TS 38.473 section 8.2.8.1.
f1ap_message generate_f1_removal_request(unsigned transaction_id);

/// \brief Generates dummy F1 REMOVAL RESPONSE message based on the request, as per TS 38.473 section 8.2.8.2.
f1ap_message generate_f1_removal_response(const f1ap_message& f1_removal_request);

/// \brief Generates dummy F1AP UE CONTEXT SETUP REQUEST message, sent by the CU to the DU, as per TS 38.473
/// section 8.3.1.1.
f1ap_message generate_ue_context_setup_request(gnb_cu_ue_f1ap_id_t                cu_ue_id,
                                               std::optional<gnb_du_ue_f1ap_id_t> du_ue_id,
                                               uint32_t                           rrc_container_pdcp_sn,
                                               const std::vector<drb_id_t>&       drbs_to_setup,
                                               nr_cell_global_id_t                nr_cgi);

/// \brief Generates dummy F1AP UE CONTEXT SETUP RESPONSE message, sent by the DU to the CU, as per TS 38.473
/// section 8.3.1.2.
f1ap_message
generate_ue_context_setup_response(gnb_cu_ue_f1ap_id_t   cu_ue_id,
                                   gnb_du_ue_f1ap_id_t   du_ue_id,
                                   std::optional<rnti_t> crnti             = std::nullopt,
                                   byte_buffer           cell_group_config = test_helpers::create_cell_group_config(),
                                   const std::vector<drb_id_t>& drbs_setup_list = {});

/// \brief Generates dummy F1AP UE CONTEXT SETUP FAILURE message, sent by the DU to the CU, as per TS 38.473
/// section 8.3.1.3.
f1ap_message generate_ue_context_setup_failure(gnb_cu_ue_f1ap_id_t cu_ue_id, gnb_du_ue_f1ap_id_t du_ue_id);

/// \brief Generates dummy F1AP UE CONTEXT RELEASE REQUEST message, sent by the DU to the CU, as per TS 38.473
/// section 8.3.2.1.
f1ap_message generate_ue_context_release_request(gnb_cu_ue_f1ap_id_t cu_ue_id, gnb_du_ue_f1ap_id_t du_ue_id);

/// \brief Generates dummy F1AP UE CONTEXT RELEASE COMMAND message, sent by the CU to the DU, as per TS 38.473
/// section 8.3.3.1.
f1ap_message
generate_ue_context_release_command(gnb_cu_ue_f1ap_id_t cu_ue_id,
                                    gnb_du_ue_f1ap_id_t du_ue_id,
                                    srb_id_t            srb_id        = srb_id_t::srb1,
                                    byte_buffer         rrc_container = byte_buffer::create({0x1, 0x2, 0x3}).value());

/// \brief Generates dummy F1AP UE CONTEXT RELEASE COMPLETE message, sent by the DU to the CU, as per TS 38.473
/// section 8.3.3.2.
f1ap_message generate_ue_context_release_complete(const f1ap_message& ue_ctxt_release_cmd);
f1ap_message generate_ue_context_release_complete(gnb_cu_ue_f1ap_id_t cu_ue_id, gnb_du_ue_f1ap_id_t du_ue_id);

/// \brief Generates dummy F1AP UE CONTEXT MODIFICATION REQUEST message, sent by the CU to the DU, as per TS 38.473
/// section 8.3.4.1.
f1ap_message generate_ue_context_modification_request(gnb_du_ue_f1ap_id_t                    du_ue_id,
                                                      gnb_cu_ue_f1ap_id_t                    cu_ue_id,
                                                      const std::initializer_list<drb_id_t>& drbs_to_setup = {},
                                                      const std::initializer_list<drb_id_t>& drbs_to_mod   = {},
                                                      const std::initializer_list<drb_id_t>& drbs_to_rem   = {},
                                                      byte_buffer                            dl_dcch_msg   = {});

/// \brief Generates dummy F1AP UE CONTEXT MODIFICATION RESPONSE message, sent by the DU to the CU, as per TS 38.473
/// section 8.3.4.2.
f1ap_message
generate_ue_context_modification_response(gnb_du_ue_f1ap_id_t          du_ue_id,
                                          gnb_cu_ue_f1ap_id_t          cu_ue_id,
                                          rnti_t                       crnti               = to_rnti(0x4601),
                                          const std::vector<drb_id_t>& drbs_setup_mod_list = {drb_id_t::drb1},
                                          const std::vector<drb_id_t>& drbs_modified_list  = {},
                                          byte_buffer cell_group_config = test_helpers::create_cell_group_config());

/// \brief Generates dummy F1AP UE CONTEXT MODIFICATION FAILURE message, sent by the DU to the CU, as per TS 38.473
/// section 8.3.4.3.
f1ap_message generate_ue_context_modification_failure(gnb_cu_ue_f1ap_id_t cu_ue_id, gnb_du_ue_f1ap_id_t du_ue_id);

/// \brief Generates dummy F1AP INITIAL UL RRC Transfer message without DU to CU container, sent by the DU to the CU, as
/// per TS 38.473 section 8.4.1.1.
f1ap_message
generate_init_ul_rrc_message_transfer_without_du_to_cu_container(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                 rnti_t              crnti = to_rnti(0x4601),
                                                                 plmn_identity plmn_id = plmn_identity::test_value());

/// \brief Generates dummy F1AP Initial UL RRC TRANSFER message, sent by the DU to the CU, as per TS 38.473
/// section 8.4.1.1.
f1ap_message generate_init_ul_rrc_message_transfer(gnb_du_ue_f1ap_id_t du_ue_id,
                                                   rnti_t              crnti          = to_rnti(0x4601),
                                                   plmn_identity       plmn_id        = plmn_identity::test_value(),
                                                   byte_buffer         cell_group_cfg = {},
                                                   byte_buffer         rrc_container  = {});

/// \brief Generates dummy F1AP DL RRC TRANSFER message with provided RRC container, sent by the CU to the DU, as per
/// TS 38.473 section 8.4.2.1.
f1ap_message generate_dl_rrc_message_transfer(gnb_du_ue_f1ap_id_t du_ue_id,
                                              gnb_cu_ue_f1ap_id_t cu_ue_id,
                                              srb_id_t            srb_id,
                                              byte_buffer         rrc_container);

/// \brief Generates dummy F1AP UL RRC TRANSFER message with provided RRC container, sent by the DU to the CU, as per
/// TS 38.473 section 8.4.3.1.
f1ap_message generate_ul_rrc_message_transfer(gnb_du_ue_f1ap_id_t du_ue_id,
                                              gnb_cu_ue_f1ap_id_t cu_ue_id,
                                              srb_id_t            srb_id = srb_id_t::srb1,
                                              byte_buffer rrc_container = byte_buffer::create({0x1, 0x2, 0x3}).value());

/// \brief Generates dummy F1AP UL RRC TRANSFER message with provided RRC UL DCCH message and PDCP header with provided
/// SN, sent by the DU to the CU, as per TS 38.473 section 8.4.3.1.
f1ap_message generate_ul_rrc_message_transfer(gnb_du_ue_f1ap_id_t    du_ue_id,
                                              gnb_cu_ue_f1ap_id_t    cu_ue_id,
                                              srb_id_t               srb_id,
                                              uint32_t               pdcp_sn,
                                              byte_buffer            ul_dcch_msg,
                                              std::array<uint8_t, 4> mac = {0x00, 0x00, 0x00, 0x00});

/// \brief Create RRC Container with PDCP header and DL-DCCH message.
byte_buffer create_dl_dcch_rrc_container(uint32_t pdcp_sn, const std::initializer_list<uint8_t>& dl_dcch_msg);
byte_buffer create_dl_dcch_rrc_container(uint32_t pdcp_sn, const byte_buffer& dl_dcch_msg);

/// \brief Remove PDCP header from DL-DCCH message.
byte_buffer extract_dl_dcch_msg(const byte_buffer& rrc_container);

/// \brief Generate F1AP paging message.
f1ap_message generate_f1ap_paging_message(uint64_t tmsi48);

/// \brief Generates dummy F1AP WRITE-REPLACE WARNING REQUEST message, sent by the CU to the DU, as per
/// TS 38.473 section 8.9.1.1.
f1ap_message generate_f1ap_write_replace_warning_request(unsigned transaction_id);

/// \brief Generates dummy F1AP WRITE-REPLACE WARNING RESPONSE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.9.1.2.
f1ap_message generate_f1ap_write_replace_warning_response(unsigned                                transaction_id,
                                                          const std::vector<nr_cell_global_id_t>& completed_cells = {});

/// \brief Generates dummy F1AP TRP INFORMATION REQUEST message, sent by the CU to the DU, as per
/// TS 38.473 section 8.13.8.2.
f1ap_message generate_trp_information_request();

/// \brief Generates dummy F1AP TRP INFORMATION RESPONSE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.13.8.2.
f1ap_message generate_trp_information_response(const std::vector<trp_id_t>& trp_ids);

/// \brief Generates dummy F1AP TRP INFORMATION FAILURE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.13.8.3.
f1ap_message generate_trp_information_failure();

/// \brief Generates dummy F1AP POSITIONING INFORMATION REQUEST message, sent by the CU to the DU, as per
/// TS 38.473 section 8.13.9.2.
f1ap_message generate_positioning_information_request(gnb_du_ue_f1ap_id_t du_ue_id, gnb_cu_ue_f1ap_id_t cu_ue_id);

/// \brief Generates dummy F1AP POSITIONING INFORMATION RESPONSE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.13.9.2.
f1ap_message generate_positioning_information_response(gnb_du_ue_f1ap_id_t du_ue_id, gnb_cu_ue_f1ap_id_t cu_ue_id);

/// \brief Generates dummy F1AP POSITIONING INFORMATION FAILURE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.13.9.3.
f1ap_message generate_positioning_information_failure(gnb_du_ue_f1ap_id_t du_ue_id, gnb_cu_ue_f1ap_id_t cu_ue_id);

/// \brief Generates an F1AP E-CID MEASUREMENT INITIATION RESPONSE, as per TS 38.473, Section 9.2.12.21.
/// \param[in] azimuth_aoa Azimuth Angle of Arrival carried in the E-CID Measurement Result IE, in units of 0.1
/// degrees. When empty, the message carries no E-CID Measurement Result IE.
f1ap_message generate_e_cid_measurement_initiation_response(gnb_du_ue_f1ap_id_t     du_ue_id,
                                                            gnb_cu_ue_f1ap_id_t     cu_ue_id,
                                                            uint16_t                lmf_ue_meas_id,
                                                            uint16_t                ran_ue_meas_id,
                                                            std::optional<uint16_t> azimuth_aoa,
                                                            std::optional<uint16_t> zenith_aoa = std::nullopt);

/// \brief Generates an F1AP E-CID MEASUREMENT INITIATION FAILURE, as per TS 38.473, Section 9.2.12.22.
f1ap_message generate_e_cid_measurement_initiation_failure(gnb_du_ue_f1ap_id_t du_ue_id,
                                                           gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                           uint16_t            lmf_ue_meas_id,
                                                           uint16_t            ran_ue_meas_id);

/// \brief Generates dummy F1AP POSITIONING ACTIVATION RESPONSE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.13.10.2.
f1ap_message generate_positioning_activation_response(gnb_du_ue_f1ap_id_t du_ue_id, gnb_cu_ue_f1ap_id_t cu_ue_id);

/// \brief Generates dummy F1AP POSITIONING ACTIVATION FAILURE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.13.10.3.
f1ap_message generate_positioning_activation_failure(gnb_du_ue_f1ap_id_t du_ue_id, gnb_cu_ue_f1ap_id_t cu_ue_id);

/// \brief Generates dummy F1AP E-CID MEASUREMENT INITIATION REQUEST message, sent by the CU to the DU, as per
/// TS 38.473 section 8.13.12.
f1ap_message generate_e_cid_measurement_initiation_request(
    gnb_cu_ue_f1ap_id_t                                                        cu_ue_id,
    gnb_du_ue_f1ap_id_t                                                        du_ue_id,
    const std::vector<asn1::f1ap::e_c_id_meas_quantities_value_opts::options>& quantities =
        {asn1::f1ap::e_c_id_meas_quantities_value_opts::options::angle_of_arrival_nr},
    asn1::f1ap::e_c_id_report_characteristics_opts::options report_characteristics =
        asn1::f1ap::e_c_id_report_characteristics_opts::options::on_demand,
    uint16_t                                                              lmf_ue_meas_id          = 1,
    uint16_t                                                              ran_ue_meas_id          = 2,
    std::optional<asn1::f1ap::pos_meas_periodicity_nr_ao_a_opts::options> meas_periodicity_nr_aoa = std::nullopt,
    std::optional<asn1::f1ap::meas_periodicity_opts::options>             e_cid_meas_periodicity  = std::nullopt);

/// \brief Generates dummy F1AP E-CID MEASUREMENT TERMINATION COMMAND message, sent by the CU to the DU, as per
/// TS 38.473 section 8.13.15.
f1ap_message generate_e_cid_measurement_termination_command(gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                            gnb_du_ue_f1ap_id_t du_ue_id,
                                                            uint16_t            lmf_ue_meas_id = 1,
                                                            uint16_t            ran_ue_meas_id = 2);

/// \brief Generates dummy F1AP POSITIONING MEASUREMENT REQUEST message, sent by the CU to the DU, as per
/// TS 38.473 section 8.13.3.2.
f1ap_message
generate_positioning_measurement_request(const std::vector<trp_id_t>& trp_ids,
                                         lmf_meas_id_t                lmf_meas_id,
                                         ran_meas_id_t                ran_meas_id,
                                         const std::vector<asn1::f1ap::pos_meas_type_opts::options>&
                                             pos_meas_type_list = {asn1::f1ap::pos_meas_type_opts::options::ul_rtoa},
                                         subcarrier_spacing scs = subcarrier_spacing::kHz15,
                                         unsigned           srs_offset = 0U,
                                         /// Response Time in units of ten milliseconds. Empty leaves the IE out.
                                         std::optional<uint8_t> resp_time_ten_msec = std::nullopt);

/// \brief Generates dummy F1AP POSITIONING MEASUREMENT RESPONSE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.13.3.2.
f1ap_message generate_positioning_measurement_response(lmf_meas_id_t                lmf_meas_id,
                                                       ran_meas_id_t                ran_meas_id,
                                                       const std::vector<trp_id_t>& trp_ids,
                                                       unsigned                     transaction_id = 1);

/// \brief Generates dummy F1AP POSITIONING MEASUREMENT RESPONSE message containing a UL Angle of Arrival (AoA)
/// measurement result for each TRP, sent by the DU to the CU, as per TS 38.473 section 8.13.3.2 and section
/// 9.3.1.167 (UL Angle of Arrival).
f1ap_message generate_positioning_measurement_response_with_aoa(lmf_meas_id_t                lmf_meas_id,
                                                                ran_meas_id_t                ran_meas_id,
                                                                const std::vector<trp_id_t>& trp_ids,
                                                                uint16_t                     azimuth_ao_a,
                                                                unsigned                     transaction_id = 1);

/// \brief Generates dummy F1AP POSITIONING MEASUREMENT RESPONSE message containing a Zenith Angle of Arrival
/// Information measurement result for each TRP, sent by the DU to the CU, as per TS 38.473 section 8.13.3.2 and
/// section 9.3.1.239 (Zenith Angle of Arrival Information).
f1ap_message
generate_positioning_measurement_response_with_zoa(lmf_meas_id_t                           lmf_meas_id,
                                                   ran_meas_id_t                           ran_meas_id,
                                                   const std::vector<trp_id_t>&            trp_ids,
                                                   uint16_t                                zenith_ao_a,
                                                   std::optional<lcs_to_gcs_translation_t> lcs_to_gcs_translation,
                                                   unsigned                                transaction_id = 1);

/// \brief Generates dummy F1AP POSITIONING MEASUREMENT FAILURE message, sent by the DU to the CU, as per
/// TS 38.473 section 8.13.3.3.
f1ap_message generate_positioning_measurement_failure(lmf_meas_id_t lmf_meas_id, ran_meas_id_t ran_meas_id);

} // namespace test_helpers
} // namespace ocudu
