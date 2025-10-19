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

#include "lower_phy_baseband_processor.h"
#include "srsran/adt/gps_clock.h"
#include "srsran/adt/interval.h"
#include "srsran/instrumentation/traces/ru_traces.h"
#include <uhd/types/time_spec.hpp>

using namespace srsran;

lower_phy_baseband_processor::lower_phy_baseband_processor(const lower_phy_baseband_processor::configuration& config) :
  scs(config.scs),
  srate(config.srate),
  nof_samples_per_subframe(srate.to_kHz()),
  nof_slots_per_subframe(get_nof_slots_per_subframe(config.scs)),
  nof_symbols_per_slot(get_nsymb_per_slot(config.cp)),
  nof_symbols_per_sec(nof_symbols_per_slot * get_nof_slots_per_subframe(scs) * NOF_SUBFRAMES_PER_FRAME * 100),
  symbol_duration(1e9 / nof_symbols_per_sec),
  tx_buffer_size(config.tx_buffer_size),
  rx_buffer_size(config.rx_buffer_size),
  cpu_throttling_time((config.tx_buffer_size * static_cast<uint64_t>(config.system_time_throttling * 1e6)) /
                      config.srate.to_kHz()),
  logger(*config.logger),
  rx_executor(*config.rx_task_executor),
  tx_executor(*config.tx_task_executor),
  uplink_executor(*config.ul_task_executor),
  downlink_executor(*config.dl_task_executor),
  receiver(*config.receiver),
  transmitter(*config.transmitter),
  uplink_processor(*config.ul_bb_proc),
  downlink_processor(*config.dl_bb_proc),
  rx_buffers(config.nof_rx_buffers),
  tx_buffers(config.nof_tx_buffers),
  tx_time_offset(config.tx_time_offset),
  rx_to_tx_max_delay(config.rx_to_tx_max_delay)
{
  static constexpr interval<float> system_time_throttling_range(0, 1);

  srsran_assert(tx_buffer_size, "Invalid buffer size.");
  srsran_assert(rx_buffer_size, "Invalid buffer size.");
  srsran_assert(config.rx_task_executor, "Invalid receive task executor.");
  srsran_assert(system_time_throttling_range.contains(config.system_time_throttling),
                "System time throttling (i.e., {}) is out of the range {}.",
                config.system_time_throttling,
                system_time_throttling_range);
  srsran_assert(config.tx_task_executor, "Invalid transmit task executor.");
  srsran_assert(config.ul_task_executor, "Invalid uplink task executor.");
  srsran_assert(config.dl_task_executor, "Invalid downlink task executor.");
  srsran_assert(config.receiver, "Invalid baseband receiver.");
  srsran_assert(config.transmitter, "Invalid baseband transmitter.");
  srsran_assert(config.ul_bb_proc, "Invalid uplink processor.");
  srsran_assert(config.dl_bb_proc, "Invalid downlink processor.");
  srsran_assert(config.nof_rx_ports != 0, "Invalid number of receive ports.");
  srsran_assert(config.nof_tx_ports != 0, "Invalid number of transmit ports.");

  // Create queue of receive buffers.
  while (!rx_buffers.full()) {
    rx_buffers.push_blocking(std::make_unique<baseband_gateway_buffer_dynamic>(config.nof_rx_ports, rx_buffer_size));
  }

  // Create queue of transmit buffers.
  while (!tx_buffers.full()) {
    tx_buffers.push_blocking(std::make_unique<baseband_gateway_buffer_dynamic>(config.nof_tx_ports, tx_buffer_size));
  }

  unsigned symbol_size_no_cp        = srate.get_dft_size(config.scs);
  unsigned nof_symbols_per_subframe = nof_symbols_per_slot * nof_slots_per_subframe;

  // Setup symbol sizes.
  symbol_sizes.reserve(nof_symbols_per_subframe);
  for (unsigned i_symbol = 0; i_symbol != nof_symbols_per_subframe; ++i_symbol) {
    unsigned cp_size = config.cp.get_length(i_symbol, config.scs).to_samples(srate.to_Hz());
    symbol_sizes.emplace_back(cp_size + symbol_size_no_cp);
  }
}

uint32_t calculate_slot_diff(slot_point src, slot_point dst)
{
  int diff = dst.system_slot() - src.system_slot();
  int dis = diff >= 0? diff : diff+dst.nof_slots_per_system_frame();
  return dis;
}

