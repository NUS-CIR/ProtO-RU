// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_cp_test_environment.h"
#include "lib/xnap/procedures/ngran_node_cfg_update_asn1_helpers.h"
#include "lib/xnap/procedures/xn_setup_procedure_asn1_helpers.h"
#include "tests/test_doubles/e1ap/e1ap_cu_cp_test_messages.h"
#include "tests/test_doubles/e1ap/e1ap_test_message_validators.h"
#include "tests/test_doubles/f1ap/f1ap_test_message_validators.h"
#include "tests/test_doubles/ngap/ngap_test_message_validators.h"
#include "tests/test_doubles/rrc/rrc_test_message_validators.h"
#include "tests/test_doubles/rrc/rrc_test_messages.h"
#include "tests/test_doubles/xnap/xnap_test_message_validators.h"
#include "tests/unittests/cu_cp/test_doubles/mock_cu_up.h"
#include "tests/unittests/cu_cp/test_helpers.h"
#include "tests/unittests/ngap/ngap_test_messages.h"
#include "tests/unittests/xnap/xnap_test_messages.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents_ue.h"
#include "ocudu/asn1/ngap/ngap_pdu_contents.h"
#include "ocudu/cu_cp/cell_meas_manager_config.h"
#include "ocudu/cu_cp/cu_cp_configuration.h"
#include "ocudu/cu_cp/cu_cp_configuration_helpers.h"
#include "ocudu/cu_cp/cu_cp_factory.h"
#include "ocudu/e1ap/common/e1ap_message.h"
#include "ocudu/e1ap/common/e1ap_types.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/ngap/ngap_message.h"
#include "ocudu/ran/cu_types.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/support/executors/task_worker.h"
#include "ocudu/xnap/xnap_configuration.h"

using namespace ocudu;
using namespace ocucp;

class cu_cp_test_environment::worker_manager
{
  const unsigned WORKER_QUEUE_SIZE = 1024;

public:
  void stop() { worker.stop(); }

  void wait_pending_tasks() { worker.wait_pending_tasks(); }

  task_worker                    worker{"cu_cp_worker", WORKER_QUEUE_SIZE};
  std::unique_ptr<task_executor> exec{std::make_unique<task_worker_executor>(worker)};
};

