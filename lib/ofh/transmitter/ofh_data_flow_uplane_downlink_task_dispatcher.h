// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_data_flow_uplane_data.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ofh_controller.h"
#include "ocudu/ofh/ofh_sector_executor_mapper.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include "ocudu/support/executors/task_executor.h"
#include "ocudu/support/ocudu_assert.h"
#include "ocudu/support/rtsan.h"
#include "ocudu/support/synchronization/stop_event.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// \brief Open Fronthaul User-Plane downlink data flow task dispatcher implementation.
///
/// For a requested task the dispatcher fetches an executor associated with an eAxC.
class data_flow_uplane_downlink_task_dispatcher : public data_flow_uplane_data, public operation_controller
{
public:
  data_flow_uplane_downlink_task_dispatcher(ocudulog::basic_logger&                       logger_,
                                            std::unique_ptr<data_flow_uplane_data>        data_flow_uplane_,
                                            ofh_sector_executor_mapper&                   exec_mapper_,
                                            unsigned                                      sector_id_,
                                            data_flow_message_encoding_metrics_collector* metrics_collector_) :
    logger(logger_),
    exec_mapper(exec_mapper_),
    data_flow_uplane(std::move(data_flow_uplane_)),
    sector_id(sector_id_),
    metrics_collector(metrics_collector_)
  {
    ocudu_assert(data_flow_uplane, "Invalid data flow");
  }

  // See interface for documentation.
  void start() override { stop_manager.reset(); }

  // See interface for documentation.
  void stop() override { stop_manager.stop(); }

  // See interface for documentation.
  operation_controller& get_operation_controller() override { return *this; }

  // See interface for documentation.
  void enqueue_section_type_1_message(const data_flow_uplane_resource_grid_context& context,
                                      const shared_resource_grid&                   grid) override
  {
    // Do not process User Plane if the stop was requested.
    auto token = stop_manager.get_token();
    if (OCUDU_UNLIKELY(token.is_stop_requested())) {
      return;
    }

    if (OCUDU_UNLIKELY(
            !exec_mapper.get_dl_up_executor(context.eaxc)
                 .defer([this, context, rg = grid.copy(), tk = std::move(token)]() noexcept OCUDU_RTSAN_NONBLOCKING {
                   data_flow_uplane->enqueue_section_type_1_message(context, rg);
                 }))) {
      if (metrics_collector != nullptr) {
        metrics_collector->increment_dispatch_failures();
      }
      logger.warning("Sector#{}: failed to dispatch message in the downlink data flow User-Plane for slot '{}'",
                     sector_id,
                     context.slot);
    }
  }

  // See interface for documentation.
  data_flow_message_encoding_metrics_collector* get_metrics_collector() override
  {
    return data_flow_uplane->get_metrics_collector();
  }

private:
  ocudulog::basic_logger&                       logger;
  ofh_sector_executor_mapper&                   exec_mapper;
  std::unique_ptr<data_flow_uplane_data>        data_flow_uplane;
  const unsigned                                sector_id;
  data_flow_message_encoding_metrics_collector* metrics_collector;
  rt_stop_event_source                          stop_manager;
};

} // namespace ofh
} // namespace ocudu
