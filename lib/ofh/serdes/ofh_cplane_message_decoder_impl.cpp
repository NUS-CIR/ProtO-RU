// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_cplane_message_decoder_impl.h"
#include "../support/network_order_binary_deserializer.h"
#include "ofh_cuplane_constants.h"
#include "ocudu/ofh/compression/compression_params.h"

using namespace ocudu;
using namespace ofh;

/// Number of bytes of the Control-Plane radio application header (data direction, frame, subframe/slot, slot/start
/// symbol, number of sections and section type).
static constexpr unsigned NOF_BYTES_CP_RADIO_HDR = 6;

/// Number of bytes of the section fields common to section types 0, 1 and 3.
static constexpr unsigned NOF_BYTES_COMMON_SECTION = 6;

void cplane_message_decoder_impl::decode_common_section_fields(cplane_common_section_0_1_3_5_fields& section,
                                                               network_order_binary_deserializer&    deserializer)
{
  // 8 MSB of the section identifier.
  uint8_t section_id_msb = deserializer.read<uint8_t>();

  // 4 LSB of section identifier, RB indicator, symbol increment and 2 MSB of PRB start.
  uint8_t sect_id_rb_symbols = deserializer.read<uint8_t>();
  section.section_id         = (uint16_t(section_id_msb) << 4) | (sect_id_rb_symbols >> 4);

  // PRB start (8 LSB) completed with the 2 MSB decoded above.
  uint8_t prb_start_lsb = deserializer.read<uint8_t>();
  section.prb_start     = (uint16_t(sect_id_rb_symbols & 0x3) << 8) | prb_start_lsb;

  // Number of PRBs.
  section.nof_prb = deserializer.read<uint8_t>();

  // 8 MSB of the RE mask.
  uint8_t re_mask_msb = deserializer.read<uint8_t>();

  // 4 LSB of the RE mask and number of symbols.
  uint8_t re_mask_and_symbols = deserializer.read<uint8_t>();
  section.re_mask             = (uint16_t(re_mask_msb) << 4) | (re_mask_and_symbols >> 4);
  section.nof_symbols         = re_mask_and_symbols & 0x0f;
}

void cplane_message_decoder_impl::decode_compression_header(ru_compression_params&             compr,
                                                            network_order_binary_deserializer& deserializer)
{
  // For static compression and for downlink dynamic compression this byte is reserved (zero); the actual compression
  // is then configured out of band. It is decoded here unconditionally as the field is always present.
  uint8_t value       = deserializer.read<uint8_t>();
  compr.type          = to_compression_type(value & 0x0f);
  unsigned data_width = value >> 4U;
  compr.data_width    = (data_width == 0) ? MAX_IQ_WIDTH : data_width;
}

bool cplane_message_decoder_impl::decode_radio_app_header(cplane_message_decoder_results&    results,
                                                          network_order_binary_deserializer& deserializer)
{
  if (OCUDU_UNLIKELY(deserializer.remaining_bytes() < NOF_BYTES_CP_RADIO_HDR)) {
    logger.info("Sector#{}: dropped received Open Fronthaul Control-Plane message of '{}' bytes as it is smaller than "
                "the radio application header",
                sector_id,
                deserializer.remaining_bytes());

    return false;
  }

  // Data direction, payload version and filter index.
  uint8_t direction_byte         = deserializer.read<uint8_t>();
  results.radio_hdr.direction    = static_cast<data_direction>(direction_byte >> 7);
  unsigned version               = (direction_byte >> 4) & 0x7;
  results.radio_hdr.filter_index = to_filter_index_type(direction_byte & 0xf);

  if (OCUDU_UNLIKELY(version != OFH_PAYLOAD_VERSION)) {
    logger.info("Sector#{}: dropped received Open Fronthaul Control-Plane message as its payload version is '{}' but "
                "only version '{}' is supported",
                sector_id,
                version,
                OFH_PAYLOAD_VERSION);

    return false;
  }

  uint8_t  frame             = deserializer.read<uint8_t>();
  uint8_t  subframe_and_slot = deserializer.read<uint8_t>();
  uint8_t  subframe          = subframe_and_slot >> 4;
  unsigned slot_id           = (subframe_and_slot & 0x0f) << 2;

  uint8_t slot_and_symbol = deserializer.read<uint8_t>();
  slot_id |= (slot_and_symbol >> 6) & 0x3;
  results.radio_hdr.start_symbol = slot_and_symbol & 0x3f;
  results.radio_hdr.slot         = slot_point(to_numerology_value(scs), frame, subframe, slot_id);

  results.num_sections = deserializer.read<uint8_t>();
  results.section_type = deserializer.read<uint8_t>();

  if (OCUDU_UNLIKELY(results.num_sections != 1)) {
    logger.info("Sector#{}: dropped received Open Fronthaul Control-Plane message containing '{}' sections as only "
                "single-section messages are supported",
                sector_id,
                results.num_sections);

    return false;
  }

  return true;
}

