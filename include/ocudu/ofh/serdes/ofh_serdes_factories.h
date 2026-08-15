// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/serdes/ofh_cplane_message_builder.h"
#include "ocudu/ofh/serdes/ofh_cplane_message_decoder.h"
#include "ocudu/ofh/serdes/ofh_uplane_message_builder.h"
#include "ocudu/ofh/serdes/ofh_uplane_message_decoder.h"
#include <memory>

namespace ocudu {
namespace ofh {

class iq_decompressor;
class iq_compressor;

/// Creates an Open Fronthaul Control-Plane static compression message builder.
std::unique_ptr<cplane_message_builder> create_ofh_control_plane_static_compression_message_builder();

/// Creates an Open Fronthaul Control-Plane dynamic compression message builder.
std::unique_ptr<cplane_message_builder> create_ofh_control_plane_dynamic_compression_message_builder();

/// Creates an Open Fronthaul Control-Plane message decoder (O-RU side).
std::unique_ptr<cplane_message_decoder>
create_ofh_control_plane_message_decoder(ocudulog::basic_logger& logger, subcarrier_spacing scs, unsigned sector_id);

/// Creates an Open Fronthaul User-Plane packet builder with static compression header.
std::unique_ptr<uplane_message_builder>
create_static_compr_method_ofh_user_plane_packet_builder(ocudulog::basic_logger& logger, iq_compressor& compressor);

/// Creates an Open Fronthaul User-Plane packet builder with dynamic compression header.
std::unique_ptr<uplane_message_builder>
create_dynamic_compr_method_ofh_user_plane_packet_builder(ocudulog::basic_logger& logger, iq_compressor& compressor);

/// Creates an Open Fronthaul User-Plane packet decoder which supports static compression method.
std::unique_ptr<uplane_message_decoder>
create_static_compr_method_ofh_user_plane_packet_decoder(ocudulog::basic_logger&          logger,
                                                         subcarrier_spacing               scs,
                                                         cyclic_prefix                    cp,
                                                         unsigned                         ru_nof_prbs,
                                                         unsigned                         sector_id_,
                                                         std::unique_ptr<iq_decompressor> decompressor,
                                                         const ru_compression_params&     compr_params,
                                                         data_direction expected_direction = data_direction::uplink);

/// Creates an Open Fronthaul User-Plane packet decoder which supports dynamic compression method.
std::unique_ptr<uplane_message_decoder>
create_dynamic_compr_method_ofh_user_plane_packet_decoder(ocudulog::basic_logger&          logger,
                                                          subcarrier_spacing               scs,
                                                          cyclic_prefix                    cp,
                                                          unsigned                         ru_nof_prbs,
                                                          unsigned                         sector_id_,
                                                          std::unique_ptr<iq_decompressor> decompressor,
                                                          data_direction expected_direction = data_direction::uplink);

} // namespace ofh
} // namespace ocudu
