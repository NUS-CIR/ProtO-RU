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

#include "data_flow_uplane_uplink.h"
#include "srsran/phy/support/shared_resource_grid.h"
#include "srsran/support/executors/task_executor.h"
#include <memory>

namespace srsran {
namespace ofh {

/// Open Fronthaul User-Plane downlink data flow task dispatcher implementation.
class data_flow_uplane_uplink_task_dispatcher : public data_flow_uplane_uplink_data
{
public:
  data_flow_uplane_uplink_task_dispatcher(std::unique_ptr<data_flow_uplane_uplink_data> data_flow_uplane_,
                                            task_executor&                                  executor_,
                                            unsigned                                        sector_id_) :
    data_flow_uplane(std::move(data_flow_uplane_)), executor(executor_), sector_id(sector_id_)
  {
    srsran_assert(data_flow_uplane, "Invalid data flow");
  }

  // See interface for documentation.
  void enqueue_section_type_1_message(const data_flow_uplane_rg_context& context,
                                      const shared_resource_grid&        grid,
                                      const unsigned                     symbol_id) override
  {
    if (!executor.execute(
            [this, context, rg = grid.copy(), symbol_id]() { 
              data_flow_uplane->enqueue_section_type_1_message(context, rg, symbol_id);
            })) {
      srslog::fetch_basic_logger("OFH_UL").warning(
          "Sector#{}: failed to dispatch message in the uplink data flow User-Plane for slot '{}'",
          sector_id,
          context.slot);
    }
  }

  // See interface for documentation.
  void enqueue_prach_message(const data_flow_uplane_prach_context& context,
                             const prach_buffer&         buffer) override
  {
    if (!executor.execute(
            [this, context, &buffer] { 
              data_flow_uplane->enqueue_prach_message(context, buffer);
            })) {
      srslog::fetch_basic_logger("OFH_UL").warning(
          "Sector#{}: failed to dispatch message in the uplink PRACH flow User-Plane for slot '{}'",
          sector_id,
          context.slot);
    }
  }

private:
  std::unique_ptr<data_flow_uplane_uplink_data> data_flow_uplane;
  task_executor&                                  executor;
  const unsigned                                  sector_id;
};

} // namespace ofh
} // namespace srsran
