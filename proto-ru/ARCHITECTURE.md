<!--
SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
SPDX-License-Identifier: BSD-3-Clause-Open-MPI
-->

# ProtO-RU architecture

ProtO-RU combines the application under `apps/examples/ofh/ru_emulator` with the reusable O-RU sector in `lib/ofh`.
Direction-independent OFH logic lives in the library and serves both the O-DU and O-RU.
The application remains a thin driver that connects either test IQ or a radio to the library interfaces.

The presence of an `sdr` configuration section selects the operating mode for each RU:

- **Loopback:** Reply to O-DU Control-Plane requests immediately with generated test IQ.
- **SDR:** Drive a UHD radio and the lower PHY through the in-tree `radio_unit` created by `create_sdr_ru`.
  The RU transmits downlink IQ over the air and returns captured uplink IQ over the fronthaul.

---

## 1. Layered architecture

```
┌───────────────────────────── apps/examples/ofh  (thin driver) ─────────────────────────────┐
│  main()                                                                                      │
│   ├─ worker_manager (executors + CPU affinity)        ru_emulator_timing_notifier (GPS clock)│
│   └─ create_ru_emulator(i)                                                                    │
│        ├─ LOOPBACK ─► ru_emulator_upper_phy            (test-IQ source)                       │
│        └─ SDR ──────► ru_emulator_sdr_upper_phy  +  ru_emulator_tti_orchestrator              │
│                       ru_emulator_rx_symbol_adapter ─► create_sdr_ru (REUSED radio_unit)      │
│        Ethernet: ether::create_receiver / create_transmitter (same factories as the O-DU)     │
└──────────────▲────────────────────────────▲──────────────────────────────▲──────────────────┘
       ru_upper_phy │             ru_uplink_iq_sink │        ota_symbol_boundary_notifier │  (public seams)
┌──────────────┴────────────────────────────┴──────────────────────────────┴──────────────────┐
│  lib/ofh   O-RU sector  (public: include/ocudu/ofh/ru_sector.h, create_ru_sector)             │
│   RX :  ru_message_receiver ─► ru_rx_cplane_data_flow ─► dispatcher ─► {handler | recorder}   │
│                              ─► ru_rx_uplane_data_flow ─► rx_grid_context_repository           │
│         (sequence_id_checker · rx_window_checker)                                              │
│   TX :  data_flow_uplane_data (direction-neutral) ─► eth_frame_pool ─► ru_message_transmitter │
│   DL :  ru_downlink_rx_window_handler  (finalises received downlink)                           │
│   store-and-respond (SDR): ru_uplink_scheduling_recorder + uplink/prach responders            │
│                            + ru_uplink_request_repository                                     │
│   serdes (generalised): ofh_uplane_message_builder/decoder · ofh_cplane_message_decoder       │
└────────────────────────┬────────────────────────────────────────────────────────────────────┘
                         │  (SDR only) ru_uplink_plane · ru_timing_notifier · ru_downlink_plane
┌─────────────────────────▼────────────────────────────────────────────────────────────────────┐
│  lib/ru/sdr + lib/phy/lower + lib/radio   (REUSED in-tree, not forked)                         │
│   create_sdr_ru ─► radio_unit ─► lower PHY (mod/demod) ─► baseband gateway ─► UHD driver       │
└──────────────────────────────────────────────────────────────────────────────────────────────┘
```

Three public interfaces connect the application to the O-RU sector:

- `ofh::ru_upper_phy` provides the sector with a downlink grid sink and uplink and PRACH IQ sources.
- `ofh::ru_uplink_iq_sink` accepts captured uplink IQ from the radio in store-and-respond mode.
- `ofh::ota_symbol_boundary_notifier` advances the receive-window checker and downlink finalizer on the timing thread.
  It also schedules the message transmitter on `ru_ofh_tx` through `ru_ota_symbol_task_dispatcher`.

---

## 2. Everything is decoupled through buffers

