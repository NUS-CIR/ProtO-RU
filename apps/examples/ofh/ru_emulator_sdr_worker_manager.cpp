// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ru_emulator_sdr_worker_manager.h"
#include "ru_emulator_appconfig.h"
#include "ocudu/support/cpu_architecture_info.h"
#include "ocudu/support/error_handling.h"
#include <algorithm>

using namespace ocudu;

namespace {

constexpr unsigned task_worker_queue_size = 1024;
constexpr unsigned phy_pool_queue_size    = 2048;

os_sched_affinity_bitmask make_cpu_mask(const std::vector<unsigned>& cpus)
{
  os_sched_affinity_bitmask mask;
  for (unsigned cpu : cpus) {
    mask.set(cpu);
  }
  return mask;
}

} // namespace

ru_emulator_sdr_thread_profile ocudu::resolve_ru_emulator_sdr_thread_profile(const ru_emulator_sdr_appconfig& config)
{
  if (config.device_driver == "zmq" || config.execution_profile == "sequential") {
    return ru_emulator_sdr_thread_profile::sequential;
  }
  if (config.execution_profile == "single") {
    return ru_emulator_sdr_thread_profile::single;
  }
  if (config.execution_profile == "dual") {
    return ru_emulator_sdr_thread_profile::dual;
  }
  if (config.execution_profile == "triple") {
    return ru_emulator_sdr_thread_profile::triple;
  }
  report_error_if_not(
      config.execution_profile == "auto", "Unknown SDR execution profile '{}'", config.execution_profile);
  const unsigned nof_cpus = cpu_architecture_info::get().get_host_nof_available_cpus();
  return nof_cpus < 4   ? ru_emulator_sdr_thread_profile::single
         : nof_cpus < 8 ? ru_emulator_sdr_thread_profile::dual
                        : ru_emulator_sdr_thread_profile::triple;
}

std::optional<execution_config_helper::worker_pool>
ocudu::generate_ru_emulator_phy_worker_pool_config(const ru_emulator_appconfig& config)
{
  unsigned nof_compute_cells = 0;
  unsigned estimated_workers = 2;
  for (const auto& cell : config.ru_cfg) {
    if (cell.sdr_config &&
        resolve_ru_emulator_sdr_thread_profile(*cell.sdr_config) != ru_emulator_sdr_thread_profile::sequential) {
      ++nof_compute_cells;
      estimated_workers += cell.ru_dl_port_id.size() + cell.ru_ul_port_id.size();
    }
  }
  if (nof_compute_cells == 0) {
    return std::nullopt;
  }

  const bool explicit_affinity = !config.phy_cpus.empty();
  // DPDK can narrow the caller's affinity after the startup CPU snapshot.
  auto mask = explicit_affinity ? make_cpu_mask(config.phy_cpus) : os_sched_affinity_bitmask::available_cpus();
  const unsigned available_cpus = mask.count();
  report_error_if_not(available_cpus != 0, "No CPUs available for the lower-PHY compute pool");
  report_error_if_not(!config.nof_phy_threads || *config.nof_phy_threads != 0,
                      "The lower-PHY compute pool requires at least one thread");

  // An unrestricted pool leaves thread capacity for transport and timing; an explicit mask defines the compute CPUs.
  const unsigned spare_cpus  = explicit_affinity ? 0 : std::min(available_cpus, 3U);
  const unsigned max_workers = std::max(1U, available_cpus - spare_cpus);
  const unsigned nof_workers = std::min(config.nof_phy_threads.value_or(estimated_workers), max_workers);

  return execution_config_helper::worker_pool{
      "ru_phy_pool",
      nof_workers,
      {{"ru_phy_compute_exec", concurrent_queue_policy::lockfree_mpmc, phy_pool_queue_size}},
      std::chrono::microseconds{50},
      os_thread_realtime_priority::max() - 2,
      {mask}};
}

ru_emulator_sdr_worker_manager::ru_emulator_sdr_worker_manager(const ru_emulator_appconfig& config)
{
  sdr_exec_mappers.resize(config.ru_cfg.size());
  if (auto pool = generate_ru_emulator_phy_worker_pool_config(config)) {
    report_error_if_not(exec_mng.add_execution_context(create_execution_context(*pool)),
                        "Failed to create the lower-PHY compute pool");
    phy_compute_exec = exec_mng.executors().at("ru_phy_compute_exec");
    ocudulog::fetch_basic_logger("CONFIG").info("Lower-PHY compute pool: workers={}, queue_size={}, backoff={}us",
                                                pool->nof_workers,
                                                phy_pool_queue_size,
                                                pool->sleep_time.count());
  }
  for (unsigned i = 0; i != config.ru_cfg.size(); ++i) {
    if (config.ru_cfg[i].sdr_config) {
      create_sdr_executors(i, *config.ru_cfg[i].sdr_config);
    }
  }
}

ru_emulator_sdr_worker_manager::~ru_emulator_sdr_worker_manager()
{
  stop();
}

ru_sdr_executor_mapper* ru_emulator_sdr_worker_manager::get_executor_mapper(unsigned cell_index) const
{
  ocudu_assert(cell_index < sdr_exec_mappers.size(), "Invalid cell index '{}'", cell_index);
  return sdr_exec_mappers[cell_index].get();
}

