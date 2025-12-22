#! /bin/bash

set -e

# Define image name and tag
# Note: Update it as needed
IMAGE_NAME="proto-ru"
IMAGE_TAG="latest"

# Navigate to two levels up from the script directory
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
PROJECT_ROOT=$(dirname "$(dirname "$SCRIPT_DIR")")
echo "ProtO-RU project root directory: $PROJECT_ROOT"

# Build the Docker image for proto-ru
echo "Building ProtO-RU Docker image..."
docker build -f $PROJECT_ROOT/docker/Dockerfile -t ${IMAGE_NAME}:${IMAGE_TAG} $PROJECT_ROOT

# Verify the image was built successfully
if [[ "$(docker images -q ${IMAGE_NAME}:${IMAGE_TAG} 2> /dev/null)" == "" ]]; then
  echo "Error: Docker image ${IMAGE_NAME}:${IMAGE_TAG} failed to build."
  exit 1
fi

echo "Docker image ${IMAGE_NAME}:${IMAGE_TAG} built successfully."