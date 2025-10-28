# Time Synchronization
The split 7.2 interface requires tight timing synchronization between the DU and RU. 
O-RAN WG 4 has defined various synchronization methods for use with Open Fronthaul. 
These are outlined in O-RAN.WG4.CUS.0-R003-v11.00 Section 11.

## Clocking & Synchronization
PTP synchronization can be established using tools like `ptp4l` and `phc2sys`.
Depending on your setup, PTP components can be deployed in different configurations. 
The most common ones are LLS-C1 and LLS-C3, which can use either unicast or multicast transmission.

In the LLS-C1 configuration, the DU server drives PTP synchronization, and the RU acts as a client. 
The RU receives PTP messages from the DU. 
As for the LLS-C3 configuration, both the DU and RU are clients receiving PTP messages from a common PTP grandmaster (usually from a fronthaul switch).	

In this tutorial, we demonstrate how to deploy **LLS-C1** configurations using the G.8275.1 multicast profile of `linuxptp`. 
For more information, refer to the official linuxptp [documentation](https://linuxptp.nwtime.org/documentation/).

## Setting Up PTP Synchronization (LLS-C1)

In the context of ProtO-RU, the DU is configured as the PTP Grandmaster, while the RU is set up as the PTP Slave. 
This corresponds to the LLS-C1 configuration.

We have created an example configuration file for G.8275.1 Multicast PTP profile for LinuxPTP version 4. 
The sample configurations for the ptp-gm and ptp-slave can be found [here](/ptp_conf_files/).

### Prerequisites
First, we need to disable NTP on the DU and RU hosts.
Using Ubuntu 22.04 as an example:
```bash
sudo timedatectl set-ntp false
```

Then, we need to install LinuxPTP on both the DU and RU hosts.
```bash
git clone http://git.code.sf.net/p/linuxptp/code linuxptp
cd linuxptp/
git checkout v4.4
make
sudo make install
```

### ptp4l 

First, we will setup `ptp4l` on both the DU and RU hosts to handle PTP message exchanges.

#### PTP Grandmaster (DU host)

Next, we will setup the PTP grandmaster on the DU host.
We will be using the example configuration file `ptp-gm.cfg`.

```bash
sudo ./ptp4l -i $INTERFACE_TO_RU_HOST -f /path/to/ptp-gm.cfg -m -H -2
```

You should then see the following output:
```bash
ptp4l[1078213.818]: selected /dev/ptp0 as PTP clock
ptp4l[1078213.845]: port 1 (ens1f0): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[1078213.907]: port 0 (/var/run/ptp4l): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[1078213.907]: port 0 (/var/run/ptp4lro): INITIALIZING to LISTENING on INIT_COMPLETE
ptp4l[1078213.925]: selected local clock 507c6f.fffe.55c6ac as best master
ptp4l[1078213.925]: port 4 (ens1f0): assuming the grand master role
```

In the above output, the configured interface (in this example, `ens1f0`) should get into master role.

#### PTP Slave (RU host)

Finally, we will setup the PTP slave on the RU host.
We will be using the example configuration file `ptp-slave.cfg`.

```bash
sudo ./ptp4l -i $INTERFACE_TO_DU_HOST -f /path/to/ptp-slave.cfg -m -H -s
```

You should then see the following output:

```bash
ptp4l[865401.032]: rms    6 max   13 freq     -7 +/-   8 delay     7 +/-   1
ptp4l[865402.033]: rms    8 max   14 freq     +6 +/-   8 delay     8 +/-   1
ptp4l[865403.033]: rms    4 max    8 freq     +4 +/-   5 delay     8 +/-   1
ptp4l[865404.033]: rms    5 max   10 freq     -1 +/-   8 delay     8 +/-   1
```

In the above output, the rms value can be used to determine if the PTP synchronization is working fine or not, for this we generally look for a value < 30.

### phc2sys

Next, we will setup phc2sys on both the DU and RU hosts to synchronize the system clock with the PTP hardware clock (PHC).

#### PTP Grandmaster (DU host)

Configure `phc2sys` to synchronize the server's system clock (CLOCK_REALTIME) to the PTP hardware clock (PHC). 
```bash   
sudo ./phc2sys -s $INTERFACE_TO_RU_HOST -w -m -r -r -n 24
```

You should then see the following output:
```bash
phc2sys[864348.303]: CLOCK_REALTIME phc offset       -25 s2 freq   +11026 delay   567
phc2sys[864348.428]: CLOCK_REALTIME phc offset       -13 s2 freq   +11033 delay   566
phc2sys[864348.553]: CLOCK_REALTIME phc offset       -28 s2 freq   +11016 delay   496
phc2sys[864348.678]: CLOCK_REALTIME phc offset        -5 s2 freq   +11028 delay   497
```

The phc offset is used to determine if the system clock is synchronized with the PTP hardware clock, for this we generally look for a value in the range of -100 to 100.

#### PTP Slave (RU host)
   
Configure the RU to synchronize its system clock with its local PHC.
```bash   
sudo ./phc2sys -s $INTERFACE_TO_DU_HOST -w -m -r -r -n 24
```
   
You should then see the following output:
```bash   
phc2sys[907589.359]: CLOCK_REALTIME phc offset       -20 s2 freq  -12026 delay    670
phc2sys[907590.359]: CLOCK_REALTIME phc offset       -19 s2 freq  -12031 delay    675
phc2sys[907591.359]: CLOCK_REALTIME phc offset        -9 s2 freq  -12026 delay    676
phc2sys[907592.360]: CLOCK_REALTIME phc offset        -5 s2 freq  -12025 delay    677
```

The phc offset is used to determine if the system clock is synchronized with the PTP hardware clock, for this we generally look for a value in the range of -100 to 100.

