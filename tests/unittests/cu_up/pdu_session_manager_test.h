// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "cu_up_test_helpers.h"
#include "lib/cu_up/ngu_session_manager_impl.h"
#include "lib/cu_up/pdu_session_manager_impl.h"
#include "ocudu/support/executors/manual_task_worker.h"
#include <gtest/gtest.h>

namespace ocudu {
namespace ocuup {

/// Fixture base class for PDU session manager tests
class pdu_session_manager_test_base
{
protected:
  ~pdu_session_manager_test_base() = default;

  void init()
  {
    ocudulog::fetch_basic_logger("TEST").set_level(ocudulog::basic_levels::debug);
    ocudulog::init();

    // create required objects
    security_info      = get_dummy_up_security_info();
    gtpu_rx_demux      = std::make_unique<dummy_gtpu_demux_ctrl>();
    gtpu_tx_notifier   = std::make_unique<dummy_gtpu_network_gateway_adapter>();
    f1u_gw             = std::make_unique<dummy_f1u_gateway>(f1u_bearer);
    ngu_allocator      = std::make_unique<dummy_gtpu_teid_pool>();
    xnu_allocator      = std::make_unique<dummy_gtpu_teid_pool>();
    f1u_allocator      = std::make_unique<dummy_gtpu_teid_pool>();
    xnu_session_mngr   = with_xnu_socket ? std::make_unique<dummy_xnu_session_manager>() : nullptr;
    cu_up_mngr_pdcp_if = std::make_unique<dummy_cu_up_manager_pdcp_interface>();
    e1ap               = std::make_unique<dummy_e1ap>(cu_up_e1_index_t{0});

    if (ngu_gw_snssais.empty()) {
      ngu_session_mngr = std::make_unique<dummy_ngu_session_manager>();
    } else {
      // One gateway per S-NSSAI entry, bound to 127.0.1.1, 127.0.1.2, ... in order.
      for (unsigned i = 0, e = ngu_gw_snssais.size(); i != e; ++i) {
        auto gw = std::make_unique<dummy_gtpu_gateway>();
        gw->set_bind_address(fmt::format("127.0.1.{}", i + 1));
        ngu_gws.push_back(std::move(gw));
      }
      ngu_session_mngr = std::make_unique<ngu_session_manager_impl>(ngu_gws, ngu_gw_snssais);
    }

    // create DUT object
    ue_inactivity_timer = timers_factory.create_timer();
    ue_inactivity_timer.set(std::chrono::milliseconds(10000), []() {});
    std::map<five_qi_t, ocuup::cu_up_qos_config> qos;
    qos[uint_to_five_qi(9)] = {};

    manual_task_worker teid_worker{128};

    const uint64_t ue_dl_ambr = 1000000000;
    pdu_session_mng =
        std::make_unique<pdu_session_manager_impl>(MIN_CU_UP_UE_INDEX,
                                                   qos,
                                                   security_info,
                                                   ngu_config,
                                                   cu_up_test_mode_config{},
                                                   ue_dl_ambr,
                                                   pdu_session_manager_dependencies{logger,
                                                                                    ue_inactivity_timer,
                                                                                    timers_factory,
                                                                                    timers_factory,
                                                                                    *e1ap,
                                                                                    *f1u_gw,
                                                                                    *ngu_session_mngr,
                                                                                    *cu_up_mngr_pdcp_if,
                                                                                    *ngu_allocator,
                                                                                    *xnu_allocator,
                                                                                    xnu_session_mngr.get(),
                                                                                    *f1u_allocator,
                                                                                    *gtpu_rx_demux,
                                                                                    teid_worker,
                                                                                    teid_worker,
                                                                                    teid_worker,
                                                                                    teid_worker,
                                                                                    gtpu_pcap});
  }

  void finish()
  {
    // flush logger after each test
    ocudulog::flush();
  }

  /// S-NSSAI of each NG-U gateway. When not empty, the fixture uses the real NG-U session manager.
  std::vector<std::optional<s_nssai_t>> ngu_gw_snssais;
  /// NG-U gateways of the real NG-U session manager.
  std::vector<std::unique_ptr<gtpu_tnl_pdu_session>> ngu_gws;

