<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Example Configuration Files

This directory contains common ProtO-RU configurations for the current application schema, plus separate O-DU configurations for OCUDU and OAI.
Each ProtO-RU file includes a UHD `sdr` block and uses short PRACH format B4.
ZMQ is recognized by the source but has no validated end-to-end profile in this port; these examples use UHD.

The examples are starting points.
Update the Ethernet interface, RU and DU MAC addresses, VLAN ID, gains, CPU sets, and UHD device arguments for your testbed.

These profiles originate from end-to-end testbeds on the original `proto-ru` branch.
The upstream port updates their schema and worker layout; those changes need renewed RF validation before claiming equivalent hardware performance.
The values are not tuned for peak RF performance, and the radio-facing settings in particular are host and deployment
specific: expect to tune gains, `iq_scaling`, power levels, and the DTX and SNR thresholds for the best throughput and
BLER on your own hardware.

## Included configuration pairs

| ProtO-RU | OCUDU O-DU | OAI O-DU |
| --- | --- | --- |
| `protoru-B210-FDD-n3-10MHz-1x1-15kHz.yml` | `gnb-ocudu-B210-FDD-n3-10MHz-1x1-15kHz.yml` | Not provided |
| `protoru-B210-FDD-n3-10MHz-2x2-15kHz.yml` | `gnb-ocudu-B210-FDD-n3-10MHz-2x2-15kHz.yml` | Not provided |
| `protoru-B210-TDD-n78-20MHz-2x2-30kHz.yml` | `gnb-ocudu-B210-TDD-n78-20MHz-2x2-30kHz.yml` | `gnb-oai-B210-TDD-n78-20MHz-2x2-30kHz.conf` |
| `protoru-B210-TDD-n78-40MHz-1x1-30kHz.yml` | `gnb-ocudu-B210-TDD-n78-40MHz-1x1-30kHz.yml` | `gnb-oai-B210-TDD-n78-40MHz-1x1-30kHz.conf` |

The same ProtO-RU file is used with either O-DU.
Only the O-DU configuration is implementation-specific.

`protoru-B210-TDD-n78-40MHz-1x1-30kHz-dpdk.yml` is a DPDK variant of the 40 MHz profile.
Replace its sample PCIe address in both `dpdk.eal_args` and `network_interface`, then adjust its CPU sets for the host.
It uses the same radio and timing profile as the matching OCUDU or OAI O-DU configuration shown above.
Its example assumes a hardware port VLAN on the RU VF: keep the DU's software VLAN settings only if the DU port
does not already insert that VLAN itself. MAC addresses must match the actual VF configuration.

The matching O-DU configuration must use the same bandwidth, SCS, ARFCN, antenna/eAxC lists, compression settings, MAC addresses, and VLAN.
The O-DU T1a transmit range must fit inside the RU T2a receive range with network margin, and the O-DU Ta4 window
must contain the RU Ta3 window. The supplied receive windows are deliberately wider than the transmit ranges.

This branch includes the larger OCUDU timing range required by the samples; no further range patch is needed here.
Use the `gnb_split_7_2` target. See [Integration Notes](../INTEGRATION_NOTES.md) when building a separate O-DU checkout.

See the [Configuration Reference](../CONFIG_REFERENCE.md) for every available key.

## CPU layout and upstream schema

The RU examples use the eight-CPU layout documented in [Architecture](../ARCHITECTURE.md#4-worker--cpu-mapping):

| Setting | CPU IDs | Work |
| --- | --- | --- |
| `timing_cpus` | 1 | Shared OTA timing |
| `ofh_cpus` | 2, 3 | Per-cell Ethernet receive, OFH decode, and OFH transmit |
| `sdr.ru_cpus` | 4, 5 | Radio events and baseband TX/RX/UL workers, with round-robin pinning |
| `phy_cpus` | 6, 7 | Two compute workers shared across non-sequential SDR cells |
| DPDK EAL `-l` | 0 | EAL main lcore, in the DPDK example only |

These logical CPU IDs illustrate placement, not a throughput guarantee. Adjust the sets and `nof_phy_threads`
for the host and load; a busy OFH or MIMO deployment may need more CPUs. The DU needs additional capacity if it
shares the host. The CPU masks do not reserve cores or pin driver/background threads.

The former clipping keys are removed to match the current upstream lower PHY. `gain_backoff` remains available,
but the former clipping behavior is not reproduced; recheck RF amplitude settings on hardware.

## Run a pair

From the repository root, after adapting the Ethernet and core-network addresses:

```bash
./build/apps/gnb_split_7_2/gnb --dryrun -c proto-ru/conf-files/gnb-ocudu-B210-TDD-n78-20MHz-2x2-30kHz.yml
sudo ./build/apps/gnb_split_7_2/gnb -c proto-ru/conf-files/gnb-ocudu-B210-TDD-n78-20MHz-2x2-30kHz.yml
```

On the RU host, run:

```bash
sudo ./build/apps/examples/ofh/ru_emulator -c proto-ru/conf-files/protoru-B210-TDD-n78-20MHz-2x2-30kHz.yml
```

The dry run parses the O-DU configuration; it does not validate hardware or RF behavior.