cu_cp_test_environment::cu_cp_test_environment(cu_cp_test_env_params params_) :
  params(std::move(params_)),
  cu_cp_workers(std::make_unique<worker_manager>()),
  timers(64),
  cu_cp_deps([this]() {
    return cu_cp_dependencies{.n2_gws            = {},
                              .ng_setup_notifier = nullptr,
                              .xnc_gws           = {},
                              .cu_cp_executor    = *cu_cp_workers->exec,
                              .timers            = timers,
                              .metrics_notifier  = nullptr};
  }()),
  amf_configs(std::move(params.amf_configs)),
  xnc_peers(std::move(params.peer_xnc_configs))
{
  // Initialize logging.
  test_logger.set_level(ocudulog::basic_levels::debug);
  cu_cp_logger.set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("NRPPA").set_level(ocudulog::basic_levels::debug);
  ocudulog::fetch_basic_logger("PDCP").set_level(ocudulog::basic_levels::info);
  ocudulog::fetch_basic_logger("NGAP").set_hex_dump_max_size(32);
  ocudulog::fetch_basic_logger("RRC").set_hex_dump_max_size(32);
  ocudulog::fetch_basic_logger("SEC").set_hex_dump_max_size(32);
  ocudulog::init();

  // Create CU-CP config.
  cu_cp_cfg.admission.max_nof_dus         = params.max_nof_dus;
  cu_cp_cfg.admission.max_nof_cu_ups      = params.max_nof_cu_ups;
  cu_cp_cfg.admission.max_nof_ues         = params.max_nof_ues;
  cu_cp_cfg.admission.max_nof_drbs_per_ue = params.max_nof_drbs_per_ue;
  cu_cp_cfg.ntn_location_mappings         = params.ntn_location_mappings;

  // Fill NGAP config.
  std::vector<n2_connection_client*> n2_gws;
  for (const auto& [amf_index, amf_config] : amf_configs) {
    n2_gws.push_back(&*amf_config.amf_stub);
    cu_cp_cfg.ngap.amfs.push_back(cu_cp_configuration::amf_configuration{amf_config.supported_tas, amf_addr});
  }

  // Fill XNAP config. Each peer test stub becomes its own XnAP gateway; record the peer-gateway mapping.
  std::vector<xnc_connection_gateway*> xnc_gws;
  for (const auto& [_, peer] : xnc_peers) {
    const auto peer_idx = uint_to_xnc_peer_index(cu_cp_cfg.xnap.peers.size());
    const auto gw_idx   = uint_to_xnc_gateway_index(xnc_gws.size());
    xnc_gws.push_back(peer.get());
    cu_cp_cfg.xnap.peer_to_gateway[peer_idx] = gw_idx;
    cu_cp_cfg.xnap.peers.push_back(cu_cp_configuration::xnc_peer_configuration{
        .peer_addrs = {transport_layer_address::create_from_string("127.0.0.1")}});
    next_xnc_peer_idx++;
  }

  // Fill PWS config.
  cu_cp_cfg.pws.max_warning_message_segment_size = params.pws_max_warning_message_segment_size;

  // Fill logging and metrics config.
  cu_cp_cfg.f1ap.json_log_enabled                  = true;
  cu_cp_cfg.e1ap.json_log_enabled                  = true;
  cu_cp_cfg.metrics.layers_cfg.enable_ngap_metrics = true;
  cu_cp_cfg.metrics.layers_cfg.enable_rrc_metrics  = true;

  // Fill mobility config.
  cu_cp_cfg.mobility.mobility_mgr_config.trigger_handover_from_measurements = params.trigger_ho_from_measurements;
  {
    // Fill meas manager config.
    cell_meas_manager_config meas_mng_cfg;
    {
      // Generate NCIs.
      gnb_id_t         gnb_id1 = cu_cp_cfg.node.gnb_id;
      nr_cell_identity nci1    = nr_cell_identity::create(gnb_id1, 0).value();
      nr_cell_identity nci2    = nr_cell_identity::create(gnb_id1, 1).value();
      gnb_id_t         gnb_id2 = {cu_cp_cfg.node.gnb_id.id + 1, cu_cp_cfg.node.gnb_id.bit_length};
      nr_cell_identity nci3    = nr_cell_identity::create(gnb_id2, 0).value();
      gnb_id_t         gnb_id3 = {cu_cp_cfg.node.gnb_id.id + 2, cu_cp_cfg.node.gnb_id.bit_length};
      nr_cell_identity nci4    = nr_cell_identity::create(gnb_id3, 0).value();

      // Create cell 1.
      {
        cell_meas_config cell_cfg_1;
        cell_cfg_1.periodic_report_cfg_id             = uint_to_report_cfg_id(1);
        cell_cfg_1.serving_cell_cfg.gnb_id_bit_length = gnb_id1.bit_length;
        cell_cfg_1.serving_cell_cfg.nci               = nci1;
        // A neighbour that is both a measurement-report neighbour and a CHO candidate carries the regular
        // event-triggered report config plus the conditional trigger used by condExecutionCond.
        std::vector<report_cfg_id_t> ncell_report_cfg_ids = {uint_to_report_cfg_id(2)};
        if (params.add_cho_cond_trigger) {
          ncell_report_cfg_ids.push_back(uint_to_report_cfg_id(3));
        }
        cell_cfg_1.ncells.push_back({nci2, ncell_report_cfg_ids});
        // Add external cells (for inter CU handover tests).
        cell_cfg_1.ncells.push_back({nci3, ncell_report_cfg_ids});
        if (!xnc_peers.empty()) {
          cell_cfg_1.ncells.push_back({nci4, ncell_report_cfg_ids});
        }
        meas_mng_cfg.cells.emplace(nci1, cell_cfg_1);
      }

      // Create cell 2.
      {
        cell_meas_config cell_cfg_2;
        cell_cfg_2.periodic_report_cfg_id             = uint_to_report_cfg_id(1);
        cell_cfg_2.serving_cell_cfg.gnb_id_bit_length = gnb_id1.bit_length;
        cell_cfg_2.serving_cell_cfg.nci               = nci2;
        cell_cfg_2.ncells.push_back({nci1, {uint_to_report_cfg_id(2)}});
        meas_mng_cfg.cells.emplace(nci2, cell_cfg_2);
      }

      // Create an external cell.
      {
        cell_meas_config cell_cfg_3;
        cell_cfg_3.periodic_report_cfg_id             = uint_to_report_cfg_id(1);
        cell_cfg_3.serving_cell_cfg.gnb_id_bit_length = gnb_id2.bit_length;
        cell_cfg_3.serving_cell_cfg.nci               = nci3;
        cell_cfg_3.serving_cell_cfg.pci               = 3;
        cell_cfg_3.serving_cell_cfg.plmn              = plmn_identity::test_value();
        cell_cfg_3.serving_cell_cfg.tac               = 7;
        cell_cfg_3.serving_cell_cfg.ssb_arfcn         = 632628;
        cell_cfg_3.serving_cell_cfg.band              = nr_band::n78;
        cell_cfg_3.serving_cell_cfg.ssb_scs           = subcarrier_spacing::kHz15;
        cell_cfg_3.serving_cell_cfg.ssb_mtc = rrc_ssb_mtc{{rrc_periodicity_and_offset::periodicity_t::sf20, 0}, 5};

        cell_cfg_3.ncells.push_back({nci1, {uint_to_report_cfg_id(2)}});
        meas_mng_cfg.cells.emplace(nci3, cell_cfg_3);
      }

      if (!xnc_peers.empty()) {
        // Create an external Xn-C cell.
        {
          cell_meas_config cell_cfg_4;
          cell_cfg_4.periodic_report_cfg_id             = uint_to_report_cfg_id(1);
          cell_cfg_4.serving_cell_cfg.gnb_id_bit_length = gnb_id3.bit_length;
          cell_cfg_4.serving_cell_cfg.nci               = nci4;
          cell_cfg_4.serving_cell_cfg.pci               = 4;
          cell_cfg_4.serving_cell_cfg.plmn              = plmn_identity::test_value();
          cell_cfg_4.serving_cell_cfg.tac               = 7;
          cell_cfg_4.serving_cell_cfg.ssb_arfcn         = 621300;
          cell_cfg_4.serving_cell_cfg.band              = nr_band::n78;
          cell_cfg_4.serving_cell_cfg.ssb_scs           = subcarrier_spacing::kHz30;
          cell_cfg_4.serving_cell_cfg.ssb_mtc = rrc_ssb_mtc{{rrc_periodicity_and_offset::periodicity_t::sf20, 0}, 5};

          cell_cfg_4.ncells.push_back({nci1, {uint_to_report_cfg_id(2)}});
          meas_mng_cfg.cells.emplace(nci4, cell_cfg_4);
        }
      }

      // Add periodic event.
      {
        rrc_periodical_report_cfg periodical_cfg;
        periodical_cfg.rs_type                 = ocucp::rrc_nr_rs_type::ssb;
        periodical_cfg.report_interv           = 1024;
        periodical_cfg.report_amount           = -1;
        periodical_cfg.report_quant_cell.rsrp  = true;
        periodical_cfg.report_quant_cell.rsrq  = true;
        periodical_cfg.report_quant_cell.sinr  = true;
        periodical_cfg.max_report_cells        = 4;
        periodical_cfg.include_beam_meass      = true;
        periodical_cfg.use_allowed_cell_list   = false;
        periodical_cfg.periodic_ho_rsrp_offset = 2;

        meas_mng_cfg.report_config_ids.emplace(uint_to_report_cfg_id(1), rrc_report_cfg_nr{periodical_cfg});
      }

      // Add event A3.
      {
        rrc_event_trigger_cfg event_trigger_cfg = {};

        rrc_event_id& event_a3 = event_trigger_cfg.event_id;
        event_a3.id            = rrc_event_id::event_id_t::a3;
        event_a3.meas_trigger_quant_thres_or_offset.emplace();
        event_a3.meas_trigger_quant_thres_or_offset.value().rsrp.emplace() = 6;
        event_a3.hysteresis                                                = 0;
        event_a3.time_to_trigger                                           = 100;
        event_a3.use_allowed_cell_list                                     = false;

        event_trigger_cfg.rs_type                = ocucp::rrc_nr_rs_type::ssb;
        event_trigger_cfg.report_interv          = 1024;
        event_trigger_cfg.report_amount          = -1;
        event_trigger_cfg.report_quant_cell.rsrp = true;
        event_trigger_cfg.report_quant_cell.rsrq = true;
        event_trigger_cfg.report_quant_cell.sinr = true;
        event_trigger_cfg.max_report_cells       = 4;
        event_trigger_cfg.include_beam_meass     = true;

        rrc_meas_report_quant report_quant_rs_idxes;
        report_quant_rs_idxes.rsrp              = true;
        report_quant_rs_idxes.rsrq              = true;
        report_quant_rs_idxes.sinr              = true;
        event_trigger_cfg.report_quant_rs_idxes = report_quant_rs_idxes;

        meas_mng_cfg.report_config_ids.emplace(uint_to_report_cfg_id(2), rrc_report_cfg_nr{event_trigger_cfg});
      }

      // Add CHO conditional trigger (event A3).
      if (params.add_cho_cond_trigger) {
        rrc_cond_trigger_cfg cond_trigger;

        rrc_event_id& cond_event_a3 = cond_trigger.cond_event_id;
        cond_event_a3.id            = rrc_event_id::event_id_t::a3;
        cond_event_a3.meas_trigger_quant_thres_or_offset.emplace();
        cond_event_a3.meas_trigger_quant_thres_or_offset.value().rsrp.emplace() = 6;
        cond_event_a3.hysteresis                                                = 0;
        cond_event_a3.time_to_trigger                                           = 100;

        cond_trigger.rs_type = ocucp::rrc_nr_rs_type::ssb;

        meas_mng_cfg.report_config_ids.emplace(uint_to_report_cfg_id(3), rrc_report_cfg_nr{cond_trigger});
      }
    }
    cu_cp_cfg.mobility.meas_mgr_config = meas_mng_cfg;
  }

  // Fill RRC config.
  cu_cp_cfg.rrc.rrc_procedure_guard_time_ms =
      std::chrono::milliseconds(10000); // procedure timeouts should only occur intentionally

  // Fill RRC Reject waitTime value.
  cu_cp_cfg.rrc.rrc_reject_wait_time = params.rrc_reject_wait_time;

  // Fill F1AP config.
  cu_cp_cfg.f1ap.proc_timeout = std::chrono::milliseconds(10000); // procedure timeouts should only occur intentionally

  // Fill E1AP config.
  cu_cp_cfg.e1ap.proc_timeout = std::chrono::milliseconds(10000); // procedure timeouts should only occur intentionally

  // Fill UE config.
  cu_cp_cfg.ue.request_pdu_session_timeout =
      std::chrono::seconds(10); // procedure timeouts should only occur intentionally
  cu_cp_cfg.ue.enable_rrc_inactive = params.enable_rrc_inactive;

  // Fill declared logical cells.
  cu_cp_cfg.cells = params.logical_cells;

  // Fill CU-CP dependencies.
  cu_cp_deps.n2_gws  = std::move(n2_gws);
  cu_cp_deps.xnc_gws = std::move(xnc_gws);

  // Create CU-CP instance.
  cu_cp_inst = create_cu_cp(cu_cp_cfg, cu_cp_deps);
}

cu_cp_test_environment::~cu_cp_test_environment()
{
  cu_cp_inst->stop();
  dus.clear();
  cu_ups.clear();
  xnc_peers.clear();
  cu_cp_workers->stop();

  ocudulog::flush();
}

void cu_cp_test_environment::tick()
{
  // Dispatch clock ticking to CU-CP worker.
  cu_cp_workers->worker.push_task_blocking([this]() { timers.tick(); });
}

