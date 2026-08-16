<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Configuration Reference

ProtO-RU uses the OCUDU YAML configuration parser.
Unknown keys are rejected.
This reference describes the current ProtO-RU configuration schema.

Use `apps/examples/ofh/ru_emulator_example_config.yml` as the fully annotated template.
The shorter, paired testbed examples are under [`conf-files/`](./conf-files/README.md).

## Top-level structure

```yaml
log:
  filename: /tmp/proto-ru.log
  level: info

ru_emu:
  timing_cpus: [1]
  cells:
    - network_interface: enp1s0f0
      ru_mac_addr: 50:7c:6f:45:f3:07
      du_mac_addr: 50:7c:6f:45:f3:06
      vlan_tag: 5
      bandwidth: 20
      common_scs: 30
      dl_arfcn: 628032
      dl_port_id: [0]
      ul_port_id: [0]
      prach_port_id: [4]
      prach_format: short
      ofh_cpus: [2, 3]
      sdr:
        device_driver: uhd
        device_args: type=b200
        srate: 23.04
        tx_gain: 60
        rx_gain: 60
        ru_cpus: [4, 5]
```

The `sdr` section is per cell.
Include it to run ProtO-RU with a UHD radio.
If the section is absent, the RU emulator runs in its original Ethernet loopback mode using generated test IQ.
ZMQ operation is not supported in this release.

## `log`

| Parameter | Default | Description |
| --- | --- | --- |
| `filename` | `stdout` | Log destination, either `stdout` or a writable file path; the sample profiles use `/tmp/proto-ru.log`. |
| `level` | `info` | Log level: `none`, `error`, `warning`, `info`, or `debug`. |

## `ru_emu`

| Parameter | Default | Description |
| --- | --- | --- |
| `timing_cpus` | unset | CPU set for the GPS/host-clock timing worker shared by all cells. |
| `cells` | required | List of RU cells that must all use the same `common_scs`. |

## `ru_emu.cells[]`: Ethernet and cell parameters

| Parameter | Default | Description |
| --- | --- | --- |
| `network_interface` | required | Kernel interface name for AF_PACKET, or PCIe address when DPDK is enabled. |
| `ru_mac_addr` | required | O-RU source MAC address. |
| `du_mac_addr` | required | O-DU destination MAC address. |
| `vlan_tag` | unset | VLAN ID in the range 1 to 4094, inserted in software. Omit it when the interface tags the traffic itself, such as an SR-IOV VF with a port VLAN, otherwise the frames leave double-tagged. |
| `enable_promiscuous` | `false` | Enable promiscuous mode on the Ethernet interface. |
| `bandwidth` | `100` | Channel bandwidth in MHz: 5 to 100 using valid NR bandwidth steps. |
| `common_scs` | `30` | Common subcarrier spacing in kHz: `15` or `30`. |
| `dl_arfcn` | `632628` | Downlink ARFCN used to derive the radio center frequency. |
| `band` | derived | NR band derived from `dl_arfcn` when omitted. |
| `dl_port_id` | `[0,1,2,3]` | Downlink eAxC IDs whose list length sets the SDR TX antenna count. |
| `ul_port_id` | `[0,1]` | Uplink eAxC IDs whose list length sets the SDR RX antenna count. |
| `prach_port_id` | `[4,5]` | PRACH eAxC IDs. |
| `prach_format` | `long` | `long` for format 0 or `short` for B4, with SDR mode currently requiring `short`. |
| `ofh_cpus` | unset | CPU set for Ethernet receive, OFH decode, and uplink OFH construction/transmission. |

## `ru_emu.cells[]`: compression

| Parameter | Default | Description |
| --- | --- | --- |
| `compr_method_ul` | `bfp` | Uplink U-plane compression: `none` or `bfp`. |
| `compr_bitwidth_ul` | `9` | Uplink IQ bit width: `9` or `16`. |
| `compr_method_dl` | `bfp` | Downlink U-plane compression: `none` or `bfp`. |
| `compr_bitwidth_dl` | `9` | Downlink IQ bit width: `9` or `16`. |
| `compr_method_prach` | `bfp` | PRACH U-plane compression: `none` or `bfp`. |
| `compr_bitwidth_prach` | `9` | PRACH IQ bit width: `9` or `16`. |
| `is_ul_static_compr_hdr` | `true` | Use an out-of-band static compression header for uplink and PRACH. |
| `is_dl_static_compr_hdr` | `true` | Use an out-of-band static compression header for downlink. |
| `iq_scaling` | `1.0` | Scaling applied before uplink compression. |

The compression method, bit width, and static-header settings must match the O-DU.

## `ru_emu.cells[]`: timing windows

All values are in microseconds and accept the range 0 to 5000.

| Parameter | Meaning |
| --- | --- |
| `t2a_max_cp_dl`, `t2a_min_cp_dl` | O-RU receive window for downlink Control-Plane messages. |
| `t2a_max_cp_ul`, `t2a_min_cp_ul` | O-RU receive window for uplink/PRACH Control-Plane messages. |
| `t2a_max_up`, `t2a_min_up` | O-RU receive window for downlink User-Plane messages. |
| `ta3_max_up`, `ta3_min_up` | O-RU transmit window for uplink and PRACH User-Plane messages. |

The O-DU must use matching T1a windows and a Ta4 window that contains the RU's Ta3 window.
The stock OCUDU O-DU limits T1a and Ta4 values to 1960 microseconds.
ProtO-RU's SDR pipeline generally needs larger T1a values, so apply the change in [Integration Notes](./INTEGRATION_NOTES.md).

SDR mode also enforces these lower bounds at startup:

