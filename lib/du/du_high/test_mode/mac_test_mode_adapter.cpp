// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "mac_test_mode_adapter.h"
#include "du_test_mode_controller.h"
#include "mac_test_mode_helpers.h"
#include "ocudu/adt/ring_buffer.h"
#include "ocudu/mac/mac_cell_timing_context.h"
#include "ocudu/mac/mac_factory.h"
#include "ocudu/ran/harq_id.h"
#include <functional>
#include <utility>

using namespace ocudu;
using namespace odu;

/// Buffer state that the fake RLC will report to the MAC. This value should be large enough for the scheduler to fill
/// the largest TB possible.
static constexpr unsigned TEST_UE_DL_BUFFER_STATE_UPDATE_SIZE = 10000000;

namespace {

/// \brief Adapter for the MAC SDU TX builder that auto fills the DL buffer state update.
class test_ue_mac_sdu_tx_builder_adapter : public mac_sdu_tx_builder
{
  static constexpr unsigned TX_SDU_MAX_SIZE = 5000;

public:
  test_ue_mac_sdu_tx_builder_adapter()
  {
    report_fatal_error_if_not(tx_sdu.append(std::vector<uint8_t>(TX_SDU_MAX_SIZE, 0)),
                              "Unable to allocate byte buffer");
  }

  size_t on_new_tx_sdu(span<uint8_t> mac_sdu_buf) override
  {
    // Return empty MAC SDU so that the MAC PDU is padded.
    return 0;
  }

  rlc_buffer_state on_buffer_state_update() override
  {
    rlc_buffer_state bs = {};
    bs.pending_bytes    = TEST_UE_DL_BUFFER_STATE_UPDATE_SIZE;
    // TODO: set bs.hol_toa
    return bs;
  }

private:
  byte_buffer tx_sdu;
};

} // namespace

class mac_test_mode_adapter::mac_wrapper final : public mac_interface,
                                                 public mac_ue_control_information_handler,
                                                 public mac_cell_manager,
                                                 public mac_ue_configurator,
                                                 public mac_cell_rach_handler
{
public:
  mac_wrapper(mac_test_mode_adapter& parent_) : parent(parent_) {}

  // -- mac_cell_rach_handler interface --
  mac_cell_rach_handler& get_rach_handler(du_cell_index_t cell_index) override { return *this; }
  void                   handle_rach_indication(const mac_rach_indication& rach_ind) override
  {
    // Ignores all RACH indications coming from the PHY. All UEs will be created by the test mode controller.
  }
  std::optional<rnti_t>
  handle_msga_ccch_sdu(rnti_t ra_rnti, uint8_t rapid, slot_point sl_rx, const ue_con_res_id_t& con_res_id) override
  {
    return std::nullopt;
  }
  std::optional<ue_con_res_id_t> resolve_msga_con_res_id(rnti_t tc_rnti) override { return std::nullopt; }

  // -- mac_cell_manager interface --
  mac_cell_manager&    get_cell_manager() override { return *this; }
  mac_cell_controller& add_cell(const mac_cell_creation_request& cell_cfg) override
  {
    return parent.add_cell(cell_cfg);
  }
  void                 remove_cell(du_cell_index_t cell_index) override { return parent.remove_cell(cell_index); }
  mac_cell_controller& get_cell_controller(du_cell_index_t cell_index) override
  {
    return parent.mac_adapted->get_cell_manager().get_cell_controller(cell_index);
  }
  mac_subframe_time_mapper& get_subframe_time_mapper() override
  {
    // bypasses MAC adapter.
    return parent.mac_adapted->get_subframe_time_mapper();
  }

  // -- mac_ue_configurator interface --
  mac_ue_configurator&               get_ue_configurator() override { return *this; }
  async_task<mac_ue_create_response> handle_ue_create_request(const mac_ue_create_request& cfg) override
  {
    return parent.handle_ue_create_request(cfg);
  }
  async_task<mac_ue_reconfiguration_response>
  handle_ue_reconfiguration_request(const mac_ue_reconfiguration_request& cfg) override
  {
    return parent.handle_ue_reconfiguration_request(cfg);
  }
  async_task<mac_ue_delete_response> handle_ue_delete_request(const mac_ue_delete_request& cfg) override
  {
    return parent.handle_ue_delete_request(cfg);
  }
  bool handle_ul_ccch_msg(du_ue_index_t    ue_index,
                          du_cell_index_t  cell_index,
                          slot_point       slot_rx,
                          byte_buffer      ul_ccch_msg,
                          msg3_mac_ce_list mac_ces) override
  {
    // bypasses MAC adapter.
    return parent.mac_adapted->get_ue_configurator().handle_ul_ccch_msg(
        ue_index, cell_index, slot_rx, std::move(ul_ccch_msg), std::move(mac_ces));
  }
  void handle_ue_config_applied(du_ue_index_t ue_index) override
  {
    // bypasses MAC adapter.
    parent.mac_adapted->get_ue_configurator().handle_ue_config_applied(ue_index);
  }

  mac_cell_control_information_handler& get_control_info_handler(du_cell_index_t cell_index) override
  {
    return *parent.cell_info_handler[cell_index];
  }

  // -- mac_ue_control_information_handler interface --
  mac_ue_control_information_handler& get_ue_control_info_handler() override { return *this; }
  void handle_dl_buffer_state_update(const mac_dl_buffer_state_indication_message& dl_bs) override
  {
    return parent.handle_dl_buffer_state_update(dl_bs);
  }

  mac_cell_slot_handler& get_slot_handler(du_cell_index_t cell_index) override
  {
    return *parent.cell_info_handler[cell_index];
  }
  mac_pdu_handler& get_pdu_handler() override
  {
    // bypasses MAC adapter.
    return parent.mac_adapted->get_pdu_handler();
  }
  mac_paging_information_handler& get_cell_paging_info_handler() override
  {
    // bypasses MAC adapter.
    return parent.mac_adapted->get_cell_paging_info_handler();
  }
  mac_positioning_measurement_handler& get_positioning_handler() override
  {
    // bypasses MAC adapter.
    return parent.mac_adapted->get_positioning_handler();
  }

private:
  mac_test_mode_adapter& parent;
};

