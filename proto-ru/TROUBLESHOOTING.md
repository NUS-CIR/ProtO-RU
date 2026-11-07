<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Troubleshooting

Use the following checks to diagnose common ProtO-RU problems.

## Network Issues

If ProtO-RU and the DU do not receive packets from each other, check the following items:

1. Confirm that both network interfaces are up.
1. Confirm that the RU and DU MAC addresses, network interface name, and VLAN ID match at both ends.
1. Consider setting `enable_promiscuous: true` on the DU and ProtO-RU.
1. Check whether the fronthaul interface already tags the traffic itself. An SR-IOV VF configured with a port VLAN
   (`ip link` reports `vf 0 ... vlan 5`) has the PF insert the tag on egress and strip it on ingress, regardless of
   whether the VF is bound to the kernel driver or to `vfio-pci`. Combining that with `vlan_tag` puts a second 802.1Q
   header on the wire, and each frame arrives 4 bytes longer than expected. Omit `vlan_tag` on such an interface, and
   omit `vlan_tag_cp`/`vlan_tag_up` on the DU only when its own interface also inserts the VLAN in hardware.
   Otherwise, retain the DU software VLAN with the same ID.

## Timing and Stability Issues

Restart ProtO-RU if the KPI output reports `Error: exceeded maximum number of timed out transmissions.`, which indicates a USRP transmission problem.

Persistent occurrences of the following symptoms usually indicate timing problems:

- Missed or dropped OFH packets at the DU.
- Repeated `Real-time failure in lower PHY: PUxCH request late for sector` warnings in the ProtO-RU logs.
- Repeated `LATE` packet arrivals from the DU in the ProtO-RU KPI output, while occasional `EARLY` arrivals are acceptable.

Watch the logs and the per-thread CPU utilization together before changing any timing parameter, because a worker pool
that is short of CPUs produces the same symptoms as a genuinely mistuned window. Run `htop` with the thread view
enabled:

```bash
htop -H     # or press "H" inside htop to toggle individual threads
```

Each worker thread is named after the pool that owns it, so the saturated pool is visible directly:

| Threads | Work | Setting |
| --- | --- | --- |
| `ru_rx_#N`, `ru_emu_#N`, `ru_ofh_tx_#N` | Open Fronthaul receive, decode, build, and transmit | `ofh_cpus` |
| `ru_radio_#N`, `ru_phy_#N`, `ru_phy_tx_#N`, `ru_phy_rx_#N`, `ru_phy_ul_#N` | Radio events and profile-specific baseband transport/preprocessing | `sdr.ru_cpus`, `sdr.pinning_policy` |
| `ru_phy_pool#K` | Shared DL modulation, UL OFDM demodulation, and PRACH processing | `phy_cpus`, `nof_phy_threads` |
| `ru_timing` | Shared OTA timing worker | `timing_cpus` |

High utilization together with late-task warnings can indicate CPU contention or insufficient processing capacity.
Check the affected worker, its affinity, queue warnings, and scheduling privileges before changing timing windows.
A high utilization percentage alone does not establish the cause.

Missed or dropped packets reported at the DU point at `ofh_cpus` in the same way, even when the ProtO-RU logs stay
clean. OFH decoding on `ru_emu_#N` and uplink encoding/transmission on `ru_ofh_tx_#N` both need capacity; starving
either stage can leave gaps at the DU that resemble a network fault. Check the OFH thread
utilization before suspecting the link.

Keep these points in mind while reading the utilization:

- DPDK receive polls the NIC and sleeps briefly when no frames arrive; AF_PACKET waits with a short `select`
  timeout before `recvmsg`. Receive-thread utilization alone does not measure useful packet-processing load.
  Each cell has three OFH workers, but they can share CPUs. There is no fixed requirement for three OFH CPUs per
  cell: size the mask from packet loss, deadline warnings, queue pressure, and the intended bandwidth/antenna load.
- DPDK's EAL `-l` list does not assign the OFH workers. Their masks come from `ofh_cpus`. Keep EAL, timing,
  OFH, baseband, and compute CPU IDs separate when the host has enough capacity.
- Increasing `sdr.ru_cpus` changes baseband/radio placement, not the number of compute workers. For a saturated
  shared compute pool, adjust `phy_cpus` and `nof_phy_threads` together. Increasing the OFH mask does not create
  more OFH workers; each cell still has one receiver, one decoder, and one transmitter.

Use the following steps to address timing problems:

1. Run ProtO-RU on a dedicated machine to avoid CPU contention with other applications.
1. Use `taskset` to separate the CPU cores assigned to the DU and ProtO-RU when they run on the same machine.
1. Confirm that the `ptp4l` RMS value and `phc2sys` offset remain stable without large spikes while ProtO-RU runs.
1. Pin `ptp4l` and `phc2sys` to a CPU core that ProtO-RU does not use if their measurements are unstable.
1. Run the CPU in performance mode with idle states disabled, and use the real-time kernel required by [Hardware Requirements](./HW_REQUIREMENTS.md).
1. Check baseband transport workers on `sdr.ru_cpus` separately from compute workers on `phy_cpus`.
   Increase `nof_phy_threads` within the compute CPU capacity when compute is the bottleneck, and choose the
   `single`, `dual`, or `triple` baseband profile for the host.
1. Add CPU cores to `ofh_cpus` for missing or late DU packets because this set hosts the OFH receive, decode, build, and transmit workers.
1. Raise the DU Ta4 values for late uplink packet arrivals.
1. Reduce the bandwidth and USRP sample rate if timing failures persist.

Use these commands to switch between the performance and power-saving CPU modes:

```bash
# Enable performance mode and disable idle states.
sudo cpupower frequency-set -g performance
sudo cpupower idle-set -D 0

# Enable power-saving mode and idle states.
sudo cpupower frequency-set -g powersave
sudo cpupower idle-set -E
```

Reserve these CPUs through the host kernel configuration.
The current ProtO-RU schema does not have a top-level `isolated_cpus` setting.

## UE Attachment and Performance Issues

If timing is stable but the UE cannot attach or experiences poor throughput or high BLER, try the following steps:

1. Set the MAC scheduling advance higher than the configured T1a values.
   The TDD examples use O-DU `expert_phy.max_proc_delay: 10` for OCUDU and `sl_ahead = 10` for OAI;
   the 15 kHz FDD OCUDU examples use five slots.
   Do not confuse the O-DU setting with ProtO-RU's `sdr.max_proc_delay`, which controls lower-PHY processing depth and defaults to 2.
1. Tune the DU downlink IQ scaling through `iq_scaling` for OCUDU or `tx_amp_backoff_dB` for OAI.
1. Tune the ProtO-RU USRP gains through `tx_gain` and `rx_gain`.
1. Adjust the ProtO-RU `iq_scaling` value when uplink IQ samples require additional scaling.
1. Tune DU power parameters such as the PRACH preamble receive power, P0 nominal, and target SINR values.
1. Increase the number of LDPC decoding iterations when the DU uses a hardware accelerator such as ACC100.
