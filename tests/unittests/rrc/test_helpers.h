// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/rrc/rrc_resume.h"
#include "ocudu/rrc/rrc_ue.h"

namespace ocudu::ocucp {

class dummy_rrc_f1ap_pdu_notifier : public rrc_pdu_f1ap_notifier
{
public:
  dummy_rrc_f1ap_pdu_notifier() = default;

  void on_new_rrc_pdu(const srb_id_t srb_id, const byte_buffer& pdu) override
  {
    last_rrc_pdu = pdu.copy();
    last_srb_id  = srb_id;
  }

  byte_buffer last_rrc_pdu;
  srb_id_t    last_srb_id;
};

class dummy_rrc_ue_ngap_adapter : public rrc_ue_ngap_notifier
{
public:
  void set_ue_context_release_outcome(bool outcome) { ue_context_release_outcome = outcome; }

  void on_initial_ue_message(const cu_cp_initial_ue_message& msg) override
  {
    logger.info("Received Initial UE Message");
    initial_ue_msg_received = true;
  }

  bool on_ul_nas_transport_message(const cu_cp_ul_nas_transport& msg) override
  {
    logger.info("Received UL NAS Transport message");
    return true;
  }

  bool initial_ue_msg_received = false;

private:
  bool                    ue_context_release_outcome = false;
  ocudulog::basic_logger& logger                     = ocudulog::fetch_basic_logger("TEST");
};

class dummy_rrc_ue_cu_cp_adapter : public rrc_ue_context_update_notifier, public rrc_ue_measurement_notifier
{
public:
  void add_ue_context(rrc_ue_reestablishment_context_response context) { reest_context = std::move(context); }

  bool                  next_ue_setup_response          = true;
  bool                  next_ue_setup_complete_response = true;
  amf_selection_request last_amf_selection_request;

  /// Outcome and payload the dummy answers a UE context retrieval with, plus the last request it saw.
  bool                                            context_retrieval_succeeds = false;
  bool                                            path_switch_requested      = false;
  bool                                            path_switch_succeeds       = true;
  security::security_context                      retrieved_sec_context;
  byte_buffer                                     retrieved_rrc_context;
  std::optional<rrc_ue_context_retrieval_request> last_context_retrieval_request;

  bool on_ue_setup_request(establishment_cause_t establishment_cause) override { return next_ue_setup_response; }

  bool on_ue_setup_complete_received(const amf_selection_request& request) override
  {
    last_amf_selection_request = request;
    return next_ue_setup_complete_response;
  }

  rrc_ue_reestablishment_context_response on_rrc_reestablishment_request(pci_t old_pci, rnti_t old_c_rnti) override
  {
    logger.info("old_pci={} old_c-rnti={}: Received RRC Reestablishment Request", old_pci, old_c_rnti);

    return reest_context;
  }

  async_task<rrc_ue_context_retrieval_response>
  on_ue_context_retrieval_required(const rrc_ue_context_retrieval_request& request) override
  {
    logger.info("Received UE Context Retrieval Required");
    last_context_retrieval_request = request;

    rrc_ue_context_retrieval_response response;
    response.success     = context_retrieval_succeeds;
    response.sec_context = retrieved_sec_context;
    response.rrc_context = retrieved_rrc_context.copy();

    return launch_async(
        [resp = std::move(response)](coro_context<async_task<rrc_ue_context_retrieval_response>>& ctx) mutable {
          CORO_BEGIN(ctx);
          CORO_RETURN(std::move(resp));
        });
  }

  async_task<bool> on_retrieved_context_path_switch_required() override
  {
    logger.info("Received Retrieved Context Path Switch Required");
    path_switch_requested = true;

    return launch_async([this](coro_context<async_task<bool>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN(path_switch_succeeds);
    });
  }

  async_task<bool> on_rrc_reestablishment_context_modification_required() override
  {
    logger.info("Received Reestablishment Context Modification Required");

    return launch_async([](coro_context<async_task<bool>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN(true);
    });
  }

  void on_rrc_reestablishment_failure(const cu_cp_ue_context_release_request& request) override
  {
    logger.info("ue={}: Received RRC Reestablishment failure notification", request.ue_index);
  }

  void on_rrc_reestablishment_complete(cu_cp_ue_index_t old_ue_index) override
  {
    logger.info("ue={}: Received RRC Reestablishment complete notification", old_ue_index);
  }

