<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Hardware Requirements

ProtO-RU needs the following resources for reliable real-time operation.

- **CPU**: A modern processor with at least four CPU cores running at 2.0 GHz or faster is *required*.
  Higher core counts are necessary to support larger bandwidths.
- **OS / Kernel**: A real-time kernel on the RU host is *required* for stable real-time operation.
  The CPU should also run in performance mode with idle states disabled (see [Troubleshooting](./TROUBLESHOOTING.md)).
- **Memory**: A minimum of 8 GB RAM is recommended.
- **Network Interface**: A 10GbE Ethernet interface is *required*.
  The NIC must also support hardware timestamping for PTP.
- **SDR**: A compatible SDR device is *required*.
  This release supports NI USRPs through the OCUDU UHD radio driver.
    - ProtO-RU has been validated with configurations up to 2x2 MIMO and 100 MHz bandwidth.
    - Other SDRs, such as the BladeRF, may work but have not been validated; see this [discussion](https://github.com/srsran/srsRAN_Project/discussions/222#discussioncomment-7120314).
