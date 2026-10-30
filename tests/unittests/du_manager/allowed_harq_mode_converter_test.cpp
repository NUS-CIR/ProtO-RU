// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/du/du_high/du_manager/converters/asn1_rrc_config_helpers.h"
#include "ocudu/mac/config/mac_config_helpers.h"
#include "ocudu/rlc/rlc_srb_config_factory.h"
#include "ocudu/support/error_handling.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocudu::odu;
using namespace asn1::rrc_nr;

namespace {

using ul_specific_params = lc_ch_cfg_s::ul_specific_params_s_;

/// Builds a UE resource configuration holding SRB1 with the given allowed HARQ mode, if any.
du_ue_resource_config make_ue_resource_config(std::optional<ul_harq_mode> allowed_harq_mode)
{
  du_ue_resource_config cfg{};
  cfg.srbs.emplace(srb_id_t::srb1);
  du_ue_srb_config& srb         = cfg.srbs[srb_id_t::srb1];
  srb.srb_id                    = srb_id_t::srb1;
  srb.rlc_cfg                   = make_default_srb_rlc_config();
  srb.mac_cfg                   = make_default_srb_mac_lc_config(LCID_SRB1);
  srb.mac_cfg.allowed_harq_mode = allowed_harq_mode;
  return cfg;
}

/// Returns the uplink specific parameters of the only bearer of the generated cell group.
ul_specific_params generate_ul_specific_params(std::optional<ul_harq_mode> allowed_harq_mode)
{
  const du_ue_resource_config src{};
  const du_ue_resource_config dest = make_ue_resource_config(allowed_harq_mode);

  cell_group_cfg_s cell_group;
  calculate_cell_group_config_diff(cell_group, src, dest);

  report_error_if_not(cell_group.rlc_bearer_to_add_mod_list.size() == 1, "Expected a single bearer");
  return cell_group.rlc_bearer_to_add_mod_list[0].mac_lc_ch_cfg.ul_specific_params;
}

} // namespace

/// TS 38.331 makes allowedHARQ-mode optional, and an absent one leaves the mapping unrestricted.
TEST(allowed_harq_mode_converter_test, an_unset_mode_is_not_packed)
{
  const ul_specific_params ul_params = generate_ul_specific_params(std::nullopt);

  EXPECT_FALSE(ul_params.allowed_harq_mode_r17_present);
}

/// TS 38.331 places allowedHARQ-mode in an extension group.
TEST(allowed_harq_mode_converter_test, mode_a_is_packed_in_the_extension_group)
{
  const ul_specific_params ul_params = generate_ul_specific_params(ul_harq_mode::mode_a);
  ASSERT_TRUE(ul_params.allowed_harq_mode_r17_present);
  EXPECT_EQ(ul_params.allowed_harq_mode_r17.value, ul_specific_params::allowed_harq_mode_r17_opts::harq_mode_a);
}

TEST(allowed_harq_mode_converter_test, mode_b_is_packed_in_the_extension_group)
{
  const ul_specific_params ul_params = generate_ul_specific_params(ul_harq_mode::mode_b);
  ASSERT_TRUE(ul_params.allowed_harq_mode_r17_present);
  EXPECT_EQ(ul_params.allowed_harq_mode_r17.value, ul_specific_params::allowed_harq_mode_r17_opts::harq_mode_b);
}

/// A packed configuration has to survive a round trip, since the extension group is only read back when \c ext is set.
TEST(allowed_harq_mode_converter_test, a_packed_mode_survives_an_encode_decode_round_trip)
{
  const du_ue_resource_config src{};
  const du_ue_resource_config dest = make_ue_resource_config(ul_harq_mode::mode_b);

  cell_group_cfg_s cell_group;
  calculate_cell_group_config_diff(cell_group, src, dest);

  byte_buffer   packed;
  asn1::bit_ref bref{packed};
  ASSERT_EQ(cell_group.pack(bref), asn1::OCUDUASN_SUCCESS);

  cell_group_cfg_s decoded;
  asn1::cbit_ref   dec_bref{packed};
  ASSERT_EQ(decoded.unpack(dec_bref), asn1::OCUDUASN_SUCCESS);

  ASSERT_EQ(decoded.rlc_bearer_to_add_mod_list.size(), 1);
  const ul_specific_params& ul_params = decoded.rlc_bearer_to_add_mod_list[0].mac_lc_ch_cfg.ul_specific_params;
  ASSERT_TRUE(ul_params.allowed_harq_mode_r17_present);
  EXPECT_EQ(ul_params.allowed_harq_mode_r17.value, ul_specific_params::allowed_harq_mode_r17_opts::harq_mode_b);
}