mac_test_mode_cell_adapter::mac_test_mode_cell_adapter(const du_test_mode_config::test_mode_ue_config& test_ue_cfg_,
                                                       const mac_cell_creation_request&                cell_cfg,
                                                       mac_cell_control_information_handler&           adapted_,
                                                       mac_pdu_handler&                                pdu_handler_,
                                                       mac_cell_slot_handler&                          slot_handler_,
                                                       mac_cell_result_notifier&                       result_notifier_,
                                                       std::function<void(du_ue_index_t)>              dl_bs_notifier_,
                                                       mac_test_mode_event_notifier&                   ev_notifier_,
                                                       mac_test_mode_event_handler&                    event_handler_) :
  cell_index(cell_cfg.cell_index),
  test_ue_cfg(test_ue_cfg_),
  adapted(adapted_),
  pdu_handler(pdu_handler_),
  slot_handler(slot_handler_),
  result_notifier(result_notifier_),
  test_rnti_start(to_rnti(static_cast<unsigned>(test_ue_cfg_.rnti) + cell_cfg.cell_index * test_ue_cfg_.nof_ues)),
  test_rnti_end(to_rnti(static_cast<unsigned>(test_rnti_start) + test_ue_cfg_.nof_ues)),
  dl_bs_notifier(std::move(dl_bs_notifier_)),
  ev_notifier(ev_notifier_),
  logger(ocudulog::fetch_basic_logger("MAC")),
  history(cell_cfg),
  event_handler(event_handler_)
{
}

