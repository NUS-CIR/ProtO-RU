# ProtO-RU 

ProtO-RU is a software implementation of an O-RAN split 7.2-compatible Radio Unit.
This is based on a fork of the [srsRAN Project](https://github.com/srsran/srsran_project), version 24.10.

ProtO-RU extends the srsRAN RU emulator to become a full-fledged O-RAN RU with split 7.2 support using SDRs like the USRP B210.
Benefiting from the portability and modularity of srsRAN, ProtO-RU can be used on different hardware platforms (e.g., x86, ARM) and easily adapt to various use cases.

A software-based O-RU implementation would allow researchers to customize and innovate more freely in end-to-end setups, especially given the fixed-function and proprietary nature of commercial O-RUs.

Technical report - Coming soon.

ACM Open AI-RAN 2025 workshop (invited) demo - [[Poster]](https://drive.google.com/file/d/10YtGOLr3b2fomS6cWCzG7gJtb5P75KzK/view?usp=sharing) [[YouTube]](https://www.youtube.com/watch?v=KSdTqXCAuGs).

Supported features - [Features](./proto-ru/FEATURES.md).

Known issues - [Known Issues](./proto-ru/KNOWN_ISSUES.md).

> Note: ProtO-RU is currently under active development. Please check back frequently for updates and new documentation.

## Building ProtO-RU

### Dependencies

Since ProtO-RU is based on the srsRAN project, it inherits all the dependencies of srsRAN.

Please refer to the srsRAN project documentation, sections "Build  Tools and Dependencies" and "RF-drivers" (especially UHD drivers) for the list of dependencies required to build ProtO-RU.

If your current system can successfully build and run srsRAN in Split-8 mode with any UHD-compatible SDR (e.g., B210, N310), then you should be able to build and run ProtO-RU without any issues.

### Compilation

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
cd ./apps/examples/ofh/ # ProtO-RU extends the srsRAN RU emulator
make -j $(nproc)
```

## Running ProtO-RU

### Time Synchronization

Before running ProtO-RU, depending on your testbed setup (see [Testbed Hardware Setup](./proto-ru/TESTBED_SETUP.md)), the host system(s) must be time synchronized with the gNB host over PTP.
We have prepared a quick guide on setting up PTP time synchronization at [Time Synchronization](./proto-ru/TIME_SYNC.md).

### Starting ProtO-RU

Once the system is time synchronized, we can then start ProtO-RU.
To run ProtO-RU, run the following command from the `build/apps/examples/ofh/` directory (assuming the configuration file is located at `/path/to/ru_emu.yml`):

```bash
sudo ./ru_emulator -c /path/to/ru_emu.yml
```

For more details on configuration options, please refer to the [Configuration Reference](./proto-ru/CONFIG_REFERENCE.md).

We also provide sample configuration files in the [proto-ru/conf-files/](./proto-ru/conf-files/) directory of the repository.

**Important Note:** Given that ProtO-RU requires a large delay profile, the integration with other 5G CU/DU implementations may require some patching. 
Please refer to the [Integration Notes](./proto-ru/INTEGRATION_NOTES.md) for more details.

## Citation

If you find ProtO-RU useful to your research, please cite our technical report:

```bibtex
@techreport{zhou2025protoru,
  author      = "Zhiyu Zhou and Xin Zhe Khooi and Satis Kumar Permal and Mun Choon Chan",
  title       = "ProtO-RU: An O-RAN Split-7.2 Radio Unit using SDRs",
  institution = "National University of Singapore",
  year        = "2025"
}
```

## Contact

For any questions, or if you have any comments/feedback/feature requests, there are several ways to reach out.
- File a GitHub issue under this repo.
- Start a discussion on the GitHub [Discussion board](https://github.com/NUS-CIR/ProtO-RU/discussions).
- Drop an email to `khooixz [at] comp [dot] nus [dot] edu [dot] sg`.
