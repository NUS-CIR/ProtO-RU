// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/du/du_high/du_manager/ran_resource_management/du_ran_resource_manager_impl.h"
#include "tests/ocudu_test_requirements.h"
#include "ocudu/du/du_cell_config_helpers.h"
#include "ocudu/du/du_high/du_qos_config_helpers.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace odu;

namespace {

/// Number of CSI-RS antenna ports the cell under test is configured with.
constexpr unsigned cell_nof_ports = 4;
/// Number of Type-II beams the cell under test configures.
constexpr unsigned cell_nof_beams = 2;

/// Builds a cell configuration that enables the Type-II codebook for its CSI reports.
du_cell_config create_du_cell_config_with_type2(const cell_config_builder_params& params)
{
  du_cell_config cell = config_helpers::make_default_du_cell_config(params);

  // The Type-II codebook requires four or more CSI-RS ports, and aperiodic CSI reporting, as its PMI is carried in
  // CSI Part 2.
  cell.ran.dl_carrier.nof_ant = cell_nof_ports;

  report_fatal_error_if_not(cell.ran.init_bwp.csi.has_value(), "The cell does not configure CSI");
  cell.ran.init_bwp.csi->csi_report_slot_offset.reset();
  cell.ran.init_bwp.csi->enable_aperiodic_report = true;

  du_type2_codebook_params& type2 = cell.ran.init_bwp.csi->type2_codebook.emplace();
  type2.nof_beams                 = cell_nof_beams;
  type2.phase_alphabet_size       = pmi_codebook_typeII_phase_size::qpsk;

  return cell;
}

/// Builds UE capabilities declaring the given Type-II codebook support for the band of the cell under test.
ue_capability_summary make_type2_caps(nr_band band, std::optional<ue_capability_summary::type2_codebook_params> type2)
{
  ue_capability_summary                 caps;
  ue_capability_summary::supported_band band_caps;
  band_caps.type2_codebook = type2;
  caps.bands.emplace(band, band_caps);

  return caps;
}

/// Type-II capabilities that cover everything the cell under test configures.
ue_capability_summary::type2_codebook_params make_supported_type2_caps()
{
  ue_capability_summary::type2_codebook_params type2;
  type2.max_nof_beams                 = 4;
  type2.max_nof_tx_ports_per_resource = 8;
  type2.subband_amplitude_supported   = true;

  return type2;
}

class du_csi_codebook_config_tester : public ::testing::Test
{
protected:
  explicit du_csi_codebook_config_tester(du_test_mode_config test_mode_cfg_ = {}) :
    test_mode_cfg(test_mode_cfg_),
    cell_cfg_list({create_du_cell_config_with_type2(params)}),
    qos_cfg_list(config_helpers::make_default_du_qos_config_list(/* warn_on_drop */ true, 1000)),
    res_mng(cell_cfg_list,
            scheduler_expert_config{.ue = {.max_pucchs_per_slot = 31}},
            srb_cfg_list,
            qos_cfg_list,
            test_mode_cfg)
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-2");

    auto result = res_mng.create_ue_resource_configurator(ue_idx, to_du_cell_index(0), true);
    report_fatal_error_if_not(result.has_value(), "Failed to create UE resources");
    ue_res.emplace(std::move(result.value()));
  }

  /// Runs a UE configuration carrying the given capabilities and returns the resulting codebook configuration.
  const codebook_config& update_ue_caps(const ue_capability_summary& caps)
  {
    f1ap_ue_context_update_request req;
    req.ue_index = ue_idx;
    req.srbs_to_setup.push_back(srb_id_t::srb1);

    const du_ue_resource_update_response resp = ue_res->update(to_du_cell_index(0), req, nullptr, &caps);
    report_fatal_error_if_not(not resp.failed(), "UE configuration failed");

    return current_codebook();
  }

  /// Returns the codebook configuration currently held by the UE.
  const codebook_config& current_codebook() const
  {
    const ue_cell_config&      ue_cell_cfg   = ue_res->value().cell_group.cells.at(SERVING_PCELL_IDX);
    const serving_cell_config& serv_cell_cfg = ue_cell_cfg.serv_cell_cfg;
    report_fatal_error_if_not(serv_cell_cfg.csi_meas_cfg.has_value(), "No CSI meas config for the UE");
    for (const csi_report_config& rep : serv_cell_cfg.csi_meas_cfg->csi_report_cfg_list) {
      if (rep.codebook_cfg.has_value()) {
        // The scheduler reads the codebook from the UE BWP configuration. It must match the RRC copy.
        const csi_codebook_type rrc_codebook =
            std::holds_alternative<codebook_config::type2>(rep.codebook_cfg->codebook_type) ? csi_codebook_type::type2
                                                                                            : csi_codebook_type::type1;
        EXPECT_EQ(ue_cell_cfg.bwps[0].ul.csi_codebook, rrc_codebook);
        return rep.codebook_cfg.value();
      }
    }
    report_fatal_error("No CSI report with a codebook config");
  }

