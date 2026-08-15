// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_data_flow_uplane_data_impl.h"
#include "ofh_uplane_fragment_size_calculator.h"
#include "ocudu/ocuduvec/conversion.h"
#include "ocudu/ofh/ethernet/ethernet_frame_pool.h"
#include "ocudu/ofh/timing/slot_symbol_point.h"
#include "ocudu/phy/support/prach_buffer.h"
#include "ocudu/phy/support/resource_grid_context.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include "ocudu/ran/prach/prach_frequency_mapping.h"
#include "ocudu/ran/resource_block.h"
#include <thread>

using namespace ocudu;
using namespace ofh;

/// Generates and returns the Open Fronthaul user parameters for the given context and data direction.
static uplane_message_params generate_ofh_user_parameters(data_direction               direction,
                                                          filter_index_type            filter_index,
                                                          slot_point                   slot,
                                                          unsigned                     symbol_id,
                                                          unsigned                     start_prb,
                                                          unsigned                     nof_prb,
                                                          const ru_compression_params& comp,
                                                          uint16_t                     section_id)
{
  uplane_message_params params;
  params.direction                     = direction;
  params.slot                          = slot;
  params.filter_index                  = filter_index;
  params.start_prb                     = start_prb;
  params.nof_prb                       = nof_prb;
  params.symbol_id                     = symbol_id;
  params.sect_type                     = section_type::type_1;
  params.compression_params.type       = comp.type;
  params.compression_params.data_width = comp.data_width;
  params.section_id                    = section_id;

  return params;
}

/// Generates and returns the eCPRI IQ data parameters.
static ecpri::iq_data_parameters generate_ecpri_data_parameters(uint16_t seq_id, uint16_t eaxc)
{
  ecpri::iq_data_parameters params;
  // Only supporting 1 Port, 1 band and 1 CC.
  params.pc_id  = eaxc;
  params.seq_id = 0;

  // Set seq_id.
  params.seq_id |= (seq_id & 0x00ff) << 8;
  // Set the E bit to 1 and subsequence ID to 0. E bit set to 1 indicates that there is no radio transport layer
  // fragmentation.
  params.seq_id |= uint16_t(1U) << 7;

  return params;
}

data_flow_uplane_data_impl::data_flow_uplane_data_impl(const data_flow_uplane_data_impl_config&  config,
                                                       data_flow_uplane_data_impl_dependencies&& dependencies) :
  logger(*dependencies.logger),
  nof_symbols_per_slot(get_nsymb_per_slot(config.cp)),
  ru_nof_prbs(config.ru_nof_prbs),
  sector_id(config.sector),
  compr_params(config.compr_params),
  direction(config.direction),
  frame_pool(std::move(dependencies.frame_pool)),
  compressor_sel(std::move(dependencies.compressor_sel)),
  eth_builder(std::move(dependencies.eth_builder)),
  ecpri_builder(std::move(dependencies.ecpri_builder)),
  up_builder(std::move(dependencies.up_builder)),
  formatted_trace_names(config.dl_eaxc)
{
  ocudu_assert(eth_builder, "Invalid Ethernet VLAN packet builder");
  ocudu_assert(ecpri_builder, "Invalid eCPRI packet builder");
  ocudu_assert(compressor_sel, "Invalid compressor selector");
  ocudu_assert(up_builder, "Invalid User-Plane message builder");
  ocudu_assert(frame_pool, "Invalid frame pool");
}

void data_flow_uplane_data_impl::enqueue_section_type_1_message(const data_flow_uplane_resource_grid_context& context,
                                                                const shared_resource_grid&                   grid)
{
  trace_point tp = ofh_tracer.now();
  enqueue_section_type_1_message_symbol_burst(context, grid);

  ofh_tracer << trace_event(formatted_trace_names[context.eaxc].c_str(), tp);
}

