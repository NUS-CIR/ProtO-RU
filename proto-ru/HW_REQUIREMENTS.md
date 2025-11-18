# Hardware Requirements

To get ProtO-RU reliably, there are several system requirements that need to be met in order to meet the RT performance and functionality needs.

- **CPU**: A modern processor with at least 4 CPU cores running at >=2.0 GHz is *required*. Higher core counts is necessary to support larger bandwidths.
- **Memory**: A minimum of 8GB RAM is recommended. 
- **Network Interface**: 10GbE Ethernet interface is *required*. The NIC must also support hardware timestamping for PTP.
    - NOTE: Technically, 1GbE/2.5 GbE interfaces should work, but so far we have had limited success with them and kept on running into timing-related issues. Please consult [Known Issues](KNOWN_ISSUES.md) for more details.
- **SDR**: A compatible SDR device is *required*. Currently, only the NI USRPs are supported -- as they are the only ones supported by the srsRAN library.
    - NOTE: Other SDRs like the BladeRF may work (see this [discussion](https://github.com/srsran/srsRAN_Project/discussions/222#discussioncomment-7120314)), but we have not tested them ourselves.

## Tested Host Systems

ProtO-RU has been tested to work on the following systems:

1. Supermicro SYS-120C-TR Server + USRP B210
    - CPU: Dual-socket Intel Xeon Gold 6326 (16C32T) @ 2.9 GHz
    - Memory: 256GB DDR4 ECC
    - NIC: Intel E810-XXVDA4 4x25GbE Ethernet Adapter
    - OS: RHEL9.4
    - Linux kernel: 5.14.0-503.22.1.el9_5.x86_64+rt

1. Asus RS700-E10-RS4U Server + USRP N310
    - CPU: Dual-socket Intel Xeon Gold 6334 (8C16T) @ 3.6 GHz
    - Memory: 256GB DDR4 ECC
    - NIC: Intel X710-DA2 2x10GbE Ethernet Adapter
    - OS: Ubuntu 24.04.3 LTS
    - Linux kernel: 6.8.0-87-generic

1. Dell XPS 8910 Desktop + USRP B210 (only up to 20 MHz bandwidth)
    - CPU: Intel Core i7-6700 (4C8T) @ 3.4 GHz
    - Memory: 16GB DDR4
    - NIC: Intel X710-DA2 2x10GbE Ethernet Adapter
    - OS: Ubuntu 24.04.3 LTS
    - Linux kernel: 6.8.0-87-generic
