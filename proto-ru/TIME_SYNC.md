# Time Synchronization
The split 7.2 interface requires tight timing synchronization between the DU and RU. O-RAN WG 4 has defined various synchronization methods for use with Open Fronthaul. These are outlined in O-RAN.WG4.CUS.0-R003-v11.00 Section 11.

<br>

## Clocking & Synchronization
PTP synchronization can be established using tools like ptp4l and phc2sys.
Depending on your setup, PTP components can be deployed in different configurations. The most common ones are LLS-C1 and LLS-C3, which can use either unicast or multicast transmission.

* In the LLS-C1 configuration, the DU server drives PTP synchronization, and the RU acts as a client. The RU receives PTP messages from the DU.
* In the LLS-C3 configuration, both the DU and RU are clients receiving PTP messages from a common PTP grandmaster.	

In this tutorial, we demonstrate how to deploy **LLS-C1** configurations using the G.8275.1 multicast profile of linuxptp. For more information, refer to the [official linuxptp documentation](https://linuxptp.nwtime.org/documentation/).

<br>

## Set Up PTP Synchronization
PTP is used to synchronize the DU with the fronthaul switch. The PTP process should be running on the DU and the PTP sync should be checked to ensure it is synchronized correctly.

ptp4l and phc2sys are the two main components of the PTP synchronization with linuxptp. To avoid any issues with the PTP, it is recommended to disable the Network Time Protocol (NTP) on the DU. 

1. Use the following command to disable NTP on Ubuntu 22.04:

        sudo timedatectl set-ntp false

2. We have created an example configuration file for G.8275.1 Multicast PTP profile for LinuxPTP version 4. Sample configuration for ptp-gm and ptp-slave can be downloaded [here](/ptp_conf_files/).

    **LLS-C1 example configuration:**
    ~~~
    [global]
        dataset_comparison		        G.8275.x
        G.8275.defaultDS.localPriority	128
        maxStepsRemoved			        255
        logAnnounceInterval		        -3
        logSyncInterval			        -4
        logMinDelayReqInterval		    -4
        serverOnly			            0
        G.8275.portDS.localPriority	    128
        ptp_dst_mac			            01:80:C2:00:00:0E
        network_transport		        L2
        domainNumber			        24
    ~~~

3. Install the LinuxPTP v4 using the following command:

        git clone http://git.code.sf.net/p/linuxptp/code linuxptp
        cd linuxptp/
        git checkout v4.4
        make
        sudo make install

4. Configure ptp4l
    ##### 4.1 Configure the DU as PTP-GM using the following command:

        sudo ptp4l -f ptp-gm.cfg -i ens1f0 -m -H -2

    You should then see the following output:

        ptp4l[1078213.818]: selected /dev/ptp0 as PTP clock
        ptp4l[1078213.845]: port 1 (ens1f0): INITIALIZING to LISTENING on INIT_COMPLETE
        ptp4l[1078213.907]: port 0 (/var/run/ptp4l): INITIALIZING to LISTENING on INIT_COMPLETE
        ptp4l[1078213.907]: port 0 (/var/run/ptp4lro): INITIALIZING to LISTENING on INIT_COMPLETE
        ptp4l[1078213.925]: selected local clock 507c6f.fffe.55c6ac as best master
        ptp4l[1078213.925]: port 4 (ens1f0): assuming the grand master role

    In the above output, the configured interface should get into master role.

    ##### 4.2 Configure the RU as PTP-SLAVE using the following command:
   
        sudo ptp4l -i ens1f0 -f ptp-slave.cfg -m -H -s
   
    You should then see the following output:
   
        ptp4l[865401.032]: rms    6 max   13 freq     -7 +/-   8 delay     7 +/-   1
        ptp4l[865402.033]: rms    8 max   14 freq     +6 +/-   8 delay     8 +/-   1
        ptp4l[865403.033]: rms    4 max    8 freq     +4 +/-   5 delay     8 +/-   1
        ptp4l[865404.033]: rms    5 max   10 freq     -1 +/-   8 delay     8 +/-   1

   In the above output, the rms value can be used to determine if the PTP sync is correct, for this we look for a value < 30.

   In both of the above commands *ens1f0* is the network interface on our DU that gets the PTP sync.

<br>

5. Next, configure phc2sys:
    ##### 5.1 Configure phc2sys to synchronize the server's system clock (CLOCK_REALTIME) to the PTP hardware clock (ens1f0's PHC). 
   
        sudo ./phc2sys -s ens6f2np2 -w -m -r -r -n 24

    You should then see the following output:

        phc2sys[864348.303]: CLOCK_REALTIME phc offset       -25 s2 freq   +11026 delay   567
        phc2sys[864348.428]: CLOCK_REALTIME phc offset       -13 s2 freq   +11033 delay   566
        phc2sys[864348.553]: CLOCK_REALTIME phc offset       -28 s2 freq   +11016 delay   496
        phc2sys[864348.678]: CLOCK_REALTIME phc offset        -5 s2 freq   +11028 delay   497

   
    ##### 5.2 Configure the RU to synchronize its system clock with its local PHC.
   
        sudo phc2sys -c CLOCK_REALTIME -s /dev/ptp2 -w -m -r -r -n 24
   
    You should then see the following output:
   
        phc2sys[907589.359]: CLOCK_REALTIME phc offset       -20 s2 freq  -12026 delay    670
        phc2sys[907590.359]: CLOCK_REALTIME phc offset       -19 s2 freq  -12031 delay    675
        phc2sys[907591.359]: CLOCK_REALTIME phc offset        -9 s2 freq  -12026 delay    676
        phc2sys[907592.360]: CLOCK_REALTIME phc offset        -5 s2 freq  -12025 delay    677
   
   The first value here is used to determine if the PTP sync is correct, for this we look for a value in the range of -100 to 100.