void lower_phy_baseband_processor::start(baseband_gateway_timestamp init_time)
{
  last_rx_timestamp = init_time;
  // fmt::print("rx_buffer_size: {}; tx_buffer_size: {}; tx_time_offset: {}; rx_to_tx_max_delay: {}\n", rx_buffer_size, tx_buffer_size, tx_time_offset, rx_to_tx_max_delay);
  /// Calculate expected slot and symbol index.
  baseband_gateway_timestamp start_time = init_time + rx_to_tx_max_delay;
  auto i_sf =
    static_cast<unsigned>((start_time / nof_samples_per_subframe) % (NOF_SFNS * NOF_SUBFRAMES_PER_FRAME));
  // Calculate the sample index within the subframe.
  unsigned i_sample_sf = start_time % nof_samples_per_subframe;
  // Calculate symbol index within the subframe and the sample index within the OFDM symbol.
  unsigned i_sample_symbol = i_sample_sf;
  unsigned i_symbol_sf     = 0;
  while (i_sample_symbol >= symbol_sizes[i_symbol_sf]) {
    i_sample_symbol -= symbol_sizes[i_symbol_sf];
    ++i_symbol_sf;
  }
  // Calculate system slot index and the symbol index within the slot.
  unsigned i_slot   = i_sf * nof_slots_per_subframe + i_symbol_sf / nof_symbols_per_slot;
  // Create slot point.
  slot_point phy_slot(to_numerology_value(scs), i_slot);

  // Calculate slot from gps clock.
  auto now = gps_clock::now();
  auto ns_fraction = gps_clock::calculate_ns_fraction_from(now);
  slot_point gps_slot = gps_clock::calculate_slot_point(scs,
                    std::chrono::time_point_cast<std::chrono::seconds>(now).time_since_epoch().count(),
                    std::chrono::duration_cast<std::chrono::microseconds>(ns_fraction).count(),
                    1000 / get_nof_slots_per_subframe(scs));

  // Calculate the difference between the two slots plus calibration for the 0.1s `delay`.
  uint32_t diff = calculate_slot_diff(phy_slot, gps_slot);// + 10 * nof_slots_per_subframe * NOF_SUBFRAMES_PER_FRAME;
  slot_point ofh_slot(gps_slot.numerology(), gps_slot.sfn()%NOF_OFH_SFNS, gps_slot.slot_index());
  logger.warning("starting at slot {}", ofh_slot);
  unsigned cali =  ceil(static_cast<double>(rx_to_tx_max_delay) / static_cast<double>(nof_samples_per_subframe/nof_slots_per_subframe));
  diff += cali;

  rx_state.start();
  report_fatal_error_if_not(rx_executor.execute([this, diff]() {
    ul_process(diff); 
  }), "Failed to execute initial uplink task.");

  tx_state.start();
  report_fatal_error_if_not(
      downlink_executor.execute([this, start_time, diff]() {
        dl_process(start_time, diff); 
      }),
      "Failed to execute initial downlink task.");
}

void lower_phy_baseband_processor::stop()
{
  rx_state.request_stop();
  tx_state.request_stop();
  rx_state.wait_stop();
  tx_state.wait_stop();
}

