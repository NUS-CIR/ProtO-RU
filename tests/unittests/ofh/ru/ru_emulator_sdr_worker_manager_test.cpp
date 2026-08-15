// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "apps/examples/ofh/ru_emulator_appconfig.h"
#include "apps/examples/ofh/ru_emulator_sdr_worker_manager.h"
#include "ocudu/ocudulog/ocudulog.h"
#include <algorithm>
#include <future>
#include <gtest/gtest.h>
#include <sched.h>

using namespace ocudu;
using namespace std::chrono_literals;

namespace {

ru_emulator_ofh_appconfig make_sdr_cell(const std::string& profile)
{
  ru_emulator_ofh_appconfig cell;
  cell.sdr_config.emplace();
  cell.sdr_config->device_driver     = "uhd";
  cell.sdr_config->execution_profile = profile;
  return cell;
}

std::vector<unsigned> available_cpus()
{
  const auto            cpuset = cpu_architecture_info::get().get_available_cpuset();
  std::vector<unsigned> cpus;
  for (unsigned cpu = 0; cpu != CPU_SETSIZE; ++cpu) {
    if (CPU_ISSET(cpu, &cpuset)) {
      cpus.push_back(cpu);
    }
  }
  return cpus;
}

std::string executor_thread_name(task_executor& executor)
{
  auto completion = std::make_shared<std::promise<std::string>>();
  auto result     = completion->get_future();
  if (!executor.defer([completion]() { completion->set_value(this_thread_name()); })) {
    ADD_FAILURE() << "Failed to enqueue the worker probe";
    return {};
  }
  if (result.wait_for(3s) != std::future_status::ready) {
    ADD_FAILURE() << "Worker probe timed out";
    return {};
  }
  return result.get();
}

class ru_emulator_sdr_worker_manager_test : public ::testing::Test
{
protected:
  static void SetUpTestSuite() { ocudulog::init(); }
};

} // namespace

TEST_F(ru_emulator_sdr_worker_manager_test, loopback_and_sequential_cells_do_not_create_a_compute_pool)
{
  ru_emulator_appconfig config;
  EXPECT_FALSE(generate_ru_emulator_phy_worker_pool_config(config));

  config.ru_cfg = {make_sdr_cell("sequential")};
  EXPECT_FALSE(generate_ru_emulator_phy_worker_pool_config(config));

  config.ru_cfg.front().sdr_config->device_driver     = "zmq";
  config.ru_cfg.front().sdr_config->execution_profile = "triple";
  EXPECT_EQ(resolve_ru_emulator_sdr_thread_profile(*config.ru_cfg.front().sdr_config),
            ru_emulator_sdr_thread_profile::sequential);
  EXPECT_FALSE(generate_ru_emulator_phy_worker_pool_config(config));
}

TEST_F(ru_emulator_sdr_worker_manager_test, compute_pool_uses_only_its_own_cpu_mask_and_caps_worker_count)
{
  auto cpus = available_cpus();
  ASSERT_FALSE(cpus.empty());
  cpus.resize(std::min<size_t>(cpus.size(), 2));

  ru_emulator_appconfig config;
  config.ru_cfg                             = {make_sdr_cell("single")};
  config.phy_cpus                           = cpus;
  config.nof_phy_threads                    = 1000;
  config.ru_cfg.front().ofh_cpus            = {cpus.front()};
  config.ru_cfg.front().sdr_config->ru_cpus = {cpus.back()};

  auto pool = generate_ru_emulator_phy_worker_pool_config(config);
  ASSERT_TRUE(pool);
  EXPECT_EQ(pool->nof_workers, cpus.size());
  ASSERT_EQ(pool->masks.size(), 1);
  EXPECT_EQ(pool->masks.front().count(), cpus.size());
  for (unsigned cpu : cpus) {
    EXPECT_TRUE(pool->masks.front().test(cpu));
  }

  config.nof_phy_threads = 1;
  EXPECT_EQ(generate_ru_emulator_phy_worker_pool_config(config)->nof_workers, 1);
}

TEST_F(ru_emulator_sdr_worker_manager_test, profiles_share_compute_but_keep_baseband_transport_separate)
{
  ru_emulator_appconfig config;
  config.ru_cfg          = {{}, make_sdr_cell("single"), make_sdr_cell("dual"), make_sdr_cell("triple")};
  config.nof_phy_threads = 2;
  ru_emulator_sdr_worker_manager workers(config);

  EXPECT_EQ(workers.get_executor_mapper(0), nullptr);
  auto&          single  = workers.get_executor_mapper(1)->get_sector_mapper(0);
  auto&          dual    = workers.get_executor_mapper(2)->get_sector_mapper(0);
  auto&          triple  = workers.get_executor_mapper(3)->get_sector_mapper(0);
  task_executor* compute = &single.downlink_executor();
  for (auto* mapper : {&single, &dual, &triple}) {
    EXPECT_EQ(&mapper->downlink_executor(), compute);
    EXPECT_EQ(&mapper->uplink_demodulation_executor(), compute);
    EXPECT_EQ(&mapper->prach_executor(), compute);
    EXPECT_NE(&mapper->receiver_executor(), compute);
    EXPECT_NE(&mapper->transmitter_executor(), compute);
  }
  EXPECT_EQ(&single.receiver_executor(), &single.transmitter_executor());
  EXPECT_NE(&dual.receiver_executor(), &dual.transmitter_executor());
  EXPECT_NE(&triple.uplink_executor(), &triple.receiver_executor());
  EXPECT_NE(&triple.uplink_executor(), compute);

  EXPECT_EQ(executor_thread_name(*compute).find("ru_phy_pool#"), 0);
  EXPECT_EQ(executor_thread_name(single.receiver_executor()), "ru_phy_#1");
  EXPECT_EQ(executor_thread_name(dual.receiver_executor()), "ru_phy_rx_#2");
  EXPECT_EQ(executor_thread_name(triple.uplink_executor()), "ru_phy_ul_#3");
  workers.stop();
  workers.stop();
}

