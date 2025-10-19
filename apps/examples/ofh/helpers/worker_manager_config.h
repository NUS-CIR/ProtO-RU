/*
 *
 * Copyright 2021-2025 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#pragma once

#include "../../../services/worker_manager/os_sched_affinity_manager.h"

namespace srsran {

class timer_manager;

/// Worker manager configuration.
struct worker_manager_config {
  /// RU SDR worker configuration.
  struct ru_sdr_config {
    /// Lower physical layer thread profiles.
    enum class lower_phy_thread_profile {
      /// Same task worker as the rest of the PHY (ZMQ only).
      blocking = 0,
      /// Single task worker for all the lower physical layer task executors.
      single,
      /// Two task workers - one for the downlink and one for the uplink.
      dual,
      /// Dedicated task workers for each of the subtasks (downlink processing, uplink processing, reception and
      /// transmission).
      quad
    };

    lower_phy_thread_profile profile;
    unsigned                 nof_cells;
  };


  /// PCAP worker configuration.
  struct pcap_config {
    bool is_f1ap_enabled = false;
    bool is_ngap_enabled = false;
    bool is_e1ap_enabled = false;
    bool is_e2ap_enabled = false;
    bool is_n3_enabled   = false;
    bool is_f1u_enabled  = false;
    bool is_mac_enabled  = false;
    bool is_rlc_enabled  = false;
  };

  /// Number of low priority threads.
  unsigned nof_low_prio_threads = 4;
  /// Low priority task worker queue size.
  unsigned low_prio_task_queue_size = 2048;
  /// Low priority CPU bitmasks.
  // os_sched_affinity_config low_prio_sched_config = {sched_affinity_mask_types::low_priority,
  //                                                   [](){ os_sched_affinity_bitmask m; m.fill(5,16); return m; }(),
  //                                                   sched_affinity_mask_policy::mask};
  os_sched_affinity_config low_prio_sched_config = {sched_affinity_mask_types::low_priority,
                                                   {},
                                                   sched_affinity_mask_policy::mask};
  /// PCAP configuration.
  pcap_config pcap_cfg;
  /// Timer config.
  timer_manager* app_timers = nullptr;
  /// Vector of affinities mask indexed by cell.
  std::vector<std::vector<os_sched_affinity_config>> config_affinities;
  /// Number of emulators.
  unsigned nof_emulators;
  /// RU SDR configuration.
  std::optional<ru_sdr_config> ru_sdr_cfg;
};

} // namespace srsran