bool cu_cp_test_environment::tick_until(std::chrono::milliseconds    timeout,
                                        const std::function<bool()>& stop_condition,
                                        bool                         real_time)
{
  std::mutex              mutex;
  std::condition_variable cvar;
  bool                    done = false;

  // Tick up to "timeout" times, waiting for stop_condition() to return true.
  for (unsigned i = 0; i != timeout.count(); ++i) {
    if (stop_condition()) {
      return true;
    }

    // Push to CU-CP worker task that checks the state of the condition.
    done = false;
    cu_cp_workers->worker.push_task_blocking([&]() {
      // Need to tick the clock.
      tick();

      std::scoped_lock lock(mutex);
      done = true;
      cvar.notify_one();
    });

    // Wait for tick to be processed.
    {
      std::unique_lock<std::mutex> lock(mutex);
      cvar.wait(lock, [&done]() { return done; });
    }
    if (real_time) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  return stop_condition();
}

void cu_cp_test_environment::run_on_cu_cp_worker(const std::function<void()>& task)
{
  std::mutex              mutex;
  std::condition_variable cvar;
  bool                    done = false;

  cu_cp_workers->worker.push_task_blocking([&]() {
    task();

    std::lock_guard<std::mutex> lock(mutex);
    done = true;
    cvar.notify_one();
  });

  std::unique_lock<std::mutex> lock(mutex);
  cvar.wait(lock, [&done]() { return done; });
}

bool cu_cp_test_environment::wait_ready_on_cu_cp_worker(std::chrono::milliseconds    timeout,
                                                        const std::function<bool()>& is_ready)
{
  bool ready = false;

  // is_ready is evaluated on the CU-CP worker, alongside the clock tick, so that reading state mutated by the
  // CU-CP worker (e.g. an async task's completion flag) is properly synchronized.
  for (unsigned i = 0; i != timeout.count(); ++i) {
    run_on_cu_cp_worker([&]() {
      ready = is_ready();
      if (not ready) {
        // Already running on the CU-CP worker; tick the clock directly rather than via \c tick(), which
        // would try to (and fail to) push a blocking task to this same worker.
        timers.tick();
      }
    });
    if (ready) {
      return true;
    }
  }

  return ready;
}

bool cu_cp_test_environment::wait_for_ngap_tx_pdu(ngap_message&             pdu,
                                                  std::chrono::milliseconds timeout,
                                                  unsigned                  amf_idx)
{
  return tick_until(timeout, [&]() { return amf_configs.at(amf_idx).amf_stub->try_pop_rx_pdu(pdu); });
}

bool cu_cp_test_environment::wait_for_xnap_tx_pdu(unsigned                  xnc_peer_idx,
                                                  xnap_message&             pdu,
                                                  std::chrono::milliseconds timeout)
{
  return tick_until(timeout, [&]() { return xnc_peers.at(xnc_peer_idx)->try_pop_rx_pdu(pdu); });
}

bool cu_cp_test_environment::wait_for_e1ap_tx_pdu(unsigned                  cu_up_idx,
                                                  e1ap_message&             pdu,
                                                  std::chrono::milliseconds timeout)
{
  return tick_until(timeout, [&]() { return cu_ups.at(cu_up_idx)->try_pop_rx_pdu(pdu); });
}

bool cu_cp_test_environment::wait_for_f1ap_tx_pdu(unsigned du_idx, f1ap_message& pdu, std::chrono::milliseconds timeout)
{
  report_fatal_error_if_not(dus.size() >= du_idx and dus[du_idx] != nullptr, "DU index out of range");

  return tick_until(timeout, [&]() {
    if (du_idx >= dus.size() or dus[du_idx] == nullptr) {
      return false;
    }
    return dus[du_idx]->try_pop_dl_pdu(pdu);
  });
}

const cu_cp_test_environment::ue_context* cu_cp_test_environment::find_ue_context(unsigned            du_idx,
                                                                                  gnb_du_ue_f1ap_id_t du_ue_id) const
{
  auto it = du_ue_id_to_ran_ue_id_map.at(du_idx).find(du_ue_id);
  if (it == du_ue_id_to_ran_ue_id_map.at(du_idx).end()) {
    return nullptr;
  }
  return &attached_ues.at(it->second);
}

ngap_message cu_cp_test_environment::generate_amf_ng_setup_response(unsigned amf_idx) const
{
  auto it = amf_configs.find(amf_idx);
  report_fatal_error_if_not(it != amf_configs.end(), "AMF {} not found", amf_idx);

  std::vector<plmn_identity> plmns;
  for (const auto& ta : it->second.supported_tas) {
    for (const auto& plmn_item : ta.plmn_list) {
      if (std::find(plmns.begin(), plmns.end(), plmn_item.plmn_id) == plmns.end()) {
        plmns.push_back(plmn_item.plmn_id);
      }
    }
  }
  report_fatal_error_if_not(not plmns.empty(), "AMF {} has no PLMN", amf_idx);

  ngap_message ng_setup_response = ocucp::generate_ng_setup_response(plmns.front());
  auto&        asn1_response     = ng_setup_response.pdu.successful_outcome().value.ng_setup_resp();
  for (unsigned i = 1; i != plmns.size(); ++i) {
    asn1::ngap::plmn_support_item_s plmn_support_item = asn1_response->plmn_support_list[0];
    plmn_support_item.plmn_id                         = plmns[i].to_bytes();
    asn1_response->plmn_support_list.push_back(plmn_support_item);
  }
  return ng_setup_response;
}

void cu_cp_test_environment::enqueue_procedure_outcome_pdus_and_start_cu_cp()
{
  // Enqueue NG Setup responses.s.
  for (const auto& [amf_index, amf_config] : amf_configs) {
    get_amf(amf_index).enqueue_next_tx_pdu(generate_amf_ng_setup_response(amf_index));
  }

  // Enqueue XN Setup responses. Each peer advertises one served cell, so that tests can resolve it by PCI the way a
  // UE context retrieval does.
  for (const auto& [xnc_peer_idx, xnc_peer] : xnc_peers) {
    get_xnc_cu_cp(xnc_peer_idx)
        .enqueue_next_tx_pdu(generate_xn_setup_response_with_served_cell(
            xnap_configuration{.gnb_id           = get_xnc_peer_gnb_id(),
                               .tai_support_list = amf_configs.begin()->second.supported_tas},
            xnc_peer_served_pci,
            nr_cell_global_id_t{plmn_identity::test_value(), xnc_peer_served_nci()}));
  }

  // Attach Xn-C handler before starting CU-CP (matching real app startup order).
  for (auto* gateway : cu_cp_deps.xnc_gws) {
    gateway->attach_cu_cp(get_cu_cp().get_xnc_handler());
  }

  // Start CU-CP.
  get_cu_cp().start();
}

void cu_cp_test_environment::run_ng_setup()
{
  // Enqueue NG setup procedure responses and start CU-CP.
  enqueue_procedure_outcome_pdus_and_start_cu_cp();

  ngap_message ngap_pdu;
  for (const auto& [amf_index, amf_config] : amf_configs) {
    report_fatal_error_if_not(get_amf(amf_index).try_pop_rx_pdu(ngap_pdu),
                              "CU-CP did not send the NG Setup Request to the AMF {}",
                              amf_index);
    report_fatal_error_if_not(is_pdu_type(ngap_pdu, asn1::ngap::ngap_elem_procs_o::init_msg_c::types::ng_setup_request),
                              "CU-CP did not setup the AMF connection");
  }
}

bool cu_cp_test_environment::drop_amf_connection(unsigned amf_idx)
{
  auto it = amf_configs.find(amf_idx);
  if (it == amf_configs.end()) {
    return false;
  }
  it->second.amf_stub->drop_connection();
  // Wait for the CU-CP to process the disconnection.
  cu_cp_workers->wait_pending_tasks();
  return true;
}

bool cu_cp_test_environment::reconnect_amf(unsigned amf_idx)
{
  auto it = amf_configs.find(amf_idx);
  if (it == amf_configs.end()) {
    return false;
  }

  it->second.amf_stub->allow_reconnection();

  it->second.amf_stub->enqueue_next_tx_pdu(generate_amf_ng_setup_response(amf_idx));

  tick_until(cu_cp_cfg.ngap.amf_reconnection_retry_time, [&]() { return false; }, false);

  ngap_message ng_setup_req;
  return wait_for_ngap_tx_pdu(ng_setup_req, std::chrono::milliseconds{1000}, amf_idx);
}

void cu_cp_test_environment::run_xn_setup()
{
  xnap_message xnap_pdu;
  for (const auto& [xnc_peer_idx, xnc_peer] : xnc_peers) {
    report_fatal_error_if_not(wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu),
                              "CU-CP did not send the XN Setup Request to the Xn-C peer CU-CP {}",
                              xnc_peer_idx);
    report_fatal_error_if_not(
        test_helpers::is_pdu_type(xnap_pdu, asn1::xnap::xnap_elem_procs_o::init_msg_c::types::xn_setup_request),
        "CU-CP did not setup the Xn-C connection");
  }
}

