#!/bin/bash

# Downloads and unzips the given version of WASM3
WASM3_VERSION="0.5.0"
BASEDIR=$(dirname "$0")
TEMP_DIR="${BASEDIR}/temp"
WASM3_SOURCES_DIR="${BASEDIR}/wasm3"

# make temporary download directory
rm -rf "$TEMP_DIR"
mkdir -p "$TEMP_DIR"

WASM3_DOWNLOAD_PATH="$TEMP_DIR/wasm3-${WASM3_VERSION}.zip"
curl --request GET -sL \
     --url "https://github.com/wasm3/wasm3/archive/refs/tags/v${WASM3_VERSION}.zip" \
     --output "$WASM3_DOWNLOAD_PATH"

rm -rf "$WASM3_SOURCES_DIR"
unzip "$WASM3_DOWNLOAD_PATH"
mv "wasm3-$WASM3_VERSION" "$WASM3_SOURCES_DIR"

# cleanup
rm -rf "$TEMP_DIR"

#gcc -O3 -g0 -s  -Dd_m3HasWASI source/*.c platforms/app/main.c -lm -o wasm3
#gcc -o libKWasm3.so -lm -shared \
#  -I"${JAVA_HOME}include" \
#  -I"${JAVA_HOME}include/darwin" \
#  -I"wasm3/source" \
#  -I"src/main/cpp" \
#  wasm3/source/*.c \
#  src/main/cpp/KWasm3.c
