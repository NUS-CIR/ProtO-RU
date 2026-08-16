<!-- SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited -->
<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# ProtO-RU

ProtO-RU is a software implementation of an O-RAN split 7.2-compatible Radio Unit based on [OCUDU](https://gitlab.com/ocudu/ocudu) (release 26.04).
It extends the OCUDU Open Fronthaul library and RU emulator with an SDR-backed O-RU mode.

ProtO-RU supports UHD-compatible radios such as the USRP B210.
ZMQ operation is not supported in this release.

ProtO-RU is intended for research and development where a customizable software O-RU is preferable to a fixed-function commercial radio.
It can run on x86 and ARM hosts supported by OCUDU and UHD.

- Technical report: [arXiv](https://arxiv.org/abs/2512.02398)
- ACM Open AI-RAN 2025 invited demo: [poster](https://drive.google.com/file/d/10YtGOLr3b2fomS6cWCzG7gJtb5P75KzK/view?usp=sharing), [video](https://www.youtube.com/watch?v=KSdTqXCAuGs)
- Summer 2026 OAI Workshop demo: [video](https://youtu.be/LIG8f8c_5q4)

> **Note:** For earlier releases of ProtO-RU, check out the [`legacy`](https://github.com/NUS-CIR/ProtO-RU/tree/legacy) branch.

## Documentation

- **Overview and setup**
  - [Features](./proto-ru/FEATURES.md)
  - [ProtO-RU architecture](./proto-ru/ARCHITECTURE.md)
  - [Hardware requirements](./proto-ru/HW_REQUIREMENTS.md)
  - [Testbed setup](./proto-ru/TESTBED_SETUP.md)
  - [Time synchronization](./proto-ru/TIME_SYNC.md)
  - [Docker quickstart](./proto-ru/DOCKER_QUICKSTART.md)
- **Configuration and integration**
  - [Example configurations](./proto-ru/conf-files/README.md)
  - [Configuration reference](./proto-ru/CONFIG_REFERENCE.md)
  - [Integration notes](./proto-ru/INTEGRATION_NOTES.md)
- **Support**
  - [Troubleshooting](./proto-ru/TROUBLESHOOTING.md)
  - [Known issues](./proto-ru/KNOWN_ISSUES.md)

## Build ProtO-RU

Install the required dependencies by following the [OCUDU installation guide](https://docs.ocudu.org/user_manual/installation/).
Then build ProtO-RU in the same way as OCUDU:

```bash
mkdir build
cd build
cmake ../
make -j $(nproc)
```

The resulting executable is `build/apps/examples/ofh/ru_emulator`.

See the [Docker quickstart](./proto-ru/DOCKER_QUICKSTART.md) for container-based build and run instructions.

## Run ProtO-RU

ProtO-RU requires synchronized DU and RU host clocks and a real-time kernel on the RU host for sustained operation.
Review the [hardware requirements](./proto-ru/HW_REQUIREMENTS.md) and [time synchronization guide](./proto-ru/TIME_SYNC.md) before starting it.

Adapt one of the [paired example configurations](./proto-ru/conf-files/README.md), then run:

```bash
sudo ./build/apps/examples/ofh/ru_emulator -c /absolute/path/to/protoru.yml
```

ProtO-RU needs a larger Open Fronthaul delay profile than the stock OCUDU O-DU accepts.
Apply the range adjustment described in the [integration notes](./proto-ru/INTEGRATION_NOTES.md) before using the included OCUDU O-DU samples.

## Citation

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

For questions and feedback, use one of these channels:

- [Issue tracker](https://github.com/NUS-CIR/ProtO-RU/issues)
- [Discussion board](https://github.com/NUS-CIR/ProtO-RU/discussions)
- [Discord server](https://discord.gg/RJaAYUMPs7)
- Email: `khooixz [at] comp [dot] nus [dot] edu [dot] sg`

---

# The OCUDU Project

[![Pipeline](https://gitlab.com/ocudu/ocudu/badges/main/pipeline.svg)](https://gitlab.com/ocudu/ocudu/-/pipelines?scope=branches)
[![Documentation](https://img.shields.io/badge/docs-built-green?logo=docusaurus)](https://docs.ocudu.org)
![Code](https://img.shields.io/badge/code-C++17-informational)
![Build](https://img.shields.io/badge/build-CMake-informational)
[![License](https://img.shields.io/badge/license-BSD--3--Clause--Open--MPI-blue)](https://spdx.org/licenses/BSD-3-Clause-Open-MPI.html)
[![OpenSSF Best Practices](https://www.bestpractices.dev/projects/11899/badge)](https://www.bestpractices.dev/projects/11899)
[![Coverage](https://gitlab.com/ocudu/ocudu/badges/main/coverage.svg?min_good=98&min_acceptable=60)](https://docs.ocudu.org/coverage/index.html)

<img src="https://srs.io/wp-content/uploads/ocudu_color.png" alt="image" width="50%"/>

OCUDU is a permissively-licensed, open-source 5G (and beyond) CU/DU project designed for commercial deployment and broad industry adoption, as well as advanced research and development. OCUDU is a complete radio access network (RAN) solution compliant with 3GPP and O-RAN Alliance specifications and includes the full L1/2/3 stack with minimal external dependencies. OCUDU is governed under the Linux Foundation.

This repository contains the RAN source code, architecture documentation, and tooling.

For general information, visit https://ocudu.org.

## Getting started

Build instructions and user guides are provided in the [OCUDU User manual](https://docs.ocudu.org/user_manual/installation/). We also host an extensive selection of tutorials.

## Documentation

Complete project documentation including developer guideline, configuration reference, tutorials, etc. is
hosted in [this](https://gitlab.com/ocudu/ocudu_docs) repo. The most recent version of the documentation
is available [here](https://docs.ocudu.org).

## Contributing

Our project welcomes contributions from any member of our community. To get started contributing,
please take a look at the [Developer Guide](https://docs.ocudu.org/dev_guide/contributing_guide/) with detailed instructions on how to best engange with us.

## Governance

The OCUDU project is governed by a framework of principles, values, policies and processes to help our community and constituents towards our shared goals.

The [Governance](https://gitlab.com/ocudu/Governance) repo is used by the Technical Steering Committee, which oversees governance of the project.

## License

This project is licensed under the BSD 3-Clause Open MPI variant License – see the [LICENSE](./LICENSE) file for details.
Portions of this software may implement 3GPP specifications, which may be subject to additional licensing requirements.
