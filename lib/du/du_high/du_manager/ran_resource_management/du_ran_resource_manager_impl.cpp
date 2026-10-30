// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "du_ran_resource_manager_impl.h"
#include "ocudu/adt/format.h"
#include "ocudu/mac/config/mac_cell_group_config_factory.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include "ocudu/scheduler/rrm/configured_grant_rrm_factory.h"
#include "ocudu/scheduler/rrm/srs_resource_manager_factory.h"
#include "ocudu/scheduler/scheduler_configurator.h"

using namespace ocudu;
using namespace odu;

du_ue_ran_resource_updater_impl::du_ue_ran_resource_updater_impl(du_ue_resource_config* cell_grp_cfg_,
                                                                 const std::optional<ue_capability_summary>& ue_caps_,
                                                                 du_ran_resource_manager_impl&               parent_,
                                                                 du_ue_index_t ue_index_) :
  cell_grp(cell_grp_cfg_), ue_caps(&ue_caps_), parent(&parent_), ue_index(ue_index_)
{
}

du_ue_ran_resource_updater_impl::~du_ue_ran_resource_updater_impl()
{
  parent->deallocate_context(ue_index);
}

du_ue_resource_update_response
du_ue_ran_resource_updater_impl::update(du_cell_index_t                       pcell_index,
                                        const f1ap_ue_context_update_request& upd_req,
                                        const du_ue_resource_config*          reestablished_context,
                                        const ue_capability_summary*          reestablished_ue_caps)
{
  return parent->update_context(ue_index, pcell_index, upd_req, reestablished_context, reestablished_ue_caps);
}

void du_ue_ran_resource_updater_impl::config_applied()
{
  parent->ue_config_applied(ue_index);
}

void du_ue_ran_resource_updater_impl::set_cs_rnti(rnti_t cs_rnti)
{
  cell_grp->cell_group.pcg_cfg.cs_rnti = cs_rnti;
}

void du_ue_ran_resource_updater_impl::clear_cs_rnti()
{
  cell_grp->cell_group.pcg_cfg.cs_rnti.reset();
}

///////////////////////////

// Helper that removes the SRS and CG configurations from the default serving cell configuration. The SRS resource
// manager allocates the SRS configuration, and the CG resource manager allocates the CG configuration once the UE has
// DRBs.
static void reset_serv_cell_cfg(serving_cell_config& serv_cell_cfg)
{
  ocudu_assert(serv_cell_cfg.ul_config.has_value(), "UL configuration in Serving cell config not configured");

  serv_cell_cfg.ul_config->init_ul_bwp.srs_cfg.reset();
  serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.reset();
}