void data_flow_uplane_data_impl::enqueue_prach_message(const data_flow_uplane_prach_context& context,
                                                       const prach_buffer&                   buffer)
{
  trace_point tp = ofh_tracer.now();

  const unsigned nof_re = context.nof_prb * NOF_SUBCARRIERS_PER_RB;
  ocudu_assert(
      nof_re <= MAX_NOF_PRBS * NOF_SUBCARRIERS_PER_RB, "PRACH section is too large: '{}' PRBs", context.nof_prb);

  subcarrier_spacing                  pusch_scs = to_subcarrier_spacing(context.slot.numerology());
  prach_frequency_mapping_information mapping   = prach_frequency_mapping_get(context.prach_scs, pusch_scs);
  if (mapping.nof_rb_ra == 0) {
    logger.warning("Sector#{}: cannot build PRACH User-Plane message for slot '{}': unsupported PRACH SCS '{}' and "
                   "PUSCH SCS '{}'",
                   sector_id,
                   context.slot,
                   to_string(context.prach_scs),
                   to_string(pusch_scs));
    return;
  }

  const unsigned offset = mapping.k_bar;
  if ((offset > nof_re) || (buffer.get_sequence_length() > nof_re - offset)) {
    logger.warning("Sector#{}: cannot build PRACH User-Plane message for slot '{}': section of '{}' REs cannot hold "
                   "'{}' PRACH samples after the '{}' RE frequency-domain offset",
                   sector_id,
                   context.slot,
                   nof_re,
                   buffer.get_sequence_length(),
                   offset);
    return;
  }

  units::bytes headers_size = eth_builder->get_header_size() +
                              ecpri_builder->get_header_size(ecpri::message_type::iq_data) +
                              up_builder->get_header_size(compr_params);

  // PRACH IQ for one symbol, zero-padded up to the PRB grid. PRACH is small and always fits in a single message.
  std::array<cbf16_t, MAX_NOF_PRBS * NOF_SUBCARRIERS_PER_RB> iq_buffer;

  for (unsigned prach_symbol = 0; prach_symbol != context.nof_symbols; ++prach_symbol) {
    unsigned          ofh_symbol_id = context.start_symbol + prach_symbol;
    slot_symbol_point symbol_point(context.slot, ofh_symbol_id, nof_symbols_per_slot);

    // Read the PRACH preamble samples for this symbol (single occasion) and place them after the k_bar frequency-domain
    // guard derived from the PRACH and PUSCH subcarrier spacings.
    span<const cbf16_t> prach_iq = buffer.get_symbol(context.port, 0, 0, prach_symbol);
    span<cbf16_t>       iq_data(iq_buffer.data(), nof_re);
    std::fill(iq_data.begin(), iq_data.end(), cbf16_t{});
    std::copy(prach_iq.begin(), prach_iq.end(), iq_data.begin() + offset);

    auto scoped_buffer = frame_pool->reserve(symbol_point);
    if (OCUDU_UNLIKELY(!scoped_buffer)) {
      logger.warning("Sector#{}: not enough space in the buffer pool to create a PRACH User-Plane message for slot "
                     "'{}' and eAxC '{}', symbol_id '{}'",
                     sector_id,
                     context.slot,
                     context.eaxc,
                     ofh_symbol_id);
      return;
    }
    span<uint8_t> data = scoped_buffer->get_buffer();
    if (OCUDU_UNLIKELY(data.size() <= headers_size.value())) {
      logger.warning(
          "Sector#{}: frame buffer of '{}' bytes is too small to carry a PRACH message", sector_id, data.size());
      continue;
    }

    uplane_message_params up_params = generate_ofh_user_parameters(direction,
                                                                   context.filter_index,
                                                                   context.slot,
                                                                   ofh_symbol_id,
                                                                   context.prb_start,
                                                                   context.nof_prb,
                                                                   compr_params,
                                                                   context.section_id);

    unsigned used_size = enqueue_section_type_1_message_symbol(iq_data, up_params, context.eaxc, data);
    scoped_buffer->set_size(used_size);
  }

  ofh_tracer << trace_event(formatted_trace_names[context.eaxc].c_str(), tp);
}

