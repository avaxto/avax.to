#!/usr/bin/env bash

# avax.to build script

cd "$(dirname "$0")/.."

mkdir -p ./build

if [[ "$OSTYPE" == "darwin"* ]]; then
    BUILD_DIR="./build/mac"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    BUILD_DIR="./build/linux"
else
    echo "Unsupported OS type: $OSTYPE"
    exit 1
fi

cmake -B $BUILD_DIR -S .
cmake --build $BUILD_DIR # -v