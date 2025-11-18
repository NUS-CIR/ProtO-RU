# Testbed Setup

ProtO-RU is validated in a (logical) testbed setup as shown below:

```
gNB Host                          ProtO-RU Host
+----------------+               +----------------+
|  srsRAN / OAI  |   Ethernet    |    ProtO-RU    |
|      gNB       |<------------->|                |
+----------------+               +----------------+
                                         |
                                         |
                                SDR (e.g., USRP B210)
```

In practice, the gNB host and ProtO-RU host can be the same machine or different machines connected via Ethernet (we have used both).
If they are on different machines, ensure that both machines have synchronized clocks (i.e., using PTP) to avoid timing issues.
If they are on the same machine, make sure to isolate the CPU cores used by the gNB and ProtO-RU to avoid performance degradation.

The interconnection between the gNB and ProtO-RU is *required* to be at least 10 GbE Ethernet to ensure sufficient bandwidth and low latency. 
In our setup, all CUS-plane traffic between the gNB and ProtO-RU is carried over the shared Ethernet link, it is also possible to use a separate link for S-plane.
Please consult the [HW Requirements](HW_REQUIREMENTS.md) document for more details on hardware requirements to run ProtO-RU reliably.