  async_task<bool> on_ue_transfer_required(cu_cp_ue_index_t old_ue_index) override
  {
    logger.info("Requested a UE context transfer from old_ue={}.", old_ue_index);
    return launch_async([](coro_context<async_task<bool>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN(true);
    });
  }

  async_task<void> on_ue_release_required(const cu_cp_ue_context_release_request& request) override
  {
    logger.info("ue={}: Requested a UE release", request.ue_index);
    last_cu_cp_ue_context_release_request = request;

    return launch_async([](coro_context<async_task<void>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN();
    });
  }

  void on_up_context_setup_required(const up_context& ctxt) override { logger.info("UP context setup requested"); }

  void on_ue_location_update() override
  {
    logger.info("UE location update reported");
    ++nof_ue_location_updates;
  }

  void on_location_measurement_indication() override
  {
    logger.info("Location measurement indication received");
    ++nof_location_meas_indications;
  }

  up_context on_up_context_required() override
  {
    logger.info("UP context requested");
    return up_ctxt;
  }

  async_task<void> on_ue_removal_required() override
  {
    logger.info("UE removal requested");
    return launch_async([](coro_context<async_task<void>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN();
    });
  }

  async_task<rrc_resume_request_response> on_rrc_resume_request(const rrc_resume_request& request) override
  {
    logger.info("ue={}: Received RRC Resume Request", request.ue_index);
    return launch_async([](coro_context<async_task<rrc_resume_request_response>>& ctx) mutable {
      CORO_BEGIN(ctx);
      CORO_RETURN(rrc_resume_request_response{.success = true});
    });
  }

  void on_ran_paging_required() override { logger.info("Received RAN paging required notification"); }

  std::optional<rrc_meas_cfg>
  on_measurement_config_request(nr_cell_identity                   nci,
                                const std::optional<rrc_meas_cfg>& current_meas_config = std::nullopt,
                                bool                               cond_meas           = false,
                                span<const pci_t>                  candidate_pcis      = {}) override
  {
    return next_meas_cfg;
  }

  void on_measurement_report(const rrc_meas_results& meas_results) override {}

  /// If set, on_measurement_config_request() returns this value; allows tests to inject a real config.
  std::optional<rrc_meas_cfg> next_meas_cfg;

  /// Returned by on_up_context_required(); allows tests to inject the UE's active DRBs.
  up_context up_ctxt;

  cu_cp_ue_context_release_request last_cu_cp_ue_context_release_request;

  /// Counts the coarse UE location updates reported to the CU-CP.
  unsigned nof_ue_location_updates = 0;
  /// Number of location measurement starts and stops the RRC UE notified.
  unsigned nof_location_meas_indications = 0;

private:
  rrc_ue_reestablishment_context_response reest_context = {};
  ocudulog::basic_logger&                 logger        = ocudulog::fetch_basic_logger("TEST");
};

class dummy_rrc_ue_rrc_du_adapter : public rrc_ue_event_notifier
{
public:
  void on_new_rrc_connection() override { ++nof_new_rrc_connections; }

  void on_successful_rrc_connection_establishment(establishment_cause_t     cause,
                                                  std::chrono::milliseconds setup_time) override
  {
  }

  void on_failed_rrc_connection_establishment(establishment_fail_cause_t cause) override
  {
    ++nof_failed_rrc_connection_establishments;
    last_establishment_fail_cause = cause;
  }

  void on_successful_rrc_connection_reestablishment() override {}

  void on_successful_rrc_connection_reestablishment_fallback() override {}

  void on_attempted_rrc_connection_reestablishment_followed_by_rrc_setup() override {}

  void on_successful_rrc_connection_resume(resume_cause_t cause) override {}

  void on_successful_rrc_connection_resume_with_fallback(resume_cause_t cause) override {}

  void on_rrc_connection_resume_followed_by_network_release(resume_cause_t cause) override {}

  void on_attempted_rrc_connection_resume_followed_by_rrc_setup(resume_cause_t cause) override {}

  /// Counts the new RRC connections the RRC UE reported.
  unsigned nof_new_rrc_connections = 0;

  /// Counts the failed RRC connection establishments the RRC UE reported.
  unsigned                   nof_failed_rrc_connection_establishments = 0;
  establishment_fail_cause_t last_establishment_fail_cause            = establishment_fail_cause_t::other;
};

} // namespace ocudu::ocucp
