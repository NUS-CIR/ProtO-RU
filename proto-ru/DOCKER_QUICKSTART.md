<!-- SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore -->
<!-- SPDX-License-Identifier: BSD-3-Clause-Open-MPI -->

# Docker Quickstart

The upstream OCUDU Dockerfile builds and installs `ru_emulator` together with UHD and the other OCUDU applications.

## Build the image

Run this command from the repository root:

```bash
docker build --target runtime -t protoru:26.04 -f docker/Dockerfile .
```

The build uses the host architecture by default and can take considerable time because it builds UHD, DPDK, and OCUDU from source.

## Run ProtO-RU

Use an absolute configuration path and give the container access to the host network and radio devices:

```bash
docker run --rm -it \
  --privileged \
  --network host \
  -v /absolute/path/to/protoru.yml:/opt/protoru.yml:ro \
  -v /dev:/dev \
  -v /tmp:/tmp \
  protoru:26.04 \
  ru_emulator -c /opt/protoru.yml
```

The Ethernet interface named in the configuration must exist in the container's host network namespace.
The UHD device must also be visible under `/dev` or reachable over the host network, depending on the USRP model.

## Run with Docker Compose

The sample [`docker-compose.yml`](./docker-compose.yml) builds the same runtime image and starts ProtO-RU with host networking and radio-device access.
It uses the common B210 TDD n78, 20 MHz, 2x2 profile by default.

Update the interface name, MAC addresses, CPU sets, gains, and UHD arguments in the selected configuration.
Select a configuration by exporting its absolute path, then start the service from the repository root:

```bash
export PROTORU_CONFIG_PATH="$PWD/proto-ru/conf-files/protoru-B210-TDD-n78-20MHz-2x2-30kHz.yml"
docker compose -f proto-ru/docker-compose.yml up
```

Export the variable on its own line rather than prefixing it to the `docker compose` command.
A prefixed assignment is discarded when the command runs under `sudo`, which resets the environment, and when a line
continuation is broken by a stray space after the backslash.
`PROTORU_CONFIG_PATH` then arrives empty and the Compose file falls back to its default 20 MHz profile without
reporting an error, so the wrong configuration starts silently.
Confirm which file Compose resolved before starting anything:

```bash
docker compose -f proto-ru/docker-compose.yml config | tail -3
```

The container keeps the fixed `protoru` name and stays behind after ProtO-RU exits.
Remove it with `docker compose -f proto-ru/docker-compose.yml down` before starting a different configuration.

Add `--build` only when the `protoru:26.04` image does not exist yet, or after changing the sources.
Check with `docker images protoru`.
Every other run should omit it, because rebuilding recompiles UHD, DPDK, and OCUDU from source and takes a long time.

The sample ProtO-RU configurations write logs to `/tmp/proto-ru.log`.
Follow the log from the host with:

```bash
tail -f /tmp/proto-ru.log
```

The Compose service also mounts the host's `/tmp` at `/tmp`.
This mount makes `/tmp/proto-ru.log` available at the same path on the host.

Switch to another common ProtO-RU configuration by exporting a different absolute path and running the service again.

## Run with DPDK

Use the same Compose file for AF_PACKET and DPDK.
The selected ProtO-RU configuration controls the Ethernet backend.
The image includes DPDK support, and the Compose service provides privileged access to `/dev` for VFIO and hugepages.

Before starting the container:

- Configure hugepages on the host.
- Create an SR-IOV virtual function on the fronthaul NIC and bind it to `vfio-pci`, following [Preparing the fronthaul port](./CONFIG_REFERENCE.md#preparing-the-fronthaul-port).
- Replace the sample PCIe address in both `dpdk.eal_args` and `network_interface` with the virtual function's own address.
- Adjust the DPDK and application CPU sets so that they do not overlap.

Run the 40 MHz DPDK example from the repository root:

```bash
export PROTORU_CONFIG_PATH="$PWD/proto-ru/conf-files/protoru-B210-TDD-n78-40MHz-1x1-30kHz-dpdk.yml"
docker compose -f proto-ru/docker-compose.yml config | tail -3
docker compose -f proto-ru/docker-compose.yml up
```

The middle command prints the configuration file Compose resolved.
Check that it names the DPDK profile before starting the container, because an empty `PROTORU_CONFIG_PATH` silently
selects the default AF_PACKET profile instead.

Stop and remove the container with:

```bash
docker compose -f proto-ru/docker-compose.yml down
```

No prebuilt ProtO-RU image is documented for this release.
The older `latest` images from the v0.3 series use the legacy configuration schema and should not be used with these sample files.
