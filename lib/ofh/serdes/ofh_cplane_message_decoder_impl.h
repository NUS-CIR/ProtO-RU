// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/serdes/ofh_cplane_message_decoder.h"
#include "ocudu/ran/subcarrier_spacing.h"

namespace ocudu {
namespace ofh {

class network_order_binary_deserializer;

/// Open Fronthaul Control-Plane message decoder implementation.
class cplane_message_decoder_impl : public cplane_message_decoder
{
public:
  cplane_message_decoder_impl(ocudulog::basic_logger& logger_, subcarrier_spacing scs_, unsigned sector_id_) :
    logger(logger_), scs(scs_), sector_id(sector_id_)
  {
  }

  // See interface for documentation.
  bool decode(cplane_message_decoder_results& results, span<const uint8_t> message) override;

private:
  /// Decodes the radio application header (common to all section types) and returns true on success.
  bool decode_radio_app_header(cplane_message_decoder_results&    results,
                               network_order_binary_deserializer& deserializer);

  /// Decodes a section type 0 (idle/guard period) message tail and returns true on success.
  bool decode_section_type_0(cplane_message_decoder_results& results, network_order_binary_deserializer& deserializer);

  /// Decodes a section type 1 (DL/UL radio channel) message tail and returns true on success.
  bool decode_section_type_1(cplane_message_decoder_results& results, network_order_binary_deserializer& deserializer);

  /// Decodes a section type 3 (PRACH/mixed-numerology) message tail and returns true on success.
  bool decode_section_type_3(cplane_message_decoder_results& results, network_order_binary_deserializer& deserializer);

  /// Decodes the section fields common to section types 0, 1 and 3.
  void decode_common_section_fields(cplane_common_section_0_1_3_5_fields& section,
                                    network_order_binary_deserializer&    deserializer);

  /// Decodes the one-byte compression header into the given compression parameters.
  void decode_compression_header(ru_compression_params& compr, network_order_binary_deserializer& deserializer);

  ocudulog::basic_logger&  logger;
  const subcarrier_spacing scs;
  const unsigned           sector_id;
};

} // namespace ofh
} // namespace ocudu
