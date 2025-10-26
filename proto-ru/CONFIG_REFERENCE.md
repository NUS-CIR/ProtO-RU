# Configuration Reference

This document provides a comprehensive reference for the configuration options available for ProtO-RU.

Following the convention of srsRAN, ProtO-RU uses YAML files for configuration. 
The configuration files are structured into sections, each corresponding to a specific component or functionality of the ProtO-RU system.

All configuration parameters are presented here below in the following format:

```yaml
parameter: default_value         # Optional/Required TYPE (default value). Parameter description. Format: <format description> OR Supported: <supported values>.
```

ProtO-RU configuration:

```yaml
log:
    filename: /tmp/ru_emu.log                       # Optional STRING (/tmp/ru_emu.log). File path for logs. Logs can be redirected to stdout by setting the filename to "stdout".
    level: warning                                  # Optional STRING (warning). Logging level. Supported: trace, debug, info, warning, error, critical, off.

ru_emu:
    cells:
    - bandwidth: 40                                 # Required UINT (0). Sets the channel bandwidth in MHz. Supported: [5,10,15,20,25,30,40,50,60,70,80,90,100].
      dl_arfcn: 628032                              # Required UINT (536020). Sets the Downlink ARFCN (center frequency). 
      common_scs: 30                                # Required UINT (15). Sets the subcarrier spacing in KHz to be used by the RU. Supported: [15, 30].
      band: 78                                      # Optional TEXT (auto). Sets the NR band being used for the RU. If not specified, will be set automatically based on ARFCN. Supported: all release 17 bands.
      network_interface: enp1s0f0                   # Optional TEXT (enp1s0f0). Sets the ethernet network interface name for the RU or PCIe identifier when using DPDK. Format: a string, e.g. [interface_name].
      enable_promiscuous: true                      # Optional BOOL (false). Enables promiscuous mode.
      ru_mac_addr: 50:7c:6f:45:f3:07                # Optional TEXT (70:b3:d5:e1:5b:06). Sets the RU MAC address. Format: a string, e.g. [AA:BB:CC:DD:11:22:33].
      du_mac_addr: 50:7c:6f:45:f3:06                # Optional TEXT (70:b3:d5:e1:5b:06). Sets the DU MAC address. Format: a string, e.g. [AA:BB:CC:DD:11:22:33].
      vlan_tag: 5                                   # Optional UINT (1). Sets the VLAN tag value for both U-plane and C-Plane.
      dl_port_id: [0]                               # Optional UINT (0, 1). Sets the RU downlink eAxC port ID. Format: vector containing all DL eXaC ports, e.g. [0, ...\ , N].
      ul_port_id: [0]                               # Optional UINT (0). Sets the RU uplink eAxC port ID. Format: vector containing all UL eXaC ports, e.g. [0, ...\ , N].
      prach_port_id: [4]                            # Optional UINT (4). Sets the RU PRACH eAxC port ID. Format: vector containing all PRACH eXaC ports, e.g. [0, ...\ , N].
      nof_antennas_dl: 1                            # Optional UINT (1). Sets the number of antennas for downlink transmission. Supported: [1, 2, 4].
      nof_antennas_ul: 1                            # Optional UINT (1). Sets the number of antennas for uplink transmission. Supported: [1, 2, 4].
      compr_method_dl: bfp                          # Optional TEXT (bfp). Sets the downlink compression method. Supported: [none, bfp].
      compr_bitwidth_dl: 9                          # Optional UINT (9). Sets the downlink compression bit width. Supported: [1 - 16].
      compr_method_ul: bfp                          # Optional TEXT (bfp). Sets the uplink compression method. Supported: [none, bfp].
      compr_bitwidth_ul: 9                          # Optional UINT (9). Sets the uplink compression bit width. Supported: [1 - 16].
      enable_ul_static_compr_hdr: true              # Optional BOOLEAN (true). Uplink static compression header enabled flag. Supported: [false, true].
      enable_dl_static_compr_hdr: true              # Optional BOOLEAN (true). Downlink static compression header enabled flag. Supported: [false, true].
      iq_scaling: 1.0                               # Optional FLOAT (0.35). Sets the IQ scaling factor. Supported: [0 - 20].
      t2a_max_cp_dl: 2635                           # Optional INT (2635). Sets T2a maximum value for downlink control-plane. Supported: [0 - 5000].
      t2a_min_cp_dl: 2221                           # Optional INT (2221). Sets T2a minimum value for downlink control-plane. Supported: [0 - 5000].
      t2a_max_cp_ul: 2635                           # Optional INT (2635). Sets T2a maximum value for uplink control-plane. Supported: [0 - 5000].
      t2a_min_cp_ul: 2221                           # Optional INT (2221). Sets T2a minimum value for uplink control-plane. Supported: [0 - 5000].
      t2a_max_up: 2454                              # Optional INT (2454). Sets T2a maximum value for user-plane. Supported: [0 - 5000].
      t2a_min_up: 2015                              # Optional INT (2015). Sets T2a minimum value for user-plane. Supported: [0 - 5000].
      ta3_max_up: 1480                              # Optional INT (1480). Sets Ta3 maximum value for uplink user-plane. Supported: [0 - 5000].
      ta3_min_up: 1125                              # Optional INT (1125). Sets Ta3 minimum value for uplink user-plane. Supported: [0 - 5000].

radio:
  srate: 61.44                                      # Required FLOAT (61.44). Sets the sampling rate of the RF-frontend in MHz. 
  device_driver: uhd                                # Required TEXT (uhd). RF device driver name. Supported: [uhd].
  device_args:                                      # Optional TEXT. An argument that gets passed to the selected RF driver.
  tx_gain: 50                                       # Required FLOAT (50). Sets the transmit gain in dB. Supported: [0 - max value supported by radio].
  rx_gain: 60                                       # Required FLOAT (60). Sets the receive gain in dB. Supported: [0 - max value supported by radio]. 
  amplitude_control:
    tx_gain_backoff: 12                             # Optional FLOAT (12.0). Sets baseband gain back-off in dB. This accounts for the signal PAPR and is applied regardless of clipping settings. Format: positive float.
    enable_clipping: true                           # Optional BOOLEAN (false). Sets clipping of the baseband samples on or off. If enabled, samples that exceed the power ceiling are clipped.
    ceiling: -0.1                                   # Optional FLOAT (-0.1). Sets the power ceiling in dB, relative to the full scale amplitude of the radio. Format: negative float or 0.

expert_execution:
  cell_affinities:                                  # Optional TEXT. Sets the cell CPU affinities configuration on a per cell basis. Entry order is the same as the order in the defined cell list.
    -                           
      ru_cpus:        5,6,7,8,9,10,11,12,13,14      # Optional TEXT. Sets the CPU core(s) used for the Radio Unit tasks. Supported: [1, 2, 3 , ..., N].
      ofh_rx_cpus:    1,2                           # Optional TEXT. Sets the CPU core(s) used for the OFH RX tasks. Supported: [1, 2, 3 , ..., N].
      ru_timing_cpus: 3,4                           # Optional TEXT. Sets the CPU core(s) used for the RU Timing tasks. Supported: [1, 2, 3 , ..., N].
  affinities:
    isolated_cpus:      1,2,3,4,5,6,7,8,9,10,11,12,13,14,15     # Optional TEXT. Sets the CPU core(s) isolated for the gNB application. Supported: [1, 2, 3 , ..., N].
    low_priority_cpus:  15                                      # Optional TEXT. Sets the CPU core(s) assigned to low priority tasks. Supported: [1, 2, 3 , ..., N].
  
  threads:                                          
    lower_phy:
      execution_profile: quad                       # Optional TEXT (quad). Sets the lower physical layer executor profile. Supported: [single, dual, quad].