# Example Configuration Files

This directory contains some example configuration files for ProtO-RU. 

We provide both the gNB and ProtO-RU configuration files.
The configuration files come in ``pairs'', where each gNB configuration file corresponds to a ProtO-RU configuration file with matching parameters (e.g., bandwidth, MIMO mode, subcarrier spacing).

You should adapt these configuration files to your specific setup and hardware.
More details on ProtO-RU's configuration options can be found in the [Configuration Reference](../CONFIG_REFERENCE.md).

## Naming Convention

### srsRAN

For srsRAN gNB configuration files, we use the following naming convention:
`gNB-srsRAN-<SDR>-<DUPLEX_MODE>-<NR_BAND>-<BANDWIDTH>-<MIMO_MODE>-<SCS>.yml`.

An example file name is `srsRAN-gNB-B210-TDD-n78-20MHz-2x2-30kHz.yml`, which indicates a gNB configuration file for srsRAN using a USRP B210 SDR in TDD mode running at MIMO2x2, 20 MHz bandwidth, and 30 kHz subcarrier spacing.

### OpenAirInterface5G

For OAI gNB configuration files, we use the following naming convention:
`gNB-OAI-<SDR>-<DUPLEX_MODE>-<NR_BAND>-<BANDWIDTH>-<MIMO_MODE>-<SCS>.conf`.

An example file name is `OAI-gNB-B210-TDD-n78-20MHz-2x2-30kHz.conf`, which indicates a gNB configuration file for OAI using a USRP B210 SDR in TDD mode running at MIMO2x2, 20 MHz bandwidth, and 30 kHz subcarrier spacing.

### ProtO-RU

For ProtO-RU configuration files, we use the following naming convention:
`protoru-<5GSTACK>-<SDR>-<DUPLEX_MODE>-<NR_BAND>-<BANDWIDTH>-<MIMO_MODE>-<SCS>.yml`.

An example file name is `protoru-srsRAN-B210-TDD-n78-20MHz-2x2-30kHz.yml`, which indicates a ProtO-RU configuration file for srsRAN integration using a USRP B210 SDR in TDD mode running at MIMO2x2, 20 MHz bandwidth, and 30 kHz subcarrier spacing.

## Important Notes

### RX_EARLY Warnings
In some of the provided configuration files, ProtO-RU may report RX_EARLY warnings during operation, but that does not affect the overall operation of ProtO-RU with the gNB as the implemnentation will accept early arrivals.
We will continuously test and update this set of example configuration files over time to minimize such warnings.
