
#pragma once

#include "context_repository_helpers.h"
#include "srsran/adt/expected.h"
#include "srsran/ofh/ofh_constants.h"
#include "srsran/phy/support/resource_grid.h"
#include "srsran/phy/support/resource_grid_context.h"
#include "srsran/phy/support/resource_grid_reader.h"
#include "srsran/phy/support/resource_grid_writer.h"
#include "srsran/phy/support/shared_resource_grid.h"
#include "srsran/ran/cyclic_prefix.h"
#include "srsran/ran/resource_allocation/ofdm_symbol_range.h"
#include "srsran/ran/resource_block.h"
#include "srsran/srsvec/copy.h"
#include "srsran/srslog/srslog.h"

#include <mutex>

namespace srsran {
namespace ofh {

inline srslog::basic_logger& get_logger(){
  static srslog::basic_logger& logger = srslog::fetch_basic_logger("DL repo");
  return logger;
}

/// downlink context.
class downlink_context
{
public:
  /// Information related to the resource grid stored in the downlink context.
  struct downlink_context_resource_grid_info {
    resource_grid_context context;
    shared_resource_grid  grid;
  };

  /// Default constructor.
  downlink_context() = default;

  downlink_context copy() const
  {
    downlink_context context;
    context.symbol       = symbol;
    context.grid.context = grid.context;
    context.grid.grid    = grid.grid.copy();
    context.re_written   = re_written;
    return context;
  }

  /// Constructs an downlink slot context with the given resource grid and resource grid context.
  downlink_context(unsigned symbol_, const resource_grid_context& context_, const shared_resource_grid& grid_) :
    symbol(symbol_), grid({context_, grid_.copy()})
  {
    const resource_grid_reader& reader = grid.grid->get_reader();

    re_written = static_vector<bounded_bitset<MAX_NOF_PRBS * NRE>, MAX_NOF_SUPPORTED_EAXC>(
        reader.get_nof_ports(), bounded_bitset<MAX_NOF_PRBS * NRE>(size_t(reader.get_nof_subc())));
  }

  /// Returns true if this context is empty, otherwise false.
  bool empty() const { return !grid.grid.is_valid(); }

  /// Returns the number of PRBs of the context grid or zero if no grid was configured for this context.
  unsigned get_grid_nof_prbs() const
  {
    return (grid.grid) ? (grid.grid.get_reader().get_nof_subc() / NOF_SUBCARRIERS_PER_RB) : 0U;
  }

  /// Returns the resource grid context.
  const resource_grid_context& get_grid_context() const { return grid.context; }

  /// Returns a span of bitmaps that indicate the REs that have been written for the given symbol. Each element of the
  /// span corresponds to a port.
  span<const bounded_bitset<MAX_NOF_PRBS * NRE>> get_re_written_mask() const { return re_written; }

  /// Writes the given RE IQ buffer into the port and start RE.
  void write_grid(unsigned port, unsigned start_re, span<const cbf16_t> re_iq_buffer)
  {
    // srsran_assert(grid.grid, "Invalid resource grid for slot {}, symbol {}, and pool is {}, ref is {}", grid.context.slot, symbol,
    //               grid.grid.get_pool()? 1:0, grid.grid.get_ref_count()? 1: 0);
    if(!grid.grid){
      get_logger().warning("No valid resource grid for slot {}, symbol {}, cannot write grid\n", grid.context.slot, symbol);
      return;
    }
    resource_grid_writer& writer = grid.grid->get_writer();

    // Skip writing if the given port does not fit in the grid.
    if (port >= writer.get_nof_ports()) {
      return;
    }
    span<cbf16_t> grid_view = grid.grid->get_writer().get_view(port, symbol).subspan(start_re, re_iq_buffer.size());
    srsvec::copy(grid_view, re_iq_buffer);
    re_written[port].fill(start_re, start_re + re_iq_buffer.size());
    writer.clear_empty(port);
  }

  /// Tries to get a complete resource grid. A resource grid is considered completed when all the PRBs for all the ports
  /// have been written.
  expected<downlink_context_resource_grid_info> try_getting_complete_resource_grid() const
  {
    if (!grid.grid) {
      return make_unexpected(default_error_t{});
    }

    if (!have_all_prbs_been_written()) {
      return make_unexpected(default_error_t{});
    }

    return downlink_context_resource_grid_info{grid.context, grid.grid.copy()};
  }

  /// Returns the context grid information.
  const downlink_context_resource_grid_info& get_downlink_context_resource_grid_info() const { return grid; }

  /// Gets the context grid information and clears it.
  downlink_context_resource_grid_info pop_downlink_context_resource_grid_info() { return std::move(grid); }

private:
  /// Returns true when all the REs for the current symbol have been written.
  bool have_all_prbs_been_written() const
  {
    return std::all_of(
        re_written.begin(), re_written.end(), [](const auto& port_re_written) { return port_re_written.all(); });
  }

private:
  unsigned                                                                  symbol;
  downlink_context_resource_grid_info                                         grid;
  static_vector<bounded_bitset<MAX_NOF_PRBS * NRE>, MAX_NOF_SUPPORTED_EAXC> re_written;
};

/// downlink context repository.
class downlink_context_repository
{
  std::vector<std::array<downlink_context, MAX_NSYMB_PER_SLOT>> buffer;
  //: TODO: make this lock free
  mutable std::mutex mutex;

  /// Returns the entry of the repository for the given slot and symbol.
  downlink_context& entry(slot_point slot, unsigned symbol)
  {
    srsran_assert(symbol < MAX_NSYMB_PER_SLOT, "Invalid symbol index '{}'", symbol);

    unsigned index = calculate_repository_index(slot, buffer.size());
    return buffer[index][symbol];
  }

