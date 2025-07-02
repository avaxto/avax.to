#!/usr/bin/env bash

cd "$(dirname "$0")"

# avax.to clean and rebuild script
date
echo 'Clean'
./clean.sh
echo 'DONE Clean'
echo 'Build'
./build.sh
echo 'DONE Build'
date