  timer_manager                                               timers_manager;
  manual_task_worker                                          worker{64};
  timer_factory                                               timers_factory{timers_manager, worker};
  unique_timer                                                ue_inactivity_timer;
  std::unique_ptr<dummy_gtpu_demux_ctrl>                      gtpu_rx_demux;
  std::unique_ptr<gtpu_tunnel_common_tx_upper_layer_notifier> gtpu_tx_notifier;
  dummy_inner_f1u_bearer                                      f1u_bearer;
  std::unique_ptr<dummy_f1u_gateway>                          f1u_gw;
  std::unique_ptr<ngu_session_manager>                        ngu_session_mngr;
  std::unique_ptr<cu_up_manager_pdcp_interface>               cu_up_mngr_pdcp_if;
  std::unique_ptr<e1ap_interface>                             e1ap;
  std::unique_ptr<dummy_gtpu_teid_pool>                       ngu_allocator;
  std::unique_ptr<dummy_gtpu_teid_pool>                       xnu_allocator;
  std::unique_ptr<dummy_xnu_session_manager>                  xnu_session_mngr;
  bool                                                        with_xnu_socket        = true;
  bool                                                        direct_forwarding_path = false;
  std::unique_ptr<dummy_gtpu_teid_pool>                       f1u_allocator;
  std::unique_ptr<pdu_session_manager_ctrl>                   pdu_session_mng;
  null_dlt_pcap                                               gtpu_pcap;
  security::sec_as_config                                     security_info;
  ngu_interface_config                                        ngu_config = {};
  cu_up_ue_logger                                             logger{"CU-UP", {MIN_CU_UP_UE_INDEX}};
};

/// Fixture class for PDU session manager tests with default network interface config.
class pdu_session_manager_test : public pdu_session_manager_test_base, public ::testing::Test
{
protected:
  void SetUp() override { init(); }
  void TearDown() override { finish(); }
};

/// Fixture class for PDU session manager tests with NG-U sockets dedicated to slices.
class pdu_session_manager_slice_socket_test : public pdu_session_manager_test_base, public ::testing::Test
{
protected:
  void SetUp() override
  {
    // 127.0.1.1 and 127.0.1.2 serve SST 1 with SD 0x1, 127.0.1.3 serves SST 2 without SD.
    ngu_gw_snssais = {s_nssai_t{slice_service_type{1}, slice_differentiator::create(1).value()},
                      s_nssai_t{slice_service_type{1}, slice_differentiator::create(1).value()},
                      s_nssai_t{slice_service_type{2}}};
    init();
  }
  void TearDown() override { finish(); }
};

inline e1ap_pdu_session_res_to_setup_item
generate_pdu_session_res_to_setup_item(pdu_session_id_t psi, drb_id_t drb_id, qos_flow_id_t qfi, five_qi_t five_qi)
{
  // Prepare request.
  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item;
  pdu_session_setup_item.pdu_session_id                        = psi;
  pdu_session_setup_item.pdu_session_type                      = pdu_session_type_t::ipv4;
  pdu_session_setup_item.snssai.sst                            = slice_service_type{1};
  pdu_session_setup_item.snssai.sd                             = slice_differentiator::create(10203).value();
  pdu_session_setup_item.security_ind.integrity_protection_ind = integrity_protection_indication_t::not_needed;
  pdu_session_setup_item.security_ind.confidentiality_protection_ind =
      confidentiality_protection_indication_t::not_needed;
  pdu_session_setup_item.pdu_session_res_dl_ambr      = 330000000;
  pdu_session_setup_item.ng_ul_up_tnl_info.tp_address = transport_layer_address::create_from_string("127.0.0.1");
  pdu_session_setup_item.ng_ul_up_tnl_info.gtp_teid   = int_to_gtpu_teid(0x12345678);
  pdu_session_setup_item.ng_ul_up_tnl_info            = {transport_layer_address::create_from_string("0.0.0.0"),
                                                         int_to_gtpu_teid(0)};

  e1ap_drb_to_setup_item_ng_ran drb_to_setup_item;
  drb_to_setup_item.drb_id                      = drb_id;
  drb_to_setup_item.sdap_cfg.default_drb        = true;
  drb_to_setup_item.sdap_cfg.sdap_hdr_ul        = sdap_hdr_ul_cfg::present;
  drb_to_setup_item.sdap_cfg.sdap_hdr_dl        = sdap_hdr_dl_cfg::present;
  drb_to_setup_item.pdcp_cfg.pdcp_sn_size_ul    = pdcp_sn_size::size18bits;
  drb_to_setup_item.pdcp_cfg.pdcp_sn_size_dl    = pdcp_sn_size::size18bits;
  drb_to_setup_item.pdcp_cfg.rlc_mod            = pdcp_rlc_mode::am;
  drb_to_setup_item.pdcp_cfg.t_reordering_timer = pdcp_t_reordering::ms100;
  drb_to_setup_item.pdcp_cfg.discard_timer      = pdcp_discard_timer::infinity;

  e1ap_cell_group_info_item cell_group_info_item;
  cell_group_info_item.cell_group_id = 0;
  drb_to_setup_item.cell_group_info.push_back(cell_group_info_item);

  e1ap_qos_flow_qos_param_item qos_flow_info{};
  qos_flow_info.qos_flow_id = qfi;
  non_dyn_5qi_descriptor non_dyn_5qi;
  non_dyn_5qi.five_qi                                                           = five_qi;
  qos_flow_info.qos_flow_level_qos_params.qos_desc                              = non_dyn_5qi;
  qos_flow_info.qos_flow_level_qos_params.ng_ran_alloc_retention.prio_level_arp = 1;
  drb_to_setup_item.qos_flow_info_to_be_setup.emplace(qos_flow_info.qos_flow_id, qos_flow_info);

  pdu_session_setup_item.drb_to_setup_list_ng_ran.emplace(drb_to_setup_item.drb_id, drb_to_setup_item);

  return pdu_session_setup_item;
}

inline e1ap_pdu_session_res_to_modify_item generate_pdu_session_res_to_modify_item_to_remove_drb(pdu_session_id_t psi,
                                                                                                 drb_id_t drb_id)
{
  // Prepare modification request (to remove bearers).
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item;
  pdu_session_modify_item.pdu_session_id = psi;

  pdu_session_modify_item.drb_to_rem_list_ng_ran.push_back(drb_id);

  return pdu_session_modify_item;
}

inline e1ap_pdu_session_res_to_modify_item generate_pdu_session_res_to_modify_item_to_modify_drb(pdu_session_id_t psi,
                                                                                                 drb_id_t drb_id)
{
  // Prepare modification request (to remove bearers).
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item;
  pdu_session_modify_item.pdu_session_id = psi;

  e1ap_drb_to_modify_item_ng_ran drb_to_mod = {};
  drb_to_mod.drb_id                         = drb_id;
  pdu_session_modify_item.drb_to_modify_list_ng_ran.insert(drb_to_mod.drb_id, drb_to_mod);
  return pdu_session_modify_item;
}

inline e1ap_pdu_session_res_to_modify_item
generate_pdu_session_res_to_modify_item_to_setup_drb(pdu_session_id_t           psi,
                                                     drb_id_t                   drb_id,
                                                     std::vector<qos_flow_id_t> qfi_list,
                                                     five_qi_t                  five_qi)
{
  // Prepare modification request (to add further bearers).
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item;
  pdu_session_modify_item.pdu_session_id = psi;

  e1ap_drb_to_setup_item_ng_ran drb_to_setup_item;
  drb_to_setup_item.drb_id                      = drb_id;
  drb_to_setup_item.sdap_cfg.default_drb        = true;
  drb_to_setup_item.sdap_cfg.sdap_hdr_ul        = sdap_hdr_ul_cfg::present;
  drb_to_setup_item.sdap_cfg.sdap_hdr_dl        = sdap_hdr_dl_cfg::present;
  drb_to_setup_item.pdcp_cfg.pdcp_sn_size_ul    = pdcp_sn_size::size18bits;
  drb_to_setup_item.pdcp_cfg.pdcp_sn_size_dl    = pdcp_sn_size::size18bits;
  drb_to_setup_item.pdcp_cfg.rlc_mod            = pdcp_rlc_mode::am;
  drb_to_setup_item.pdcp_cfg.t_reordering_timer = pdcp_t_reordering::ms100;
  drb_to_setup_item.pdcp_cfg.discard_timer      = pdcp_discard_timer::infinity;

  e1ap_cell_group_info_item cell_group_info_item;
  cell_group_info_item.cell_group_id = 0;
  drb_to_setup_item.cell_group_info.push_back(cell_group_info_item);

  for (const auto& qfi : qfi_list) {
    e1ap_qos_flow_qos_param_item qos_flow_info{};
    qos_flow_info.qos_flow_id = qfi;
    non_dyn_5qi_descriptor non_dyn_5qi{};
    non_dyn_5qi.five_qi                                                           = five_qi;
    qos_flow_info.qos_flow_level_qos_params.qos_desc                              = non_dyn_5qi;
    qos_flow_info.qos_flow_level_qos_params.ng_ran_alloc_retention.prio_level_arp = 1;
    drb_to_setup_item.qos_flow_info_to_be_setup.emplace(qos_flow_info.qos_flow_id, qos_flow_info);
  }

  pdu_session_modify_item.drb_to_setup_list_ng_ran.emplace(drb_to_setup_item.drb_id, drb_to_setup_item);

  return pdu_session_modify_item;
}

inline e1ap_pdu_session_res_to_modify_item
generate_pdu_session_res_to_modify_item_with_ng_ul_up_tnl_info(pdu_session_id_t   psi,
                                                               const std::string& peer_addr,
                                                               uint32_t           peer_teid)
{
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item;
  pdu_session_modify_item.pdu_session_id = psi;
  pdu_session_modify_item.ng_ul_up_tnl_info =
      up_transport_layer_info(transport_layer_address::create_from_string(peer_addr), int_to_gtpu_teid(peer_teid));
  return pdu_session_modify_item;
}

} // namespace ocuup
} // namespace ocudu
