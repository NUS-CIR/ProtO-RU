<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Integration Notes

This file contains notes for integrating ProtO-RU with OCUDU and OpenAirInterface (OAI) CU/DU implementations.

- ProtO-RU requires a large T2a/Ta3 delay profile.
- The corresponding O-DU must support sufficiently large T1a/Ta4 values.

## OCUDU split-7.2

This branch is based on `ocudu/main` and includes the T1a/Ta4 range extension to 0 through 5000 microseconds.
No additional range patch is required when the O-DU is built from this checkout.
Build and run the explicit split-7.2 target:

```bash
cmake --build build --target gnb_split_7_2 -j "$(nproc)"
sudo ./build/apps/gnb_split_7_2/gnb -c proto-ru/conf-files/gnb-ocudu-B210-TDD-n78-20MHz-2x2-30kHz.yml
```

Before starting hardware, check that the YAML parses:

```bash
./build/apps/gnb_split_7_2/gnb --dryrun -c proto-ru/conf-files/gnb-ocudu-B210-TDD-n78-20MHz-2x2-30kHz.yml
```

`--dryrun` checks parsing and parameter derivation; it exits before full configuration validation and does not
validate NICs, RF operation, or end-to-end timing.

### Using a separate OCUDU checkout

The OCUDU base used for this port limits T1a/Ta4 to 1960 microseconds. If a separate checkout rejects the larger
values in these examples, inspect `apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_cli11_schema.cpp`.
Extend all eight T1a/Ta4 schema bounds to 5000 microseconds and rebuild that checkout's split-7.2 O-DU.
The schema in this branch expresses those bounds as `->range(0, 5000)`.

Keep the DU scheduling advance large enough for its transmit windows. In the samples, OCUDU's
`expert_phy.max_proc_delay` is 10 slots for 30 kHz TDD and 5 slots for 15 kHz FDD.
These are separate from the RU's `sdr.max_proc_delay`, which defaults to two slots.

The paired files must agree on antenna/eAxC lists, bandwidth, SCS, ARFCN, MAC addresses, compression, and VLAN
handling. The DU's T1a transmit range must fit inside the RU's corresponding T2a receive range with propagation
and processing margin; the DU's Ta4 receive window must contain the RU's Ta3 transmit window.
Adapt interface names, AMF addresses, PCIe addresses, and CPU masks for the deployment.

## OpenAirInterface (OAI)

- Earlier OAI xRAN integrations did not support T1a/Ta4 values larger than a TTI interval.

### xRAN F release

- Large DU delay-profile support was upstreamed in OAI merge request [!3690](https://gitlab.eurecom.fr/oai/openairinterface5g/-/merge_requests/3690).
- The first OAI weekly tag containing the change is `2026.w06`.

### xRAN K release

- Large DU delay-profile support was upstreamed in [openairinterface/o-du-phy#3](https://github.com/openairinterface/o-du-phy/pull/3).
- The fix is included in xRAN K tag `11.1.2`.
- Duranta pull request [#257](https://github.com/duranta-project/openairinterface5g/pull/257) updated the bundled K dependency from `11.1.1` to `11.1.2`.
- The first Duranta weekly tag containing the update is [`2026.w27`](https://github.com/duranta-project/openairinterface5g/tree/2026.w27).
- Use `2026.w27` or a later weekly tag without applying the large-delay-profile patch manually.
- Duranta weekly tags `2026.w17` through `2026.w26` contain the initial K-release integration but predate the dependency update.