TEST_F(ru_emulator_sdr_worker_manager_test, default_compute_affinity_does_not_inherit_a_narrowed_caller_mask)
{
#ifdef HAVE_TSAN
  GTEST_SKIP() << "Thread affinity is disabled under ThreadSanitizer";
#endif
  const auto cpus = available_cpus();
  ASSERT_FALSE(cpus.empty());
  os_sched_affinity_bitmask caller_mask;
  caller_mask.set(cpus.front());

  unique_thread caller("affinity_probe", caller_mask, []() {
    cpu_set_t affinity;
    ASSERT_EQ(::sched_getaffinity(0, sizeof(affinity), &affinity), 0);
    EXPECT_EQ(CPU_COUNT(&affinity), 1);

    ru_emulator_appconfig config;
    config.ru_cfg          = {make_sdr_cell("single")};
    config.nof_phy_threads = 1;
    ru_emulator_sdr_worker_manager workers(config);
    auto                           completed = std::make_shared<std::promise<cpu_set_t>>();
    auto                           result    = completed->get_future();
    ASSERT_TRUE(workers.get_executor_mapper(0)->get_sector_mapper(0).downlink_executor().defer([completed]() {
      cpu_set_t worker_affinity;
      EXPECT_EQ(::sched_getaffinity(0, sizeof(worker_affinity), &worker_affinity), 0);
      completed->set_value(worker_affinity);
    }));
    ASSERT_EQ(result.wait_for(3s), std::future_status::ready);
    const auto worker_affinity  = result.get();
    const auto startup_affinity = cpu_architecture_info::get().get_available_cpuset();
    EXPECT_TRUE(CPU_EQUAL(&worker_affinity, &startup_affinity));
  });
  caller.join();
}

TEST_F(ru_emulator_sdr_worker_manager_test, compute_tasks_from_different_cells_run_concurrently)
{
  auto cpus = available_cpus();
  if (cpus.size() < 2) {
    GTEST_SKIP() << "Concurrent compute test needs two available CPUs";
  }
  cpus.resize(2);
  ru_emulator_appconfig config;
  config.ru_cfg          = {make_sdr_cell("single"), make_sdr_cell("triple")};
  config.phy_cpus        = cpus;
  config.nof_phy_threads = 2;
  ru_emulator_sdr_worker_manager workers(config);

  auto               first_started  = std::make_shared<std::promise<unsigned>>();
  auto               second_started = std::make_shared<std::promise<unsigned>>();
  auto               first_result   = first_started->get_future();
  auto               second_result  = second_started->get_future();
  std::promise<void> release;
  auto               can_finish = release.get_future().share();

  const bool first_queued =
      workers.get_executor_mapper(0)->get_sector_mapper(0).downlink_executor().defer([first_started, can_finish]() {
        first_started->set_value(::sched_getcpu());
        can_finish.wait();
      });
  const bool second_queued = workers.get_executor_mapper(1)->get_sector_mapper(0).uplink_demodulation_executor().defer(
      [second_started, can_finish]() {
        second_started->set_value(::sched_getcpu());
        can_finish.wait();
      });

  const auto first_status  = first_result.wait_for(3s);
  const auto second_status = second_result.wait_for(3s);
  release.set_value();
  ASSERT_TRUE(first_queued);
  ASSERT_TRUE(second_queued);
  ASSERT_EQ(first_status, std::future_status::ready);
  ASSERT_EQ(second_status, std::future_status::ready);
  for (unsigned cpu : {first_result.get(), second_result.get()}) {
#ifndef HAVE_TSAN
    EXPECT_NE(std::find(cpus.begin(), cpus.end(), cpu), cpus.end());
#else
    (void)cpu;
#endif
  }
}

TEST_F(ru_emulator_sdr_worker_manager_test, sequential_compute_stays_inline_on_the_baseband_worker)
{
  ru_emulator_appconfig config;
  config.ru_cfg = {make_sdr_cell("sequential")};
  ru_emulator_sdr_worker_manager workers(config);
  auto&                          mapper = workers.get_executor_mapper(0)->get_sector_mapper(0);

  EXPECT_EQ(&mapper.downlink_executor(), &mapper.uplink_demodulation_executor());
  EXPECT_EQ(&mapper.downlink_executor(), &mapper.prach_executor());
  EXPECT_EQ(&mapper.receiver_executor(), &mapper.transmitter_executor());
  auto completed = std::make_shared<std::promise<std::string>>();
  auto result    = completed->get_future();
  ASSERT_TRUE(mapper.receiver_executor().defer([&mapper, completed]() {
    EXPECT_TRUE(
        mapper.uplink_demodulation_executor().execute([completed]() { completed->set_value(this_thread_name()); }));
  }));
  ASSERT_EQ(result.wait_for(3s), std::future_status::ready);
  EXPECT_EQ(result.get(), "ru_phy_#0");
}
