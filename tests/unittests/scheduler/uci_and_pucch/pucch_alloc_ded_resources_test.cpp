// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "pucch_alloc_base_tester.h"
#include "uci_test_utils.h"
#include "ocudu/ran/csi_report/csi_report_config_helpers.h"
#include "ocudu/ran/csi_report/csi_report_on_pucch_helpers.h"
#include "ocudu/ran/pucch/pucch_configuration.h"
#include "ocudu/scheduler/config/pucch_resource_builder_params.h"
#include <gtest/gtest.h>

using namespace ocudu;

///////   Test allocation of dedicated PUCCH resources    ///////

class pucch_alloc_ded_resources_test : public ::testing::TestWithParam<pucch_format>, public pucch_allocator_base_test
{
public:
  pucch_alloc_ded_resources_test() :
    pucch_allocator_base_test({.pucch_ded_params = [format = GetParam()]() -> pucch_resource_builder_params {
      pucch_resource_builder_params params{.res_set_size = 3};
      switch (format) {
        case pucch_format::FORMAT_2:
          params.f2_or_f3_or_f4_params.emplace<pucch_f2_params>();
          break;
        case pucch_format::FORMAT_3:
          params.f2_or_f3_or_f4_params.emplace<pucch_f3_params>();
          break;
        case pucch_format::FORMAT_4:
          params.f2_or_f3_or_f4_params.emplace<pucch_f4_params>();
          break;
        default:
          ocudu_assertion_failure("Invalid PUCCH Format");
      }
      return params;
    }()})
  {
    const auto& cell_resources = t_bench.cell_cfg.bwp_res[to_bwp_id(0)].ul().pucch;
    const auto& res_params     = t_bench.params.pucch_ded_params;

    const auto sr_res_id      = pucch_sr_resource_id(0);
    const auto res_set_cfg_id = pucch_resource_set_config_id(0);
    const auto csi_res_id     = pucch_csi_resource_id(0);

    // Set the expected SR grant to the SR resource.
    pucch_expected_sr = test_helpers::make_pucch_info(
        t_bench.cell_cfg, cell_resources.get_ded(res_params.sr_res_id(sr_res_id)), {.sr_bits = sr_nof_bits::one});

    // Set the expected Resource Set ID 0 HARQ-ACK grant to the first resource in Resource Set ID 0.
    pucch_expected_res_set_0 =
        test_helpers::make_pucch_info(t_bench.cell_cfg,
                                      cell_resources.get_ded(res_params.harq_res_id<0>(res_set_cfg_id, 0)),
                                      {.harq_ack_nof_bits = 1U});

    // Set the expected Resource Set ID 1 HARQ-ACK grant to the first resource in Resource Set ID 1.
    pucch_expected_res_set_1 =
        test_helpers::make_pucch_info(t_bench.cell_cfg,
                                      cell_resources.get_ded(res_params.harq_res_id<1>(res_set_cfg_id, 0)),
                                      {.harq_ack_nof_bits = 3U});

    // Set the expected HARQ CSI grant to the CSI resource.
    pucch_expected_csi = test_helpers::make_pucch_info(t_bench.cell_cfg,
                                                       cell_resources.get_ded(res_params.csi_res_id(csi_res_id)),
                                                       {.csi_part1_nof_bits = default_csi_part1_bits});
    ocudu_assert(pucch_expected_res_set_1.format() == GetParam(),
                 "PUCCH format mismatch between test parameter and expected grant");
    ocudu_assert(pucch_expected_csi.format() == GetParam(),
                 "PUCCH format mismatch between test parameter and expected grant");
  }

protected:
  static constexpr unsigned default_csi_part1_bits = 4U;

  // Parameters that are passed by the routine to run the tests.
  pucch_info pucch_expected_sr;
  pucch_info pucch_expected_res_set_0;
  pucch_info pucch_expected_res_set_1;
  pucch_info pucch_expected_csi;
};

/////////////// Tests PUCCH allocator for SR.

TEST_P(pucch_alloc_ded_resources_test, alloc_sr_opportunity_succeeds)
{
  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));

  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(
      default_slot_grid.result_reader().ul().pucchs().unsorted(),
      [&expected = pucch_expected_sr](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
}

TEST_P(pucch_alloc_ded_resources_test, alloc_sr_opportunity_fails_when_no_free_sr_resources)
{
  t_bench.add_ue();
  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_ue(t_bench.last_added_ue_idx)));
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());

  // Try to allocate SR for the main UE, which should fail as there are no more free SR resources.
  ASSERT_FALSE(alloc_sr_opportunity(t_bench.get_main_ue()));
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
}

/////////////// Tests PUCCH allocator for CSI.

TEST_P(pucch_alloc_ded_resources_test, alloc_csi_opportunity_succeeds)
{
  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_main_ue()));

  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(
      default_slot_grid.result_reader().ul().pucchs().unsorted(),
      [&expected = pucch_expected_csi](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
  ASSERT_TRUE(default_slot_grid.result_reader().ul().pucchs().begin()->csi_rep_cfg.has_value());
}

TEST_P(pucch_alloc_ded_resources_test,
       alloc_csi_opportunity_when_existing_sr_succeeds_and_grants_are_multiplexed_on_csi_resource)
{
  pucch_expected_csi.uci_bits.sr_bits = sr_nof_bits::one;

  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_main_ue()));

  // The CSI and SR should be multiplexed into the CSI resource.
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(
      default_slot_grid.result_reader().ul().pucchs().unsorted(),
      [&expected = pucch_expected_csi](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
  ASSERT_TRUE(default_slot_grid.result_reader().ul().pucchs().begin()->csi_rep_cfg.has_value());
}

TEST_P(pucch_alloc_ded_resources_test, alloc_csi_opportunity_fails_when_no_free_csi_resources)
{
  t_bench.add_ue();
  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_ue(t_bench.last_added_ue_idx)));
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());

  // Try to allocate CSI for the main UE, which should fail as there are no more free CSI resources.
  ASSERT_FALSE(alloc_csi_opportunity(t_bench.get_main_ue()));
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
}

