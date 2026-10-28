// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "pdu_session_manager_test.h"
#include "cu_up_test_helpers.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocuup;

/// PDU session handling tests (creation/deletion)
TEST_F(pdu_session_manager_test, when_valid_pdu_session_setup_item_session_can_be_added)
{
  // no sessions added yet
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // prepare request
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  // attempt to add session
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  // check successful outcome
  ASSERT_TRUE(setup_result.success);
  ASSERT_EQ(setup_result.gtp_tunnel.gtp_teid.value(), 1);

  const std::string tp_address_expect = "127.0.0.2"; // address of dummy gateway
  ASSERT_EQ(setup_result.gtp_tunnel.tp_address.to_string(), tp_address_expect);
  ASSERT_EQ(setup_result.drb_setup_results[0].gtp_tunnel.gtp_teid.value(), 1);
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);

  // attempt to remove non-existing session
  pdu_session_mng->remove_pdu_session(uint_to_pdu_session_id(2));

  // check successful outcome (unchanged)
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);

  // attempt to remove existing session
  pdu_session_mng->remove_pdu_session(uint_to_pdu_session_id(1));

  // check successful outcome (unchanged)
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);
}

TEST_F(pdu_session_manager_test, when_dl_data_forwarding_is_requested_then_tunnel_endpoints_are_reported)
{
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  direct_forwarding_path = true;

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  // Request DL data forwarding at both the PDU session and the DRB level.
  e1ap_data_forwarding_info_request forwarding_request;
  forwarding_request.data_forwarding_request                      = e1ap_data_forwarding_request::dl;
  pdu_session_setup_item.pdu_session_data_forwarding_info_request = forwarding_request;
  pdu_session_setup_item.drb_to_setup_list_ng_ran[drb_id].drb_data_forwarding_info_request = forwarding_request;

  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  ASSERT_TRUE(setup_result.success);

  // Both levels got their own DL forwarding endpoint, and neither reports an UL endpoint.
  ASSERT_TRUE(setup_result.data_forwarding_info.has_value());
  ASSERT_TRUE(setup_result.data_forwarding_info->dl_data_forwarding.has_value());
  ASSERT_FALSE(setup_result.data_forwarding_info->ul_data_forwarding.has_value());

  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);
  ASSERT_TRUE(setup_result.drb_setup_results[0].data_forwarding_info.has_value());
  ASSERT_TRUE(setup_result.drb_setup_results[0].data_forwarding_info->dl_data_forwarding.has_value());
  ASSERT_FALSE(setup_result.drb_setup_results[0].data_forwarding_info->ul_data_forwarding.has_value());

  // The source forwards over a direct path, so both endpoints are on the Xn-U bind address, which only the source
  // reaches, and both TEIDs come from the Xn-U pool.
  const up_transport_layer_info session_fwd = setup_result.data_forwarding_info->dl_data_forwarding.value();
  const up_transport_layer_info drb_fwd =
      setup_result.drb_setup_results[0].data_forwarding_info->dl_data_forwarding.value();
  // One Xn-U socket serves the whole PDU session, so both endpoints share its address and differ from NG-U.
  ASSERT_EQ(session_fwd.tp_address, transport_layer_address::create_from_string("127.0.50.1"));
  ASSERT_EQ(drb_fwd.tp_address, session_fwd.tp_address);
  ASSERT_NE(session_fwd.tp_address, setup_result.gtp_tunnel.tp_address);
  ASSERT_NE(session_fwd.gtp_teid, drb_fwd.gtp_teid);

  // Removing the session releases both forwarding TEIDs back to the Xn-U pool.
  ASSERT_FALSE(xnu_allocator->was_teid_released(session_fwd.gtp_teid));
  ASSERT_FALSE(xnu_allocator->was_teid_released(drb_fwd.gtp_teid));

  pdu_session_mng->remove_pdu_session(psi);
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  ASSERT_TRUE(xnu_allocator->was_teid_released(session_fwd.gtp_teid));
  ASSERT_TRUE(xnu_allocator->was_teid_released(drb_fwd.gtp_teid));
}

