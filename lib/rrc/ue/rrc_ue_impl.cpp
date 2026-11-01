// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "rrc_ue_impl.h"
#include "procedures/rrc_ue_information_procedure.h"
#include "rrc_asn1_helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/rrc_nr.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/support/ocudu_assert.h"

using namespace ocudu;
using namespace ocucp;
using namespace asn1::rrc_nr;

rrc_ue_impl::rrc_ue_impl(rrc_pdu_f1ap_notifier&                 f1ap_pdu_notifier_,
                         rrc_ue_ngap_notifier&                  ngap_notifier_,
                         rrc_ue_context_update_notifier&        cu_cp_notifier_,
                         rrc_ue_measurement_notifier&           measurement_notifier_,
                         rrc_ue_cu_cp_ue_notifier&              cu_cp_ue_notifier_,
                         rrc_ue_event_notifier&                 metrics_notifier_,
                         rrc_ue_srb_pdcp_manager&               pdcp_manager_,
                         const cu_cp_ue_index_t                 ue_index_,
                         const rnti_t                           c_rnti_,
                         const rrc_cell_context&                cell_,
                         const rrc_ue_cfg_t&                    cfg_,
                         const byte_buffer&                     du_to_cu_container_,
                         std::optional<rrc_ue_transfer_context> rrc_context,
                         std::optional<rrc_resume_context_t>    remote_resume_context) :
  logger("RRC", {ue_index_, c_rnti_}),
  context(ue_index_, c_rnti_, cell_, cfg_, rrc_context, remote_resume_context, pdcp_manager_, logger),
  f1ap_pdu_notifier(f1ap_pdu_notifier_),
  ngap_notifier(ngap_notifier_),
  cu_cp_notifier(cu_cp_notifier_),
  measurement_notifier(measurement_notifier_),
  cu_cp_ue_notifier(cu_cp_ue_notifier_),
  metrics_notifier(metrics_notifier_),
  du_to_cu_container(du_to_cu_container_),
  event_mng(std::make_unique<rrc_ue_event_manager>(cu_cp_ue_notifier.get_timer_factory()))
{
  ocudu_assert(context.cell.bands.empty() == false, "Band must be present in RRC cell configuration.");

  // Update security context and keys.
  if (rrc_context.has_value()) {
    if (!rrc_context.value().is_inter_cu_handover) {
      cu_cp_ue_notifier.update_security_context(rrc_context.value().sec_context);
      cu_cp_ue_notifier.perform_horizontal_key_derivation(cell_.pci, cell_.ssb_arfcn.value());
    }

    // Create SRBs.
    for (const auto& srb : rrc_context.value().srbs) {
      srb_creation_message srb_msg{};
      srb_msg.ue_index        = ue_index_;
      srb_msg.srb_id          = srb;
      srb_msg.enable_security = true;
      srb_msg.pdcp_cfg        = {}; // TODO: add support for non-default config.
      create_srb(srb_msg);
    }
  }
}

rrc_ue_impl::~rrc_ue_impl() {}

void rrc_ue_impl::create_srb(const srb_creation_message& msg)
{
  logger.log_debug("Creating {}", msg.srb_id);

  // Create adapter objects and PDCP bearer as needed.
  if (msg.srb_id == srb_id_t::srb0) {
    // SRB0 is already set up.
    return;
  }

  if (msg.srb_id <= srb_id_t::srb2) {
    rrc_ue_pdcp_notifier& notifier                     = context.pdcp_manager.create_srb(msg.srb_id);
    context.pdcp_notifiers[srb_id_to_uint(msg.srb_id)] = &notifier;

    if (msg.srb_id == srb_id_t::srb2 || msg.enable_security) {
      security::sec_128_as_config sec_cfg = security::truncate_config(cu_cp_ue_notifier.get_rrc_as_config());
      notifier.enable_tx_security(security::integrity_enabled::on, security::ciphering_enabled::on, sec_cfg);
      notifier.enable_rx_security(security::integrity_enabled::on, security::ciphering_enabled::on, sec_cfg);
    }
  } else {
    logger.log_error("Couldn't create {}", msg.srb_id);
  }
}

static_vector<srb_id_t, MAX_NOF_SRBS> rrc_ue_impl::get_srbs()
{
  return context.pdcp_manager.get_srb_ids();
}

rrc_state rrc_ue_impl::get_rrc_state() const
{
  return context.state;
}

void rrc_ue_impl::cancel_handover_reconfiguration_transaction(uint8_t transaction_id)
{
  logger.log_debug("Cancelling the ongoing handover reconfiguration transaction");
  if (not event_mng->transactions.cancel_transaction(transaction_id)) {
    logger.log_warning("Unexpected RRC transaction id={}", transaction_id);
  }
}

void rrc_ue_impl::cancel_all_transactions(protocol_transaction_failure cause)
{
  logger.log_debug("Cancelling all ongoing RRC transactions");
  event_mng->cancel_all(cause);
}