void cu_cp_test_environment::run_ngran_node_cfg_update(span<const test_helpers::served_cell_item_info> added_cells)
{
  xnap_message xnap_pdu;
  for (const auto& [xnc_peer_idx, xnc_peer] : xnc_peers) {
    report_fatal_error_if_not(wait_for_xnap_tx_pdu(xnc_peer_idx, xnap_pdu),
                              "CU-CP did not send the NG-RAN Node Configuration Update to the Xn-C peer CU-CP {}",
                              xnc_peer_idx);
    report_fatal_error_if_not(
        test_helpers::is_pdu_type(xnap_pdu, asn1::xnap::xnap_elem_procs_o::init_msg_c::types::ngran_node_cfg_upd),
        "CU-CP did not report its served cells to the Xn-C peer CU-CP {}",
        xnc_peer_idx);

    const auto& asn1_cells_to_add = xnap_pdu.pdu.init_msg()
                                        .value.ngran_node_cfg_upd()
                                        ->cfg_upd_init_node_choice.gnb()
                                        .served_cells_to_upd_nr.served_cells_to_add_nr;
    report_fatal_error_if_not(asn1_cells_to_add.size() == added_cells.size(),
                              "CU-CP reported {} added cells to the Xn-C peer CU-CP {}, expected {}",
                              asn1_cells_to_add.size(),
                              xnc_peer_idx,
                              added_cells.size());
    for (unsigned i = 0; i != added_cells.size(); ++i) {
      const auto& asn1_cell_info = asn1_cells_to_add[i].served_cell_info_nr;
      report_fatal_error_if_not(asn1_cell_info.nr_pci == added_cells[i].pci,
                                "CU-CP reported pci={} for the cell nci={}, expected pci={}",
                                asn1_cell_info.nr_pci,
                                added_cells[i].nci,
                                added_cells[i].pci);
      report_fatal_error_if_not(asn1_cell_info.cell_id.nr_ci.to_number() == added_cells[i].nci.value(),
                                "CU-CP reported an unexpected cell to the Xn-C peer CU-CP {}",
                                xnc_peer_idx);
    }

    last_ngran_node_cfg_update = xnap_pdu;
    get_xnc_cu_cp(xnc_peer_idx).push_tx_pdu(generate_asn1_ngran_node_cfg_update_ack());
  }
}

std::optional<unsigned> cu_cp_test_environment::connect_new_du()
{
  auto du_stub = create_mock_du({get_cu_cp().get_f1c_handler()});
  if (not du_stub) {
    return std::nullopt;
  }
  for (; dus.count(next_du_idx) != 0; ++next_du_idx) {
  }
  auto ret = dus.insert(std::make_pair(next_du_idx, std::move(du_stub)));
  report_fatal_error_if_not(ret.second, "Race condition detected");
  return next_du_idx;
}

bool cu_cp_test_environment::drop_du_connection(unsigned du_idx)
{
  auto it = dus.find(du_idx);
  if (it == dus.end()) {
    return false;
  }
  dus.erase(it);
  return true;
}

bool cu_cp_test_environment::run_f1_setup(unsigned                                                du_idx,
                                          gnb_du_id_t                                             gnb_du_id,
                                          const std::vector<test_helpers::served_cell_item_info>& cells)
{
  f1ap_message f1_setup_req = test_helpers::generate_f1_setup_request(gnb_du_id, cells);
  rrc_test_timer_values     = ocucp::get_timers(f1_setup_req.pdu.init_msg().value.f1_setup_request());
  get_du(du_idx).push_ul_pdu(f1_setup_req);
  f1ap_message f1ap_pdu;
  bool         result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);

  // The cells of the DU are reported to the Xn-C peers.
  run_ngran_node_cfg_update(cells);

  return result;
}

std::optional<unsigned> cu_cp_test_environment::connect_new_cu_up()
{
  auto cu_up_obj = create_mock_cu_up(get_cu_cp().get_e1_handler());
  if (not cu_up_obj) {
    return std::nullopt;
  }
  for (; cu_ups.count(next_cu_up_idx) != 0; ++next_cu_up_idx) {
  }
  auto ret = cu_ups.insert(std::make_pair(next_cu_up_idx, std::move(cu_up_obj)));
  report_fatal_error_if_not(ret.second, "Race condition detected");
  return next_cu_up_idx;
}

bool cu_cp_test_environment::drop_cu_up_connection(unsigned cu_up_idx)
{
  auto it = cu_ups.find(cu_up_idx);
  if (it == cu_ups.end()) {
    return false;
  }
  cu_ups.erase(it);
  return true;
}

bool cu_cp_test_environment::run_e1_setup(unsigned cu_up_idx)
{
  get_cu_up(cu_up_idx).push_tx_pdu(generate_valid_cu_up_e1_setup_request());
  e1ap_message e1ap_pdu;
  bool         result = this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu, std::chrono::milliseconds{1000});
  return result;
}

bool cu_cp_test_environment::connect_new_ue(unsigned            du_idx,
                                            gnb_du_ue_f1ap_id_t du_ue_id,
                                            rnti_t              crnti,
                                            plmn_identity       plmn,
                                            byte_buffer         rrc_setup_complete)
{
  ngap_message ngap_pdu;
  ocudu_assert(not this->get_amf().try_pop_rx_pdu(ngap_pdu), "there are still NGAP messages to pop from AMF");
  f1ap_message f1ap_pdu;
  ocudu_assert(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu), "there are still F1AP DL messages to pop from DU");

  // Inject Initial UL RRC message.
  f1ap_message init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(du_ue_id, crnti, plmn);
  test_logger.info("c-rnti={} du_ue={}: Injecting Initial UL RRC message", crnti, fmt::underlying(du_ue_id));
  get_du(du_idx).push_ul_pdu(init_ul_rrc_msg);

  // Wait for DL RRC message transfer (containing RRC Setup)
  bool result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu, std::chrono::milliseconds{1000});
  if (not result) {
    return false;
  }

  // Check if the DL RRC Message with Msg4 is valid.
  report_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer_with_msg4(f1ap_pdu), "invalid DL RRC message");

  // Check if the UE Id matches.
  auto& dl_rrc_msg = *f1ap_pdu.pdu.init_msg().value.dl_rrc_msg_transfer();
  report_error_if_not(int_to_gnb_du_ue_f1ap_id(dl_rrc_msg.gnb_du_ue_f1ap_id) == du_ue_id, "invalid gNB-DU-UE-F1AP-ID");
  report_error_if_not(int_to_srb_id(dl_rrc_msg.srb_id) == srb_id_t::srb0, "invalid SRB-Id");

  // Send RRC Setup Complete.
  get_du(du_idx).push_rrc_ul_dcch_message(du_ue_id, srb_id_t::srb1, std::move(rrc_setup_complete));

  // CU-CP should send an NGAP Initial UE Message.
  result = this->wait_for_ngap_tx_pdu(ngap_pdu);
  if (not result) {
    return false;
  }
  report_fatal_error_if_not(test_helpers::is_valid_init_ue_message(ngap_pdu), "Invalid init UE message");

  ue_context ue_ctx{};
  ue_ctx.crnti     = crnti;
  ue_ctx.du_ue_id  = du_ue_id;
  ue_ctx.cu_ue_id  = int_to_gnb_cu_ue_f1ap_id(dl_rrc_msg.gnb_cu_ue_f1ap_id);
  ue_ctx.ran_ue_id = uint_to_ran_ue_id(ngap_pdu.pdu.init_msg().value.init_ue_msg()->ran_ue_ngap_id);

  report_fatal_error_if_not(attached_ues.insert(std::make_pair(ue_ctx.ran_ue_id.value(), ue_ctx)).second,
                            "UE already exists");
  report_fatal_error_if_not(
      du_ue_id_to_ran_ue_id_map[du_idx].insert(std::make_pair(du_ue_id, ue_ctx.ran_ue_id.value())).second,
      "DU UE ID already exists");

  return true;
}

