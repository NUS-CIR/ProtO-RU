#pragma once

#include "srsran/ran/resource_allocation/ofdm_symbol_range.h"
#include "srsran/ran/slot_point.h"
#include "srsran/instrumentation/traces/ofh_traces.h"
#include "srsran/ofh/compression/iq_compressor.h"
#include "srsran/ofh/ecpri/ecpri_packet_builder.h"
#include "srsran/ofh/ethernet/ethernet_frame_builder.h"
#include "srsran/ofh/serdes/ofh_uplane_message_builder.h"
#include "srsran/ofh/ethernet/ethernet_frame_pool.h"
#include "srsran/ran/cyclic_prefix.h"
#include "srsran/srslog/srslog.h"
#include "srsran/ofh/ofh_constants.h"
#include "srsran/support/srsran_assert.h"
#include "srsran/support/error_handling.h"
#include "srsran/phy/support/prach_buffer_context.h"
#include "srsran/phy/support/prach_buffer.h"
#include <array>
#include <atomic>

namespace srsran {
struct resource_grid_context;
class shared_resource_grid;

namespace ether {
class eth_frame_pool;
}

namespace ofh {

/// Scoped helper class that retrieves an Ethernet frame buffer from a buffer pool and marks it as ready to send upon
/// destruction.
class scoped_frame_buffer
{
  ether::eth_frame_pool&          frame_pool;
  const ether::frame_pool_context context;
  const span<ether::frame_buffer> frames;

public:
  /// On construction, acquire Ethernet frame buffers for the given slot, symbol and Open Fronthaul type.
  scoped_frame_buffer(ether::eth_frame_pool& frame_pool_,
                      slot_symbol_point      symbol_point,
                      message_type           type,
                      data_direction         direction) :
    frame_pool(frame_pool_), context({{type, direction}, symbol_point}), frames(frame_pool.get_frame_buffers(context))
  {
  }

  /// Returns the frame retrieved from the pool.
  ether::frame_buffer& get_next_frame()
  {
    for (auto& frame : frames) {
      if (frame.empty()) {
        return frame;
      }
    }

    srsran_terminate("No empty Ethernet frame available in slot '{}' symbol '{}'\n",
                     context.symbol_point.get_slot(),
                     context.symbol_point.get_symbol_index());
  }

  bool empty() const { return frames.empty(); }

  /// Destructor marks the acquired buffers as ready to be sent.
  ~scoped_frame_buffer()
  {
    if (!frames.empty()) {
      frame_pool.push_frame_buffers(context, frames);
    }
  }
};

/// Sequence identifier generator.
class sequence_identifier_generator
{
  std::array<std::atomic<uint8_t>, MAX_SUPPORTED_EAXC_ID_VALUE> counters;

public:
  /// Default constructor.
  explicit sequence_identifier_generator(unsigned init_value = 0)
  {
    for (unsigned K = 0; K != MAX_SUPPORTED_EAXC_ID_VALUE; ++K) {
      counters[K] = init_value;
    }
  }

  /// Generates a new sequence identifier and returns it.
  uint8_t generate(unsigned eaxc)
  {
    srsran_assert(eaxc < MAX_SUPPORTED_EAXC_ID_VALUE,
                  "Invalid eAxC value '{}'. Maximum eAxC value is '{}'",
                  eaxc,
                  MAX_SUPPORTED_EAXC_ID_VALUE);

    auto& value = counters[eaxc];
    return value++;
  }
};

/// Open Fronthaul User-Plane uplink data flow resource grid context.
struct data_flow_uplane_rg_context {
  /// Provides the slot context within the system frame.
  slot_point slot;
  /// Provides the sector identifier.
  unsigned sector;
  /// Provides the port identifier.
  unsigned port;
  /// eAxC.
  unsigned eaxc;
  /// Symbol id.
  unsigned symbol_id;
  /// Section ID.
  uint16_t section_id;
  /// Symbol range in the slot.
  ofdm_symbol_range symbol_range;
};

/// Open Fronthaul User-Plane uplink data flow PRACH context.
struct data_flow_uplane_prach_context {
  /// Provides the slot context within the system frame.
  slot_point slot;
  /// Provides the sector identifier.
  unsigned sector;
  /// Provides the port identifier.
  unsigned port;
  /// eAxC.
  unsigned eaxc;
  /// Section ID.
  uint16_t section_id;
  /// Starting PRB of data section.
  uint16_t prb_start;
  /// Number of contiguous PRBs per data section.
  unsigned nof_prb;
  /// Number of symbols.
  uint8_t nof_symbols;
  /// Starting symbol of the PRACH occasion within the slot.
  uint8_t start_symbol;
};

/// Open Fronthaul User-Plane uplink data flow.
class data_flow_uplane_uplink_data
{
public:
  /// Default destructor.
  virtual ~data_flow_uplane_uplink_data() = default;