void rrc_ue_impl::update_c_rnti(rnti_t crnti)
{
  context.c_rnti = crnti;
  logger.set_prefix(rrc_ue_log_prefix{context.ue_index, crnti});
}

void rrc_ue_impl::update_cell_group_config(byte_buffer cell_group_config)
{
  context.cell_group_config = std::move(cell_group_config);
}

void rrc_ue_impl::on_new_dl_ccch(const asn1::rrc_nr::dl_ccch_msg_s& dl_ccch_msg)
{
  send_dl_ccch(dl_ccch_msg);
}

void rrc_ue_impl::on_new_dl_dcch(srb_id_t srb_id, const asn1::rrc_nr::dl_dcch_msg_s& dl_dcch_msg)
{
  send_dl_dcch(srb_id, dl_dcch_msg);
}

void rrc_ue_impl::on_new_as_security_context(bool security_mode_active)
{
  ocudu_sanity_check(context.pdcp_manager.has_srb(srb_id_t::srb1),
                     "Attempted to configure security, but there is no interface to PDCP");

  security::sec_128_as_config sec_cfg = cu_cp_ue_notifier.get_rrc_128_as_config();
  rrc_ue_pdcp_notifier&       srb1    = *context.get_pdcp_notifier(srb_id_t::srb1);
  srb1.enable_rx_security(security_mode_active ? security::integrity_enabled::on
                                               : security::integrity_enabled::smc_transition,
                          security::ciphering_enabled::off,
                          sec_cfg);
  srb1.enable_tx_security(security::integrity_enabled::on, security::ciphering_enabled::off, sec_cfg);
}

void rrc_ue_impl::on_as_security_activated()
{
  // The UE comes back on a new RRC UE with no coarse location, and no Security Mode Command to ask for one.
  // TS 38.300 sec. 16.14.8 gates the request on AS security, which is active by the time this is called.
  request_coarse_ue_location();
}

/// \brief Whether the coarse UE location is put to use in \c cell.
///
/// Only an NTN cell, whose footprint can span several tracking areas, and only one configured with the areas that turn
/// a position into a TAC or a Mapped Cell ID. Elsewhere no position is ever asked for, so none is awaited either.
static bool uses_coarse_ue_location(const rrc_cell_context& cell)
{
  return not cell.location_mapping.empty() and
         std::any_of(cell.bands.begin(), cell.bands.end(), band_helper::is_ntn_band);
}

void rrc_ue_impl::request_coarse_ue_location()
{
  if (not uses_coarse_ue_location(context.cell)) {
    return;
  }

  cu_cp_ue_notifier.schedule_async_task(launch_async<rrc_ue_information_procedure>(
      context, *this, cu_cp_notifier, cu_cp_ue_notifier, *event_mng, logger));
}

void rrc_ue_impl::fill_ue_derived_location(cu_cp_user_location_info_nr& user_location_info) const
{
  const std::optional<tac_t>            derived_tac = get_ue_location_derived_tac();
  const std::optional<nr_cell_identity> mapped_nci  = get_ue_mapped_cell_id();

  // A cell that names its areas by Mapped Cell ID cannot place a UE whose position never arrived, so say so rather
  // than let the CU-CP read the absent identity as the UE being outside every area, TS 38.413 sec. 9.3.1.67.
  // Only a cell that asks for a position can be waiting for one: a mapping on a terrestrial cell still resolves the
  // identities the core names, but derives none for a UE, so it must not report every UE of that cell unknown.
  user_location_info.mapped_nci_unknown = not context.coarse_location.has_value() and
                                          uses_coarse_ue_location(context.cell) and
                                          context.cell.location_mapping.names_mapped_cell_ids();

  // An area whose TAC the cell does not broadcast still names a Mapped Cell ID, so the two are taken separately.
  if (not derived_tac.has_value() and not mapped_nci.has_value()) {
    return;
  }

  user_location_info.ue_location_derived_tac = derived_tac;
  user_location_info.mapped_nci              = mapped_nci;

  // The Time Stamp of TS 38.413 sec. 9.3.1.75 is the first four octets of an RFC 5905 timestamp: seconds since
  // 1900-01-01. The UE sends no time with its position, so this dates it by its arrival, measured as an age on the
  // monotonic clock so that correcting the wall clock in between corrects the result rather than skewing it.
  constexpr uint64_t seconds_from_1900_to_1970 = 2208988800;
  const auto         reported_at =
      std::chrono::system_clock::now() - (std::chrono::steady_clock::now() - context.coarse_location->received_at);
  const auto reported_at_s      = std::chrono::duration_cast<std::chrono::seconds>(reported_at.time_since_epoch());
  user_location_info.time_stamp = seconds_from_1900_to_1970 + static_cast<uint64_t>(reported_at_s.count());
}

