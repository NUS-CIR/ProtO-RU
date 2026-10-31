// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1c_test_mode_adapter.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/f1ap_message.h"

using namespace ocudu;
using namespace asn1::f1ap;

namespace {

/// Instance of a stub CU-CP that simulates the behavior of a CU-CP from the DU perspective, when in test mode.
class stub_cu_cp
{
public:
  void handle_new_connection(std::unique_ptr<f1ap_message_notifier> du_rx_notifier_)
  {
    du_rx_notifier = std::move(du_rx_notifier_);
  }

  void handle_connection_removal() { du_rx_notifier = nullptr; }

  void handle_message(const f1ap_message& msg)
  {
    if (msg.pdu.type().value != f1ap_pdu_c::types_opts::init_msg) {
      return;
    }

    switch (msg.pdu.init_msg().proc_code) {
      case ASN1_F1AP_ID_F1_SETUP:
        du_rx_notifier->on_new_message(make_f1_setup_response(msg));
        break;
      case ASN1_F1AP_ID_GNB_DU_CFG_UPD:
        du_rx_notifier->on_new_message(make_gnb_du_cfg_upd_ack(msg));
        break;
      case ASN1_F1AP_ID_F1_REMOVAL:
        du_rx_notifier->on_new_message(make_f1_removal_response(msg));
        break;
      default:
        break;
    }
  }

private:
  static f1ap_message make_f1_setup_response(const f1ap_message& req_msg)
  {
    using namespace asn1::f1ap;

    const auto& req = req_msg.pdu.init_msg().value.f1_setup_request();

    f1ap_message resp;
    resp.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_F1_SETUP);
    f1_setup_resp_s& resp_ie = resp.pdu.successful_outcome().value.f1_setup_resp();

    resp_ie->transaction_id      = req->transaction_id;
    resp_ie->gnb_cu_name_present = true;
    resp_ie->gnb_cu_name.from_string("stub-cu-cp");
    resp_ie->gnb_cu_rrc_version.latest_rrc_version.from_number(2);

    resp_ie->cells_to_be_activ_list_present = true;
    resp_ie->cells_to_be_activ_list.resize(req->gnb_du_served_cells_list.size());
    for (unsigned i = 0; i != req->gnb_du_served_cells_list.size(); ++i) {
      const auto& req_cell = req->gnb_du_served_cells_list[i]->gnb_du_served_cells_item();
      resp_ie->cells_to_be_activ_list[i].load_info_obj(ASN1_F1AP_ID_CELLS_TO_BE_ACTIV_LIST_ITEM);
      auto& cell          = resp_ie->cells_to_be_activ_list[i].value().cells_to_be_activ_list_item();
      cell.nr_cgi         = req_cell.served_cell_info.nr_cgi;
      cell.nr_pci_present = true;
      cell.nr_pci         = req_cell.served_cell_info.nr_pci;
    }

    return resp;
  }

  static f1ap_message make_gnb_du_cfg_upd_ack(const f1ap_message& req_msg)
  {
    using namespace asn1::f1ap;

    const gnb_du_cfg_upd_s& req = req_msg.pdu.init_msg().value.gnb_du_cfg_upd();

    f1ap_message resp;
    resp.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_GNB_DU_CFG_UPD);
    gnb_du_cfg_upd_ack_s& ack = resp.pdu.successful_outcome().value.gnb_du_cfg_upd_ack();
    ack->transaction_id       = req->transaction_id;

    return resp;
  }

  static f1ap_message make_f1_removal_response(const f1ap_message& req_msg)
  {
    using namespace asn1::f1ap;

    f1ap_message resp;
    resp.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_F1_REMOVAL);
    f1_removal_resp_s& ie = resp.pdu.successful_outcome().value.f1_removal_resp();
    ie->transaction_id    = req_msg.pdu.init_msg().value.f1_removal_request()->transaction_id;

    return resp;
  }

  std::unique_ptr<f1ap_message_notifier> du_rx_notifier;
};

/// Stub CU-CP F1-C client that auto-responds to DU-initiated F1AP procedures.
///
/// Intended for test mode DU application operation where no real CU-CP is available.
class stub_cu_cp_f1c_client final : public odu::f1c_connection_client
{
  class cu_rx_notifier final : public f1ap_message_notifier
  {
  public:
    cu_rx_notifier(stub_cu_cp& parent_) : cu_cp(parent_) {}
    ~cu_rx_notifier() override { cu_cp.handle_connection_removal(); }

    void on_new_message(const f1ap_message& msg) override { cu_cp.handle_message(msg); }

  private:
    stub_cu_cp& cu_cp;
  };

public:
  std::unique_ptr<f1ap_message_notifier>
  handle_du_connection_request(std::unique_ptr<f1ap_message_notifier> du_rx_pdu_notifier) override
  {
    cu_cp.handle_new_connection(std::move(du_rx_pdu_notifier));
    return std::make_unique<cu_rx_notifier>(cu_cp);
  }

private:
  /// Object emulating the CU-CP behavior.
  stub_cu_cp cu_cp;
};

} // namespace

std::unique_ptr<odu::f1c_connection_client> ocudu::make_test_mode_f1c_connection_client()
{
  return std::make_unique<stub_cu_cp_f1c_client>();
}
