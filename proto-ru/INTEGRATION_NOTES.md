<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Integration Notes

This file contains notes for integrating ProtO-RU with OCUDU and OpenAirInterface (OAI) CU/DU implementations.

- ProtO-RU requires a large T2a/Ta3 delay profile.
- The corresponding O-DU must support sufficiently large T1a/Ta4 values.

## Upstream OCUDU

- Upstream OCUDU limits O-DU T1a/Ta4 values to 1960 microseconds.
- The provided ProtO-RU profiles require values up to 5000 microseconds.
- This ProtO-RU branch already includes the required range adjustment.
- When porting ProtO-RU to an unmodified upstream OCUDU tree:
  - From the repository root, open `apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_cli11_schema.cpp`.
  - Change every T1a/Ta4 `CLI::Range(0, 1960)` check to `CLI::Range(0, 5000)`.
  - Rebuild the OCUDU O-DU.
  - Note that the sample OCUDU configurations under `conf-files/` will be rejected until this change is applied.

The expected timing checks are:

```cpp
  ...
  // Note: For the timing parameters, worst case is 2 slots for scs 15KHz and 14 symbols. Implementation defined.
  add_option(app, "--t1a_max_cp_dl", config.T1a_max_cp_dl, "T1a maximum value for downlink Control-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  add_option(app, "--t1a_min_cp_dl", config.T1a_min_cp_dl, "T1a minimum value for downlink Control-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  add_option(app, "--t1a_max_cp_ul", config.T1a_max_cp_ul, "T1a maximum value for uplink Control-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  add_option(app, "--t1a_min_cp_ul", config.T1a_min_cp_ul, "T1a minimum value for uplink Control-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  add_option(app, "--t1a_max_up", config.T1a_max_up, "T1a maximum value for User-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  add_option(app, "--t1a_min_up", config.T1a_min_up, "T1a minimum value for User-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  add_option(app, "--ta4_max", config.Ta4_max, "Ta4 maximum value for User-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  add_option(app, "--ta4_min", config.Ta4_min, "Ta4 minimum value for User-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
   ...
```

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
