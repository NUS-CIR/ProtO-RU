#include "data_flow_uplane_uplink.h"
#include "ofh_uplane_fragment_size_calculator.h"
#include "srsran/phy/support/resource_grid_context.h"
#include "srsran/phy/support/resource_grid_reader.h"
#include "srsran/phy/support/shared_resource_grid.h"
#include "srsran/ofh/compression/compression_properties.h"
#include "srsran/ran/resource_block.h"
#include "srsran/srsvec/conversion.h"
#include "helpers.h"
#include <thread>

using namespace srsran;
using namespace ofh;

/// Generates and returns uplink Open Fronthaul user parameters for the given context.
static uplane_message_params generate_ul_ofh_user_parameters(slot_point                   slot,
                                                             unsigned                     symbol_id,
                                                             unsigned                     start_prb,
                                                             unsigned                     nof_prb,
                                                             const ru_compression_params& comp,
                                                             uint16_t                     section_id,
                                                             bool                         is_prach = false)
{
  uplane_message_params params;
  params.direction                     = data_direction::uplink;
  params.slot                          = slot;
  if(!is_prach) params.filter_index                  = srsran::ofh::filter_index_type::standard_channel_filter;
  else params.filter_index = srsran::ofh::filter_index_type::ul_prach_preamble_short;
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

data_flow_uplane_uplink_data_impl::data_flow_uplane_uplink_data_impl(
    const data_flow_uplane_uplink_data_impl_config&  config,
    data_flow_uplane_uplink_data_impl_dependencies&& dependencies) :
  logger(*dependencies.logger),
  nof_symbols_per_slot(get_nsymb_per_slot(config.cp)),
  ru_nof_prbs(config.ru_nof_prbs),
  sector_id(config.sector),
  compr_params(config.compr_params),
  frame_pool(std::move(dependencies.frame_pool)),
  compressor_sel(std::move(dependencies.compressor_sel)),
  eth_builder(std::move(dependencies.eth_builder)),
  ecpri_builder(std::move(dependencies.ecpri_builder)),
  up_builder(std::move(dependencies.up_builder)),
  formatted_trace_names(config.ul_eaxc)
{
  srsran_assert(eth_builder, "Invalid Ethernet VLAN packet builder");
  srsran_assert(ecpri_builder, "Invalid eCPRI packet builder");
  srsran_assert(compressor_sel, "Invalid compressor selector");
  srsran_assert(up_builder, "Invalid User-Plane message builder");
  srsran_assert(frame_pool, "Invalid frame pool");
}

void data_flow_uplane_uplink_data_impl::enqueue_section_type_1_message(
    const data_flow_uplane_rg_context& context,
    const shared_resource_grid&        grid,
    const unsigned                     symbol_id)
{
  trace_point tp = ofh_tracer.now();
  enqueue_section_type_1_message_symbol_burst(context, grid, symbol_id);
  
  ofh_tracer << trace_event(formatted_trace_names[context.eaxc].c_str(), tp);
}

void data_flow_uplane_uplink_data_impl::enqueue_prach_message(
    const data_flow_uplane_prach_context& context,
    const prach_buffer&         buffer)
{
  trace_point tp = ofh_tracer.now();
  enqueue_prach_message_symbol_burst(context, buffer);
  ofh_tracer << trace_event(formatted_trace_names[context.eaxc].c_str(), tp);
}

void data_flow_uplane_uplink_data_impl::enqueue_section_type_1_message_symbol_burst(
    const data_flow_uplane_rg_context& context,
    const shared_resource_grid&        grid,
    const unsigned                     symbol_id)
{
  const resource_grid_reader& reader = grid.get_reader();

  // Temporary buffer used to store IQ data when the RU operating bandwidth is not the same to the cell bandwidth.
  std::array<cbf16_t, MAX_NOF_PRBS * NOF_SUBCARRIERS_PER_RB> temp_buffer;
  if (SRSRAN_UNLIKELY(ru_nof_prbs * NOF_SUBCARRIERS_PER_RB != reader.get_nof_subc())) {
    // Zero out the elements that won't be filled after reading the resource grid.
    std::fill(temp_buffer.begin() + reader.get_nof_subc(), temp_buffer.end(), 0);
  }

  units::bytes headers_size = eth_builder->get_header_size() +
                              ecpri_builder->get_header_size(ecpri::message_type::iq_data) +
                              up_builder->get_header_size(compr_params);
  
  // for(unsigned symbol_id = context.symbol_range.start(), e = context.symbol_range.stop(); 
  //     symbol_id < e;
  //     symbol_id++ ){
    trace_point         pool_access_tp = ofh_tracer.now();
    slot_symbol_point   symbol_point(context.slot, symbol_id, nof_symbols_per_slot);
    scoped_frame_buffer scoped_buffer(*frame_pool, symbol_point, message_type::user_plane, data_direction::uplink);
    if (scoped_buffer.empty()) {
      logger.warning("Sector#{}: not enough space in the buffer pool to create an uplink User-Plane message for slot "
                      "'{}' and eAxC '{}', symbol_id '{}'",
                      sector_id,
                      context.slot,
                      context.eaxc,
                      symbol_id);
      return;
    }

    ofh_tracer << trace_event("ofh_uplane_pool_access", pool_access_tp);

    span<const cbf16_t> iq_data;
    if (SRSRAN_LIKELY(ru_nof_prbs * NOF_SUBCARRIERS_PER_RB == reader.get_nof_subc())) {
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
      ether::frame_buffer& frame_buffer = scoped_buffer.get_next_frame();
      span<uint8_t>        data         = frame_buffer.data();

      is_last_fragment = prb_fragment_calculator.calculate_fragment_size(
          fragment_start_prb, fragment_nof_prbs, data.size() - headers_size.value());

      // Skip frame buffers so small that cannot carry one PRB.
      if (fragment_nof_prbs == 0) {
        logger.warning("Sector#{}: skipped frame buffer as it cannot store data for a single PRB, required buffer size "
                        "is '{}' bytes",
                        sector_id,
                        data.size());

        continue;
      }

      ofh_tracer << instant_trace_event{"ofh_uplane_symbol", instant_trace_event::cpu_scope::thread};

      const uplane_message_params& up_params =
          generate_ul_ofh_user_parameters(context.slot, symbol_id, fragment_start_prb, fragment_nof_prbs, 
                                          compr_params, context.section_id);

      unsigned used_size = enqueue_section_type_1_message_symbol(
          iq_data.subspan(fragment_start_prb * NOF_SUBCARRIERS_PER_RB, fragment_nof_prbs * NOF_SUBCARRIERS_PER_RB),
          up_params,
          context.eaxc,
          data);
      frame_buffer.set_size(used_size);
      logger.debug("Done packing an uplink User-Plane message for slot {} , symbol_id {},  used size = {}\n",context.slot, symbol_id, used_size);
    } while (!is_last_fragment);
}

void  data_flow_uplane_uplink_data_impl::
      enqueue_prach_message_symbol_burst(const data_flow_uplane_prach_context& context,
                                         const prach_buffer&                   buffer)
{
  unsigned start_symbol = context.start_symbol;
  unsigned nof_symbols  = context.nof_symbols;
  unsigned symbol_count = 0;
  for(; symbol_count < nof_symbols; ++symbol_count) {
    unsigned symbol_id = start_symbol + symbol_count;

    trace_point  pool_access_tp = ofh_tracer.now();
    span<const cbf16_t> iq_data = buffer.get_symbol(0, 0, 0, symbol_id);
    /// Pad the IQ data size to be multiple of 'prb_size' bytes for compression.
    std::array<cbf16_t,  12 * NOF_SUBCARRIERS_PER_RB> temp_buffer;
    temp_buffer.fill(cbf16_t(0,0));
    unsigned i = 0;
    for(;i<iq_data.size(); i++){
      temp_buffer[i+2] = iq_data[i];
    }
    span<const cbf16_t> iq_data_padded(temp_buffer.data(), temp_buffer.size());
    
    slot_symbol_point   symbol_point(context.slot, symbol_id, nof_symbols_per_slot);
    scoped_frame_buffer scoped_buffer(*frame_pool, symbol_point, message_type::uplane_prach, data_direction::uplink);
    if (scoped_buffer.empty()) {
      logger.warning("Sector#{}: not enough space in the buffer pool to create a PRACH message for slot '{}' and eAxC "
                     "'{}', symbol_id '{}'",
                     context.sector,
                     context.slot,
                     context.eaxc, 
                     symbol_id);
      return;
    }
    ofh_tracer << trace_event("ofh_uplane_prach_pool_access", pool_access_tp);
    ether::frame_buffer& frame_buffer = scoped_buffer.get_next_frame();
    span<uint8_t>        data         = frame_buffer.data();
    const uplane_message_params& up_params =
          generate_ul_ofh_user_parameters(context.slot, symbol_id, context.prb_start, 
                                          context.nof_prb, compr_params, context.section_id, true);

    unsigned used_size = enqueue_section_type_1_message_symbol(
        iq_data_padded,
        up_params,
        context.eaxc,
        data);
    frame_buffer.set_size(used_size);
  }
}

unsigned data_flow_uplane_uplink_data_impl::enqueue_section_type_1_message_symbol(span<const cbf16_t> iq_symbol_data,
                                                                                    const uplane_message_params& params,
                                                                                    unsigned                     eaxc,
                                                                                    span<uint8_t>                buffer)
{
  // if(params.slot.sfn()==21 && params.slot.slot_index()==19 && params.symbol_id==1 && 
  //   params.filter_index==srsran::ofh::filter_index_type::standard_channel_filter){
  //   print_iq_samples(iq_symbol_data, file_count++);
  // }
  // Build the Open Fronthaul data message. Only one port supported.
  units::bytes  ether_header_size = eth_builder->get_header_size();
  units::bytes  ecpri_hdr_size    = ecpri_builder->get_header_size(ecpri::message_type::iq_data);
  units::bytes  offset            = ether_header_size + ecpri_hdr_size;
  span<uint8_t> ofh_buffer        = span<uint8_t>(buffer).last(buffer.size() - offset.value());
  unsigned      bytes_written     = up_builder->build_message(ofh_buffer, iq_symbol_data, params);
  // if(params.slot.sfn()==21 && params.slot.slot_index()==19 && params.symbol_id==1 && 
  //   params.filter_index==srsran::ofh::filter_index_type::ul_prach_preamble_short){
  //   dump_hex(ofh_buffer);
  // }

  // Add eCPRI header. Create a subspan with the payload that skips the Ethernet header.
  span<uint8_t> ecpri_buffer =
      span<uint8_t>(buffer).subspan(ether_header_size.value(), ecpri_hdr_size.value() + bytes_written);
  ecpri_builder->build_data_packet(ecpri_buffer, generate_ecpri_data_parameters(up_seq_gen.generate(eaxc), eaxc));

  // Update the number of bytes written.
  bytes_written += ecpri_hdr_size.value();

  // Add Ethernet header.
  span<uint8_t> eth_buffer = span<uint8_t>(buffer).first(ether_header_size.value() + bytes_written);
  eth_builder->build_frame(eth_buffer);

  logger.debug("Sector#{}: packing a uplink User-Plane message for slot '{}' and eAxC '{}', symbol_id '{}', PRB "
               "range '{}:{}', size '{}' bytes",
               sector_id,
               params.slot,
               eaxc,
               params.symbol_id,
               params.start_prb,
               params.nof_prb,
               eth_buffer.size());

  return eth_buffer.size();
}