///////  Test HARQ-ACK allocation on ded. resources  - Resource Set ID 0   ///////

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_succeeds)
{
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());

  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(0U, pri);
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(
      default_slot_grid.result_reader().ul().pucchs().unsorted(),
      [&expected = pucch_expected_res_set_0](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
}

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_with_existing_sr_succeeds)
{
  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  for (unsigned i = 0; i != 2U; ++i) {
    pucch_expected_sr.uci_bits.harq_ack_nof_bits        = i + 1;
    pucch_expected_res_set_0.uci_bits.harq_ack_nof_bits = i + 1;

    auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
    ASSERT_TRUE(pri.has_value());
    ASSERT_EQ(0U, pri.value());

    // Expect 2 PUCCH grants, one for SR and one for HARQ.
    ASSERT_EQ(2U, default_slot_grid.result_reader().ul().pucchs().size());
    ASSERT_TRUE(find_pucch_pdu(
        default_slot_grid.result_reader().ul().pucchs().unsorted(),
        [&expected = pucch_expected_sr](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
    ASSERT_TRUE(find_pucch_pdu(
        default_slot_grid.result_reader().ul().pucchs().unsorted(),
        [&expected = pucch_expected_res_set_0](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
  }

  pucch_expected_res_set_1.uci_bits.harq_ack_nof_bits = 3U;
  pucch_expected_res_set_1.uci_bits.sr_bits           = sr_nof_bits::one;

  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());

  // The HARQ grant should be promoted to Resource Set ID 1 and SR should be multiplexed there.
  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(0U, pri.value());
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(
      default_slot_grid.result_reader().ul().pucchs().unsorted(),
      [&expected = pucch_expected_res_set_1](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
}

// TODO: add the same test but with existing CSI/SR grants.
TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_succeeds_until_max_payload_reached)
{
  const unsigned max_payload_f2_f3_f4 = t_bench.params.pucch_ded_params.max_payload_234();

  // Add HARQ grants to reach the max_payload.
  for (unsigned n = 0; n != max_payload_f2_f3_f4; ++n) {
    // These grants should be allocated successfully, as we are below the max payload.
    auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
    ASSERT_TRUE(pri.has_value());
    ASSERT_EQ(0U, pri.value());

    ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());

    const unsigned harq_ack_nof_bits = n + 1;
    if (harq_ack_nof_bits <= 2U) {
      pucch_expected_res_set_0.uci_bits.harq_ack_nof_bits = harq_ack_nof_bits;
      ASSERT_TRUE(find_pucch_pdu(
          default_slot_grid.result_reader().ul().pucchs().unsorted(),
          [&expected = pucch_expected_res_set_0](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
    } else {
      pucch_expected_res_set_1.uci_bits.harq_ack_nof_bits = harq_ack_nof_bits;
      ASSERT_TRUE(find_pucch_pdu(
          default_slot_grid.result_reader().ul().pucchs().unsorted(),
          [&expected = pucch_expected_res_set_1](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
    }
  }

  // The last HARQ allocation should fail, as the max PUCCH payload has been reached.
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());
}

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_with_existing_sr_succeeds_until_max_payload_reached)
{
  const unsigned max_payload_f2_f3_f4 = t_bench.params.pucch_ded_params.max_payload_234();

  // Add HARQ grants to reach the max_payload.
  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  for (unsigned n = 0; n != (max_payload_f2_f3_f4 - 1); ++n) {
    // These grants should be allocated successfully, as we are below the max payload.
    auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
    ASSERT_TRUE(pri.has_value());
    ASSERT_EQ(0U, pri.value());

    const unsigned harq_ack_nof_bits = n + 1;
    if (harq_ack_nof_bits <= 2U) {
      ASSERT_EQ(2U, default_slot_grid.result_reader().ul().pucchs().size());
      pucch_expected_res_set_0.uci_bits.harq_ack_nof_bits = harq_ack_nof_bits;
      ASSERT_TRUE(find_pucch_pdu(
          default_slot_grid.result_reader().ul().pucchs().unsorted(),
          [&expected = pucch_expected_res_set_0](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
      pucch_expected_sr.uci_bits.harq_ack_nof_bits = harq_ack_nof_bits;
      ASSERT_TRUE(find_pucch_pdu(
          default_slot_grid.result_reader().ul().pucchs().unsorted(),
          [&expected = pucch_expected_sr](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
    } else {
      ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
      pucch_expected_res_set_1.uci_bits.harq_ack_nof_bits = harq_ack_nof_bits;
      pucch_expected_res_set_1.uci_bits.sr_bits           = sr_nof_bits::one;
      ASSERT_TRUE(find_pucch_pdu(
          default_slot_grid.result_reader().ul().pucchs().unsorted(),
          [&expected = pucch_expected_res_set_1](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
    }
  }

  // The last HARQ allocation should fail, as the max PUCCH payload has been reached.
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());
}

///////  Test HARQ-ACK allocation on ded. resources - Resource Set ID 0  - Multi UEs ///////

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_succeeds_until_no_free_res_set_0_resources)
{
  // Fill all resources in Resource Set ID 0 with UEs having their own HARQ grants.
  const unsigned res_set_size = t_bench.params.pucch_ded_params.res_set_size.value();
  for (unsigned i = 0; i != res_set_size; ++i) {
    t_bench.add_ue();
    auto pri = alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
    ASSERT_TRUE(pri.has_value());
    ASSERT_EQ(i, pri.value());
    ASSERT_EQ(i + 1, default_slot_grid.result_reader().ul().pucchs().size());
    ASSERT_EQ(1,
              test_helpers::find_ue_pucchs(default_slot_grid.result_reader().ul().pucchs().unsorted(),
                                           t_bench.last_added_ue_rnti)
                  .size());
  }

  // Try to allocate HARQ for the main UE, which should fail as there are no more free resources in Resource Set ID 0.
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());
  ASSERT_EQ(res_set_size, default_slot_grid.result_reader().ul().pucchs().size());
}

///////  Test HARQ-ACK allocation on ded. resources - Resource Set ID 1   ///////

TEST_P(pucch_alloc_ded_resources_test,
       alloc_ded_harq_ack_res_set_0_with_existing_csi_succeeds_and_grants_are_multiplexed_on_res_set_1_resource)
{
  // With a Resource Set ID 0 HARQ grant and an existing CSI grant, they will overlap and be multiplexed into a PUCCH
  // resource from Resource Set ID 1.
  pucch_expected_res_set_1.uci_bits.harq_ack_nof_bits  = 1U;
  pucch_expected_res_set_1.uci_bits.csi_part1_nof_bits = default_csi_part1_bits;

  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_main_ue()));
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(0U, pri.value());

  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  span<const pucch_info> pucch_pdus = default_slot_grid.result_reader().ul().pucchs().unsorted();
  ASSERT_TRUE(find_pucch_pdu(pucch_pdus, [&expected = pucch_expected_res_set_1](const pucch_info& pdu) {
    return pucch_info_match(expected, pdu) and pdu.csi_rep_cfg.has_value();
  }));

  pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(0U, pri.value());

  pucch_expected_res_set_1.uci_bits.harq_ack_nof_bits = 2U;
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(pucch_pdus, [&expected = pucch_expected_res_set_1](const pucch_info& pdu) {
    return pucch_info_match(expected, pdu) and pdu.csi_rep_cfg.has_value();
  }));
}

TEST_P(pucch_alloc_ded_resources_test,
       alloc_ded_harq_ack_3bits_with_existing_csi_and_sr_succeeds_and_all_grants_are_multiplexed)
{
  // With SR and with PUCCH Format 1 it is guaranteed that the resources will be multiplexed, as PUCCH Format 1 for SR
  // spans over the 14 symbols.
  pucch_expected_res_set_1.uci_bits.harq_ack_nof_bits  = 3;
  pucch_expected_res_set_1.uci_bits.sr_bits            = sr_nof_bits::one;
  pucch_expected_res_set_1.uci_bits.csi_part1_nof_bits = 4;

  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_main_ue()));
  ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_main_ue()));
  ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_main_ue()));
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(0U, pri.value());

  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(default_slot_grid.result_reader().ul().pucchs().unsorted(),
                             [&expected = pucch_expected_res_set_1](const pucch_info& pdu) {
                               return pucch_info_match(expected, pdu) and pdu.csi_rep_cfg.has_value();
                             }));
}

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_with_existing_csi_and_sr_fails_when_max_payload_reached)
{
  const unsigned max_payload_f2_f3_f4 = t_bench.params.pucch_ded_params.max_payload_234();
  ASSERT_TRUE(max_payload_f2_f3_f4 > default_csi_part1_bits + 1U);

  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_main_ue()));
  for (unsigned n = 0; n != max_payload_f2_f3_f4 - (default_csi_part1_bits + 1U); ++n) {
    // These grants should be allocated successfully, as we are below the max payload.
    auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
    ASSERT_TRUE(pri.has_value());
  }

  // This should fail, as the max PUCCH payload has been reached.
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());
}

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_with_existing_csi_selects_free_res_set_1_indicator)
{
  // This makes PUCCH resource indicator 0 busy for PUCCH Resource Set ID 0 (irrelevant here, since the HARQ-ACK
  // grant ends up using a Resource Set ID 1 resource to be able to carry the CSI bits).
  t_bench.add_ue();
  alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));

  alloc_csi_opportunity(t_bench.get_main_ue());
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(0U, pri.value());
}

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_with_existing_csi_and_sr_selects_free_res_set_1_indicator)
{
  // This makes PUCCH resource indicator 0 busy for PUCCH Resource Set ID 0 (irrelevant here, since the HARQ-ACK
  // grant ends up using a Resource Set ID 1 resource to be able to carry the CSI bits).
  t_bench.add_ue();
  alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));

  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_main_ue()));
  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(0U, pri.value());
}

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_res_set_1_adding_extra_bit_promotion_selects_free_pri)
{
  // This makes PUCCH resource indicator 0 busy for PUCCH Resource Set ID 0.
  t_bench.add_ue();
  alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));

  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(1U, pri.value());

  auto pri_new = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri_new.has_value());
  ASSERT_EQ(pri.value(), pri_new.value());

  // With 3 HARQ-ACK bits, the PUCCH resource is promoted to Resource Set ID 1; the PRI is selected independently of
  // the Resource Set ID 0 PRI, so it is the first available PRI in Resource Set ID 1 (PRI 0, still unused there).
  pri_new = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri_new.has_value());
  ASSERT_EQ(0U, pri_new.value());
}