bool cu_cp_test_environment::authenticate_ue(unsigned du_idx, gnb_du_ue_f1ap_id_t du_ue_id, amf_ue_id_t amf_ue_id)
{
  ngap_message ngap_pdu;
  ocudu_assert(not this->get_amf().try_pop_rx_pdu(ngap_pdu), "there are still NGAP messages to pop from AMF");
  f1ap_message f1ap_pdu;
  ocudu_assert(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu), "there are still F1AP DL messages to pop from DU");

  auto& ue_ctx     = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));
  ue_ctx.amf_ue_id = amf_ue_id;

  // Inject NGAP DL message (authentication request).
  ngap_message dl_nas_transport =
      ocucp::generate_downlink_nas_transport_message(ue_ctx.amf_ue_id.value(), ue_ctx.ran_ue_id.value());
  get_amf().push_tx_pdu(dl_nas_transport);

  // Wait for DL RRC message transfer (containing NAS message).
  bool result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  if (not result) {
    return false;
  }
  report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                            "Invalid DL RRC message transfer");

  // Inject UL RRC msg transfer (authentication response).
  f1ap_message ul_rrc_msg_transfer = test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id,
      ue_ctx.cu_ue_id.value(),
      srb_id_t::srb1,
      make_byte_buffer("00013a0abf002b96882dac46355c4f34464ddaf7b43fde37ae8000000000").value());
  get_du(du_idx).push_ul_pdu(ul_rrc_msg_transfer);

  // Wait for UL NAS Message (containing authentication response).
  result = this->wait_for_ngap_tx_pdu(ngap_pdu);
  if (not result) {
    return false;
  }

  // Inject DL NAS Transport message (ue security mode command).
  dl_nas_transport = generate_downlink_nas_transport_message(amf_ue_id, ue_ctx.ran_ue_id.value());
  get_amf().push_tx_pdu(dl_nas_transport);

  result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  if (not result) {
    return false;
  }
  report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                            "Invalid DL RRC message transfer");

  // Inject UL RRC msg transfer (ue security mode complete).
  ul_rrc_msg_transfer = test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id,
      ue_ctx.cu_ue_id.value(),
      srb_id_t::srb1,
      make_byte_buffer("00023a1cbf0243241cb5003f002f3b80048290a1b283800000f8b880103f0020bc800680807888787f800008192a3b4"
                       "c080080170170700c0080a980808000000000")
          .value());
  get_du(du_idx).push_ul_pdu(ul_rrc_msg_transfer);

  // Wait for UL NAS Message (containing ue security mode complete).
  result = this->wait_for_ngap_tx_pdu(ngap_pdu);
  return result;
}

bool cu_cp_test_environment::setup_ue_security_and_ue_capabilities(
    unsigned                                                   du_idx,
    gnb_du_ue_f1ap_id_t                                        du_ue_id,
    std::optional<cu_cp_core_network_assist_info_for_inactive> cn_assist_info_for_inactive,
    bool                                                       rrc_inactive_supported,
    std::optional<location_report_request>                     location_reporting_request)
{
  ngap_message ngap_pdu;
  ocudu_assert(not this->get_amf().try_pop_rx_pdu(ngap_pdu), "there are still NGAP messages to pop from AMF");
  f1ap_message f1ap_pdu;
  ocudu_assert(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu), "there are still F1AP DL messages to pop from DU");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));

  // Inject NGAP Initial Context Setup Request.
  ngap_message init_ctxt_setup_req =
      generate_valid_initial_context_setup_request_message(ue_ctx.amf_ue_id.value(),
                                                           ue_ctx.ran_ue_id.value(),
                                                           std::move(cn_assist_info_for_inactive),
                                                           std::move(location_reporting_request));
  get_amf().push_tx_pdu(init_ctxt_setup_req);

  // Wait for F1AP UE Context Setup Request (containing Security Mode Command).
  bool result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive Security Mode Command");
  report_fatal_error_if_not(test_helpers::is_valid_ue_context_setup_request(f1ap_pdu),
                            "Invalid UE Context Setup Request");
  {
    const byte_buffer& rrc_container = test_helpers::get_rrc_container(f1ap_pdu);
    report_fatal_error_if_not(
        test_helpers::is_valid_rrc_security_mode_command(test_helpers::extract_dl_dcch_msg(rrc_container)),
        "Invalid Security Mode command");
  }

  // Inject UE Context Setup Response.
  f1ap_message ue_ctxt_setup_response =
      test_helpers::generate_ue_context_setup_response(ue_ctx.cu_ue_id.value(), du_ue_id);
  get_du(du_idx).push_ul_pdu(ue_ctxt_setup_response);

  // Inject RRC Security Mode Complete.
  f1ap_message ul_rrc_msg_transfer = test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id, ue_ctx.cu_ue_id.value(), srb_id_t::srb1, make_byte_buffer("00032a00fd5ec7ff").value());
  get_du(du_idx).push_ul_pdu(ul_rrc_msg_transfer);

  // Wait for UE Capability Enquiry.
  result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive DL RRC Message, containing RRC UE Capability Enquiry");
  report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                            "Invalid DL RRC Message Transfer");
  {
    const byte_buffer& rrc_container = test_helpers::get_rrc_container(f1ap_pdu);
    report_fatal_error_if_not(
        test_helpers::is_valid_rrc_ue_capability_enquiry(test_helpers::extract_dl_dcch_msg(rrc_container)),
        "Invalid UE Capability Enquiry");
  }

  // Inject UL RRC Message Transfer (containing UE Capability Info).
  get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id,
      ue_ctx.cu_ue_id.value(),
      srb_id_t::srb1,
      4,
      test_helpers::pack_ul_dcch_msg(test_helpers::create_ue_capability_info(2, rrc_inactive_supported)),
      rrc_inactive_supported ? std::array<uint8_t, 4>{0x3c, 0x9b, 0x5c, 0x4d}
                             : std::array<uint8_t, 4>{0xd8, 0x0a, 0xdc, 0xc0}));

  // Wait for DL RRC Message Transfer (containing NAS Registration Accept).
  result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive DL RRC Message, containing NAS Registration Accept");
  report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                            "Invalid DL RRC Message Transfer");

  // Wait for Initial Context Setup Response.
  result = this->wait_for_ngap_tx_pdu(ngap_pdu);
  report_fatal_error_if_not(result, "Failed to receive first NGAP PDU after ICS");
  report_fatal_error_if_not(test_helpers::is_valid_initial_context_setup_response(ngap_pdu), "Invalid init ctxt setup");

  // Wait for UE Radio Capability Info Indication.
  result = this->wait_for_ngap_tx_pdu(ngap_pdu);
  report_fatal_error_if_not(result, "Failed to receive UE Radio Capability Info Indication");
  report_fatal_error_if_not(test_helpers::is_valid_ue_radio_capability_info_indication(ngap_pdu),
                            "Invalid UE Radio Capability Info Indication");

  // Do not wait for Location Report, as tests may check its contents.
  return true;
}

std::optional<ngap_message> cu_cp_test_environment::get_location_report_if_required(
    const std::optional<location_report_request>& location_reporting_request)
{
  // Wait for location report, if required.
  if (location_reporting_request.has_value()) {
    using event_type = location_report_request::event_type;
    if (location_reporting_request->location_reporting_type == event_type::direct ||
        location_reporting_request->location_reporting_type == event_type::change_of_serve_cell ||
        location_reporting_request->location_reporting_type ==
            event_type::change_of_serving_cell_and_ue_presence_in_the_area_of_interest) {
      ngap_message ngap_pdu;
      bool         result = this->wait_for_ngap_tx_pdu(ngap_pdu);
      report_fatal_error_if_not(result, "Failed to transmit Location Report");
      report_fatal_error_if_not(test_helpers::is_valid_location_report(ngap_pdu), "Invalid Location Report");

      return ngap_pdu;
    }
  }
  return std::nullopt;
}
bool cu_cp_test_environment::finish_ue_registration(unsigned du_idx, unsigned cu_up_idx, gnb_du_ue_f1ap_id_t du_ue_id)
{
  ngap_message ngap_pdu;
  ocudu_assert(not this->get_amf().try_pop_rx_pdu(ngap_pdu), "there are still NGAP messages to pop from AMF");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));

  // Inject Registration Complete and wait UL NAS message.
  get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id,
      ue_ctx.cu_ue_id.value(),
      srb_id_t::srb1,
      make_byte_buffer("00053a053f015362c51680bf00218086b09a5b").value()));
  bool result = this->wait_for_ngap_tx_pdu(ngap_pdu);
  report_fatal_error_if_not(result, "Failed to receive Registration Complete");

  return true;
}

