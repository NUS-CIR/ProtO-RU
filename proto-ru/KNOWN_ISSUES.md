# Known Issues

This document lists known issues and limitations in ProtO-RU.

1. T2a/Ta3 value validation
    - Currently, ProtO-RU does not validate the T2a/Ta3 values configured in the RU configuration file.
    - Users must ensure that the T2a/Ta3 values are sufficiently large to provide enough time for the RU to process and transmit the data within the required deadlines.

1. 1/ 2.5 GbE NICs
    - Technically, ProtO-RU can run on hosts with 1/ 2.5 GbE NICs: one can either split the C/U-plane and S-plane traffic across two 1/2.5GbE NICs or by sharing a single 1/2.5 GbE NIC, while running at a smaller bandwidth/ with fewer antennas.
    - However, our testings have found that when using systems with 1/ 2.5 GbE NICs (e.g., Intel I225-V, I210), the performance is not stable -- the UE can attach and communicate with the gNB/DU for a while (e.g., less than 30 seconds), before the DU/gNB reports a large number of missed/dropped packets at the OFH and eventually the connection drops.
    - To that end, we strongly recommend using at least 10 GbE NICs for ProtO-RU hosts for now; if you manage to run ProtO-RU stably on 1/ 2.5 GbE NICs, please let us know!

1. USRP clock drift over long runs
    - By default the USRP runs off its free-running onboard oscillator (`clock: internal`, `sync: internal`). Over long, continuous operation this oscillator drifts relative to the PTP-synchronized host clock; the drift accumulates until the fronthaul timing windows can no longer be met, at which point ProtO-RU crashes. The only remedy once this happens is to restart ProtO-RU.
    - For stable long-running operation, discipline the USRP from an external time/frequency reference instead -- e.g., an external 10 MHz reference and PPS (`clock: external`, `sync: external`), or an onboard GPSDO (`clock: gpsdo`, `sync: gpsdo`). This requires a USRP with the corresponding reference inputs. See [Time Synchronization](TIME_SYNC.md) for details.