  nr_band cell_band() const { return cell_cfg_list[0].ran.dl_carrier.band; }

  static constexpr du_ue_index_t ue_idx = to_du_ue_index(0);

  cell_config_builder_params                  params{};
  du_test_mode_config                         test_mode_cfg;
  std::vector<du_cell_config>                 cell_cfg_list;
  std::map<srb_id_t, du_srb_config>           srb_cfg_list;
  std::map<five_qi_t, du_qos_config>          qos_cfg_list;
  du_ran_resource_manager_impl                res_mng;
  std::optional<ue_ran_resource_configurator> ue_res;
};

/// Runs the same cell in test mode, where the UE capabilities are never decoded.
class du_csi_codebook_config_test_mode_tester : public du_csi_codebook_config_tester
{
protected:
  du_csi_codebook_config_test_mode_tester() :
    du_csi_codebook_config_tester(du_test_mode_config{.test_ue = du_test_mode_config::test_mode_ue_config{}})
  {
  }
};

} // namespace

TEST_F(du_csi_codebook_config_tester, type1_codebook_is_configured_before_the_capabilities_are_decoded)
{
  // Signalling the Type-II codebook to a UE whose capabilities are unknown would ask for a report it may not be able
  // to produce.
  ASSERT_TRUE(std::holds_alternative<codebook_config::type1>(current_codebook().codebook_type));
}

TEST_F(du_csi_codebook_config_test_mode_tester, type2_codebook_is_configured_for_a_test_mode_ue)
{
  // The test mode UE reports no capabilities, so the cell configuration is applied as is.
  ASSERT_TRUE(std::holds_alternative<codebook_config::type2>(current_codebook().codebook_type));
}

TEST_F(du_csi_codebook_config_tester, type2_codebook_is_configured_for_a_capable_ue)
{
  const codebook_config& codebook = update_ue_caps(make_type2_caps(cell_band(), make_supported_type2_caps()));

  ASSERT_TRUE(std::holds_alternative<codebook_config::type2>(codebook.codebook_type))
      << "The Type-II codebook must be kept for a UE that supports it";
}

TEST_F(du_csi_codebook_config_tester, type1_codebook_is_configured_for_a_ue_without_type2_support)
{
  // Signalling the Type-II codebook would ask for a report the UE cannot produce.
  const codebook_config& codebook = update_ue_caps(make_type2_caps(cell_band(), std::nullopt));

  ASSERT_TRUE(std::holds_alternative<codebook_config::type1>(codebook.codebook_type));
}

TEST_F(du_csi_codebook_config_tester, type1_codebook_is_configured_when_the_ue_supports_fewer_beams)
{
  ue_capability_summary::type2_codebook_params type2_caps = make_supported_type2_caps();
  type2_caps.max_nof_beams                                = cell_nof_beams - 1;

  const codebook_config& codebook = update_ue_caps(make_type2_caps(cell_band(), type2_caps));

  ASSERT_TRUE(std::holds_alternative<codebook_config::type1>(codebook.codebook_type));
}

TEST_F(du_csi_codebook_config_tester, type1_codebook_is_configured_when_the_ue_supports_fewer_ports)
{
  ue_capability_summary::type2_codebook_params type2_caps = make_supported_type2_caps();
  type2_caps.max_nof_tx_ports_per_resource                = cell_nof_ports - 1;

  const codebook_config& codebook = update_ue_caps(make_type2_caps(cell_band(), type2_caps));

  ASSERT_TRUE(std::holds_alternative<codebook_config::type1>(codebook.codebook_type));
}

TEST_F(du_csi_codebook_config_tester, type1_codebook_is_configured_for_a_ue_that_does_not_report_the_band)
{
  // The capabilities are reported per band, so a UE that does not list the band of the cell cannot be assumed to
  // support the Type-II codebook in it.
  const ue_capability_summary caps = make_type2_caps(static_cast<nr_band>(0xff), make_supported_type2_caps());

  ASSERT_TRUE(std::holds_alternative<codebook_config::type1>(update_ue_caps(caps).codebook_type));
}