  /// Enqueues the User-Plane uplink data messages with the given context and resource grid.
  virtual void enqueue_section_type_1_message(const data_flow_uplane_rg_context& context,
                                              const shared_resource_grid&        grid,
                                              const unsigned                     symbol_id) = 0;

  /// Enqueues the User-Plane uplink PRACH data messages with the given context and PRACH buffer.
  virtual void enqueue_prach_message(const data_flow_uplane_prach_context& context,
                                     const prach_buffer&        buffer) = 0;
};

/// Open Fronthaul User-Plane uplink data flow implementation configuration.
struct data_flow_uplane_uplink_data_impl_config {
  /// Radio sector identifier.
  unsigned sector;
  /// Cyclic prefix.
  cyclic_prefix cp;
  /// RU bandwidth in PRBs.
  unsigned ru_nof_prbs;
  /// uplink eAxCs.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  /// Compression parameters.
  ru_compression_params compr_params;
};

/// Open Fronthaul User-Plane uplink data flow implementation dependencies.
struct data_flow_uplane_uplink_data_impl_dependencies {
  /// Logger
  srslog::basic_logger* logger = nullptr;
  /// Ethernet frame pool.
  std::shared_ptr<ether::eth_frame_pool> frame_pool;
  /// VLAN frame builder.
  std::unique_ptr<ether::frame_builder> eth_builder;
  /// eCPRI packet builder.
  std::unique_ptr<ecpri::packet_builder> ecpri_builder;
  /// IQ compressor.
  std::unique_ptr<iq_compressor> compressor_sel;
  /// User-Plane message builder.
  std::unique_ptr<uplane_message_builder> up_builder;
};

/// Stores trace names used by the \c data_flow_uplane_uplink_data_impl class when OFH tracing is enabled.
template <bool Enabled = true>
class ofh_uplane_trace_names
{
  std::array<std::string, MAX_SUPPORTED_EAXC_ID_VALUE> trace_names;

public:
  explicit ofh_uplane_trace_names(span<const unsigned> dl_eaxc)
  {
    for (unsigned eaxc : dl_eaxc) {
      trace_names[eaxc] = fmt::format("ofh_uplane_eaxc_{}", eaxc);
    }
  }

  const std::string& operator[](std::size_t eaxc) const { return trace_names[eaxc]; }
};

/// Specialization of ofh_uplane_trace_names used when OFH event tracing is disabled.
template <>
class ofh_uplane_trace_names<false>
{
public:
  explicit ofh_uplane_trace_names(span<const unsigned> dl_eaxc) {}

  const std::string operator[](std::size_t eaxc) const { return ""; }
};

/// Open Fronthaul User-Plane uplink data flow implementation.
class data_flow_uplane_uplink_data_impl : public data_flow_uplane_uplink_data
{
public:
  explicit data_flow_uplane_uplink_data_impl(const data_flow_uplane_uplink_data_impl_config&  config,
                                               data_flow_uplane_uplink_data_impl_dependencies&& dependencies);

  // See interface for documentation.
  void enqueue_section_type_1_message(const data_flow_uplane_rg_context& context,
                                      const shared_resource_grid&        grid,
                                      const unsigned                     symbol_id) override;
  // See interface for documentation.
  void enqueue_prach_message(const data_flow_uplane_prach_context& context,
                             const prach_buffer&         buffer) override;

private:
  /// Enqueues an User-Plane message burst.
  void enqueue_section_type_1_message_symbol_burst(const data_flow_uplane_rg_context& context,
                                                   const shared_resource_grid&        grid,
                                                   const unsigned                     symbol_id);
  
  /// Enqueues an User-Plane PRACH message burst.
  void enqueue_prach_message_symbol_burst(const data_flow_uplane_prach_context& context,
                                          const prach_buffer&         buffer);

  /// Enqueues an User-Plane message symbol with the given context and grid.
  unsigned enqueue_section_type_1_message_symbol(span<const cbf16_t>          iq_symbol_data,
                                                 const uplane_message_params& params,
                                                 unsigned                     eaxc,
                                                 span<uint8_t>                buffer);

private:
  srslog::basic_logger&                     logger;
  const unsigned                            nof_symbols_per_slot;
  const unsigned                            ru_nof_prbs;
  const unsigned                            sector_id;
  const ru_compression_params               compr_params;
  sequence_identifier_generator             up_seq_gen;
  std::shared_ptr<ether::eth_frame_pool>    frame_pool;
  std::unique_ptr<iq_compressor>            compressor_sel;
  std::unique_ptr<ether::frame_builder>     eth_builder;
  std::unique_ptr<ecpri::packet_builder>    ecpri_builder;
  std::unique_ptr<uplane_message_builder>   up_builder;
  ofh_uplane_trace_names<OFH_TRACE_ENABLED> formatted_trace_names;
  unsigned                                  file_count = 0;
};


} // namespace ofh
} // namespace srsran
