# Known Issues

This document lists known issues and limitations in ProtO-RU.

1. Realtime KPI monitoring 
    - TX_TOTAL counter is currently not implemented.

1. T2a/Ta3 value validation
    - Currently, ProtO-RU does not validate the T2a/Ta3 values configured in the RU configuration file.
    - Users must ensure that the T2a/Ta3 values are sufficiently large to provide enough time for the RU to process and transmit the data within the required deadlines.

