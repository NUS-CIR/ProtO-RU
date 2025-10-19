#pragma once

#include "srsran/ru/generic/ru_generic_configuration.h"

namespace srsran {

namespace srs_du {
struct du_cell_config;
}

struct ru_sdr_unit_config;
struct worker_manager_config;
lower_phy_configuration generate_low_phy_config(const srs_du::du_cell_config& config,
                                                const ru_sdr_unit_config&     ru_cfg,
                                                unsigned                       max_processing_delay_slot);
/// Converts and returns the given RU SDR application unit configuration to a SDR RU configuration.
ru_generic_configuration generate_ru_sdr_config(const ru_sdr_unit_config&          ru_cfg,
                                                span<const srs_du::du_cell_config> du_cells,
                                                unsigned                           max_processing_delay_slots);

/// Fills radio config.    
void generate_radio_config(radio_configuration::radio& out_cfg, const ru_sdr_unit_config& ru_cfg, span<const srs_du::du_cell_config> du_cells);

/// Fills the SDR worker manager parameters of the given worker manager configuration.
void fill_ru_worker_manager_config(worker_manager_config& config, const ru_sdr_unit_config& ru_cfg);

} // namespace srsran