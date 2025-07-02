#!/usr/bin/env bash

# avax.to build script

cd "$(dirname "$0")/.."

mkdir -p ./build

echo "Inspecting ~/Qt directory structure:"
ls -ld ~/Qt/* 2>/dev/null || echo "No directories found in ~/Qt"

QT_LIB_DIR=$(find ~/Qt -type d -path "*/gcc_64/lib" 2>/dev/null | head -n 1)

if [ -n "$QT_LIB_DIR" ]; then
    QT_VERSION=$(echo "$QT_LIB_DIR" | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' || echo "Unknown")
    QT_CMAKE_PREFIX=~/Qt/$QT_VERSION/gcc_64/lib/cmake

    echo "Found Qt library directory: $QT_LIB_DIR"
    echo "Qt version: $QT_VERSION"
    echo "Qt cmake dir: $QT_CMAKE_PREFIX"

    if [[ "$OSTYPE" == "darwin"* ]]; then
        BUILD_DIR="./build/mac"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        BUILD_DIR="./build/linux"
    else
        echo "Unsupported OS type: $OSTYPE"
        exit 1
    fi

    

    cmake -DCMAKE_PREFIX_PATH:PATH=$QT_CMAKE_PREFIX -B $BUILD_DIR -S .
    cmake --build $BUILD_DIR # -v    
else
    echo "Error: No Qt library directory found in ~/Qt/*/gcc_64/lib/"
    echo "Possible reasons:"
    echo "- Qt is not installed in ~/Qt"
    echo "- Directory structure differs (e.g., not ~/Qt/X.Y.Z/gcc_64/lib)"
    echo "- Permission issues or Qt installed elsewhere"
    echo "Try running 'find ~ -type d -name lib 2>/dev/null' to locate Qt manually."
    exit 1
fi