du_ran_resource_manager_impl::du_ran_resource_manager_impl(span<const du_cell_config>                cell_cfg_list_,
                                                           const scheduler_expert_config&            scheduler_cfg,
                                                           const std::map<srb_id_t, du_srb_config>&  srbs,
                                                           const std::map<five_qi_t, du_qos_config>& qos,
                                                           const du_test_mode_config&                test_cfg_) :
  cell_cfg_list(cell_cfg_list_),
  logger(ocudulog::fetch_basic_logger("DU-MNG")),
  test_cfg(test_cfg_),
  pucch_res_mng(scheduler_cfg.ue.max_pucchs_per_slot),
  pdsch_res_mng(cell_cfg_list, test_cfg),
  pusch_res_mng(cell_cfg_list, test_cfg),
  csi_res_mng(test_cfg.test_ue.has_value() and test_cfg.test_ue->rnti != rnti_t::INVALID_RNTI),
  bearer_res_mng(srbs, qos, logger),
  // Extract Type of CG (i.e., Type1 or 2) from the first cell; all cell share the same type.
  srs_res_mng(create_srs_resource_manager(cell_cfg_list_[0].ran)),
  cg_res_mng(create_configured_grant_rrm(cell_cfg_list_[0].ran)),
  meas_cfg_mng(cell_cfg_list),
  drx_res_mng(cell_cfg_list),
  ra_res_alloc(cell_cfg_list),
  cell_ue_ctxts(cell_cfg_list_.size())
{
  // The Configured Grant resource manager is built once for the whole DU, from the CG type of the first cell (see the
  // initialization list above), and every CG-enabled cell is then registered with it below. A cell configured for the
  // other CG type would be served by the implementation of the wrong type, which builds it grants of a shape it cannot
  // use, so refuse the configuration outright rather than fail later, once UEs are being admitted.
  // NOTE: a first cell with no CG at all yields the Type 1 manager, so a Type 2 cell behind it is a mismatch too.
  const bool du_cg_is_type2 =
      cell_cfg_list[0].ran.init_bwp.cg_cfg.has_value() and cell_cfg_list[0].ran.init_bwp.cg_cfg->is_type2();
  for (unsigned cell_idx_uint = 0; cell_idx_uint != cell_cfg_list.size(); ++cell_idx_uint) {
    const auto& cg_cfg = cell_cfg_list[cell_idx_uint].ran.init_bwp.cg_cfg;
    if (cg_cfg.has_value() and cg_cfg->is_type2() != du_cg_is_type2) {
      report_error("ERROR: Invalid DU Cell Configuration. Cause: cell={} configures Configured Grant type {}, while "
                   "the DU is set up for type {} (taken from cell=0). All cells of a DU must use the same Configured "
                   "Grant type.\n",
                   cell_idx_uint,
                   cg_cfg->is_type2() ? 2 : 1,
                   du_cg_is_type2 ? 2 : 1);
    }
  }

  for (unsigned cell_idx_uint = 0; cell_idx_uint != cell_cfg_list.size(); ++cell_idx_uint) {
    const auto&           cell     = cell_cfg_list[cell_idx_uint];
    const du_cell_index_t cell_idx = to_du_cell_index(cell_idx_uint);
    pucch_res_mng.add_cell(cell_idx, cell.ran);
    srs_res_mng->add_cell(cell_idx, cell.ran);
    csi_res_mng.add_cell(cell_idx, cell.ran);
    if (cell.ran.init_bwp.cg_cfg.has_value()) {
      cg_res_mng->add_cell(cell_idx, cell.ran);
    }

    // The resource pools of a cell do not change during the lifetime of this class, so the capacity of the cell can be
    // determined once, while all its resources are still free.
    cell_ue_ctxts[cell_idx].max_nof_established = compute_max_nof_established_ue_ctxts(cell_idx);

    const bool is_periodic_csi_report =
        cell.ran.init_bwp.csi.has_value() and cell.ran.init_bwp.csi->csi_report_slot_offset.has_value();
    const bool is_periodic_srs = cell.ran.init_bwp.srs_cfg.srs_type_enabled == srs_type::periodic;

    auto& config_logger = ocudulog::fetch_basic_logger("CONFIG");
    config_logger.info(
        "The upper-bound on the number of UEs supported by cell {{pci={}, du_cell_index={}}} is {} (the actual "
        "number might be lower than that). This is determined by the lowest of the following limits: SR ({}), "
        "CSI ({}), SRS ({}) and UE contexts per cell ({}). Up to {} UE contexts can be created in the cell, the extra "
        "ones being used to RRC Reject UEs.",
        cell.ran.pci,
        cell_idx,
        cell_ue_ctxts[cell_idx].max_nof_established,
        pucch_res_mng.get_nof_free_sr_configs(cell_idx),
        is_periodic_csi_report ? fmt::to_string(pucch_res_mng.get_nof_free_csi_configs(cell_idx)) : "n/a",
        is_periodic_srs ? fmt::to_string(srs_res_mng->get_nof_srs_free_res_offsets(cell_idx)) : "n/a",
        max_nof_ue_ctxts_per_cell - max_nof_rejected_ue_ctxts,
        cell_ue_ctxts[cell_idx].max_nof_ue_ctxts());
  }
}

