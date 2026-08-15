// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ocudu/ofh/serdes/ofh_serdes_factories.h"
#include "ofh_cplane_message_builder_dynamic_compression_impl.h"
#include "ofh_cplane_message_builder_static_compression_impl.h"
#include "ofh_cplane_message_decoder_impl.h"
#include "ofh_uplane_message_builder_dynamic_compression_impl.h"
#include "ofh_uplane_message_builder_static_compression_impl.h"
#include "ofh_uplane_message_decoder_dynamic_compression_impl.h"
#include "ofh_uplane_message_decoder_static_compression_impl.h"

using namespace ocudu;
using namespace ofh;

std::unique_ptr<cplane_message_builder> ocudu::ofh::create_ofh_control_plane_static_compression_message_builder()
{
  return std::make_unique<cplane_message_builder_static_compression_impl>();
}

std::unique_ptr<cplane_message_builder> ocudu::ofh::create_ofh_control_plane_dynamic_compression_message_builder()
{
  return std::make_unique<cplane_message_builder_dynamic_compression_impl>();
}

std::unique_ptr<cplane_message_decoder>
ocudu::ofh::create_ofh_control_plane_message_decoder(ocudulog::basic_logger& logger,
                                                     subcarrier_spacing      scs,
                                                     unsigned                sector_id)
{
  return std::make_unique<cplane_message_decoder_impl>(logger, scs, sector_id);
}

std::unique_ptr<uplane_message_builder>
ocudu::ofh::create_static_compr_method_ofh_user_plane_packet_builder(ocudulog::basic_logger& logger,
                                                                     iq_compressor&          compressor)
{
  return std::make_unique<ofh_uplane_message_builder_static_compression_impl>(logger, compressor);
}

std::unique_ptr<uplane_message_builder>
ocudu::ofh::create_dynamic_compr_method_ofh_user_plane_packet_builder(ocudulog::basic_logger& logger,
                                                                      iq_compressor&          compressor)
{
  return std::make_unique<ofh_uplane_message_builder_dynamic_compression_impl>(logger, compressor);
}

std::unique_ptr<uplane_message_decoder>
ocudu::ofh::create_static_compr_method_ofh_user_plane_packet_decoder(ocudulog::basic_logger&          logger,
                                                                     subcarrier_spacing               scs,
                                                                     cyclic_prefix                    cp,
                                                                     unsigned                         ru_nof_prbs,
                                                                     unsigned                         sector_id_,
                                                                     std::unique_ptr<iq_decompressor> decompressor,
                                                                     const ru_compression_params&     compr_params,
                                                                     data_direction expected_direction)
{
  return std::make_unique<uplane_message_decoder_static_compression_impl>(logger,
                                                                          scs,
                                                                          get_nsymb_per_slot(cp),
                                                                          ru_nof_prbs,
                                                                          sector_id_,
                                                                          std::move(decompressor),
                                                                          compr_params,
                                                                          expected_direction);
}

std::unique_ptr<uplane_message_decoder>
ocudu::ofh::create_dynamic_compr_method_ofh_user_plane_packet_decoder(ocudulog::basic_logger&          logger,
                                                                      subcarrier_spacing               scs,
                                                                      cyclic_prefix                    cp,
                                                                      unsigned                         ru_nof_prbs,
                                                                      unsigned                         sector_id_,
                                                                      std::unique_ptr<iq_decompressor> decompressor,
                                                                      data_direction expected_direction)
{
  return std::make_unique<uplane_message_decoder_dynamic_compression_impl>(
      logger, scs, get_nsymb_per_slot(cp), ru_nof_prbs, sector_id_, std::move(decompressor), expected_direction);
}
