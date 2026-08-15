// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/span.h"
#include "ocudu/ofh/serdes/ofh_cplane_message_properties.h"

namespace ocudu {
namespace ofh {

/// Result of decoding an Open Fronthaul Control-Plane message.
///
/// A Control-Plane message carries a single section. The valid fields depend on \c section_type: the radio application
/// header and common section fields are always valid, the compression parameters are valid for section types 1 and 3,
/// the frame structure / time offset / cyclic prefix length are valid for section types 0 and 3, and the frequency
/// offset is valid for section type 3 only.
struct cplane_message_decoder_results {
  /// Number of sections present in the message.
  uint8_t num_sections;
  /// Section type (0, 1 or 3) of the message.
  uint8_t section_type;
  /// Radio application header, common to all section types.
  cplane_radio_application_header radio_hdr;
  /// Common section fields (section types 0, 1 and 3).
  cplane_common_section_0_1_3_5_fields section;
  /// Compression parameters (section types 1 and 3).
  ru_compression_params compr_params;
  /// Time offset from the start of the slot (section types 0 and 3).
  uint16_t time_offset;
  /// Subcarrier spacing carried in the frame structure field (section types 0 and 3).
  cplane_scs frame_structure_scs;
  /// FFT size carried in the frame structure field (section types 0 and 3).
  cplane_fft_size fft_size;
  /// Cyclic prefix length (section types 0 and 3).
  uint16_t cp_length;
  /// Frequency offset with respect to the carrier centre frequency (section type 3 only).
  int frequency_offset;
};

/// \brief Open Fronthaul Control-Plane message decoder interface.
///
/// Decodes a Control-Plane message following the O-RAN Open Fronthaul specification. It is the counterpart of
/// \ref cplane_message_builder: the builder is used by an O-DU to encode Control-Plane scheduling commands, while this
/// decoder is used by an O-RU to recover them.
class cplane_message_decoder
{
public:
  /// Default destructor.
  virtual ~cplane_message_decoder() = default;

  /// \brief Decodes the given Control-Plane message into \c results.
  ///
  /// \param[out] results Decoded Control-Plane parameters.
  /// \param[in]  message Raw Control-Plane message bytes.
  /// \return True on success, false if the message is incomplete, malformed or of an unsupported section type.
  virtual bool decode(cplane_message_decoder_results& results, span<const uint8_t> message) = 0;
};

} // namespace ofh
} // namespace ocudu
