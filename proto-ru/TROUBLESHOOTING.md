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
   omit `vlan_tag_cp`/`vlan_tag_up` on the corresponding DU cell.

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
| `ru_radio_#N`, `ru_phy_#N`, `ru_phy_hp_#N` | SDR radio and lower PHY | `sdr.ru_cpus` |
| `ru_timing` | Shared OTA timing worker | `timing_cpus` |

Real-time warnings that coincide with those threads at 90% utilization or above indicate a pool short of CPUs rather
than a mistuned window. Add cores to the corresponding setting and re-check before tuning anything else.

Missed or dropped packets reported at the DU point at `ofh_cpus` in the same way, even when the ProtO-RU logs stay
clean. The uplink the DU is missing is built, compressed, and transmitted by the `ru_emu_#N` and `ru_ofh_tx_#N`
threads, so starving that pool leaves gaps on the DU side that look like a network fault. Check the OFH thread
utilization before suspecting the link.

Two things to keep in mind while reading the utilization:

- Under DPDK, `ru_rx_#N` polls the NIC in a loop and sits near 100% whether or not traffic is arriving, so its
  utilization on its own proves nothing. Under AF_PACKET the same thread blocks in `recvmsg` and its utilization does
  track the real load. The reliable check for `ofh_cpus` is the thread count: every cell runs three real-time OFH
  threads, so a mask with fewer than three CPUs per cell forces them to share and reports late packets at any load.
- The DPDK EAL cores from `eal_args: -l ...` do no Open Fronthaul work. Packet processing runs on the `ofh_cpus`
  threads listed above, so EAL cores must not be counted towards the OFH budget, and they should not overlap it.

Use the following steps to address timing problems:

1. Run ProtO-RU on a dedicated machine to avoid CPU contention with other applications.
1. Use `taskset` to separate the CPU cores assigned to the DU and ProtO-RU when they run on the same machine.
1. Confirm that the `ptp4l` RMS value and `phc2sys` offset remain stable without large spikes while ProtO-RU runs.
1. Pin `ptp4l` and `phc2sys` to a CPU core that ProtO-RU does not use if their measurements are unstable.
1. Run the CPU in performance mode with idle states disabled, and use the real-time kernel required by [Hardware Requirements](./HW_REQUIREMENTS.md).
1. Add CPU cores to `sdr.ru_cpus` for low-PHY failures, and use the `auto`, `dual`, or `triple` execution profile as appropriate for the host.
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
   The examples use the O-DU setting `max_proc_delay: 10` for OCUDU and `sl_ahead = 10` for OAI.
   Do not confuse the O-DU setting with ProtO-RU's `sdr.max_proc_delay`, which controls lower-PHY processing depth and defaults to 2.
1. Tune the DU downlink IQ scaling through `iq_scaling` for OCUDU or `tx_amp_backoff_dB` for OAI.
1. Tune the ProtO-RU USRP gains through `tx_gain` and `rx_gain`.
1. Adjust the ProtO-RU `iq_scaling` value when uplink IQ samples require additional scaling.
1. Tune DU power parameters such as the PRACH preamble receive power, P0 nominal, and target SINR values.
1. Increase the number of LDPC decoding iterations when the DU uses a hardware accelerator such as ACC100.