///////   Test allocation of common + dedicated resources.    ///////

TEST_P(pucch_alloc_ded_resources_test, alloc_common_and_ded_harq_ack_succeeds)
{
  auto pri = alloc_common_and_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_EQ(0, pri.value());
  ASSERT_EQ(2U, default_slot_grid.result_reader().ul().pucchs().size());

  // PUCCH dedicated resource.
  ASSERT_TRUE(find_pucch_pdu(
      default_slot_grid.result_reader().ul().pucchs().unsorted(),
      [&expected = pucch_expected_res_set_0](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));

  // PUCCH common resource.
  const auto& common_pdu = default_slot_grid.result_reader().ul().pucchs().unsorted()[1];
  ASSERT_TRUE(common_pdu.res->res_id.is_cmn());
  ASSERT_EQ(pucch_format::FORMAT_1, common_pdu.format());
  ASSERT_EQ(1U, common_pdu.uci_bits.harq_ack_nof_bits);
  ASSERT_EQ(sr_nof_bits::no_sr, common_pdu.uci_bits.sr_bits);
  ASSERT_TRUE(common_pdu.res->second_hop_prb.has_value());
}

TEST_P(pucch_alloc_ded_resources_test, alloc_common_and_ded_harq_ack_with_existing_sr_succeeds)
{
  pucch_expected_sr.uci_bits.harq_ack_nof_bits = 1U;

  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));

  auto pri = alloc_common_and_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());

  span<const pucch_info> pucch_pdus = default_slot_grid.result_reader().ul().pucchs().unsorted();
  ASSERT_EQ(3U, default_slot_grid.result_reader().ul().pucchs().size());
  // All resources are Format 1.
  ASSERT_TRUE(std::all_of(pucch_pdus.begin(), pucch_pdus.end(), [](const pucch_info& pdu) {
    return pdu.format() == pucch_format::FORMAT_1;
  }));
  // We expect 2 PUCCH dedicated resource with HARQ-ACK and SR bits.
  ASSERT_TRUE(find_pucch_pdu(pucch_pdus, [&expected = pucch_expected_res_set_0](const pucch_info& pdu) {
    return pucch_info_match(expected, pdu);
  }));
  ASSERT_TRUE(find_pucch_pdu(
      pucch_pdus, [&expected = pucch_expected_sr](const pucch_info& pdu) { return pucch_info_match(expected, pdu); }));
  // PUCCH common resource.
  ASSERT_TRUE(find_pucch_pdu(pucch_pdus, [](const pucch_info& pdu) {
    return pdu.format() == pucch_format::FORMAT_1 and pdu.uci_bits.sr_bits == sr_nof_bits::no_sr and
           pdu.uci_bits.harq_ack_nof_bits == 1U and pdu.res->res_id.is_cmn();
  }));
}