void mac_test_mode_cell_adapter::handle_slot_indication(const mac_cell_timing_context& context)
{
  if (test_ue_cfg.auto_ack_indication_delay.has_value()) {
    // auto-generation of CRC/UCI indication is enabled.
    slot_point sl_rx = context.sl_tx.without_hyper_sfn() - test_ue_cfg.auto_ack_indication_delay.value();
    const auto entry = history.read(sl_rx);
    if (entry != nullptr) {
      // Handle auto-generation of pending CRC indications.
      if (not entry->puschs.empty()) {
        mac_crc_indication_message crc_ind{};
        crc_ind.sl_rx = sl_rx;

        for (const ul_sched_info& pusch : entry->puschs) {
          auto& crc_pdu   = crc_ind.crcs.emplace_back();
          crc_pdu.rnti    = pusch.pusch_cfg.rnti;
          crc_pdu.harq_id = pusch.pusch_cfg.harq_id;
          // Force CRC=OK for test UE.
          crc_pdu.tb_crc_success = true;
          // Force UL SINR.
          crc_pdu.ul_sinr_dB = 100;
        }

        // Forward CRC to the real MAC.
        forward_crc_ind_to_mac(crc_ind);
      }

      // Handle auto-generation of pending UCI indications.
      mac_uci_indication_message uci_ind;
      uci_ind.sl_rx = sl_rx;

      // > Handle pending PUCCHs.
      for (const pucch_info& pucch : entry->pucchs) {
        uci_ind.ucis.emplace_back(create_uci_pdu(pucch, test_ue_cfg));
      }

      // > Handle pending PUSCHs.
      for (const ul_sched_info& pusch : entry->puschs) {
        if (pusch.uci.has_value()) {
          uci_ind.ucis.emplace_back(create_uci_pdu(pusch, test_ue_cfg));
        }
      }

      // Forward UCI indication to real MAC.
      forward_uci_ind_to_mac(uci_ind);
    } else {
      logger.warning("TEST_MODE: Failed to generate UCI and CRC for slot={}. Cause: Overflow of the test mode internal "
                     "storage detected",
                     sl_rx);
    }
  }

  slot_handler.handle_slot_indication(context);
}

void mac_test_mode_cell_adapter::handle_error_indication(slot_point sl_tx, error_event event)
{
  slot_handler.handle_error_indication(sl_tx, event);

  if (event.pusch_and_pucch_discarded) {
    // Dispatch clearing of the history to the slot ind executor.
    if (not event_handler.schedule(cell_index, [this, sl_tx]() { history.clear(sl_tx); })) {
      logger.warning("TEST_MODE: Failed to handle error indication for slot={}. Cause: Overflow of the test mode "
                     "internal storage detected",
                     sl_tx);
    }
  }
}

void mac_test_mode_cell_adapter::handle_stop_indication()
{
  slot_handler.handle_stop_indication();
}

void mac_test_mode_cell_adapter::handle_crc(const mac_crc_indication_message& msg)
{
  mac_crc_indication_message msg_copy = msg;

  if (not test_ue_cfg.auto_ack_indication_delay.has_value()) {
    // Acquire history.
    const auto entry = history.read(msg.sl_rx);
    if (entry != nullptr) {
      // Forward CRC to MAC, but remove the UCI for the test mode UE.
      for (mac_crc_pdu& crc : msg_copy.crcs) {
        if (is_test_ue(crc.rnti)) {
          // test mode UE case.

          // Find respective PUSCH PDU that was previously scheduled.
          bool found = std::any_of(entry->puschs.begin(), entry->puschs.end(), [&](const ul_sched_info& pusch) {
            return pusch.pusch_cfg.rnti == crc.rnti and pusch.pusch_cfg.harq_id == crc.harq_id;
          });
          if (not found) {
            logger.warning("TEST_MODE c-rnti={}: Failed to set CRC value for slot={}. Cause: Mismatch between provided "
                           "CRC and expected PUSCH",
                           crc.rnti,
                           msg.sl_rx);
            continue;
          }

          // Intercept the CRC indication and force crc=OK and UL SNR.
          crc.tb_crc_success = true;
          crc.ul_sinr_dB     = 100;
        }
      }
    } else {
      // Failed to lock entry in ring.
      logger.warning("TEST_MODE: Unable to set CRC value for slot={}. Cause: Overflow detected in test "
                     "mode internal storage",
                     msg.sl_rx);
    }
  } else {
    // In case of auto-ACK mode, test mode UEs are removed from CRC.
    msg_copy.crcs.erase(std::remove_if(msg_copy.crcs.begin(),
                                       msg_copy.crcs.end(),
                                       [this](const auto& crc) { return is_test_ue(crc.rnti); }),
                        msg_copy.crcs.end());
  }

  // Forward resulting CRC indication to real MAC.
  forward_crc_ind_to_mac(msg_copy);
}

