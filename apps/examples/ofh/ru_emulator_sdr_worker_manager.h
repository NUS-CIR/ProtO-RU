// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "apps/services/worker_manager/worker_manager_config.h"
#include "ocudu/ru/sdr/ru_sdr_executor_mapper.h"
#include "ocudu/support/executors/task_execution_manager.h"
#include <optional>

namespace ocudu {

struct ru_emulator_appconfig;
struct ru_emulator_sdr_appconfig;

using ru_emulator_sdr_thread_profile = worker_manager_config::ru_sdr_config::lower_phy_thread_profile;

/// Resolves the lower-PHY thread profile.
ru_emulator_sdr_thread_profile resolve_ru_emulator_sdr_thread_profile(const ru_emulator_sdr_appconfig& config);

/// Generates the shared lower-PHY compute pool configuration.
std::optional<execution_config_helper::worker_pool>
generate_ru_emulator_phy_worker_pool_config(const ru_emulator_appconfig& config);

/// SDR transport workers and shared lower-PHY compute pool.
class ru_emulator_sdr_worker_manager
{
public:
  explicit ru_emulator_sdr_worker_manager(const ru_emulator_appconfig& config);
  ~ru_emulator_sdr_worker_manager();

  /// Returns the SDR executor mapper, or nullptr for a loopback cell.
  ru_sdr_executor_mapper* get_executor_mapper(unsigned cell_index) const;

  /// Stops the SDR workers and the compute pool.
  void stop();

private:
  void create_sdr_executors(unsigned cell_index, const ru_emulator_sdr_appconfig& config);

  task_execution_manager                               exec_mng;
  task_executor*                                       phy_compute_exec = nullptr;
  std::vector<std::unique_ptr<ru_sdr_executor_mapper>> sdr_exec_mappers;
};

} // namespace ocudu