The pipeline does not process requests and responses in lockstep.
Each stage writes to a buffer that another stage drains on its own clock.

| Buffer | Holds | Filled by (thread) | Drained by (thread) |
|---|---|---|---|
| `rx_grid_context_repository` | received **downlink** grid (assembling) | `ru_emu` OFH decode path | DL finalizer; SDR mode then pushes the completed grid to the radio |
| `ru_uplink_request_repository` | recorded **uplink/PRACH C-plane requests** | recorder on `ru_emu` | responder in the radio IQ callback context |
| `eth_frame_pool` (UL + PRACH, 20-slot ring) | built **uplink/PRACH U-plane** frames | UL build deferred to `ru_ofh_tx` (PRACH inline) | `ru_message_transmitter`, deferred to `ru_ofh_tx` |

O-DU frames arrive over Ethernet and populate the downlink-grid and Control-Plane request repositories.
The lower PHY reads the downlink grid and transmits it over the air.
Captured IQ callbacks match uplink data to recorded requests. UL encoding runs on `ru_ofh_tx`; PRACH encoding
runs inline in its callback. Both paths write frames to the Ethernet frame pool.
The OFH transmitter drains that pool at the required air time.

---

## 3. Pipeline (block diagram)

### Receive path (shared)

```
DU ──Ethernet──► ru_emulator.on_new_frame ──[dispatch to ru_emu_exec]──► sector.on_new_frame
                                                                                │
   ru_message_receiver: eth_decode → ecpri_decode → rx-window check → seq-id check
                                                                                │
              ┌───── iq_data (downlink U-plane) ─────┐     ┌──── rt_control (C-plane) ────┐
              ▼                                       │     ▼                              │
   ru_rx_uplane_data_flow → decode →                 │   ru_rx_cplane_data_flow → decode
   rx_symbol_writer → rx_grid_context_repository      │   → dispatcher (drop unconfigured eAxC)
              (assembles received downlink)           │       │ PRACH(sect3)→prach_eaxc
                                                      │       │ uplink      →ul_eaxc
                                                      │       │ downlink    →dl_eaxc
                                                      │       ▼
                                                      │  LOOPBACK handler         SDR recorder
                                                      │   DL → get grid + repo.add  DL → get grid + repo.add
                                                      │   UL → get_uplink_tx_grid   UL/PRACH → record context
                                                      │        (test IQ) → enqueue        (reply later)
                                                      │   PRACH → get_prach_tx_buffer
                                                      │           → enqueue
                                                      ▼
                                               data_flow_uplane_data → eth_frame_pool
```

### Transmit drain (both modes: GPS timing tick, send on `ru_ofh_tx`)

```
ru_emulator_timing_notifier ──on_new_symbol──► ru_ota_symbol_task_dispatcher ─[defer]─► ru_ofh_tx :
                            │                       ru_message_transmitter ── drain pool over tx-window ──► DU
                            ├────────────────► rx_window_checker          ── advance OTA (inline, timing)
                            └────────────────► ru_downlink_rx_window_handler ── finalise received DL ──┐ (inline)
                                  loopback: count   SDR: sdr_upper_phy pushes handle_dl_data ─► AIR (DL TX)
```

### SDR radio path (baseband and compute workers)

```
radio_unit (radio clock) ──on_tti_boundary(slot)──► gps_slot_aligner ─► ru_emulator_tti_orchestrator
        │                       ├─ handle_new_uplink_slot(slot)             ── ask radio to capture UL
        │                       └─ peek PRACH occasion (slot − max_proc_delay) ── ask radio to capture PRACH
        │
        └─ captured UL/PRACH ─► rx_symbol_adapter ─► ru_uplink_iq_sink ─► uplink/prach responder
                                (match recorded C-plane ctx) ─► data_flow_uplane_data ─► eth_frame_pool ─┘
(the received downlink is NOT pulled here: it is pushed to the radio when the reception window finalises it)
```

---

## 4. Worker / CPU mapping