// Without a direct path a UPF relays the forwarded data, so the endpoints must be on NG-U, where the UPF reaches this
// node (TS 37.483 section 8.3.1.2).
TEST_F(pdu_session_manager_test, when_no_direct_forwarding_path_is_signalled_then_the_endpoints_are_on_ngu)
{
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  e1ap_data_forwarding_info_request forwarding_request;
  forwarding_request.data_forwarding_request                      = e1ap_data_forwarding_request::dl;
  pdu_session_setup_item.pdu_session_data_forwarding_info_request = forwarding_request;
  pdu_session_setup_item.drb_to_setup_list_ng_ran[drb_id].drb_data_forwarding_info_request = forwarding_request;

  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  ASSERT_TRUE(setup_result.success);
  ASSERT_TRUE(setup_result.data_forwarding_info.has_value());
  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);
  ASSERT_TRUE(setup_result.drb_setup_results[0].data_forwarding_info.has_value());

  const up_transport_layer_info session_fwd = setup_result.data_forwarding_info->dl_data_forwarding.value();
  const up_transport_layer_info drb_fwd =
      setup_result.drb_setup_results[0].data_forwarding_info->dl_data_forwarding.value();
  ASSERT_EQ(session_fwd.tp_address, setup_result.gtp_tunnel.tp_address);
  ASSERT_EQ(drb_fwd.tp_address, setup_result.gtp_tunnel.tp_address);

  pdu_session_mng->remove_pdu_session(psi);
  ASSERT_TRUE(ngu_allocator->was_teid_released(session_fwd.gtp_teid));
  ASSERT_TRUE(ngu_allocator->was_teid_released(drb_fwd.gtp_teid));
}

// A direct path needs an Xn-U socket. Without one no endpoint can be offered, since an NG-U endpoint would not be
// reachable by the source.
TEST_F(pdu_session_manager_test, when_no_xnu_socket_is_configured_then_no_tunnel_is_reported)
{
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  // Rebuild the manager without an Xn-U socket, although the source forwards over a direct path.
  direct_forwarding_path = true;
  with_xnu_socket        = false;
  init();

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  e1ap_data_forwarding_info_request forwarding_request;
  forwarding_request.data_forwarding_request                      = e1ap_data_forwarding_request::dl;
  pdu_session_setup_item.pdu_session_data_forwarding_info_request = forwarding_request;
  pdu_session_setup_item.drb_to_setup_list_ng_ran[drb_id].drb_data_forwarding_info_request = forwarding_request;

  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  ASSERT_TRUE(setup_result.success);

  // Neither level can offer an endpoint.
  ASSERT_FALSE(setup_result.data_forwarding_info.has_value());
  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);
  ASSERT_FALSE(setup_result.drb_setup_results[0].data_forwarding_info.has_value());
}

TEST_F(pdu_session_manager_test, when_dl_data_forwarding_is_not_requested_then_no_tunnel_endpoints_are_reported)
{
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  ASSERT_TRUE(setup_result.success);
  ASSERT_FALSE(setup_result.data_forwarding_info.has_value());
  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);
  ASSERT_FALSE(setup_result.drb_setup_results[0].data_forwarding_info.has_value());
}

TEST_F(pdu_session_manager_test, when_pdu_session_with_same_id_is_setup_session_cant_be_added)
{
  // no sessions added yet
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // prepare request
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  // attempt to add session
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  // check successful outcome
  ASSERT_TRUE(setup_result.success);
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);

  // attempt to add the same session again
  setup_result = pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  // check unsuccessful outcome
  ASSERT_FALSE(setup_result.success);
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
}

TEST_F(pdu_session_manager_test, when_unexisting_pdu_session_is_modified_operation_failed)
{
  // no sessions added yet
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // attempt to modify unexisting PDU session
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item;
  pdu_session_modify_item.pdu_session_id = uint_to_pdu_session_id(1);

  pdu_session_modification_result modification_result =
      pdu_session_mng->modify_pdu_session(pdu_session_modify_item, false);

  // check successful outcome
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);
}