/// \brief Determines whether a UE may be sent a Differential Koffset, as per TS 38.321, Section 5.18.24.
///
/// Requires an NTN cell deriving a UE-specific Koffset and a UE declaring ue-specific-K-Offset-r17 for the band.
static bool supports_ue_specific_koffset(const cell_group_config&     cell_grp_cfg,
                                         span<const du_cell_config>   cell_cfg_list,
                                         const ue_capability_summary& ue_caps)
{
  if (not cell_grp_cfg.cells.contains(SERVING_PCELL_IDX)) {
    return false;
  }

  const du_cell_config& cell_cfg = cell_cfg_list[cell_grp_cfg.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.cell_index];
  if (not get_ue_koffset_cfg(cell_cfg.ran.ntn_params).has_value()) {
    return false;
  }

  const auto band_it = ue_caps.bands.find(cell_cfg.ran.dl_carrier.band);
  return band_it != ue_caps.bands.end() and band_it->second.ue_specific_k_offset_supported;
}

/// \brief Configures tar-Config for a UE, enabling variation-triggered Timing Advance reporting.
///
/// Only an NTN cell configures it, and only for a UE that declares uplink-TA-Reporting-r17 for the band: signalling it
/// otherwise asks for a procedure the UE does not implement. timingAdvanceSR is dropped separately when the UE cannot
/// raise an SR for a report, as that is a distinct capability.
static void update_tar_config(cell_group_config&           cell_grp_cfg,
                              span<const du_cell_config>   cell_cfg_list,
                              const ue_capability_summary& ue_caps,
                              du_ue_index_t                ue_index,
                              ocudulog::basic_logger&      logger)
{
  cell_grp_cfg.mcg_cfg.tar_cfg.reset();

  if (not cell_grp_cfg.cells.contains(SERVING_PCELL_IDX)) {
    return;
  }

  const du_cell_config& cell_cfg = cell_cfg_list[cell_grp_cfg.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.cell_index];
  if (not cell_cfg.ran.ntn_params.has_value() or not cell_cfg.ran.ntn_params->tar_cfg.has_value()) {
    return;
  }

  const auto band_it = ue_caps.bands.find(cell_cfg.ran.dl_carrier.band);
  if (band_it == ue_caps.bands.end() or not band_it->second.ul_ta_reporting_supported) {
    return;
  }

  tar_config tar = *cell_cfg.ran.ntn_params->tar_cfg;
  if (tar.sr_enabled and not ue_caps.sr_triggered_by_ta_report_supported) {
    // The UE would not raise the SR anyway, so dropping the field is the only correct outcome. Logged because the cell
    // asked for it and the operator otherwise has no way to tell why the feature ended up off for this UE.
    logger.info("ue={}: Not signalling timingAdvanceSR. Cause: UE does not support sr-TriggeredBy-TA-Report", ue_index);
    tar.sr_enabled = false;
  }
  cell_grp_cfg.mcg_cfg.tar_cfg.emplace(tar);
}

unsigned du_ran_resource_manager_impl::get_max_nof_established_ue_contexts(du_cell_index_t cell_index) const
{
  return cell_ue_ctxts[cell_index].max_nof_established;
}

unsigned du_ran_resource_manager_impl::get_max_nof_rejected_ue_contexts(du_cell_index_t /* unused */) const
{
  return max_nof_rejected_ue_ctxts;
}

unsigned du_ran_resource_manager_impl::compute_max_nof_established_ue_ctxts(du_cell_index_t cell_index) const
{
  const auto& cell = cell_cfg_list[cell_index];

  // The number of supported UEs is bounded by the lowest of the available SR, CSI and SRS resources.
  unsigned max_nof_ues = pucch_res_mng.get_nof_free_sr_configs(cell_index);
  if (cell.ran.init_bwp.csi.has_value() and cell.ran.init_bwp.csi->csi_report_slot_offset.has_value()) {
    max_nof_ues = std::min(max_nof_ues, pucch_res_mng.get_nof_free_csi_configs(cell_index));
  }
  if (cell.ran.init_bwp.srs_cfg.srs_type_enabled == srs_type::periodic) {
    max_nof_ues = std::min(max_nof_ues, srs_res_mng->get_nof_srs_free_res_offsets(cell_index));
  }

  // The UE contexts reserved for RRC Rejects have to fit within the maximum number of UE contexts of a cell.
  return std::min(max_nof_ues, max_nof_ue_ctxts_per_cell - max_nof_rejected_ue_ctxts);
}

