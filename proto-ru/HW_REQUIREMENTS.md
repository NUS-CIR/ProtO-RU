<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Hardware Requirements

ProtO-RU needs the following resources for reliable real-time operation.

- **CPU**: A modern processor with at least four CPU cores running at 2.0 GHz or faster is *required*.
  Higher core counts are necessary to support larger bandwidths. The supplied CPU layout uses eight logical CPU
  IDs; adapt all masks for smaller hosts and validate capacity under load.
  Compute uses `phy_cpus`/`nof_phy_threads`, separately from OFH, timing, and baseband transport.
- **OS / Kernel**: A real-time kernel on the RU host is *required* for stable real-time operation.
  The CPU should also run in performance mode with idle states disabled (see [Troubleshooting](./TROUBLESHOOTING.md)).
- **Memory**: A minimum of 8 GB RAM is recommended.
- **Network Interface**: A 10GbE Ethernet interface is *required*.
  The NIC must also support hardware timestamping for PTP.
- **SDR**: A compatible SDR device is *required*.
  This release supports NI USRPs through the OCUDU UHD radio driver.
    - The original `proto-ru` branch reported validation up to 2x2 MIMO and 100 MHz bandwidth.
      The rebased worker model and amplitude settings need renewed RF validation.
    - Other SDR families need a compatible in-tree radio backend; the supplied profiles target UHD.
