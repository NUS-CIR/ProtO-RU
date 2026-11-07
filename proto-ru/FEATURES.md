<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Features

The current implementation of ProtO-RU aligns with the key requirements of a Category A O-RAN Radio Unit supporting split 7.2.

The hardware coverage below was reported for the original `proto-ru` branch. This port follows the current
upstream lower-PHY APIs and shared compute model; its automated checks do not replace renewed RF validation.

## Current Features

- UHD support for NI USRPs such as the B200 mini, B210, N310, and X410
- TDD and FDD support
- BFP compression and decompression
- 15kHz and 30kHz SCS support
- Short PRACH preamble (limited to format B4)
- Per-slot C-plane packet handling
- MIMO support validated up to 2x2 with 100 MHz bandwidth
- Shared lower-PHY compute pool with separate timing, OFH, and baseband CPU settings
- Real-time KPI monitoring
- AF_PACKET and DPDK fronthaul

ProtO-RU plans to add Category B support after the required functionality becomes available in upstream OCUDU.
See the [OCUDU releases and roadmap](https://docs.ocudu.org/releases/) for upstream plans.