TEST_P(pucch_alloc_ded_resources_test, alloc_common_and_ded_harq_ack_with_existing_csi_succeeds)
{
  pucch_expected_res_set_1.uci_bits.harq_ack_nof_bits  = 1U;
  pucch_expected_res_set_1.uci_bits.csi_part1_nof_bits = 4U;

  alloc_csi_opportunity(t_bench.get_main_ue());

  auto pri = alloc_common_and_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());

  span<const pucch_info> pucch_pdus = default_slot_grid.result_reader().ul().pucchs().unsorted();
  ASSERT_EQ(2, pucch_pdus.size());
  // PUCCH dedicated resource for HARQ-ACK.
  ASSERT_TRUE(find_pucch_pdu(pucch_pdus, [&expected = pucch_expected_res_set_1](const pucch_info& pdu) {
    return pucch_info_match(expected, pdu);
  }));
  // PUCCH common resource.
  ASSERT_TRUE(find_pucch_pdu(pucch_pdus, [](const pucch_info& pdu) {
    return pdu.format() == pucch_format::FORMAT_1 and pdu.uci_bits.sr_bits == sr_nof_bits::no_sr and
           pdu.uci_bits.harq_ack_nof_bits == 1U and pdu.res->res_id.is_cmn();
  }));
}

TEST_P(pucch_alloc_ded_resources_test, alloc_common_and_ded_harq_ack_fails_when_no_free_res_set_0_resources)
{
  const unsigned res_set_size = t_bench.params.pucch_ded_params.res_set_size.value();
  for (unsigned i = 0; i != res_set_size; ++i) {
    t_bench.add_ue();
    alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
  }
  ASSERT_EQ(res_set_size, default_slot_grid.result_reader().ul().pucchs().size());

  // Try to allocate common + dedicated resources.
  // This should fail as all PUCCH resources from Resource Set 0 are occupied.
  auto pri = alloc_common_and_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());
  ASSERT_EQ(res_set_size, default_slot_grid.result_reader().ul().pucchs().size());
}

TEST_P(pucch_alloc_ded_resources_test,
       alloc_common_and_ded_harq_ack_fails_when_existing_sr_and_no_free_res_set_0_resources)
{
  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  const unsigned res_set_size = t_bench.params.pucch_ded_params.res_set_size.value();
  for (unsigned i = 0; i != res_set_size; ++i) {
    t_bench.add_ue();
    alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
  }
  ASSERT_EQ(res_set_size + 1, default_slot_grid.result_reader().ul().pucchs().size());

  // Try to allocate common + dedicated resources.
  // This should fail as all PUCCH resources from Resource Set 0 are occupied.
  auto pri = alloc_common_and_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());
  ASSERT_EQ(res_set_size + 1, default_slot_grid.result_reader().ul().pucchs().size());
}

TEST_P(pucch_alloc_ded_resources_test,
       alloc_common_and_ded_harq_ack_fails_when_existing_csi_and_no_free_res_set_1_resources)
{
  alloc_csi_opportunity(t_bench.get_main_ue());
  const unsigned res_set_size = t_bench.params.pucch_ded_params.res_set_size.value();
  for (unsigned i = 0; i != res_set_size; ++i) {
    t_bench.add_ue();
    alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
    alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
    alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
  }
  ASSERT_EQ(res_set_size + 1, default_slot_grid.result_reader().ul().pucchs().size());

  // Try to allocate common + dedicated resources.
  // This should fail as all PUCCH resources from Resource Set 1 are occupied.
  auto pri = alloc_common_and_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());
  ASSERT_EQ(res_set_size + 1, default_slot_grid.result_reader().ul().pucchs().size());
}

