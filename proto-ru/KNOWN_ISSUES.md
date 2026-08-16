<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Known Issues

This document lists known issues and limitations in ProtO-RU.

1. Timing-window validation is incomplete
    - ProtO-RU validates the 0 to 5000 microsecond range and the hard SDR pipeline lower bounds.
    - It does not validate every relationship between each minimum and maximum value or the complete O-DU/RU window overlap.
      Check the resulting T1a/T2a and Ta3/Ta4 windows for the deployment.

1. SDR PRACH format
    - SDR mode currently supports short PRACH format B4 only.
    - Configure `prach_format: short` and use a matching O-DU PRACH configuration.

1. Multi-section Control-Plane messages
    - ProtO-RU currently supports Control-Plane messages containing exactly one section.
    - It logs and rejects messages whose `numberOfSections` field is not one.

1. USRP clock drift over long runs
    - With `clock_source: internal` and `sync_source: internal`, the USRP runs from its free-running onboard oscillator.
      During long-running operation, it can drift relative to the PTP-synchronized host clock until traffic leaves the fronthaul timing windows.
      Restart ProtO-RU after this occurs.
    - For stable long-running operation, discipline the USRP from an external time/frequency reference, for example an external 10 MHz reference and PPS (`clock_source: external`, `sync_source: external`) or an onboard GPSDO (`clock_source: gpsdo`, `sync_source: gpsdo`).
      This requires a USRP with the corresponding reference inputs.
      See [Time Synchronization](./TIME_SYNC.md) for details.
