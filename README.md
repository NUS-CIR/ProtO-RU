# ProtO-RU 

ProtO-RU is a software implementation of an O-RAN split 7.2-compatible Radio Unit.
This is based on a fork of the srsRAN Project, version 24.10.

ProtO-RU extends the srsRAN RU emulator to become a full-fledged O-RAN RU with split 7.2 support using SDRs like the USRP B210.
Benefiting from the portability and modularity of srsRAN, ProtO-RU can be used on different hardware platforms (e.g., x86, ARM) and easily adapt to various use cases.

Support - [Discussion board](https://github.com/NUS-CIR/ProtO-RU/discussions).

Features and roadmap - [Features](./proto-ru/FEATURES.md).

## Dependencies

Since ProtO-RU is based on the srsRAN project, it inherits all the dependencies of srsRAN.

Please refer to the srsRAN project documentation, sections "Build  Tools and Dependencies" and "RF-drivers" (especially UHD drivers) for the list of dependencies required to build ProtO-RU.

If your current system can successfully build and run srsRAN in Split-8 mode with any UHD-compatible SDR (e.g., B210, N310), then you should be able to build and run ProtO-RU without any issues.

## Build Instructions

To download and build ProtO-RU, first, clone the ProtO-RU repository:

```bash
git clone https://github.com/NUS-CIR/ProtO-RU.git
```

Then, build ProtO-RU.

```bash
cd ProtO-RU
mkdir build
cd build
cmake ../
cd build/apps/examples/ofh/ # ProtO-RU extends the srsRAN RU emulator
make -j $(nproc)
```

## Running ProtO-RU

To run ProtO-RU, run the following command from the `build/apps/examples/ofh/` directory (assuming the configuration file is located at `/path/to/ru_emu.yml`):

```bash
sudo ./ru_emulator -c /path/to/ru_emu.yml
```

For more details on configuration options, please refer to the [Configuration Reference](./proto-ru/CONFIG_REFERENCE.md).

We also provide sample configuration files in the `proto-ru/configs/` directory of the repository.