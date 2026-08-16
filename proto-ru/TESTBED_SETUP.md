<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Testbed Setup

ProtO-RU has been validated with the following logical testbed:

```
gNB Host                          ProtO-RU Host
+----------------+               +----------------+
|  OCUDU / OAI   |   Ethernet    |    ProtO-RU    |
|      gNB       |<------------->|                |
+----------------+               +----------------+
                                         |
                                         |
                                SDR (e.g., USRP B210)
```

The gNB and ProtO-RU can run on the same machine or on separate machines connected through Ethernet.

1. Synchronize the clocks with PTP when the gNB and ProtO-RU run on separate machines.
1. Isolate the CPU cores assigned to each application when the gNB and ProtO-RU run on the same machine.

Use a 10GbE or faster Ethernet connection between the gNB and ProtO-RU to provide sufficient bandwidth and low latency.
The validated setup carries all CUS-plane traffic over the shared Ethernet link.
A separate link may carry S-plane traffic.
See [Hardware Requirements](./HW_REQUIREMENTS.md) for the resources required to run ProtO-RU reliably.
