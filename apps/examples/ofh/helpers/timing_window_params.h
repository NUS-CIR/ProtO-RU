
#pragma once

#include "srsran/ran/cyclic_prefix.h"
#include "fmt/chrono.h"

namespace srsran{
namespace ofh{

constexpr size_t MAX_NOF_SYMBOLS = get_nsymb_per_slot(cyclic_prefix::NORMAL);

/// Structure storing the reception window timing parameters expressed in a number of symbols.
struct ru_window_timing_parameters {
  /// Offset from the current OTA symbol to the start of DL Control-Plane reception window. Must be calculated based on
  /// \c T2a_max_cp_dl parameter.
  unsigned sym_cp_dl_start;
  /// Offset from the current OTA symbol to end of DL Control-Plane message reception window. Must be calculated based
  /// on \c T2a_min_cp_dl parameter.
  unsigned sym_cp_dl_end;
  /// Offset from the current OTA symbol to the start of UL Control-Plane reception window. Must be calculated based on
  /// \c T2a_max_cp_ul parameter.
  unsigned sym_cp_ul_start;
  /// Offset from the current OTA symbol to the end of UL Control-Plane reception window. Must be calculated based on \c
  /// \c T2a_min_cp_ul parameter.
  unsigned sym_cp_ul_end;
  /// Offset from the current OTA symbol to the start of DL User-Plane reception window. Must be calculated based on \c
  /// \c T2a_max_up parameter.
  unsigned sym_up_dl_start;
  /// Offset from the current OTA symbol to the start of DL User-Plane reception window. Must be calculated based on \c
  /// \c T2a_min_up parameter.
  unsigned sym_up_dl_end;
  /// Offset from the current OTA symbol to the start of UL User-Plane transmission window. Must be calculated based  
  /// on \c Ta3_min_up parameter.
  unsigned sym_up_ul_start;
  /// Offset from the current OTA symbol to the end of the UL User-Plane transmission window. Must be calculated based
  /// on \c Ta3_max_up parameter.
  unsigned sym_up_ul_end;
};

/// Converts timing parameters expressed in microseconds into the ones expressed in number of OFDM symbols.
inline ru_window_timing_parameters rx_timing_window_params_us_to_symbols(std::chrono::microseconds T2a_max_cp_dl,
                                                                         std::chrono::microseconds T2a_min_cp_dl,
                                                                         std::chrono::microseconds T2a_max_cp_ul,
                                                                         std::chrono::microseconds T2a_min_cp_ul,
                                                                         std::chrono::microseconds T2a_max_up,
                                                                         std::chrono::microseconds T2a_min_up,
                                                                         std::chrono::microseconds Ta3_max_up,
                                                                         std::chrono::microseconds Ta3_min_up,
                                                                         subcarrier_spacing scs)
{
  std::chrono::duration<double, std::nano> symbol_duration(
      (1e6 / (MAX_NOF_SYMBOLS * get_nof_slots_per_subframe(scs))));

  ru_window_timing_parameters rx_window_timing_params;
  rx_window_timing_params.sym_cp_dl_start = std::floor(T2a_max_cp_dl / symbol_duration);
  rx_window_timing_params.sym_cp_dl_end   = std::ceil(T2a_min_cp_dl / symbol_duration);
  rx_window_timing_params.sym_cp_ul_start = std::floor(T2a_max_cp_ul / symbol_duration);
  rx_window_timing_params.sym_cp_ul_end   = std::ceil(T2a_min_cp_ul / symbol_duration);
  rx_window_timing_params.sym_up_dl_start = std::floor(T2a_max_up / symbol_duration);
  rx_window_timing_params.sym_up_dl_end   = std::ceil(T2a_min_up / symbol_duration);
  rx_window_timing_params.sym_up_ul_start = std::floor(Ta3_min_up / symbol_duration);
  rx_window_timing_params.sym_up_ul_end = std::floor(Ta3_max_up / symbol_duration);

  return rx_window_timing_params;
}

/// Converts timing parameter expressed in microseconds into one expressed in number of slots.
// inline unsigned rx_timing_window_params_us_to_slots(std::chrono::microseconds T_min, std::chrono::microseconds T_max)
// {
//   std::chrono::duration<double, std::nano> slot_duration(1e6 / get_nof_slots_per_subframe(subcarrier_spacing::kHz30));
//   unsigned n_slot = std::max(std::floor(),std::ceil());
// }


} // namespace ofh
} // namespace srsran

