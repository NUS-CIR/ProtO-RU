<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Example Configuration Files

This directory contains common ProtO-RU configurations for the current application schema, plus separate O-DU configurations for OCUDU and OAI.
Each ProtO-RU file includes a UHD `sdr` block and uses short PRACH format B4.
ZMQ examples are intentionally excluded because this release does not support ZMQ operation.

The examples are starting points.
Update the Ethernet interface, RU and DU MAC addresses, VLAN ID, gains, CPU sets, and UHD device arguments for your testbed.

Every configuration here has been tested end to end and brings a UE into service on the testbed it was captured from.
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
It uses the same matching OCUDU or OAI O-DU configuration shown above; only the RU Ethernet backend differs.

The matching O-DU configuration must use the same bandwidth, SCS, ARFCN, antenna/eAxC lists, compression settings, MAC addresses, and VLAN.
The O-DU T1a windows should match the RU T2a windows, and the O-DU Ta4 window must contain the RU Ta3 window.

The stock OCUDU O-DU rejects some delay values used by these samples.
Apply the range adjustment in [Integration Notes](../INTEGRATION_NOTES.md) before starting an OCUDU O-DU with a matching configuration.

See the [Configuration Reference](../CONFIG_REFERENCE.md) for every available key.