void mac_test_mode_cell_adapter::forward_uci_ind_to_mac(const mac_uci_indication_message& uci_msg)
{
  if (uci_msg.ucis.empty()) {
    return;
  }

  // Forward UCI indication to real MAC.
  adapted.handle_uci(uci_msg);
}

void mac_test_mode_cell_adapter::forward_crc_ind_to_mac(const mac_crc_indication_message& crc_msg)
{
  if (crc_msg.crcs.empty()) {
    return;
  }

  // Forward CRC indication to real MAC.
  adapted.handle_crc(crc_msg);

  if (not test_ue_cfg.pusch_active) {
    return;
  }

  for (const mac_crc_pdu& pdu : crc_msg.crcs) {
    if (not is_test_ue(pdu.rnti)) {
      continue;
    }

    auto rx_pdu = create_test_pdu_with_bsr(cell_index, crc_msg.sl_rx, pdu.rnti, to_harq_id(pdu.harq_id));
    if (not rx_pdu.has_value()) {
      logger.warning("TEST_MODE c-rnti={}: Unable to create test PDU with BSR", pdu.rnti);
      continue;
    }
    // In case of test mode UE, auto-forward a positive BSR.
    pdu_handler.handle_rx_data_indication(std::move(rx_pdu.value()));
  }
}

void mac_test_mode_cell_adapter::handle_uci(const mac_uci_indication_message& msg)
{
  mac_uci_indication_message msg_copy{msg};

  if (not test_ue_cfg.auto_ack_indication_delay.has_value()) {
    const auto entry = history.read(msg.sl_rx);
    if (entry != nullptr) {
      // Forward UCI to MAC, but alter the UCI for the test mode UE.
      for (mac_uci_pdu& test_uci : msg_copy.ucis) {
        if (is_test_ue(test_uci.rnti)) {
          bool entry_found = false;
          if (std::holds_alternative<mac_uci_pdu::pusch_type>(test_uci.pdu)) {
            for (const ul_sched_info& pusch : entry->puschs) {
              if (pusch.pusch_cfg.rnti == test_uci.rnti and pusch.uci.has_value()) {
                test_uci    = create_uci_pdu(pusch, test_ue_cfg);
                entry_found = true;
              }
            }
          } else {
            // PUCCH case.
            for (const pucch_info& pucch : entry->pucchs) {
              if (pucch_info_and_uci_ind_match(pucch, test_uci)) {
                // Intercept the UCI indication and force HARQ-ACK=ACK and UCI.
                test_uci    = create_uci_pdu(pucch, test_ue_cfg);
                entry_found = true;
              }
            }
          }

          if (not entry_found) {
            logger.warning(
                "TEST_MODE c-rnti={}: Failed to set UCI value for slot={}. Cause: Mismatch between provided UCI and "
                "expected UCI",
                test_uci.rnti,
                msg.sl_rx);
          }
        }
      }
    } else {
      // Failed to lock entry in history ring.
      logger.warning("TEST_MODE: Unable to set UCI value for slot={}. Cause: Overflow detected in test mode "
                     "internal storage",
                     msg.sl_rx);
    }
  } else {
    // In case of auto-ACK mode, test mode UEs are removed from UCI.
    msg_copy.ucis.erase(std::remove_if(msg_copy.ucis.begin(),
                                       msg_copy.ucis.end(),
                                       [this](const auto& u) { return is_test_ue(u.rnti); }),
                        msg_copy.ucis.end());
  }

  // Forward UCI indication to real MAC.
  forward_uci_ind_to_mac(msg_copy);
}

void mac_test_mode_cell_adapter::handle_srs(const mac_srs_indication_message& msg)
{
  // TODO: Implement this method.
}

