// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../common/test_helpers.h"
#include "tests/test_doubles/f1ap/f1c_test_local_gateway.h"
#include "ocudu/f1ap/cu_cp/f1ap_configuration.h"
#include "ocudu/f1ap/cu_cp/f1ap_cu.h"
#include "ocudu/f1ap/cu_cp/gnb_cu_ue_f1ap_id_allocator.h"
#include "ocudu/f1ap/f1ap_message_notifier.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/support/async/fifo_async_task_scheduler.h"
#include "ocudu/support/executors/manual_task_worker.h"
#include <gtest/gtest.h>
#include <unordered_map>

namespace ocudu::ocucp {

/// \brief Generate a random gnb_cu_ue_f1ap_id
gnb_cu_ue_f1ap_id_t generate_random_gnb_cu_ue_f1ap_id();

/// \brief Reusable F1-C gateway test class for CU-CP unit tests. This class includes:
/// a) Requests a new DU connection to the CU-CP.
/// b) Logs and stores the last transmitted/received PDU by/from the CU-CP.
class dummy_cu_cp_f1c_gateway
{
public:
  dummy_cu_cp_f1c_gateway() : logger(ocudulog::fetch_basic_logger("TEST")) {}

  void attach_cu_cp_du_repo(ocucp::cu_cp_f1c_handler& cu_cp_du_mng_)
  {
    local_f1c_gw.attach_cu_cp_du_repo(cu_cp_du_mng_);
  }

  f1ap_message_notifier* request_new_du_connection()
  {
    class sink_f1ap_message_notifier : public f1ap_message_notifier
    {
    public:
      void on_new_message(const f1ap_message& msg) override {}
    };

    auto notifier = local_f1c_gw.handle_du_connection_request(std::make_unique<sink_f1ap_message_notifier>());
    if (notifier != nullptr) {
      du_tx_notifiers.push_back(std::move(notifier));
      return du_tx_notifiers.back().get();
    }
    return nullptr;
  }

  f1ap_message_notifier& get_du(cu_cp_du_index_t du_idx) { return *du_tx_notifiers.at((unsigned)du_idx); }

  void remove_du_connection(size_t connection_idx) { du_tx_notifiers.erase(du_tx_notifiers.begin() + connection_idx); }

  span<const f1ap_message> last_rx_pdus(size_t connection_idx) const
  {
    return local_f1c_gw.get_last_cu_cp_rx_pdus(connection_idx);
  }
  span<const f1ap_message> last_tx_pdus(size_t connection_idx) const
  {
    return local_f1c_gw.get_last_cu_cp_tx_pdus(connection_idx);
  }

  void push_cu_cp_rx_pdu(size_t du_connectin_idx, const f1ap_message& msg)
  {
    du_tx_notifiers[du_connectin_idx]->on_new_message(msg);
  }

  size_t nof_connections() const { return du_tx_notifiers.size(); }

private:
  ocudulog::basic_logger& logger;
  f1c_test_local_gateway  local_f1c_gw;

  std::vector<std::unique_ptr<f1ap_message_notifier>> du_tx_notifiers;
};

class dummy_f1ap_ul_ccch_message_notifier : public ocucp::f1ap_ul_ccch_notifier
{
public:
  dummy_f1ap_ul_ccch_message_notifier() = default;
  void on_ul_ccch_pdu(byte_buffer pdu, rnti_t c_rnti) override
  {
    logger.info("Received UL CCCH RRC message");
    last_ul_ccch_pdu = std::move(pdu);
  }

  byte_buffer last_ul_ccch_pdu;

private:
  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("TEST");
};

class dummy_f1ap_ul_dcch_message_notifier : public ocucp::f1ap_ul_dcch_notifier
{
public:
  dummy_f1ap_ul_dcch_message_notifier() = default;
  void on_ul_dcch_pdu(byte_buffer pdu) override
  {
    logger.info("Received UL DCCH RRC message");
    last_ul_dcch_pdu = std::move(pdu);
  }

  byte_buffer last_ul_dcch_pdu;

private:
  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("TEST");
};

class dummy_f1ap_du_processor_notifier : public ocucp::f1ap_du_processor_notifier
{
public:
  dummy_f1ap_du_processor_notifier(const unsigned max_nof_supported_ues_) :
    max_nof_supported_ues(max_nof_supported_ues_), logger(ocudulog::fetch_basic_logger("TEST"))
  {
  }