bool cu_cp_test_environment::request_pdu_session_resource_setup(unsigned            du_idx,
                                                                unsigned            cu_up_idx,
                                                                gnb_du_ue_f1ap_id_t du_ue_id)
{
  ngap_message ngap_pdu;
  ocudu_assert(not this->get_amf().try_pop_rx_pdu(ngap_pdu), "there are still NGAP messages to pop from AMF");
  f1ap_message f1ap_pdu;
  ocudu_assert(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu), "there are still F1AP DL messages to pop from DU");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));

  // Inject PDU Session Establishment Request and wait UL NAS message.
  get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id,
      ue_ctx.cu_ue_id.value(),
      srb_id_t::srb1,
      make_byte_buffer("00063a253f011ffa9203013f0033808018970080e0ffffc9d8bd8013404010880080000840830000000041830000000"
                       "00000800001800005000006000006800008800900c092838339b939b0b83700e03a21bb")
          .value()));
  bool result = this->wait_for_ngap_tx_pdu(ngap_pdu);
  report_fatal_error_if_not(result, "Failed to receive Registration Complete");

  // Inject Configuration Update Command.
  ngap_message dl_nas_transport_msg = generate_downlink_nas_transport_message(
      ue_ctx.amf_ue_id.value(),
      ue_ctx.ran_ue_id.value(),
      make_byte_buffer("7e0205545bfc027e0054430f90004f00700065006e00350047005346004732800131235200490100").value());
  get_amf().push_tx_pdu(dl_nas_transport_msg);
  result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive NAS Configuration Update Command");
  report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                            "Invalid DL RRC Message Transfer");

  return true;
}

expected<e1ap_message>
cu_cp_test_environment::send_pdu_session_resource_setup_request_and_await_bearer_context_setup_request(
    const ngap_message& pdu_session_resource_setup_request,
    unsigned            du_idx,
    unsigned            cu_up_idx,
    gnb_du_ue_f1ap_id_t du_ue_id)
{
  e1ap_message e1ap_pdu;
  ocudu_assert(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
               "there are still E1AP messages to pop from CU-UP");

  // Inject PDU Session Resource Setup Request and wait for Bearer Context Setup Request.
  get_amf().push_tx_pdu(pdu_session_resource_setup_request);
  bool result = this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive Bearer Context Setup Request");
  report_fatal_error_if_not(test_helpers::is_valid_bearer_context_setup_request(e1ap_pdu),
                            "Invalid Bearer Context Setup Request");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));
  ue_ctx.cu_cp_e1ap_id =
      int_to_gnb_cu_cp_ue_e1ap_id(e1ap_pdu.pdu.init_msg().value.bearer_context_setup_request()->gnb_cu_cp_ue_e1ap_id);

  return e1ap_pdu;
}

bool cu_cp_test_environment::send_pdu_session_resource_setup_request_and_await_bearer_context_modification_request(
    const ngap_message& pdu_session_resource_setup_request,
    unsigned            cu_up_idx)
{
  e1ap_message e1ap_pdu;
  ocudu_assert(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
               "there are still E1AP messages to pop from CU-UP");

  // Inject PDU Session Resource Setup Request and wait for Bearer Context Setup Request.
  get_amf().push_tx_pdu(pdu_session_resource_setup_request);
  bool result = this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive Bearer Context Modification Request");
  report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                            "Invalid Bearer Context Modification Request");

  return true;
}

bool cu_cp_test_environment::send_bearer_context_setup_response_and_await_ue_context_modification_request(
    unsigned                                                        du_idx,
    unsigned                                                        cu_up_idx,
    gnb_du_ue_f1ap_id_t                                             du_ue_id,
    gnb_cu_up_ue_e1ap_id_t                                          cu_up_e1ap_id,
    const std::map<pdu_session_id_t, std::vector<drb_test_params>>& pdu_sessions_to_add,
    const std::vector<pdu_session_id_t>&                            pdu_sessions_to_fail)
{
  f1ap_message f1ap_pdu;
  ocudu_assert(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu), "there are still F1AP DL messages to pop from DU");

  auto& ue_ctx         = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));
  ue_ctx.cu_up_e1ap_id = cu_up_e1ap_id;

  // Inject Bearer Context Setup Response and wait for UE Context Modification Request.
  get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_setup_response(
      ue_ctx.cu_cp_e1ap_id.value(), ue_ctx.cu_up_e1ap_id.value(), pdu_sessions_to_add, pdu_sessions_to_fail));
  bool result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive UE Context Modification Request");
  report_fatal_error_if_not(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu),
                            "Invalid UE Context Modification Request");

  return true;
}

bool cu_cp_test_environment::send_bearer_context_modification_response_and_await_ue_context_modification_request(
    unsigned            du_idx,
    unsigned            cu_up_idx,
    gnb_du_ue_f1ap_id_t du_ue_id,
    pdu_session_id_t    psi,
    drb_id_t            drb_id,
    qos_flow_id_t       qfi)
{
  f1ap_message f1ap_pdu;
  ocudu_assert(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu), "there are still F1AP DL messages to pop from DU");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));

  // Inject Bearer Context Modification Response and wait for UE Context Modification Request.
  get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_response(
      ue_ctx.cu_cp_e1ap_id.value(), ue_ctx.cu_up_e1ap_id.value(), {{psi, {drb_test_params{drb_id, qfi}}}}, {}));
  bool result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive UE Context Modification Request");
  report_fatal_error_if_not(test_helpers::is_valid_ue_context_modification_request(f1ap_pdu),
                            "Invalid UE Context Modification Request");

  return true;
}

bool cu_cp_test_environment::send_ue_context_modification_response_and_await_bearer_context_modification_request(
    unsigned            du_idx,
    unsigned            cu_up_idx,
    gnb_du_ue_f1ap_id_t du_ue_id,
    rnti_t              crnti)
{
  e1ap_message e1ap_pdu;
  ocudu_assert(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
               "there are still E1AP messages to pop from CU-UP");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));

  // Inject UE Context Modification Response and wait for Bearer Context Modification Request.
  get_du(du_idx).push_ul_pdu(
      test_helpers::generate_ue_context_modification_response(du_ue_id, ue_ctx.cu_ue_id.value(), crnti));
  bool result = this->wait_for_e1ap_tx_pdu(cu_up_idx, e1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive Bearer Context Modification Request");
  report_fatal_error_if_not(test_helpers::is_valid_bearer_context_modification_request(e1ap_pdu),
                            "Invalid Bearer Context Modification Request");

  return true;
}

bool cu_cp_test_environment::send_bearer_context_modification_response_and_await_rrc_reconfiguration(
    unsigned                                           du_idx,
    unsigned                                           cu_up_idx,
    gnb_du_ue_f1ap_id_t                                du_ue_id,
    const std::map<pdu_session_id_t, drb_test_params>& pdu_sessions_to_add,
    const std::map<pdu_session_id_t, drb_id_t>&        pdu_sessions_to_modify,
    const std::optional<std::vector<srb_id_t>>&        expected_srbs_to_add_mod,
    const std::optional<std::vector<drb_id_t>>&        expected_drbs_to_add_mod,
    const std::vector<pdu_session_id_t>&               pdu_sessions_failed_to_modify)
{
  f1ap_message f1ap_pdu;
  ocudu_assert(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu), "there are still F1AP DL messages to pop from DU");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));

  // Inject E1AP Bearer Context Modification Response and wait for DL RRC Message (containing RRC Reconfiguration).
  get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_response(ue_ctx.cu_cp_e1ap_id.value(),
                                                                                 ue_ctx.cu_up_e1ap_id.value(),
                                                                                 pdu_sessions_to_add,
                                                                                 pdu_sessions_to_modify,
                                                                                 pdu_sessions_failed_to_modify));
  bool result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  report_fatal_error_if_not(result, "Failed to receive F1AP DL RRC Message (containing RRC Reconfiguration)");
  report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                            "Invalid DL RRC Message Transfer");
  {
    const byte_buffer& rrc_container = test_helpers::get_rrc_container(f1ap_pdu);
    report_fatal_error_if_not(
        test_helpers::is_valid_rrc_reconfiguration(
            test_helpers::extract_dl_dcch_msg(rrc_container), true, expected_srbs_to_add_mod, expected_drbs_to_add_mod),
        "Invalid RRC Reconfiguration");
  }

  return true;
}

