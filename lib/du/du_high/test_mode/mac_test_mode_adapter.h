// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "mac_test_mode_decision_history.h"
#include "mac_test_mode_event_handler.h"
#include "ocudu/du/du_high/du_test_mode_config.h"
#include "ocudu/mac/mac.h"
#include "ocudu/mac/mac_cell_manager.h"
#include "ocudu/mac/mac_cell_result.h"
#include "ocudu/ocudulog/ocudulog.h"

namespace ocudu::odu {

/// Notifier used by MAC test mode adapter to notify the test mode controller about MAC events.
class mac_test_mode_event_notifier
{
public:
  virtual ~mac_test_mode_event_notifier() = default;

  /// Called from MAC when UE completes contention resolution.
  /// \note Called from cell thread (dispatches to ctrl_exec internally):
  virtual void on_con_res_completed(du_cell_index_t cell_index, rnti_t rnti) = 0;

  /// Called from MAC on slot completion.
  /// \note Called from cell thread (dispatches to ctrl_exec internally):
  virtual void on_slot_completed(du_cell_index_t cell_index, slot_point sl_tx) = 0;
};

class phy_test_mode_adapter : public mac_result_notifier
{
public:
  phy_test_mode_adapter(mac_result_notifier& phy_notifier_) : adapted_phy(phy_notifier_), cells(MAX_NOF_DU_CELLS) {}

  mac_cell_result_notifier& get_cell(du_cell_index_t cell_index) override;

  void connect(du_cell_index_t cell_index, mac_cell_result_notifier& test_mode_cell_notifier);

  void disconnect(du_cell_index_t cell_index);

  mac_result_notifier& adapted_phy;

private:
  class phy_cell final : public mac_cell_result_notifier
  {
  public:
    mac_cell_result_notifier* ptr = nullptr;

    void on_new_downlink_scheduler_results(const mac_dl_sched_result& dl_res) override;
    void on_new_downlink_data(const mac_dl_data_result& dl_data) override;
    void on_new_uplink_scheduler_results(const mac_ul_sched_result& ul_res) override;
    void on_cell_results_completion(slot_point slot) override;
  };

  std::vector<phy_cell> cells;
};

/// \brief Adapter of MAC cell for testing purposes. It automatically forces ACK/CRC=OK for the test UE.
class mac_test_mode_cell_adapter : public mac_cell_control_information_handler,
                                   public mac_cell_result_notifier,
                                   public mac_cell_slot_handler
{
public:
  mac_test_mode_cell_adapter(const odu::du_test_mode_config::test_mode_ue_config& test_ue_cfg_,
                             const mac_cell_creation_request&                     cell_cfg,
                             mac_cell_control_information_handler&                adapted_,
                             mac_pdu_handler&                                     pdu_handler_,
                             mac_cell_slot_handler&                               slot_handler_,
                             mac_cell_result_notifier&                            result_notifier_,
                             std::function<void(du_ue_index_t)>                   dl_bs_notifier_,
                             mac_test_mode_event_notifier&                        ev_notifier_,
                             mac_test_mode_event_handler&                         event_handler_);

  void on_new_downlink_scheduler_results(const mac_dl_sched_result& dl_res) override;

  void on_new_downlink_data(const mac_dl_data_result& dl_data) override
  {
    result_notifier.on_new_downlink_data(dl_data);
  }

  // Intercepts the calls coming from the real MAC.
  void on_new_uplink_scheduler_results(const mac_ul_sched_result& ul_res) override;
  void on_cell_results_completion(slot_point slot) override;

  void handle_slot_indication(const mac_cell_timing_context& context) override;
  void handle_error_indication(slot_point sl_tx, error_event event) override;
  void handle_stop_indication() override;

  void handle_crc(const mac_crc_indication_message& msg) override;

  void handle_uci(const mac_uci_indication_message& msg) override;

  void handle_srs(const mac_srs_indication_message& msg) override;

private:
  void forward_uci_ind_to_mac(const mac_uci_indication_message& uci_msg);
  void forward_crc_ind_to_mac(const mac_crc_indication_message& crc_msg);

  /// Returns true if the RNTI belongs to a test UE of this cell.
  bool is_test_ue(rnti_t rnti) const { return rnti >= test_rnti_start and rnti < test_rnti_end; }

  /// Reacts to the Msg4 of a test UE.
  void handle_msg4(const dl_msg_alloc& grant, slot_point sl_tx);

  const du_cell_index_t                           cell_index;
  const du_test_mode_config::test_mode_ue_config& test_ue_cfg;
  mac_cell_control_information_handler&           adapted;
  mac_pdu_handler&                                pdu_handler;
  mac_cell_slot_handler&                          slot_handler;
  mac_cell_result_notifier&                       result_notifier;
  /// First RNTI of the test UEs of this cell.
  const rnti_t test_rnti_start;
  /// RNTI after the last test UE of this cell.
  const rnti_t                       test_rnti_end;
  std::function<void(du_ue_index_t)> dl_bs_notifier;
  mac_test_mode_event_notifier&      ev_notifier;
  ocudulog::basic_logger&            logger;

  /// Ring buffer of slot decision history.
  mac_test_mode_cell_decision_history history;

  mac_test_mode_event_handler& event_handler;

  slot_point last_slot_ind;
};

/// MAC controller in test mode.
class mac_test_mode_adapter
{
public:
  explicit mac_test_mode_adapter(const du_test_mode_config::test_mode_ue_config& test_ue_cfg,
                                 mac_result_notifier&                            phy_notifier_,
                                 std::unique_ptr<mac_test_mode_event_notifier>   ev_notifier_,
                                 unsigned                                        nof_cells_);
  ~mac_test_mode_adapter();

