// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/nrppa/du_context/nrppa_du_context.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "ocudu/adt/format.h"
#include "ocudu/support/async/async_no_op_task.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocucp;

class dummy_nrppa_f1ap_notifier : public nrppa_f1ap_notifier
{
public:
  async_task<expected<positioning_information_response_t, positioning_information_failure_t>>
  on_positioning_information_request(const positioning_information_request_t& request) override
  {
    return launch_no_op_task(expected<positioning_information_response_t, positioning_information_failure_t>{
        positioning_information_response_t{}});
  }

  async_task<expected<positioning_activation_response_t, positioning_activation_failure_t>>
  on_positioning_activation_request(const positioning_activation_request_t& request) override
  {
    return launch_no_op_task(expected<positioning_activation_response_t, positioning_activation_failure_t>{
        positioning_activation_response_t{}});
  }

  async_task<expected<measurement_response_t, measurement_failure_t>>
  on_measurement_information_request(const measurement_request_t& request) override
  {
    return launch_no_op_task(expected<measurement_response_t, measurement_failure_t>{measurement_response_t{}});
  }

  async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>
  on_e_cid_measurement_request(const e_cid_measurement_request_t& request) override
  {
    return launch_no_op_task(
        expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>{e_cid_measurement_response_t{}});
  }

  void on_e_cid_measurement_termination_command(const e_cid_measurement_termination_command_t& command) override
  {
    e_cid_measurement_terminated = true;
    last_termination_command     = command;
  }

  bool                                                   e_cid_measurement_terminated = false;
  std::optional<e_cid_measurement_termination_command_t> last_termination_command;
};

// Test class.
class nrppa_du_context_list_test : public nrppa_du_context_list
{
public:
  nrppa_du_context_list_test(ocudulog::basic_logger& logger_) : nrppa_du_context_list(logger_) {}
};

/// Fixture class for NRPPA DU context.
class nrppa_du_context_test : public ::testing::Test
{
protected:
  nrppa_du_context_test()
  {
    nrppa_logger.set_level(ocudulog::basic_levels::debug);
    ocudulog::init();
  }

  ~nrppa_du_context_test()
  {
    // Flush logger after each test.
    ocudulog::flush();
  }

  static cu_cp_du_index_t generate_random_du_index()
  {
    return uint_to_cu_cp_du_index(test_rng::uniform_int<uint64_t>(cu_cp_du_index_to_uint(cu_cp_du_index_t::min),
                                                                  cu_cp_du_index_to_uint(cu_cp_du_index_t::max) - 1));
  }

  ocudulog::basic_logger&    nrppa_logger = ocudulog::fetch_basic_logger("nrppa");
  dummy_nrppa_f1ap_notifier  f1ap_notifier;
  nrppa_du_context_list_test du_ctxt_list{nrppa_logger};
};

TEST_F(nrppa_du_context_test, when_du_added_then_du_exists)
{
  cu_cp_du_index_t du_index = generate_random_du_index();

  du_ctxt_list.add_du(du_index, f1ap_notifier);

  ASSERT_TRUE(du_ctxt_list.contains(du_index));
  ASSERT_EQ(du_ctxt_list[du_index].du_index, du_index);
  ASSERT_EQ(du_ctxt_list.size(), 1);
}

TEST_F(nrppa_du_context_test, when_du_not_added_then_du_doesnt_exist)
{
  cu_cp_du_index_t du_index = generate_random_du_index();

  ASSERT_FALSE(du_ctxt_list.contains(du_index));
}

TEST_F(nrppa_du_context_test, when_du_exists_then_removal_succeeds)
{
  cu_cp_du_index_t du_index = generate_random_du_index();

  du_ctxt_list.add_du(du_index, f1ap_notifier);

  du_ctxt_list.remove_du_context(du_index);

  ASSERT_FALSE(du_ctxt_list.contains(du_index));
  ASSERT_EQ(du_ctxt_list.size(), 0);
}

TEST_F(nrppa_du_context_test, when_unknown_du_is_removed_then_it_is_ignored)
{
  cu_cp_du_index_t du_index = generate_random_du_index();

  du_ctxt_list.remove_du_context(du_index);

  ASSERT_EQ(du_ctxt_list.size(), 0);
}

TEST_F(nrppa_du_context_test, when_du_index_is_updated_then_context_is_moved_to_new_index)
{
  cu_cp_du_index_t old_du_index = generate_random_du_index();
  cu_cp_du_index_t new_du_index = generate_random_du_index();
  while (new_du_index == old_du_index) {
    new_du_index = generate_random_du_index();
  }

  du_ctxt_list.add_du(old_du_index, f1ap_notifier);

  dummy_nrppa_f1ap_notifier new_f1ap_notifier;
  du_ctxt_list.update_du_index(new_du_index, old_du_index, new_f1ap_notifier);

  ASSERT_FALSE(du_ctxt_list.contains(old_du_index));
  ASSERT_TRUE(du_ctxt_list.contains(new_du_index));
  ASSERT_EQ(du_ctxt_list[new_du_index].f1ap, &new_f1ap_notifier);
}