bool cplane_message_decoder_impl::decode_section_type_1(cplane_message_decoder_results&    results,
                                                        network_order_binary_deserializer& deserializer)
{
  // Compression header, reserved byte, common section fields and section type 1 extension.
  static constexpr unsigned NOF_BYTES = 1 + 1 + NOF_BYTES_COMMON_SECTION + 2;
  if (OCUDU_UNLIKELY(deserializer.remaining_bytes() < NOF_BYTES)) {
    logger.info("Sector#{}: dropped received Open Fronthaul Control-Plane section type 1 message as it is incomplete",
                sector_id);

    return false;
  }

  decode_compression_header(results.compr_params, deserializer);

  // Reserved byte.
  deserializer.advance(1);

  decode_common_section_fields(results.section, deserializer);

  // Section type 1 extension (EF and beam identifier); extensions and beamforming are not supported.
  deserializer.advance(2);

  return true;
}

bool cplane_message_decoder_impl::decode_section_type_0(cplane_message_decoder_results&    results,
                                                        network_order_binary_deserializer& deserializer)
{
  // Time offset, frame structure, cyclic prefix length, reserved byte, common section fields and section type 0
  // extension.
  static constexpr unsigned NOF_BYTES = 2 + 1 + 2 + 1 + NOF_BYTES_COMMON_SECTION + 2;
  if (OCUDU_UNLIKELY(deserializer.remaining_bytes() < NOF_BYTES)) {
    logger.info("Sector#{}: dropped received Open Fronthaul Control-Plane section type 0 message as it is incomplete",
                sector_id);

    return false;
  }

  results.time_offset = deserializer.read<uint16_t>();

  uint8_t frame_structure     = deserializer.read<uint8_t>();
  results.frame_structure_scs = static_cast<cplane_scs>(frame_structure & 0x0f);
  results.fft_size            = static_cast<cplane_fft_size>(frame_structure >> 4);

  results.cp_length = deserializer.read<uint16_t>();

  // Reserved byte.
  deserializer.advance(1);

  decode_common_section_fields(results.section, deserializer);

  // Section type 0 extension (EF and reserved); extensions are not supported.
  deserializer.advance(2);

  return true;
}

bool cplane_message_decoder_impl::decode_section_type_3(cplane_message_decoder_results&    results,
                                                        network_order_binary_deserializer& deserializer)
{
  // Time offset, frame structure, cyclic prefix length, compression header, common section fields, section type 3
  // extension (EF and beam id), frequency offset and reserved byte.
  static constexpr unsigned NOF_BYTES = 2 + 1 + 2 + 1 + NOF_BYTES_COMMON_SECTION + 2 + 3 + 1;
  if (OCUDU_UNLIKELY(deserializer.remaining_bytes() < NOF_BYTES)) {
    logger.info("Sector#{}: dropped received Open Fronthaul Control-Plane section type 3 message as it is incomplete",
                sector_id);

    return false;
  }

  results.time_offset = deserializer.read<uint16_t>();

  uint8_t frame_structure     = deserializer.read<uint8_t>();
  results.frame_structure_scs = static_cast<cplane_scs>(frame_structure & 0x0f);
  results.fft_size            = static_cast<cplane_fft_size>(frame_structure >> 4);

  results.cp_length = deserializer.read<uint16_t>();

  decode_compression_header(results.compr_params, deserializer);

  decode_common_section_fields(results.section, deserializer);

  // Section type 3 extension (EF and beam identifier); extensions and beamforming are not supported.
  deserializer.advance(2);

  // Frequency offset, a signed 24-bit value.
  uint8_t  freq_offset_msb = deserializer.read<uint8_t>();
  uint16_t freq_offset_lsb = deserializer.read<uint16_t>();
  int      freq_offset     = (int(freq_offset_msb) << 16) | freq_offset_lsb;
  // Sign-extend from 24 bits.
  if (freq_offset & 0x800000) {
    freq_offset |= ~0xffffff;
  }
  results.frequency_offset = freq_offset;

  // Reserved byte.
  deserializer.advance(1);

  return true;
}

bool cplane_message_decoder_impl::decode(cplane_message_decoder_results& results, span<const uint8_t> message)
{
  results = cplane_message_decoder_results{};

  network_order_binary_deserializer deserializer(message);

  if (!decode_radio_app_header(results, deserializer)) {
    return false;
  }

  switch (results.section_type) {
    case 0:
      return decode_section_type_0(results, deserializer);
    case 1:
      return decode_section_type_1(results, deserializer);
    case 3:
      return decode_section_type_3(results, deserializer);
    default:
      logger.info("Sector#{}: dropped received Open Fronthaul Control-Plane message with unsupported section type '{}'",
                  sector_id,
                  results.section_type);
      return false;
  }
}
