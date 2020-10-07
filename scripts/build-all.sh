#!/usr/bin/env bash
set -e
BASEDIR=$(dirname "$0")
"$BASEDIR"/build-emcc.sh
"$BASEDIR"/build-ios.sh