`worker_manager` owns the OFH workers and delegates SDR workers to `ru_emulator_sdr_worker_manager`.
The SDR manager uses the upstream `ru_sdr_executor_mapper`: baseband transport stays on dedicated per-cell workers,
while non-sequential cells share one lower-PHY compute pool.
OFH and timing keep their own CPU settings.

| Worker | Scope | Responsibility | Priority | YAML affinity setting |
| --- | --- | --- | --- | --- |
| `ru_rx_#i` | Per cell | Receive Ethernet frames from AF_PACKET or DPDK and dispatch them to `ru_emu` | `rt_max - 1` | `ru_emu.cells[i].ofh_cpus` |
| `ru_emu_#i` | Per cell | Collect receive statistics, decode OFH, dispatch C-plane requests, and assemble DL grids | `rt_max - 1` | `ru_emu.cells[i].ofh_cpus` |
| `ru_ofh_tx_#i` | Per cell | Build/compress UL U-plane and drain UL/PRACH Ethernet frames within the Ta3 window | `rt_max - 1` | `ru_emu.cells[i].ofh_cpus` |
| `ru_timing` | One shared | Advance OTA window checks and finalize DL grids; dispatch Ethernet transmission | `rt_max` | `ru_emu.timing_cpus` |
| `ru_radio_#i` | Per SDR cell | Handle asynchronous radio events | Normal | `ru_emu.cells[i].sdr.ru_cpus` |
| `ru_phy_#i` | Sequential or single | Combined baseband TX/RX | `rt_max` | `ru_emu.cells[i].sdr.ru_cpus` |
| `ru_phy_tx_#i` | Dual or triple | Transmit baseband samples | `rt_max` | `ru_emu.cells[i].sdr.ru_cpus` |
| `ru_phy_rx_#i` | Dual or triple | Receive baseband samples | Dual: `rt_max - 1`; triple: `rt_max - 2` | `ru_emu.cells[i].sdr.ru_cpus` |
| `ru_phy_ul_#i` | Triple | Preprocess UL baseband samples | `rt_max - 1` | `ru_emu.cells[i].sdr.ru_cpus` |
| `ru_phy_pool#k` | Shared across non-sequential SDR cells | DL modulation, UL OFDM demodulation, and PRACH processing | `rt_max - 2` | `ru_emu.phy_cpus` |

`ru_rx` stays separate from `ru_emu` so the receiver can continue draining packets while the decoder processes an
older frame. The handoff adds a queue but prevents decoding and decompression from directly delaying the next receive.
The workers may share CPUs through `ofh_cpus`; separate threads do not require separate cores.

UL building/compression uses `ru_uplink_task_dispatcher`, and OTA-driven transmission uses
`ru_ota_symbol_task_dispatcher`. Both defer work to `ru_ofh_tx`; the timing worker does not perform the Ethernet send.
PRACH building remains inline in its calling context, with transmission deferred to `ru_ofh_tx`.

### Lower-PHY profiles

| `sdr.execution_profile` | Baseband workers per SDR cell | Compute placement |
| --- | --- | --- |
| `sequential` | `ru_phy` handles TX/RX | DL, UL, demodulation, and PRACH executors run inline in the calling context |
| `single` | `ru_phy` handles TX/RX; UL preprocessing runs inline | Shared `ru_phy_pool` for DL modulation, UL demodulation, and PRACH |
| `dual` | Separate `ru_phy_tx` and `ru_phy_rx`; UL preprocessing runs inline on RX | Shared `ru_phy_pool` |
| `triple` | Separate `ru_phy_tx`, `ru_phy_rx`, and `ru_phy_ul` | Shared `ru_phy_pool` |
| `auto` | `single` below four available host CPUs, `dual` below eight, otherwise `triple` | As selected above |

The ZMQ driver always selects `sequential`. Loopback cells and sequential SDR cells do not need the compute pool.
The radio worker exists for every SDR profile.

### Shared compute pool

