# OCUDU Multi-Container Solution

This folder contains multiple docker compose configurations for different deployment scenarios:

## Available Docker Compose Files

| Compose File               | Services                          | Purpose                                       |
| -------------------------- | --------------------------------- | --------------------------------------------- |
| `docker-compose.yml`       | `5gc`, `gnb`                      | Complete gNB + Core deployment                |
| `docker-compose.split.yml` | `cu-cp`, `cu-up`, `du`            | CU/DU split architecture that replace the gNB |
| `docker-compose.ui.yml`    | `telegraf`, `influxdb`, `grafana` | Monitoring and metrics visualization          |

## Quick Start

### Running the default stack

To just launch the solution with gNB + core network:

```bash
docker compose -f docker/docker-compose.yml up
```

or

```bash
cd docker/
docker compose up
```

### Combining Multiple Compose Files to run custom deployments

You can use [docker compose override feature](https://docs.docker.com/compose/how-tos/multiple-compose-files/merge/) to
combine the compose files to create custom deployments:

```bash
# Run gNB + core
docker compose -f docker/docker-compose.yml up

# Run cu-cp + cu-up + du + core (gNB is replaced)
docker compose -f docker/docker-compose.yml -f docker/docker-compose.split.yml up

# Run gNB + core with monitoring stack
docker compose -f docker/docker-compose.yml -f docker/docker-compose.ui.yml up

# Run split architecture with monitoring stack
docker compose -f docker/docker-compose.yml -f docker/docker-compose.split.yml -f docker/docker-compose.ui.yml up

# Run only the monitoring components (useful when connecting to external gNB
docker compose -f docker/docker-compose.ui.yml up
```

## Service Management

### Extra Start Options

- To force a new build of the containers (including a new build of OCUDU gNB), please add a `--build` flag at the end of
  the previous command.
- To run it in background, please add a `-d` flag at the end of the previous command.
- For more options, check `docker compose up --help`

### See logs

To see services' output, you can run:

```bash
docker compose logs [OPTIONS] [SERVICE...]
```

- For more options, check `docker compose logs --help`

### Tear down the deployment

To stop any deployment:

```bash
docker compose -f docker/docker-compose.yml -f docker/docker-compose.split.yml -f docker/docker-compose.ui.yml down --remove-orphans
```

- If you also want to remove all internal data except the setup, you can add `--volumes` flag at the end of the previous
  command.
- For more options, check `docker compose down --help`

If you're not familiarized with `docker compose` tool, it will be recommended to check its
[website](https://docs.docker.com/compose/) and `docker compose --help` output.

## Configuration

### Enabling metrics reporting in the gNB

To be able to see gNB's metrics in the monitoring UI (Grafana + InfluxDB + Telegraf), it's required to enable metrics
reporting in the gNB config.

**Note**: When using the monitoring stack (`docker-compose.ui.yml` or combined deployments), ensure your gNB
configuration includes:

```yml
metrics:
  autostart_stdout_metrics: true
  enable_json: true
remote_control:
  bind_addr: 0.0.0.0
  enabled: true
```

`gnb` and `du` services already have those options configured in their respective docker-compose.yml files.

### Base image: `OS` and `OS_VERSION`

The gNB image (`docker/Dockerfile`) is built from `${OS}:${OS_VERSION}`. Defaults are **ubuntu** and **26.04**. Override
them when you run Compose so the build uses another base image (for example **debian:bookworm**, **fedora:43**, or
**registry.access.redhat.com/ubi10:latest**).

**Using environment variables** (recommended; works with any `docker compose` command that builds `gnb`). Podman users
can run the same commands with `podman compose` instead of `docker compose`.

```bash
# Debian bookworm instead of Ubuntu
OS=debian OS_VERSION=bookworm docker compose -f docker/docker-compose.yml build gnb

# Another Ubuntu LTS tag
OS=ubuntu OS_VERSION=24.04 docker compose -f docker/docker-compose.yml up --build

# Fedora 43 (matches Docker Hub tag fedora:43; install scripts use dnf and /etc/os-release ID=fedora)
OS=fedora OS_VERSION=43 docker compose -f docker/docker-compose.yml build gnb

# UBI 10 (requires a RHEL-entitled build host; see "Building on UBI 10" below)
OS=registry.access.redhat.com/ubi10 OS_VERSION=latest docker compose -f docker/docker-compose.yml build gnb

# UBI 10 minimal (microdnf; same entitlement requirements)
OS=registry.access.redhat.com/ubi10-minimal OS_VERSION=latest docker compose -f docker/docker-compose.yml build gnb
```

**Using a `.env` file** in the `docker/` directory (or project root, depending on where you run Compose from): add lines
such as:

```dotenv
OS=debian
OS_VERSION=bookworm
```

or, for **fedora:43**:

```dotenv
OS=fedora
OS_VERSION=43
```

or, for **UBI 10**:

```dotenv
OS=registry.access.redhat.com/ubi10
OS_VERSION=latest
```

Then run `docker compose` as usual; Compose substitutes these into `docker-compose.yml` / `docker-compose.split.yml`
build args.

**Using `docker build` directly** (from the repository root):

```bash
docker build -f docker/Dockerfile \
  --build-arg OS=debian \
  --build-arg OS_VERSION=bookworm \
  .

docker build -f docker/Dockerfile \
  --build-arg OS=fedora \
  --build-arg OS_VERSION=43 \
  .

docker build -f docker/Dockerfile \
  --build-arg OS=registry.access.redhat.com/ubi10 \
  --build-arg OS_VERSION=latest \
  .
```

### Building on Fedora

Use an official **`fedora:NN`** image (for example **43**) so `/etc/os-release` reports **`ID=fedora`**. The
`docker/scripts/install_*.sh` helpers install **dnf** packages for that ID (toolchain, ROHC autotools, UHD/DPDK build
deps, **chrony** for the small runtime helper set instead of legacy **ntp**).

If you previously built **`gnb`** with another **`OS`** (for example Ubuntu) and then switch to Fedora, reuse of cached
layers can hide missing packages or wrong paths. Prefer a clean rebuild when changing the base family:

```bash
OS=fedora OS_VERSION=43 docker compose -f docker/docker-compose.yml build --no-cache gnb
```

The ROHC tarball’s **`autogen.sh`** locates **`aclocal`** and friends with the **`which`** command; minimal Fedora
images do not ship **`which`** by default, so the Fedora package lists in the install scripts include it.

### Building on UBI 10

Use a UBI 10 base such as **`registry.access.redhat.com/ubi10:latest`** (or **`registry.redhat.io/ubi10:latest`** after
a Red Hat Registry login). The image reports **`ID=rhel`** with **`PLATFORM_ID=platform:el10`**; the install scripts
detect that via **`is_ubi10`** and enable el10 CRB/EPEL. UBI 9 / RHEL 9 bases are not supported.

**UBI minimal** (`registry.access.redhat.com/ubi10-minimal:latest`) is also supported. That image ships **`microdnf`**
instead of **`dnf`** (see
[Understanding the UBI minimal images](https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/9/html-single/building_running_and_managing_containers/index#con_understanding-the-ubi-minimal-images_assembly_types-of-container-images)).
The helpers select the tool with
`DNF=$([[ -x /usr/bin/dnf ]] && echo dnf || echo "microdnf --setopt install_weak_deps=0 ${UBI10_MICRODNF_REPO_ARGS[*]}")`
and run `${DNF} -y install …` / `${DNF} clean all`. UBI repos are enabled by default;
`--enablerepo=codeready-builder-for-rhel-10-*-rpms` is folded into `DNF` only when that host CRB repo is present
(subscribed build). The same RHEL entitlement requirements below still apply.

**RHEL entitlement is required.** Several packages used at build and runtime (notably individual **`boost-*`** libraries
for UHD) live in subscribed **RHEL 10 AppStream**, not in the free UBI-only repos. The build host must present a valid
RHEL subscription to the container so those repos resolve. Typical options:

- Build on a RHEL host that is already registered; Podman/Docker usually expose the host entitlement into the build
  (`subscription-manager` in the UBI image can then enable AppStream/CRB).
- Or pass Red Hat entitlement files as BuildKit secrets (`rh_entitlement_cert`, `rh_entitlement_key`, `rh_rhsm_conf`,
  `rh_rhsm_ca`), as the RH image CI jobs do via **`BUILD_SECRETS`**. The Dockerfile already declares matching
  `--mount=type=secret,id=rh_*` mounts on the install `RUN` steps.

Without entitlement, `dnf` cannot find packages such as **`boost-filesystem`**, and the gNB image build fails in the
UHD/DPDK runtime stages.

Example (Podman Compose on an entitled host):

```bash
OS=registry.access.redhat.com/ubi10 OS_VERSION=latest \
  podman compose -f docker/docker-compose.yml build gnb
```

Prefer a clean rebuild when switching to UBI from another base family:

```bash
OS=registry.access.redhat.com/ubi10 OS_VERSION=latest \
  docker compose -f docker/docker-compose.yml build --no-cache gnb
```

The install scripts under `docker/scripts/` dispatch on `/etc/os-release` (for example `ID=debian`, `ID=ubuntu`,
`ID=fedora`, `ID=rhel`, `ID=centos`, `ID=arch`). Choosing `OS`/`OS_VERSION` only selects the base image; the same script
logic applies as long as the distribution is one of the supported families. UBI 9 / RHEL 9 bases are not supported.

### Customizations

- Default docker compose uses `configs/gnb_rf_b200_tdd_n78_20mhz.yml` config file. You can change it by setting the
  variable `${GNB_CONFIG_PATH}` in the shell, in the `docker compose up` command line or using the existing env-file
  `.env`. More info about how to do it in docker documentation here:
  [https://docs.docker.com/compose/environment-variables/set-environment-variables/](https://docs.docker.com/compose/environment-variables/set-environment-variables/)

F.e.:

```bash
# Set variables for specific deployment
export GNB_CONFIG_PATH=configs/gnb_custom.yml
docker compose -f docker-compose.yml -f docker-compose.ui.yml up
```

- Network: If you are using an existing core-network on same machine, then you can comment the `5gc` service section and
  also link your ocudu container to some existing AMF N2/N3 subnet, doing something like this:

```yml
  gnb: ...
    networks:
      network1:
          ipv4_address: 192.168.70.163 # Setting a fixed IP in the "network1" net

networks:
  network1:
    name: my-pre-existing-network
    external: true
```

More info here: [https://docs.docker.com/compose/networking/](https://docs.docker.com/compose/networking/)

### Open5GS Container Parameters

Advanced parameters for the Open5GS container are stored in [open5gs.env](open5gs/open5gs.env) file. You can modify it
or use a totally different file by setting `OPEN_5GS_ENV_FILE` variable like in:

```bash
OPEN_5GS_ENV_FILE=/my/open5gs.env docker compose -f docker/docker-compose.yml up 5gc
```

The following parameters can be set:

- MONGODB_IP (default: 127.0.0.1): This is the IP of the mongodb to use. 127.0.0.1 is the mongodb that runs inside this
  container.
- SUBSCRIBER_DB (default:
  "001010123456780,00112233445566778899aabbccddeeff,opc,63bfa50ee6523365ff14c1f45f88737d,8000,10.45.1.2"): This adds
  subscriber data for a single or multiple users to the Open5GS mongodb. It contains either:
  - Comma separated string with information to define a subscriber
  - `subscriber_db.csv`. This is a csv file that contains entries to add to open5gs mongodb. Each entry will represent a
    subscriber. It must be stored in `docker/open5gs/`
- OPEN5GS_IP: This must be set to the IP of the container (here: 10.53.1.2).
- UE_IP_BASE: Defines the IP base used for connected UEs (here: 10.45.0).
- DEBUG (default: false): This can be set to true to run Open5GS in debug mode.

For more info, please check its own [README.md](open5gs/README.md).

### Open5GS Container Applications

Open5Gs container includes other binaries such as

- 5gc: 5G Core Only
- epc: EPC Only
- app: Both 5G Core and EPC

By default 5gc is launched. If you want to run another binary, remember you can use `docker compose run` to run any
command inside the container. For example:

```bash
docker compose -f docker/docker-compose.yml run 5gc epc -c open5gs-5gc.yml
```

If you need to use custom configuration files, remember you can share folder and files between your local PC (host) and
the container:

```bash
docker compose -f docker/docker-compose.yml run -v /tmp/my-open5gs-5gc.yml:/config/my-open5gs-5gc.yml 5gc epc -c /config/my-open5gs-5gc.yml
```

### Metric UI Setup

Change the environment variables define in `.env` that are used to setup and deploy the stack

```bash
├── .env         <---
├── docker-compose.yml
├── Dockerfile
└── ...
```

#### Connecting to gNB in Different Deployment Scenarios

The monitoring stack (Telegraf + InfluxDB + Grafana) connects to the gNB via WebSocket to collect metrics. The `WS_URL`
variable in `.env` controls where Telegraf looks for the gNB. Choose the scenario that matches your deployment:

| Scenario                  | `WS_URL` value              | When to use                                                      |
| ------------------------- | --------------------------- | ---------------------------------------------------------------- |
| **[A] gNB in Docker**     | `gnb:8001` (default)        | gNB runs via `docker-compose.yml` alongside the monitoring stack |
| **[B] gNB on Host**       | `host.docker.internal:8001` | gNB built and running natively on the host machine               |
| **[C] gNB in Kubernetes** | `<K8S_NODE_IP>:<NODE_PORT>` | gNB deployed in a Kubernetes cluster                             |
| **[D] gNB on Remote**     | `<REMOTE_IP>:8001`          | gNB runs on a different machine on the network                   |

##### Scenario A: gNB in Docker (default)

No changes needed. The default `.env` already uses Docker DNS (`gnb:8001`):

```bash
# Just start both compose files together
docker compose -f docker/docker-compose.yml -f docker/docker-compose.ui.yml up
```

##### Scenario B: gNB on Host Machine

When the gNB runs directly on the host (not in Docker), Telegraf inside the container needs to reach the host network
via `host.docker.internal`:

```bash
# 1. Edit docker/.env
sed -i 's/^WS_URL=.*/WS_URL=host.docker.internal:8001/' docker/.env

# 2. Start the monitoring stack only
docker compose -f docker/docker-compose.ui.yml up -d

# 3. Start the gNB natively (ensure metrics are enabled in the config)
sudo ./ocudu_gnb -c gnb.yml
```

> **Linux note:** If `host.docker.internal` does not resolve, the `extra_hosts` entry in `docker-compose.ui.yml` should
> handle it automatically. If it still fails, use your host's IP directly: `WS_URL=<HOST_IP>:8001` or the Docker bridge
> gateway `WS_URL=172.17.0.1:8001`.

##### Scenario C: gNB in Kubernetes

When the gNB is deployed in a Kubernetes cluster:

```bash
# 1. Expose the gNB metrics port via NodePort or LoadBalancer
#    Example: kubectl expose deployment ocudu-gnb --type=NodePort --port=8001

# 2. Find the assigned NodePort
#    kubectl get svc ocudu-gnb -o jsonpath='{.spec.ports[0].nodePort}'

# 3. Edit docker/.env with the K8s node IP and port
sed -i 's/^WS_URL=.*/WS_URL=10.0.0.50:30801/' docker/.env

# 4. Start the monitoring stack
docker compose -f docker/docker-compose.ui.yml up -d
```

Alternatively, if using K8s service DNS from within the same cluster: `WS_URL=ocudu-gnb-metrics.ocudu:8001`

For more information on Kubernetes deployments, see [https://docs.ocudu.org](https://docs.ocudu.org).

##### Scenario D: gNB on Remote Server

When the gNB runs on a different machine:

```bash
# 1. Edit docker/.env with the remote server's IP
sed -i 's/^WS_URL=.*/WS_URL=192.168.1.100:8001/' docker/.env

# 2. Ensure the remote gNB is reachable on port 8001
#    (check firewalls, security groups, etc.)

# 3. Start the monitoring stack
docker compose -f docker/docker-compose.ui.yml up -d
```

##### Troubleshooting

| Symptom                                 | Likely Cause                                  | Fix                                                                              |
| --------------------------------------- | --------------------------------------------- | -------------------------------------------------------------------------------- |
| Grafana shows "No data"                 | `WS_URL` points to wrong address              | Verify with `docker exec telegraf printenv WS_URL` and check connectivity        |
| `host.docker.internal` does not resolve | Missing `extra_hosts` or old Docker version   | Ensure `docker-compose.ui.yml` has `extra_hosts` entry; upgrade Docker to 20.10+ |
| Connection refused on port 8001         | gNB metrics not enabled or wrong bind address | Add `remote_control: { enabled: true, bind_addr: 0.0.0.0 }` to gNB config        |
| Telegraf logs show WebSocket errors     | Network/firewall blocking the connection      | Check `docker logs telegraf`; ensure port 8001 is open between hosts             |
| Metrics stop after gNB restart          | Telegraf doesn't auto-reconnect immediately   | Restart Telegraf: `docker compose restart telegraf`                              |

##### Manual Verification

To verify the connection is working:

```bash
# Check Telegraf logs for successful WebSocket connection
docker logs telegraf 2>&1 | grep -i "ws\|websocket\|connect"

# Verify the WS_URL environment variable inside the container
docker exec telegraf printenv WS_URL

# Test WebSocket connectivity manually (requires python3 + websockets)
docker exec telegraf python3 -c "
import asyncio, websockets
async def test():
    import os
    url = f\"ws://{os.environ['WS_URL']}\"
    try:
        async with websockets.connect(url, open_timeout=5) as ws:
            print(f'Connected to {url}')
    except Exception as e:
        print(f'Failed to connect to {url}: {e}')
asyncio.run(test())
"
```

You can access grafana in [http://localhost:3300](http://localhost:3300). By default, you'll be in view mode without
needing to log in. If you want to modify anything, you need to log in using following credentials:

- username: `admin`
- password: `admin`

After your fist log, it will ask you to change the password for a new one, but it can be skipped.

Provisioned Dashboards are into `Home > Dashboards`. **They don't support variable substitution**, so if you change
default values in `.env` file, you'll need to go to `grafana/dashboards/` and manually search and replace values such as
influxdb uid or bucket in every `.json` file.