// Intercepts the sched + signalling results coming from the MAC.
void mac_test_mode_cell_adapter::on_new_downlink_scheduler_results(const mac_dl_sched_result& dl_res)
{
  if (last_slot_ind != dl_res.slot) {
    // Process any pending tasks for the test mode asynchronously.
    event_handler.process_pending_tasks(cell_index);
    last_slot_ind = dl_res.slot;
  }

  for (const dl_msg_alloc& grant : dl_res.dl_res.ue_grants()) {
    if (not is_test_ue(grant.pdsch_cfg.rnti) or not grant.pdsch_cfg.codewords[0].new_data) {
      continue;
    }

    // A new tx carrying an SRB0 SDU marks the RRCSetup of a test UE. SRB0 is only used for the RRCSetup, so this fires
    // once per UE attach.
    const auto& lchs = grant.tb_list[0].lc_chs_to_sched;
    if (std::any_of(lchs.begin(), lchs.end(), [](const auto& lc) {
          return lc.lcid.is_sdu() and lc.lcid.to_lcid() == LCID_SRB0;
        })) {
      handle_msg4(grant, dl_res.slot);
    }
  }

  // Dispatch result to lower layers.
  result_notifier.on_new_downlink_scheduler_results(dl_res);
}

void mac_test_mode_cell_adapter::handle_msg4(const dl_msg_alloc& grant, slot_point sl_tx)
{
  const rnti_t crnti = grant.pdsch_cfg.rnti;

  if (test_ue_cfg.pdsch_active) {
    // Update DL buffer state automatically.
    dl_bs_notifier(grant.context.ue_index);
  }

  if (test_ue_cfg.pusch_active) {
    auto rx_pdu = create_test_pdu_with_bsr(cell_index, sl_tx, crnti, to_harq_id(0));
    if (not rx_pdu.has_value()) {
      logger.warning("TEST_MODE c-rnti={}: Unable to create test PDU with BSR", crnti);
    } else {
      // In case of PUSCH test mode is enabled, push a BSR to trigger the first PUSCH.
      pdu_handler.handle_rx_data_indication(std::move(rx_pdu.value()));
    }
  }

  // Notify test mode controller about the UE establishment.
  ev_notifier.on_con_res_completed(cell_index, crnti);

  // Push an UL PDU that will serve as rrcSetupComplete and get the UE out of fallback mode.
  auto rx_pdu = create_test_pdu_with_rrc_setup_complete(cell_index, sl_tx, crnti, to_harq_id(0));
  if (not rx_pdu.has_value()) {
    logger.warning("TEST_MODE c-rnti={}: Unable to create test PDU with rrcSetupComplete", crnti);
    return;
  }
  pdu_handler.handle_rx_data_indication(std::move(rx_pdu.value()));
}

// Intercepts the UL results coming from the MAC.
void mac_test_mode_cell_adapter::on_new_uplink_scheduler_results(const mac_ul_sched_result& ul_res)
{
  if (last_slot_ind != ul_res.slot) {
    // Process any pending tasks for the test mode asynchronously.
    event_handler.process_pending_tasks(cell_index);
    last_slot_ind = ul_res.slot;
  }

  // Update history.
  {
    auto entry = history.write(ul_res.slot);
    if (entry != nullptr) {
      // Fill the ring element with the scheduler decisions.
      for (const pucch_info& pucch : ul_res.ul_res.pucchs()) {
        if (is_test_ue(pucch.crnti)) {
          entry->pucchs.push_back(pucch);
        }
      }
      for (const ul_sched_info& pusch : ul_res.ul_res.puschs()) {
        if (is_test_ue(pusch.pusch_cfg.rnti)) {
          entry->puschs.push_back(pusch);
        }
      }
    } else {
      logger.warning("TEST_MODE: Unable to write UL results for slot={}. Cause: Overflow detected in test mode "
                     "internal storage",
                     ul_res.slot);
    }
  }

  // Forward results to PHY.
  result_notifier.on_new_uplink_scheduler_results(ul_res);
}

void mac_test_mode_cell_adapter::on_cell_results_completion(slot_point slot)
{
  // Forward result to lower layers.
  result_notifier.on_cell_results_completion(slot);

  // Notify test mode controller about completed slot.
  ev_notifier.on_slot_completed(cell_index, slot);
}