`ru_emu.nof_phy_threads` sets the requested pool size. It must be positive and is capped by CPU availability.
When omitted, the estimate is two workers plus the sum of DL and UL antenna counts over non-sequential SDR cells.
An explicit `phy_cpus` mask caps the pool at the number of CPUs in that mask.
Without an explicit mask, the cap leaves up to three available CPUs of thread capacity for other work, with a
minimum of one compute worker. This limit does not reserve or exclude particular CPUs.

All pool workers share a bounded lock-free MPMC queue of 2048 tasks, a 50 microsecond idle backoff, and the same
CPU affinity mask. A worker can execute any queued DL, UL-demodulation, or PRACH task from any participating cell.
There is no fixed worker-to-cell or worker-to-direction assignment.

An empty `phy_cpus` list uses the CPUs available at application startup, including when DPDK later pins its main
thread. It does not automatically exclude `ofh_cpus`, `timing_cpus`, or `sdr.ru_cpus`.
Set disjoint masks explicitly when those workloads need separate CPUs.

### Example: one SDR cell on eight CPU IDs

The following is the CPU/threading portion of a configuration; add the Ethernet, cell, and radio parameters from
[Configuration Reference](CONFIG_REFERENCE.md) or a [paired example](conf-files/README.md).

```yaml
ru_emu:
  timing_cpus: [1]
  phy_cpus: [6, 7]
  nof_phy_threads: 2
  cells:
    - ofh_cpus: [2, 3]
      sdr:
        execution_profile: triple
        ru_cpus: [4, 5]
        pinning_policy: round-robin
```

| Thread | Allowed CPU IDs | Responsibility |
| --- | --- | --- |
| `ru_timing` | 1 | OTA timing |
| `ru_rx_#0` | 2, 3 | Ethernet receive |
| `ru_emu_#0` | 2, 3 | OFH decode and DL grid assembly |
| `ru_ofh_tx_#0` | 2, 3 | UL encode/compress and Ethernet transmission |
| `ru_radio_#0` | 4 | Radio asynchronous events |
| `ru_phy_tx_#0` | 5 | Baseband TX |
| `ru_phy_rx_#0` | 4 | Baseband RX |
| `ru_phy_ul_#0` | 5 | UL preprocessing |
| `ru_phy_pool#0` | 6, 7 | Shared PHY compute |
| `ru_phy_pool#1` | 6, 7 | Shared PHY compute |

Round-robin allocation follows creation order: radio, TX, RX, then UL. With `pinning_policy: mask`, every radio and
baseband worker may instead run anywhere in `ru_cpus`. Both compute workers may run on either CPU 6 or 7.
CPU 0 has no application worker assigned by these settings; a DPDK example can use it for the EAL main lcore.
This example creates ten application workers. The main thread, logging, and radio driver can create additional threads.

CPU numbers are Linux logical CPU IDs. Check the host's core and SMT topology before assigning them.
Affinity settings do not isolate or exclusively reserve cores, and do not pin the main, logger, or driver threads.

### Tuning the CPUs

- Keep timing, OFH, baseband transport, and compute masks separate when enough CPUs are available.
- Give `ru_timing` a low-jitter CPU and avoid busy SMT siblings on baseband and compute CPUs.
- Use `phy_cpus` and `nof_phy_threads` for compute capacity. Adding CPUs only to `sdr.ru_cpus` does not grow the pool.
- Give `ofh_cpus` enough capacity for packet bursts and the Ta3 send deadline. Its three per-cell workers may share CPUs;
  the required capacity depends on bandwidth, antenna count, backend, and host performance.
- Run with real-time scheduling privileges, such as root or `CAP_SYS_NICE` with suitable scheduling/resource limits.
- Use a real-time kernel for live SDR operation and validate the CPU layout under the intended traffic load.

SDR mode uses two clocks: the GPS/host-clock worker advances OFH windows, while radio baseband notifications drive
the TTI orchestrator. The two paths meet through buffered grids, recorded requests, and the Ethernet frame pool.