/// PDU session handling tests (creation/deletion)
TEST_F(pdu_session_manager_test, drb_create_modify_remove)
{
  // no sessions added yet
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // prepare request
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  // attempt to add session
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  // check successful outcome
  ASSERT_TRUE(setup_result.success);
  ASSERT_EQ(setup_result.pdu_session_id, psi);
  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->drb_id, drb_id);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->qos_flow_results.size(), 1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->qos_flow_results.begin()->qos_flow_id, qfi);

  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
  ASSERT_FALSE(gtpu_rx_demux->created_teid_list.empty());
  gtpu_rx_demux->created_teid_list.pop_front();
  ASSERT_TRUE(gtpu_rx_demux->created_teid_list.empty());

  ASSERT_FALSE(f1u_gw->created_ul_teid_list.empty());
  gtpu_teid_t ul_teid = f1u_gw->created_ul_teid_list.front();
  f1u_gw->created_ul_teid_list.pop_front();
  ASSERT_TRUE(f1u_gw->created_ul_teid_list.empty());

  // prepare modification request (to remove bearers)
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item =
      generate_pdu_session_res_to_modify_item_to_remove_drb(psi, drb_id);

  // Add invalid DRB to remove
  drb_id_t invalid_drb_to_remove = uint_to_drb_id(0x0f);
  pdu_session_modify_item.drb_to_rem_list_ng_ran.push_back(invalid_drb_to_remove);

  // attempt to remove bearers
  pdu_session_modification_result modification_result =
      pdu_session_mng->modify_pdu_session(pdu_session_modify_item, false);

  // check successful outcome
  ASSERT_TRUE(setup_result.success);

  // validate pdu session is not disconnected from GTP-U gateway
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
  ASSERT_TRUE(gtpu_rx_demux->removed_teid_list.empty());

  // validate bearer is disconnected from F1-U gateway
  ASSERT_FALSE(f1u_gw->removed_ul_teid_list.empty());
  ASSERT_EQ(f1u_gw->removed_ul_teid_list.front(), ul_teid);
  f1u_gw->removed_ul_teid_list.pop_front();
  ASSERT_TRUE(f1u_gw->removed_ul_teid_list.empty());
}

/// Create a DRB with only one QFI, but the QFI is already mapped
TEST_F(pdu_session_manager_test, drb_create_with_one_qfi_which_is_already_mapped)
{
  // no sessions added yet
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // prepare request
  pdu_session_id_t psi     = uint_to_pdu_session_id(1);
  drb_id_t         drb_id1 = uint_to_drb_id(1);
  drb_id_t         drb_id2 = uint_to_drb_id(2);
  qos_flow_id_t    qfi     = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id1, qfi, uint_to_five_qi(9));

  // attempt to add session
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  // check successful outcome
  ASSERT_TRUE(setup_result.success);
  ASSERT_EQ(setup_result.pdu_session_id, psi);
  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->drb_id, drb_id1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->qos_flow_results.size(), 1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->qos_flow_results.begin()->qos_flow_id, qfi);

  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
  ASSERT_FALSE(gtpu_rx_demux->created_teid_list.empty());
  gtpu_rx_demux->created_teid_list.pop_front();
  ASSERT_TRUE(gtpu_rx_demux->created_teid_list.empty());

  ASSERT_FALSE(f1u_gw->created_ul_teid_list.empty());
  f1u_gw->created_ul_teid_list.pop_front();
  ASSERT_TRUE(f1u_gw->created_ul_teid_list.empty());

  // prepare modification request adding a new DRB and map it to a QFI that is already mapped
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item =
      generate_pdu_session_res_to_modify_item_to_setup_drb(psi, drb_id2, {qfi}, uint_to_five_qi(9));

  // attempt to perform the modification
  pdu_session_modification_result mod_result = pdu_session_mng->modify_pdu_session(pdu_session_modify_item, false);

  // check the result
  EXPECT_TRUE(mod_result.success);
  ASSERT_EQ(mod_result.drb_setup_results.size(), 1);
  EXPECT_FALSE(mod_result.drb_setup_results[0].success);
  EXPECT_EQ(mod_result.drb_setup_results[0].cause, e1ap_cause_t{e1ap_cause_radio_network_t::unspecified});
  ASSERT_EQ(mod_result.drb_setup_results[0].qos_flow_results.size(), 1);
  EXPECT_FALSE(mod_result.drb_setup_results[0].qos_flow_results[0].success);
  EXPECT_EQ(mod_result.drb_setup_results[0].qos_flow_results[0].cause,
            e1ap_cause_t{e1ap_cause_radio_network_t::multiple_qos_flow_id_instances});

  // validate pdu session is not disconnected from GTP-U gateway
  EXPECT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
  EXPECT_TRUE(gtpu_rx_demux->removed_teid_list.empty());

  // validate the dangling bearer was not created and removed from F1-U gateway
  EXPECT_EQ(f1u_gw->removed_ul_teid_list.size(), 0);
}

