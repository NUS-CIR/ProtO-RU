// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../transmitter/ofh_data_flow_uplane_data.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include "ocudu/support/error_handling.h"
#include "ocudu/support/executors/task_executor.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// \brief Decouples the uplink User-Plane build + compression from the calling thread.
///
/// Wraps the uplink User-Plane data flow and defers \ref enqueue_section_type_1_message to a dedicated OFH encode
/// executor, so the captured IQ is handed off (a cheap shared-grid ref copy) and the baseband / immediate-response
/// thread is not blocked by the compression. This mirrors the O-DU's \ref data_flow_uplane_downlink_task_dispatcher
/// (the downlink direction). The PRACH path is forwarded inline — PRACH is sparse, so it is not worth deferring.
///
/// Lifetime: the deferred tasks reference the wrapped data flow, so the encode executor must be stopped before this
/// dispatcher is destroyed (the application stops its workers before tearing down the sector).
class ru_uplink_task_dispatcher : public data_flow_uplane_data
{
public:
  ru_uplink_task_dispatcher(ocudulog::basic_logger&                logger_,
                            std::unique_ptr<data_flow_uplane_data> data_flow_,
                            task_executor&                         executor_,
                            unsigned                               sector_id_) :
    logger(logger_), data_flow(std::move(data_flow_)), executor(executor_), sector_id(sector_id_)
  {
    ocudu_assert(data_flow, "Invalid uplink data flow");
  }

  // See interface for documentation.
  operation_controller& get_operation_controller() override { return data_flow->get_operation_controller(); }

  // See interface for documentation.
  void enqueue_section_type_1_message(const data_flow_uplane_resource_grid_context& context,
                                      const shared_resource_grid&                   grid) override
  {
    if (!executor.defer(
            [this, context, rg = grid.copy()]() noexcept { data_flow->enqueue_section_type_1_message(context, rg); })) {
      logger.warning(
          "Sector#{}: failed to dispatch the uplink User-Plane encode for slot '{}'", sector_id, context.slot);
    }
  }

  // See interface for documentation. PRACH is sparse, so it is built inline rather than deferred.
  void enqueue_prach_message(const data_flow_uplane_prach_context& context, const prach_buffer& buffer) override
  {
    data_flow->enqueue_prach_message(context, buffer);
  }

  // See interface for documentation.
  data_flow_message_encoding_metrics_collector* get_metrics_collector() override
  {
    return data_flow->get_metrics_collector();
  }

private:
  ocudulog::basic_logger&                logger;
  std::unique_ptr<data_flow_uplane_data> data_flow;
  task_executor&                         executor;
  const unsigned                         sector_id;
};

} // namespace ofh
} // namespace ocudu
