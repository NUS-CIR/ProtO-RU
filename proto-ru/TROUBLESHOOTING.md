# Troubleshooting

Below, we provide some common troubleshooting steps.

## Network-related Issues

If you are not receiving any packets at ProtO-RU from the DU (or vice versa), please check the following:
1. ensure that both network interfaces are up.
1. ensure that the DU/RU MAC addresses, network interface name, VLAN tags are correctly specified at both ends.
1. consider enabling promiscuous mode by specifying the option `enable_promiscuous: true` at the DU/ ProtO-RU.

## Timing Issues

If you observe the following symptoms, it is likely that your system is experiencing timing issues:
1. missed/dropped OFH packets at the DU, 
1. warnings other than `Invalid resource grid for slot XX.YY` (this one is OK), at ProtO-RU logs,  such as:
    - `Error: exceeded maximum number of timed out transmissions.` 
    - `Real-time failure in low-phy: PUxCH request late for sector`
1. LATE packet arrivals from the DU (EARLY packets are OK) at ProtO-R as shown in the KPI output

To troubleshoot the above timing issues, please attempt the following steps:
1. ensure that PTP is working properly and the ptp4l rms/ phc offset is stable, i.e., no huge spikes when ProtO-RU is running; if so, you should considering pinning `ptp4l` and `phc2sys` to a CPU core not used by ProtO-RU.
1. ensure that your CPU is running at performance mode, and with idle states disabled for maximum responsiveness; having a real-time kernel is optional, but recommended.
1. for realtime failures, considering adding more CPU cores for low PHY processing through the parameter `ru_cpus`; if issues persist, consider changing the low PHY's execution profile from `single` to  `quad`.
1. for missing/late packet arrivals at the DU, consider adding more CPU cores for uplink packet processing through the parameter `timing_cpus`
1. for late packet arrivals at the DU, consider raising the Ta4 values at the DU.
1. if issues persists, consider reducing the bandwidth as well as the USRP's sampling rate.

Note that when tuning the CPUs, the set of CPUs that you use should be added to the list of `isolated_cpus` in the config file.

## UE Attachment/ Performance Issues

If there are no timing issues, if the UE can detect the network but is unable to attach or the UE can attach but is experiencing poor throughput performance/high BLERs, try the following steps:
1. make sure that the scheduling advancement at the MAC is larger than the T1a values used. for example, we always use `max_proc_delay` = 10 for srsRAN and `sl_ahead` = 10 for OAI.
1. at the DU, try tuning the downlink IQ scaling parameter, i.e., `iq_scaling` for srsRAN and `tx_amp_backoff_dB` for OAI.
1. at ProtO-RU, try tuning the USRP's gain through the parameter `tx_gain` and `rx_gain`; optionally, the parameter `iq_scaling` can also be tuned at ProtO-RU to scale the uplink IQ samples.
1. consider tuning power-related parameters at the DU, such as PRACH preamble RX power, P0 nominal, target SINR values etc.
1. if you are using any hardware accelerators (e.g., ACC100) at the DU, consider raising the number of LDPC decoding iterations.