/// Create a DRB with only one QFI, but the QFI is already mapped
TEST_F(pdu_session_manager_test, drb_create_with_unknown_five_qi)
{
  // no sessions added yet
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // prepare request
  pdu_session_id_t psi     = uint_to_pdu_session_id(1);
  drb_id_t         drb_id1 = uint_to_drb_id(1);
  qos_flow_id_t    qfi     = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id1, qfi, uint_to_five_qi(8));

  // attempt to add session adding a new DRB and map it to a 5QI that is unknown
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  // check successful outcome
  ASSERT_TRUE(setup_result.success);
  ASSERT_EQ(setup_result.pdu_session_id, psi);
  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);

  EXPECT_FALSE(setup_result.drb_setup_results[0].success);
  EXPECT_EQ(setup_result.drb_setup_results[0].cause, e1ap_cause_t{e1ap_cause_radio_network_t::not_supported_5qi_value});
  ASSERT_EQ(setup_result.drb_setup_results[0].qos_flow_results.size(), 0);
}

/// Create a DRB with two QFIs, of which one QFI is already mapped
TEST_F(pdu_session_manager_test, drb_create_with_two_qfi_of_which_one_is_already_mapped)
{
  // no sessions added yet
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // prepare request
  pdu_session_id_t psi     = uint_to_pdu_session_id(1);
  drb_id_t         drb_id1 = uint_to_drb_id(1);
  drb_id_t         drb_id2 = uint_to_drb_id(2);
  qos_flow_id_t    qfi1    = uint_to_qos_flow_id(8);
  qos_flow_id_t    qfi2    = uint_to_qos_flow_id(9);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id1, qfi1, uint_to_five_qi(9));

  // attempt to add session
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  // check successful outcome
  ASSERT_TRUE(setup_result.success);
  ASSERT_EQ(setup_result.pdu_session_id, psi);
  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->drb_id, drb_id1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->qos_flow_results.size(), 1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->qos_flow_results.begin()->qos_flow_id, qfi1);

  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
  ASSERT_FALSE(gtpu_rx_demux->created_teid_list.empty());
  gtpu_rx_demux->created_teid_list.pop_front();
  ASSERT_TRUE(gtpu_rx_demux->created_teid_list.empty());

  ASSERT_FALSE(f1u_gw->created_ul_teid_list.empty());
  f1u_gw->created_ul_teid_list.pop_front();
  ASSERT_TRUE(f1u_gw->created_ul_teid_list.empty());

  // prepare modification request adding a new DRB and map it to a QFI that is already mapped
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item =
      generate_pdu_session_res_to_modify_item_to_setup_drb(psi, drb_id2, {qfi1, qfi2}, uint_to_five_qi(9));

  // attempt to perform the modification
  pdu_session_modification_result mod_result = pdu_session_mng->modify_pdu_session(pdu_session_modify_item, false);

  // check the result
  EXPECT_TRUE(mod_result.success);
  ASSERT_EQ(mod_result.drb_setup_results.size(), 1);
  EXPECT_TRUE(mod_result.drb_setup_results[0].success); // success, since at least one QFI mapping was valid
  EXPECT_EQ(mod_result.drb_setup_results[0].cause, e1ap_cause_t{e1ap_cause_radio_network_t::unspecified});
  ASSERT_EQ(mod_result.drb_setup_results[0].qos_flow_results.size(), 2);
  EXPECT_FALSE(mod_result.drb_setup_results[0].qos_flow_results[0].success); // the first was invalid
  EXPECT_EQ(mod_result.drb_setup_results[0].qos_flow_results[0].cause,
            e1ap_cause_t{e1ap_cause_radio_network_t::multiple_qos_flow_id_instances});
  EXPECT_TRUE(mod_result.drb_setup_results[0].qos_flow_results[1].success); // the second was valid
  EXPECT_EQ(mod_result.drb_setup_results[0].qos_flow_results[1].cause,
            e1ap_cause_t{e1ap_cause_radio_network_t::unspecified});

  // validate pdu session is not disconnected from GTP-U gateway
  EXPECT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
  EXPECT_TRUE(gtpu_rx_demux->removed_teid_list.empty());

  // validate the dangling bearer was not removed from F1-U gateway
  EXPECT_TRUE(f1u_gw->removed_ul_teid_list.empty());
}