void du_ran_resource_manager_impl::move_ue_ctxt_count(ue_resource_context& u, du_cell_index_t new_pcell_index)
{
  if (u.counted_cell_index != INVALID_DU_CELL_INDEX) {
    --cell_ue_ctxts[u.counted_cell_index].nof_allocated;
  }
  if (new_pcell_index != INVALID_DU_CELL_INDEX) {
    ++cell_ue_ctxts[new_pcell_index].nof_allocated;
  }
  u.counted_cell_index = new_pcell_index;
}

expected<ue_ran_resource_configurator, std::string>
du_ran_resource_manager_impl::create_ue_resource_configurator(du_ue_index_t   ue_index,
                                                              du_cell_index_t pcell_index,
                                                              bool            has_tc_rnti)
{
  if (ue_res_pool.contains(ue_index)) {
    return make_unexpected(std::string("Double allocation of same UE not supported"));
  }

  cell_ue_context_count& cell_ctxts = cell_ue_ctxts[pcell_index];
  if (cell_ctxts.full()) {
    logger.warning("cell={}: Refused new UE context. Cause: Reached the maximum number of UE contexts ({} for "
                   "established UEs and {} for UEs to be RRC Rejected)",
                   pcell_index,
                   cell_ctxts.max_nof_established,
                   max_nof_rejected_ue_ctxts);
    return make_unexpected(fmt::format("No UE contexts left in cell={}", pcell_index));
  }
  ++cell_ctxts.nof_allocated;

  ue_res_pool.emplace(ue_index, *this);
  auto& ue_res              = ue_res_pool[ue_index];
  auto& mcg                 = ue_res.cg_cfg;
  ue_res.counted_cell_index = pcell_index;

  // UE initialized PCell.
  // Note: In case of lack of RAN resource availability, the return will be error type.
  error_type<std::string> err = allocate_cell_resources(ue_index, pcell_index, SERVING_PCELL_IDX);
  if (not err.has_value()) {
    logger.info("Failed to create a configuration for ue={}. Cause: {}", ue_index, err.error());
  }

  // Initialize UE with DRX disabled.
  drx_res_mng.handle_ue_creation(ue_res.cg_cfg.cell_group);

  // Allocate CFRA resources when TC-RNTI was not yet assigned (e.g. during for Handover). A UE without PCell resources
  // cannot complete the handover.
  if (not has_tc_rnti and err.has_value()) {
    ra_res_alloc.allocate_cfra_resources(ue_res.cg_cfg);
  }

  return ue_ran_resource_configurator{
      std::make_unique<du_ue_ran_resource_updater_impl>(&mcg, ue_res.ue_cap_manager.summary(), *this, ue_index),
      err.has_value() ? std::string{} : err.error()};
}

