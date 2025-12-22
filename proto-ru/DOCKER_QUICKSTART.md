# Docker Quickstart Guide 

This guide provides instructions on how to quickly get started with ProtO-RU using Docker. 
Follow the steps below to build the Docker image and run ProtO-RU in a containerized environment.

## Pulling the Docker Image

You can pull the pre-built ProtO-RU Docker image from Docker Hub using the following command:

```bash
docker pull khooi8913/proto-ru:latest
```

> Note: The pre-built image is only provided for x86_64 architecture. 
> If you are using a different architecture, you may need to build the image locally.

## Building the Docker Image (Optional)

If you prefer to build the Docker image locally, you can do so by navigating to the `proto-ru` directory and running the following script:

```bash 
./proto-ru/scripts/build_image.sh
```

The build script will create a Docker image named `proto-ru:latest`.
Rename the image or change the tag as needed by modifying the `IMAGE_NAME` and `IMAGE_TAG` variables in the script.

## Running ProtO-RU in a Docker Container

To run ProtO-RU in a Docker container, use the following command:

```bash
./proto-ru/scripts/start_image.sh $CONFIG_FILE_PATH
```

> Note: If you are using the pre-built image from Docker Hub, ensure that you have pulled the image first.
> Then, you need to modify the `IMAGE_NAME` variable in the `start_image.sh` script to match the pulled image.

Replace `$CONFIG_FILE_PATH` with the absolute path to your ProtO-RU configuration file on the host machine.
The scripts will mount the configuration file into the container and start ProtO-RU with the specified settings.

For example, to start ProtO-RU with a provided configuration file, you can run:
```bash
./proto-ru/scripts/start_image.sh ~/ProtO-RU/proto-ru/conf-files/protoru-OAI-B210-TDD-n78-20MHz-2x2-30kHz.yml
```
