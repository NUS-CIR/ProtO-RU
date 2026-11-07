<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Time Synchronization

The split 7.2 interface requires tight timing synchronization between the DU and RU.
O-RAN WG4 defines several synchronization methods for Open Fronthaul in Section 11 of O-RAN.WG4.CUS.0-R003-v11.00.

## Clocking and Synchronization

Use tools such as `ptp4l` and `phc2sys` to establish PTP synchronization.
Deploy the PTP components in an LLS-C1 or LLS-C3 configuration using either unicast or multicast transmission.

In an LLS-C1 configuration, the DU server drives PTP synchronization and the RU acts as a client.
The RU receives PTP messages from the DU.
In an LLS-C3 configuration, both the DU and RU receive PTP messages from a common grandmaster, usually a fronthaul switch.

This guide demonstrates an **LLS-C1** deployment using the G.8275.1 multicast profile of `linuxptp`.
For more information, refer to the official [linuxptp documentation](https://linuxptp.nwtime.org/documentation/).

## Setting Up PTP Synchronization (LLS-C1)

Configure the DU as the PTP grandmaster and the RU as the PTP slave for the LLS-C1 setup.

Use the sample LinuxPTP version 4 configurations for the G.8275.1 multicast profile under [`ptp-conf-files/`](./ptp-conf-files/).

### Prerequisites

Disable NTP on the DU and RU hosts.
For example, run the following command on Ubuntu 22.04:

```bash
sudo timedatectl set-ntp false
```

Install LinuxPTP on both hosts:

```bash
git clone http://git.code.sf.net/p/linuxptp/code linuxptp
cd linuxptp/
git checkout v4.4
make
sudo make install
```

### ptp4l

Set up `ptp4l` on both hosts to exchange PTP messages.

#### PTP Grandmaster (DU host)

Start the PTP grandmaster on the DU host with `ptp-gm.cfg`:

```bash
sudo ./ptp4l -i $INTERFACE_TO_RU_HOST -f /path/to/ptp-gm.cfg -m -H -2
```

The output should resemble:

```bash
ptp4l[1078213.818]: selected /dev/ptp0 as PTP clock
ptp4l[1078213.845]: port 1 (ens1f0): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[1078213.907]: port 0 (/var/run/ptp4l): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[1078213.907]: port 0 (/var/run/ptp4lro): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[1078213.925]: selected local clock 507c6f.fffe.55c6ac as best master
ptp4l[1078213.925]: port 4 (ens1f0): assuming the grand master role
```

Confirm that the configured interface, `ens1f0` in this example, enters the grandmaster role.

#### PTP Slave (RU host)

Start the PTP slave on the RU host with `ptp-slave.cfg`:

```bash
sudo ./ptp4l -i $INTERFACE_TO_DU_HOST -f /path/to/ptp-slave.cfg -m -H -s
```

The output should resemble:

```bash
ptp4l[865401.032]: rms    6 max   13 freq     -7 +/-   8 delay     7 +/-   1
ptp4l[865402.033]: rms    8 max   14 freq     +6 +/-   8 delay     8 +/-   1
ptp4l[865403.033]: rms    4 max    8 freq     +4 +/-   5 delay     8 +/-   1
ptp4l[865404.033]: rms    5 max   10 freq     -1 +/-   8 delay     8 +/-   1
```

Confirm that the `rms` value remains below approximately 30 ns.

### phc2sys

Set up `phc2sys` on both hosts to synchronize the system clock with the PTP hardware clock (PHC).

#### PTP Grandmaster (DU host)

Configure `phc2sys` to synchronize the DU system clock (`CLOCK_REALTIME`) to the PHC:

```bash
sudo ./phc2sys -s $INTERFACE_TO_RU_HOST -w -m -r -r -n 24
```

The output should resemble:

```bash
phc2sys[864348.303]: CLOCK_REALTIME phc offset       -25 s2 freq   +11026 delay   567
phc2sys[864348.428]: CLOCK_REALTIME phc offset       -13 s2 freq   +11033 delay   566
phc2sys[864348.553]: CLOCK_REALTIME phc offset       -28 s2 freq   +11016 delay   496
phc2sys[864348.678]: CLOCK_REALTIME phc offset        -5 s2 freq   +11028 delay   497
```

Confirm that the PHC offset generally remains between -100 ns and 100 ns.

#### PTP Slave (RU host)

Configure the RU to synchronize its system clock with its local PHC:

```bash
sudo ./phc2sys -s $INTERFACE_TO_DU_HOST -w -m -r -r -n 24
```

The output should resemble:

```bash
phc2sys[907589.359]: CLOCK_REALTIME phc offset       -20 s2 freq  -12026 delay    670
phc2sys[907590.359]: CLOCK_REALTIME phc offset       -19 s2 freq  -12031 delay    675
phc2sys[907591.359]: CLOCK_REALTIME phc offset        -9 s2 freq  -12026 delay    676
phc2sys[907592.360]: CLOCK_REALTIME phc offset        -5 s2 freq  -12025 delay    677
```

Confirm that the PHC offset generally remains between -100 ns and 100 ns.

## USRP Clock Reference (Long-Running Stability)

The PTP setup above keeps the DU and RU *host* clocks aligned, but the USRP itself runs off its own onboard oscillator.
With `clock_source: internal` and `sync_source: internal` in a cell's `sdr` section, that oscillator is free-running and drifts relative to the PTP-disciplined host clock.
Over long runs the drift can move traffic outside the fronthaul timing windows.
See [Known Issues](./KNOWN_ISSUES.md).

For stable long-running operation, discipline the USRP from an external time/frequency reference:

- **External 10 MHz + PPS**: Feed a common 10 MHz reference and PPS into the USRP's REF/PPS inputs and set:
  ```yaml
  sdr:
    clock_source: external
    sync_source: external
  ```
- **Onboard GPSDO**: If the USRP is equipped with a GPSDO, set:
  ```yaml
  sdr:
    clock_source: gpsdo
    sync_source: gpsdo
  ```

Use a USRP model with the corresponding reference inputs, such as a B210 with REF IN and PPS IN or an N310 equipped with a GPSDO.