bool cu_cp_test_environment::send_rrc_reconfiguration_complete_and_await_pdu_session_setup_response(
    unsigned                             du_idx,
    gnb_du_ue_f1ap_id_t                  du_ue_id,
    byte_buffer                          rrc_reconfiguration_complete,
    const std::vector<pdu_session_id_t>& expected_pdu_sessions_to_setup,
    const std::vector<pdu_session_id_t>& expected_pdu_sessions_failed_to_setup)
{
  ngap_message ngap_pdu;
  ocudu_assert(not this->get_amf().try_pop_rx_pdu(ngap_pdu), "there are still NGAP messages to pop from AMF");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));

  // Inject UL RRC Message (containing RRC Reconfiguration Complete) and wait for PDU Session Resource Setup Response.
  get_du(du_idx).push_ul_pdu(test_helpers::generate_ul_rrc_message_transfer(
      du_ue_id, ue_ctx.cu_ue_id.value(), srb_id_t::srb1, std::move(rrc_reconfiguration_complete)));
  bool result = this->wait_for_ngap_tx_pdu(ngap_pdu);
  report_fatal_error_if_not(result, "Failed to receive PDU Session Resource Setup Response");
  report_fatal_error_if_not(test_helpers::is_valid_pdu_session_resource_setup_response(ngap_pdu),
                            "Invalid PDU Session Resource Setup Response");
  report_fatal_error_if_not(test_helpers::is_expected_pdu_session_resource_setup_response(
                                ngap_pdu, expected_pdu_sessions_to_setup, expected_pdu_sessions_failed_to_setup),
                            "Unsuccessful PDU Session Resource Setup Response");

  return true;
}

bool cu_cp_test_environment::setup_pdu_session(unsigned                             du_idx,
                                               unsigned                             cu_up_idx,
                                               gnb_du_ue_f1ap_id_t                  du_ue_id,
                                               rnti_t                               crnti,
                                               gnb_cu_up_ue_e1ap_id_t               cu_up_e1ap_id,
                                               pdu_session_id_t                     psi,
                                               drb_id_t                             drb_id,
                                               qos_flow_id_t                        qfi,
                                               byte_buffer                          rrc_reconfiguration_complete,
                                               bool                                 is_initial_session,
                                               std::optional<security_indication_t> security_indication)
{
  ngap_message ngap_pdu;
  ocudu_assert(not this->get_amf().try_pop_rx_pdu(ngap_pdu), "there are still NGAP messages to pop from AMF");
  f1ap_message f1ap_pdu;
  ocudu_assert(not this->get_du(du_idx).try_pop_dl_pdu(f1ap_pdu), "there are still F1AP DL messages to pop from DU");
  e1ap_message e1ap_pdu;
  ocudu_assert(not this->get_cu_up(cu_up_idx).try_pop_rx_pdu(e1ap_pdu),
               "there are still E1AP messages to pop from CU-UP");

  auto& ue_ctx = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(du_ue_id));

  ngap_message pdu_session_resource_setup_request =
      generate_valid_pdu_session_resource_setup_request_message(ue_ctx.amf_ue_id.value(),
                                                                ue_ctx.ran_ue_id.value(),
                                                                {{psi, {pdu_session_type_t::ipv4, {{qfi, 9}}}}},
                                                                security_indication);

  if (is_initial_session) {
    // Inject PDU Session Resource Setup Request and wait for Bearer Context Setup Request.
    if (not send_pdu_session_resource_setup_request_and_await_bearer_context_setup_request(
            pdu_session_resource_setup_request, du_idx, cu_up_idx, du_ue_id)) {
      return false;
    }

    // Inject Bearer Context Setup Response and wait for F1AP UE Context Modification Request.
    if (not send_bearer_context_setup_response_and_await_ue_context_modification_request(
            du_idx, cu_up_idx, du_ue_id, cu_up_e1ap_id, {{psi, {drb_test_params{drb_id_t::drb1, qfi}}}}, {})) {
      return false;
    }
  } else {
    // Inject PDU Session Resource Setup Request and wait for Bearer Context Modification Request.
    if (not send_pdu_session_resource_setup_request_and_await_bearer_context_modification_request(
            pdu_session_resource_setup_request, du_idx)) {
      return false;
    }

    // Inject Bearer Context Modification Response and wait for F1AP UE Context Modification Request.
    if (not send_bearer_context_modification_response_and_await_ue_context_modification_request(
            du_idx, cu_up_idx, du_ue_id, psi, drb_id, qfi)) {
      return false;
    }
  }

  // Inject UE Context Modification Response and wait for Bearer Context Modification to be sent to CU-UP.
  if (not send_ue_context_modification_response_and_await_bearer_context_modification_request(
          du_idx, cu_up_idx, du_ue_id, crnti)) {
    return false;
  }

  // Inject Bearer Context Modification Response and wait for DL RRC Message (containing RRC Reconfiguration).
  if (not send_bearer_context_modification_response_and_await_rrc_reconfiguration(
          du_idx, cu_up_idx, du_ue_id, {}, {{psi, drb_id}})) {
    return false;
  }

  // Inject RRC Reconfiguration Complete and wait for PDU Session Resource Setup Response to be sent to AMF.
  if (not send_rrc_reconfiguration_complete_and_await_pdu_session_setup_response(
          du_idx, du_ue_id, std::move(rrc_reconfiguration_complete), {psi}, {})) {
    return false;
  }

  return true;
}

bool cu_cp_test_environment::attach_ue(
    unsigned                                                   du_idx,
    unsigned                                                   cu_up_idx,
    gnb_du_ue_f1ap_id_t                                        du_ue_id,
    rnti_t                                                     crnti,
    amf_ue_id_t                                                amf_ue_id,
    gnb_cu_up_ue_e1ap_id_t                                     cu_up_e1ap_id,
    pdu_session_id_t                                           psi,
    drb_id_t                                                   drb_id,
    qos_flow_id_t                                              qfi,
    byte_buffer                                                rrc_setup_complete,
    byte_buffer                                                rrc_reconfiguration_complete,
    std::optional<cu_cp_core_network_assist_info_for_inactive> cn_assist_info_for_inactive,
    bool                                                       rrc_inactive_supported,
    std::optional<location_report_request>                     location_reporting_request,
    std::optional<security_indication_t>                       security_indication)
{
  if (not connect_new_ue(du_idx, du_ue_id, crnti, ue_plmn, std::move(rrc_setup_complete))) {
    return false;
  }
  if (not authenticate_ue(du_idx, du_ue_id, amf_ue_id)) {
    return false;
  }
  if (not setup_ue_security_and_ue_capabilities(du_idx,
                                                du_ue_id,
                                                std::move(cn_assist_info_for_inactive),
                                                rrc_inactive_supported,
                                                std::move(location_reporting_request))) {
    return false;
  }

  get_location_report_if_required(location_reporting_request);

  if (not finish_ue_registration(du_idx, cu_up_idx, du_ue_id)) {
    return false;
  }
  if (not request_pdu_session_resource_setup(du_idx, cu_up_idx, du_ue_id)) {
    return false;
  }
  if (not setup_pdu_session(du_idx,
                            cu_up_idx,
                            du_ue_id,
                            crnti,
                            cu_up_e1ap_id,
                            psi,
                            drb_id,
                            qfi,
                            std::move(rrc_reconfiguration_complete),
                            true)) {
    return false;
  }

  return true;
}

