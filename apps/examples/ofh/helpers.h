/*
 *
 * Copyright 2021-2025 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#pragma once

#include "srsran/adt/span.h"
#include "srsran/ofh/ethernet/ethernet_mac_address.h"
#include "srsran/ran/bs_channel_bandwidth.h"
#include "srsran/ru/ofh/ru_ofh_configuration.h"
#include <fstream>

namespace srsran {

// inline void print_iq_samples_hex(const cbf16_t* data, std::size_t count) {
//   std::string filepath = "/tmp/RU_iq_samples" + std::to_string(file_count++) + ".txt";
//   std::ofstream out(filepath, std::ios::out);

//   const unsigned char* p = reinterpret_cast<const unsigned char*>(data);
//   const std::size_t nbytes = count * sizeof(cbf16_t);

//   // 备份/设置格式
//   std::ios old_state(nullptr);
//   old_state.copyfmt(out);
//   out << std::hex << std::setfill('0') << std::uppercase;

//   for (std::size_t i = 0; i < nbytes; ++i) {
//       if (i % 16 == 0) {
//           if (i) out << '\n';
//           out << std::setw(8) << i << "  ";
//       }
//       out << std::setw(2) << static_cast<unsigned>(p[i]) << ' ';
//   }
//   if (nbytes) out << '\n';
//   out.copyfmt(old_state);
//   fmt::print("new packet\n");
//   return;
// }
// inline void print_iq_samples(span<const cbf16_t> iq_samples, unsigned file_count) {
//   std::string filepath = "/tmp/RU_samples" + std::to_string(file_count) + ".txt";
//   std::ofstream ofs(filepath, std::ios::out);

//   for (size_t i = 0; i < iq_samples.size(); ++i) {
//         const auto& sample = iq_samples[i];
        
//         float real_val = to_float(sample.real);
//         float imag_val = to_float(sample.imag);
//         ofs << fmt::format("Sample {} : real= {}, imag = {}\n", i, real_val, imag_val);
//   }

//   fmt::print("New packet\n");
// }

// static void dump_hex(span<uint8_t> packet)
// {
//   std::string tmp_file = "/tmp/RU_Prach" + std::to_string(file_count++) + ".txt";
//   std::ofstream ofs(tmp_file, std::ios::out);
//   for (size_t i = 0; i < packet.size(); ++i) {
//       ofs << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet[i]) << " ";
//       if ((i + 1) % 16 == 0) {
//           ofs << "\n";
//       }
//   }
//   fmt::print("new packet\n");
//   ofs << std::dec;
//   ofs.close();
// }

/// Compare two mac addresses.
inline bool compare_mac_addresses(const ether::mac_address& mac_src, const ether::mac_address& mac_dst){
  unsigned i = 0;
  bool flag = true;
  for(;i< ether::ETH_ADDR_LEN; i++){
    flag =  mac_src[i]==mac_dst[i];
  }
  return flag;
}

/// Parses the string containing Ethernet MAC address.
inline bool parse_mac_address(const std::string& mac_str, ether::mac_address& mac)
{
  std::array<unsigned, 6> data       = {};
  int                     bytes_read = std::sscanf(
      mac_str.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x", &data[0], &data[1], &data[2], &data[3], &data[4], &data[5]);
  if (bytes_read != ether::ETH_ADDR_LEN) {
    fmt::print("Invalid MAC address provided: {}\n", mac_str);
    return false;
  }

  std::copy(data.begin(), data.end(), mac.begin());

  return true;
}

/// Validates the bandwidth argument provided as a user input.
inline bool is_valid_bw(unsigned bandwidth)
{
  // Bandwidth cannot be less than 5MHz.
  if (bandwidth < 5U) {
    return false;
  }

  // Check from [5-25] in steps of 5.
  if (bandwidth < 26U) {
    return ((bandwidth % 5) == 0);
  }

  // Check from [30-100] in steps of 10.
  if (bandwidth < 101U) {
    return ((bandwidth % 10) == 0);
  }

  return false;
}

namespace ru_emu_stats {

/// Helper class that represents a KPI counter.
class kpi_counter
{
  std::atomic<uint64_t> counter{0};
  uint64_t              last_value_printed = 0U;

public:
  /// Returns value accumulated since last call.
  uint64_t calculate_acc_value()
  {
    uint64_t current_value = counter.load(std::memory_order_relaxed);
    uint64_t total         = current_value - last_value_printed;
    last_value_printed     = current_value;
    return total;
  }

  /// Increments value by the given amount.
  void increment(unsigned n = 1) { counter.fetch_add(n, std::memory_order_relaxed); }
};

} // namespace ru_emu_stats
} // namespace srsran
