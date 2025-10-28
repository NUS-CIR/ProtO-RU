# Integration Notes

This file contains notes and instructions for integrating ProtO-RU with other open source 5G CU/DU implementations, namely srsRAN and OpenAirInterface (OAI).

As ProtO-RU requires a large delay profile (T2a/Ta3) to operate correctly, we need to patch both srsRAN and OAI DU implementations to support larger delay values.

## srsRAN 

For srsRAN, the current maximum allowed for the T1a/Ta4 values is 1960 microseconds. 
To support ProtO-RU, it will require larger delay values.

We will need to modify `srsRAN_Project/apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_cli11_schema.cpp` and change all `->check(CLI::Range(0, 1960));` to `->check(CLI::Range(0, 5000));` for the T1a/Ta4 parameters.

The expected code snippet is as follows:

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

Once the changes are made, recompile srsRAN.

## OpenAirInterface (OAI) 

As for OAI, the xRAN fronthaul library does not support T1a/Ta4 values larger than a TTI interval.

We have created a patch to modify xRAN implementation to support larger T1a/Ta4 values, and we are in the process of engaging with the OAI maintainers to upstream the changes.

Please refer to the merge request [!3690](https://gitlab.eurecom.fr/oai/openairinterface5g/-/merge_requests/3690) for the patch and instructions on how to apply it.