  /// Gets notifier that will be intercepting the MAC results.
  mac_result_notifier& get_phy_notifier() const { return *phy_notifier; }

  /// Creates a wrapper of the real MAC that adapts its behavior in test mode.
  std::unique_ptr<mac_interface> decorate(std::unique_ptr<mac_interface> mac_ptr);

  /// Inject UL-CCCH messages, forcing the creation of a UE context in the DU-High.
  void inject_ul_ccch_msg(slot_point sl_rx, du_cell_index_t cell_index, rnti_t tc_rnti, byte_buffer ul_ccch_pdu);

private:
  class mac_wrapper;

  // adapted MAC methods.
  mac_cell_controller&               add_cell(const mac_cell_creation_request& cell_cfg);
  void                               remove_cell(du_cell_index_t cell_index);
  async_task<mac_ue_create_response> handle_ue_create_request(const mac_ue_create_request& cfg);
  async_task<mac_ue_reconfiguration_response>
                                     handle_ue_reconfiguration_request(const mac_ue_reconfiguration_request& cfg);
  async_task<mac_ue_delete_response> handle_ue_delete_request(const mac_ue_delete_request& cfg);
  void                               handle_dl_buffer_state_update(const mac_dl_buffer_state_indication_message& dl_bs);

  /// Returns true if the RNTI belongs to a test UE of any cell.
  bool is_test_ue(rnti_t rnti) const;

  /// Returns true if the UE index belongs to a test UE of any cell.
  bool is_test_ue(du_ue_index_t ue_index) const { return ue_index < test_ue.nof_ues * nof_cells; }

  std::vector<mac_logical_channel_config>
  adapt_bearers(const std::vector<mac_logical_channel_config>& orig_bearers) const;

  du_test_mode_config::test_mode_ue_config      test_ue;
  const unsigned                                nof_cells;
  std::unique_ptr<mac_test_mode_event_notifier> ev_notifier;

  std::unique_ptr<mac_interface> mac_adapted;

  mac_test_mode_event_handler event_handler;

  std::unique_ptr<phy_test_mode_adapter> phy_notifier;

  std::vector<std::unique_ptr<mac_test_mode_cell_adapter>> cell_info_handler;
};

} // namespace ocudu::odu