void ru_emulator_sdr_worker_manager::stop()
{
  exec_mng.stop();
}

void ru_emulator_sdr_worker_manager::create_sdr_executors(unsigned i, const ru_emulator_sdr_appconfig& sdr_cfg)
{
  using namespace execution_config_helper;

  const std::string suffix = "_#" + std::to_string(i);

  os_sched_affinity_manager affinity_manager({{sched_affinity_mask_types::ru,
                                               make_cpu_mask(sdr_cfg.ru_cpus),
                                               to_affinity_mask_policy(sdr_cfg.pinning_policy)}});

  auto make_worker = [&](const std::string&          name,
                         const std::string&          exec,
                         unsigned                    queue_size,
                         std::chrono::microseconds   sleep,
                         os_thread_realtime_priority prio) -> task_executor* {
    const single_worker worker{name,
                               {exec, concurrent_queue_policy::lockfree_mpmc, queue_size},
                               sleep,
                               prio,
                               affinity_manager.calcute_affinity_mask(sched_affinity_mask_types::ru)};
    if (!exec_mng.add_execution_context(create_execution_context(worker))) {
      report_fatal_error("Failed to instantiate {} execution context", worker.name);
    }
    return exec_mng.executors().at(exec);
  };

  // Radio housekeeping polls asynchronous events without preempting sample transport.
  task_executor* radio = make_worker("ru_radio" + suffix,
                                     "ru_radio_exec" + suffix,
                                     task_worker_queue_size,
                                     std::chrono::microseconds{50},
                                     os_thread_realtime_priority::no_realtime());

  const auto profile = resolve_ru_emulator_sdr_thread_profile(sdr_cfg);

  if (profile == ru_emulator_sdr_thread_profile::sequential) {
    task_executor* bb = make_worker("ru_phy" + suffix,
                                    "ru_phy_exec" + suffix,
                                    task_worker_queue_size,
                                    std::chrono::microseconds{10},
                                    os_thread_realtime_priority::max());

    ru_sdr_executor_mapper_sequential_configuration map_cfg;
    map_cfg.asynchronous_exec = radio;
    map_cfg.common_exec       = bb;
    map_cfg.nof_sectors       = 1;
    sdr_exec_mappers[i]       = create_ru_sdr_executor_mapper(map_cfg);
    return;
  }

  ocudu_assert(phy_compute_exec != nullptr, "Missing lower-PHY compute executor");

  if (profile == ru_emulator_sdr_thread_profile::single) {
    task_executor* bb = make_worker("ru_phy" + suffix,
                                    "ru_phy_exec" + suffix,
                                    128,
                                    std::chrono::microseconds{10},
                                    os_thread_realtime_priority::max());

    ru_sdr_executor_mapper_single_configuration map_cfg;
    map_cfg.radio_exec         = radio;
    map_cfg.high_prio_executor = phy_compute_exec;
    map_cfg.baseband_exec      = {bb};
    sdr_exec_mappers[i]        = create_ru_sdr_executor_mapper(map_cfg);
  } else if (profile == ru_emulator_sdr_thread_profile::dual) {
    task_executor* tx = make_worker("ru_phy_tx" + suffix,
                                    "ru_phy_tx_exec" + suffix,
                                    128,
                                    std::chrono::microseconds{10},
                                    os_thread_realtime_priority::max());
    task_executor* rx = make_worker("ru_phy_rx" + suffix,
                                    "ru_phy_rx_exec" + suffix,
                                    2,
                                    std::chrono::microseconds{10},
                                    os_thread_realtime_priority::max() - 1);

    ru_sdr_executor_mapper_dual_configuration map_cfg;
    map_cfg.radio_exec         = radio;
    map_cfg.high_prio_executor = phy_compute_exec;
    map_cfg.baseband_exec = {ru_sdr_executor_mapper_dual_configuration::cell_executors{.tx_exec = tx, .rx_exec = rx}};
    sdr_exec_mappers[i]   = create_ru_sdr_executor_mapper(map_cfg);
  } else {
    task_executor* tx = make_worker("ru_phy_tx" + suffix,
                                    "ru_phy_tx_exec" + suffix,
                                    128,
                                    std::chrono::microseconds{10},
                                    os_thread_realtime_priority::max());
    task_executor* rx = make_worker("ru_phy_rx" + suffix,
                                    "ru_phy_rx_exec" + suffix,
                                    1,
                                    std::chrono::microseconds{10},
                                    os_thread_realtime_priority::max() - 2);
    task_executor* ul = make_worker("ru_phy_ul" + suffix,
                                    "ru_phy_ul_exec" + suffix,
                                    128,
                                    std::chrono::microseconds{10},
                                    os_thread_realtime_priority::max() - 1);

    ru_sdr_executor_mapper_triple_configuration map_cfg;
    map_cfg.radio_exec         = radio;
    map_cfg.high_prio_executor = phy_compute_exec;
    map_cfg.baseband_exec      = {
        ru_sdr_executor_mapper_triple_configuration::cell_executors{.tx_exec = tx, .rx_exec = rx, .ul_exec = ul}};
    sdr_exec_mappers[i] = create_ru_sdr_executor_mapper(map_cfg);
  }
}