TEST_P(pucch_alloc_ded_resources_test, if_ded_common_alloc_fails_no_harq_grants_should_be_kept_in_the_scheduler)
{
  // This test recreates an edge-case scenario where the allocation of common + dedicated resources could fail if the
  // scheduler didn't clean the HARQ grant after the resource multiplexing fails.
  // The conditions to recreate this scenario are:
  // - Occupy all PUCCH resources from PUCCH res. set 1.
  // - We need to force the multiplexing for UE under test; for this, we add a CSI grant. For this peculiar case, we
  // need the allocation to fail in the multiplexing process, and not because of lack of available PUCCH resource
  // indicators from PUCCH set 0.
  // - It is important that we only occupy the PUCCH resources PUCCH res. set 1 with a PUCCH resource indicator that
  // also exists in PUCCH res. set 0; if this is not the case, the PUCCH allocator fails because of lack of available
  // PUCCH resource indicators from PUCCH set 0.
  //
  // 1) At this point, we call the allocator to allocate common + dedicated resources for the UE under test. The
  // allocation should fail because all PUCCH resources PUCCH res. set 1 are occupied (failure during multiplexing).
  // 2) We release the PUCCH resource for one of the UE occupying PUCCH res. set 1. This frees one of the PUCCH res.
  // indicator from PUCCH res. set 1.
  // 3) We repeat the allocation of common + dedicated resources for the UE under test; this time we expect a success;
  // if the allocator didn't clean the HARQ grants, the allocation would fail because it finds a dedicated HARQ grant
  // for the UE (which is not allowed the allocation of common + dedicated resources).

  for (unsigned i = 0; i != t_bench.params.pucch_ded_params.res_set_size.value(); ++i) {
    t_bench.add_ue();
    ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)).has_value());
    ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)).has_value());
    ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)).has_value());
  }

  alloc_csi_opportunity(t_bench.get_main_ue());

  auto pri = alloc_common_and_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());

  remove_ue_uci_from_pucch(t_bench.get_ue(to_du_ue_index(2)));

  pri = alloc_common_and_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_TRUE(pri.has_value());
  ASSERT_EQ(1U, pri);
}

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_fails_when_existing_common_harq_ack)
{
  // NOTE: this allocation should be done with the function \ref alloc_common_and_ded_harq_ack, which handles this
  // special case.
  alloc_common_harq_ack(t_bench.get_main_ue());

  auto pri = alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_FALSE(pri.has_value());
}

///////  Test HARQ-ACK allocation on ded. resources - Resource Set ID 1  - Multi UEs ///////

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_res_set_1_over_csi_from_other_ue_succeeds)
{
  // Allocate a CSI grant for UE 0x4601.
  alloc_csi_opportunity(t_bench.get_main_ue());
  ASSERT_EQ(1, default_slot_grid.result_reader().ul().pucchs().size());
  const auto ue_pucchs = test_helpers::find_ue_pucchs(default_slot_grid.result_reader().ul().pucchs().unsorted(),
                                                      t_bench.get_main_ue().crnti);
  ASSERT_EQ(1, ue_pucchs.size());
  ASSERT_EQ(GetParam(), ue_pucchs[0]->format());
  ASSERT_EQ(default_csi_part1_bits, ue_pucchs[0]->uci_bits.csi_part1_nof_bits);

  const unsigned res_set_size = t_bench.params.pucch_ded_params.res_set_size.value();
  for (unsigned i = 0; i < res_set_size; ++i) {
    t_bench.add_ue();
    alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
    alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
    alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx));
  }
  ASSERT_EQ(res_set_size + 1, default_slot_grid.result_reader().ul().pucchs().size());
}

TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_res_set_1_promotion_fails_if_no_free_res_set_1_resources)
{
  const unsigned res_set_size = t_bench.params.pucch_ded_params.res_set_size.value();
  for (unsigned i = 0; i != res_set_size; ++i) {
    // Allocate a HARQ-ACK grant from Resource Set ID 1 for each UE.
    t_bench.add_ue();
    ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)).has_value());
    ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)).has_value());
    ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)).has_value());
    ASSERT_EQ(i + 1, default_slot_grid.result_reader().ul().pucchs().size());
    const auto ue_pucchs = test_helpers::find_ue_pucchs(default_slot_grid.result_reader().ul().pucchs().unsorted(),
                                                        t_bench.last_added_ue_rnti);
    ASSERT_EQ(1, ue_pucchs.size());
    ASSERT_EQ(GetParam(), ue_pucchs[0]->format());
    ASSERT_EQ(pucch_uci_bits{.harq_ack_nof_bits = 3U}, ue_pucchs[0]->uci_bits);
  }
  ASSERT_EQ(res_set_size, default_slot_grid.result_reader().ul().pucchs().size());

  // A new UE's initial HARQ-ACK allocation only checks Resource Set ID 0, which is entirely free, so it succeeds
  // (using a Resource Set ID 0 grant, hence no extra PUCCH PDU is added on top of the res_set_size Set ID 1 grants).
  t_bench.add_ue();
  ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)).has_value());
  ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)).has_value());
  ASSERT_EQ(res_set_size + 1, default_slot_grid.result_reader().ul().pucchs().size());

  // Promoting to Resource Set ID 1 on the 3rd HARQ-ACK bit fails, as there are no more free resources there.
  ASSERT_FALSE(alloc_ded_harq_ack(t_bench.get_ue(t_bench.last_added_ue_idx)));
  ASSERT_EQ(res_set_size + 1, default_slot_grid.result_reader().ul().pucchs().size());
}

///////   Test removal of dedicated PUCCH resources    ///////

TEST_P(pucch_alloc_ded_resources_test, remove_ue_uci_from_pucch_with_existing_sr)
{
  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());

  pucch_uci_bits removed_bits = remove_ue_uci_from_pucch(t_bench.get_main_ue());

  ASSERT_EQ(0U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_EQ(0U, removed_bits.harq_ack_nof_bits);
  ASSERT_EQ(sr_nof_bits::one, removed_bits.sr_bits);
  ASSERT_EQ(0U, removed_bits.csi_part1_nof_bits);
}

TEST_P(pucch_alloc_ded_resources_test, remove_ue_uci_from_pucch_with_existing_csi)
{
  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_main_ue()));
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());

  pucch_uci_bits removed_bits = remove_ue_uci_from_pucch(t_bench.get_main_ue());

  ASSERT_EQ(0U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_EQ(0U, removed_bits.harq_ack_nof_bits);
  ASSERT_EQ(sr_nof_bits::no_sr, removed_bits.sr_bits);
  ASSERT_EQ(4U, removed_bits.csi_part1_nof_bits);
}