---

## 5. Timing relations (O-RAN WG4 windows) and how to set them

All timing is referenced to the **air time `T`** of a slot:

```
        DU transmits DL          RU air time T            RU transmits UL
   ─────────┼────────────────────────┼────────────────────────┼──────────►  time
       T − T1a                       T                     T + Ta3
            └─ RU must RECEIVE within ─┘                       └─ DU must RECEIVE within ─┐
               its T2a window                                     its Ta4 window
               [T−T2a_max, T−T2a_min]                             [T+Ta4_min, T+Ta4_max]
```

### T2a receive windows

T2a defines when the RU receives downlink Control-Plane, downlink User-Plane, and uplink Control-Plane messages.
Configure these windows with `T2a_{max,min}_cp_dl`, `T2a_{max,min}_up`, and `T2a_{max,min}_cp_ul` in microseconds.

These values populate `ru_sector_config::rx_window` and determine how far ahead the uplink request arrives.
The `rx_window_checker` classifies frames as early, on time, or late for statistics.
It does not drop frames.

### Ta3 transmit window

Ta3 defines when the RU transmits uplink User-Plane messages.
Configure this window with the cell-level `ta3_max_up` and `ta3_min_up` values in microseconds.
Their defaults are 300 and 85 microseconds.

`ru_message_transmitter` converts these values to `ru_sector_config::tx_window_start_symbols` and `tx_window_end_symbols`.
It drains a frame for air symbol `s` while the current OTA symbol lies between `s - tx_window_start` and `s - tx_window_end`.

### Lower-PHY processing lead

In SDR mode, `max_proc_delay` sets `lower_phy_configuration::max_processing_delay_slots`.
The default is two slots.
This setting controls the upstream lower PHY created by `create_sdr_ru`.

The lower PHY advances its `on_tti_boundary` slot and timestamp by `max_proc_delay` slots.
Its transmit pipeline also maintains the upstream 1 ms RX-to-TX baseband lead.
ProtO-RU does not use the advanced TTI notification as the downlink fronthaul delivery deadline.

The downlink reception-window handler finalizes and pushes a grid to the radio 1 ms plus one slot before air time.
The PRACH path looks back by `max_proc_delay` slots from the advanced TTI.
This selects the occasion approaching the radio capture point.
These mechanisms decouple fronthaul deadlines from `max_proc_delay`.

> **Legacy implementation:** Earlier ProtO-RU releases used a custom lower PHY named `ru_lower_phy_baseband_processor`.
> Its configuration did not provide `max_processing_delay_slots`.
> It combined `rx_to_tx_max_delay = srate.to_kHz() + tx_time_offset` with GPS slot-difference alignment during `start()`.
> The current implementation uses the upstream lower PHY instead of maintaining that fork.

### Timing constraints

The fronthaul and radio paths impose these constraints:

- `T1a > T2a + propagation` ensures that downlink data reaches the RU in time.
- `Ta4_min ≤ Ta3 + propagation ≤ Ta4_max` places uplink data inside the O-DU receive window.
- `T2a_max_cp_dl > 1 ms + slot` gives the RU time to deliver the downlink grid to the radio.
- `T2a_max_cp_ul > 1 ms + slot/2` gives the RU time to request PRACH capture.
- `T2a_max_up > 1 ms + slot + 13 symbols` gives the RU time to assemble downlink User-Plane data.

The exact lower bounds depend on the configured subcarrier spacing.
`create_ru_emulator` checks these constraints at startup.

Configure the same windows on the O-DU and leave headroom for receive and decode latency.
The CLI accepts T2a values up to 5000 microseconds for software-fronthaul deployments.

### Why ProtO-RU uses large values

A software fronthaul adds latency in Ethernet decoding, IQ compression, OFDM processing, and encoding.
Scheduling jitter also changes this latency between packets.

Large windows keep received frames within their expected timing range.
They also give each buffered stage enough time to deliver data before the next stage needs it.

### The clock model

