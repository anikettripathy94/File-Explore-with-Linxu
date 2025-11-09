#!/usr/bin/env bash
set -e
mkdir -p build
g++ -std=c++17 -Wall -Wextra main.cpp -o build/explorer
echo "Build complete -> build/explorer"
