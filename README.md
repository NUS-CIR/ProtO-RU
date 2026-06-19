# ProtO-RU 

ProtO-RU is a software implementation of an O-RAN split 7.2-compatible Radio Unit.
This is based on a fork of the [srsRAN Project](https://github.com/srsran/srsran_project), version 24.10.

ProtO-RU extends the srsRAN RU emulator to become a full-fledged O-RAN RU with split 7.2 support using SDRs like the USRP B210.
Benefiting from the portability and modularity of srsRAN, ProtO-RU can be used on different hardware platforms (e.g., x86, ARM) and easily adapt to various use cases.

A software-based O-RU implementation would allow researchers to customize and innovate more freely in end-to-end setups, especially given the fixed-function and proprietary nature of commercial O-RUs.

Technical report - [arxiv](https://arxiv.org/abs/2512.02398).

ACM Open AI-RAN 2025 workshop (invited) demo - [[Poster]](https://drive.google.com/file/d/10YtGOLr3b2fomS6cWCzG7gJtb5P75KzK/view?usp=sharing) [[YouTube]](https://www.youtube.com/watch?v=KSdTqXCAuGs).

OAI Summer 2026 Workshop demo - [[YouTube]](https://youtu.be/LIG8f8c_5q4); a preview of the next version of ProtO-RU based on OCUDU 26.04.

Supported features - [Features](./proto-ru/FEATURES.md).

> Note: ProtO-RU is currently under active development. Please check back frequently for updates and new documentation. The next version of ProtO-RU, based on OCUDU 26.04, will be released soon in Q3 2026. As part of this rebase, ProtO-RU will also transition to the licensing model used by the upstream OCUDU project.

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

### Alternative: Using Docker

You can also build a Docker image that contains ProtO-RU and all its dependencies pre-installed.
To build the Docker image, run the following command from the root directory of the ProtO-RU repository:

```bash
./proto-ru/scripts/build_image.sh
```

This will create a Docker image named `protoru:latest`.
Alternatively, you can pull the pre-built Docker image from Docker Hub:

```bash
docker pull khooi8913/protoru:latest
```

For more details, please refer to the [Docker Quickstart Guide](./proto-ru/DOCKER_QUICKSTART.md).

## Running ProtO-RU

### Time Synchronization

Before running ProtO-RU, please ensure that you have a suitable system to host ProtO-RU (see [HW_REQUIREMENTS](./proto-ru/HW_REQUIREMENTS.md)).

Depending on your testbed setup (see [Testbed Setup](./proto-ru/TESTBED_SETUP.md)), the DU and ProtO-RU host system(s) must be time synchronized with the gNB host over PTP.
We have prepared a quick guide on setting up PTP time synchronization at [Time Synchronization](./proto-ru/TIME_SYNC.md).

Once the system is time synchronized, we can then start ProtO-RU.

### Starting ProtO-RU


If you have built ProtO-RU from source, run the following command from the `build/apps/examples/ofh/` directory (assuming the configuration file is located at `/path/to/ru_emu.yml`):

```bash
sudo ./ru_emulator -c /path/to/ru_emu.yml
```

If you are using the Docker image, run the following command (assuming the configuration file is located at `/path/to/ru_emu.yml` on the host system):

```bash
./proto-ru/scripts/start_image.sh /path/to/ru_emu.yml
```

For more details on configuration options, please refer to the [Configuration Reference](./proto-ru/CONFIG_REFERENCE.md).

We also provide sample configuration files in the [proto-ru/conf-files/](./proto-ru/conf-files/) directory of the repository.

**Important Note:** Given that ProtO-RU requires a large delay profile, the integration with other 5G CU/DU implementations may require some patching. 
Please refer to the [Integration Notes](./proto-ru/INTEGRATION_NOTES.md) for more details.

### Troubleshooting

If you encounter any issues while running ProtO-RU, please consult to the [Troubleshooting Guide](./proto-ru/TROUBLESHOOTING.md) for common problems and their solutions.

The list of known issues can also be found under [Known Issues](./proto-ru/KNOWN_ISSUES.md).

## Citation

If you find ProtO-RU useful to your research, please cite our technical report:

```bibtex
@techreport{zhou2025protoru,
  title={ProtO-RU: An O-RAN Split-7.2 Radio Unit using SDRs}, 
  author={Zhiyu Zhou and Xin Zhe Khooi and Satis Kumar Permal and Mun Choon Chan},
  year={2025},
  eprint={2512.02398},
  archivePrefix={arXiv},
  primaryClass={cs.NI},
  url={https://arxiv.org/abs/2512.02398}, 
}
```

## Contact

For any questions, or if you have any comments/feedback/feature requests, there are several ways to reach out.
- File a GitHub issue under this repo.
- Start a discussion on the GitHub [Discussion board](https://github.com/NUS-CIR/ProtO-RU/discussions).
- Drop an email to `khooixz [at] comp [dot] nus [dot] edu [dot] sg`.