du_ue_resource_update_response
du_ran_resource_manager_impl::update_context(du_ue_index_t                         ue_index,
                                             du_cell_index_t                       pcell_idx,
                                             const f1ap_ue_context_update_request& upd_req,
                                             const du_ue_resource_config*          reestablished_context,
                                             const ue_capability_summary*          reestablished_ue_caps)
{
  ocudu_assert(ue_res_pool.contains(ue_index), "This function should only be called for an already allocated UE");
  ue_resource_context&           u      = ue_res_pool[ue_index];
  du_ue_resource_config&         ue_mcg = u.cg_cfg;
  du_ue_resource_update_response resp;

  // > Deallocate resources for previously configured cells that have now been removed or changed.
  if (ue_mcg.cell_group.cells.contains(SERVING_PCELL_IDX) and
      ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.cell_index != pcell_idx) {
    // >> PCell changed. The new cell needs to have room for the UE context, and it is checked before any resource is
    // released, so that the UE is left untouched if the update cannot proceed.
    if (cell_ue_ctxts[pcell_idx].full()) {
      resp.procedure_error = make_unexpected(fmt::format("No UE contexts left in cell={}", pcell_idx));
      return resp;
    }

    // >> Deallocate PCell resources.
    move_ue_ctxt_count(u, pcell_idx);
    deallocate_cell_resources(ue_index, SERVING_PCELL_IDX);
  }
  for (serv_cell_index_t scell_idx : upd_req.scells_to_rem) {
    // >> SCells to be removed. Deallocate them.
    deallocate_cell_resources(ue_index, scell_idx);
  }
  for (const f1ap_scell_to_setup& scell : upd_req.scells_to_setup) {
    // >> If SCells to be modified changed DU Cell Index.
    if (ue_mcg.cell_group.cells.contains(scell.serv_cell_index) and
        ue_mcg.cell_group.cells.at(scell.serv_cell_index).serv_cell_cfg.cell_index != scell.cell_index) {
      deallocate_cell_resources(ue_index, scell.serv_cell_index);
    }
  }

  // > Allocate resources for new or modified cells.
  bool pcell_newly_allocated = false;
  if (not ue_mcg.cell_group.cells.contains(SERVING_PCELL_IDX) or
      ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.cell_index != pcell_idx) {
    // >> PCell changed. Allocate new PCell resources.
    const error_type<std::string> outcome = allocate_cell_resources(ue_index, pcell_idx, SERVING_PCELL_IDX);
    if (not outcome.has_value()) {
      resp.procedure_error = outcome;
      return resp;
    }
    pcell_newly_allocated = true;
  }
  for (const f1ap_scell_to_setup& sc : upd_req.scells_to_setup) {
    // >> SCells Added/Modified. Allocate new SCell resources.
    if (not allocate_cell_resources(ue_index, sc.cell_index, sc.serv_cell_index).has_value()) {
      resp.failed_scells.push_back(sc.serv_cell_index);
    }
  }

  // > Process UE NR capabilities and update UE dedicated configuration only if test mode is not configured.
  if (not test_cfg.test_ue.has_value() or test_cfg.test_ue->rnti == rnti_t::INVALID_RNTI) {
    if (reestablished_ue_caps != nullptr) {
      u.ue_cap_manager.update(*reestablished_ue_caps);
    }
    if (not upd_req.ue_cap_rat_list.empty()) {
      u.ue_cap_manager.update(upd_req.ue_cap_rat_list);
    } else if (not upd_req.ho_prep_info.empty()) {
      u.ue_cap_manager.update_from_ho_prep_info(upd_req.ho_prep_info);
    }
  }

  // Update measGaps based on the UE measConfig, location measurements and supportedGapPatterns UE capabilities.
  const ue_capability_summary* ue_caps_summary =
      u.ue_cap_manager.summary().has_value() ? &*u.ue_cap_manager.summary() : nullptr;
  if (not meas_cfg_mng.update(ue_mcg, upd_req.meas_cfg, upd_req.location_meas_info, ue_caps_summary)) {
    resp.procedure_error = make_unexpected(std::string{"Unable to configure a measurement gap for the PRS"});
    return resp;
  }

  if (u.ue_cap_manager.summary().has_value()) {
    pdsch_res_mng.update_resources(ue_mcg.cell_group, *u.ue_cap_manager.summary());
    pusch_res_mng.update_resources(ue_mcg.cell_group, *u.ue_cap_manager.summary());
    drx_res_mng.handle_ue_cap_update(ue_mcg.cell_group, u.ue_cap_manager.summary());
    if (ue_mcg.cell_group.cells.contains(SERVING_PCELL_IDX)) {
      pucch_res_mng.update_resources(ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX), *u.ue_cap_manager.summary());
      csi_res_mng.update_resources(
          ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX), *u.ue_cap_manager.summary(), ue_index);
    }
    update_tar_config(ue_mcg.cell_group, cell_cfg_list, *u.ue_cap_manager.summary(), ue_index, logger);
    ue_mcg.ue_specific_koffset_supported =
        supports_ue_specific_koffset(ue_mcg.cell_group, cell_cfg_list, *u.ue_cap_manager.summary());
  }

  // > Update UE SRBs and DRBs.
  du_ue_bearer_resource_update_response bearer_resp =
      bearer_res_mng.update(ue_mcg,
                            du_ue_bearer_resource_update_request{
                                upd_req.srbs_to_setup, upd_req.drbs_to_setup, upd_req.drbs_to_mod, upd_req.drbs_to_rem},
                            reestablished_context);
  resp.failed_drbs = std::move(bearer_resp.drbs_failed_to_setup);
  resp.failed_drbs.insert(
      resp.failed_drbs.end(), bearer_resp.drbs_failed_to_mod.begin(), bearer_resp.drbs_failed_to_mod.end());

  // > Allocate or deallocate Configured Grant resources based on DRB presence.
  if (ue_mcg.cell_group.cells.contains(SERVING_PCELL_IDX)) {
    const bool has_drbs = not ue_mcg.drbs.empty();
    const bool cg_was_active =
        ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.has_value() and
        ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value();
    if (has_drbs and not cg_was_active) {
      // NOTE: cg_res_mng.build_ue_cg_config returns true if allocation is successful or if the Configured grant
      // resource allocation was not requested (i.e., not set in by the user).
      if (not cg_res_mng->build_ue_cg_config(ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX))) {
        // NOTE: This is ONLY IF CG allocation was requested and failed.
        // Deallocate previously allocated resources on PCell.
        if (pcell_newly_allocated) {
          deallocate_cell_resources(ue_index, SERVING_PCELL_IDX);
        }
        // Deallocate previously allocated resources on SCells (that didn't fail).
        for (const f1ap_scell_to_setup& sc : upd_req.scells_to_setup) {
          if (std::find(resp.failed_scells.begin(), resp.failed_scells.end(), sc.serv_cell_index) ==
              resp.failed_scells.end()) {
            deallocate_cell_resources(ue_index, sc.serv_cell_index);
          }
        }
        resp.procedure_error =
            make_unexpected(fmt::format("Unable to allocate CG resources for ue={} at cell={}", ue_index, pcell_idx));
      }
      // After this point, allocation succeeded (either CG resources were allocated or allocation was skipped due to CG
      // config not requested).
      else if (ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.has_value() and
               ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg.has_value()) {
        // CG was actually allocated; if CG was not requested, the DU res. manager does not enter this else-if block.
        resp.cs_rnti_requested = true;
      }
      // NOTE: no-op for case with CG config not requested.
    }
    // The UE keeps its CG config, so it keeps needing its CS-RNTI; otherwise the MAC would release it.
    else if (has_drbs and cg_was_active) {
      resp.cs_rnti_requested = true;
    }
    // Remove old CG config when DRBs are removed.
    else if (not has_drbs and cg_was_active) {
      cg_res_mng->reset_ue_cg_config(ue_mcg.cell_group.cells.at(SERVING_PCELL_IDX));
    }
  }

  return resp;
}