// ----

mac_cell_result_notifier& phy_test_mode_adapter::get_cell(du_cell_index_t cell_index)
{
  return cells[cell_index];
}
void phy_test_mode_adapter::connect(du_cell_index_t cell_index, mac_cell_result_notifier& test_mode_cell_notifier)
{
  cells[cell_index].ptr = &test_mode_cell_notifier;
}
void phy_test_mode_adapter::disconnect(du_cell_index_t cell_index)
{
  cells[cell_index].ptr = nullptr;
}
void phy_test_mode_adapter::phy_cell::on_new_downlink_scheduler_results(const mac_dl_sched_result& dl_res)
{
  ptr->on_new_downlink_scheduler_results(dl_res);
}
void phy_test_mode_adapter::phy_cell::on_new_downlink_data(const mac_dl_data_result& dl_data)
{
  ptr->on_new_downlink_data(dl_data);
}

void phy_test_mode_adapter::phy_cell::on_new_uplink_scheduler_results(const mac_ul_sched_result& ul_res)
{
  ptr->on_new_uplink_scheduler_results(ul_res);
}
void phy_test_mode_adapter::phy_cell::on_cell_results_completion(slot_point slot)
{
  ptr->on_cell_results_completion(slot);
}

// ----

mac_test_mode_adapter::mac_test_mode_adapter(const du_test_mode_config::test_mode_ue_config& test_ue_cfg_,
                                             mac_result_notifier&                            phy_notifier_,
                                             std::unique_ptr<mac_test_mode_event_notifier>   ev_notifier_,
                                             unsigned                                        nof_cells_) :
  test_ue(test_ue_cfg_),
  nof_cells(nof_cells_),
  ev_notifier(std::move(ev_notifier_)),
  event_handler(nof_cells),
  phy_notifier(std::make_unique<phy_test_mode_adapter>(phy_notifier_))
{
}

mac_test_mode_adapter::~mac_test_mode_adapter() = default;

bool mac_test_mode_adapter::is_test_ue(rnti_t rnti) const
{
  const unsigned rnti_start = static_cast<unsigned>(test_ue.rnti);
  const unsigned rnti_val   = static_cast<unsigned>(rnti);
  return rnti_val >= rnti_start and rnti_val < rnti_start + test_ue.nof_ues * nof_cells;
}

std::unique_ptr<mac_interface> mac_test_mode_adapter::decorate(std::unique_ptr<mac_interface> mac_ptr)
{
  mac_adapted = std::move(mac_ptr);
  return std::make_unique<mac_wrapper>(*this);
}

void mac_test_mode_adapter::inject_ul_ccch_msg(slot_point      sl_rx,
                                               du_cell_index_t cell_index,
                                               rnti_t          tc_rnti,
                                               byte_buffer     ul_ccch_pdu)
{
  mac_adapted->get_pdu_handler().handle_rx_data_indication(
      mac_rx_data_indication{sl_rx, cell_index, {mac_rx_pdu{tc_rnti, 0, std::move(ul_ccch_pdu)}}});
}

mac_cell_controller& mac_test_mode_adapter::add_cell(const mac_cell_creation_request& cell_cfg)
{
  // Create cell in real MAC.
  mac_cell_controller& cell = mac_adapted->get_cell_manager().add_cell(cell_cfg);

  // Create the cell in the MAC test mode.
  auto func_dl_bs_push = [this](du_ue_index_t ue_index) {
    handle_dl_buffer_state_update({ue_index, lcid_t::LCID_SRB1, TEST_UE_DL_BUFFER_STATE_UPDATE_SIZE});
  };

  auto new_cell =
      std::make_unique<mac_test_mode_cell_adapter>(test_ue,
                                                   cell_cfg,
                                                   mac_adapted->get_control_info_handler(cell_cfg.cell_index),
                                                   mac_adapted->get_pdu_handler(),
                                                   mac_adapted->get_slot_handler(cell_cfg.cell_index),
                                                   phy_notifier->adapted_phy.get_cell(cell_cfg.cell_index),
                                                   func_dl_bs_push,
                                                   *ev_notifier,
                                                   event_handler);

  if (cell_info_handler.size() <= cell_cfg.cell_index) {
    cell_info_handler.resize(cell_cfg.cell_index + 1);
  }
  cell_info_handler[cell_cfg.cell_index] = std::move(new_cell);

  phy_notifier->connect(cell_cfg.cell_index, *cell_info_handler[cell_cfg.cell_index]);

  return cell;
}