std::optional<tac_t> rrc_ue_impl::get_ue_location_derived_tac() const
{
  if (not context.coarse_location.has_value()) {
    return std::nullopt;
  }

  std::optional<tac_t> derived_tac =
      derive_tac_from_location(context.cell.location_mapping, context.coarse_location->position);
  if (not derived_tac.has_value()) {
    logger.log_debug("No TAC derived from the coarse UE location lat={:.4f} lon={:.4f} reported {}s ago. Cause: {}",
                     context.coarse_location->position.latitude,
                     context.coarse_location->position.longitude,
                     std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() -
                                                                      context.coarse_location->received_at)
                         .count(),
                     context.cell.location_mapping.empty()
                         ? "the cell has no location mapping"
                         : "the position is outside every area, or inside one that maps no TAC");
    return std::nullopt;
  }

  logger.log_debug("Derived TAC={} from the coarse UE location", derived_tac.value());
  return derived_tac;
}

std::optional<nr_cell_identity> rrc_ue_impl::get_ue_mapped_cell_id() const
{
  if (not context.coarse_location.has_value()) {
    return std::nullopt;
  }

  const std::optional<nr_cell_identity> mapped_nci =
      derive_mapped_cell_id_from_location(context.cell.location_mapping, context.coarse_location->position);
  if (not mapped_nci.has_value()) {
    // Reporting the Uu Cell ID is what TS 38.300 sec. 16.14.5 leaves in place, so this is a normal outcome and not a
    // failure: an area is free to map no Mapped Cell ID.
    logger.log_debug("Reporting the Uu Cell ID rather than a Mapped Cell ID for the coarse UE location lat={:.4f} "
                     "lon={:.4f}. Cause: {}",
                     context.coarse_location->position.latitude,
                     context.coarse_location->position.longitude,
                     context.cell.location_mapping.empty()
                         ? "the cell has no location mapping"
                         : "the position is outside every area, or inside one that maps no Mapped Cell ID");
    return std::nullopt;
  }

  logger.log_debug("Derived Mapped Cell ID={:#x} from the coarse UE location", mapped_nci.value());
  return mapped_nci;
}

// Builds the UE's current radio bearer configuration (all active DRBs across all PDU sessions) from the UP
// context.
static rrc_radio_bearer_config build_source_radio_bearer_config(const up_context& up_ctxt)
{
  rrc_radio_bearer_config radio_bearer_config;
  for (const auto& [psi, pdu_session_ctxt] : up_ctxt.pdu_sessions) {
    for (const auto& [drb_id, drb_ctxt] : pdu_session_ctxt.drbs) {
      rrc_drb_to_add_mod drb_to_add_mod;
      drb_to_add_mod.drb_id   = drb_id;
      drb_to_add_mod.pdcp_cfg = drb_ctxt.pdcp_cfg;

      rrc_cn_assoc cn_assoc;
      cn_assoc.sdap_cfg       = drb_ctxt.sdap_cfg;
      drb_to_add_mod.cn_assoc = cn_assoc;

      radio_bearer_config.drb_to_add_mod_list.emplace(drb_id, drb_to_add_mod);
    }
  }

  // TODO: Fill SRB config and security config.

  return radio_bearer_config;
}

byte_buffer rrc_ue_impl::get_packed_handover_preparation_message()
{
  struct ho_prep_info_s ho_prep;
  ho_prep_info_ies_s&   ies = ho_prep.crit_exts.set_c1().set_ho_prep_info();

  if (not context.capabilities_list.has_value()) {
    logger.log_error("No UE capabilities stored. Handover preparation message can't be generated");
    // No capabilities present, return empty buffer.
    return {};
  }
  ies.ue_cap_rat_list = *context.capabilities_list;

  // Fill AS-Config with the UE's current radio bearer configuration. TS 38.331 Section 11.2.3 mandates that the
  // RRCReconfiguration embedded here reflects the UE's complete AS configuration, not a delta relative to prior
  // signalling, so it is built from the UP context rather than from any previously sent RRCReconfiguration.
  rrc_radio_bearer_config source_radio_bearer_cfg =
      build_source_radio_bearer_config(cu_cp_notifier.on_up_context_required());
  if (!source_radio_bearer_cfg.drb_to_add_mod_list.empty()) {
    ies.source_cfg_present = true;
    rrc_reconfiguration_procedure_request source_cfg_request;
    source_cfg_request.radio_bearer_cfg = source_radio_bearer_cfg;
    rrc_recfg_s source_recfg;
    fill_asn1_rrc_reconfiguration_msg(source_recfg, 0, source_cfg_request);
    ies.source_cfg.rrc_recfg = pack_into_pdu(source_recfg, "AS-Config RRCReconfiguration");
  }

  // TODO: Fill measurement configuration and MCG/SCG.

  return pack_into_pdu(ho_prep, "handover preparation info");
}

void rrc_ue_impl::on_ue_release_required(const ngap_cause_t& cause)
{
  cu_cp_ue_notifier.schedule_async_task(launch_async([this, cause](coro_context<async_task<void>>& ctx) mutable {
    CORO_BEGIN(ctx);

    CORO_AWAIT(cu_cp_notifier.on_ue_release_required({context.ue_index, {}, cause}));

    CORO_RETURN();
  }));
}