TEST_P(pucch_alloc_ded_resources_test, remove_ue_uci_from_pucch_with_existing_harq_res_set_0)
{
  ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_main_ue()));
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());

  pucch_uci_bits removed_bits = remove_ue_uci_from_pucch(t_bench.get_main_ue());

  ASSERT_EQ(0U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_EQ(1U, removed_bits.harq_ack_nof_bits);
  ASSERT_EQ(sr_nof_bits::no_sr, removed_bits.sr_bits);
  ASSERT_EQ(0U, removed_bits.csi_part1_nof_bits);
}

TEST_P(pucch_alloc_ded_resources_test, remove_ue_uci_from_pucch_with_existing_sr_and_harq_res_set_0)
{
  ASSERT_TRUE(alloc_sr_opportunity(t_bench.get_main_ue()));
  ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_main_ue()));
  ASSERT_EQ(2U, default_slot_grid.result_reader().ul().pucchs().size());

  pucch_uci_bits removed_bits = remove_ue_uci_from_pucch(t_bench.get_main_ue());

  ASSERT_EQ(0U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_EQ(1U, removed_bits.harq_ack_nof_bits);
  ASSERT_EQ(sr_nof_bits::one, removed_bits.sr_bits);
  ASSERT_EQ(0U, removed_bits.csi_part1_nof_bits);
}

TEST_P(pucch_alloc_ded_resources_test, remove_ue_uci_from_pucch_with_existing_csi_and_harq_res_set_0)
{
  ASSERT_TRUE(alloc_csi_opportunity(t_bench.get_main_ue()));
  ASSERT_TRUE(alloc_ded_harq_ack(t_bench.get_main_ue()));
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());

  pucch_uci_bits removed_bits = remove_ue_uci_from_pucch(t_bench.get_main_ue());

  ASSERT_EQ(0U, default_slot_grid.result_reader().ul().pucchs().size());
  ASSERT_EQ(1U, removed_bits.harq_ack_nof_bits);
  ASSERT_EQ(sr_nof_bits::no_sr, removed_bits.sr_bits);
  ASSERT_EQ(4U, removed_bits.csi_part1_nof_bits);
}

///////   Test allocation over different slots.    ///////

// Allocate multiple HARQ-ACK grants over the same target slot.
TEST_P(pucch_alloc_ded_resources_test, alloc_ded_harq_ack_over_multiple_slots_in_tdd_succeeds)
{
  // All the allocation allocate a HARQ-ACK grant at slot 5.
  // t_bench.sl_tx = 0; k0 = 0; k1 = 5  =>  t_bench.sl_tx + k0 + k1 = 5.
  unsigned k1 = 5;

  for (unsigned i = 0; i < 5; ++i) {
    auto pri = t_bench.pucch_alloc.alloc_ded_harq_ack(t_bench.res_grid,
                                                      t_bench.get_main_ue().get_pcell().cfg(),
                                                      t_bench.k0,
                                                      k1,
                                                      t_bench.get_main_ue().get_pcell().ntn_koffset());
    ASSERT_TRUE(pri.has_value());
    ASSERT_EQ(0U, pri.value());

    const auto& slot_grid = t_bench.res_grid[t_bench.k0 + k1];
    ASSERT_EQ(1U, slot_grid.result_reader().ul().pucchs().size());
    const auto ue_pucchs =
        test_helpers::find_ue_pucchs(slot_grid.result_reader().ul().pucchs().unsorted(), t_bench.get_main_ue().crnti);
    ASSERT_EQ(1U, ue_pucchs.size());
    if (i < 2) {
      ASSERT_EQ(pucch_format::FORMAT_1, ue_pucchs[0]->format());
    } else {
      ASSERT_EQ(GetParam(), ue_pucchs[0]->format());
    }
    ASSERT_EQ(i + 1, ue_pucchs[0]->uci_bits.harq_ack_nof_bits);

    // Advance by 1 slot and decrease k1 accordingly to keep targeting the same slot for the next allocation.
    t_bench.slot_indication(++t_bench.sl_tx);
    --k1;
  }
}

