#!/usr/bin/env bash
set -e
BASEDIR=$(dirname "$0")
BUILD_DIR=$BASEDIR/../build-emcc
mkdir -p "$BUILD_DIR"
pushd "$BUILD_DIR"
cmake .. -DCMAKE_BUILD_TYPE=Debug -DPP_FORCE_EMSCRIPTEN=1 -DCMAKE_TOOLCHAIN_FILE=~/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
cmake --build .
popd