bool cu_cp_test_environment::reestablish_ue(unsigned            du_idx,
                                            unsigned            cu_up_idx,
                                            gnb_du_ue_f1ap_id_t new_du_ue_id,
                                            rnti_t              new_crnti,
                                            rnti_t              old_crnti,
                                            pci_t               old_pci)
{
  f1ap_message f1ap_pdu;

  // Send Initial UL RRC Message (containing RRC Reestablishment Request) to CU-CP.
  byte_buffer rrc_container = test_helpers::pack_ul_ccch_msg(
      test_helpers::create_rrc_reestablishment_request(old_crnti, old_pci, "1111010001000010"));
  f1ap_message f1ap_init_ul_rrc_msg = test_helpers::generate_init_ul_rrc_message_transfer(
      new_du_ue_id, new_crnti, ue_plmn, {}, std::move(rrc_container));
  get_du(du_idx).push_ul_pdu(f1ap_init_ul_rrc_msg);

  // Wait for DL RRC message transfer (with RRC Reestablishment / RRC Setup / RRC Reject).
  bool result = this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu);
  report_fatal_error_if_not(result, "F1AP DL RRC Message Transfer with Msg4 not sent to DU");
  report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer_with_msg4(f1ap_pdu), "Invalid Msg4");

  auto& dl_rrc_msg = *f1ap_pdu.pdu.init_msg().value.dl_rrc_msg_transfer();
  report_fatal_error_if_not(int_to_gnb_du_ue_f1ap_id(dl_rrc_msg.gnb_du_ue_f1ap_id) == new_du_ue_id,
                            "Invalid gNB-DU-UE-F1AP-ID");

  if (dl_rrc_msg.srb_id == 0) {
    // RRC Setup / RRC Reject.

    // Send RRC Setup Complete.
    // > Generate UL DCCH message (containing RRC Setup Complete).
    byte_buffer pdu = test_helpers::pack_ul_dcch_msg(test_helpers::create_rrc_setup_complete());
    // > Generate UL RRC Message (containing RRC Setup Complete) with PDCP SN=0.
    get_du(du_idx).push_rrc_ul_dcch_message(new_du_ue_id, srb_id_t::srb1, std::move(pdu));

    // CU-CP should send an NGAP Initial UE Message.
    ngap_message ngap_pdu;
    result = this->wait_for_ngap_tx_pdu(ngap_pdu);
    report_fatal_error_if_not(result, "Failed to send NGAP Initial UE Message");
    report_fatal_error_if_not(test_helpers::is_valid_init_ue_message(ngap_pdu), "Invalid init UE message");

    ue_context ue_ctx{};
    ue_ctx.crnti     = new_crnti;
    ue_ctx.du_ue_id  = new_du_ue_id;
    ue_ctx.cu_ue_id  = int_to_gnb_cu_ue_f1ap_id(dl_rrc_msg.gnb_cu_ue_f1ap_id);
    ue_ctx.ran_ue_id = uint_to_ran_ue_id(ngap_pdu.pdu.init_msg().value.init_ue_msg()->ran_ue_ngap_id);

    report_fatal_error_if_not(attached_ues.insert(std::make_pair(ue_ctx.ran_ue_id.value(), ue_ctx)).second,
                              "UE already exists");
    report_fatal_error_if_not(
        du_ue_id_to_ran_ue_id_map[du_idx].insert(std::make_pair(new_du_ue_id, ue_ctx.ran_ue_id.value())).second,
        "DU UE ID already exists");

    return false;
  }

  gnb_du_ue_f1ap_id_t old_du_ue_id = int_to_gnb_du_ue_f1ap_id(dl_rrc_msg.old_gnb_du_ue_f1ap_id);
  ue_context&         old_ue       = attached_ues.at(du_ue_id_to_ran_ue_id_map.at(du_idx).at(old_du_ue_id));
  old_ue.du_ue_id                  = new_du_ue_id;
  old_ue.cu_ue_id                  = int_to_gnb_cu_ue_f1ap_id(dl_rrc_msg.gnb_cu_ue_f1ap_id);
  old_ue.crnti                     = new_crnti;
  ran_ue_id_t ran_ue_id            = *old_ue.ran_ue_id;
  du_ue_id_to_ran_ue_id_map.at(du_idx).erase(old_du_ue_id);
  report_fatal_error_if_not(du_ue_id_to_ran_ue_id_map[du_idx].insert(std::make_pair(new_du_ue_id, ran_ue_id)).second,
                            "DU UE ID already exists");

  // EVENT: Send RRC Reestablishment Complete.
  // > Generate UL-DCCH message (containing RRC Reestablishment Complete).
  byte_buffer pdu = test_helpers::pack_ul_dcch_msg(test_helpers::create_rrc_reestablishment_complete());
  // > Prepend PDCP header and append MAC.
  report_error_if_not(pdu.prepend(std::array<uint8_t, 2>{0x00U, 0x00U}), "bad alloc");
  report_error_if_not(pdu.append(std::array<uint8_t, 4>{0x85, 0xc1, 0x04, 0xf1}), "bad alloc");
  // > Send UL RRC Message to CU-CP.
  get_du(du_idx).push_ul_pdu(
      test_helpers::generate_ul_rrc_message_transfer(new_du_ue_id, *old_ue.cu_ue_id, srb_id_t::srb1, std::move(pdu)));

  // STATUS: CU-CP sends E1AP Bearer Context Modification Request.
  e1ap_message e1ap_pdu;
  report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(0, e1ap_pdu), "E1AP BearerContextModificationRequest NOT sent");

  gnb_cu_cp_ue_e1ap_id_t cu_cp_e1ap_id =
      int_to_gnb_cu_cp_ue_e1ap_id(e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request()->gnb_cu_cp_ue_e1ap_id);
  gnb_cu_up_ue_e1ap_id_t cu_up_e1ap_id =
      int_to_gnb_cu_up_ue_e1ap_id(e1ap_pdu.pdu.init_msg().value.bearer_context_mod_request()->gnb_cu_up_ue_e1ap_id);

  // EVENT: Inject E1AP Bearer Context Modification Response.
  get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_response(cu_cp_e1ap_id, cu_up_e1ap_id));

  // STATUS: CU-CP sends F1AP UE Context Modification Request to DU.
  report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu), "F1AP UEContextModificationRequest NOT sent");
  report_fatal_error_if_not(f1ap_pdu.pdu.init_msg().value.ue_context_mod_request()->drbs_to_be_modified_list_present,
                            "UE Context Modification Request for RRC Reestablishment must contain DRBs to be modified");
  report_fatal_error_if_not(
      not f1ap_pdu.pdu.init_msg().value.ue_context_mod_request()->drbs_to_be_setup_mod_list_present,
      "UE Context Modification Request for RRC Reestablishment must not contain DRBs to be setup");

  // EVENT: Inject F1AP UE Context Modification Response.
  get_du(du_idx).push_ul_pdu(test_helpers::generate_ue_context_modification_response(
      new_du_ue_id, *this->find_ue_context(du_idx, new_du_ue_id)->cu_ue_id, new_crnti));

  // STATUS: CU-CP sends E1AP Bearer Context Modification Request.
  report_fatal_error_if_not(this->wait_for_e1ap_tx_pdu(0, e1ap_pdu), "E1AP BearerContextModificationRequest NOT sent");

  // EVENT: CU-UP sends E1AP Bearer Context Modification Response.
  get_cu_up(cu_up_idx).push_tx_pdu(generate_bearer_context_modification_response(cu_cp_e1ap_id, cu_up_e1ap_id));

  // STATUS: CU-CP sends F1AP DL RRC Message Transfer (containing RRC Reconfiguration).
  report_fatal_error_if_not(this->wait_for_f1ap_tx_pdu(du_idx, f1ap_pdu), "F1AP DL RRC Message NOT sent");
  report_fatal_error_if_not(test_helpers::is_valid_dl_rrc_message_transfer(f1ap_pdu),
                            "Invalid DL RRC Message Transfer");

  // EVENT: DU sends F1AP UL RRC Message Transfer (containing RRC Reconfiguration Complete).
  pdu = test_helpers::pack_ul_dcch_msg(test_helpers::create_rrc_reconfiguration_complete(1U));
  // > Prepend PDCP header and append MAC.
  report_error_if_not(pdu.prepend(std::array<uint8_t, 2>{0x00U, 0x01U}), "bad alloc");
  report_error_if_not(pdu.append(std::array<uint8_t, 4>{0xf1, 0x21, 0x02, 0x5e}), "bad alloc");
  get_du(du_idx).push_ul_pdu(
      test_helpers::generate_ul_rrc_message_transfer(new_du_ue_id, *old_ue.cu_ue_id, srb_id_t::srb1, std::move(pdu)));

  return true;
}