TEST_P(pucch_alloc_ded_resources_test, test_for_private_fnc_retrieving_existing_grants)
{
  // All the allocation allocate a HARQ-ACK grant at slot 7.
  // t_bench.sl_tx = 0; k0 = 0; k1 = 7  =>  t_bench.sl_tx + k0 + k1 = 7.
  unsigned         k1         = 7;
  auto&            slot_grid  = t_bench.res_grid[t_bench.k0 + k1];
  const slot_point pucch_slot = slot_grid.slot;

  // Allocate 1 HARQ at k1 = 7.
  t_bench.add_ue();
  du_ue_index_t ue1_idx = t_bench.last_added_ue_idx;
  t_bench.add_ue();
  du_ue_index_t ue2_idx = t_bench.last_added_ue_idx;

  // NOTE: In the following, allocate first the PUCCH dedicated resource and then the common resource. This is to test
  // that the function retrieving the existing PUCCH resources does not mess up when with common resources when PUCCH
  // grants are removed from the scheduler output.

  // Allocate:
  // - 1 PUCCH ded with 1 HARQ-ACK bit to UE 1.
  // - 1 PUCCH common with 1 HARQ-ACK bit to UE 0.
  // - 1 PUCCH ded with 1 HARQ-ACK bit to UE 2.
  // The scheduler output should have 3 PUCCH resources.
  // - 1 PUCCH Resource Set ID 0 - RNTI = UE1 - HARQ-BITS = 1.
  // - 1 PUCCH common HARQ-ACK   - RNTI = UE0 - HARQ-BITS = 1.
  // - 1 PUCCH Resource Set ID 0 - RNTI = UE2 - HARQ-BITS = 1.
  auto pri_ue1 = t_bench.pucch_alloc.alloc_ded_harq_ack(t_bench.res_grid,
                                                        t_bench.get_ue(ue1_idx).get_pcell().cfg(),
                                                        t_bench.k0,
                                                        k1,
                                                        t_bench.get_ue(ue1_idx).get_pcell().ntn_koffset());

  ASSERT_TRUE(pri_ue1.has_value());
  ASSERT_EQ(0U, pri_ue1.value());
  ASSERT_EQ(1U, slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue1_idx).crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U;
                             }));

  auto pri_ue0 = t_bench.pucch_alloc.alloc_common_harq_ack(t_bench.res_grid,
                                                           t_bench.get_main_ue().crnti,
                                                           t_bench.k0,
                                                           k1,
                                                           t_bench.dci_info,
                                                           t_bench.get_main_ue().get_pcell().ntn_koffset());
  ASSERT_TRUE(pri_ue0.has_value());
  // The dedicated resources do not overlap the common ones, so the first common resource is available.
  ASSERT_EQ(0U, pri_ue0.value());
  ASSERT_EQ(2U, slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_main_ue().crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U and pdu.res->res_id.is_cmn();
                             }));

  auto pri_ue2 = t_bench.pucch_alloc.alloc_ded_harq_ack(t_bench.res_grid,
                                                        t_bench.get_ue(ue2_idx).get_pcell().cfg(),
                                                        t_bench.k0,
                                                        k1,
                                                        t_bench.get_ue(ue2_idx).get_pcell().ntn_koffset());
  ASSERT_TRUE(pri_ue2.has_value());
  ASSERT_EQ(1U, pri_ue2.value());
  ASSERT_EQ(3U, slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue2_idx).crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U;
                             }));
  // Test now that the previous allocations have not been messed up.
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue1_idx).crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U;
                             }));
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_main_ue().crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U and pdu.res->res_id.is_cmn();
                             }));

  // Advance by 1 slot. Allocate:
  // - 1 PUCCH ded with 1 HARQ-ACK bit to UE 1 (Resource Set ID 0).
  // The scheduler output should have 3 PUCCH resources.
  // - 1 PUCCH Resource Set ID 0 - RNTI = UE1 - HARQ-BITS = 2.
  // - 1 PUCCH common HARQ-ACK   - RNTI = UE0 - HARQ-BITS = 1.
  // - 1 PUCCH Resource Set ID 0 - RNTI = UE2 - HARQ-BITS = 1.
  t_bench.slot_indication(++t_bench.sl_tx);
  // t_bench.sl_tx = 1; k0 = 0; k1 = 4  =>  t_bench.sl_tx + k0 + k1 = 5.
  --k1;
  ASSERT_EQ(pucch_slot, slot_grid.slot);

  pri_ue1 = t_bench.pucch_alloc.alloc_ded_harq_ack(t_bench.res_grid,
                                                   t_bench.get_ue(ue1_idx).get_pcell().cfg(),
                                                   t_bench.k0,
                                                   k1,
                                                   t_bench.get_ue(ue1_idx).get_pcell().ntn_koffset());
  ASSERT_TRUE(pri_ue1.has_value());
  ASSERT_EQ(0U, pri_ue1.value());
  ASSERT_EQ(3U, slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue1_idx).crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 2U;
                             }));
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_main_ue().crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U and pdu.res->res_id.is_cmn();
                             }));
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue2_idx).crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U;
                             }));

  // Advance by 1 slot. Allocate:
  // - 1 PUCCH ded with 1 HARQ-ACK bit to UE 1 (Convert to Resource Set ID 1).
  // NOTE: This change the other in which the PUCCH grants are stored in the scheduler output.
  // The scheduler output should have 3 PUCCH resources.
  // - 1 PUCCH common HARQ-ACK   - RNTI = UE0 - HARQ-BITS = 1.
  // - 1 PUCCH Resource Set ID 0 - RNTI = UE2 - HARQ-BITS = 1.
  // - 1 PUCCH Resource Set ID 1 - RNTI = UE1 - HARQ-BITS = 3.
  t_bench.slot_indication(++t_bench.sl_tx);
  --k1;
  ASSERT_EQ(pucch_slot, slot_grid.slot);
  // t_bench.sl_tx = 1; k0 = 0; k1 = 4  =>  t_bench.sl_tx + k0 + k1 = 5.
  //  auto& slot_grid_3 = t_bench.res_grid[t_bench.k0 + --k1];

  pri_ue1 = t_bench.pucch_alloc.alloc_ded_harq_ack(t_bench.res_grid,
                                                   t_bench.get_ue(ue1_idx).get_pcell().cfg(),
                                                   t_bench.k0,
                                                   k1,
                                                   t_bench.get_ue(ue1_idx).get_pcell().ntn_koffset());
  ASSERT_TRUE(pri_ue1.has_value());
  ASSERT_EQ(0U, pri_ue1.value());
  ASSERT_EQ(3U, slot_grid.result_reader().ul().pucchs().size());
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue1_idx).crnti, format = GetParam()](const pucch_info& pdu) {
                               return pdu.format() == format and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 3U;
                             }));
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_main_ue().crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U and pdu.res->res_id.is_cmn();
                             }));
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue2_idx).crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 1U;
                             }));

  // Advance by 1 slot. Allocate:
  // - 1 PUCCH ded with 1 HARQ-ACK bit to UE 2 (Multiplex on existing Resource Set ID 0).
  // NOTE: This change the other in which the PUCCH grants are stored in the scheduler output.
  // The scheduler output should have 3 PUCCH resources.
  // - 1 PUCCH common HARQ-ACK   - RNTI = UE0 - HARQ-BITS = 1.
  // - 1 PUCCH Resource Set ID 0 - RNTI = UE2 - HARQ-BITS = 2.
  // - 1 PUCCH Resource Set ID 1 - RNTI = UE1 - HARQ-BITS = 3.
  t_bench.slot_indication(++t_bench.sl_tx);
  --k1;
  ASSERT_EQ(pucch_slot, slot_grid.slot);
  // t_bench.sl_tx = 2; k0 = 0; k1 = 3  =>  t_bench.sl_tx + k0 + k1 = 5.
  //  auto& slot_grid_4 = t_bench.res_grid[t_bench.k0 + --k1];

  pri_ue2 = t_bench.pucch_alloc.alloc_ded_harq_ack(t_bench.res_grid,
                                                   t_bench.get_ue(ue2_idx).get_pcell().cfg(),
                                                   t_bench.k0,
                                                   k1,
                                                   t_bench.get_ue(ue2_idx).get_pcell().ntn_koffset());
  ASSERT_TRUE(pri_ue2.has_value());
  ASSERT_EQ(1U, pri_ue2.value());
  ASSERT_EQ(3U, slot_grid.result_reader().ul().pucchs().size());

  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue1_idx).crnti, format = GetParam()](const pucch_info& pdu) {
                               return pdu.format() == format and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 3U;
                             }));
  ASSERT_TRUE(find_pucch_pdu(
      slot_grid.result_reader().ul().pucchs().unsorted(), [rnti = t_bench.get_main_ue().crnti](const pucch_info& pdu) {
        return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and pdu.uci_bits.harq_ack_nof_bits == 1U;
      }));
  ASSERT_TRUE(find_pucch_pdu(slot_grid.result_reader().ul().pucchs().unsorted(),
                             [rnti = t_bench.get_ue(ue2_idx).crnti](const pucch_info& pdu) {
                               return pdu.format() == pucch_format::FORMAT_1 and pdu.crnti == rnti and
                                      pdu.uci_bits.harq_ack_nof_bits == 2U;
                             }));
}