void lower_phy_baseband_processor::dl_process(baseband_gateway_timestamp timestamp, uint32_t offset)
{
  // Check if it is running, notify stop and return without enqueueing more tasks.
  if (!tx_state.is_running()) {
    tx_state.notify_stop();
    return;
  }
    // Get transmit baseband buffer. It blocks if all the buffers are enqueued for transmission.
    std::unique_ptr<baseband_gateway_buffer_dynamic> dl_buffer = tx_buffers.pop_blocking();

    // Throttling mechanism to keep a maximum latency of one millisecond in the transmit buffer based on the latest
    // received timestamp.
    {
      // Calculate maximum waiting time to avoid deadlock.
      std::chrono::microseconds timeout_duration = 2 * std::chrono::microseconds(tx_buffer_size * 1000 / srate.to_kHz());
      // Maximum time point to wait for.
      std::chrono::time_point<std::chrono::steady_clock> wait_until_tp =
          std::chrono::steady_clock::now() + timeout_duration;
      // Wait until one of these conditions is met:
      // - The reception timestamp reaches the desired value;
      // - The system time reaches the maximum waiting time; or
      // - The lower PHY was stopped.
      while ((timestamp > (last_rx_timestamp.load(std::memory_order_acquire) + rx_to_tx_max_delay)) &&
            (std::chrono::steady_clock::now() < wait_until_tp) && tx_state.is_running()) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
      }
    }

    // Throttling mechanism to slow down the baseband processing.
    if ((cpu_throttling_time.count() > 0) && (last_tx_time.has_value())) {
      std::chrono::time_point<std::chrono::high_resolution_clock> now     = std::chrono::high_resolution_clock::now();
      std::chrono::nanoseconds                                    elapsed = now - last_tx_time.value();

      if (elapsed < cpu_throttling_time) {
        std::this_thread::sleep_until(last_tx_time.value() + cpu_throttling_time);
      }
    }
    last_tx_time.emplace(std::chrono::high_resolution_clock::now());

    // Process downlink buffer.
    trace_point                           tp          = ru_tracer.now();
    baseband_gateway_transmitter_metadata baseband_md = downlink_processor.process(dl_buffer->get_writer(), timestamp, offset);
    ru_tracer << trace_event("downlink_baseband", tp);

    // Set transmission timestamp.
    baseband_md.ts = timestamp + tx_time_offset;

    // Enqueue transmission.
    report_fatal_error_if_not(tx_executor.execute([this, tx_buffer = std::move(dl_buffer), baseband_md]() mutable {
      trace_point tx_tp = ru_tracer.now();

      // Transmit buffer.
      // uhd::time_spec_t time_spec = uhd::time_spec_t::from_ticks(baseband_md.ts, srate.to_Hz());
      // slot_point ofh_slot = gps_clock::get_ofh_slot_now(scs);
      // logger.warning("[{}] : DL new samples {}", ofh_slot, time_spec.get_real_secs());
      transmitter.transmit(tx_buffer->get_reader(), baseband_md);
      // Return transmit buffer to the queue.
      tx_buffers.push_blocking(std::move(tx_buffer));

      ru_tracer << trace_event("transmit_baseband", tx_tp);
    }),
                              "Failed to execute transmit task.");

    // Enqueue DL process task.
    report_fatal_error_if_not(downlink_executor.defer([this, timestamp, offset]() { dl_process(timestamp + tx_buffer_size, offset); }),
                              "Failed to execute downlink processing task");
}

void lower_phy_baseband_processor::ul_process(uint32_t offset)
{
  // Check if it is running, notify stop and return without enqueueing more tasks.
  if (!rx_state.is_running()) {
    rx_state.notify_stop();
    return;
  }

  // Get receive buffer.
  std::unique_ptr<baseband_gateway_buffer_dynamic> rx_buffer = rx_buffers.pop_blocking();

  // Receive baseband.
  trace_point                         tp          = ru_tracer.now();
  baseband_gateway_receiver::metadata rx_metadata = receiver.receive(rx_buffer->get_writer());
  ru_tracer << trace_event("receive_baseband", tp);

  // Update last timestamp.
  last_rx_timestamp.store(rx_metadata.ts + rx_buffer->get_nof_samples(), std::memory_order_release);

  // Queue uplink buffer processing.
  report_fatal_error_if_not(uplink_executor.execute([this, ul_buffer = std::move(rx_buffer), rx_metadata, offset]() mutable {
    trace_point ul_tp = ru_tracer.now();

    // Process UL.
    // uhd::time_spec_t time_spec = uhd::time_spec_t::from_ticks(rx_metadata.ts, srate.to_Hz());
    // slot_point ofh_slot = gps_clock::get_ofh_slot_now(scs);
    // logger.warning("[{}] : UL new samples {}", ofh_slot, time_spec.get_real_secs());
    uplink_processor.process(ul_buffer->get_reader(), rx_metadata.ts, offset);
    // Return buffer to receive.
    rx_buffers.push_blocking(std::move(ul_buffer));

    ru_tracer << trace_event("uplink_baseband", ul_tp);
  }),
                            "Failed to execute uplink processing task.");
  
  //fmt::print("ul process about to finish\n");
  // Enqueue next iteration if it is running.
  report_fatal_error_if_not(rx_executor.defer([this, offset]() { ul_process(offset); }), "Failed to execute receive task.");
}