void data_flow_uplane_data_impl::enqueue_section_type_1_message_symbol_burst(
    const data_flow_uplane_resource_grid_context& context,
    const shared_resource_grid&                   grid)
{
  const resource_grid_reader& reader = grid.get_reader();

  // Temporary buffer used to store IQ data when the RU operating bandwidth is not the same to the cell bandwidth.
  std::array<cbf16_t, MAX_NOF_SUBCARRIERS> temp_buffer;
  if (OCUDU_UNLIKELY(ru_nof_prbs * NOF_SUBCARRIERS_PER_RB != reader.get_nof_subc())) {
    // Zero out the elements that won't be filled after reading the resource grid.
    std::fill(temp_buffer.begin() + reader.get_nof_subc(), temp_buffer.end(), 0);
  }

  units::bytes headers_size = eth_builder->get_header_size() +
                              ecpri_builder->get_header_size(ecpri::message_type::iq_data) +
                              up_builder->get_header_size(compr_params);

  // Iterate over the scheduled symbols [start, stop). Using stop() (rather than length()) makes this correct for a
  // non-zero start symbol, which an O-RU transmitter can have; for the O-DU the start is 0 so the result is unchanged.
  for (unsigned symbol_id = context.symbol_range.start(), symbol_end = context.symbol_range.stop();
       symbol_id != symbol_end;
       ++symbol_id) {
    slot_symbol_point symbol_point(context.slot, symbol_id, nof_symbols_per_slot);

    span<const cbf16_t> iq_data;
    if (OCUDU_LIKELY(ru_nof_prbs * NOF_SUBCARRIERS_PER_RB == reader.get_nof_subc())) {
      iq_data = reader.get_view(context.port, symbol_id);
    } else {
      span<cbf16_t> temp_iq_data(temp_buffer.data(), ru_nof_prbs * NOF_SUBCARRIERS_PER_RB);
      reader.get(temp_iq_data.first(reader.get_nof_subc()), context.port, symbol_id, 0);
      iq_data = temp_iq_data;
    }

    // Split the data into multiple messages when it does not fit into a single one.
    ofh_uplane_fragment_size_calculator prb_fragment_calculator(0, ru_nof_prbs, compr_params);
    bool                                is_last_fragment   = false;
    unsigned                            fragment_start_prb = 0U;
    unsigned                            fragment_nof_prbs  = 0U;
    do {
      trace_point pool_access_tp = ofh_tracer.now();
      auto        scoped_buffer  = frame_pool->reserve(symbol_point);
      ofh_tracer << trace_event("ofh_uplane_pool_access", pool_access_tp);

      if (OCUDU_UNLIKELY(!scoped_buffer)) {
        logger.warning("Sector#{}: not enough space in the buffer pool to create a User-Plane message for slot "
                       "'{}' and eAxC '{}', symbol_id '{}'",
                       sector_id,
                       context.slot,
                       context.eaxc,
                       symbol_id);
        return;
      }
      span<uint8_t> data = scoped_buffer->get_buffer();

      is_last_fragment = prb_fragment_calculator.calculate_fragment_size(
          fragment_start_prb, fragment_nof_prbs, data.size() - headers_size.value());

      // Skip frame buffers so small that cannot carry one PRB.
      if (OCUDU_UNLIKELY(fragment_nof_prbs == 0)) {
        logger.warning("Sector#{}: skipped frame buffer as it cannot store data for a single PRB, required buffer size "
                       "is '{}' bytes",
                       sector_id,
                       data.size());

        continue;
      }

      ofh_tracer << instant_trace_event{"ofh_uplane_symbol", instant_trace_event::cpu_scope::thread};

      uplane_message_params up_params = generate_ofh_user_parameters(direction,
                                                                     filter_index_type::standard_channel_filter,
                                                                     context.slot,
                                                                     symbol_id,
                                                                     fragment_start_prb,
                                                                     fragment_nof_prbs,
                                                                     compr_params,
                                                                     context.section_id);

      unsigned used_size = enqueue_section_type_1_message_symbol(
          iq_data.subspan(fragment_start_prb * NOF_SUBCARRIERS_PER_RB, fragment_nof_prbs * NOF_SUBCARRIERS_PER_RB),
          up_params,
          context.eaxc,
          data);
      scoped_buffer->set_size(used_size);
    } while (!is_last_fragment);
  }
}

unsigned data_flow_uplane_data_impl::enqueue_section_type_1_message_symbol(span<const cbf16_t>          iq_symbol_data,
                                                                           const uplane_message_params& params,
                                                                           unsigned                     eaxc,
                                                                           span<uint8_t>                buffer)
{
  // Build the Open Fronthaul data message. Only one port supported.
  units::bytes  ether_header_size = eth_builder->get_header_size();
  units::bytes  ecpri_hdr_size    = ecpri_builder->get_header_size(ecpri::message_type::iq_data);
  units::bytes  offset            = ether_header_size + ecpri_hdr_size;
  span<uint8_t> ofh_buffer        = span<uint8_t>(buffer).last(buffer.size() - offset.value());
  unsigned      bytes_written     = up_builder->build_message(ofh_buffer, iq_symbol_data, params);

  // Add eCPRI header. Create a subspan with the payload that skips the Ethernet header.
  span<uint8_t> ecpri_buffer =
      span<uint8_t>(buffer).subspan(ether_header_size.value(), ecpri_hdr_size.value() + bytes_written);
  ecpri_builder->build_data_packet(ecpri_buffer, generate_ecpri_data_parameters(up_seq_gen.generate(eaxc), eaxc));

  // Update the number of bytes written.
  bytes_written += ecpri_hdr_size.value();

  // Add Ethernet header.
  span<uint8_t> eth_buffer = span<uint8_t>(buffer).first(ether_header_size.value() + bytes_written);
  eth_builder->build_frame(eth_buffer);

  if (OCUDU_UNLIKELY(logger.debug.enabled())) {
    logger.debug("Sector#{}: packing a User-Plane message for slot '{}' and eAxC '{}', symbol_id '{}', PRB "
                 "range '{}:{}', size '{}' bytes",
                 sector_id,
                 params.slot,
                 eaxc,
                 params.symbol_id,
                 params.start_prb,
                 params.nof_prb,
                 eth_buffer.size());
  }

  return eth_buffer.size();
}

data_flow_message_encoding_metrics_collector* data_flow_uplane_data_impl::get_metrics_collector()
{
  return nullptr;
}
