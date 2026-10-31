// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1c_du_test_mode_adapter.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents_ue.h"
#include "ocudu/f1ap/du/f1ap_du.h"
#include "ocudu/f1ap/f1ap_message.h"

using namespace ocudu;
using namespace odu;

namespace {

/// Notifier that intercepts F1AP PDUs sent by the F1AP-DU and generates responses emulating a CU-CP.
class tx_interceptor : public f1ap_message_notifier
{
public:
  tx_interceptor(const std::function<void(const f1ap_message& msg)>& on_tx_pdu_,
                 const std::function<void()>&                        on_tx_notifier_removed_) :
    on_tx_pdu(on_tx_pdu_), on_tx_notifier_removed(on_tx_notifier_removed_)
  {
  }
  ~tx_interceptor() override { on_tx_notifier_removed(); }

  void on_new_message(const f1ap_message& msg) override
  {
    using namespace asn1::f1ap;

    // Forward it via the adapter.
    on_tx_pdu(msg);
  }

private:
  std::function<void(const f1ap_message& msg)> on_tx_pdu;
  std::function<void()>                        on_tx_notifier_removed;
};

} // namespace

f1c_du_test_mode_adapter::f1c_du_test_mode_adapter(f1c_connection_client&                        f1c_client_,
                                                   std::unique_ptr<f1c_test_mode_event_notifier> ev_notifier_,
                                                   ocudulog::basic_logger&                       logger_) :
  f1c_client(f1c_client_), ev_notifier(std::move(ev_notifier_)), logger(logger_)
{
}

class dummy_f1c_rx_pdu_notifier final : public f1ap_message_notifier
{
public:
  void on_new_message(const f1ap_message& msg) override {}
};

std::unique_ptr<f1ap_message_notifier>
f1c_du_test_mode_adapter::handle_du_connection_request(std::unique_ptr<f1ap_message_notifier> du_rx_pdu_notifier)
{
  rx_notifier = std::move(du_rx_pdu_notifier);
  tx_upstream = f1c_client.handle_du_connection_request(std::make_unique<dummy_f1c_rx_pdu_notifier>());
  if (tx_upstream == nullptr) {
    rx_notifier = nullptr;
    return nullptr;
  }

  // Create an interceptor of Tx PDUs and pass it as notifier to F1AP-DU.
  return std::make_unique<tx_interceptor>([this](const f1ap_message& msg) { handle_tx_pdu(msg); },
                                          [this]() { handle_tx_notifier_removed(); });
}

void f1c_du_test_mode_adapter::handle_tx_pdu(const f1ap_message& msg)
{
  using namespace asn1::f1ap;

  switch (msg.pdu.type().value) {
    case f1ap_pdu_c::types_opts::init_msg:
      handle_tx_init(msg);
      break;
    case f1ap_pdu_c::types_opts::successful_outcome:
      handle_tx_success(msg);
      break;
    default:
      break;
  }
}