  void connect_f1ap(f1ap_cu* f1ap_) { f1ap = f1ap_; }

  du_setup_result on_new_du_setup_request(const du_setup_request& msg) override
  {
    logger.info("Received F1SetupRequest");
    last_f1_setup_request_msg = msg;
    return next_du_setup_resp;
  }

  du_config_update_result on_new_du_config_update(const du_config_update_request& msg) override
  {
    du_config_update_result res;
    res.result.emplace<du_config_update_result::accepted>();
    return res;
  }

  cu_cp_ue_index_t request_new_ue_creation() override
  {
    logger.info("Received request to create new UE");
    return on_new_cu_cp_ue_required();
  }

  ocucp::ue_rrc_context_creation_outcome
  on_ue_rrc_context_creation_request(const ue_rrc_context_creation_request& msg) override
  {
    logger.info("Received {}", __FUNCTION__);
    f1ap_ue_context_release_command release_cmd = {.ue_index = msg.ue_index,
                                                   .cause    = f1ap_cause_radio_network_t::no_radio_res_available,
                                                   .rrc_pdu  = byte_buffer::create({0x0, 0x0}).value(),
                                                   .srb_id   = srb_id_t::srb0};

    if (to_underlying(msg.ue_index) >= max_nof_supported_ues) {
      logger.warning("UE creation failed. Maximum number of supported UEs ({}) exceeded", max_nof_supported_ues);

      if (f1ap != nullptr) {
        task_sched.schedule(launch_async([this, release_cmd](coro_context<async_task<void>>& ctx) {
          CORO_BEGIN(ctx);
          CORO_AWAIT(f1ap->handle_ue_context_release_command(release_cmd));
          f1ap->remove_ue_context(release_cmd.ue_index);
          CORO_RETURN();
        }));
      }
      return make_unexpected(default_error_t{});
    }

    last_ue_creation_msg.ue_index               = msg.ue_index;
    last_ue_creation_msg.cgi                    = msg.cgi;
    last_ue_creation_msg.du_to_cu_rrc_container = msg.du_to_cu_rrc_container.copy();
    last_ue_creation_msg.c_rnti                 = msg.c_rnti;
    last_ue_creation_msg.is_redcap              = msg.is_redcap;

    ocucp::ue_rrc_context_creation_response response{
        msg.ue_index, f1ap_srb0_notifier.get(), f1ap_srb1_notifier.get(), f1ap_srb2_notifier.get()};
    if (msg.ue_index == cu_cp_ue_index_t::invalid) {
      response.ue_index = on_new_cu_cp_ue_required();
    }

    // Return failure if no UE index is available.
    if (response.ue_index == cu_cp_ue_index_t::invalid) {
      if (f1ap != nullptr) {
        task_sched.schedule(launch_async([this, release_cmd](coro_context<async_task<void>>& ctx) {
          CORO_BEGIN(ctx);
          CORO_AWAIT(f1ap->handle_ue_context_release_command(release_cmd));
          f1ap->remove_ue_context(release_cmd.ue_index);
          CORO_RETURN();
        }));
      }
      return make_unexpected(default_error_t{});
    }

    return response;
  }

  cu_cp_ue_index_t on_new_cu_cp_ue_required()
  {
    cu_cp_ue_index_t ue_index = cu_cp_ue_index_t::invalid;
    if (ue_id < static_cast<unsigned>(cu_cp_ue_index_t::invalid)) {
      ue_index              = uint_to_ue_index(ue_id);
      last_created_ue_index = ue_index;
      ue_id++;
    }

    return ue_index;
  }

  void on_du_initiated_ue_context_release_request(const ocucp::f1ap_ue_context_release_request& req) override
  {
    logger.info("Received UEContextReleaseRequest");
    // TODO
  }

  void on_access_success(const ocucp::f1ap_access_success& msg) override
  {
    logger.info("Received AccessSuccess for ue={}", msg.ue_index);
    last_access_success_msg = msg;
  }

  bool schedule_async_task(async_task<void> task) override { return task_sched.schedule(std::move(task)); }