INSTANTIATE_TEST_SUITE_P(,
                         pucch_alloc_ded_resources_test,
                         ::testing::Values(pucch_format::FORMAT_2, pucch_format::FORMAT_3, pucch_format::FORMAT_4));

///////   Test allocation of dedicated PUCCH resources with different code rates and max payloads   ///////

class pucch_alloc_small_code_rate_test : public ::testing::Test, public pucch_allocator_base_test
{
public:
  pucch_alloc_small_code_rate_test() :
    pucch_allocator_base_test(test_bench_params{
        .pucch_ded_params{.f2_or_f3_or_f4_params = pucch_f2_params{.max_code_rate = max_pucch_code_rate::dot_15}}})
  {
  }

protected:
  static constexpr unsigned default_csi_part1_bits = 4;
};

TEST_F(pucch_alloc_small_code_rate_test, with_4_bits_payload_csi_plus_harq_mplex_not_allowed)
{
  alloc_csi_opportunity(t_bench.get_main_ue());
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  auto ue_pucchs = test_helpers::find_ue_pucchs(default_slot_grid.result_reader().ul().pucchs().unsorted(),
                                                t_bench.get_main_ue().crnti);
  ASSERT_EQ(1U, ue_pucchs.size());
  ASSERT_EQ(default_csi_part1_bits, ue_pucchs[0]->uci_bits.csi_part1_nof_bits);

  // HARQ-ACK grant is expected NOT to be multiplexed with CSI grants.
  alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ue_pucchs = test_helpers::find_ue_pucchs(default_slot_grid.result_reader().ul().pucchs().unsorted(),
                                           t_bench.get_main_ue().crnti);
  ASSERT_EQ(1U, ue_pucchs.size());
  ASSERT_EQ(default_csi_part1_bits, ue_pucchs[0]->uci_bits.csi_part1_nof_bits);
  ASSERT_EQ(0U, ue_pucchs[0]->uci_bits.harq_ack_nof_bits);
}

class pucch_alloc_16bit_payload_test : public ::testing::Test, public pucch_allocator_base_test
{
public:
  pucch_alloc_16bit_payload_test() :
    pucch_allocator_base_test(test_bench_params{
        .pucch_ded_params{.f2_or_f3_or_f4_params =
                              pucch_f2_params{.max_nof_rbs = 2U, .max_code_rate = max_pucch_code_rate::dot_35}},
        .nof_ul_dl_ports = 4})
  {
    ocudu_assert(t_bench.get_main_ue().get_pcell().cfg().has_csi_cfg(), "CSI configuration needed for this test");
    csi_report_size =
        get_csi_report_pucch_size(t_bench.get_main_ue().get_pcell().cfg().csi_report()).part1_size.value();
  }

protected:
  unsigned csi_report_size;
};

TEST_F(pucch_alloc_16bit_payload_test, with_16_bits_payload_csi_plus_harq_mplex_allowed_in_mimo_4x4)
{
  alloc_csi_opportunity(t_bench.get_main_ue());
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  auto ue_pucchs = test_helpers::find_ue_pucchs(default_slot_grid.result_reader().ul().pucchs().unsorted(),
                                                t_bench.get_main_ue().crnti);
  ASSERT_EQ(1U, ue_pucchs.size());
  ASSERT_EQ(csi_report_size, ue_pucchs[0]->uci_bits.csi_part1_nof_bits);

  // HARQ-ACK grants are expected to be multiplexed with CSI grants.
  alloc_ded_harq_ack(t_bench.get_main_ue());
  alloc_ded_harq_ack(t_bench.get_main_ue());
  ASSERT_EQ(1U, default_slot_grid.result_reader().ul().pucchs().size());
  ue_pucchs = test_helpers::find_ue_pucchs(default_slot_grid.result_reader().ul().pucchs().unsorted(),
                                           t_bench.get_main_ue().crnti);
  ASSERT_EQ(1U, ue_pucchs.size());
  ASSERT_EQ(csi_report_size, ue_pucchs[0]->uci_bits.csi_part1_nof_bits);
  ASSERT_EQ(2U, ue_pucchs[0]->uci_bits.harq_ack_nof_bits);
}