  /// Returns the entry of the repository for the given slot and symbol.
  const downlink_context& entry(slot_point slot, unsigned symbol) const
  {
    srsran_assert(symbol < MAX_NSYMB_PER_SLOT, "Invalid symbol index '{}'", symbol);

    unsigned index = calculate_repository_index(slot, buffer.size());
    return buffer[index][symbol];
  }

public:
  explicit downlink_context_repository(unsigned size_) : buffer(size_) {}

  /// Adds the given entry to the repository at slot.
  void
  add(const resource_grid_context& context, const shared_resource_grid& grid, const ofdm_symbol_range& symbol_range)
  {
    std::lock_guard<std::mutex> lock(mutex);
    // get_logger().warning("Adding entry for slot {} symbol range [{}, {})\n", context.slot, symbol_range.start(), symbol_range.stop());
    srsran_assert(grid.is_valid(), "Invalid resource grid provided for slot {}", context.slot);
    for (unsigned symbol_id = symbol_range.start(), symbol_end = symbol_range.stop(); symbol_id != symbol_end;
         ++symbol_id) {
      entry(context.slot, symbol_id) = downlink_context(symbol_id, context, grid);
      // get_logger().warning("Entry for slot {} symbol {} added!\n", context.slot, symbol_id);
    }
  }

  /// Writes to the grid at the given slot, port, symbol and start resource element the given IQ buffer.
  void write_grid(slot_point slot, unsigned port, unsigned symbol, unsigned start_re, span<const cbf16_t> re_iq_buffer)
  {
    std::lock_guard<std::mutex> lock(mutex);
    entry(slot, symbol).write_grid(port, start_re, re_iq_buffer);
  }

  /// Returns the entry of the repository for the given slot and symbol.
  downlink_context get(slot_point slot, unsigned symbol) const
  {
    std::lock_guard<std::mutex> lock(mutex);
    return entry(slot, symbol).copy();
  }

  /// \brief Tries to pop a complete resource grid for the given slot and symbol.
  ///
  /// A resource grid is considered completed when all the PRBs for all the ports have been written.
  expected<downlink_context::downlink_context_resource_grid_info> try_popping_complete_resource_grid_symbol(slot_point slot,
                                                                                                        unsigned symbol)
  {
    std::lock_guard<std::mutex> lock(mutex);

    auto result = entry(slot, symbol).try_getting_complete_resource_grid();

    // Symbol is complete or exists. Clear the context.
    if (result.has_value()) {
      entry(slot, symbol) = {};
    }

    return result;
  }

  /// Pops a resource grid for the given slot and symbol.
  expected<downlink_context::downlink_context_resource_grid_info> pop_resource_grid_symbol(slot_point slot, unsigned symbol)
  {
    std::lock_guard<std::mutex> lock(mutex);

    auto& result = entry(slot, symbol);

    // Symbol does not exists. Do nothing.
    if (result.empty()) {
      return make_unexpected(default_error_t{});
    }

    // Pop and clear the slot/symbol information.
    downlink_context::downlink_context_resource_grid_info info = result.pop_downlink_context_resource_grid_info();
    return info;
  }

  /// Clears the repository entry for the given slot and symbol.
  void clear(slot_point slot, unsigned symbol)
  {
    std::lock_guard<std::mutex> lock(mutex);
    entry(slot, symbol) = {};
  }

  
  // Merges different entries into one rg for a given slot point.
  srsran::shared_resource_grid pop_and_merge_slot_resource_grid(slot_point slot, srsran::shared_resource_grid& dest_grid)
  {
      // To store every poped resource grid info(one per symbol).
      std::vector<downlink_context::downlink_context_resource_grid_info> pop_grids;

      {
          // Lock.
          std::lock_guard<std::mutex> lock(mutex);
          // Iterate all symbols in slot（assume the nof symbols is  MAX_NSYMB_PER_SLOT）
          for (unsigned symbol = 0; symbol < MAX_NSYMB_PER_SLOT; ++symbol) {
              downlink_context &ctx = entry(slot, symbol);
              if (!ctx.empty()) {
                  // Pop resource grid info of this symbol and move.
                  pop_grids.push_back(std::move(ctx.pop_downlink_context_resource_grid_info()));
              }
          }
      }

      // If no valid grid，return an empty shared_resource_grid
      if (pop_grids.empty()) {
          return {};
      }

      // Extract information from the first poped grid ：nof antenna ports and nof subcarriers
      const auto &base_grid = pop_grids.front().grid;
      const auto &base_reader = base_grid.get_reader();
      unsigned nof_ports = base_reader.get_nof_ports();
      //unsigned nof_subc = base_reader.get_nof_subc();
      // The nof merged OFDM symbols should match the nof poped grids.
      unsigned total_symbols = pop_grids.size();
      
      auto &dest_writer = dest_grid->get_writer();

      // Iterate every poped grid，copy its data to the symbol position of the corresponding dest_grid.
      for (unsigned idx = 0; idx < total_symbols; ++idx) {
          const auto &grid_info = pop_grids[idx];
          const auto &src_grid = grid_info.grid;
          const auto &src_reader = src_grid.get_reader();
          // get_logger().warning("reading grid for slot {}, symbol {}\n", grid_info.context.slot, idx);
          for (unsigned port = 0; port < nof_ports; ++port) {
              // Read the data from the source grid for the given port and symbol index.
              srsran::span<const srsran::cbf16_t> src_view = src_reader.get_view(port, idx);
              // In dest grid，the symbol index is 'idx'.
              srsran::span<srsran::cbf16_t> dest_view = dest_writer.get_view(port, idx);
              srsvec::copy(dest_view, src_view);
              dest_writer.clear_empty(port);
          }
      }
      return std::move(dest_grid);
  }
};

} // namespace ofh
} // namespace srsran
