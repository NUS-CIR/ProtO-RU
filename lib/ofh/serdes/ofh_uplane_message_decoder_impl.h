// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/compression/iq_decompressor.h"
#include "ocudu/ofh/serdes/ofh_uplane_message_decoder.h"

namespace ocudu {
namespace ofh {

class iq_decompressor;
class network_order_binary_deserializer;

/// Open Fronthaul User-Plane section parameter for this decoder implementation.
struct decoder_uplane_section_params {
  /// Section identifier.
  unsigned section_id;
  /// Resource block indicator.
  bool is_every_rb_used;
  /// Symbol number increment command.
  bool use_current_symbol_number;
  /// Start PRB.
  unsigned start_prb;
  /// Number of PRBs (though a value of 0 signals more than 255 PRBs in the OFH specification, this field always
  /// contains the real amount of PRBs).
  unsigned nof_prbs;
  /// User data compression header.
  ru_compression_params ud_comp_hdr;
  /// User data compression length.
  std::optional<unsigned> ud_comp_len;
  /// User data compression parameter.
  /// \note For simplicity, all the PRBs use the same compression parameters.
  std::optional<unsigned> ud_comp_param;
};

/// Open Fronthaul User-Plane message decoder implementation.
class uplane_message_decoder_impl : public uplane_message_decoder
{
protected:
  /// Decoded status of a section.
  enum class decoded_section_status { ok, incomplete, malformed };

public:
  uplane_message_decoder_impl(ocudulog::basic_logger&          logger_,
                              subcarrier_spacing               scs_,
                              unsigned                         nof_symbols_,
                              unsigned                         ru_nof_prbs_,
                              unsigned                         sector_id_,
                              std::unique_ptr<iq_decompressor> decompressor_,
                              data_direction                   expected_direction_ = data_direction::uplink) :
    logger(logger_),
    decompressor(std::move(decompressor_)),
    scs(scs_),
    nof_symbols(nof_symbols_),
    ru_nof_prbs(ru_nof_prbs_),
    sector_id(sector_id_),
    expected_direction(expected_direction_)
  {
    ocudu_assert(decompressor, "Invalid IQ decompressor");
  }

  // See interface for documentation.
  bool decode(uplane_message_decoder_results& results, span<const uint8_t> message) override;

private:
  /// Decodes the User-Plane message header and returns true on success, otherwise false.
  bool decode_header(uplane_message_params& params, network_order_binary_deserializer& deserializer);

  /// Decodes all sections and returns true on success, otherwise false.
  bool decode_all_sections(uplane_message_decoder_results& results, network_order_binary_deserializer& deserializer);

  /// Decodes a single section and returns the decoded section status.
  decoded_section_status decode_section(uplane_message_decoder_results&    results,
                                        network_order_binary_deserializer& deserializer);

  /// Decodes the section header and returns the decoded section status.
  decoded_section_status decode_section_header(decoder_uplane_section_params&     results,
                                               network_order_binary_deserializer& deserializer);

  /// Decodes the compression length field and returns the decoded section status.
  decoded_section_status decode_compression_length(decoder_uplane_section_params&     results,
                                                   network_order_binary_deserializer& deserializer,
                                                   const ru_compression_params&       compression_params);

  /// Decodes the IQ data from the given deserializer.
  void decode_iq_data(uplane_section_params&             results,
                      network_order_binary_deserializer& deserializer,
                      const ru_compression_params&       compression_params);

  /// Decodes the compression header and returns the decoded section status.
  virtual decoded_section_status decode_compression_header(decoder_uplane_section_params&     results,
                                                           network_order_binary_deserializer& deserializer) = 0;

protected:
  ocudulog::basic_logger&          logger;
  std::unique_ptr<iq_decompressor> decompressor;
  const subcarrier_spacing         scs;
  const unsigned                   nof_symbols;
  const unsigned                   ru_nof_prbs;
  const unsigned                   sector_id;
  /// Expected data direction of received messages (uplink for a DU receiver, downlink for an RU receiver).
  const data_direction expected_direction;
};

} // namespace ofh
} // namespace ocudu