- `t2a_max_cp_dl` must exceed 1 ms plus one slot.
- `t2a_max_cp_ul` must exceed 1 ms plus half a slot.
- `t2a_max_up` must exceed 1 ms plus one slot plus the offset of the final OFDM symbol.

Add processing and network-jitter headroom above these bounds.

## `ru_emu.cells[].sdr`

| Parameter | Default | Description |
| --- | --- | --- |
| `device_driver` | required | Radio driver, which must be `uhd` in this release. |
| `device_args` | unset | UHD device arguments, for example `type=b200`. |
| `srate` | `23.04` | Radio sample rate in MHz, which must cover the configured channel bandwidth. |
| `dl_freq_override` | derived | Optional downlink center-frequency override in Hz. |
| `ul_freq_override` | derived | Optional uplink center-frequency override in Hz. |
| `tx_gain` | `60` | Radio transmit gain in dB. |
| `rx_gain` | `60` | Radio receive gain in dB. |
| `center_freq_offset` | `0` | Center-frequency correction in Hz. |
| `calibrate_clock_ppm` | `0` | Carrier-frequency clock correction in parts per million. |
| `lo_offset` | `0` | Local-oscillator offset in MHz. |
| `time_alignment_calibration` | driver default | RX-to-TX time-alignment correction in samples. |
| `transmission_mode` | `continuous` | `continuous`, `discontinuous`, or `same-port`. |
| `power_ramping` | `0` | Transmit power-ramping duration in microseconds. |
| `gain_backoff` | `12` | Baseband amplitude backoff in dB. |
| `power_ceiling` | `-0.1` | Optional clipping ceiling in dBFS. |
| `enable_clipping` | `false` | Enable amplitude clipping at `power_ceiling`. |
| `otw_format` | `default` | Optional UHD over-the-wire format; omit this field to use the UHD default. |
| `clock_source` | `default` | UHD clock source: `default`, `internal`, `external`, or `gpsdo`. |
| `sync_source` | `default` | UHD synchronization source: `default`, `internal`, `external`, or `gpsdo`. |
| `max_proc_delay` | `2` | Lower-PHY processing depth in slots. |
| `execution_profile` | `auto` | `auto`, `sequential`, `single`, `dual`, or `triple`. |
| `ru_cpus` | unset | CPU set for the UHD radio and lower-PHY workers. |
| `pinning_policy` | `mask` | `mask` or `round-robin` thread placement within `ru_cpus`. |

## `dpdk`

The optional top-level `dpdk` section selects the DPDK Ethernet backend.
The application must be built with `-DENABLE_DPDK=ON`.

```yaml
dpdk:
  eal_args: "-l 0-3 -a 0000:01:00.0 --proc-type auto"
```

Set each cell's `network_interface` to its DPDK PCIe address and allow-list the same device in `eal_args`.

### Preparing the fronthaul port

Bind an SR-IOV virtual function rather than the physical function.
The PF stays available for management, and the NIC enforces the fronthaul VLAN in hardware.

Enlarge the PF ring descriptors, create one VF, give it its MAC address, port VLAN, and MTU, then bind it to
`vfio-pci`:

```bash
set -x

# Enlarge the ring descriptors on the physical function.
sudo ethtool -G ens6f0np0 rx 8160
sudo ethtool -G ens6f0np0 tx 8160

# Create a single virtual function and configure it through the physical function.
sudo modprobe iavf
sudo sh -c 'echo 0 > /sys/class/net/ens6f0np0/device/sriov_numvfs'
sudo sh -c 'echo 1 > /sys/class/net/ens6f0np0/device/sriov_numvfs'
sudo ip link set ens6f0np0 vf 0 mac aa:bb:cc:dd:ee:ff vlan 5 qos 0 spoofchk off mtu 9600

# Bind the virtual function to the userspace driver.
sudo /usr/local/bin/dpdk-devbind.py --unbind 70:01.0
sudo modprobe vfio-pci
sudo /usr/local/bin/dpdk-devbind.py --bind vfio-pci 70:01.0
```

Replace `ens6f0np0` with the fronthaul PF and `70:01.0` with the PCIe address of the **virtual function**, not the
physical one.
Read the VF address from `dpdk-devbind.py --status` or from `readlink /sys/class/net/ens6f0np0/device/virtfn0`.
Both `network_interface` and the `-a` entry in `eal_args` must name that same VF address.

Five details decide whether the link comes up:

- `vlan 5` is a port VLAN, so the PF inserts the tag on egress and strips it on ingress. The cell must therefore
  **omit** `vlan_tag`, and the matching O-DU cell must omit `vlan_tag_cp` and `vlan_tag_up`. Configuring both puts a
  second 802.1Q header on the wire.
- `mac aa:bb:cc:dd:ee:ff` is the source address the O-DU sees. Either set the cell's `ru_mac_addr` to the same value,
  or keep `spoofchk off` so the VF may transmit from a different source address.
- ProtO-RU requests an MTU of 9000 bytes on the port and aborts at startup if the NIC rejects it, so the VF MTU must
  leave room for it. The ring sizes above give the port enough descriptors to absorb a fronthaul burst.
- Run the `ip link` command before binding to `vfio-pci`. It configures the VF through the PF, which the kernel driver
  must still own at that point.
- `modprobe iavf` loads the VF driver for Intel E810 and X710 class NICs. Substitute the VF driver for other vendors.

None of this survives a reboot. Re-run the sequence, or install it as a boot-time unit.

The EAL cores in `eal_args` run no Open Fronthaul work, so keep them separate from and additional to `ofh_cpus`.
See [Troubleshooting](./TROUBLESHOOTING.md) for how to size those sets.
