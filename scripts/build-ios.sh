#!/usr/bin/env bash
set -e
BASEDIR=$(dirname "$0")
BUILD_DIR=$BASEDIR/../build-ios
mkdir -p "$BUILD_DIR"
pushd "$BUILD_DIR"
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_INSTALL_PREFIX=`pwd`/_install -DPP_FORCE_IOS=1
cmake --build . --target install
popd