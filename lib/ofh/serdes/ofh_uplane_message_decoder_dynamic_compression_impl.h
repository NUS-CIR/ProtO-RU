// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_uplane_message_decoder_impl.h"
#include "ocudu/ocudulog/logger.h"

namespace ocudu {
namespace ofh {

class iq_decompressor;
class network_order_binary_deserializer;

/// Open Fronthaul User-Plane message decoder implementation with dynamic IQ compression.
class uplane_message_decoder_dynamic_compression_impl : public uplane_message_decoder_impl
{
public:
  uplane_message_decoder_dynamic_compression_impl(ocudulog::basic_logger&          logger_,
                                                  subcarrier_spacing               scs_,
                                                  unsigned                         nof_symbols_,
                                                  unsigned                         ru_nof_prbs_,
                                                  unsigned                         sector_id_,
                                                  std::unique_ptr<iq_decompressor> decompressor_,
                                                  data_direction expected_direction_ = data_direction::uplink) :
    uplane_message_decoder_impl(logger_,
                                scs_,
                                nof_symbols_,
                                ru_nof_prbs_,
                                sector_id_,
                                std::move(decompressor_),
                                expected_direction_)
  {
  }

private:
  // See parent for documentation.
  decoded_section_status decode_compression_header(decoder_uplane_section_params&     results,
                                                   network_order_binary_deserializer& deserializer) override;
};

} // namespace ofh
} // namespace ocudu