The emulator runs a GPS wall clock and a radio sample clock.
`ru_emulator_timing_notifier` uses the GPS wall clock to drive the OFH transmit drain, receive-window checker, and downlink finalizer.
The lower PHY uses the radio sample clock to drive the orchestrator's `on_tti_boundary` callback.

A GPS-disciplined radio aligns both clocks to the same seconds.
The upstream `start_time` and `sfn0_ref_time` mechanism anchors SFN0 to the radio PPS.
The radio and O-DU still use different slot-number origins.

The radio counts slots from its start PPS.
The O-DU derives wire slots from the GPS timescale and carries SFN modulo 256.
ProtO-RU therefore measures an offset between radio slots and wire slots.

`ru_emulator_gps_slot_aligner` measures this offset from TTI notifications at runtime.
Each notification provides a radio slot and a host-clock time point.
The aligner adds the lower PHY's 1 ms RX-to-TX lead to obtain the slot's air time.
It subtracts the radio slot from the GPS slot at that air time.

The aligner ignores the first second while the pipeline fills.
It applies the median of the next 64 measurements as the initial offset.
It continues measuring and applies a new offset when two consecutive windows agree on a change.
The TTI orchestrator and RX-symbol adapter use the current offset to translate between radio and wire slots.

Radio requests retain radio slot numbering.
O-RU sector lookups use wire slot numbering.
This arrangement requires an accurate host clock and supports `gpsdo`, `external`, and `internal` radio clock sources.

### Practical guidance

- `generate_ru_emulator_sdr_config` sets `start_time` to a near-future whole-second boundary.
- Keep the host clock synchronized through GPS or NTP because the slot aligner measures wire numbering against it.
- Use `gpsdo` or a shared external 10 MHz and PPS reference to limit long-term radio drift.
- Use `calibrate_clock_ppm` to compensate for measured drift when the radio uses its internal TCXO.
- Replace the broad software timing windows with hardware-calibrated values after validating the deployment.

### SDR timing implementation

- `ru_sector_config::dl_grid_finalize_offset_symbols` controls downlink finalization.
  Loopback mode retains the default one-slot delay and discards the downlink grid.
  SDR mode finalizes the grid 1 ms plus one slot before air time and pushes it to the radio downlink plane.
- The radio TTI does not trigger downlink delivery because it occurs before practical T2a windows can supply the grid.
- At each TTI, the PRACH path looks back by `max_proc_delay` slots and requests capture 1 ms before that recorded occasion reaches air time.
- `ru_emulator_gps_slot_aligner` measures the GPS slot offset at runtime against the host clock.
  The initial log message reports the applied offset and states that drift tracking remains active.
  A later log message reports any offset adjustment confirmed by two consecutive measurement windows.
- `ru_uplink_request_repository` consumes each uplink or PRACH Control-Plane request after an exact-slot match.
  This prevents stale requests from generating further traffic.
- Uplink and PRACH User-Plane replies copy the section ID from the Control-Plane request.

Unit tests cover these timing mechanisms and slot-number translations.
The original ProtO-RU hardware coverage is summarized in [Features](FEATURES.md); the rebased worker model needs renewed RF validation.

The PRACH radio path uses `ru_sector::get_recorded_prach_occasion(slot)` to obtain a recorded Control-Plane occasion.
The orchestrator then calls `handle_prach_occasion` with a short-format B4 `prach_buffer_context`.
The O-RAN Control-Plane request does not carry the detector configuration, so ProtO-RU supplies this context locally.

---

## 6. Configuring an RU: lower-PHY profile and examples

The emulator runs in loopback mode by default.
Adding an `sdr:` block switches that cell to SDR mode through `create_sdr_ru`.
`generate_ru_emulator_sdr_config` maps the application configuration to a `radio_configuration` and a single-sector `lower_phy_configuration`.

### The knobs that matter