void du_ran_resource_manager_impl::deallocate_context(du_ue_index_t ue_index)
{
  ocudu_assert(ue_res_pool.contains(ue_index), "This function should only be called for an already allocated UE");
  ue_resource_context&   ue_res      = ue_res_pool[ue_index];
  du_ue_resource_config& ue_mcg      = ue_res.cg_cfg;
  const du_cell_index_t  pcell_index = ue_res.counted_cell_index;

  ra_res_alloc.deallocate_cfra_resources(ue_mcg);
  drx_res_mng.handle_ue_removal(ue_mcg.cell_group);

  // Each deallocation removes the cell from the list.
  while (not ue_mcg.cell_group.cells.empty()) {
    deallocate_cell_resources(ue_index, ue_mcg.cell_group.cells.begin()->first);
  }

  ue_res_pool.erase(ue_index);

  if (pcell_index != INVALID_DU_CELL_INDEX) {
    --cell_ue_ctxts[pcell_index].nof_allocated;
  }
}

void du_ran_resource_manager_impl::ue_config_applied(du_ue_index_t ue_index)
{
  ocudu_assert(ue_res_pool.contains(ue_index), "This function should only be called for an already allocated UE");
  ue_resource_context&   ue_res = ue_res_pool[ue_index];
  du_ue_resource_config& ue_mcg = ue_res.cg_cfg;

  // We can remove previously used CFRA resources, if any.
  ra_res_alloc.deallocate_cfra_resources(ue_mcg);
}

