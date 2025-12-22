#! /bin/bash

set -xe

# Define image name and tag
# Note: Update it as needed
IMAGE_NAME="proto-ru"
IMAGE_TAG="latest"

# First argument should be the full path to the config file
if [ "$#" -ne 1 ]; then
    echo "ERROR: Missing config file argument."
    echo "  Usage: $0 /full/path/to/config.yaml"
    exit 1
fi

CONFIG_FILE="$1"
# Make sure that it is an absolute path
if [[ "$CONFIG_FILE" != /* ]]; then
    echo "ERROR: Please provide an absolute path to the config file."
    exit 1
fi

# Start the ProtO-RU Docker container
sudo docker run -it --rm --privileged --net=host -v ${CONFIG_FILE}:/opt/config.yaml -v /dev:/dev -v /tmp:/tmp ${IMAGE_NAME}:${IMAGE_TAG}