  async_task<void> on_transaction_info_loss(const ue_transaction_info_loss_event& ev) override
  {
    return launch_async([](coro_context<async_task<void>>& ctx) {
      CORO_BEGIN(ctx);
      CORO_RETURN();
    });
  }

  void on_e_cid_measurement_report(const e_cid_measurement_report_t& report) override
  {
    last_e_cid_measurement_report = report;
  }

  void on_e_cid_measurement_failure_indication(const e_cid_measurement_failure_indication_t& ind) override
  {
    last_e_cid_measurement_failure_indication = ind;
  }

  std::optional<e_cid_measurement_report_t>             last_e_cid_measurement_report;
  std::optional<e_cid_measurement_failure_indication_t> last_e_cid_measurement_failure_indication;

  void on_ref_time_info_report(const f1ap_time_ref_info& info) override { last_ref_time_info_report = info; }

  void set_ue_id(uint16_t ue_id_) { ue_id = ue_id_; }

  ocucp::du_setup_request last_f1_setup_request_msg;
  ocucp::du_setup_result  next_du_setup_resp;

  std::optional<ocucp::f1ap_access_success>            last_access_success_msg;
  std::optional<f1ap_time_ref_info>                    last_ref_time_info_report;
  ocucp::ue_rrc_context_creation_request               last_ue_creation_msg;
  std::optional<cu_cp_ue_index_t>                      last_created_ue_index;
  std::unique_ptr<dummy_f1ap_ul_ccch_message_notifier> f1ap_srb0_notifier =
      std::make_unique<dummy_f1ap_ul_ccch_message_notifier>();
  std::unique_ptr<dummy_f1ap_ul_dcch_message_notifier> f1ap_srb1_notifier =
      std::make_unique<dummy_f1ap_ul_dcch_message_notifier>();
  std::unique_ptr<dummy_f1ap_ul_dcch_message_notifier> f1ap_srb2_notifier =
      std::make_unique<dummy_f1ap_ul_dcch_message_notifier>();

private:
  const unsigned            max_nof_supported_ues;
  f1ap_cu*                  f1ap = nullptr;
  ocudulog::basic_logger&   logger;
  unsigned                  ue_id = to_underlying(cu_cp_ue_index_t::min);
  fifo_async_task_scheduler task_sched{16};
};

/// \brief Creates a dummy UE CONTEXT SETUP REQUEST.
f1ap_ue_context_setup_request create_ue_context_setup_request(const std::initializer_list<drb_id_t>& drbs_to_add);

/// \brief Creates a dummy GNB-CU CONFIGURATION UPDATE.
f1ap_gnb_cu_configuration_update create_gnb_cu_configuration_update();

/// Fixture class for F1AP
class f1ap_cu_test : public ::testing::Test
{
protected:
  struct test_ue {
    cu_cp_ue_index_t                   ue_index;
    std::optional<gnb_cu_ue_f1ap_id_t> cu_ue_id;
    std::optional<gnb_du_ue_f1ap_id_t> du_ue_id;
  };

  f1ap_cu_test(const f1ap_configuration& f1ap_cfg = {});
  ~f1ap_cu_test() override;

  /// \brief Helper method to successfully create UE instance in F1AP.
  test_ue& create_ue(gnb_du_ue_f1ap_id_t du_ue_id);

  /// \brief Helper method to run F1AP CU UE Context Setup procedure to completion for a given UE.
  test_ue& run_ue_context_setup();

  bool was_rrc_reject_sent();

  void tick();

  ocudulog::basic_logger& f1ap_logger = ocudulog::fetch_basic_logger("CU-CP-F1");
  ocudulog::basic_logger& test_logger = ocudulog::fetch_basic_logger("TEST");

  std::unordered_map<cu_cp_ue_index_t, test_ue> test_ues;

  const unsigned max_nof_ues = 8192;

  dummy_f1ap_pdu_notifier          f1ap_pdu_notifier;
  dummy_f1ap_du_processor_notifier du_processor_notifier{max_nof_ues};
  gnb_cu_ue_f1ap_id_allocator      cu_ue_id_alloc;
  timer_manager                    timers;
  manual_task_worker               ctrl_worker{128};
  std::unique_ptr<f1ap_cu>         f1ap;
};

} // namespace ocudu::ocucp