| Field (`sdr:` block) | How to set it |
|---|---|
| `device_driver` | Select `uhd` for a USRP. |
| `device_args` | Set UHD driver arguments such as `type=b200`. |
| `srate` (MHz) | Use the sample rate from a validated profile for the selected bandwidth and radio. |
| `common_scs` (cell-level) | Select 15 or 30 kHz for all cells because the shared numerology controls T2a/Ta3 conversion, slot duration, valid sample rates, and short-preamble PRACH SCS. |
| `bandwidth` (cell, MHz) | Set `bandwidth_rb` while ensuring that `srate` meets its Nyquist rate. |
| `max_proc_delay` | Set the lower-PHY TTI advance in slots, with a default of 2 (see §5). |
| `dl_arfcn` / `band` (cell-level) | Set the DL ARFCN and optional NR band from which the band tables derive the DL and UL centre frequencies. |
| `dl_freq_override` / `ul_freq_override` (Hz) | Override the centre frequencies or omit these values to use the band-derived frequencies. |
| `tx_gain` / `rx_gain` (dB) | Set the USRP radio gains. |
| `center_freq_offset` (Hz) | Apply an RF calibration offset to the centre frequency of every channel. |
| `calibrate_clock_ppm` | Apply carrier clock calibration in PPM to compensate for measured TCXO drift (see §5). |
| `lo_offset` (MHz) | Shift the LO away from the centre to move LO leakage outside the channel. |
| `time_alignment_calibration` | Override the RF driver's RX-to-TX alignment in samples, using a positive value to move PRACH earlier within its window. |
| `transmission_mode` | Select `continuous` (default), `discontinuous` (TDD), or `same-port`. |
| `power_ramping` (µs) | Set the TX power-ramp time for discontinuous transmission. |
| `gain_backoff` (dB) | Reserve amplitude headroom for signal PAPR and DFT normalization, with a default of 12 dB. |
| `clock_source` / `sync_source` | Select `default`, `internal`, `external`, or `gpsdo`, using `gpsdo` or a shared external 10 MHz and PPS reference for long-running synchronization (see §5). |
| `execution_profile` | Select `auto`, `sequential`, `single`, `dual`, or `triple` for the lower-PHY baseband threads (see §4). |
| `ru_cpus` | Set the CPU affinity for radio and baseband workers (see §4, *Tuning the CPUs*). |
| `pinning_policy` | Select `mask` to share the entire CPU set or `round-robin` to pin each worker to one CPU in turn. |

The cell's eAxC lists determine the number of radio antenna ports.
The number of `dl_port_id` entries sets the TX port count, and the number of `ul_port_id` entries sets the RX port count.
This mapping keeps the radio and fronthaul configurations consistent.

ProtO-RU sets the remaining lower-PHY fields internally.
It uses these values:

- `cp = NORMAL`
- `dft_window_offset = 0.5`
- `ta_offset = get_ta_offset(FR1)`
- `max_nof_prach_concurrent_requests = max_proc_delay + 2`
- `baseband_rx_buffer_size_policy = single_packet`
- `system_time_throttling = 0`

Cell-level compression settings mirror the O-DU split 7.2 OFH configuration.
The library stores one `ru_compression_params` value per direction and selects static or dynamic serialization from these settings.

| Field (cell-level) | Meaning |
|---|---|
| `compr_method_ul` / `compr_bitwidth_ul` | compression of the **uplink** U-plane the RU *transmits* (`none`/`bfp`, 9 or 16). |
| `compr_method_dl` / `compr_bitwidth_dl` | compression of the **downlink** U-plane the RU *receives* / decompresses. |
| `compr_method_prach` / `compr_bitwidth_prach` | compression of the **PRACH** U-plane the RU transmits. |
| `iq_scaling` | scaling applied before compression (1.0 = none; lower for real captured signals). |
| `is_ul_static_compr_hdr` / `is_dl_static_compr_hdr` | **static** (out-of-band) vs **dynamic** (in-band) compression header: must match the O-DU. |

