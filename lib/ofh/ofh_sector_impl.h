// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_metrics_collector_impl.h"
#include "ofh_sector_controller.h"
#include "support/prach_context_repository.h"
#include "support/rx_grid_context_repository.h"
#include "support/uplink_cplane_context_repository.h"
#include "ocudu/ofh/ethernet/ethernet_receiver.h"
#include "ocudu/ofh/ethernet/ethernet_transmitter.h"
#include "ocudu/ofh/ofh_sector.h"
#include "ocudu/ofh/receiver/ofh_receiver.h"
#include "ocudu/ofh/transmitter/ofh_transmitter.h"

namespace ocudu {
namespace ofh {

/// Sector implementation configuration.
struct sector_impl_config {
  unsigned sector_id;
  bool     are_metrics_enabled = false;
};

/// Sector implementation dependencies.
struct sector_impl_dependencies {
  std::unique_ptr<receiver>                   ofh_receiver;
  std::unique_ptr<transmitter>                ofh_transmitter;
  std::shared_ptr<rx_grid_context_repository> ul_data_repo;
  std::shared_ptr<prach_context_repository>   ul_prach_repo;
  ether::transmitter&                         eth_transmitter;
  ether::receiver&                            eth_receiver;
};

/// Open Fronthaul sector implementation.
class sector_impl : public sector
{
public:
  sector_impl(const sector_impl_config& config, sector_impl_dependencies&& dependencies) :
    ofh_receiver(std::move(dependencies.ofh_receiver)),
    ofh_transmitter(std::move(dependencies.ofh_transmitter)),
    ofh_sector_controller(*ofh_transmitter,
                          *ofh_receiver,
                          std::move(dependencies.ul_data_repo),
                          std::move(dependencies.ul_prach_repo)),
    ofh_metrics_collector(ofh_receiver->get_metrics_collector(),
                          ofh_transmitter->get_metrics_collector(),
                          config.sector_id)
  {
    ocudu_assert(ofh_receiver, "Invalid Open Fronthaul receiver");
    ocudu_assert(ofh_transmitter, "Invalid Open Fronthaul transmitter");
  }

  // See interface for documentation.
  receiver& get_receiver() override;

  // See interface for documentation.
  transmitter& get_transmitter() override;

  // See interface for documentation.
  operation_controller& get_operation_controller() override;

  // See interface for documentation.
  metrics_collector* get_metrics_collector() override;

private:
  std::unique_ptr<receiver>    ofh_receiver;
  std::unique_ptr<transmitter> ofh_transmitter;
  sector_controller            ofh_sector_controller;
  metrics_collector_impl       ofh_metrics_collector;
};

} // namespace ofh
} // namespace ocudu