error_type<std::string> du_ran_resource_manager_impl::allocate_cell_resources(du_ue_index_t     ue_index,
                                                                              du_cell_index_t   cell_index,
                                                                              serv_cell_index_t serv_cell_index)
{
  du_ue_resource_config& ue_res = ue_res_pool[ue_index].cg_cfg;

  const du_cell_config& cell_cfg_cmn = cell_cfg_list[cell_index];

  if (serv_cell_index == SERVING_PCELL_IDX) {
    // It is a PCell.
    ocudu_assert(not ue_res.cell_group.cells.contains(SERVING_PCELL_IDX), "Reallocation of PCell detected");
    ue_res.cell_group.cells.emplace(SERVING_PCELL_IDX,
                                    config_helpers::make_default_ue_cell_config(cell_cfg_cmn.ran, cell_index));
    ue_res.cell_group.mcg_cfg = config_helpers::make_initial_mac_cell_group_config(cell_cfg_cmn.mcg_params);
    // TODO: Move to helper.
    if (cell_cfg_cmn.pcg_params.p_nr_fr1.has_value()) {
      ue_res.cell_group.pcg_cfg.p_nr_fr1 = cell_cfg_cmn.pcg_params.p_nr_fr1->value();
    }
    ue_res.cell_group.pcg_cfg.pdsch_harq_codebook = pdsch_harq_ack_codebook::dynamic;

    reset_serv_cell_cfg(ue_res.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg);

    // On failure, the UE keeps no dedicated configuration. It uses only the common configuration of the cell.
    if (not srs_res_mng->alloc_resources(ue_res.cell_group.cells.at(SERVING_PCELL_IDX))) {
      ue_res.cell_group.cells.erase(SERVING_PCELL_IDX);
      return make_unexpected(fmt::format("Unable to allocate SRS resources for cell={}", cell_index));
    }

    if (not pucch_res_mng.alloc_resources(ue_res.cell_group.cells.at(SERVING_PCELL_IDX))) {
      srs_res_mng->dealloc_resources(ue_res.cell_group.cells.at(SERVING_PCELL_IDX));
      ue_res.cell_group.cells.erase(SERVING_PCELL_IDX);
      return make_unexpected(fmt::format("Unable to allocate dedicated PUCCH resources for cell={}", cell_index));
    }

    pdsch_res_mng.alloc_resources(ue_res.cell_group);
    pusch_res_mng.alloc_resources(ue_res.cell_group);
    csi_res_mng.alloc_resources(ue_res.cell_group.cells.at(SERVING_PCELL_IDX), ue_index);
  } else {
    ocudu_assert(not ue_res.cell_group.cells.contains(serv_cell_index), "Reallocation of SCell detected");
    ue_res.cell_group.cells.emplace(serv_cell_index, ue_cell_config{});
    ue_res.cell_group.cells.at(serv_cell_index) =
        config_helpers::make_default_ue_cell_config(cell_cfg_cmn.ran, cell_index);
    // TODO: Allocate SCell params.
  }
  return {};
}

void du_ran_resource_manager_impl::deallocate_cell_resources(du_ue_index_t ue_index, serv_cell_index_t serv_cell_index)
{
  ue_resource_context&   ue_res_updater = ue_res_pool[ue_index];
  du_ue_resource_config& ue_res         = ue_res_updater.cg_cfg;

  ocudu_assert(ue_res.cell_group.cells.contains(serv_cell_index),
               "ue={}: Deallocation of serv_cell_index={}, which the UE does not have",
               ue_index,
               serv_cell_index);

  // Return resources back to free lists. SCells hold no resources yet.
  if (serv_cell_index == SERVING_PCELL_IDX) {
    pucch_res_mng.dealloc_resources(ue_res.cell_group.cells.at(SERVING_PCELL_IDX));
    srs_res_mng->dealloc_resources(ue_res.cell_group.cells.at(SERVING_PCELL_IDX));
    pdsch_res_mng.dealloc_resources(ue_res.cell_group);
    pusch_res_mng.dealloc_resources(ue_res.cell_group);
    cg_res_mng->reset_ue_cg_config(ue_res.cell_group.cells.at(SERVING_PCELL_IDX));
  }
  ue_res.cell_group.cells.erase(serv_cell_index);
}

du_ran_resource_manager_impl::ue_resource_context::ue_resource_context(du_ran_resource_manager_impl& parent) :
  ue_cap_manager(parent.cell_cfg_list, parent.logger)
{
}