TEST_F(pdu_session_manager_test, dtor_rm_all_sessions_and_bearers)
{
  // no sessions added yet
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // prepare request
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  // attempt to add session
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);

  // check successful outcome
  ASSERT_TRUE(setup_result.success);
  ASSERT_EQ(setup_result.pdu_session_id, psi);
  ASSERT_EQ(setup_result.drb_setup_results.size(), 1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->drb_id, drb_id);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->qos_flow_results.size(), 1);
  ASSERT_EQ(setup_result.drb_setup_results.begin()->qos_flow_results.begin()->qos_flow_id, qfi);

  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
  ASSERT_FALSE(gtpu_rx_demux->created_teid_list.empty());
  gtpu_teid_t teid = gtpu_rx_demux->created_teid_list.front();
  gtpu_rx_demux->created_teid_list.pop_front();
  ASSERT_TRUE(gtpu_rx_demux->created_teid_list.empty());

  ASSERT_FALSE(f1u_gw->created_ul_teid_list.empty());
  gtpu_teid_t ul_teid = f1u_gw->created_ul_teid_list.front();
  f1u_gw->created_ul_teid_list.pop_front();
  ASSERT_TRUE(f1u_gw->created_ul_teid_list.empty());

  // delete pdu_session_mng, all remaining sessions and bearers shall be removed and detached from all gateways
  pdu_session_mng.reset();

  // validate pdu session is disconnected from GTP-U gateway
  ASSERT_FALSE(gtpu_rx_demux->removed_teid_list.empty());
  ASSERT_EQ(gtpu_rx_demux->removed_teid_list.front(), teid);
  gtpu_rx_demux->removed_teid_list.pop_front();
  ASSERT_TRUE(gtpu_rx_demux->removed_teid_list.empty());

  // validate bearer is disconnected from F1-U gateway
  ASSERT_FALSE(f1u_gw->removed_ul_teid_list.empty());
  ASSERT_EQ(f1u_gw->removed_ul_teid_list.front(), ul_teid);
  f1u_gw->removed_ul_teid_list.pop_front();
  ASSERT_TRUE(f1u_gw->removed_ul_teid_list.empty());
}

/// PDU session handling tests (creation/deletion)
TEST_F(pdu_session_manager_test, when_new_ul_info_is_requested_f1u_is_disconnected)
{
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  // prepare request
  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));

  pdu_session_setup_result set_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
  drb_setup_result drb_setup_res = set_result.drb_setup_results[0];
  ASSERT_EQ(drb_setup_res.gtp_tunnel.gtp_teid, 0x1);

  // prepare modification request (request new UL TNL info)
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item =
      generate_pdu_session_res_to_modify_item_to_modify_drb(psi, drb_id);

  pdu_session_modification_result mod_result  = pdu_session_mng->modify_pdu_session(pdu_session_modify_item, true);
  drb_modified_result             drb_mod_res = mod_result.drb_modification_results[0];
  ASSERT_EQ(drb_mod_res.gtp_tunnel.gtp_teid, 0x2);

  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
}

/// NG-U (N3) UL tunnel update via ng_ul_up_tnl_info in a Bearer Context Modification (Xn path switch).
TEST_F(pdu_session_manager_test, when_ng_ul_up_tnl_info_is_set_in_modify_item_then_tunnel_update_succeeds)
{
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  // Set up the PDU session first.
  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);
  ASSERT_TRUE(setup_result.success);
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);

  // Modify with a new NG-U (N3) UL tunnel endpoint (simulates AMF providing new UPF address after Xn HO).
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item =
      generate_pdu_session_res_to_modify_item_with_ng_ul_up_tnl_info(psi, "10.0.0.1", 0xabcd1234);
  pdu_session_modification_result mod_result = pdu_session_mng->modify_pdu_session(pdu_session_modify_item, false);

  // The session should still exist and the modification should succeed.
  EXPECT_TRUE(mod_result.success);
  EXPECT_EQ(mod_result.pdu_session_id, psi);
  EXPECT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
}