void mac_test_mode_adapter::remove_cell(du_cell_index_t cell_index)
{
  phy_notifier->disconnect(cell_index);

  // Remove cell in the MAC test mode.
  cell_info_handler[cell_index].reset();

  // Remove cell in real MAC.
  mac_adapted->get_cell_manager().remove_cell(cell_index);
}

void mac_test_mode_adapter::handle_dl_buffer_state_update(const mac_dl_buffer_state_indication_message& dl_bs)
{
  mac_dl_buffer_state_indication_message dl_bs_copy = dl_bs;
  if (is_test_ue(dl_bs.ue_index) and test_ue.pdsch_active and dl_bs.lcid != LCID_SRB0) {
    // It is the test UE. Set a positive DL buffer state if PDSCH is set to "activated".
    dl_bs_copy.bs = TEST_UE_DL_BUFFER_STATE_UPDATE_SIZE;
  }
  mac_adapted->get_ue_control_info_handler().handle_dl_buffer_state_update(dl_bs_copy);
}

std::vector<mac_logical_channel_config>
mac_test_mode_adapter::adapt_bearers(const std::vector<mac_logical_channel_config>& orig_bearers) const
{
  static test_ue_mac_sdu_tx_builder_adapter dl_adapter;

  auto test_ue_adapted_bearers = orig_bearers;
  for (auto& bearer : test_ue_adapted_bearers) {
    if (bearer.lcid != lcid_t::LCID_SRB0) {
      if (test_ue.pdsch_active) {
        bearer.dl_bearer = &dl_adapter;
      }
    }
  }

  return test_ue_adapted_bearers;
}

async_task<mac_ue_create_response> mac_test_mode_adapter::handle_ue_create_request(const mac_ue_create_request& cfg)
{
  if (is_test_ue(cfg.crnti)) {
    // It is the test UE.
    const unsigned rnti_offset = static_cast<unsigned>(cfg.crnti) - static_cast<unsigned>(test_ue.rnti);
    ocudu_assert(rnti_offset / test_ue.nof_ues == cfg.sched_cfg.cells.value()[0].cell_index,
                 "Test UE rnti={} does not belong to its PCell",
                 cfg.crnti);
    mac_ue_create_request cfg_copy = cfg;
    cfg_copy.initial_fallback      = true;

    // Add adapters to the UE config bearers before passing it to MAC.
    cfg_copy.bearers = adapt_bearers(cfg.bearers);

    // Forward test UE creation request to MAC.
    return mac_adapted->get_ue_configurator().handle_ue_create_request(cfg_copy);
  }

  // Forward normal UE creation request to MAC.
  return mac_adapted->get_ue_configurator().handle_ue_create_request(cfg);
}

async_task<mac_ue_reconfiguration_response>
mac_test_mode_adapter::handle_ue_reconfiguration_request(const mac_ue_reconfiguration_request& cfg)
{
  if (is_test_ue(cfg.crnti)) {
    // If it is the test UE.
    auto cfg_adapted = cfg;

    // Add adapters to the UE config bearers before passing it to MAC.
    cfg_adapted.bearers_to_addmod = adapt_bearers(cfg.bearers_to_addmod);

    return mac_adapted->get_ue_configurator().handle_ue_reconfiguration_request(cfg_adapted);
  }

  // otherwise, just forward config.
  return mac_adapted->get_ue_configurator().handle_ue_reconfiguration_request(cfg);
}

async_task<mac_ue_delete_response> mac_test_mode_adapter::handle_ue_delete_request(const mac_ue_delete_request& cfg)
{
  return mac_adapted->get_ue_configurator().handle_ue_delete_request(cfg);
}