`--cells` is a YAML **list of mappings**; each entry is one RU.
The `sdr:` block is a nested mapping inside the cell (it parses through the per-cell sub-app).
Without it, the cell is a loopback RU.

### Example: loopback (default, no radio)

```yaml
log:
  level: info
ru_emu:
  timing_cpus: [1]                         # ru_timing: GPS OTA tick only: window checker + DL finalise (the transmit drain+sendto is handed to ru_ofh_tx)
  cells:
    # AF_PACKET uses a kernel NIC name or veth.
    # DPDK uses the NIC's PCIe address as described below.
    - network_interface: enp1s0f0
      ru_mac_addr: 0e:42:a3:ef:fd:71
      du_mac_addr: aa:bb:cc:dd:ee:ff
      vlan_tag: 1
      bandwidth: 20
      ul_port_id: [0]
      dl_port_id: [0]
      prach_port_id: [4]
      compr_method_ul: bfp                  # UL/DL/PRACH compression (none|bfp), 9 or 16 bits
      compr_bitwidth_ul: 9
      ofh_cpus: [2, 3]                      # ru_rx + ru_emu + ru_ofh_tx: fronthaul receive/decode + the OFH transmit (UL build/compress + drain+sendto)
# To drive the fronthaul over DPDK instead of AF_PACKET:
#   1. Build with -DENABLE_DPDK=ON because the application rejects the dpdk block without DPDK support.
#   2. Create an SR-IOV virtual function and bind it to vfio-pci. See "Preparing the fronthaul port" in
#      proto-ru/CONFIG_REFERENCE.md for the full sequence, including the VF MAC, port VLAN, and MTU.
#   3. Set network_interface above to the virtual function's PCIe address, such as "0000:01:00.0", instead of enp1s0f0.
#   4. Add the dpdk block below and allow-list the same PCIe address with `-a`.
#   5. Run with root privileges and configured hugepages.
# dpdk:
#   eal_args: "-l 0-3 -a 0000:01:00.0 --proc-type auto"
```

### Example: SDR over UHD (USRP, GPS-disciplined)

```yaml
ru_emu:
  timing_cpus: [1]                           # shared OTA timing worker
  phy_cpus: [6, 7]                          # shared modulation/demodulation/PRACH pool
  nof_phy_threads: 2
  cells:
    - network_interface: enp1s0f0
      ru_mac_addr: 0e:42:a3:ef:fd:71
      du_mac_addr: aa:bb:cc:dd:ee:ff
      vlan_tag: 1
      bandwidth: 20
      dl_arfcn: 632628                       # n78 at 3489.42 MHz; DL/UL freq derived from this + band
      prach_format: short                    # SDR mode requires short PRACH (B4)
      ul_port_id: [0]
      dl_port_id: [0]
      prach_port_id: [4]
      t2a_max_cp_dl: 2500                     # Must exceed 1 ms plus one slot.
      t2a_max_cp_ul: 2500
      t2a_max_up:    2500                     # Must exceed 1 ms plus one slot plus the final-symbol offset.
      ta3_max_up:    300                      # uplink U-plane transmit (Ta3) window
      ta3_min_up:    85
      compr_method_ul: bfp                    # per-direction compression (ul / dl / prach)
      compr_bitwidth_ul: 9
      ofh_cpus: [2, 3]                        # ru_rx + ru_emu + ru_ofh_tx: fronthaul receive/decode + UL build/compress + send (shareable set)
      sdr:
        device_driver: uhd
        device_args: type=b200
        srate: 23.04
        tx_gain: 60
        rx_gain: 60
        calibrate_clock_ppm: 0.0             # dial in your TCXO offset on a non-GPSDO clock
        gain_backoff: 12                     # transmit baseband gain backoff (dB)
        clock_source: gpsdo                  # anchor SFN0 to the PPS (see §5 known items)
        sync_source: gpsdo
        max_proc_delay: 4
        execution_profile: triple
        ru_cpus: [4, 5]                       # radio and baseband transport workers
        pinning_policy: round-robin
```