/// Modifying a PDU session without ng_ul_up_tnl_info must succeed without touching the NG-U (N3) tunnel
/// (negative: absence of ng_ul_up_tnl_info must not cause failure or unintended side effects).
TEST_F(pdu_session_manager_test, when_ng_ul_up_tnl_info_absent_in_modify_item_then_no_tunnel_change)
{
  ASSERT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);

  pdu_session_id_t psi    = uint_to_pdu_session_id(1);
  drb_id_t         drb_id = uint_to_drb_id(1);
  qos_flow_id_t    qfi    = uint_to_qos_flow_id(8);

  e1ap_pdu_session_res_to_setup_item pdu_session_setup_item =
      generate_pdu_session_res_to_setup_item(psi, drb_id, qfi, uint_to_five_qi(9));
  pdu_session_setup_result setup_result =
      pdu_session_mng->setup_pdu_session(pdu_session_setup_item, direct_forwarding_path);
  ASSERT_TRUE(setup_result.success);

  // Modification without ng_ul_up_tnl_info — must succeed and leave the session intact.
  e1ap_pdu_session_res_to_modify_item pdu_session_modify_item;
  pdu_session_modify_item.pdu_session_id = psi;
  ASSERT_FALSE(pdu_session_modify_item.ng_ul_up_tnl_info.has_value());

  pdu_session_modification_result mod_result = pdu_session_mng->modify_pdu_session(pdu_session_modify_item, false);

  EXPECT_TRUE(mod_result.success);
  EXPECT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 1);
}

/// NG-U socket selection by S-NSSAI.

TEST_F(pdu_session_manager_slice_socket_test, when_slice_has_dedicated_sockets_then_sessions_take_turns_on_them)
{
  const s_nssai_t                  slice{slice_service_type{1}, slice_differentiator::create(1).value()};
  const std::array<std::string, 3> expected_addrs = {"127.0.1.1", "127.0.1.2", "127.0.1.1"};

  for (unsigned i = 0; i != expected_addrs.size(); ++i) {
    e1ap_pdu_session_res_to_setup_item item = generate_pdu_session_res_to_setup_item(
        uint_to_pdu_session_id(i + 1), uint_to_drb_id(i + 1), uint_to_qos_flow_id(8), uint_to_five_qi(9));
    item.snssai = slice;

    pdu_session_setup_result result = pdu_session_mng->setup_pdu_session(item, direct_forwarding_path);

    ASSERT_TRUE(result.success);
    EXPECT_EQ(result.gtp_tunnel.tp_address.to_string(), expected_addrs[i]);
  }
  EXPECT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 3);
}

TEST_F(pdu_session_manager_slice_socket_test, when_slice_without_sd_has_a_dedicated_socket_then_session_uses_it)
{
  e1ap_pdu_session_res_to_setup_item item = generate_pdu_session_res_to_setup_item(
      uint_to_pdu_session_id(1), uint_to_drb_id(1), uint_to_qos_flow_id(8), uint_to_five_qi(9));
  item.snssai = s_nssai_t{slice_service_type{2}};

  pdu_session_setup_result result = pdu_session_mng->setup_pdu_session(item, direct_forwarding_path);

  ASSERT_TRUE(result.success);
  EXPECT_EQ(result.gtp_tunnel.tp_address.to_string(), "127.0.1.3");
}

TEST_F(pdu_session_manager_slice_socket_test, when_no_socket_serves_the_slice_then_session_setup_fails)
{
  e1ap_pdu_session_res_to_setup_item item = generate_pdu_session_res_to_setup_item(
      uint_to_pdu_session_id(1), uint_to_drb_id(1), uint_to_qos_flow_id(8), uint_to_five_qi(9));
  item.snssai = s_nssai_t{slice_service_type{3}};

  pdu_session_setup_result result = pdu_session_mng->setup_pdu_session(item, direct_forwarding_path);

  EXPECT_FALSE(result.success);
  EXPECT_EQ(result.cause, e1ap_cause_t{e1ap_cause_radio_network_t::res_not_available_for_the_slice});
  EXPECT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);
}

TEST_F(pdu_session_manager_slice_socket_test, when_only_the_sst_matches_a_dedicated_socket_then_session_setup_fails)
{
  // SST 1 without SD is a different S-NSSAI than SST 1 with SD 0x1.
  e1ap_pdu_session_res_to_setup_item item = generate_pdu_session_res_to_setup_item(
      uint_to_pdu_session_id(1), uint_to_drb_id(1), uint_to_qos_flow_id(8), uint_to_five_qi(9));
  item.snssai = s_nssai_t{slice_service_type{1}};

  pdu_session_setup_result result = pdu_session_mng->setup_pdu_session(item, direct_forwarding_path);

  EXPECT_FALSE(result.success);
  EXPECT_EQ(pdu_session_mng->get_nof_pdu_sessions(), 0);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