void f1c_du_test_mode_adapter::handle_tx_init(const f1ap_message& msg)
{
  using namespace asn1::f1ap;
  const auto& init = msg.pdu.init_msg();

  switch (init.value.type().value) {
    case f1ap_elem_procs_o::init_msg_c::types_opts::f1_setup_request: {
      const auto& f1_setup = msg.pdu.init_msg().value.f1_setup_request();

      f1ap_message resp;
      resp.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_F1_SETUP);
      auto& setup_resp           = resp.pdu.successful_outcome().value.f1_setup_resp();
      setup_resp->transaction_id = f1_setup->transaction_id;
      setup_resp->gnb_cu_name.from_string("gnb01");
      if (f1_setup->gnb_du_served_cells_list_present) {
        setup_resp->cells_to_be_activ_list_present = true;
        for (const auto& cell : f1_setup->gnb_du_served_cells_list) {
          cells_to_be_activ_list_item_s cell_item;
          cell_item.nr_cgi         = cell->gnb_du_served_cells_item().served_cell_info.nr_cgi;
          cell_item.nr_pci_present = true;
          cell_item.nr_pci         = cell->gnb_du_served_cells_item().served_cell_info.nr_pci;
          setup_resp->cells_to_be_activ_list.push_back({});
          setup_resp->cells_to_be_activ_list.back().load_info_obj(ASN1_F1AP_ID_CELLS_TO_BE_ACTIV_LIST_ITEM);
          setup_resp->cells_to_be_activ_list.back().value().cells_to_be_activ_list_item() = cell_item;
        }
      }
      setup_resp->gnb_cu_rrc_version = f1_setup->gnb_du_rrc_version;

      logger.info("TEST_MODE: Injected F1 Setup Response");
      rx_notifier->on_new_message(resp);
    } break;
    case f1ap_elem_procs_o::init_msg_c::types_opts::init_ul_rrc_msg_transfer: {
      // Add UE context.
      const auto&  ie       = msg.pdu.init_msg().value.init_ul_rrc_msg_transfer();
      const rnti_t rnti     = to_rnti(ie->c_rnti);
      const auto   du_ue_id = int_to_gnb_du_ue_f1ap_id(ie->gnb_du_ue_f1ap_id);
      auto         ret      = du_ue_ids.insert(std::make_pair(du_ue_id, ue_context{du_ue_id, std::nullopt, rnti}));
      ocudu_assert(ret.second, "Duplicate gNB-DU-UE-F1AP-ID creation");

      // Prepare response.
      f1ap_message resp;
      if (ie->du_to_cu_rrc_container_present) {
        ret.first->second.cu_ue_id = int_to_gnb_cu_ue_f1ap_id(next_cu_ue_id);
        resp.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_DL_RRC_MSG_TRANSFER);
        auto& dl_rrc              = resp.pdu.init_msg().value.dl_rrc_msg_transfer();
        dl_rrc->gnb_du_ue_f1ap_id = ie->gnb_du_ue_f1ap_id;
        dl_rrc->gnb_cu_ue_f1ap_id = next_cu_ue_id;
        dl_rrc->srb_id            = 0;
        dl_rrc->rrc_container.from_string(
            "204004094ae00580088bd76380830f0003e0102341e0400020904c0ca8000ff800000000183708420001e01650020c00000081"
            "a6040514280038e2400040d55f21070004103081430727122858c1a3879022000010a00010016a00021910a00031916a000400"
            "90a00050096a00061890a00071896a00080210a0009032080280c8240b0320a0300c82c0d0320c0380c8340f0320e040588201"
            "103a0a4092e4a9286050e23a2b3c4de4d03a41078bbf03043800000071ffa5294a529e502c0000432ec0000000000000000001"
            "8ad5450047001800082000e21005c400e0202108001c4200b8401c080441000388401708038180842000710802e18070401104"
            "000e21005c300080000008218081018201c1a0001c71000000080100020180020240088029800008c40089c7001800");
        logger.info("TEST_MODE rnti={}: Injected DL RRC Message (rrcSetup)", rnti);

      } else {
        resp.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_RELEASE);
        auto& rel_cmd                            = resp.pdu.init_msg().value.ue_context_release_cmd();
        rel_cmd->gnb_du_ue_f1ap_id               = ie->gnb_du_ue_f1ap_id;
        rel_cmd->gnb_cu_ue_f1ap_id               = next_cu_ue_id;
        rel_cmd->cause.set_radio_network().value = cause_radio_network_opts::no_radio_res_available;
        rel_cmd->srb_id_present                  = true;
        rel_cmd->srb_id                          = 0;
        rel_cmd->rrc_container_present           = true;
        // Normally the RRC container contains a RRC Release, but in test mode it is not important what it contains
        // as it is transparent to the DU.
        rel_cmd->rrc_container.from_string("012345");
        logger.warning("TEST_MODE rnti={}: Injected UE Context Release (rrcReject)", rnti);
      }

      ++next_cu_ue_id;
      rx_notifier->on_new_message(resp);
    } break;

    case f1ap_elem_procs_o::init_msg_c::types_opts::f1_removal_request: {
      const auto&  ie = msg.pdu.init_msg().value.f1_removal_request();
      f1ap_message resp;
      resp.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_F1_REMOVAL);
      auto& removal_resp           = resp.pdu.successful_outcome().value.f1_removal_resp();
      removal_resp->transaction_id = ie->transaction_id;

      logger.info("TEST_MODE: Injected F1 Removal Response");
      rx_notifier->on_new_message(resp);
    } break;
    default:
      break;
  }
}

void f1c_du_test_mode_adapter::handle_tx_success(const f1ap_message& msg)
{
  using namespace asn1::f1ap;

  const auto& success = msg.pdu.successful_outcome();
  switch (success.value.type().value) {
    case f1ap_elem_procs_o::successful_outcome_c::types_opts::ue_context_release_complete: {
      // Remove UE context from du_ue_ids container.
      auto it =
          du_ue_ids.find(int_to_gnb_du_ue_f1ap_id(success.value.ue_context_release_complete()->gnb_du_ue_f1ap_id));
      ocudu_assert(it != du_ue_ids.end(), "Unable to remove UE context");
      const rnti_t rnti = it->second.rnti;
      du_ue_ids.erase(it);

      // Notify UE removal to test mode controller.
      ev_notifier->on_ue_removed(rnti);
    } break;
    default:
      break;
  }
}

void f1c_du_test_mode_adapter::handle_tx_notifier_removed()
{
  // Delete F1-C tx/rx notifiers.
  tx_upstream.reset();
  rx_notifier.reset();

  // Notify DU of the F1-C connection shutdown.
  ev_notifier->on_f1c_connection_drop();

  // Eliminate all UE contexts.
  du_ue_ids.clear();
}

bool f1c_du_test_mode_adapter::try_release_ue(rnti_t rnti)
{
  auto it = std::find_if(du_ue_ids.begin(), du_ue_ids.end(), [&rnti](const auto& p) { return p.second.rnti == rnti; });
  if (it != du_ue_ids.end()) {
    return release_ue(it->first);
  }
  return false;
}

bool f1c_du_test_mode_adapter::release_ue(gnb_du_ue_f1ap_id_t du_ue_id)
{
  auto it = du_ue_ids.find(du_ue_id);
  if (it == du_ue_ids.end() or not it->second.cu_ue_id.has_value()) {
    logger.warning("TEST_MODE: Cannot release UE. gNB-CU-UE-F1AP-ID not set");
    return false;
  }

  // Prepare F1AP UE Context Release Command.
  f1ap_message rel_cmd;
  rel_cmd.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_RELEASE);
  auto& cmd                            = rel_cmd.pdu.init_msg().value.ue_context_release_cmd();
  cmd->gnb_du_ue_f1ap_id               = to_underlying(du_ue_id);
  cmd->gnb_cu_ue_f1ap_id               = to_underlying(*it->second.cu_ue_id);
  cmd->cause.set_radio_network().value = asn1::f1ap::cause_radio_network_opts::options::normal_release;

  logger.debug("TEST_MODE rnti={}: Injecting UE Context Release Command", it->second.rnti);
  rx_notifier->on_new_message(rel_cmd);

  return true;
}
