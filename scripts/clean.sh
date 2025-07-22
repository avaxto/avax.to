#!/usr/bin/env bash

# avax.to clean script

cd "$(dirname "$0")/.."
rm -rf ./build/
rm -rf supportlibs/librnp/Build/
rm -rf supportlibs/udp-discovery-cpp/Makefile
rm -rf supportlibs/udp-discovery-cpp/CMakeCache.txt
